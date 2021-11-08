import requests
import cv2
import numpy as np
import serial
import time

#serialcomm = serial.Serial('/dev/ttyACM0', 9600)
#serialcomm.timeout = 1

def empty(x):
    pass

cv2.namedWindow("HSV")
cv2.resizeWindow("HSV", 512, 300)
cv2.createTrackbar("HUE_min", "HSV", 0, 179, empty)
cv2.createTrackbar("HUE_max", "HSV", 179, 179, empty)
cv2.createTrackbar("Saturation_min", "HSV",  0, 255, empty)
cv2.createTrackbar("Saturation_max", "HSV", 255, 255, empty)
cv2.createTrackbar("Value_min", "HSV", 0, 255, empty)
cv2.createTrackbar("Value_max", "HSV", 255, 255, empty)


cap = cv2.VideoCapture("http://192.168.29.87:4747/video")
#cap=cv2.VideoCapture(0)

def coordinates(frame):
    # print(frame)
    img=cv2.resize(frame, None, fx=1 , fy=1)
    HSV_img=cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    h_min=cv2.getTrackbarPos("HUE_min", "HSV")
    h_max=cv2.getTrackbarPos("HUE_max", "HSV")
    s_min=cv2.getTrackbarPos("Saturation_min", "HSV")
    s_max=cv2.getTrackbarPos("Saturation_max", "HSV")
    v_min=cv2.getTrackbarPos("Value_min", "HSV")
    v_max=cv2.getTrackbarPos("Value_max", "HSV")

    lower=np.array([h_min, s_min, v_min])
    upper=np.array([h_max, s_max, v_max])
    #lower=np.array([30, 69, 0])
    #upper=np.array([72, 255, 255])

    mask=cv2.inRange(HSV_img, lower, upper)

    result=cv2.bitwise_and(img, img, mask=mask)

    result=cv2.GaussianBlur(result, (5, 5), -0)     #Light filter

    img_gray=cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)
    __, thresh= cv2.threshold(img_gray, 110, 255, cv2.THRESH_BINARY)
    contours, hiererachy= cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    A=0

    for contour in contours:
        area=cv2.contourArea(contour)
        if A<area:
            # CONT=countour
            A=area
        
    Cx=0
    Cy=0
    for contour in contours:
        area=cv2.contourArea(contour)
        if A==area:
            cv2.drawContours(result, contour, -1, (0, 0, 255), 2)
            M=cv2.moments(contour)
            if M["m00"] != 0:
                Cx=int(M["m10"]/M["m00"])
                Cy=int(M["m01"]/M["m00"])
                break
            

    print("X: ", Cx) 
    print("Y: ", Cy)
    points = [Cx,Cy]
    cv2.imshow("Gray Image", img_gray)
    cv2.imshow("Original Image", img)
    cv2.imshow("Result", result)
    # cv2.imshow('frame',img)
    #if cv2.waitKey(1)==ord('q'):
    #    break
    cv2.waitKey(1)
    return points
#cap.release()
#cv2.destroyAllWindows()

while True:
    ret, frame = cap.read()
    obj_points = coordinates(frame)
    for i in range(2):
        #serialcomm.write(obj_points[i].encode())
        #time.sleep(0.5)
        print(obj_points[i])
    print("next")

serialcomm.close()
