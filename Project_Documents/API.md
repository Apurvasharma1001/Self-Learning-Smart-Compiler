# API Reference
## Self-Learning Smart Compiler

**Version:** 1.0.0  
**Base URL:** `https://api.smartcompiler.dev/v1`  
**Date:** March 2026

---

## 1. Overview

All API endpoints follow REST conventions and return JSON. WebSocket is used for real-time compilation status streaming.

### Authentication

Every request must include a Bearer token:

```
Authorization: Bearer <jwt_token>
```

Tokens are issued by Auth0 via OAuth2 / SAML flows.

### Response Format

```json
{
  "success": true,
  "data": { ... },
  "meta": {
    "timestamp": "2026-03-23T10:00:00Z",
    "requestId": "req_abc123"
  }
}
```

Error response:

```json
{
  "success": false,
  "error": {
    "code": "VALIDATION_ERROR",
    "message": "File path is required",
    "details": [ ... ]
  }
}
```

### Rate Limits

| Plan | Requests/min | Compilations/hour |
|------|-------------|-------------------|
| Free | 60 | 20 |
| Team | 300 | 100 |
| Enterprise | Unlimited | Unlimited |

---

## 2. Auth Endpoints

### `POST /auth/token`
Exchange Auth0 code for a JWT access token.

**Request:**
```json
{
  "code": "auth0_code",
  "redirect_uri": "https://app.smartcompiler.dev/callback"
}
```

**Response:**
```json
{
  "success": true,
  "data": {
    "access_token": "eyJ...",
    "expires_in": 86400,
    "token_type": "Bearer"
  }
}
```

---

### `POST /auth/logout`
Revoke the current session token.

**Response:** `204 No Content`

---

## 3. User Endpoints

### `GET /users/me`
Returns the authenticated user's profile.

**Response:**
```json
{
  "id": "uuid",
  "email": "dev@example.com",
  "display_name": "Arjun Sharma",
  "role": "developer",
  "org_id": "uuid",
  "preferences": {
    "theme": "dark",
    "learning_mode": false
  },
  "created_at": "2026-01-01T00:00:00Z"
}
```

---

### `PATCH /users/me`
Update user preferences.

**Request:**
```json
{
  "display_name": "Arjun S.",
  "preferences": {
    "theme": "light",
    "learning_mode": true
  }
}
```

---

## 4. Project Endpoints

### `GET /projects`
List all projects for the authenticated user.

**Query Params:**
| Param | Type | Description |
|-------|------|-------------|
| `org_id` | string | Filter by organization |
| `page` | int | Page number (default: 1) |
| `limit` | int | Items per page (default: 20, max: 100) |

---

### `POST /projects`
Create a new project.

**Request:**
```json
{
  "name": "MyEmbeddedApp",
  "language": "cpp",
  "description": "RTOS scheduler in C++",
  "is_public": false
}
```

**Response:** `201 Created` with project object.

---

### `GET /projects/:id`
Get a project by ID.

---

### `PATCH /projects/:id`
Update project metadata or settings.

---

### `DELETE /projects/:id`
Soft-delete a project. Files retained for 30 days.

**Response:** `204 No Content`

---

## 5. File Endpoints

### `GET /projects/:id/files`
List all files in a project.

**Response:**
```json
{
  "files": [
    { "path": "src/main.cpp", "size": 4096, "modified_at": "..." },
    { "path": "src/scheduler.h", "size": 1024, "modified_at": "..." }
  ]
}
```

---

### `GET /projects/:id/files/:path`
Get the content of a specific file.

**Response:**
```json
{
  "path": "src/main.cpp",
  "content": "#include <iostream>\n...",
  "language": "cpp"
}
```

---

### `PUT /projects/:id/files/:path`
Create or update a file's content.

**Request:**
```json
{
  "content": "#include <iostream>\nint main() { return 0; }"
}
```

---

### `DELETE /projects/:id/files/:path`
Delete a file from the project.

---

## 6. Compilation Endpoints

### `POST /compile`
Trigger a compilation job.

**Request:**
```json
{
  "project_id": "uuid",
  "entry_file": "src/main.cpp",
  "optimization_level": "ai",
  "compiler_flags": ["-std=c++20", "-Wall"]
}
```

**Response:** `202 Accepted`
```json
{
  "job_id": "uuid",
  "status": "queued",
  "estimated_duration_ms": 3200
}
```

---

### `GET /compile/:jobId`
Get the status and result of a compilation job.

**Response:**
```json
{
  "job_id": "uuid",
  "status": "success",
  "duration_ms": 2850,
  "output_url": "https://cdn.smartcompiler.dev/builds/uuid.wasm",
  "ai_applied": true,
  "suggestions_count": 7
}
```

---

### `GET /compile/:jobId/output`
Stream raw compiler output (WASM binary or native binary).

**Response:** Binary stream with `Content-Type: application/wasm`

---

## 7. AI Suggestions Endpoints

### `GET /compile/:jobId/suggestions`
Get all AI suggestions generated for a compilation job.

**Query Params:**
| Param | Type | Description |
|-------|------|-------------|
| `category` | string | Filter: `performance`, `safety`, `style` |
| `status` | string | Filter: `pending`, `accepted`, `rejected` |

**Response:**
```json
{
  "suggestions": [
    {
      "id": "uuid",
      "category": "safety",
      "severity": "warning",
      "title": "Replace raw pointer with unique_ptr",
      "description": "Raw pointer ownership is ambiguous...",
      "file_path": "src/main.cpp",
      "line_start": 42,
      "line_end": 44,
      "code_before": "int* ptr = new int(5);",
      "code_after": "auto ptr = std::make_unique<int>(5);",
      "confidence": 0.94,
      "status": "pending"
    }
  ]
}
```

---

### `POST /suggestions/:id/accept`
Accept and apply a suggestion to the project file.

**Response:** `200 OK` with updated file path.

---

### `POST /suggestions/:id/reject`
Reject a suggestion. Feedback is used to train the user model.

**Request (optional):**
```json
{
  "reason": "intentional_pattern"
}
```

---

### `POST /suggestions/batch`
Accept or reject multiple suggestions at once.

**Request:**
```json
{
  "action": "accept",
  "suggestion_ids": ["uuid1", "uuid2", "uuid3"]
}
```

---

## 8. Learning Profile Endpoints

### `GET /users/me/learning-profile`
Get the authenticated user's AI learning profile.

**Response:**
```json
{
  "model_version": "v1.4.2-user-abc",
  "total_accepts": 142,
  "total_rejects": 18,
  "accept_rate": 0.887,
  "last_trained_at": "2026-03-20T08:00:00Z",
  "preferred_style": {
    "smart_pointers": "always",
    "const_correctness": "strict",
    "naming_convention": "snake_case"
  }
}
```

---

### `DELETE /users/me/learning-profile`
Reset the user's learning profile to baseline.

**Response:** `204 No Content`

---

## 9. Organization Endpoints (Enterprise)

### `GET /orgs/:id/members`
List all members of an organization.

### `POST /orgs/:id/members`
Invite a user to an organization.

### `PATCH /orgs/:id/members/:userId`
Update a member's role.

### `GET /orgs/:id/audit-logs`
Retrieve audit logs for the organization.

**Query Params:** `page`, `limit`, `action`, `user_id`, `from`, `to`

---

## 10. WebSocket API

### Connection
```
wss://api.smartcompiler.dev/v1/ws?token=<jwt>
```

### Events

#### Client → Server

| Event | Payload | Description |
|-------|---------|-------------|
| `compile:start` | `{ project_id, entry_file, flags }` | Start a compilation |
| `compile:cancel` | `{ job_id }` | Cancel running job |

#### Server → Client

| Event | Payload | Description |
|-------|---------|-------------|
| `compile:queued` | `{ job_id, position }` | Job added to queue |
| `compile:started` | `{ job_id }` | Compilation began |
| `compile:progress` | `{ job_id, percent, stage }` | Progress update |
| `compile:log` | `{ job_id, line, level }` | Streaming compiler log |
| `compile:success` | `{ job_id, output_url, suggestions }` | Completed successfully |
| `compile:error` | `{ job_id, message, errors[] }` | Compilation failed |
| `ai:suggestion` | `{ suggestion }` | New suggestion generated |
| `model:updated` | `{ model_version }` | User model retrained |
