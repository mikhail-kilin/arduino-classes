import cv2

face_cascade = cv2.CascadeClassifier('data/haarcascade_frontalface_default.xml')

image = cv2.imread('images/fases.jpg')
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
faces = face_cascade.detectMultiScale(gray, 1.3, 5)

print(len(faces))
