This code compares the difference in peformance (measured in millisecond-per-frame) between following the intel best-practice or not following it. 

Intel Best Practice:  Use power of two textures.

This application covers how to improve OpenGL performance by using textures, rather than images. It demonstrates this by alternating between using a texture and a 2-D image. The current performance for each (displayed in milliseconds-per-frame) will be displayed in the console window, along with the number of frames-per-second.  Pressing the spacebar will rotate through the various combinations so you can compare them. When switching, the application will animate the image as a visual indicator of the change.


Run the program and use the spacebar to switch between rendering with various texture parameters and compare it against rendering with texture images.