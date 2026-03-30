# TurboPath — AVL-Powered Path Finder

> **Assignment 2 | DS&A Lab | NUST CEME**  
> A high-speed file search utility that pre-indexes an entire directory tree into an AVL tree held in RAM, enabling **O(log n)** file lookups without touching the disk after startup.

---

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [Features](#2-features)
3. [File Structure](#3-file-structure)
4. [How to Compile & Run](#4-how-to-compile--run)
5. [Usage Guide](#5-usage-guide)
6. [Class & Method Reference](#6-class--method-reference)
   - [struct node](#61-struct-node)
   - [class AVL](#62-class-avl)
   - [class ConsoleUtils](#63-class-consoleutils)
   - [Global Functions](#64-global-functions)
7. [Complexity Analysis](#7-complexity-analysis)
8. [Design Decisions](#8-design-decisions)
9. [Known Limitations](#9-known-limitations)

---

## 1. Project Overview

DataVault Solutions manages millions of files across thousands of nested directories. The default OS search reads the disk in real-time on every query, causing multi-minute delays. **TurboPath** solves this by:

1. Traversing the entire directory tree **once** at startup
2. Loading every filename + full path into a **self-balancing AVL tree** in RAM
3. Serving all future search queries from memory in **O(log n)** time

---

## 2. Features

| Feature | Details |
|---|---|
| Pre-indexing | Full recursive directory traversal on startup |
| Fast Search | O(log n) guaranteed via AVL self-balancing |
| Duplicate Handling | Same filename in multiple folders → all paths listed |
| Refresh Command | Re-index without restarting the program |
| Colored CLI | ConsoleUtils ANSI color output with cursor control |
| Input Validation | Templated input validation with retry on bad input |

---

## 3. File Structure

```
TurboPath/
├── main.cpp              ← UI, loadDirectory, refresh
├── AVL.cpp               ← node struct + full AVL class
├── ConsoleUtils.h        ← ConsoleUtils class declaration + ConsoleColor enum
├── ConsoleUtils.cpp      ← ConsoleUtils method implementations
└── .vscode/
    ├── tasks.json        ← Build config: compiles main.cpp + ConsoleUtils.cpp
    └── launch.json       ← Debug config: opens external terminal window
```

---

## 4. How to Compile & Run

### Prerequisites
- **Compiler:** g++ with MinGW64 (or MSVC via Visual Studio)
- **C++ Standard:** C++17 (required for `<filesystem>`)
- **OS:** Windows (ConsoleUtils uses Windows ANSI + `<windows.h>`)

### Option A — VS Code (Recommended)

1. Open the `TurboPath` folder in VS Code
2. Make sure `.vscode/tasks.json` and `.vscode/launch.json` are present
3. Press **`Ctrl+Shift+B`** to build
4. Press **`F5`** to run in an external terminal window

> ⚠️ Must run in a real `cmd.exe` window — VS Code's integrated terminal does not render ANSI cursor positioning correctly.

### Option B — Manual Command Line

Open `cmd.exe` in the project folder and run:

```bat
g++ main.cpp ConsoleUtils.cpp -o TurboPath.exe -std=c++17
TurboPath.exe
```

### Option C — Visual Studio 2022

1. Create a new Empty C++ Project
2. Add all `.cpp` and `.h` files to the project
3. Go to **Project Properties → C/C++ → Language → C++ Language Standard → ISO C++17**
4. Build and run

---

## 5. Usage Guide

### On Startup
```
Enter root directory path to index:
  >> D:\TurboPath
```
Enter the full path to the root folder you want to index. If the path is invalid or empty, the program re-prompts automatically.

### Searching
```
  Search >> B1.pdf
```
Type any filename with or without extension. The result shows all matching paths:
```
  [+] Found 2 match(es) for "report.pdf":
      [1] D:\DataVault\Clients\2023\Q4\report.pdf
      [2] D:\DataVault\Archive\2022\report.pdf
```

### Commands

| Command | Action |
|---|---|
| `filename.ext` | Search for exact filename |
| `refresh` or `re-index` | Re-traverse directory and rebuild AVL tree |
| `exit` or `quit` | Exit the program |

---

## 6. Class & Method Reference

---

### 6.1 `struct node`
**File:** `AVL.h`  
The fundamental building block of the AVL tree.

| Field | Type | Purpose |
|---|---|---|
| `filename` | `string` | The search key — the file's name (e.g. `report.pdf`) |
| `paths` | `vector<string>` | All absolute paths for this filename (handles duplicates) |
| `height` | `int` | AVL balance tracking — height of this node in the tree |
| `left` | `node*` | Pointer to left child (filenames alphabetically smaller) |
| `right` | `node*` | Pointer to right child (filenames alphabetically larger) |

---

### 6.2 `class AVL`
**File:** `AVL.cpp`  
Self-balancing binary search tree keyed on filename strings.

#### Private Methods

| Method | Signature | Purpose | Time Complexity |
|---|---|---|---|
| `createNode` | `node*(string, string)` | Allocates and initializes a new node with height=1 | O(1) |
| `getHeight` | `int(node*)` | Returns node height; returns 0 for nullptr safely | O(1) |
| `updateHeight` | `void(node*)` | Recalculates height as 1 + max(left, right) heights | O(1) |
| `getBalance` | `int(node*)` | Returns left height minus right height (balance factor) | O(1) |
| `rotateRight` | `node*(node*)` | Right rotation for LL and LR imbalance cases | O(1) |
| `rotateLeft` | `node*(node*)` | Left rotation for RR and RL imbalance cases | O(1) |
| `avl_insert` | `node*(node*, string, string)` | Recursive BST insert + height update + rebalance | O(log n) |
| `findMin` | `node*(node*)` | Walks left until leaf — finds inorder successor for delete | O(log n) |
| `avl_delete` | `node*(node*, string)` | Recursive delete with 3 cases + rebalance on the way up | O(log n) |
| `searchNode` | `node*(node*, string)` | Recursive binary search by filename string | O(log n) |
| `inorder` | `void(node*)` | Left → Root → Right traversal (alphabetical order) | O(n) |
| `preorder` | `void(node*)` | Root → Left → Right traversal | O(n) |
| `postorder` | `void(node*)` | Left → Right → Root traversal | O(n) |
| `copyTree` | `node*(node*)` | Deep copy via preorder traversal — copies paths vector too | O(n) |
| `freeTree` | `void(node*)` | Postorder deletion of all nodes — used by destructor & refresh | O(n) |

#### AVL Rotation Cases

| Case | Condition | Fix |
|---|---|---|
| Left Left (LL) | `balance > 1` && new key < left child | `rotateRight(node)` |
| Right Right (RR) | `balance < -1` && new key > right child | `rotateLeft(node)` |
| Left Right (LR) | `balance > 1` && new key > left child | `rotateLeft(left)` → `rotateRight(node)` |
| Right Left (RL) | `balance < -1` && new key < right child | `rotateRight(right)` → `rotateLeft(node)` |

#### Public Methods

| Method | Signature | Purpose | Time Complexity |
|---|---|---|---|
| `AVL()` | Constructor | Initializes root to nullptr | O(1) |
| `~AVL()` | Destructor | Calls freeTree — cleans all heap memory | O(n) |
| `insert` | `void(string, string)` | Public wrapper for avl_insert | O(log n) |
| `remove` | `void(string)` | Public wrapper for avl_delete | O(log n) |
| `search` | `node*(string)` | Returns matched node* — caller handles printing | O(log n) |
| `isEmpty` | `bool()` | Checks if root is nullptr | O(1) |
| `inorder` | `void()` | Prints all filenames in alphabetical order | O(n) |
| `preorder` | `void()` | Prints filenames in preorder | O(n) |
| `postorder` | `void()` | Prints filenames in postorder | O(n) |
| `copyTree` | `AVL()` | Returns a completely independent deep copy of the tree | O(n) |
| `refresh` | `void(string)` | Nukes tree via freeTree, re-indexes from rootPath | O(n) |

---

### 6.3 `class ConsoleUtils`
**File:** `ConsoleUtils.h` / `ConsoleUtils.cpp`  
Static utility class for Windows ANSI console control.

| Method | Signature | Purpose |
|---|---|---|
| `enableVirtualTerminal` | `void()` | Enables ANSI escape sequence processing on Windows — call once at startup |
| `setColor` | `void(int, int)` | Sets both foreground and background color in one call |
| `setForegroundColor` | `void(int)` | Sets text color using ANSI 256-color escape code |
| `setBackgroundColor` | `void(int)` | Sets background color using ANSI 256-color escape code |
| `setDefaultColor` | `void()` | Resets all colors to terminal default via `\033[0m` |
| `setConsoleBackgroundColor` | `void(int, int)` | Sets console-wide background using Windows API (16 colors) |
| `clearConsole` | `void()` | Clears the screen via `system("cls")` |
| `moveCursor` | `void(int x, int y)` | Moves cursor to absolute position using ANSI escape |
| `hideCursor` | `void()` | Hides blinking cursor via `\033[?25l` |
| `showCursor` | `void()` | Restores cursor visibility via `\033[?25h` |
| `DrawRectangle` | `void(int,int,int,int)` | Draws single-line ASCII box at given position and size |
| `DrawDoubleLineRectangle` | `void(int,int,int,int)` | Draws double-line ASCII box — used for the header |
| `showCredits` | `void()` | Virtual method — prints author name and version |

#### `ConsoleColor` Enum

```cpp
Black=0, Red=1, Green=2, Yellow=3, Blue=4, Magenta=5, Cyan=6, White=7,
BrightBlack=8, BrightRed=9, BrightGreen=10, BrightYellow=11,
BrightBlue=12, BrightMagenta=13, BrightCyan=14, BrightWhite=15
```

---

### 6.4 Global Functions
**File:** `main.cpp`

| Function | Signature | Purpose | Time Complexity |
|---|---|---|---|
| `getValidInput<T>` | `T(string)` | Templated input with retry loop on invalid type | O(1) |
| `loadDirectory` | `void(fs::path, AVL&)` | Recursively traverses directory tree, inserts every file into AVL | O(n log n) |
| `AVL::refresh` | `void(string)` | Defined here — calls freeTree then loadDirectory | O(n log n) |
| `drawHeader` | `void()` | Draws double-line box header with title and subtitle | O(1) |
| `drawDivider` | `void()` | Prints a dim horizontal line separator | O(1) |
| `printHelp` | `void()` | Prints available commands in dim color | O(1) |
| `printResult` | `void(node*, string)` | Handles colored output of search results or not-found message | O(k) — k = number of matching paths |

---

## 7. Complexity Analysis

### Time Complexity

| Operation | Complexity | Reason |
|---|---|---|
| Startup indexing | O(n log n) | n files, each inserted in O(log n) |
| Search | **O(log n)** | AVL guaranteed balanced — max depth = log₂(n) |
| Refresh | O(n log n) | Full re-traversal + re-insertion |
| Delete | O(log n) | BST delete + rebalance rotations |

> **Why not O(n) worst case like plain BST?**  
> A plain BST degrades to O(n) when files are inserted in sorted/alphabetical order (which `fs::directory_iterator` often does). AVL self-balancing via rotations ensures the tree never becomes a skewed chain — height is always bounded to **⌊log₂(n)⌋**.

### Space Complexity

| What | Complexity | Details |
|---|---|---|
| AVL tree nodes | O(n) | One node per unique filename |
| Path storage | O(n) | One path string per file across all nodes |
| Call stack (recursion) | O(log n) | Maximum recursion depth = tree height |
| **Total** | **O(n)** | Linear in number of files |

> **Memory efficiency note (Assignment Section 4):**  
> Only the **filename** is used as the AVL key — not the full path. Full paths are stored once per file in the node's `vector<string> paths`. Parent directory names are **not duplicated** across nodes, addressing the memory constraint from the assignment.

---

## 8. Design Decisions

**Why AVL over plain BST?**  
File systems often store files alphabetically. Inserting sorted strings into a plain BST produces a degenerate right-skewed tree with O(n) search. AVL's self-balancing rotations prevent this, guaranteeing O(log n) in all cases.

**Why AVL over Heap?**  
Heaps are optimized for min/max extraction — they have no efficient mechanism for searching by key. Searching a heap requires O(n) traversal, which defeats the purpose of indexing.

**Why `vector<string>` for paths?**  
The assignment bonus requires listing all paths when multiple files share the same name. A vector allows appending additional paths to an existing node during insert without creating duplicate nodes.

**Why `node*` returned from `search()` instead of printing inside AVL?**  
Separation of concerns — the AVL class handles data, `main.cpp` handles UI. This makes `AVL.h` reusable in any project without ConsoleUtils dependency.

---

## 9. Known Limitations

- **Windows only** — ConsoleUtils uses `<windows.h>` and ANSI codes that require Windows 10+
- **Filenames with spaces** — `cin >> query` splits on spaces; filenames containing spaces require modification to use `getline`
- **Case sensitive** — `"Report.pdf"` and `"report.pdf"` are treated as different files
- **No persistence** — index is rebuilt from scratch every run; no disk caching
- **Static refresh** — refresh is manual via command; no automatic filesystem event listener

---

*TurboPath — Assignment 2 | Jawad Ahmed | NUST CEME*
