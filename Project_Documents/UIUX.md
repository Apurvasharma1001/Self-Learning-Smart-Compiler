# UI/UX Design Specification
## Self-Learning Smart Compiler

**Version:** 1.0.0  
**Date:** March 2026

---

## 1. Design Philosophy

The Smart Compiler IDE follows three core design principles:

- **Clarity First:** Complex AI decisions must be surfaced in plain, human-readable language.
- **Non-Intrusive Intelligence:** AI suggestions should augment, not interrupt, the developer's flow.
- **Progressive Disclosure:** Advanced options are hidden by default and revealed on demand.

---

## 2. Layout Overview

The IDE uses a four-zone layout:

```
┌──────────────────────────────────────────────────────────────┐
│                        TOP NAV BAR                           │
├────────────┬─────────────────────────────┬───────────────────┤
│            │                             │                   │
│  FILE      │       CODE EDITOR           │   AI SUGGESTIONS  │
│  EXPLORER  │       (Monaco)              │   PANEL           │
│            │                             │                   │
│            │                             │                   │
├────────────┴─────────────────────────────┴───────────────────┤
│                     CONSOLE / OUTPUT                         │
└──────────────────────────────────────────────────────────────┘
```

### Zone Descriptions

| Zone | Width | Description |
|------|-------|-------------|
| File Explorer | 220px | Project tree, file management |
| Code Editor | Flexible (fills center) | Monaco editor, main workspace |
| AI Suggestions Panel | 320px (collapsible) | Grouped AI recommendations |
| Console / Output | Full-width, resizable | Compiler output, errors, logs |

---

## 3. Top Navigation Bar

Contains the following elements left-to-right:

- **Logo + Product Name** (left)
- **Project Name** dropdown (breadcrumb)
- **Run / Compile** button (primary CTA, green)
- **AI Mode toggle** (on/off switch with label)
- **Learning Mode toggle** (beginner / expert)
- **User Avatar + Settings** (right)
- **Notifications Bell** (AI model updates, compile results)

---

## 4. File Explorer

- Tree view with folder expand/collapse
- Right-click context menu: New File, New Folder, Rename, Delete, Copy Path
- Drag-and-drop file reordering
- File type icons (`.c`, `.cpp`, `.h`, `.hpp`)
- Unsaved changes indicator (dot on tab and file entry)
- Search bar to filter files by name

---

## 5. Code Editor

Built on **Monaco Editor** (same engine as VS Code).

### 5.1 Editor Features
- Syntax highlighting for C and C++
- Line numbers with clickable error gutter icons
- Code folding on functions and blocks
- Multi-cursor editing (Alt+Click)
- Find & Replace (Ctrl+H)
- Minimap (right sidebar of editor, toggleable)
- Breadcrumb navigation (file > function > block)

### 5.2 AI Inline Markers
AI suggestions are shown inline without blocking code view:

- **Yellow squiggle:** Optimization opportunity
- **Blue squiggle:** Refactoring suggestion
- **Red squiggle:** Error or undefined behavior
- **Gutter icon:** Hover to see a 1-line summary; click to open full suggestion

### 5.3 Diff Preview
Before applying any AI change, a side-by-side diff is shown:

```
BEFORE                         AFTER
────────────────────────────────────────────
int* ptr = new int(5);    │  auto ptr = std::make_unique<int>(5);
delete ptr;               │
```

Accept / Reject buttons appear below the diff.

---

## 6. AI Suggestions Panel

### 6.1 Panel Sections
Suggestions are grouped into tabs:

- **Performance** (loop opts, memory, inlining)
- **Safety** (null checks, memory leaks, UB)
- **Style** (naming, const-correctness, modernization)
- **All** (combined sorted by priority)

### 6.2 Suggestion Card Design

Each suggestion card shows:

```
┌─────────────────────────────────────────┐
│ 🔧 Replace raw pointer with unique_ptr  │
│ Line 42 · main.cpp                      │
│ Impact: Safety ↑  Memory usage ↓        │
│                                         │
│ "Raw pointer ownership is ambiguous.    │
│  unique_ptr ensures automatic cleanup." │
│                                         │
│ [Preview Diff]  [✓ Apply]  [✗ Reject]  │
└─────────────────────────────────────────┘
```

### 6.3 Batch Actions
At the top of the panel:

- **Apply All (Performance)** — one-click all performance suggestions
- **Reject All** — dismiss all current suggestions
- **Undo Last Batch** — revert the last batch apply

---

## 7. Console / Output Panel

- Tabs: **Output**, **Errors**, **Warnings**, **AI Log**, **Build History**
- Errors are clickable — jumps to source file and line
- Each error has an expandable "AI Explanation" section
- Build stats shown after every compile: time, file count, optimization level applied
- Copy-to-clipboard button for all console text

---

## 8. Themes

| Theme | Description |
|-------|-------------|
| Dark (default) | Dark grey background, blue accents |
| Light | White background, navy accents |
| High Contrast | WCAG AAA compliant, for accessibility |
| System | Follows OS preference |

---

## 9. Onboarding Flow

### First-Time User
1. **Welcome screen** — Choose role (Student / Developer / Enterprise)
2. **Template picker** — Select a starter project or import own
3. **Tour overlay** — 5-step interactive walkthrough of IDE zones
4. **First compile prompt** — Guided through first AI-assisted compile

### Learning Mode (Student)
- Simplified AI Suggestions Panel (one suggestion at a time)
- Tooltips on all gutter icons explain the concept
- Progress bar for learning milestones
- "Why was this changed?" always expanded by default

---

## 10. Responsive & Accessibility

- Minimum supported viewport: 1280×720
- All interactive elements keyboard-navigable (Tab, Enter, Escape)
- ARIA labels on all custom components
- Focus ring visible in all themes
- Font sizes scalable (browser zoom supported to 150%)
- Color choices pass WCAG AA contrast ratio (minimum 4.5:1)
- Screen reader support for suggestion cards and error messages

---

## 11. Micro-Interactions & Feedback

| Action | Feedback |
|--------|----------|
| Compile triggered | Spinner on Run button, progress bar in console |
| AI suggestion applied | Smooth highlight fade on changed lines |
| Suggestion rejected | Card slides out with subtle animation |
| Save file | "Saved" toast (bottom-right, 2s) |
| Model improved | "Your AI model learned from this session" banner |
| Error in code | Gutter icon pulses once to draw attention |
