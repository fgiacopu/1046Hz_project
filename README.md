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
This repository currently contains an early skeleton of the project.
The system architecture and component responsibilities are defined, while individual modules are under active development.

## How to Run
Setup instructions will be added in a later development stage.

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

- Feature branches (optional)  
    Used only for medium to large features or experiments, e.g.:
    - `py-osc-routing`
    - `sc-synth-engine`
    - `juce-gui-layout`

### Keeping Your Branch Updated
To integrate changes merged by others into development:
```bash
git checkout python
git pull origin development
```
This:
- updates your branch with the latest integrated work
- does not change the upstream of your branch
- does not affect `main`

### Workflow
#### Daily Development

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

#### Feature Branch (optional)
1. Create and switch to a feature branch:
```bash
git checkout python
git checkout -b py-feature-name
```
2. Develop and commit changes:
```bash
git add .
git commit -m "feat: implement feature description"
```
3. Push the feature branch:
```bash
git push -u origin py-feature-name
```
4. Open a Pull Request:

    base: `python` compare: `py-feature-name`

### Integrating Sections
When a section is stable and locally tested, open a Pull Request:

base: `development` compare: `supercollider`\
base: `development` compare: `juce`\
base: `development` compare: `python`

All integrations must happen via Pull Request.

### Final Merge
When the entire project works correctly in development, open a Pull Request:\
base: `main` compare: `development`

Use `Squash and merge` to keep the `main` history clean and readable.

### Commit Guidelines

Keep commits focused and meaningful.
One commit should represent one logical change.
Use clear commit messages (e.g. feat: add OSC mapping).
Do not commit build artifacts, generated files, or temporary data.

### Coordination

Changes affecting shared interfaces (e.g. OSC messages, parameter names,
data formats, or ranges) must be discussed with the team before being
merged into `development` and later into `main`.