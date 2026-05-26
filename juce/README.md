# JUCE - Gesture Interface and OSC Middleware
This module contains a JUCE-based audio plugin that acts as an interface and middleware
between the gesture analysis system and the sound synthesis engine.

The plugin is responsible for receiving OSC control data, smoothing and scaling
parameters, providing a graphical user interface, and optionally applying audio effects.

## Responsibilities
- OSC reception from external processes
- Parameter smoothing and routing
- Graphical user interface
- Plugin integration (VST/AU)

## Implemented Features

- OSC message reception from Python module
- Real‑time parsing of continuous gestural descriptors
- Graphical visualization of gesture parameters using rotary sliders for the left and right hand openness, and linear sliders for left and right thumb movements
- Symmetric dual-target OSC forwarding: Concurrently syncs parameters to the SuperCollider sound engine and the Processing visualization engine
- Allowing for manual interaction when a camera is not connected.
- Receiving default ADSR envelope parameters from SuperCollider upon initialization and automatically mirroring those updates out to Processing to keep them in sync
- Enabling, disabling selectively the incoming biomechanical data streams via a 2x2 grid of GUI toggle switches
- Standalone GUI application for testing and debugging


## OSC Interface
### Inputs (listening on port 9000)
#### From Python (Biomechanical Data):
Continuous float value in range [0.0 – 1.0]

- `/hand/left/open`: Represents the openness of the left hand (mapped to low pass filter cutoff: 200 Hz - 8000 Hz exponentially)

- `/hand/right/open`: Represents the openness of the right hand (mapped to chorus percentage)

- `/hand/left/thumb`: Represents the movement of the left thumb (mapped to flanger percentage)

- `/hand/right/thumb`: Represents the movement of the right thumb (mapped to distortion percentage)

#### From SuperCollider (Envelope Defaults):
Float values to synchronize GUI on startup. When received, these values update the UI layout on the main message thread and are immediately broadcast out to Processing.
- /attack
- /decay
- /sustaain
- /releasee

### Outputs (Forwarding Streams)
#### To SuperCollider (Port 57130)
- Direct forwarding of the four /hand/... biomechanical parameters (if the corresponding UI toggle switch is active)
- Real-time forwarding of manual UI adjustments to /attack, /decay, /sustaain, and /releasee values

#### To Processing (Port 9001)
- Symmetric duplication of all outgoing /hand/... biomechanical parameters (gated by the toggle grid).
- Real-time, thread-safe mirroring of /attack, /decay, /sustaain, and /releasee values (triggered by either direct UI manipulation or incoming SuperCollider sync changes).


## Functional Description
The JUCE module performs three main operations:

### 1. Data Reception
- Listens for OSC messages on port `9000`
- Receives real‑time gesture data from Python and initialization data from SuperCollider
- Operates asynchronously with respect to the GUI and audio threads

### 2. Control Processing and Visualization
- Extracts and validates incoming float values  
- Updates GUI elements (rotary knobs for hands, linear sliders for thumbs, rotary knobs for ADSR) to reflect the current state safely on the main thread  
- Maps raw [0.0 - 1.0] floats to user-friendly text representations (e.g., exponential Hz calculations for the LPF cutoff knob, percentage labels for modulations)

### 3. Parameter Forwarding
- Checks the state of the biomechanical toggle switches
- If a channel is active, the value is forwarded via OSC to SuperCollider (127.0.0.1:57130) and Processing (127.0.0.1:9001); if a toggle is disabled, incoming stream updates are blocked, and an explicit 0.0f value is transmitted instead to safely rest the external engines

## Execution

The JUCE module is implemented as a standalone GUI application.

To run the application:

1. Open the `.jucer` file in Projucer  
2. Export the project to your chosen IDE (e.g. Visual Studio)  
3. Build and run the application  

Once running:
- Smart Window Sizing: The application boots into a custom-configured MainWindow geometry. It dynamically calculates your primary display's userArea bounds to stretch the window vertically from the absolute top to the absolute bottom of the screen (respecting native OS taskbars and docks), while locking the width to its intentional 1000-pixel footprint and centering itself horizontally.
- The GUI displays high-contrast control segments (Blue for Left Hand, Green for Right Hand, Purple for ADSR).
- Users can toggle inputs off via the 2x2 center grid to bypass camera control and tweak synthesis parameters manually.

## Code Structure

- `Main.cpp`  
  Acts as the application entry point. Implements GUIApplication and its embedded MainWindow class:
  Initializes the JUCE framework application lifecycle.
  Interrogates juce::Desktop::getInstance().getDisplays() to calculate available desktop heights.
  Dynamically calculates coordinates to lock window width while filling out the screen space vertically all the way to the bottom.
  Instantiates the MainComponent.

- `MainComponent.h`  
  Defines the primary component class layout, declaring OSCReceiver interfaces, distinct OSCSender objects targeting SuperCollider and Processing independently, labels, buttons, and slider components.

- `MainComponent.cpp`  
  Implements the core logic: UI color styling, dynamic area partitioning layouts inside resized(), and incoming network filtering.


## Implementation Details

The core logic is implemented inside the `MainComponent` class.

### OSC Reception

The system uses `juce::OSCReceiver` to listen on port `9000`.

Incoming messages are handled through:
```cpp
void oscMessageReceived(const juce::OSCMessage& message)
```
This callback is automatically invoked when a message arrives.
To ensure only relevant gestures and data are handled, the system explicitly filters for specific address patterns.

From Python (Biomechanical data):

/hand/left/open
/hand/right/open
/hand/left/thumb
/hand/right/thumb

From SuperCollider (Envelope deafults):

/attack
/decay
/sustaain
/releasee


### Message Validation
Before accessing the data, the system performs validation checks:

```cpp
if (message.size() > 0 && message[0].isFloat32())
```
This ensures:
- the message contains data
- the data is of the expected type (float)

This improves system robustness and prevents runtime errors.


### Data Extraction
The continuous gesture values are extracted as follows:

```cpp
float val = message[0].getFloat32();
```
These values are normalized in the range [0.0 – 1.0] by the Python module, which allows for direct mapping to synthesis parameters.


### GUI Initialization and Configuration
The graphical interface is initialized in the constructor.
Rotary sliders and linear sliders are used to visualize the gesture parameters:
```cpp
handKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
handKnob.setRange(0.0, 1.0);
```
The sliders are configured to intercept mouse clicks:
```cpp
handKnob.setInterceptsMouseClicks(true, false);
```
This hybrid approach ensures that the GUI provides visual feedback for the camera's gesture control, while still allowing the user to manually click and drag the parameters if they want to play the synth without a camera connected.

### Initialization Notification Trick
Because custom textFromValueFunction lambda strings are only evaluated when a value changes, the UI executes a synchronous fake notification on startup to force the labels to render text properly (e.g., showing "200 Hz" or "0 %" right away instead of empty spaces):

```cpp
leftHandKnob.setValue(0.0, juce::sendNotificationSync);
leftHandKnob.setValue(0.000001, juce::dontSendNotification);
leftHandKnob.setValue(0.0, juce::sendNotificationSync);
```

This hybrid configuration ensures that the GUI provides an immediate, user-friendly visualization layout while preserving manual click-and-drag interactions.

### Thread-Safe Architecture & Processing Sync
Because incoming network signals run asynchronously on a background network thread, the system isolates GUI component modification from network transmission to guarantee stability.

When external ADSR parameters update, they are scheduled cleanly onto the main thread execution block while keeping the visual rendering engine completely up to date, simultaneously mirroring the packet directly to Processing:

```cpp
if (pattern == "/attack" || pattern == "/decay" || pattern == "/sustaain" || pattern == "/releasee")
{
    juce::MessageManager::callAsync([this, val, pattern]() {
        processingSender.send(pattern, val);

        if (pattern == "/attack")
            attackKnob.setValue(val, juce::dontSendNotification);
        else if (pattern == "/decay")
            decayKnob.setValue(val, juce::dontSendNotification);
        else if (pattern == "/sustaain")
            sustainKnob.setValue(val, juce::dontSendNotification);
        else if (pattern == "/releasee")
            releaseKnob.setValue(val, juce::dontSendNotification);
    });

    return;
}
```

This asynchronous handoff is essential to avoid race conditions and desktop thread lockups in real‑time audio applications.

### Biomechanical Routing Logic
When structural adjustments are driven directly from the physical tracking camera, the system passes parameter changes through a functional gate mapping to all active network recipients instantly.

If the tracking stream is globally active via the toggle matrix, data is forwarded symmetrically to both SuperCollider and Processing before updating the physical UI layout:

```cpp
if (shouldProcess)
{
    // Forwarding to SuperCollider and Processing concurrently
    sender.send(pattern, val);
    processingSender.send(pattern, val);

    // Update UI safely on the main Thread
    juce::MessageManager::callAsync([this, val, pattern]() {
        if (pattern == "/hand/left/open")
            leftHandKnob.setValue(val, juce::dontSendNotification);
        // ... Repeated for other biomechanical targets ...
    });
}
```

This establishes the final link in the middleware chain, creating a direct, real-time mapping between the user's physical gesture input and external engines.

### Dynamic Layout and Styling
Instead of relying on hardcoded coordinates, the interface layout and component sizing are calculated dynamically within the `resized()` method. The UI partitions the window into relative semantic zones (Envelopes, Hands, Thumbs, and a centralized Toggle grid) ensuring the interface scales cleanly, remains symmetrical, and keeps related controls visually grouped.

### Audio Handling and Application Architecture
Because this plugin currently acts strictly as OSC middleware, the audio processing callback is implemented but intentionally left empty except for clearing the buffer:

```cpp
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Clears the buffer to prevent the output of random noise or static
    bufferToFill.clearActiveBufferRegion();
}
```

Although the JUCE module does not generate sound itself (operating entirely at the control rate while SuperCollider handles audio-rate synthesis), the project was purposefully built using JUCE's Audio Application template rather than a standard GUI Application.

This provides forward-thinking scalability: the underlying audio thread and device manager are already fully configured in case the DSP and synthesis engines are ported directly into JUCE for a future standalone release.