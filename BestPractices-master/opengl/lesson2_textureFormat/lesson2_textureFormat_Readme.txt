This code compares the difference in peformance (measured in millisecond-per-frame) between following the intel best-practice or not following it. 

Intel Best Practice:  Use power of two textures.

This example covers how to improve OpenGL performance by using native texture formats. The example cycles through a variety of different texture formats as it renders an image in a window.  For each format the current performance is displayed in milliseconds-per-frame, along with the number of frames-per-second.  Pressing the spacebar will rotate to the next texture in the list so you can see which formats work best on your hardware.  When switching, the application will animate the image as a visual indicator of the change.


Run the program and use the spacebar to cycle through various texture formats.