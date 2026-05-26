# 1046Hz - CMLS project (a.y. 2025/2026)
Gesture‑controlled synthesizer project developed for the _Computer Music: Languages and Systems_ course (a.y. 2025-2026).

## Purpose of the project
This project aims to design and prototype an expressive gesture‑controlled digital musical instrument that enables real‑time sound interaction without physical contact. The system explores how hand movements, captured through a standard webcam and analyzed via computer‑vision techniques, can be mapped to sound synthesis and audio effects parameters.
The core idea is to combine gesture recognition, real‑time parameter control, and digital sound synthesis into a modular and extensible system. Hand gestures are translated into continuous control data, which are then used to shape timbre, dynamics, and effects in a sound engine. This approach emphasizes expressivity, physicality, and experimentation beyond traditional controllers such as keyboards or MIDI devices.
The project integrates multiple technologies commonly used in computer music systems:

## System Overview

The system is composed of multiple modular components that communicate in real time:

- Python is used for real-time gesture analysis and data processing.
- A JUCE-based application acts as an interface and middleware layer, enabling user control and forwarding parameters between modules.
- SuperCollider is used as the main sound synthesis and audio processing engine.
- Processing is used as a graphical feedback unit, delivering an intuitive and dynamic visual representation of the interaction and sound processes.

Overall, the system is designed to support real-time responsiveness, modular development, and a clear separation between sensing (Python), control (JUCE), sound generation (SuperCollider), and visual feedback (Processing).


## Technologies Used
- Python (MediaPipe, OSC communication)
- JUCE (C++ GUI Audio Application)
- SuperCollider (real-time sound synthesis)
- OSC (Open Sound Control protocol)
- Processing (real-time graphical visualisation and visual feedback)

## Project Outcome
This repository presents a fully functional prototype of a gesture-controlled musical instrument for real-time interaction.

- The system integrates gesture tracking, control interface, sound synthesis, and visual feedback into a coherent pipeline.
- Real-time communication between modules is stable and reliably managed through OSC.
- The JUCE interface supports both manual interaction and gesture-based control.
- The SuperCollider engine enables dynamic sound synthesis and audio processing.
- The Processing module provides continuous visual feedback linked to both gesture data and audio parameters.
- The system can be launched reliably through a structured startup sequence, ensuring correct initialization of all components.

The project demonstrates a complete and modular interactive system, combining sensing, control, sound generation, and visualisation into a unified real-time experience.



## System Architecture and Data Flow

The system operates through real-time communication between independent modules:

- Gesture data is captured via webcam and processed in Python using MediaPipe.
- The extracted parameters (e.g. hand openness, thumb position) are sent via OSC.
- JUCE receives and optionally modifies control data through its GUI interface.
- SuperCollider receives control messages and performs sound synthesis and processing.
- Relevant audio or control parameters are sent to Processing via OSC to generate visual feedback.

This modular architecture enables flexible communication and clear separation between sensing, control, audio, and visualisation layers.

## How to Run
1. Ensure dependencies are installed:
   - Python environment with required libraries (see `python/environment.yml`)
   - SuperCollider
   - JUCE application built for your system
   - Processing (for real-time visualisation)

2. Launch the system: `start_system.bat`

The full system is launched using a system‑level batch script to ensure a
deterministic startup order and reliable MIDI handling on Windows.

Each module of the system is designed to function independently in standalone mode:
- The Python gesture‑tracking engine can be executed on its own for development and testing of computer‑vision algorithms.
- The JUCE application can run standalone as a graphical control interface, even in the absence of the Python backend.
- The SuperCollider sound engine can be launched independently for sound design, synthesis development, and MIDI testing.
- The Processing sketch can be run independently to test and develop visual feedback behaviours.

Make sure to manually run the Processing sketch to enable real-time graphical feedback during the system execution.

### Startup sequence:
1. SuperCollider is launched first to ensure exclusive MIDI access.
2. The JUCE application is launched.
3. JUCE automatically starts the Python gesture‑tracking engine as a headless background process.
4. The Processing sketch is started to display real-time visual feedback.

This approach guarantees both modular standalone operation and stable integrated
execution, favoring robustness, reproducibility, and clear separation of system
responsibilities.


## Group Members
Lorenzo Corna \
Anitha Sivasankar \
Tekla Gizella Kalmár \
Eleonora Berra \
Federico Giacopuzzi \
Antonio Treviglio

## Contributing

This project is developed collaboratively by multiple team members.
To ensure a clean and stable workflow, all contributors must follow the
guidelines below.

### Branching Strategy

- `main`   
    Contains only stable and verified versions of the project.  
    No direct commits or pushes are allowed.

- `development`  
    Acts as a shared integration branch where the different sections
    (Python, JUCE, SuperCollider) are combined and tested together.

- Section branches  
    Each main component is developed on a dedicated branch:
    - `python`
    - `juce`
    - `supercollider`

### Keeping Your Branch Updated
To integrate changes merged by others into development:
```bash
git checkout python
 git restore --source origin/development -- python/
```
This:
- updates your branch with the latest integrated work in the development
- does not affect `main`

### Workflow

1. Switch to your section branch:
```bash
git checkout python
```

2. Develop and commit changes:
```bash
git add .
git commit -m "feat: clear description of the change"
```

3. Push to your section branch:
```bash
git push
```

### Final Merge
When the entire project works correctly in development, open a Pull Request:\
base: `main` compare: `development`

Use `Squash and merge` to keep the `main` history clean and readable.