# Reestruct Library

A comprehensive C library providing various data structure implementations with generic type support.

---

## 🚀 Features

-   Generic data structures using `void*` (type-agnostic)
-   Memory leak prevention and safety checks
-   Consistent API design across all structures
-   Support for both static and dynamic library builds

## 📚 Data Structures

| Structure          | Description                              | Header         |
| ------------------ | ---------------------------------------- | -------------- |
| Singly Linked List | Linear collection with forward traversal | `linkedlist.h` |
| Doubly Linked List | Bi-directional linear collection         | `linkedlist.h` |
| Stack              | LIFO data structure                      | `stack.h`      |
| Queue              | FIFO data structure                      | `queue.h`      |
| Deque              | Double-ended queue                       | `deque.h`      |
| Binary Tree        | Tree with max 2 children per node        | `binarytree.h` |

### ✅ Implemented Data Structures

-   Singly Linked List
-   Doubly Linked List
-   Stack
-   Queue
-   Deque
-   Binary Tree _(partial)_

---

## 🧠 Usage

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
    printf("[LOG] %d ", *(int*)data);
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
