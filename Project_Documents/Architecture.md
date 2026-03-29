# System Architecture
## Self-Learning Smart Compiler

**Version:** 1.0.0  
**Date:** March 2026

---

## 1. Architecture Overview

The Smart Compiler is built as a cloud-native, microservices-based system deployed on AWS. It separates concerns into five primary service domains: Web Frontend, API Gateway, Core Services, AI/ML Pipeline, and Data Layer.

```
┌──────────────────────────────────────────────────────────────────────┐
│                          BROWSER (Client)                            │
│            React SPA + Monaco Editor + WASM Runtime                  │
└───────────────────────────┬──────────────────────────────────────────┘
                            │ HTTPS / WSS
                            ▼
┌──────────────────────────────────────────────────────────────────────┐
│                       AWS CloudFront CDN                             │
│                  (Static assets + API edge caching)                  │
└────────────────┬────────────────────────────────┬────────────────────┘
                 │                                │
        REST API │                      WebSocket │
                 ▼                                ▼
┌────────────────────────┐           ┌────────────────────────┐
│   API Gateway (NestJS) │           │   WS Gateway (NestJS)  │
│   Auth0 JWT Validation │           │   Socket.io            │
└──────────┬─────────────┘           └──────────┬─────────────┘
           │                                    │
           ▼                                    ▼
┌──────────────────────────────────────────────────────────────────────┐
│                        Core Services Layer                           │
│                                                                      │
│  ┌──────────────┐  ┌────────────────┐  ┌──────────────────────────┐ │
│  │ Project Svc  │  │ Compilation Svc│  │  Suggestion / AI Svc     │ │
│  │ (CRUD, S3)   │  │ (BullMQ jobs)  │  │  (ML inference calls)    │ │
│  └──────────────┘  └────────┬───────┘  └──────────────────────────┘ │
│                             │                                        │
└─────────────────────────────┼────────────────────────────────────────┘
                              │
                              ▼
┌──────────────────────────────────────────────────────────────────────┐
│                      Compiler Infrastructure                         │
│                                                                      │
│  ┌──────────────────────────────────────────────────────────────┐   │
│  │  Clang/LLVM Pipeline                                         │   │
│  │  Pre-process → AST → AI Analysis → IR → AI Opt → CodeGen    │   │
│  └──────────────────────────────────────────────────────────────┘   │
│                                                                      │
│  ┌──────────────────────────────────────────────────────────────┐   │
│  │  AI/ML Inference Service (vLLM / TorchServe)                 │   │
│  │  Code LLM + User Personalization Layer                       │   │
│  └──────────────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌──────────────────────────────────────────────────────────────────────┐
│                          Data Layer                                  │
│   PostgreSQL │ Redis │ Pinecone │ S3 │ Elasticsearch                │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 2. Service Descriptions

### 2.1 Frontend (React SPA)
- Served from S3 via CloudFront
- Monaco Editor for code editing
- WASM module for running compiled output in-browser
- Communicates via REST and WebSocket

### 2.2 API Gateway Service
- NestJS application handling all REST endpoints
- Auth0 JWT validation on every request
- Rate limiting, request logging, and input validation
- Routes requests to appropriate core services

### 2.3 WebSocket Gateway
- Real-time event streaming for compilation progress
- Per-connection authentication via query-param token
- Bridges compilation job events from BullMQ to client

### 2.4 Project Service
- Manages project and file CRUD
- Handles S3 uploads/downloads for source files
- Maintains project metadata in PostgreSQL

### 2.5 Compilation Service
- Receives compile requests, creates BullMQ jobs
- Workers run Clang/LLVM in isolated containers (Kubernetes pods)
- Each compilation is sandboxed with strict CPU/memory limits
- Emits progress events via Redis pub/sub

### 2.6 AI/ML Inference Service
- Receives code ASTs and context from the Compilation Service
- Calls the Code LLM to generate suggestions and optimizations
- Applies user personalization layer (federated model delta)
- Returns structured suggestion objects

### 2.7 User Learning Service
- Collects accept/reject signals from Suggestion events
- Periodically triggers fine-tuning jobs (nightly batch)
- Updates user learning profiles in PostgreSQL
- For enterprise: aggregates team-level model updates

---

## 3. Compilation Pipeline (Detailed)

```
1. User clicks "Compile" in IDE
        │
2. POST /compile → API Gateway → Compilation Service
        │
3. BullMQ job created (Redis queue)
        │
4. Worker pod picks up job (Kubernetes)
        │
5. Source files fetched from S3
        │
6. Clang Pre-processor runs
        │
7. AST generated via libclang
        │
8. AST + metadata sent to AI Inference Service
        │
9. AI returns:
   - List of optimization passes to apply
   - Suggested code transformations
   - Risk/confidence scores
        │
10. LLVM IR generated with AI-selected passes
        │
11. Code generation → WASM binary (for browser) or native binary
        │
12. Binary uploaded to S3
        │
13. Suggestions stored in PostgreSQL
        │
14. WebSocket events sent to client
        │
15. Client downloads binary + displays suggestions
```

---

## 4. Self-Learning Architecture

```
User accepts/rejects suggestion
          │
          ▼
Learning Event emitted → Kafka topic: learning.feedback
          │
          ▼
Learning Consumer Service
          │
    ┌─────┴────────────┐
    │                  │
    ▼                  ▼
User profile      Team model
updated (PG)      accumulates
                  feedback
          │
          ▼
Nightly fine-tuning job (MLflow)
          │
          ▼
New model checkpoint saved (S3)
          │
          ▼
Model registry updated (MLflow)
          │
          ▼
AI Inference Service hot-swaps model
          │
          ▼
User notified: "Your AI model improved"
```

---

## 5. Infrastructure Diagram

```
                        Route 53 (DNS)
                              │
                        CloudFront CDN
                         /          \
                   S3 (Static)    API Gateway (EKS)
                                       │
                         ┌─────────────┼──────────────┐
                         │             │              │
                    API Service    WS Service    Worker Pods
                    (NestJS)      (Socket.io)   (Clang/AI)
                         │
                    ┌────┴────┐
                 PostgreSQL  Redis
                    (RDS)   (ElastiCache)
                              │
                         Pinecone API
```

---

## 6. Scalability Strategy

| Component | Scaling Approach |
|-----------|-----------------|
| API Gateway | Horizontal pod autoscaler (HPA) in Kubernetes |
| WebSocket Gateway | Sticky sessions + Redis pub/sub for multi-instance |
| Compilation Workers | Queue-depth-based autoscaling (KEDA) |
| AI Inference | GPU node pool autoscaling on AWS (g5 instances) |
| PostgreSQL | Read replicas for reporting/audit queries |
| Redis | AWS ElastiCache cluster mode |

---

## 7. Disaster Recovery

| Metric | Target |
|--------|--------|
| RTO (Recovery Time Objective) | < 1 hour |
| RPO (Recovery Point Objective) | < 15 minutes |
| Database backup frequency | Every 5 minutes (WAL streaming) |
| Cross-region replication | Enabled (us-east-1 primary, eu-west-1 DR) |
| S3 versioning | Enabled on all project buckets |

---

## 8. Deployment Environments

| Environment | Purpose | AWS Region |
|-------------|---------|------------|
| Development | Engineer local testing | — (local Docker) |
| Staging | QA and integration testing | us-east-1 |
| Production | Live users | us-east-1 + eu-west-1 |
