# Python - Gesture Recognition and Control Data
This module implements the gesture recognition subsystem of the project.
It acquires live video from a webcam, extracts hand‑related gestural features in real time, and streams them as normalized control data to the audio engine via OSC.

## Overview
- Input: webcam video stream
- Processing: hand tracking and gesture feature extraction
- Output: continuous OSC control signals (normalized, real‑time)

The module is designed as a standalone and stable data source, decoupled from sound design and audio processing.

## Responsibilities
- Webcam video acquisition (OpenCV)
- Two‑hand tracking and handedness detection (MediaPipe Hands)
- Gesture feature computation and smoothing
- Real‑time OSC transmission

## Status
### Implemented Features (Tested)

- Real‑time hand tracking from integrated or external USB webcam
- Simultaneous tracking of two hands
- Explicit left / right hand identification
- Visualization of hand landmarks and skeletons
- Continuous hand openness descriptor
- Stable execution at interactive frame rates on Windows

### OSC Interface
The Python module exposes a fixed OSC control interface.
#### OSC Addresses

- `/hand/left/open`: Continuous float value in range [0.0 – 1.0]
Represents the openness of the left hand

- `/hand/right/open`: Continuous float value in range [0.0 – 1.0]
Represents the openness of the right hand

### Code Structure
- `main.py`:
Entry point and runtime loop.
Coordinates video capture, gesture processing, smoothing, and OSC output.
This is where OSC messages are sent once per frame.

- `hand_tracker.py`:
Computer vision core.
Handles webcam access, MediaPipe integration, two‑hand detection, handedness classification, and landmark visualization.
Main optimizations for stability and frame rate are implemented here.

- `gesture_features.py`:
Gesture descriptor definitions.
Implements the hand openness feature using:

    - a virtual palm center
weighted fingertip‑to‑palm distances
    - Includes normalization and calibration logic.

- `smoothing.py`:
Signal conditioning.
Applies low‑pass filtering to continuous gesture values before OSC transmission.

- `osc_sender.py`:
OSC communication layer.
Sends normalized control values over UDP to external software.
No gesture logic is implemented here.

- `config.py`:
Runtime configuration.
Defines camera index, OSC network parameters, and optional hardware flags.

- `arduino_input.py`:
Optional extension (not used in current tests).
Reserved for future Arduino‑based sensors.

## Future Extensions
The following features are not implemented in the current version but represent
conceptual extensions identified during development and testing

### Thumb‑based Articulation Trigger
The thumb can be processed independently from hand openness to act as a discrete
gesture trigger. A deliberate opposition movement of the thumb could be mapped to
articulation changes (e.g. increased percussiveness or attack), providing expressive
control without interfering with continuous parameters.

### Gesture‑driven Vibrato
A continuous vibrato effect could be obtained by detecting fast and sustained hand
movements over time. Unlike static control, this approach would require intentional
physical effort and velocity thresholds to avoid unintentional activation.

### Additional Physiological Inputs
Optional sensor data (e.g. heart rate via Arduino) could be integrated as an
independent control layer, enabling bio‑feedback or tempo‑related modulation.