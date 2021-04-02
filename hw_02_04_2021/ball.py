import cv2
import numpy as np
import serial
import time
ser = serial.Serial('/dev/ttyACM0',9600)
ser.flush()

cap = cv2.VideoCapture(0)
cap.set(3,320)
cap.set(4,240)

low_yellow = np.array([5, 80, 80])
high_yellow = np.array([50, 255, 255])

while cap.isOpened():
  ret, frame = cap.read()

  hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

  yellow_mask = cv2.inRange(hsv_frame, low_yellow, high_yellow)
  contours, hierarchy = cv2.findContours(yellow_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)[-2:]
  for contour in contours:
      area = cv2.contourArea(contour)
      if area > 400:
          cv2.drawContours(frame, contour, -1, (0,255,0), 2)
          M = cv2.moments(contour)
          if M['m00'] != 0:
              cx = int(M['m10']/M['m00'])
              cx = int(M['m01']/M['m00'])
              err = 160 - cx
              ser.write((str(err)+"\n").encode('utf-8'))
              ser.flush()
              time.sleep(0.01)
  
  cv2.imshow('frame', frame)
  k = cv2.waitKey(1)

  if k == 27 or k == ord('q') or k == ord('Q'):
    break
cap.release()
cv2.destroyAllWindows()