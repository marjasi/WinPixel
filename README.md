# WinPixel
A pixel art editor created with Windows API that allows you to draw 24x24 size pixel art and save it as .bmp files of the same size.
## Main Features
- Work with a 24x24 pixel drawing area and color it as you see fit.
- Save your drawings as .bmp files and then later load them back into the editor.
- Choose from a preset palette of 8 colors or specify a custom color with RGB values and hex color codes.
- Turn drawing area gridlines on or off as well as change their color from black to white and vice versa.
- Use The Pixel Pen or The Gizmo Tool to create superb pixel art.
## Top window menu
### Art
- **Save Art:** save the current drawing area pixel data to a .bmp file. Gridlines will never be saved, only the background colors of individual pixels. The final image will look like the one that can be seen in the editor with the gridlines turned off.
- **Load Art From File:** load pixel data from a .bmp file that is of size 24x24 or larger. If a .bmp file is of size 24x24, all of its data will be loaded into the drawing area. What is more, if a .bmp file is larger than 24x24 pixels, data from the top left corner of the span 24x24 will be loaded into the drawing area. However, if a .bmp file is smaller than 24x24 pixels, its data won't be loaded into the drawing area. 
### Options
- **Show/Hide Gridlines:** turn the gridlines on or off.
- **Invert Gridline Color:** changes the gridline color from black to white and vice versa. If the gridlines are turned off, this option won't draw gridlines after clicking. Otherwise, it will redraw the gridlines with the changed color.
## Drawing tools
### The Pixel Pen
A standard drawing tool that simply paints a selected pixel with the currently specified drawing color.
### The Gizmo Tool
An alternative to The Pixel Pen that is similar to a bucket tool. It paints all pixels of the same color as the selected pixel with the currently specified drawing color.
## Windows API usage
- **Resource scripts:** used to define the top window menu.
- **Common Item Dialog:** used to allow the user to choose where to save their art or where to load it from.
- **Dynamic Link Libraries:** used to save and load .bmp file data.
## FAQ
### How does it work?
The drawing area is made up of owner drawn buttons that have their background color and border color changed. There is a separate array that keeps all the current background colors of the buttons. When a user interacts with the drawing area, the values in this array are modified and then a specific owner drawn button is redrawn based on the modified value.  
When gridline visibility is changed, the whole drawing area is redrawn. Gridlines are either black or white or of the color of an owner drawn button's background (*i.e.*, the gridline becomes a part of the button's background when gridline visibility is disabled).
### Why only 24x24?
The program was written without considering that having each drawing area pixel be represented as an owner drawn button is not the best solution when it comes to good performance. Furthermore, the number of buttons required to be drawn based on the dimensions of the drawing area grows exponentially. Thus different dimensions were tested to pick a suitable one for a satisfying user experience. 24x24 turned out to be a good compromise on the time it takes to draw and its size. 16x16 was also considered, but turned out to be too limiting in the size category. If the drawing area is small, not a lot of exciting artistic decisions can be made when painting. Therefore, a bigger drawing area is better, even if it gives a slight performance loss. However, the pixel art editor shouldn't take up too much of the user's time loading.
## winPixel in action
