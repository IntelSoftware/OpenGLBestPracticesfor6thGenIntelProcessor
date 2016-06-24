This code compares the difference in peformance (measured in milliseconds-per-frame) between following the intel best-practice or not following it.  Intel Best Practice:  Avoid gpu syncronization calls, glReadPixels, glFlush, glFinish

OpenGL contains a variety of calls that force synchronization between the CPU and the GPU.  These are called Sync Objects and are designed to synchronize the activity between the GPU and the application.  Unfortunately this hurts overall performance because the CPU stalls until the GPU has completed its action.
 
This application demonstrates the effects of three different OpenGL calls that cause the CPU and GPU to synchronize. The calls are glReadPixels, glFlush, and glFinish. These are compared to a non-synchronized performance. The current performance for each approach will be displayed in a console window in milliseconds-per-frame and number of frames-per-second.  Pressing the spacebar will cycle between the methods so you can compare the effects. When switching, the application will animate the image as a visual indicator of the change.

Run the program and use the spacebar to measure the rendering cost associated with using these gpu syncronization calls. 


