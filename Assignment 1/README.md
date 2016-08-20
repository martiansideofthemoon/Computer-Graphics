# OpenGL Paint Tool!

Here is a blogpost explaining the algorithms. [http://martiansideofthemoon.github.io/2016/08/16/opengl-paint-tool.html](http://martiansideofthemoon.github.io/2016/08/16/opengl-paint-tool.html).

We have a drawing saved as **my_drawing.txt**. The corresponding image file is **image_file.png**.

## Running the Program

To run this app, type "make" and execute the mydraw file using "./mydraw"

## Using the Program

The app is initially loaded with the default configuration which is as follows -
window_height = 640 pixels
window_width = 1024 pixels
This can be changed in the code quite easily, in the first few lines of mydraw.cpp

The canvas is initialized with a WHITE pen color of size 1. The default background is black. The following keyboard functionality works :-

1. **1** - Enter the line drawing mode. Here you are free to click two points in succession to get a line drawn between them. A right click will clear the first chosen point, if any.

2. **2** - Enter the triangle drawing mode. Here you are free to click three points in successtion to get a traingle drawn between them. A right click will clear the previous chosen point in memory.

3. **f** - Enter the fill mode. Here any region that's clicked is filled with the fill color.

To change the mode, you must click one of **1**, **2**, or **f**. There is no concept of a toggle here.

4. **c** - Clears the canvas, coloring it with the background color.

5. **e** - Toggles the eraser mode. In the eraser mode, lines and triangles are drawn in the background color.

6. **g** - Allows us to change the fill color. An RGB value is entered in the terminal separated by spaces. Each value should be between 0 and 1.

7. **h** - Allows us to change the pen color. An RGB value is entered in the terminal separated by spaces. Each value should be between 0 and 1.

8. **i** - Allows us to change the background color. An RGB value is entered in the terminal separated by spaces. Each value should be between 0 and 1. On changing the background color, all pixels having the old background color are replaced by the next background color.

9. **j** - Enter the pen size. 1 is the default pen size.

10. **n** - Make a new canvas. You are given the option to set the background color here.

11. **s** - Save function. You must input a file name in the terminal. Files are saved accordingly.

12. **l** - Load function. You must input a file name in the terminal. The file is loaded and displayed on the screen.

