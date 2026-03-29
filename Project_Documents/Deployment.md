# Deployment Guide
## Self-Learning Smart Compiler

**Version:** 1.0.0  
**Date:** March 2026

---

## 1. Overview

The Smart Compiler is deployed on **AWS** using **Kubernetes (EKS)**, **Terraform** for infrastructure-as-code, and **GitHub Actions** for CI/CD. All environments are reproducible from code.

---

## 2. Environments

| Environment | Purpose | Domain | AWS Region |
|-------------|---------|--------|------------|
| `local` | Developer machines | localhost:5173 | — |
| `staging` | QA + integration testing | staging.smartcompiler.dev | us-east-1 |
| `production` | Live users | app.smartcompiler.dev | us-east-1 + eu-west-1 |

---

## 3. Prerequisites

Ensure the following tools are installed and configured:

```bash
# Required CLI tools
aws --version          # AWS CLI v2
kubectl version        # Kubernetes CLI
helm version           # Helm v3
terraform --version    # Terraform >= 1.5
docker --version       # Docker >= 24
node --version         # Node.js >= 20 LTS
```

AWS credentials must have permission to manage EKS, RDS, ElastiCache, S3, and CloudFront.

---

## 4. Infrastructure Provisioning (Terraform)

Infrastructure is defined in `infrastructure/terraform/`.

### 4.1 Initialize

```bash
cd infrastructure/terraform
terraform init
terraform workspace select staging   # or 'production'
```

### 4.2 Plan

```bash
terraform plan -var-file="envs/staging.tfvars" -out=plan.tfplan
```

### 4.3 Apply

```bash
terraform apply plan.tfplan
```

### 4.4 Core Resources Provisioned

```
modules/
├── vpc/              ← VPC, subnets, security groups
├── eks/              ← EKS cluster, node groups
├── rds/              ← PostgreSQL (RDS)
├── elasticache/      ← Redis cluster
├── s3/               ← Project files, model weights
├── cloudfront/       ← CDN distribution
├── route53/          ← DNS records
├── kms/              ← Encryption keys
├── waf/              ← WAF rules
└── iam/              ← Service roles and policies
```

---

## 5. CI/CD Pipeline (GitHub Actions)

### 5.1 Pipeline Overview

```
Pull Request opened
        │
        ▼
[ci.yml] Lint + Unit Tests + SAST
        │
    Tests pass
        │
        ▼
[ci.yml] Build Docker images
        │
        ▼
[ci.yml] Push to ECR (staging tag)
        │
        ▼
Merge to main
        │
        ▼
[deploy-staging.yml] Deploy to staging
        │
        ▼
[smoke-tests.yml] Run smoke tests on staging
        │
        ▼
Manual approval gate (GitHub Environments)
        │
        ▼
[deploy-production.yml] Deploy to production
        │
        ▼
[smoke-tests.yml] Run smoke tests on production
        │
        ▼
[notify.yml] Slack notification
```

### 5.2 Workflow Files

| File | Trigger | Purpose |
|------|---------|---------|
| `ci.yml` | PR, push to main | Lint, test, build, scan |
| `deploy-staging.yml` | Merge to main | Deploy to staging |
| `deploy-production.yml` | Manual approval | Deploy to production |
| `smoke-tests.yml` | Post-deploy | Validate endpoints |
| `db-migrate.yml` | Post-deploy | Run Flyway migrations |
| `model-deploy.yml` | Model release tag | Deploy new AI model |

---

## 6. Docker Images

Each service has its own Dockerfile. Images are stored in **AWS ECR**.

### 6.1 API Service

```dockerfile
FROM node:20-alpine AS builder
WORKDIR /app
COPY package*.json ./
RUN npm ci
COPY . .
RUN npm run build

FROM node:20-alpine
WORKDIR /app
COPY --from=builder /app/dist ./dist
COPY --from=builder /app/node_modules ./node_modules
USER node
EXPOSE 3000
CMD ["node", "dist/main.js"]
```

### 6.2 Compilation Worker

```dockerfile
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y clang-17 llvm-17 cmake
COPY worker/ /worker/
WORKDIR /worker
USER nobody
CMD ["./run-worker.sh"]
```

### 6.3 Frontend (Served via S3 + CloudFront)

```bash
# Build static assets
npm run build

# Upload to S3
aws s3 sync dist/ s3://smartcompiler-frontend-${ENV}/ --delete

# Invalidate CloudFront cache
aws cloudfront create-invalidation \
  --distribution-id ${CF_DIST_ID} \
  --paths "/*"
```

---

## 7. Kubernetes Deployment

### 7.1 Namespace Setup

```bash
kubectl create namespace smart-compiler
kubectl config set-context --current --namespace=smart-compiler
```

### 7.2 Secrets

Secrets are pulled from **AWS Secrets Manager** via the External Secrets Operator:

```yaml
apiVersion: external-secrets.io/v1beta1
kind: ExternalSecret
metadata:
  name: smart-compiler-secrets
spec:
  refreshInterval: 1h
  secretStoreRef:
    name: aws-secrets-manager
    kind: ClusterSecretStore
  target:
    name: smart-compiler-secrets
  data:
    - secretKey: DB_PASSWORD
      remoteRef:
        key: /smartcompiler/prod/db-password
```

### 7.3 Deploy Services

```bash
# Deploy via Helm
helm upgrade --install smart-compiler ./charts/smart-compiler \
  --namespace smart-compiler \
  --values charts/smart-compiler/values-production.yaml \
  --set image.tag=${GIT_SHA}
```

### 7.4 Key Helm Values

```yaml
# values-production.yaml
replicaCount:
  api: 3
  ws: 2
  worker: 5

resources:
  api:
    limits: { cpu: "1", memory: "512Mi" }
    requests: { cpu: "250m", memory: "256Mi" }
  worker:
    limits: { cpu: "2", memory: "1Gi" }
    requests: { cpu: "500m", memory: "512Mi" }

autoscaling:
  enabled: true
  minReplicas: 2
  maxReplicas: 20
  targetCPUUtilizationPercentage: 70
```

---

## 8. Database Migrations

Migrations run automatically after each deploy via a Kubernetes Job:

```bash
# Manual migration trigger
kubectl create job --from=cronjob/db-migrate db-migrate-manual \
  -n smart-compiler
```

Migration files: `db/migrations/V*.sql`
Migration tool: **Flyway**

---

## 9. AI Model Deployment

Model deployments are separate from application deployments and are versioned independently.

```bash
# Tag a model release
git tag model-v1.4.2
git push origin model-v1.4.2

# GitHub Actions triggers model-deploy.yml:
# 1. Downloads model weights from MLflow registry
# 2. Packages into TorchServe model archive (.mar)
# 3. Uploads to S3 model bucket
# 4. Updates Kubernetes ConfigMap with new model version
# 5. Rolls out AI Inference Deployment (zero-downtime)
```

---

## 10. Health Checks & Monitoring

### 10.1 Health Endpoints

| Service | Endpoint | Expected Response |
|---------|----------|-------------------|
| API | `GET /health` | `{ "status": "ok" }` |
| WS Gateway | `GET /health` | `{ "status": "ok" }` |
| AI Inference | `GET /ping` | `200 OK` |

### 10.2 Monitoring Stack

| Tool | Purpose |
|------|---------|
| Datadog APM | Application performance monitoring |
| Datadog Logs | Centralized log aggregation |
| AWS CloudWatch | AWS resource metrics |
| PagerDuty | On-call alerting |

### 10.3 Key Alerts

| Alert | Threshold | Severity |
|-------|-----------|----------|
| API error rate | > 1% | P1 |
| Compilation queue depth | > 100 jobs | P2 |
| AI inference latency (p95) | > 10s | P2 |
| DB connections > 80% | Utilization | P1 |
| Pod OOMKilled | Any instance | P1 |

---

## 11. Rollback Procedure

### Application Rollback

```bash
# Roll back to previous Helm release
helm rollback smart-compiler 0 -n smart-compiler
```

### Database Rollback

Flyway does not auto-rollback. For schema rollback:

```bash
# Create a new migration that undoes the change
# e.g., V8__rollback_add_column.sql
kubectl create job --from=cronjob/db-migrate db-rollback -n smart-compiler
```

### AI Model Rollback

```bash
# Update ConfigMap to previous model version
kubectl patch configmap model-config \
  -p '{"data":{"MODEL_VERSION":"v1.3.1"}}' \
  -n smart-compiler

# Redeploy inference pods
kubectl rollout restart deployment/ai-inference -n smart-compiler
```

---

## 12. Local Development Setup

```bash
# Clone repository
git clone https://github.com/smartcompiler/platform.git
cd platform

# Install dependencies
npm install

# Start local services (Docker Compose)
docker compose up -d postgres redis

# Copy environment file
cp .env.example .env.local

# Run database migrations
npm run db:migrate

# Start all services
npm run dev
```

Services will be available at:
- Frontend: http://localhost:5173
- API: http://localhost:3000
- API Docs (Swagger): http://localhost:3000/docs
