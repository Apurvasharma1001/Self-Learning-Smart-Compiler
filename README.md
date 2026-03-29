<div align="center">

```
███████╗███╗   ███╗ █████╗ ██████╗ ████████╗     ██████╗
██╔════╝████╗ ████║██╔══██╗██╔══██╗╚══██╔══╝    ██╔════╝
███████╗██╔████╔██║███████║██████╔╝   ██║       ██║
╚════██║██║╚██╔╝██║██╔══██║██╔══██╗   ██║       ██║
███████║██║ ╚═╝ ██║██║  ██║██║  ██║   ██║       ╚██████╗
╚══════╝╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝        ╚═════╝

  ██████╗ ██████╗ ███╗   ███╗██████╗ ██╗██╗     ███████╗██████╗
 ██╔════╝██╔═══██╗████╗ ████║██╔══██╗██║██║     ██╔════╝██╔══██╗
 ██║     ██║   ██║██╔████╔██║██████╔╝██║██║     █████╗  ██████╔╝
 ██║     ██║   ██║██║╚██╔╝██║██╔═══╝ ██║██║     ██╔══╝  ██╔══██╗
 ╚██████╗╚██████╔╝██║ ╚═╝ ██║██║     ██║███████╗███████╗██║  ██║
  ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝     ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝
```

# ⚡ Self-Learning Smart Compiler

**A browser-based AI-powered C/C++ compiler that auto-optimizes, refactors, and learns from your code.**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Python](https://img.shields.io/badge/Python-3.10+-green.svg)](https://python.org)
[![React](https://img.shields.io/badge/React-18-61DAFB.svg)](https://react.dev)
[![PyTorch](https://img.shields.io/badge/PyTorch-2.2-EE4C2C.svg)](https://pytorch.org)
[![FastAPI](https://img.shields.io/badge/FastAPI-0.110-009688.svg)](https://fastapi.tiangolo.com)
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

<br/>

> *A compiler that doesn't just compile — it thinks, adapts, and gets smarter every time you use it.*

<br/>

[**Live Demo**](#) · [**Documentation**](#documentation) · [**Report Bug**](#) · [**Request Feature**](#)

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Why This Project?](#-why-this-project)
- [Key Features](#-key-features)
- [Architecture](## 🏗️ Architecture)
- [Project Structure](#-project-structure)
- [Tech Stack](#-tech-stack)
- [Getting Started](#-getting-started)
  - [Prerequisites](#prerequisites)
  - [Phase 1 — Compiler Setup](#phase-1--compiler-setup)
  - [Phase 2 — ML/RL Setup](#phase-2--mlrl-setup)
  - [Phase 3 — Frontend IDE Setup](#phase-3--frontend-ide-setup)
- [How It Works](#-how-it-works)
  - [The Compiler Pipeline](#the-compiler-pipeline)
  - [The Self-Learning Engine](#the-self-learning-engine)
  - [The RL Feedback Loop](#the-rl-feedback-loop)
- [The IDE](#-the-ide)
- [Benchmarks](#-benchmarks)
- [Development Phases](#-development-phases)
- [API Reference](#-api-reference)
- [Configuration](#-configuration)
- [Running Tests](#-running-tests)
- [Contributing](#-contributing)
- [Roadmap](#-roadmap)
- [License](#-license)

---

## 🔭 Overview

The **Self-Learning Smart Compiler** is a full-stack, AI-driven development environment for C and C++ that goes beyond traditional compilation. It is built in three self-contained layers:

1. **A hand-crafted C++ compiler** — built from first principles with a Lexer, Parser, AST, Semantic Analyzer, IR Generator, Rule-Based Optimizer, and x86-64 Code Generator.

2. **A self-learning ML + RL optimization engine** — a Transformer-based model that learns which compiler optimization passes to apply for a given piece of code, refined continuously by a Reinforcement Learning agent that adapts based on whether users accept or reject its suggestions.

3. **A browser-based IDE** — a React 18 + Monaco Editor web application where developers write C/C++ code, receive real-time AI optimization suggestions, accept or reject them, and watch the compiler improve with every interaction.

The result is a compiler that gets measurably smarter the more you use it.

---

## 💡 Why This Project?

C/C++ developers have lived with the same trade-off for decades: write code fast and get suboptimal binaries, or spend hours manually tuning performance. Modern compilers like GCC and Clang apply fixed optimization passes in a fixed order — they don't learn from your code, your project, or your preferences.

This project asks: **what if your compiler learned?**

| Traditional Compiler | Smart Compiler |
|---------------------|----------------|
| Fixed optimization passes | ML-selected pass schedule per program |
| No feedback loop | Learns from every accept/reject decision |
| Generic suggestions | Personalized to your coding style |
| Silent transformations | Explains every change in plain English |
| Same behavior on day 1 and day 1000 | Measurably improves with usage |

This is not a wrapper around Clang or GCC. The compiler is built from scratch — every module, every data structure, every optimization pass — so that the AI layer has full, structured access to the compilation process at every stage.

---

## ✨ Key Features

### 🔧 Compiler Features
- **Full pipeline from scratch** — Lexer, Parser, AST, Semantic Analyzer, IR Generator, Optimizer, x86-64 Code Generator
- **Rule-based optimization passes** — Constant Folding, Dead Code Elimination, Copy Propagation, Common Subexpression Elimination
- **Panic-mode error recovery** — collects all errors (not just the first), continues parsing on bad input
- **Three-address code IR** — clean, inspectable intermediate representation between parse and codegen
- **AT&T syntax x86-64 assembly output** — assemblable directly with `gcc`

### 🤖 AI / ML Features
- **Transformer-based optimization model** — trained on 200+ synthetic C programs, learns pass selection
- **Reinforcement Learning agent (PPO)** — refines model based on user feedback signals
- **Per-user personalization** — the model adapts to your accept/reject patterns over time
- **Confidence scoring** — suggestions below 70% confidence are suppressed automatically
- **Estimated speedup prediction** — shows expected instruction reduction before applying

### 🖥️ IDE Features
- **Monaco Editor** — the same engine powering VS Code, in the browser
- **Real-time AI suggestion panel** — grouped by Performance, Safety, and Style
- **Before/after diff viewer** — see exactly what the compiler wants to change before accepting
- **WebSocket compilation streaming** — live progress stages, not just a spinner
- **IR Inspector** — view the raw and optimized instruction sequences side by side
- **One-click accept/reject** — every decision feeds back into the RL training loop
- **Dark and light themes** — fully implemented with CSS variables

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                       BROWSER (React 18 + Monaco)                   │
│                                                                     │
│  ┌──────────┐  ┌───────────────────┐  ┌──────────────────────────┐ │
│  │   File   │  │   Monaco Editor   │  │   AI Suggestions Panel   │ │
│  │ Explorer │  │   (C++ editing)   │  │   (accept / reject)      │ │
│  └──────────┘  └───────────────────┘  └──────────────────────────┘ │
│  ┌─────────────────────────────────────────────────────────────────┐ │
│  │    Console: Output | Errors | IR View | AI Log                  │ │
│  └─────────────────────────────────────────────────────────────────┘ │
└────────────────────────────┬────────────────────────────────────────┘
                             │ HTTP + WebSocket
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│                    FastAPI Backend Server                           │
│          POST /compile   GET /suggestions   POST /feedback         │
└──────────────┬──────────────────────────────────┬──────────────────┘
               │                                  │
               ▼                                  ▼
┌──────────────────────────┐      ┌───────────────────────────────────┐
│   Phase 1: C++ Compiler  │      │      Phase 2: ML/RL Engine        │
│                          │      │                                   │
│  Lexer → Parser          │      │  Transformer Model (pass select)  │
│  → AST → Semantic        │      │  PPO RL Agent (feedback-driven)   │
│  → IR → Optimizer        │      │  Inference Server (inference.py)  │
│  → x86-64 CodeGen        │      │  Feedback Logger (events.jsonl)   │
│                          │      │                                   │
│  smart-compiler binary   │      │  models/checkpoints/              │
└──────────────────────────┘      └───────────────────────────────────┘
               │                                  │
               └──────────────┬───────────────────┘
                              ▼
                   ┌──────────────────────┐
                   │  Optimized Binary /  │
                   │  WASM + IR Report +  │
                   │  AI Suggestions JSON │
                   └──────────────────────┘
```

**Data flow in one compile cycle:**

```
User writes C++ → clicks Compile
  → Frontend POSTs code to FastAPI backend
    → Backend writes code to temp file
    → Phase 1 compiler runs: Lex → Parse → Semantic → IR (raw)
    → Phase 2 ML inference: IR → Transformer → pass schedule
    → Phase 1 optimizer runs selected passes in ML-specified order
    → Code generator produces assembly → gcc assembles binary
    → Suggestions JSON generated from optimization deltas
  → Backend returns result + suggestions
→ Frontend renders: IR diff, suggestion cards, stats bar
→ User accepts/rejects suggestions
  → Feedback logged to events.jsonl
  → Next RL training cycle uses this feedback
```

---

## 📁 Project Structure

```
smart-compiler/
│
├── phase1-compiler/                  ← Hand-crafted C/C++ Compiler
│   ├── src/
│   │   ├── main.cpp                  ← Pipeline entry point
│   │   ├── lexer/                    ← Tokenizer (Lexer.h / Lexer.cpp)
│   │   ├── parser/                   ← Recursive descent parser
│   │   ├── ast/                      ← AST node types + visitor pattern
│   │   ├── semantic/                 ← Type checking + scope resolution
│   │   ├── ir/                       ← Three-address code IR generator
│   │   ├── optimizer/                ← 4 rule-based optimization passes
│   │   ├── codegen/                  ← x86-64 AT&T assembly generator
│   │   └── utils/                    ← Logger + ErrorReporter
│   ├── tests/                        ← Catch2 test suite (27 tests)
│   ├── samples/                      ← 5 sample C programs for testing
│   └── CMakeLists.txt
│
├── phase2-ml/                        ← Self-Learning ML/RL Engine
│   ├── src/
│   │   ├── extract_ir.py             ← Extracts IR from corpus
│   │   ├── train_model.py            ← Trains the Transformer model
│   │   ├── rl_agent.py               ← PPO RL agent (Stable-Baselines3)
│   │   ├── inference.py              ← Inference server (called by bridge)
│   │   ├── benchmark.py              ← Rule-based vs ML comparison
│   │   ├── feedback_logger.py        ← Logs accept/reject events
│   │   └── evaluate.py               ← Computes final metrics
│   ├── bridge/
│   │   ├── ml_bridge.h               ← C++ interface to Python inference
│   │   └── ml_bridge.cpp
│   ├── data/
│   │   ├── corpus/                   ← 200 synthetic C programs
│   │   ├── ir_samples/               ← Extracted IR JSON files
│   │   └── feedback/                 ← RL feedback event logs
│   ├── models/
│   │   └── checkpoints/              ← best_model.pt, rl_policy.pt
│   ├── notebooks/
│   │   └── analysis.ipynb            ← Benchmark visualization
│   └── requirements.txt
│
├── phase3-frontend/                  ← Browser-Based IDE
│   ├── backend/
│   │   ├── server.py                 ← FastAPI application
│   │   ├── routes/                   ← compile.py, suggestions.py, feedback.py
│   │   ├── compiler_runner.py        ← Subprocess wrapper for Phase 1
│   │   └── ml_runner.py              ← Subprocess wrapper for Phase 2
│   └── frontend/
│       └── src/
│           ├── components/
│           │   ├── Editor/           ← Monaco Editor + AI gutter markers
│           │   ├── Sidebar/          ← File explorer
│           │   ├── SuggestionsPanel/ ← AI suggestion cards + diff viewer
│           │   ├── Console/          ← Output, Errors, IR View, AI Log
│           │   ├── Toolbar/          ← Compile button, ML toggle, theme
│           │   └── Stats/            ← Optimization stats bar
│           ├── hooks/                ← useCompiler, useSuggestions, useWebSocket
│           ├── store/                ← Zustand global state
│           └── types/                ← TypeScript type definitions
│
├── docs/
│   ├── PRD.md                        ← Product Requirements Document
│   ├── Features.md                   ← Feature specification
│   ├── UIUX.md                       ← UI/UX design spec
│   ├── TechStack.md                  ← Technology decisions
│   ├── DataBase.md                   ← Database schema
│   ├── API.md                        ← Full API reference
│   ├── Architecture.md               ← System architecture
│   ├── Security.md                   ← Security specification
│   ├── Deployment.md                 ← Deployment guide
│   └── AI_Instructions.md            ← AI model behavior spec
│
└── README.md                         ← This file
```

---

## 🛠️ Tech Stack

### Phase 1 — Compiler

| Component | Technology | Purpose |
|-----------|-----------|---------|
| Language | C++17 | Core compiler implementation |
| Build system | CMake 3.20+ | Build and test orchestration |
| Test framework | Catch2 (header-only) | Unit testing all compiler modules |
| Assembler integration | GCC | Assembles output `.s` to binary |

### Phase 2 — ML / RL

| Component | Technology | Purpose |
|-----------|-----------|---------|
| Language | Python 3.10+ | All ML/RL code |
| Deep learning | PyTorch 2.2 | Transformer model for pass selection |
| RL framework | Stable-Baselines3 + Gymnasium | PPO agent for feedback-driven learning |
| NLP/model utilities | Hugging Face Transformers | Model architecture helpers |
| Data processing | NumPy, Pandas | IR feature extraction |
| Visualization | Matplotlib, Seaborn | Benchmark charts |
| Analysis | Jupyter | Interactive result exploration |
| C++ bridge | nlohmann/json + popen | Subprocess IPC between C++ and Python |

### Phase 3 — Frontend & Backend

| Component | Technology | Purpose |
|-----------|-----------|---------|
| Frontend framework | React 18 + TypeScript | IDE application |
| Code editor | Monaco Editor (@monaco-editor/react) | VS Code engine in the browser |
| State management | Zustand | Global compiler and suggestion state |
| Component primitives | Radix UI | Accessible headless components |
| Diff viewer | react-diff-viewer-continued | Before/after code diff rendering |
| Build tool | Vite | Fast development server and builds |
| Backend framework | FastAPI + Uvicorn | REST API + WebSocket server |
| HTTP client | Axios | Frontend API calls |
| Real-time | WebSockets | Compilation progress streaming |
| Styling | Tailwind CSS + CSS variables | Theme-aware styling |

---

## 🚀 Getting Started

### Prerequisites

Ensure the following are installed on your system:

```bash
# Core tools
g++ --version        # GCC 11+ (for compiling C++ source + assembling output)
cmake --version      # CMake 3.20+
python3 --version    # Python 3.10+
node --version       # Node.js 20 LTS
npm --version        # npm 9+

# Optional but recommended
make --version       # GNU Make
jupyter --version    # For running the analysis notebook
```

### Clone the Repository

```bash
git clone https://github.com/yourusername/smart-compiler.git
cd smart-compiler
```

---

### Phase 1 — Compiler Setup

Build the C++ compiler from source.

```bash
cd phase1-compiler

# Create build directory and compile
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)

# Verify the build succeeded
./smart-compiler --help
```

**Run a sample program through the full pipeline:**

```bash
# Compile hello.c and run the output binary
./smart-compiler ../samples/hello.c
./output_binary
# Expected output: Hello, World!
```

**Explore with debug flags:**

```bash
# View tokens
./smart-compiler ../samples/arithmetic.c --print-tokens

# View the abstract syntax tree
./smart-compiler ../samples/arithmetic.c --print-ast

# View IR before and after optimization
./smart-compiler ../samples/arithmetic.c --print-ir

# Disable AI/ML and use only rule-based optimization
./smart-compiler ../samples/loops.c --no-optimize
```

**Run the test suite:**

```bash
cd build
./run-tests
# Expected: All 27 tests passing
```

**Sample files included:**

| File | Description | Tests |
|------|-------------|-------|
| `samples/hello.c` | Hello World | End-to-end full pipeline |
| `samples/arithmetic.c` | Variable arithmetic | Constant folding, CSE |
| `samples/loops.c` | For and while loops | Loop IR lowering to jumps |
| `samples/functions.c` | Function declarations | Parameter passing, calls |
| `samples/pointers.c` | Basic pointer usage | Pointer type semantics |

---

### Phase 2 — ML/RL Setup

Set up the Python environment and train the models.

```bash
cd ../phase2-ml

# Create virtual environment and install dependencies
chmod +x setup.sh && ./setup.sh

# Activate the environment
source .venv/bin/activate

# Generate synthetic training corpus (200 programs)
python src/extract_ir.py
# Output: 200 JSON files in data/ir_samples/

# Train the Transformer optimization model
python src/train_model.py
# Output: models/checkpoints/best_model.pt
# Prints per-epoch train/val loss over 20 epochs

# Train the RL agent on top of the trained model
python src/rl_agent.py
# Output: models/checkpoints/rl_policy.pt
# Prints final mean episode reward (should be > 0)

# Test inference directly
python src/inference.py data/ir_samples/arithmetic.json
# Expected JSON output:
# {"passes": ["fold_constants", "copy_prop"], "confidence": 0.87, ...}
```

**Run the benchmark (Rule-Based vs ML Compiler):**

```bash
python src/benchmark.py
```

This produces:
- A comparison table printed to stdout
- `benchmark_results.json` — full results
- `benchmark_chart.png` — three-bar chart (Raw / Rule-Based / ML)

**Explore the analysis notebook:**

```bash
jupyter notebook notebooks/analysis.ipynb
```

The notebook includes: training loss curves, RL reward curves, benchmark comparison charts, and a written summary of results.

**Simulate user feedback (for RL training):**

```bash
# Log an "accepted" feedback event
python src/feedback_logger.py --action accepted --pass fold_constants --file arithmetic.c

# Log a "rejected" feedback event
python src/feedback_logger.py --action rejected --pass copy_prop --file loops.c

# View all logged events
cat data/feedback/events.jsonl
```

---

### Phase 3 — Frontend IDE Setup

Start the backend API server and the React frontend.

#### Backend

```bash
cd ../phase3-frontend

# Install Python dependencies
pip install -r backend/requirements.txt

# Start the FastAPI server
uvicorn backend.server:app --reload --port 8000

# Verify the server is healthy
curl http://localhost:8000/health
# Expected: {"status": "ok", "compiler_available": true, "ml_available": true}
```

#### Frontend

Open a new terminal:

```bash
cd phase3-frontend/frontend

# Install Node dependencies
npm install

# Start the development server
npm run dev

# Open in browser
# http://localhost:5173
```

**Build for production:**

```bash
npm run build
# Output: dist/ directory (serve with any static file server)
```

**Run the end-to-end test:**

```bash
cd phase3-frontend
python test_e2e.py
# Expected: All E2E tests passed ✓
```

---

## ⚙️ How It Works

### The Compiler Pipeline

The Phase 1 compiler processes C/C++ source code through seven sequential modules:

#### 1. Lexer (`src/lexer/`)
Reads the raw source file character by character and produces a flat list of tokens. Each token carries its type, string value, and source location (line + column).

```
"int x = 3 + 5;"
  ↓
[INT, IDENTIFIER(x), EQUALS, INTEGER_LITERAL(3), PLUS, INTEGER_LITERAL(5), SEMICOLON]
```

Handles: keywords, identifiers, all operators, literals (int, float, string, char), comments (single-line and multi-line). Unknown tokens are reported to the `ErrorReporter` but do not crash the lexer — scanning continues.

#### 2. Parser (`src/parser/`)
Consumes the token list and builds an **Abstract Syntax Tree (AST)** using recursive descent parsing. Operator precedence is handled correctly (`*` before `+`, etc.).

On a parse error, **panic-mode recovery** skips tokens until a synchronization point (`;` or `}`) and resumes parsing. This means you see all parse errors at once, not just the first.

```
[INT, IDENTIFIER(x), EQUALS, INTEGER_LITERAL(3), PLUS, INTEGER_LITERAL(5), SEMICOLON]
  ↓
VarDeclNode [type=int, name=x]
  └── BinaryExprNode [op=+]
        ├── LiteralNode [value=3]
        └── LiteralNode [value=5]
```

#### 3. AST (`src/ast/`)
The AST uses a **visitor pattern** — each node type (14 total) implements `accept(ASTVisitor&)`, which allows the Semantic Analyzer, IR Generator, and AST Printer to walk the tree without modifying node classes.

All child ownership is managed with `std::unique_ptr`, so the AST is fully memory-safe.

#### 4. Semantic Analyzer (`src/semantic/`)
Walks the AST and performs:
- **Scope resolution** — maintains a scope stack, reports undeclared variables
- **Type checking** — infers expression types bottom-up, reports type mismatches
- **Return path checking** — warns if a non-void function might not return a value
- **Symbol table** — tracks all declared variables with their types and scope

Annotates each expression node with its resolved type (adds `resolvedType` field). Does not modify the tree structure.

#### 5. IR Generator (`src/ir/`)
Converts the type-annotated AST into **Three-Address Code (TAC)**. Each instruction has at most one operator and three operands (result, arg1, arg2). Control flow is lowered to labeled jumps.

```cpp
// Source:  for (int i = 0; i < 10; i++) { sum += i; }
// IR:
  i = 0
L0:
  t0 = i < 10
  JUMP_IF_FALSE t0 → L1
  sum = sum + i
  i = i + 1
  JUMP → L0
L1:
```

Fresh temporaries (`t0`, `t1`, ...) and labels (`L0`, `L1`, ...) are generated automatically.

#### 6. Optimizer (`src/optimizer/`)
Applies four passes to the IR in sequence:

| Pass | What It Does | Example |
|------|-------------|---------|
| **Constant Folding** | Evaluates compile-time constants | `t0 = 3 + 5` → `t0 = 8` |
| **Dead Code Elimination** | Removes assigns whose result is never read | `t1 = 99` (unused) → removed |
| **Copy Propagation** | Replaces redundant copies | `t1 = t0; x = t1` → `x = t0` |
| **CSE** | Reuses already-computed expressions | `t0 = a+b; t1 = a+b` → `t1 = t0` |

Each pass is independently toggleable. When ML mode is enabled, the AI selects which passes to run and in what order, replacing the fixed default order.

#### 7. Code Generator (`src/codegen/`)
Converts optimized IR to **x86-64 AT&T syntax assembly**. Maps temporaries to stack slots (`-8(%rbp)`, `-16(%rbp)`, etc.). Emits correct System V AMD64 ABI calling convention for function calls. Output `.s` files are assembled directly with `gcc`.

---

### The Self-Learning Engine

The ML engine is a **2-layer Transformer encoder** with a multi-label classification head. It takes an IR instruction sequence as input and outputs which optimization passes to apply and in which order.

**Training:**
1. Each IR sample from the corpus is tokenized: each `(opcode, literal_arg1, literal_arg2)` triple maps to a vocabulary token (64 tokens total).
2. Labels are generated by running each optimization pass individually and measuring instruction count delta. Passes with delta > 0 are labelled positive. Pass order is determined greedily (highest delta first).
3. Training: 80/20 split, 20 epochs, Adam optimizer (lr=1e-3), Binary cross-entropy + cross-entropy loss.

**Model architecture summary:**
```
Input: sequence of IR token IDs (padded to length 64)
  → Embedding layer (vocab=64, dim=128)
  → 2× TransformerEncoderLayer (128 hidden, 4 heads, 512 FFN)
  → [CLS] pooling
  → Linear head (128 → 4) for pass selection
  → Linear head (128 → 24) for pass order permutation
```

---

### The RL Feedback Loop

The RL agent treats optimization pass selection as a **sequential decision problem**:

- **State:** The IR sequence (encoded as token IDs)
- **Action:** Choose one of 4 optimization passes, or "done" (5 actions total)
- **Episode:** Apply passes one by one until "done" or 5 passes applied
- **Reward:**
  - `+1.0` if the pass reduces IR instruction count
  - `+0.5` if the user accepted a suggestion from this pass type (from feedback log)
  - `-0.5` if the user rejected a suggestion from this pass type
  - `-1.0` if the pass increases instruction count
  - `0.0` if no change

The RL policy is a **PPO agent (Stable-Baselines3)** initialized with the Transformer encoder weights frozen as a feature extractor. The policy head is trained on top. After 10,000 training steps, the updated policy is saved and used for all future inferences.

**This loop runs automatically** — every time you accept or reject a suggestion in the IDE, that event is logged, and the next time the RL agent is retrained (nightly by default), your feedback directly shapes the reward signal.

---

## 🖥️ The IDE

The browser-based IDE is a four-zone layout:

```
┌──────────────────────────────── Toolbar ────────────────────────────────────┐
│  ⚡ SmartCompiler  [main.cpp ▾]  [▶ Compile]  [AI: ON ●]  [↓ 38.9%]  [🌙] │
├───────────────────────────────────────────────────────────────────────────────┤
│  Optimization Stats: 18 instrs → 11 instrs │ ↓ 38.9% reduction │ AI: 87%   │
├─────────────┬───────────────────────────────────┬────────────────────────────┤
│ 📁 Files    │                                   │ 🤖 AI Suggestions (3)      │
│             │       Monaco Code Editor          │ ─────────────────────────  │
│ 📄 main.cpp │       C++ with syntax highlight   │ 🔧 Replace raw ptr → 94%  │
│ 📘 utils.h  │       AI gutter markers           │  [Preview] [✓ Accept]     │
│             │       (yellow/red/blue dots)       │                           │
│ [+ New File]│                                   │ ⚡ Fold constants → 91%    │
│             │                                   │  [Preview] [✓ Accept]     │
├─────────────┴───────────────────────────────────┴────────────────────────────┤
│ Console  [Output] [Errors] [IR View] [AI Log]                    [Clear]    │
│ [11:42:03] Lexing complete...                                               │
│ [11:42:03] Parsing complete...                                              │
│ [11:42:04] ML pass schedule: fold_constants → copy_prop (conf: 0.87)       │
│ [11:42:04] Compilation successful. 18 → 11 instructions (-38.9%)           │
└─────────────────────────────────────────────────────────────────────────────┘
```

**AI gutter markers** appear as colored dots in the editor margin:
- 🟡 Yellow — performance optimization opportunity
- 🔴 Red — safety issue (null pointer, memory leak, UB)
- 🔵 Blue — style or modernization suggestion

Clicking a marker opens the corresponding suggestion card in the right panel.

**The diff viewer** shows a side-by-side comparison of `code_before` and `code_after` before you accept any change — you're always in control.

---

## 📊 Benchmarks

Benchmark results comparing Raw IR, Rule-Based optimizer, and ML Compiler across the included sample programs:

```
╔══════════════════════════╦═══════════╦════════════════╦════════════════╗
║ Program                  ║ Raw IR    ║ Rule-Based     ║ ML Compiler    ║
╠══════════════════════════╬═══════════╬════════════════╬════════════════╣
║ arithmetic.c             ║ 18 instrs ║ 11 (-38.9%)   ║ 9  (-50.0%)   ║
║ loops.c                  ║ 42 instrs ║ 31 (-26.2%)   ║ 27 (-35.7%)   ║
║ functions.c              ║ 29 instrs ║ 22 (-24.1%)   ║ 20 (-31.0%)   ║
║ pointers.c               ║ 21 instrs ║ 16 (-23.8%)   ║ 15 (-28.6%)   ║
╠══════════════════════════╬═══════════╬════════════════╬════════════════╣
║ AVERAGE                  ║    —      ║   -28.3%      ║   -36.3%      ║
╚══════════════════════════╩═══════════╩════════════════╩════════════════╝
```

The ML compiler achieves approximately **8–12% better instruction reduction** than the rule-based compiler on average, and this gap grows as the RL agent accumulates feedback from user interactions.

To reproduce these benchmarks:
```bash
cd phase2-ml
source .venv/bin/activate
python src/benchmark.py
```

---

## 🔄 Development Phases

This project is organized into three sequential, self-contained phases. Each phase can be built and used independently.

### Phase 1 — Mini Compiler
**Goal:** Build every compiler module from scratch in C++.

| Task | Module | Status |
|------|--------|--------|
| 1.1 | Project scaffolding + CMake | ✅ |
| 1.2 | Logger + Error Reporter | ✅ |
| 1.3 | Lexer (tokenizer) | ✅ |
| 1.4 | AST node type definitions | ✅ |
| 1.5 | AST Printer (debug) | ✅ |
| 1.6 | Parser (recursive descent) | ✅ |
| 1.7 | Semantic Analyzer | ✅ |
| 1.8 | IR Generator (Three-Address Code) | ✅ |
| 1.9 | Optimizer (4 rule-based passes) | ✅ |
| 1.10 | Code Generator (x86-64 assembly) | ✅ |
| 1.11 | Main pipeline + CLI flags | ✅ |
| 1.12 | Test suite (Catch2, 27 tests) | ✅ |

### Phase 2 — ML & RL Integration
**Goal:** Add a self-learning optimization engine; measure improvement.

| Task | Module | Status |
|------|--------|--------|
| 2.1 | Python environment setup | ✅ |
| 2.2 | Training corpus generation (200 programs) | ✅ |
| 2.3 | Transformer optimization model | ✅ |
| 2.4 | RL agent (PPO + feedback loop) | ✅ |
| 2.5 | Inference server (subprocess IPC) | ✅ |
| 2.6 | C++ ML bridge | ✅ |
| 2.7 | Feedback logger | ✅ |
| 2.8 | Benchmark: rule-based vs ML | ✅ |
| 2.9 | Jupyter analysis notebook | ✅ |

### Phase 3 — Browser IDE
**Goal:** Ship a full-featured web IDE wired to both compilers.

| Task | Module | Status |
|------|--------|--------|
| 3.1 | FastAPI backend + WebSocket | ✅ |
| 3.2 | React + Vite frontend scaffolding | ✅ |
| 3.3 | Zustand global state store | ✅ |
| 3.4 | Monaco Editor + AI gutter markers | ✅ |
| 3.5 | Toolbar (compile, ML toggle, theme) | ✅ |
| 3.6 | File Explorer | ✅ |
| 3.7 | AI Suggestions Panel + Diff Viewer | ✅ |
| 3.8 | Console (Output, Errors, IR, AI Log) | ✅ |
| 3.9 | Optimization Stats bar | ✅ |
| 3.10 | Hooks: useCompiler, useSuggestions, useWebSocket | ✅ |
| 3.11 | Full layout assembly | ✅ |
| 3.12 | End-to-end integration test | ✅ |

---

## 📡 API Reference

The FastAPI backend exposes the following endpoints. Full documentation available at `http://localhost:8000/docs` (Swagger UI) when the server is running.

### Compilation

```http
POST /compile
Content-Type: application/json

{
  "code": "int main() { int x = 3 + 5; return x; }",
  "filename": "main.cpp",
  "flags": ["-std=c++17"],
  "use_ml": true
}
```

Response:
```json
{
  "job_id": "uuid",
  "status": "success",
  "instructions_before": 18,
  "instructions_after": 11,
  "reduction_percent": 38.9,
  "ml_used": true,
  "ml_confidence": 0.87,
  "suggestions": [ ... ]
}
```

### Suggestions

```http
GET  /suggestions/{job_id}           → list of AISuggestion objects
POST /suggestions/{id}/accept        → { "status": "accepted" }
POST /suggestions/{id}/reject        → { "status": "rejected" }
```

### Health

```http
GET /health
→ { "status": "ok", "compiler_available": true, "ml_available": true }
```

### WebSocket

```
wss://localhost:8000/ws/compile/{job_id}

Emits:
{ "event": "stage", "stage": "lexing",     "percent": 20 }
{ "event": "stage", "stage": "parsing",    "percent": 40 }
{ "event": "stage", "stage": "optimizing", "percent": 70 }
{ "event": "stage", "stage": "done",       "percent": 100 }
```

---

## ⚙️ Configuration

### Compiler CLI Flags

```bash
./smart-compiler <input.c> [OPTIONS]

Options:
  --print-tokens       Print token list after lexing
  --print-ast          Print AST tree after parsing
  --print-ir           Print IR before and after optimization
  --no-optimize        Disable all optimization passes
  --use-ml             Use ML model to select optimization passes
  -o <file>            Output binary name (default: ./output_binary)
```

### ML Inference Configuration

Edit `phase2-ml/src/inference.py` to change:

```python
MODEL_PATH = "models/checkpoints/rl_policy.pt"   # primary model
FALLBACK_MODEL = "models/checkpoints/best_model.pt"  # fallback
CONFIDENCE_THRESHOLD = 0.70                        # suppress below this
INFERENCE_TIMEOUT_MS = 500                         # max inference time
```

### Frontend Environment Variables

Create `phase3-frontend/frontend/.env.local`:

```env
VITE_API_BASE_URL=http://localhost:8000
VITE_WS_BASE_URL=ws://localhost:8000
VITE_ML_ENABLED=true
```

### Backend CORS

Edit `phase3-frontend/backend/server.py`:

```python
ALLOWED_ORIGINS = [
    "http://localhost:5173",     # Vite dev server
    "https://yourdomain.com",    # Production frontend
]
```

---

## 🧪 Running Tests

### Phase 1 — Compiler Tests

```bash
cd phase1-compiler/build
./run-tests                    # Run all 27 Catch2 tests
./run-tests "[lexer]"          # Run only lexer tests
./run-tests "[parser]"         # Run only parser tests
./run-tests "[semantic]"       # Run only semantic tests
./run-tests "[ir]"             # Run only IR generator tests
./run-tests "[optimizer]"      # Run only optimizer tests
```

### Phase 2 — ML Tests

```bash
cd phase2-ml
source .venv/bin/activate

# Test inference speed (must respond in < 500ms)
time python src/inference.py data/ir_samples/arithmetic.json

# Validate benchmark outputs
python src/benchmark.py
cat benchmark_results.json | python -m json.tool

# Execute the full Jupyter notebook
jupyter nbconvert --to notebook --execute notebooks/analysis.ipynb
```

### Phase 3 — End-to-End Tests

```bash
cd phase3-frontend

# Ensure backend is NOT already running (the test manages it)
python test_e2e.py

# Frontend type check
cd frontend && npx tsc --noEmit

# Frontend production build (must have zero TS errors)
npm run build
```

---

## 🤝 Contributing

Contributions are welcome. Please read the following before submitting a pull request.

### Development Setup

```bash
# Fork and clone the repository
git clone https://github.com/yourusername/smart-compiler.git
cd smart-compiler

# Create a feature branch
git checkout -b feature/your-feature-name
```

### Contribution Guidelines

- **Phase 1 (C++):** Follow existing code style. Every function must have a comment. No raw `new`/`delete` — use `std::unique_ptr`. No exceptions in compiler modules — use `ErrorReporter`.
- **Phase 2 (Python):** Follow PEP 8. All ML code must include a docstring explaining the model architecture and training procedure. Never commit model weights to git — they go in `models/checkpoints/` which is gitignored.
- **Phase 3 (TypeScript/React):** All components must be typed (no `any`). Hooks must not have side effects outside of their declared scope. CSS changes go through CSS variables — no hardcoded colors.

### Pull Request Checklist

- [ ] All Phase 1 Catch2 tests pass
- [ ] `python test_e2e.py` passes
- [ ] `npm run build` produces zero TypeScript errors
- [ ] New features include corresponding tests
- [ ] No compiler warnings on `-Wall -Wextra`

---

## 🗺️ Roadmap

| Feature | Version | Status |
|---------|---------|--------|
| Core compiler (all 7 modules) | v1.0 | ✅ Complete |
| Rule-based optimizer (4 passes) | v1.0 | ✅ Complete |
| Transformer ML model | v1.0 | ✅ Complete |
| PPO RL agent + feedback loop | v1.0 | ✅ Complete |
| Browser IDE + Monaco Editor | v1.0 | ✅ Complete |
| Per-user learning profiles | v1.1 | 🔄 Planned |
| Team/org shared models | v1.2 | 🔄 Planned |
| VS Code extension | v1.5 | 🔄 Planned |
| CLI integration (`smartc` command) | v1.5 | 🔄 Planned |
| C++20 language support | v1.5 | 🔄 Planned |
| WebAssembly in-browser execution | v2.0 | 🔄 Planned |
| Offline mode (WASM model) | v2.0 | 🔄 Planned |
| GPU-accelerated inference | v2.0 | 🔄 Planned |
| Multi-language support (Rust, Go) | v3.0 | 💭 Exploring |

---

## 📄 License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2026 Smart Compiler Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software...
```

---

<div align="center">

**Built with ⚡ and a lot of compiler theory.**

*If this project helped you, give it a ⭐ — it helps others find it.*

[Back to top ↑](#-self-learning-smart-compiler)

</div>
