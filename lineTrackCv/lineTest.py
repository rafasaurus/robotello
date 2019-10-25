# import numpy as np
import numpy as np
import geom_util as geom
import cv2
import track_conf as tconf
import time

# import Adafruit_BBIO.GPIO as GPIO

# video_capture = cv2.VideoCapture(-1)
video_capture = cv2.VideoCapture("/home/rafael/telegram/IMG_4143.MOV")
# video_capture = cv2.VideoCapture("/home/rafael/test.h264")
video_capture.set(3, 160)
video_capture.set(4, 120)
# Setup Output Pins
#Left Forward
# GPIO.setup("P8_10", GPIO.OUT)
#Right Forward
# GPIO.setup("P9_11", GPIO.OUT)
# GPIO.output("P8_10", GPIO.HIGH)

def get_turn(turn_state, shift_state):
    turn_dir = 0
    turn_val = 0
    if shift_state != 0:
        turn_dir = shift_state
        turn_val = tconf.shift_step if shift_state != turn_state else tconf.turn_step
    elif turn_state != 0:
        turn_dir = turn_state
        turn_val = tconf.turn_step
    return turn_dir, turn_val                

# GPIO.output("P9_11", GPIO.HIGH)

def find_main_countour(image):
    cnts, hierarchy = cv2.findContours(image, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_SIMPLE)
    C = None
    if cnts is not None and len(cnts) > 0:
         C = max(cnts, key = cv2.contourArea)
    if C is None:
        return None, None
    rect = cv2.minAreaRect(C)
    box = cv2.boxPoints(rect)
    box = np.int0(box)
    box = geom.order_box(box)
    return C, box

def check_shift_turn(angle, shift):
    turn_state = 0
    if angle < tconf.turn_angle or angle > 180 - tconf.turn_angle:
        turn_state = np.sign(90 - angle)

    shift_state = 0
    if abs(shift) > tconf.shift_max:
        shift_state = np.sign(shift)
    return turn_state, shift_state

while(True):
    # Capture the frames
    ret, frame = video_capture.read()
    (h, w) = frame.shape[:2] 
    center = (w / 2, h / 2)
    scale = 1.

    M = cv2.getRotationMatrix2D(center, 180, scale)
    frame = cv2.warpAffine(frame, M, (w, h))

    # Crop the image
    # crop_img = frame[60:120, 0:160]
    # Convert to grayscale

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Gaussian blur
    blur = cv2.GaussianBlur(gray,(5,5),0)

    # Color thresholding
    ret,thresh1 = cv2.threshold(blur,60,255,cv2.THRESH_BINARY_INV)

    # Erode and dilate to remove accidental line detections
    mask = cv2.erode(thresh1, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)

    # Find the contours of the frame
    contours,hierarchy = cv2.findContours(mask.copy(), 1, cv2.CHAIN_APPROX_NONE)
    # Find the biggest contour (if detected)
    if len(contours) > 0:
        c = max(contours, key=cv2.contourArea)
        rect = cv2.minAreaRect(c)
        box = cv2.boxPoints(rect)
        box = np.int0(box)
        box = geom.order_box(box)
        # TODO wtf is this
        M = cv2.moments(c)
        cx = int(M['m10']/M['m00'])
        cy = int(M['m01']/M['m00'])

        # cv2.line(frame,(cx,0),(cx,720),(255,0,0),1)
        # cv2.line(frame,(0,cy),(1280,cy),(255,0,0),1)

        p1, p2 = geom.calc_box_vector(box)
        cv2.drawContours(frame, [c], -1, (0,0,255), 3)
        cv2.drawContours(frame, [box],0,(255,0,0), 3)

        angle = geom.get_vert_angle(p1, p2, w, h)
        shift = geom.get_horz_shift(p1[0], w)

        # print(angle)
        # print(shift)

        # TODO
        # if a is None:
        #   # there is some code omitted related to line finding
        #   break
        turn_state, shift_state = check_shift_turn(angle, shift)
        turn_dir, turn_val = get_turn(turn_state, shift_state)

        print(turn_dir, "   ", turn_val)
        if turn_dir != 0:
            # turn(turn_dir, turn_val)
            last_turn = turn_dir
        else:
            # time.sleep(tconf.straight_run)
            last_turn = 0

    #Display the resulting frame
    cv2.imshow('frame',frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
