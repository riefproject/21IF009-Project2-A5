# Reestruct Library

A comprehensive C library providing various data structure implementations with generic type support.

---

## 🚀 Features

-   Generic data structures using `void*` (type-agnostic)
-   Memory leak prevention and safety checks
-   Consistent API design across all structures
-   Support for both static and dynamic library builds
-   Ordered & unordered associative containers (map/set/umap/uset)
-   Ready-to-ship release layout (`release/include` + `release/lib`)

## 📚 Data Structures

| Structure          | Description                              | Header         |
| ------------------ | ---------------------------------------- | -------------- |
| Singly Linked List | Linear collection with forward traversal | `linkedlist.h` |
| Doubly Linked List | Bi-directional linear collection         | `linkedlist.h` |
| Stack              | LIFO data structure                      | `stack.h`      |
| Queue              | FIFO data structure                      | `queue.h`      |
| Deque              | Double-ended queue                       | `deque.h`      |
| Binary Tree        | Tree with max 2 children per node        | `binarytree.h` |
| Heap               | Binary heap (min-heap via comparator)    | `heap.h`       |
| Map                | Ordered map (BST + comparator)           | `map.h`        |
| Set                | Ordered set (BST + comparator)           | `set.h`        |
| Unordered Map      | Hash map                                 | `umap.h`       |
| Unordered Set      | Hash set                                 | `uset.h`       |
| Graph              | Adjacency-list graph                     | `graph.h`      |

### ✅ Implemented Data Structures

-   Singly Linked List
-   Doubly Linked List
-   Stack
-   Queue
-   Deque
-   Binary Tree
-   Heap
-   Map / Set
-   Unordered Map / Unordered Set
-   Graph

---

## 🧠 Usage

### ⚡ Quick Start

```bash
make release
gcc main.c -Irelease/include -Lrelease/lib -lreestruct -o main
```

Place the built library (`release/lib`) alongside your binary or update `PATH`/`LD_LIBRARY_PATH` as needed.

### 🛠 One-command build (auto-detect OS)

```bash
bash scripts/build.sh             # defaults to release
bash scripts/build.sh static      # or shared / all / clean / test / docs
```

On Windows, run the script from Git Bash/MSYS2/WSL so `make` is available. The script sets `OS` for the Makefile automatically.

### ✅ Run unit tests

```bash
make test
# or
bash scripts/build.sh test
```

### 📖 Generate Doxygen docs

Requires `doxygen` installed:

```bash
make docs
# or
bash scripts/build.sh docs
```
Output: `docs/html/index.html`.

### 📥 Include Example

You can include all structures:

```c
#include "reestruct.h"
```

Atau untuk lebih efisien (khusus kebutuhan):

```c
#include "linkedlist.h"   // Only linked list
#include "stack.h"        // Only stack
#include "queue.h"        // Only queue
#include "deque.h"        // Only deque
#include "binarytree.h"   // Only binary tree
```

### 💡 Example Program

```c
#include "stack.h"
#include <stdio.h>

void printInt(void* data) {
    printf("%d ", *(int*)data);
}

int main() {
    Stack* stack = createStack();

    int nums[] = {1, 2, 3, 4, 5};
    for(int i = 0; i < 5; i++) {
        push(stack, &nums[i]);
    }

    printStack(stack, printInt);
    freeStack(stack);
    return 0;
}
```

### 🌳 Map / Set Example (ordered, comparator-based)

```c
#include "map.h"
#include <stdio.h>

int intCompare(const void* a, const void* b) {
    int da = *(const int*)a;
    int db = *(const int*)b;
    return (da > db) - (da < db);
}

void printPair(void* key, void* value) {
    printf("%d => %s\n", *(int*)key, (char*)value);
}

int main() {
    Map* map = createMap(intCompare);
    int key1 = 10, key2 = 5;
    mapPut(map, &key1, "ten");
    mapPut(map, &key2, "five");
    mapTraverseInOrder(map, printPair);
    freeMap(map);
}
```

### 🧭 Unordered Map / Set (hash-based)

```c
#include "umap.h"
#include <string.h>
#include <stdio.h>

size_t strHash(const void* key) {
    // djb2-ish
    size_t h = 5381;
    const unsigned char* p = (const unsigned char*)key;
    while (*p) h = ((h << 5) + h) + *p++;
    return h;
}

bool strEq(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b) == 0;
}

int main() {
    UMap* map = createUMap(strHash, strEq);
    umapPut(map, "hello", "world");
    printf("%s\n", (char*)umapGet(map, "hello"));
    freeUMap(map);
}
```

### 🧩 Linking

#### Static Library (`.a`)

```bash
gcc main.c -Irelease/include -Lrelease/lib -lreestruct -o main
```

#### Shared Library (`.dll` / `.so`)

**Windows:**

```bash
gcc main.c -Irelease/include -Lrelease/lib -lreestruct -o main.exe
# Make sure reestruct.dll is in the same folder or in PATH
```

**Linux:**

```bash
gcc main.c -Irelease/include -Lrelease/lib -lreestruct -o main
# Ensure libreestruct.so is in LD_LIBRARY_PATH
```

---

## 📦 Installation

1. Download the latest release from the [GitHub Releases Page](https://github.com/riefproject/reestruct/releases)
2. Extract the files to your project directory
3. Include the appropriate headers in your code

---

## ⚙️ Building

Gunakan `Makefile` yang disediakan:

```bash
# Build both static and shared libraries
make all

# Build only static library (.a)
make static

# Build only shared library (.dll/.so)
make shared

# Generate release folder
make release

# Clean build files
make clean
```

---

## 📚 Release Structure

```
release/
├── include/
│   ├── reestruct.h
│   ├── stack.h
│   ├── queue.h
│   ├── linkedlist.h
│   └── ...
└── lib/
    ├── libreestruct.a
    └── reestruct.dll / libreestruct.so
```

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

## 👤 Author

**riefproject** – Author & Maintainer
