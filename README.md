# opencv-qt-modetect
Webcam based motion detection using OpenCV and Qt6

![Window](https://raw.githubusercontent.com/gmbows/opencv-qt-modetect/master/window.png)

## Overview
`Start Capture` begins capturing frames from the OS default capture device. A bounding box is drawn around moving objects. <br>
`Stop Capture` suspends video capture. <br>
`Reset Frame` Resets the reference frame once, click this when the camera is moved to a new position. <br>
`Auto-Reset` Resets the reference frame each capture, use this if the camera is in motion. <br>

## Build

Set environment variable OPENCV_BUILD to your OpenCV build directory, build with QMake
