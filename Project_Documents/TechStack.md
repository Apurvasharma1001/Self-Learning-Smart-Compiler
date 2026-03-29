# Technology Stack
## Self-Learning Smart Compiler

**Version:** 1.0.0  
**Date:** March 2026

---

## 1. Overview

The Smart Compiler is a cloud-native, browser-based application built with a modern full-stack architecture. The stack is selected for performance, scalability, AI/ML integration, and developer experience.

---

## 2. Frontend

| Layer | Technology | Rationale |
|-------|-----------|-----------|
| Framework | **React 18** + TypeScript | Component model, strong typing, ecosystem |
| Code Editor | **Monaco Editor** | VS Code engine; best-in-class syntax support |
| State Management | **Zustand** | Lightweight, performant global state |
| Styling | **Tailwind CSS** + CSS Modules | Rapid development + component scoping |
| Build Tool | **Vite** | Fast HMR, optimized builds |
| Component Library | **Radix UI** (headless) | Accessible primitives, custom-styled |
| Diff Viewer | **react-diff-viewer-continued** | Code diff rendering for AI suggestions |
| WebAssembly Runtime | **WASM (Emscripten)** | Run compiled C/C++ output in the browser |
| HTTP Client | **Axios** | API communication with interceptors |
| WebSocket | **Socket.io-client** | Real-time compilation status |

---

## 3. Backend

| Layer | Technology | Rationale |
|-------|-----------|-----------|
| Runtime | **Node.js 20 LTS** | Async I/O, large ecosystem |
| Framework | **NestJS** | Modular, opinionated, TypeScript-native |
| API Style | **REST + WebSocket** | REST for CRUD; WS for live compile events |
| Compiler Service | **Clang/LLVM 17** | Industry-standard C/C++ compiler with rich AST APIs |
| AST Analysis | **libclang / clang-tidy** | Static analysis and AST traversal |
| Task Queue | **BullMQ** (Redis-backed) | Async compilation jobs |
| Authentication | **Auth0** (JWT + OAuth2) | SSO, SAML, MFA support |
| File Storage | **AWS S3** | Project source files, build artifacts |

---

## 4. AI / ML Layer

| Component | Technology | Rationale |
|-----------|-----------|-----------|
| Base Model | **Code LLM (fine-tuned)** | Pre-trained on C/C++ open-source corpus |
| Fine-Tuning Framework | **Hugging Face Transformers** | Industry standard for LLM fine-tuning |
| Inference Server | **TorchServe / vLLM** | High-throughput model serving |
| Optimization Engine | **LLVM Pass Pipeline** (AI-guided) | LLVM passes controlled by ML policy |
| User Model Personalization | **Federated Learning layer** | Per-user adaptation without raw data leaving server |
| Embedding Storage | **Pinecone** | Vector DB for code pattern similarity search |
| Feature Extraction | **Tree-sitter** | Fast, incremental syntax tree parsing |
| Experiment Tracking | **MLflow** | Model versioning and A/B testing |

---

## 5. Database

| Purpose | Technology |
|---------|-----------|
| Primary (relational) | **PostgreSQL 15** |
| Cache | **Redis 7** |
| Vector store | **Pinecone** |
| Object storage | **AWS S3** |
| Search | **Elasticsearch** (audit log search) |

> See `Database.md` for full schema details.

---

## 6. Infrastructure & DevOps

| Component | Technology |
|-----------|-----------|
| Cloud Provider | **AWS** |
| Containerization | **Docker** |
| Orchestration | **Kubernetes (EKS)** |
| CI/CD | **GitHub Actions** |
| IaC | **Terraform** |
| CDN | **CloudFront** |
| DNS | **Route 53** |
| Secrets Management | **AWS Secrets Manager** |
| Monitoring | **Datadog** |
| Logging | **AWS CloudWatch** + **Loki** |
| Alerting | **PagerDuty** |

---

## 7. Compiler Infrastructure Detail

The C/C++ compilation pipeline is built on top of LLVM/Clang with custom AI-driven passes:

```
Source Code (C/C++)
       │
       ▼
[Pre-processing & Tokenization]  ← Tree-sitter
       │
       ▼
[AST Generation]  ← Clang Frontend
       │
       ▼
[AI Static Analysis Pass]  ← ML Model (code patterns)
       │
       ▼
[IR Generation]  ← LLVM IR
       │
       ▼
[AI Optimization Pass]  ← ML-guided LLVM pass selection
       │
       ▼
[Code Generation]  ← LLVM Backend
       │
       ▼
[WebAssembly Output / Binary]
```

---

## 8. Security Technologies

| Area | Technology |
|------|-----------|
| Auth | Auth0 (JWT, OAuth2, SAML) |
| Encryption at rest | AES-256 (AWS KMS) |
| Encryption in transit | TLS 1.3 |
| Container security | Snyk + Trivy |
| SAST | SonarQube |
| DDoS protection | AWS Shield Standard |
| WAF | AWS WAF |

> See `Security.md` for full security specification.

---

## 9. Developer Tooling

| Tool | Purpose |
|------|---------|
| ESLint + Prettier | Code formatting and linting |
| Husky | Pre-commit hooks |
| Jest + React Testing Library | Unit and component tests |
| Playwright | End-to-end testing |
| Storybook | Component development and documentation |
| OpenAPI / Swagger | API documentation generation |

---

## 10. Third-Party Services

| Service | Purpose |
|---------|---------|
| Auth0 | Identity and access management |
| AWS (S3, EKS, CloudFront, KMS) | Cloud infrastructure |
| Pinecone | Vector similarity search |
| MLflow | ML experiment tracking |
| Datadog | Observability and APM |
| PagerDuty | On-call alerting |
| SendGrid | Transactional email |
| Stripe | Subscription billing |
