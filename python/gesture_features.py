# gesture_features.py

FINGERTIPS = [8, 12, 16, 20]

PALM_POINTS = [0, 5, 9, 13, 17]

FINGERTIP_WEIGHTS = {
    8:  0.35,  # index
    12: 0.35,  # middle
    16: 0.15,  # ring
    20: 0.15   # pinky
}

MIN_OPENNESS = 0.035
MAX_OPENNESS = 0.185

THUMB_TIP = 4

MIN_THUMB_DIST = 0.02
MAX_THUMB_DIST = 0.15

def normalize(value, min_val, max_val):
    value_clamped = max(min_val, min(value, max_val))
    return (value_clamped - min_val) / (max_val - min_val)

def compute_hand_openness(landmarks):
    # Computing the coordinates of a virtual "palm center" as the mean of key palm landmarks
    palm_x = sum(landmarks[i].x for i in PALM_POINTS) / len(PALM_POINTS) # mean x coordinate of a virtual palm center
    palm_y = sum(landmarks[i].y for i in PALM_POINTS) / len(PALM_POINTS) # mean y coordinate of a virtual palm center

    # Accumulators for weighted fingertip distances and total weight
    weighted_sum = 0.0
    weight_total = 0.0

    # Compute weighted fingertip-to-palm distances (index & middle prioritized)
    for idx in FINGERTIPS:
        tip = landmarks[idx]
        d = ((tip.x - palm_x) ** 2 + (tip.y - palm_y) ** 2) ** 0.5
        w = FINGERTIP_WEIGHTS[idx]

        weighted_sum += w * d
        weight_total += w

    # Raw hand openness as weighted average distance from palm center
    openness_raw = weighted_sum / weight_total 

    # Calibrate based on observed min/max values (these may need adjustment)
    openness = normalize(
    openness_raw,
    min_val=MIN_OPENNESS,   # empirically calibrated: hand closed
    max_val=MAX_OPENNESS    # empirically calibrated: hand open
)

    return openness

def compute_thumb_value(landmarks):
    # Palm center
    palm_x = sum(landmarks[i].x for i in PALM_POINTS) / len(PALM_POINTS)
    palm_y = sum(landmarks[i].y for i in PALM_POINTS) / len(PALM_POINTS)

    thumb = landmarks[THUMB_TIP]

    # Distance from thumb tip to palm center
    d = ((thumb.x - palm_x) ** 2 + (thumb.y - palm_y) ** 2) ** 0.5

    # Normalize thumb distance to a 0-1 range based on observed min/max values
    thumb_value = normalize(d, MIN_THUMB_DIST, MAX_THUMB_DIST)

    return thumb_value

# def compute_hand_vibrato()

