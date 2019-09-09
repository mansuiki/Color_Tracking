import cv2 as cv
import numpy as np

def nothing(N):
    pass


cam = cv.VideoCapture(0)

cam.set(cv.CAP_PROP_FRAME_WIDTH, 640)
cam.set(cv.CAP_PROP_FRAME_HEIGHT, 480)

if cam.isOpened:
    pass
else:
    print("ERROR : Can't Open Cam")

cv.namedWindow("CONFIG")
cv.createTrackbar("HMIN", "CONFIG", 0, 255, nothing)
cv.createTrackbar("SMIN", "CONFIG", 0, 255, nothing)
cv.createTrackbar("VMIN", "CONFIG", 0, 255, nothing)
cv.createTrackbar("HMAX", "CONFIG", 255, 255, nothing)
cv.createTrackbar("SMAX", "CONFIG", 255, 255, nothing)
cv.createTrackbar("VMAX", "CONFIG", 255, 255, nothing)
cv.createTrackbar("OpenSize", "CONFIG", 0, 15, nothing)
cv.createTrackbar("CloseSize", "CONFIG", 0, 15, nothing)

while True:    
    low = np.array([cv.getTrackbarPos("HMIN", "CONFIG"), cv.getTrackbarPos("SMIN", "CONFIG"), cv.getTrackbarPos("VMIN", "CONFIG")])
    high = np.array([cv.getTrackbarPos("HMAX", "CONFIG"), cv.getTrackbarPos("SMAX", "CONFIG"), cv.getTrackbarPos("VMAX", "CONFIG")])

    OSize = cv.getTrackbarPos("OpenSize", "CONFIG")
    CSize = cv.getTrackbarPos("CloseSize", "CONFIG")
    
    _, frame = cam.read()

    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
    mask = cv.inRange(hsv, low, high)

    Okernel = np.ones((OSize, OSize),  np.uint8)
    Ckernel = np.ones((CSize, CSize), np.uint8)
    mask = cv.morphologyEx(mask, cv.MORPH_OPEN, Okernel)
    mask = cv.morphologyEx(mask, cv.MORPH_CLOSE, Ckernel)

    labels, label, stat, centroid = cv.connectedComponentsWithStats(mask)
    maxsize = -1
    current = 0
    for i in range(1, labels):
        area = stat[i, cv.CC_STAT_AREA]
        if maxsize < area:
            maxsize = area
            current = i

    height = stat[current, cv.CC_STAT_HEIGHT]
    left = stat[current, cv.CC_STAT_LEFT]
    top = stat[current, cv.CC_STAT_TOP]
    width = stat[current, cv.CC_STAT_WIDTH]
    center = (int(centroid[current, 0]), int(centroid[current, 1]))

    cv.rectangle(frame, (left, top), (left+width, top+height), (0, 0, 255), 3)
    cv.circle(frame, center, 4, (0, 255, 0), 4)
    cv.putText(frame, str(center), (int(centroid[current, 0]), int(centroid[current, 1])+40), 1, 3, (0, 255, 0), 3)

    mask = cv.cvtColor(mask, cv.COLOR_GRAY2BGR)
    result = cv.hconcat([frame, mask])
    cv.imshow("Reuslt", result)

    if cv.waitKey(5) == 27:
        break;

