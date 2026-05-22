# Python - Gesture Recognition and Control Data
The module is designed as a standalone, stable, real‑time control data source, fully decoupled from the audio engine.
It acquires live video from a webcam, extracts hand‑related gestural features in real time, and streams them as normalized control data to the audio engine via OSC.

## Overview
- Input: webcam video stream
- Processing: hand tracking and gesture feature extraction
- Output: continuous OSC control signals (normalized, real‑time)

## Responsibilities
- Webcam video acquisition (OpenCV)
- Two‑hand tracking and handedness detection (MediaPipe Hands)
- Gesture feature computation and smoothing
- Real‑time OSC transmission

## Status
### Implemented Features

- Real‑time hand tracking from integrated or external USB webcam
- Simultaneous tracking of two hands
- Explicit left / right hand identification
- Visualization of hand landmarks and skeletons
- Continuous hand openness descriptor (independent for each hand)
- Separate thumb‑openness descriptor for fine control
- Stable execution at interactive frame rates on Windows

### Environment Setup (Conda/Python)
The Python subsystem is designed to run inside a dedicated Conda environment to ensure dependency consistency and reproducibility.
An environment description is provided in the form of a .yml file.

#### Create the Conda Environment
To create the environment from the provided YAML file:
`conda env create -f environment.yml`

#### Activate the Environment
`conda activate envname`

Replace `envname` with the actual environment name specified in the YAML file, default is `cmls`.

#### Run the Application
Once the environment is activated, navigate to the Python module directory and run:

`python main.py`

If the setup is correct, a webcam window with hand landmarks should appear and OSC messages will be transmitted in real time.

### OSC Interface
The Python module exposes a fixed OSC control interface.
#### OSC Addresses

- `/hand/left/open`: Continuous float value in range [0.0 – 1.0]
Represents the openness of the left hand

- `/hand/right/open`: Continuous float value in range [0.0 – 1.0]
Represents the openness of the right hand

- `/hand/left/thumb`: Continuous float value in range [0.0 – 1.0]
Represents the openness of the left thumb

- `/hand/right/thumb`: Continuous float value in range [0.0 – 1.0]
Represents the openness of the right thumb

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
    - weighted fingertip-to-palm distances
    - normalization and calibration logic

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