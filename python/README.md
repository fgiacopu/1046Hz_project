# Python - Gesture Recognition and Control Data
This module handles real-time gesture analysis using computer vision techniques.
A standard webcam is used to capture video frames, which are processed to extract
hand-related features such as position, movement, and basic shape descriptors.

The extracted features are normalized and streamed as continuous control parameters
to the other system components using OSC (Open Sound Control).

## Responsibilities
- Webcam video acquisition
- Hand and gesture feature extraction
- Basic filtering and normalization
- OSC data transmission

## Status
Early development stage. The focus is on building a stable gesture-to-parameter
pipeline before adding more complex gesture features.