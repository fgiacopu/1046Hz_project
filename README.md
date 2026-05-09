# 1046Hz - CMLS project (a.y. 2025/2026)
Gesture‑controlled synthesizer project developed for the _Computer Music: Languages and Systems_ course (a.y. 2025-2026).

## Purpose of the project
This project aims to design and prototype an expressive gesture‑controlled digital musical instrument that enables real‑time sound interaction without physical contact. The system explores how hand movements, captured through a standard webcam and analyzed via computer‑vision techniques, can be mapped to sound synthesis and audio effects parameters.
The core idea is to combine gesture recognition, real‑time parameter control, and digital sound synthesis into a modular and extensible system. Hand gestures are translated into continuous control data, which are then used to shape timbre, dynamics, and effects in a sound engine. This approach emphasizes expressivity, physicality, and experimentation beyond traditional controllers such as keyboards or MIDI devices.
The project integrates multiple technologies commonly used in computer music systems:

## System Overview
The system is composed of multiple modular components that communicate in real time:

- Python is used for real-time gesture analysis and data processing.
- A JUCE-based plugin acts as an interface and middleware layer.
- SuperCollider is used as the main sound synthesis and audio processing engine.

An optional hardware extension may be explored to incorporate simple physiological sensing (e.g. heart rate) as an additional expressive control dimension. This extension is considered secondary to the main vision-based interaction and may not be included in the final version.

Overall, the system is designed to support real-time responsiveness, modular development, and a clear separation between sensing, control, and sound generation.

## Technologies Used
- Python (MediaPipe, OSC communication)
- JUCE (C++ audio plugin framework)
- SuperCollider (real-time sound synthesis)
- OSC (Open Sound Control protocol)
- Optional: Arduino (physiological sensing extension)

## Development Status
This repository contains a working prototype of the system.

- Core architecture is defined.
- Gesture tracking, OSC communication, and sound synthesis are functional.
- System startup and component orchestration are stable.
- Further refinements and expressive mappings are ongoing

## How to Run
1. Ensure dependencies are installed:
   - Python environment with required libraries (see `python/environment.yml`)
   - SuperCollider
   - JUCE application built for your system

2. Launch the system: `start_system.bat`

The full system is launched using a system‑level batch script to ensure a
deterministic startup order and reliable MIDI handling on Windows.

Each module of the system is designed to function independently in standalone mode:
- The Python gesture‑tracking engine can be executed on its own for development and testing of computer‑vision algorithms.
- The JUCE application can run standalone as a graphical control interface, even in the absence of the Python backend.
- The SuperCollider sound engine can be launched independently for sound design, synthesis development, and MIDI testing.

### Startup sequence:
1. SuperCollider is launched first to ensure exclusive MIDI access.
2. The JUCE application is launched.
3. JUCE automatically starts the Python gesture‑tracking engine as a headless background process.

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