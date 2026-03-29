# Database Design
## Self-Learning Smart Compiler

**Version:** 1.0.0  
**Date:** March 2026

---

## 1. Overview

The Smart Compiler uses a multi-database architecture:

| Database | Engine | Purpose |
|----------|--------|---------|
| Primary DB | PostgreSQL 15 | Users, projects, sessions, audit logs |
| Cache | Redis 7 | Sessions, job queues, temporary state |
| Vector Store | Pinecone | Code embeddings for AI pattern matching |
| Object Store | AWS S3 | Source files, compiled artifacts, model weights |
| Search Index | Elasticsearch | Full-text search on audit logs and errors |

---

## 2. PostgreSQL Schema

### 2.1 Users Table

```sql
CREATE TABLE users (
    id              UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    email           VARCHAR(255) UNIQUE NOT NULL,
    display_name    VARCHAR(100),
    role            VARCHAR(20) NOT NULL DEFAULT 'developer',
                    -- Values: 'student', 'developer', 'lead', 'admin'
    org_id          UUID REFERENCES organizations(id) ON DELETE SET NULL,
    auth_provider   VARCHAR(50),      -- 'auth0', 'github', 'google'
    auth_subject    VARCHAR(255),     -- external identity subject
    preferences     JSONB DEFAULT '{}',
    created_at      TIMESTAMPTZ DEFAULT NOW(),
    updated_at      TIMESTAMPTZ DEFAULT NOW(),
    last_login_at   TIMESTAMPTZ
);

CREATE INDEX idx_users_email ON users(email);
CREATE INDEX idx_users_org_id ON users(org_id);
```

---

### 2.2 Organizations Table

```sql
CREATE TABLE organizations (
    id              UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name            VARCHAR(255) NOT NULL,
    slug            VARCHAR(100) UNIQUE NOT NULL,
    plan            VARCHAR(30) DEFAULT 'free',
                    -- Values: 'free', 'team', 'enterprise'
    settings        JSONB DEFAULT '{}',
    created_at      TIMESTAMPTZ DEFAULT NOW(),
    updated_at      TIMESTAMPTZ DEFAULT NOW()
);
```

---

### 2.3 Projects Table

```sql
CREATE TABLE projects (
    id              UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name            VARCHAR(255) NOT NULL,
    owner_id        UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    org_id          UUID REFERENCES organizations(id) ON DELETE SET NULL,
    language        VARCHAR(20) DEFAULT 'cpp',
    description     TEXT,
    s3_prefix       VARCHAR(500),     -- S3 path to project files
    settings        JSONB DEFAULT '{}',
    is_public       BOOLEAN DEFAULT FALSE,
    created_at      TIMESTAMPTZ DEFAULT NOW(),
    updated_at      TIMESTAMPTZ DEFAULT NOW()
);

CREATE INDEX idx_projects_owner_id ON projects(owner_id);
CREATE INDEX idx_projects_org_id ON projects(org_id);
```

---

### 2.4 Compilation Jobs Table

```sql
CREATE TABLE compilation_jobs (
    id              UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    project_id      UUID NOT NULL REFERENCES projects(id) ON DELETE CASCADE,
    user_id         UUID NOT NULL REFERENCES users(id),
    status          VARCHAR(20) DEFAULT 'queued',
                    -- Values: 'queued', 'running', 'success', 'failed', 'cancelled'
    entry_file      VARCHAR(500),
    optimization_level VARCHAR(10) DEFAULT 'ai',
                    -- Values: 'O0', 'O1', 'O2', 'O3', 'Os', 'ai'
    compiler_flags  TEXT[],
    started_at      TIMESTAMPTZ,
    completed_at    TIMESTAMPTZ,
    duration_ms     INTEGER,
    output_s3_key   VARCHAR(500),
    error_log       TEXT,
    ai_applied      BOOLEAN DEFAULT FALSE,
    created_at      TIMESTAMPTZ DEFAULT NOW()
);

CREATE INDEX idx_compilation_jobs_project_id ON compilation_jobs(project_id);
CREATE INDEX idx_compilation_jobs_status ON compilation_jobs(status);
CREATE INDEX idx_compilation_jobs_created_at ON compilation_jobs(created_at DESC);
```

---

### 2.5 AI Suggestions Table

```sql
CREATE TABLE ai_suggestions (
    id              UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    job_id          UUID NOT NULL REFERENCES compilation_jobs(id) ON DELETE CASCADE,
    user_id         UUID NOT NULL REFERENCES users(id),
    file_path       VARCHAR(500),
    line_start      INTEGER,
    line_end        INTEGER,
    category        VARCHAR(30),
                    -- Values: 'performance', 'safety', 'style', 'refactor'
    severity        VARCHAR(20),
                    -- Values: 'info', 'warning', 'critical'
    title           VARCHAR(255),
    description     TEXT,
    code_before     TEXT,
    code_after      TEXT,
    status          VARCHAR(20) DEFAULT 'pending',
                    -- Values: 'pending', 'accepted', 'rejected', 'auto_applied'
    model_version   VARCHAR(50),
    confidence      FLOAT,
    created_at      TIMESTAMPTZ DEFAULT NOW(),
    resolved_at     TIMESTAMPTZ
);

CREATE INDEX idx_ai_suggestions_job_id ON ai_suggestions(job_id);
CREATE INDEX idx_ai_suggestions_user_id ON ai_suggestions(user_id);
CREATE INDEX idx_ai_suggestions_status ON ai_suggestions(status);
```

---

### 2.6 User Learning Profiles Table

```sql
CREATE TABLE user_learning_profiles (
    id              UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id         UUID UNIQUE NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    model_version   VARCHAR(50),
    total_accepts   INTEGER DEFAULT 0,
    total_rejects   INTEGER DEFAULT 0,
    accept_rate     FLOAT DEFAULT 0.0,
    preferred_style JSONB DEFAULT '{}',
                    -- Stores learned preferences: naming, spacing, patterns
    last_trained_at TIMESTAMPTZ,
    created_at      TIMESTAMPTZ DEFAULT NOW(),
    updated_at      TIMESTAMPTZ DEFAULT NOW()
);
```

---

### 2.7 Team Learning Models Table

```sql
CREATE TABLE team_learning_models (
    id              UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    org_id          UUID NOT NULL REFERENCES organizations(id) ON DELETE CASCADE,
    model_version   VARCHAR(50),
    s3_model_key    VARCHAR(500),
    training_samples INTEGER DEFAULT 0,
    is_active       BOOLEAN DEFAULT TRUE,
    created_at      TIMESTAMPTZ DEFAULT NOW()
);

CREATE INDEX idx_team_learning_models_org_id ON team_learning_models(org_id);
```

---

### 2.8 Audit Logs Table

```sql
CREATE TABLE audit_logs (
    id              UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id         UUID REFERENCES users(id) ON DELETE SET NULL,
    org_id          UUID REFERENCES organizations(id) ON DELETE SET NULL,
    action          VARCHAR(100) NOT NULL,
                    -- e.g., 'suggestion.accepted', 'project.created', 'compile.triggered'
    resource_type   VARCHAR(50),
    resource_id     UUID,
    metadata        JSONB DEFAULT '{}',
    ip_address      INET,
    user_agent      TEXT,
    created_at      TIMESTAMPTZ DEFAULT NOW()
);

CREATE INDEX idx_audit_logs_user_id ON audit_logs(user_id);
CREATE INDEX idx_audit_logs_org_id ON audit_logs(org_id);
CREATE INDEX idx_audit_logs_created_at ON audit_logs(created_at DESC);
CREATE INDEX idx_audit_logs_action ON audit_logs(action);
```

---

## 3. Redis Schema

| Key Pattern | Type | TTL | Description |
|-------------|------|-----|-------------|
| `session:{userId}` | Hash | 24h | Active session data |
| `compile:job:{jobId}` | Hash | 1h | Live compilation job state |
| `compile:queue` | List | — | BullMQ job queue |
| `ratelimit:{userId}` | Counter | 1m | API rate limit counter |
| `ai:cache:{codeHash}` | String | 1h | Cached AI suggestions for identical code |
| `user:prefs:{userId}` | Hash | — | Cached user preferences |

---

## 4. Pinecone (Vector Store)

### Namespace: `code-patterns`

Each code snippet is embedded and stored with metadata for similarity search.

| Field | Type | Description |
|-------|------|-------------|
| `id` | String | UUID of the code snippet |
| `vector` | float[1536] | Code embedding from the AI model |
| `userId` | String | Owner of the snippet |
| `orgId` | String | Organization (optional) |
| `language` | String | Always `cpp` or `c` |
| `category` | String | Pattern category |
| `snippet` | String | Short preview of code |

---

## 5. S3 Bucket Structure

```
smart-compiler-prod/
├── projects/
│   └── {projectId}/
│       ├── src/          ← source files
│       └── build/        ← compiled artifacts
├── models/
│   ├── base/             ← base LLM weights
│   └── team/{orgId}/     ← team-specific fine-tuned models
├── exports/
│   └── {userId}/         ← user-exported archives
└── audit/
    └── {orgId}/          ← audit log exports (CSV/JSON)
```

---

## 6. Data Retention Policy

| Data Type | Retention Period |
|-----------|-----------------|
| Source code files | Until project deleted + 30 days |
| Compilation job logs | 90 days |
| AI suggestions | 180 days |
| Audit logs | 1 year (enterprise), 90 days (free) |
| User learning profiles | Until account deleted |
| Session data | 24 hours |

---

## 7. Migrations

Database migrations are managed with **Flyway**. All migration files are versioned and stored in `db/migrations/`.

```
V1__create_users.sql
V2__create_organizations.sql
V3__create_projects.sql
V4__create_compilation_jobs.sql
V5__create_ai_suggestions.sql
V6__create_learning_profiles.sql
V7__create_audit_logs.sql
```
