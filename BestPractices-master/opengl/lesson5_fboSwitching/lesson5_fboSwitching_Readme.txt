This code compares the difference in peformance (measured in millisecond-per-frame) between following the intel best-practice or not following it. Intel Best Practice:  Swap FBO objects instead of swapping surfaces in a single FBO

This example shows how to improve OpenGL performance by swapping FrameBufferObjects (FBO) instead of using a single FBO and swapping surfaces. It is useful when making multiple changes to a rendered image, such as switching color, depth, or stencil attachments. The recommendation is to use dedicated FBOs for each set in use, rather than sharing an FBO amongst all attachments. Switching an entire FBO is more efficient than switching individual surfaces one at a time.

This application will display an image rendered using both an FBO reused multiple times with different data and with separate FBOs. The current performance for each approach will be displayed in a console window in milliseconds-per-frame and number of frames-per-second.  Pressing the spacebar will toggle between the two methods so you can compare the two approaches. When switching, the application will animate the image as a visual indicator of the change

Run the program and use the spacebar to mearsure rendering performance for the two conditions.

