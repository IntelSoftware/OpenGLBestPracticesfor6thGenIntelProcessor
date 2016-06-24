This code compares the difference in peformance (measured in millisecond-per-frame) between following the intel best-practice or not following it. 

Intel Best Practice:  Use power of two textures.

This example discusses how to improve OpenGL performance by using textures that have dimensions that are a power-of-two. The application will display an image rendered using both a power of two and a non-power of two texture. The current performance for each (displayed in milliseconds-per-frame) will be displayed in the console window, along with the number of frames-per-second.  Pressing the spacebar will toggle between the two textures so you can compare the two approaches. When switching, the application will animate the image as a visual indicator of the change.


Run the program and use the spacebar to switch between rendering with a power-of-two texture and rendering with a non-power-of-two texture.

