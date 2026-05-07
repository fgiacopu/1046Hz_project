# hand_tracker.py

import time
import cv2
import mediapipe as mp
from config import CAMERA_INDEX, MAX_HANDS, MIN_DET_CONF, MIN_TRACK_CONF


class HandTracker:
    def __init__(self):
        # Camera
        self.cap = cv2.VideoCapture(CAMERA_INDEX, cv2.CAP_DSHOW)
        if not self.cap.isOpened():
            raise RuntimeError(f"Impossibile aprire la webcam {CAMERA_INDEX}")

        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
        time.sleep(0.3)  # warm-up webcam

        # MediaPipe
        self.mp_hands = mp.solutions.hands
        self.hands = self.mp_hands.Hands(
            max_num_hands=MAX_HANDS,
            min_detection_confidence=MIN_DET_CONF,
            min_tracking_confidence=MIN_TRACK_CONF
        )
        self.drawer = mp.solutions.drawing_utils

    def get_frame_and_hands(self):
        success, frame = self.cap.read()
        frame = cv2.flip(frame, 1)  # Horizontal flip (mirror correction)

        if not success or frame is None:
            return None, []

        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        result = self.hands.process(frame_rgb)

        hands_data = []

        if result.multi_hand_landmarks and result.multi_handedness:
            for landmarks, handedness in zip(
                result.multi_hand_landmarks,
                result.multi_handedness
            ):
                label = handedness.classification[0].label  # "Left" / "Right"

                # Landmark drawing
                self.drawer.draw_landmarks(
                    frame,
                    landmarks,
                    self.mp_hands.HAND_CONNECTIONS
                )

                hands_data.append({
                    "label": label, # "Left" or "Right"
                    "landmarks": landmarks.landmark # List[NormalizedLandmark]
                })

        return frame, hands_data

    def show(self, frame):
        if frame is not None and frame.size > 0:
            cv2.imshow("Hand Tracker", frame)

    def should_quit(self):
        return cv2.waitKey(1) & 0xFF == ord('q')

    def release(self):
        if self.cap.isOpened():
            self.cap.release()
        cv2.destroyAllWindows()