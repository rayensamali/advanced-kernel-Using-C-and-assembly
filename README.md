<p align="center">
  <img src="https://img.icons8.com/?size=512&id=55494&format=png" width="20%" alt="C-KERNEL-logo">
</p>
<p align="center">
    <h1 align="center">C-KERNEL</h1>
</p>
<p align="center">
    <em><code>❯ REPLACE-ME</code></em>
</p>
<p align="center">
	<img src="https://img.shields.io/github/license/rayensamali/C-kernel?style=flat&logo=opensourceinitiative&logoColor=white&color=0080ff" alt="license">
	<img src="https://img.shields.io/github/last-commit/rayensamali/C-kernel?style=flat&logo=git&logoColor=white&color=0080ff" alt="last-commit">
	<img src="https://img.shields.io/github/languages/top/rayensamali/C-kernel?style=flat&color=0080ff" alt="repo-top-language">
	<img src="https://img.shields.io/github/languages/count/rayensamali/C-kernel?style=flat&color=0080ff" alt="repo-language-count">
</p>
<p align="center">
		<em>Built with the tools and technologies:</em>
</p>
<p align="center">
	<img src="https://img.shields.io/badge/C-A8B9CC.svg?style=flat&logo=C&logoColor=black" alt="C">
</p>

<br>

##### 🔗 Table of Contents

- [📍 Overview](#-overview)
- [👾 Features](#-features)
- [📂 Repository Structure](#-repository-structure)
- [🧩 Modules](#-modules)
- [🚀 Getting Started](#-getting-started)
    - [🔖 Prerequisites](#-prerequisites)
    - [📦 Installation](#-installation)
    - [🤖 Usage](#-usage)
    - [🧪 Tests](#-tests)
- [📌 Project Roadmap](#-project-roadmap)
- [🤝 Contributing](#-contributing)
- [🙌 Acknowledgments](#-acknowledgments)

---

## 📍 Overview

<code>❯ This repository contains the source code for a basic custom operating system kernel named **TUN-OS**. This version of the kernel handles basic keyboard input, output to the screen, and manages interrupts, utilizing both C and assembly code.</code>

---

## 👾 Features

<code>❯ - **Basic Keyboard Input Handling**: Processes user input via the keyboard, including special keys like Enter and Backspace.
- **Screen Output**: Direct text output to the console using video memory.
- **Interrupt Descriptor Table (IDT) Setup**: Proper initialization of interrupt handling.
- **Programmable Interrupt Controllers (PIC) Initialization**: Setting up the PIC to manage hardware interrupts.
</code>

---

## 📂 Repository Structure

```sh
└── C-kernel/
    ├── Makefile
    ├── README.md
    ├── command_line.c
    ├── command_line.h
    ├── disk.img
    ├── disk_io.c
    ├── disk_io.h
    ├── file_system.c
    ├── file_system.h
    ├── io.c
    ├── io.h
    ├── kernel-1
    ├── kernel.asm
    ├── kernel.c
    ├── kernel.h
    ├── kernel.o
    ├── keyboard_map.h
    ├── link.ld
    ├── stringlib.c
    └── stringlib.h
```

---



## 🚀 Getting Started

### 🔖 Prerequisites

**C**: `version x.y.z`

### 📦 Installation

Build the project from source:

1. Clone the C-kernel repository:
```sh
❯ git clone https://github.com/rayensamali/C-kernel
```

2. Navigate to the project directory:
```sh
❯ cd C-kernel
```
### 🤖 Usage

To run the project, execute the following command:

```sh
❯ make;make run
```

---

## 📌 Project Roadmap

- [X] **`Task 1`**: <strike>create the keyboard driver</strike>
- [X] **`Task 2`**: <strike> create a home page and command line.</strike>
- [X] **`Task 3`**: <strike> create a file system.</strike>
- [] **`Task 3`**: <strike> Memory management.</strike>

---

## 🤝 Contributing

Contributions are welcome! Here are several ways you can contribute:

- **[Report Issues](https://github.com/rayensamali/C-kernel/issues)**: Submit bugs found or log feature requests for the `C-kernel` project.
- **[Submit Pull Requests](https://github.com/rayensamali/C-kernel/blob/main/CONTRIBUTING.md)**: Review open PRs, and submit your own PRs.
- **[Join the Discussions](https://github.com/rayensamali/C-kernel/discussions)**: Share your insights, provide feedback, or ask questions.

<details closed>
<summary>Contributing Guidelines</summary>

1. **Fork the Repository**: Start by forking the project repository to your github account.
2. **Clone Locally**: Clone the forked repository to your local machine using a git client.
   ```sh
   git clone https://github.com/rayensamali/C-kernel
   ```
3. **Create a New Branch**: Always work on a new branch, giving it a descriptive name.
   ```sh
   git checkout -b new-feature-x
   ```
4. **Make Your Changes**: Develop and test your changes locally.
5. **Commit Your Changes**: Commit with a clear message describing your updates.
   ```sh
   git commit -m 'Implemented new feature x.'
   ```
6. **Push to github**: Push the changes to your forked repository.
   ```sh
   git push origin new-feature-x
   ```
7. **Submit a Pull Request**: Create a PR against the original project repository. Clearly describe the changes and their motivations.
8. **Review**: Once your PR is reviewed and approved, it will be merged into the main branch. Congratulations on your contribution!
</details>

<details closed>
<summary>Contributor Graph</summary>
<br>
<p align="left">
   <a href="https://github.com{/rayensamali/C-kernel/}graphs/contributors">
      <img src="https://contrib.rocks/image?repo=rayensamali/C-kernel">
   </a>
</p>
</details>

---


## 🙌 Acknowledgments

- List any resources, contributors, inspiration, etc. here.

---
