# main.py

from hand_tracker import HandTracker
from gesture_features import compute_hand_openness, compute_thumb_value
from smoothing import SignalSmoother
from osc_sender import OscSender
import time
from config import USE_ARDUINO, OSC_IP, OSC_PORT

def main():
    # Gesture tracking
    tracker = HandTracker()

    # Separate smoothing filters for each hand
    smooth_hand_left = SignalSmoother(alpha=0.3)
    smooth_hand_right = SignalSmoother(alpha=0.3)

    smooth_thumb_left = SignalSmoother(alpha=0.2)
    smooth_thumb_right = SignalSmoother(alpha=0.2)


    # OSC
    osc = OscSender(ip=OSC_IP, port=OSC_PORT, debug=True) # debug=True to print OSC messages values in console

    # Rate limit setup
    SEND_INTERVAL = 0.03  # ~33 Hz
    last_send_left = 0.0
    last_send_right = 0.0

    # Optional Arduino input
    arduino = None
    if USE_ARDUINO:
        from arduino_input import ArduinoInput
        arduino = ArduinoInput()
        arduino.connect()

    while True:
        # Just one reading for each frame
        frame, hands = tracker.get_frame_and_hands()

        if frame is not None:
            # Hand gesture processing
            for hand in hands:
                label = hand["label"]          # "Left" or "Right"
                landmarks = hand["landmarks"]

                openness = compute_hand_openness(landmarks)

                if label == "Left":    
                    value = smooth_hand_left.process(openness)
                    osc.send("/hand/left/open", value)
                    thumb = smooth_thumb_left.process(compute_thumb_value(landmarks))
                    osc.send("/hand/left/thumb", thumb)

                elif label == "Right":
                    value = smooth_hand_right.process(openness)
                    osc.send("/hand/right/open", value)
                    thumb = smooth_thumb_right.process(compute_thumb_value(landmarks))
                    osc.send("/hand/right/thumb", thumb)
            tracker.show(frame)

        # Optional Arduino input
        if arduino and arduino.enabled:
            bpm = arduino.read()
            if bpm is not None:
                osc.send("/bio/bpm", bpm)

        if tracker.should_quit():
            break

    tracker.release()


if __name__ == "__main__":
    main()
