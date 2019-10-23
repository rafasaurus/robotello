# import numpy as np
import cv2
# import Adafruit_BBIO.GPIO as GPIO

video_capture = cv2.VideoCapture(-1)
video_capture.set(3, 160)
video_capture.set(4, 120)
# Setup Output Pins
#Left Forward
# GPIO.setup("P8_10", GPIO.OUT)
#Right Forward
# GPIO.setup("P9_11", GPIO.OUT)
# GPIO.output("P8_10", GPIO.HIGH)
# GPIO.output("P9_11", GPIO.HIGH)
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
    _, contours,hierarchy = cv2.findContours(mask.copy(), 1, cv2.CHAIN_APPROX_NONE)
    # Find the biggest contour (if detected)
    if len(contours) > 0:
        c = max(contours, key=cv2.contourArea)
        M = cv2.moments(c)
        cx = int(M['m10']/M['m00'])
        cy = int(M['m01']/M['m00'])
        cv2.line(frame,(cx,0),(cx,720),(255,0,0),1)
        cv2.line(frame,(0,cy),(1280,cy),(255,0,0),1)
        cv2.drawContours(frame, contours, -1, (0,255,0), 1)
        print(cx)
        print(cy)
        # if cx >= 120:
        #     GPIO.output("P8_10", GPIO.HIGH)
        #     GPIO.output("P9_11", GPIO.LOW)
        # if cx < 120 and cx > 50:
        #     GPIO.output("P8_10", GPIO.LOW)
        #     GPIO.output("P9_11", GPIO.LOW)
        # if cx <= 50:
        #     GPIO.output("P8_10", GPIO.LOW)
        #     GPIO.output("P9_11", GPIO.HIGH)
    # else:
    #     GPIO.output("P8_10", GPIO.HIGH)
    #     GPIO.output("P9_11", GPIO.HIGH)
    #Display the resulting frame
    cv2.imshow('frame',frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
