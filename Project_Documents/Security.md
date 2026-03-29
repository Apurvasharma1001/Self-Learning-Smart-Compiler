# Security Specification
## Self-Learning Smart Compiler

**Version:** 1.0.0  
**Date:** March 2026

---

## 1. Security Philosophy

The Smart Compiler handles sensitive source code — the intellectual property of individuals, students, and enterprises. Security is a first-class concern, not an afterthought. All design decisions prioritize:

- **Confidentiality:** Source code is never exposed to unauthorized parties.
- **Integrity:** AI-applied changes are traceable, auditable, and reversible.
- **Availability:** The platform maintains SLA uptime even under attack.
- **Least Privilege:** Every component has exactly the permissions it needs.

---

## 2. Authentication & Authorization

### 2.1 Identity Provider
All authentication is delegated to **Auth0**.

- OAuth 2.0 / OpenID Connect for web login
- SAML 2.0 for enterprise SSO
- Social logins: GitHub, Google
- MFA enforced for enterprise accounts (TOTP / WebAuthn)

### 2.2 Token Strategy
- **Access tokens:** Short-lived JWTs (1 hour expiry)
- **Refresh tokens:** Rotating refresh tokens (Auth0 managed)
- **Service-to-service:** Signed JWTs with separate service credentials

### 2.3 Role-Based Access Control (RBAC)

| Role | Permissions |
|------|-------------|
| Viewer | Read-only: view projects, suggestions |
| Developer | Compile, edit files, accept/reject suggestions |
| Lead | All developer permissions + manage team settings |
| Admin | All permissions + user management, audit log access |

Authorization checks are performed at the API Gateway level before any service logic executes.

---

## 3. Data Encryption

### 3.1 Encryption at Rest
- All PostgreSQL data encrypted with **AES-256** (AWS RDS encryption via KMS)
- All S3 objects (source code, model weights, artifacts) encrypted with **SSE-S3** (AES-256)
- Redis data encrypted with **AES-256** (ElastiCache in-transit + at-rest encryption)
- Encryption keys managed by **AWS KMS** with automatic rotation every 90 days

### 3.2 Encryption in Transit
- All HTTP traffic enforced over **TLS 1.3**
- WebSocket connections over **WSS (TLS 1.3)**
- Internal service-to-service communication over **mTLS**
- TLS certificates issued via **AWS Certificate Manager (ACM)**

### 3.3 Source Code Handling
Source code uploaded by users is treated as confidential:

- Stored in isolated S3 paths per user/org
- Never logged in plaintext anywhere
- Purged from compilation worker memory immediately after job completes
- AI models are trained on code representations (embeddings), not raw code

---

## 4. Code Sandbox Security

Compilation jobs run in isolated containers. This is critical because user-submitted C/C++ code is compiled and potentially executed in the system.

### 4.1 Isolation Model
- Each compilation job runs in a dedicated **Kubernetes pod**
- Pods use **gVisor (runsc)** container runtime for syscall sandboxing
- Pods have **no network access** (compilation is fully offline)
- CPU limited to 2 cores, memory limited to 512 MB per job
- Pod is destroyed immediately after job completion

### 4.2 WASM Execution Security
User-compiled WASM runs in the browser, not on the server:

- WASM executes in the browser's built-in sandbox
- No filesystem or network access from WASM
- Memory isolated within the WASM linear memory model

---

## 5. API Security

### 5.1 Input Validation
All API inputs validated using **class-validator** (NestJS) and JSON Schema:

- File paths sanitized (directory traversal prevention)
- Compiler flags allowlisted (only safe flags permitted)
- File size limits enforced (max 5 MB per file, max 100 files per project)
- Max compilation payload: 50 MB per job

### 5.2 Rate Limiting
Rate limits applied per user per endpoint:

| Endpoint | Free | Team | Enterprise |
|----------|------|------|------------|
| `POST /compile` | 20/hr | 100/hr | Unlimited |
| `PUT /files/:path` | 120/min | 600/min | Unlimited |
| `GET /suggestions` | 60/min | 300/min | Unlimited |

### 5.3 CORS Policy
CORS restricted to:

- `https://app.smartcompiler.dev` (production)
- `https://staging.smartcompiler.dev` (staging)
- `http://localhost:5173` (development only)

### 5.4 Security Headers
All responses include:

```
Strict-Transport-Security: max-age=31536000; includeSubDomains
X-Content-Type-Options: nosniff
X-Frame-Options: DENY
Content-Security-Policy: default-src 'self'; ...
Referrer-Policy: strict-origin-when-cross-origin
Permissions-Policy: camera=(), microphone=()
```

---

## 6. Infrastructure Security

### 6.1 Network Architecture
- All services run inside an **AWS VPC** with private subnets
- Only the API Gateway and CloudFront are publicly accessible
- Databases are in private subnets with no public IP
- Security groups enforce least-privilege inbound rules

### 6.2 DDoS & WAF Protection
- **AWS Shield Standard** for L3/L4 DDoS protection
- **AWS WAF** rules for:
  - SQL injection prevention
  - Cross-site scripting (XSS) prevention
  - Rate-based rules for IP throttling
  - Managed rule groups (OWASP Top 10)

### 6.3 Secrets Management
- All secrets (DB passwords, API keys, model credentials) stored in **AWS Secrets Manager**
- Secrets rotated automatically
- No secrets in environment variables or source code
- GitHub Actions uses OIDC role assumption (no long-lived credentials)

### 6.4 Container Security
- Base images scanned with **Trivy** on every build
- Dependencies scanned with **Snyk** (node_modules + pip)
- Images built on minimal base (distroless where possible)
- No root processes in containers

---

## 7. Application Security Testing

| Type | Tool | Frequency |
|------|------|-----------|
| SAST | SonarQube | Every PR |
| Dependency scan | Snyk | Daily |
| Container scan | Trivy | Every build |
| DAST | OWASP ZAP | Weekly (staging) |
| Penetration test | External firm | Annually |
| Bug bounty | HackerOne program | Continuous |

---

## 8. Source Code Privacy

Given the sensitivity of user-submitted C/C++ code:

- Anthropic employees and platform operators **cannot** read user source code
- Source code is only accessed by compilation workers and AI inference, with no persistent logging of raw content
- AI models learn from **embeddings** (vector representations), not raw code
- Users can delete all their data at any time (GDPR right to erasure)
- Enterprise users can opt out of federated learning entirely

---

## 9. Compliance

| Standard | Status |
|----------|--------|
| GDPR | Compliant — data residency options for EU users |
| SOC 2 Type II | Target: Year 1 |
| ISO 27001 | Target: Year 2 |
| CCPA | Compliant |

---

## 10. Incident Response

### Severity Levels

| Level | Description | Response Time |
|-------|-------------|---------------|
| P0 | Data breach or service down | 15 minutes |
| P1 | Security vulnerability exploited | 1 hour |
| P2 | Potential vulnerability found | 4 hours |
| P3 | Low-risk finding | 48 hours |

### Incident Process
1. Alert triggered (Datadog / PagerDuty)
2. On-call engineer acknowledges within SLA
3. Incident channel created in Slack
4. Root cause analysis within 24h of resolution
5. Post-mortem published within 72h

---

## 11. Security Contacts

- **Security team email:** security@smartcompiler.dev
- **Responsible disclosure:** https://smartcompiler.dev/security
- **Bug bounty:** https://hackerone.com/smartcompiler
