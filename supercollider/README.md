# SuperCollider – Sound Synthesis Engine
This module implements the sound synthesis and audio processing engine of the project.
SuperCollider is responsible for generating sound in real time and mapping incoming
control data to synthesis and effects parameters.
The engine receives OSC messages from the middleware layer and uses them to shape
timbre, dynamics, modulation, and audio effects.
## Responsibilities

- Definition of synthesizers (SynthDefs)
- Audio effects and sound processing
- Mapping of control parameters to musical variables
- Real-time audio generation
- OSC-based control integration

## Current Implementation Status
### Keyboard and Sustain Pedal
The system has been successfully tested with a MIDI keyboard connected to SuperCollider.
Both note triggering and sustain pedal functionality are fully operational, allowing for
basic expressive performance and note articulation.
### OSC Control and Gesture Mapping
OSC communication has been integrated and tested successfully.
In particular:

- The opening and closing of the left hand is mapped to the cutoff frequency of the synthesizer filter.
- After several iterations and refinements, this control has proven to be stable and responsive, enabling real-time timbral modulation through hand gestures.

### Ongoing Work: Second Effect Control
Current development is focused on adding control for a second audio effect, which may be one of the following:

- Reverb
- Chorus
- Flanger
- Wah-wah

The selected effect will be controlled via the movement of the left thumb, captured through the same gesture-tracking system.
Several tests will be conducted to determine which effect is most musically effective and best suited to gesture-based control, as not all effects may produce satisfying results in this context.
### Future Development
Planned next steps include:

- Extending gesture controls to the right hand, enabling additional parameters or effects to be manipulated.
- Refining gesture-to-parameter mappings for improved expressiveness and musical consistency.

At the moment, development is intentionally focused on the left hand, in order to stabilize and validate the interaction model before scaling up the control complexity.
