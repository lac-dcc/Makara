<p align="center">
  <img alt="Project logo" src="./assets/images/Banner.png" width="100%" height="auto"/></br>
</p>

## Goals

The MAKARA project (Multi-Architecture Knowledge Analysis and Report Assessment) is designed to rigorously test the depth of understanding possessed by Large Language Models (LLMs) regarding the foundational principles of computer architecture. The primary goal is to determine if an LLM can effectively translate program execution behavior across different hardware environments. Specifically, the project challenges an LLM with a given program's source code and its corresponding performance counter metrics gathered from an initial architecture (Architecture A). The central objective is then to query the LLM to predict and project how those same performance counters would appear if the identical program were executed on a distinct target architecture (Architecture B). This cross-domain performance projection serves as a crucial metric for assessing the LLM's true architectural reasoning capabilities, moving beyond mere data pattern recognition to validate its grasp of the underlying hardware-software interaction principles.

---

## **Help Us Build a Global Performance Dataset!**

We're building a collective dataset of **performance counters** and need your help! This dataset is a table that associates programs with their counters on particular architectures. By contributing, you'll help us create a robust resource for researchers worldwide. Besides, you will have access to the entire dataset!

**Contributing is Easy:** We've created an automated script that handles everything:

* It **downloads and compiles** our benchmark collection ([Makara/Jotai](https://github.com/lac-dcc/Makara/tree/main/Jotai)).
* It **runs** benchmarks and efficiently **collects** performance counter values.
* It **captures** system architecture details (CPU, caches, etc.).
* It automatically **packages** all the results for easy submission.

**Ready to contribute?** Just run the script (see instructions below) and send your data via this [Google Form](https://docs.google.com/forms/d/1ADHlQ2LgZQ-MuuXVd_LyDQ2UDm77Bx1kBDIXt9bX81U/preview). Running the script takes about five-six minutes on a standard machine.

---

## Overview

The Makara Data Pipeline automates the process of compiling, executing, and profiling programs using `perf`, aggregating the results into a structured dataset.  
It supports distributed execution, making it suitable for large-scale performance analysis or research in compiler optimization and program behavior characterization.

---

## Dependencies

Ensure the following dependencies are installed on all machines participating in data collection:

- **GCC** – for compiling C/C++ programs
- **Python 3.x** – for orchestration and automation
- **Linux perf** – for collecting performance metrics

---

## Installation

You can install the dependencies using your package manager. For Ubuntu/Debian-based systems:

```bash
sudo apt update
sudo apt install build-essential python3 linux-tools-common linux-tools-$(uname -r)
```

_(If `perf` is not found after installation, you may need to create a symbolic link to `/usr/bin/perf`.)_

You can test if perf is installed with

```bash
perf --version
```

To correctly use perf without restrictions, please run the following command:

```bash
sudo sysctl -w kernel.perf_event_paranoid=1     
```

---

## ▶️ How to Run

To start the data collection process, simply execute:

```bash
python3 collect_data.py
```
*Note*: On a typical machine, such as an Intel i7 at 2.8 GHz, this script takes 5 up to 6 minutes to run.

The Makara Data Pipeline will:

1. Compile all source programs;
2. Execute each one while recording performance statistics with `perf`;
3. Store the results in a structured `results/` directory;
4. Automatically compress the collected data into a `.zip` archive for easy sharing.

After create de `.zip` folder, please submit your results in the form: https://docs.google.com/forms/d/1ADHlQ2LgZQ-MuuXVd_LyDQ2UDm77Bx1kBDIXt9bX81U/preview.

---
