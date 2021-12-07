import requests
import cv2
import numpy as np
import serial
import time
import struct
import math

serialcomm = serial.Serial('COM4', 9600)
serialcomm.timeout = 1

hold =0
def empty(x):
    pass

us_color="EMPTY"

def INPUT():
    global us_color
    us_color = input("Input the color of object to be detected (blue or green): ")

cap = cv2.VideoCapture("http://192.168.1.3:8080//video")
blue = np.array([[100,149,51],[139,255,255]])
green = np.array([[69,155,0],[80,255,255]])

def coordinates(frame,color):
    # print(frame)
    img=cv2.resize(frame, None, fx=1 , fy=1)

    img=cv2.resize(img,(512,512))
    img = img[0:270, :]
    img=cv2.resize(img,(512,512))
    HSV_img=cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    # lower=np.array([h_min, s_min, v_min])
    # upper=np.array([h_max, s_max, v_max])
    lower=color[0]
    upper=color[1]

    mask=cv2.inRange(HSV_img, lower, upper)

    result=cv2.bitwise_and(img, img, mask=mask)

    result=cv2.GaussianBlur(result, (5, 5), -0)     #Light filter

    img_gray=cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)
    __, thresh= cv2.threshold(img_gray, 50, 255, cv2.THRESH_BINARY)
    contours, hiererachy= cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    print(len(contours))
    A=0

    for contour in contours:
        area=cv2.contourArea(contour)
        if A<area:
            # CONT=countour
            A=area
        print(area)
    
    # if(A<5):
    #     print("Object not detected!")
    #     INPUT()
    #     return
    
    Cx=0
    Cy=0
    for contour in contours:
        area=cv2.contourArea(contour)
        if A==area:
            cv2.drawContours(result, contour, -1, (0, 0, 255), 2)
            M=cv2.moments(contour)
            if M["m00"] != 0:
                Cx=int(M["m10"]/M["m00"])
                Cy=512-int(M["m01"]/M["m00"])
                break
    
    Cx = (Cx/img.shape[1])*23 - 3.5
    Cy = (Cy/img.shape[0])*22

    gx = 2*Cx/math.sqrt(Cx*Cx + Cy*Cy)
    gy = 5*Cy/math.sqrt(Cx*Cx + Cy*Cy)

    print("X: ", Cx) 
    print("Y: ", Cy)
    print("gX: ", gx) 
    print("gY: ", gy)

    Cx = Cx - gx
    Cy = Cy - gy

    if(Cx<=0 and Cy<=0):
        print("Object not detected!")
        INPUT()
        return
    
    sCx = str(Cx)
    sCy = str(Cy)
    # print("X: ", Cx) 
    # print("Y: ", Cy)
    points = sCx + "," + sCy
    # cv2.imshow("Gray Image", img_gray)
    cv2.imshow("Original Image", img)
    cv2.imshow("Result", result)
    # cv2.imshow("Threshold", thresh)
    # cv2.imshow('frame',img)
    #if cv2.waitKey(1)==ord('q'):
    #    break
    cv2.waitKey(1)
    return points
#cap.release()
#cv2.destroyAllWindows()

INPUT()

while True:
    ret, frame = cap.read()
    # us_color = input("Input the color of object to be detected (blue or green): ")
    if us_color=="blue":
        obj_points = coordinates(frame,blue)
    elif us_color=="green":
        obj_points = coordinates(frame,green)
    # else :
    #     print("Object not detected")
    #     exit
    if hold%150 ==0:
        print(obj_points)
        serialcomm.write(obj_points.encode())
    hold = hold +1
    #print("next",hold)

serialcomm.close()