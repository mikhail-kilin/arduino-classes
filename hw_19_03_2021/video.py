import cv2
import numpy as np
cap = cv2.VideoCapture("color_ball.mp4")

low_yellow = np.array([20, 110, 110])
high_yellow = np.array([40, 255, 255])

while cap.isOpened():
  ret, frame = cap.read()

  hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

  yellow_mask = cv2.inRange(hsv_frame, low_yellow, high_yellow)
  contours, hierarchy = cv2.findContours(yellow_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)[-2:]
  for contour in contours:
    area = cv2.contourArea(contour)
    if area > 800:
      cv2.drawContours(frame, contour, -1, (0,255,0), 2)
  
  cv2.imshow('frame', frame)
  k = cv2.waitKey(1)

  if k == 27 or k == ord('q') or k == ord('Q'):
    break
cap.release()
cv2.destroyAllWindows()