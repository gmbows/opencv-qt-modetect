# opencv-qt-modetect

Webcam-based motion detection using OpenCV and Qt6

![Window](https://raw.githubusercontent.com/gmbows/opencv-qt-modetect/master/window.png)

## Overview

### Buttons
`Start Capture` begins capturing frames from the OS default capture device. A bounding box is drawn around moving objects. <br>
`Stop Capture` suspends video capture. <br>
`Reset Frame` Resets the reference frame once, click this when the camera is moved to a new position. <br>
`Auto-Reset` Resets the reference frame each capture, use this if the camera is in motion. <br>

### Sliders
Use the sliders to adjust image processing settings. <br>
`Contour area` sets the minimum contour bounding-box area required to classify the contour as movement. <br>
`Blur size` sets the size of a blur filter used in image processing.  Increase this for noisy environments or to "squash" smaller contours. <br>
`Diff threshold` sets the alpha threshold that must be met to consider two pixels different. <br>
Change the `Display layer` dropdown setting to select which frame to display in the viewport.

## Build

Set environment variable OPENCV_BUILD to your OpenCV build directory, build with QMake
