Peter R. Elespuru

controls:
------------------------------------------------------------------------------
 *  'm' to switch filters
 *  arrows pan
 *  PgUp/PgDn zooms in/out

summary:
------------------------------------------------------------------------------
To build and run:
(0) ensure you have a valid install of appropriate OpenCV and OpenGL libraries
(1) make && ./cvglsl

It will create a 'cvglsl' binary, and the project includes a Makefile that
will use a platform specific makefile to actually build the executable,
and it was tested successfully on Linux and OS X.

This application uses opencv to capture video frames from a webcam and do real 
time image processing on the data stream. There are 28 video filters total.
