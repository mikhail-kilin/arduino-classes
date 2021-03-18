import cv2

face_cascade = cv2.CascadeClassifier('data/haarcascade_frontalface_default.xml')

cap = cv2.VideoCapture(0)

while True:
    ret, image = cap.read()
    cv2.imshow('frame', image) 
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    if len(faces) != 0:
        print(len(faces))

    k = cv2.waitKey(1)
    if k == 27 or k == ord('q') or k == ord('Q'):
         break
cap.release()
cv2.destroyAllWindows()
