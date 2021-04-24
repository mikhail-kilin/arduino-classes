import cv2
import numpy as np
import serial
import time
ser = serial.Serial('/dev/ttyACM0',9600)
ser.flush()

cap = cv2.VideoCapture(0)
cap.set(3,320)
cap.set(4,240)

# 0 - red
# 1 - blue
# 2 - white
list = [0, 1, 1, 2, 1]

low_black = np.array([0, 0, 0])
high_black = np.array([255, 255, 40])

low_white = np.array([0, 0, 220])
high_white = np.array([255, 30, 255])

low_red = np.array([161, 155, 84])
high_red = np.array([179, 255, 255])

low_blue = np.array([94, 80, 2])
high_blue = np.array([126, 255, 255])

for color in list:
  black_flag = 0;
  
  while black_flag < 5:
    ret, frame = cap.read()

    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    black_mask = cv2.inRange(hsv_frame, low_black, high_black)
    
    color_mask = black_mask
    if color == 0:
      color_mask = cv2.inRange(hsv_frame, low_red, high_red)
    if color == 1:
      color_mask = cv2.inRange(hsv_frame, low_blue, high_blue)
    if color == 2:
      color_mask = cv2.inRange(hsv_frame, low_white, high_white)
    
    contours, hierarchy = cv2.findContours(color_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)[-2:]
    for contour in contours:
      area = cv2.contourArea(contour)
      if area > 400:
        black_flag += 1
    
    contours, hierarchy = cv2.findContours(color_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)[-2:]
    for contour in contours:
      area = cv2.contourArea(contour)
      if area > 400:
        cv2.drawContours(frame, contour, -1, (0,255,0), 2)
        M = cv2.moments(contour)
        if M['m00'] != 0:
          cx = int(M['m10']/M['m00'])
          cy = int(M['m01']/M['m00'])
          err = 160 - cx
          ser.write((str(err)+"\n").encode('utf-8'))
          ser.flush()
          time.sleep(0.01)
          print(err)
    
    cv2.imshow('frame', frame)
    k = cv2.waitKey(1)

  if k == 27 or k == ord('q') or k == ord('Q'):
    break
cap.release()
cv2.destroyAllWindows()