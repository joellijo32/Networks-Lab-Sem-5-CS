# Networks-Lab-Sem-5-CS

Computer Networks Lab coursework for Semester 5 (CS). This repository contains lab cycle solutions and manuals for the networking lab.

## Repository Structure

```
Networks-Lab-Sem-5-CS/
├── Lab_Cycle_Solutions/ 
├── references/
└── Networks_Lab_Cycle.pdf
```

- **Lab_Cycle_Solutions/** — Solved lab exercises.
- **references/** — Reference documents 
- **Networks_Lab_Cycle.pdf** — The lab cycle questions document.

## Getting Started

### Prerequisites

- A Linux environment (native, VM, or WSL) with standard networking utilities installed (`ifconfig`/`ip`, `ping`, `netstat`, `traceroute`, etc.)

### Usage

1. Clone the repository:
   ```bash
   git clone https://github.com/joellijo32/Networks-Lab-Sem-5-CS.git
   cd Networks-Lab-Sem-5-CS
   ```

2. Open `Networks Lab Cycle.pdf` to view the lab assignment questions.

3. Refer to the solution files under `Lab_Cycle_Solutions/` for each topic.

4. For Questions 5 - 9, compile both client and server files (eg: `Q5_Server.c` & `Q5_Client.c`) and run `server` object in terminal 1 and `client` object in terminal 2:

    ```bash
    cd Lab_Cycle_Solutions/<question_no.>_<name>
    gcc Qx_Server.c -o server ; gcc Qx_Client.c -o client
    ```
    (where x is the question number)
   
    Terminal 1:
    ```bash
    ./server
    ```
    Terminal 2:
    ```bash
    ./client
    ```

Made by Joel Lijo Mathew.
