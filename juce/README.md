# JUCE - Plugin Interface and Middleware
This module contains a JUCE-based audio plugin that acts as an interface and middleware
between the gesture analysis system and the sound synthesis engine.

The plugin is responsible for receiving OSC control data, smoothing and scaling
parameters, providing a graphical user interface, and optionally applying audio effects.

## Responsibilities
- OSC reception from external processes
- Parameter smoothing and routing
- Graphical user interface
- Plugin integration (VST/AU)

## Status
Skeleton project setup. Core communication and basic UI elements will be implemented
before adding advanced features.
