# TurboPath — AVL-Powered Path Finder
**Assignment 2 | DS&A Lab | NUST CEME**

---

## What is TurboPath?
A high-speed file search utility that pre-indexes an entire directory tree into an AVL tree in RAM, enabling **O(log n)** file lookups without touching the disk after startup.

---

## Files Included

```
TurboPath/
├── main.cpp          ← Entry point: UI, indexing, search loop
├── AVL.cpp           ← AVL data structure (node + class)
├── ConsoleUtils.h    ← Console color/cursor declarations
├── ConsoleUtils.cpp  ← Console color/cursor implementations
└── .vscode/
    ├── tasks.json    ← Build configuration
    └── launch.json   ← Run configuration (external terminal)
```

---

## Requirements
- **OS:** Windows 10 or later
- **Compiler:** g++ (MinGW64) or MSVC
- **C++ Standard:** C++17

---

## How to Compile & Run

### Option A — VS Code
1. Open the project folder in VS Code
2. Press **`Ctrl+Shift+B`** to build
3. Press **`F5`** to run

> ⚠️ Click **"Debug Anyway"** if prompted. The program opens in an external `cmd.exe` window — do not use the integrated terminal as colors will not render correctly.

### Option B — Command Line (cmd.exe)
Navigate to the project folder and run:
```bat
g++ main.cpp ConsoleUtils.cpp -o TurboPath.exe -std=c++17
TurboPath.exe
```

### Option C — Visual Studio 2022
1. Create a new **Empty C++ Project**
2. Add all `.cpp` and `.h` files
3. Go to **Project Properties → C/C++ → Language → C++ Language Standard → ISO C++17**
4. Press **Ctrl+F5** to build and run

---

## How to Use

**Step 1 — Enter the directory path to index:**
```
Enter root directory path to index:
  >> D:\DataVault
```

**Step 2 — Search for any file:**
```
  Search >> B1.pdf

  [+] Found 2 match(es) for "B1.pdf":
      [1] D:\DataVault\Clients\2023\Q4\report.pdf
      [2] D:\DataVault\Archive\2022\report.pdf
```

**Available commands:**

| Command | Action |
|---|---|
| `filename.ext` | Search for a file by name |
| `refresh` | Re-index the directory without restarting |
| `exit` | Quit the program |

---

*Jawad Ahmed | NUST CEME*
