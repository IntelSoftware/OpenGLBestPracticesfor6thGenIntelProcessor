This code compares the difference in peformance (measured in millisecond-per-frame) between following the intel best-practice or not following it. 

Intel Best Practice:  There are no real performance benefits to using Atomic Counter Buffers instead of Shader Storage Buffer Objects

This example shows there are no real performance benefits to using Atomic Counter Buffer (ACB) instead of Shader Storage Buffer Objects (SSBO) when trying to improve OpenGL performance. The application demonstrates this by alternating between SSBOs and ACBs while showing the current milliseconds-per-frame and the number of frames-per-second.  Pressing the spacebar will switch between using SSBOs and ACBs. When switching, the application will animate the image as a visual indicator of the change.

Run the program and use the spacebar to switch between rendering with Atomic Counter Buffers and Shader Storage Buffer Objects.

