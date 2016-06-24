
OpenGL Code samples for 6th generation Intel® Core™ processor (code-named Skylake) and Best practices

Game developers often use OpenGL to handle the rendering chores for graphics-intensive games.
OpenGL is an application programming interface for efficiently rendering two- and three-dimensional vector graphics. 
It is available on most platforms. As with any tool, there are ways to use it more efficiently.
The code samples are a series from Grahics API developer guide for for 6th generation Intel® Core™ processor (https://software.intel.com/en-us/articles/6th-gen-graphics-api-dev-guide) that demonstrates how to get the most out of OpenGL 4.4 and higher. The complete series includes:
1.	Power of two textures have better performance
2.	Use native texture formats for best rendering performance
3.	Textures have better rendering performances than images
4.	Atomic Counter Buffers (ACB) vs. Shader Storage Buffer Objects (SSBO)
5.	Swap FrameBufferObjects (FBO objects) instead of swapping surfaces in a single FBO
6.	Avoid OpenGL calls that Synchronize CPU and GPU




#Lesson 1: Use power of two textures.

This code compares the difference in performance (measured in millisecond-per-frame) between following the intel best-practice or not following it. 

Intel Best Practice:  Use power of two textures.

This example discusses how to improve OpenGL performance by using textures that have dimensions that are a power-of-two. The application will display an image rendered using both a power of two and a non-power of two texture. The current performance for each (displayed in milliseconds-per-frame) will be displayed in the console window, along with the number of frames-per-second.  Pressing the spacebar will toggle between the two textures so you can compare the two approaches. When switching, the application will animate the image as a visual indicator of the change.


Run the program and use the spacebar to switch between rendering with a power-of-two texture and rendering with a non-power-of-two texture.


#Lesson 2: Use native texture formats

This code compares the difference in performance (measured in milliseconds-per-frame) between following the intel best-practice or not following it.

Intel Best Practice:  Use native texture formats

Run the program and use the spacebar to switch between the uploading a RGB16 texture (non-native) and uploading a RGB8 texture (native).

#Lesson 3: Use textures instead of images.

This code compares the difference in peformance (measured in millisecond-per-frame) between following the intel best-practice or not following it. 

Intel Best Practice:  Use textures instead of images.

This application covers how to improve OpenGL performance by using textures, rather than images. It demonstrates this by alternating between using a texture and a 2-D image. The current performance for each (displayed in milliseconds-per-frame) will be displayed in the console window, along with the number of frames-per-second.  Pressing the spacebar will rotate through the various combinations so you can compare them. When switching, the application will animate the image as a visual indicator of the change.


Run the program and use the spacebar to switch between rendering with various texture parameters and compare it against rendering with texture images.

#Lesson 4: There are no real performance benefits to using Atomic Counter Buffers instead of Shader Storage Buffer Objects

This code compares the difference in peformance (measured in millisecond-per-frame) between following the intel best-practice or not following it. 

Intel Best Practice:  There are no real performance benefits to using Atomic Counter Buffers instead of Shader Storage Buffer Objects

This example shows there are no real performance benefits to using Atomic Counter Buffer (ACB) instead of Shader Storage Buffer Objects (SSBO) when trying to improve OpenGL performance. The application demonstrates this by alternating between SSBOs and ACBs while showing the current milliseconds-per-frame and the number of frames-per-second.  Pressing the spacebar will switch between using SSBOs and ACBs. When switching, the application will animate the image as a visual indicator of the change.

Run the program and use the spacebar to switch between rendering with Atomic Counter Buffers and Shader Storage Buffer Objects.

#Lesson 5: Swap FBO objects instead of swapping surfaces in a single FBO

This code compares the difference in peformance (measured in millisecond-per-frame) between following the intel best-practice or not following it. Intel Best Practice:  Swap FBO objects instead of swapping surfaces in a single FBO

This example shows how to improve OpenGL performance by swapping FrameBufferObjects (FBO) instead of using a single FBO and swapping surfaces. It is useful when making multiple changes to a rendered image, such as switching color, depth, or stencil attachments. The recommendation is to use dedicated FBOs for each set in use, rather than sharing an FBO amongst all attachments. Switching an entire FBO is more efficient than switching individual surfaces one at a time.

This application will display an image rendered using both an FBO reused multiple times with different data and with separate FBOs. The current performance for each approach will be displayed in a console window in milliseconds-per-frame and number of frames-per-second.  Pressing the spacebar will toggle between the two methods so you can compare the two approaches. When switching, the application will animate the image as a visual indicator of the change

Run the program and use the spacebar to mearsure rendering performance for the two conditions.

#Lesson 6: Avoid gpu syncronization calls, glReadPixels, glFlush, glFinish

This code compares the difference in peformance (measured in milliseconds-per-frame) between following the intel best-practice or not following it.  Intel Best Practice:  Avoid gpu syncronization calls, glReadPixels, glFlush, glFinish

OpenGL contains a variety of calls that force synchronization between the CPU and the GPU.  These are called Sync Objects and are designed to synchronize the activity between the GPU and the application.  Unfortunately this hurts overall performance because the CPU stalls until the GPU has completed its action.
 
This application demonstrates the effects of three different OpenGL calls that cause the CPU and GPU to synchronize. The calls are glReadPixels, glFlush, and glFinish. These are compared to a non-synchronized performance. The current performance for each approach will be displayed in a console window in milliseconds-per-frame and number of frames-per-second.  Pressing the spacebar will cycle between the methods so you can compare the effects. When switching, the application will animate the image as a visual indicator of the change.

Run the program and use the spacebar to measure the rendering cost associated with using these gpu syncronization calls. 




