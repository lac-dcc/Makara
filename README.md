<p align="center">
  <img alt="Project logo" src="./assets/images/Banner.png" width="100%" height="auto"/></br>
</p>

## Goals

The MAKARA project (Multi-Architecture Knowledge Analysis and Report Assessment) tests how deeply Large Language Models (LLMs) understand the foundational principles of computer architecture.

The core goal is to determine if an LLM can accurately predict performance behavior across different hardware. We challenge an LLM with a program's source code and its performance counter metrics from an initial architecture (Architecture A). The central objective is to query the LLM to predict how those same performance counters would appear if the identical program were executed on a distinct target architecture (Architecture B).

This cross-domain performance projection serves as a crucial metric for assessing an LLM's true architectural reasoning, moving beyond pattern recognition to validate its grasp of hardware-software interaction.

---

## **Help Us Build a Global Performance Dataset!**

We are building a comprehensive dataset of **performance counters**, and we need your help! This dataset will map programs to their performance metrics on specific hardware.

By contributing, you'll help create a robust, open resource for researchers worldwide. As a collaborator, **you will receive full access to the entire dataset!**

**Contributing is Easy:** We've created an automated script that handles everything:

- It **downloads and compiles** our benchmark collection ([Makara/Jotai](https://github.com/lac-dcc/Makara/tree/main/Jotai)).
- It **runs** the benchmarks and efficiently **collects** performance counter values.
- It **captures** system architecture details (CPU, caches, etc.).
- It automatically **packages** all the results for easy submission.

**Ready to contribute?** Just run the script (see instructions below) and send your data via this [Google Form](https://forms.gle/7tL9eBhGUPJMRt6x6). Running the script takes about **five to six minutes** on a standard machine.

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

## Configuration

To allow perf to collect data without restrictions, please run the following one-time command:

```bash
sudo sysctl -w kernel.perf_event_paranoid=1
```

---

## ▶️ How to Run

To start the data collection process, simply execute:

```bash
python3 collect_data.py
```

_Note_: On a typical machine, such as an Intel i7 at 2.8 GHz, this script takes 5 to 6 minutes to run.

The Makara Data Pipeline will:

1. Compile all source programs;
2. Execute each one while recording performance statistics with `perf`;
3. Store the results in a structured `results/` directory;
4. Automatically compress the collected data into a `.zip` archive for easy sharing.

Once the `.zip` file is created, please submit your results via the [Makara - Results Submission](forms.gle/7tL9eBhGUPJMRt6x6) form.

---
