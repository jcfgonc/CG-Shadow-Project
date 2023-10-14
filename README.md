# Shadow Mapping Engine

OpenGL Computer Graphics project I developed back in 2007 for the corresponding course 
at DEI/University of Coimbra. Got a score of 96/100.

.. image:: screenshots/panorama.jpg

This project was written in C/C++ and is a simple 3D engine.
The main focus was providing a base engine with shadow mapping, multi-texturing,
transparency support, Wavefront 3D object loading, multiple viewports,
and "first-person-shooter" controls for further works. The work supports
cubic interpolation (Catmull-Rom spline) that is used for controlling the
path and direction of both cameras and of the lights composing the scene. The shadow
mapping algorithm is done using the fixed-pipeline with a three pass rendering,
where the first pass creates the depth buffer from the light view, and both the
second and third passes render the scene with the objects either in the shadow
or lit. In order to show detailed shadows the engine renders the first pass
in a Frame Buffer Object and then uses the depth texture in the following passes.

You can do whatever you want with this code. Just give proper credit.

# Controls
Press space to enable/disable automatic camera.
Keys AWSD and arrows for movement, Q/E for rolling, Page Up/Down for moving up/down,
C to debug camera position and ESC to quit.

# Future work
- Convert C'eish code to C++
- Abandon GLee and move on to a better extension loading library
- Reduce shadow algorithm to two passes
- Use cube mapping to produce shadows for entire scene

# Help and tutorials from
- [Paul's Projects](http://www.paulsprojects.net/opengl/shadowmap/shadowmap.html)
- [Fabien Sanglard's Website](http://fabiensanglard.net/shadowmapping/index.php)

# Dependencies
- [Developer's Image Library (DevIL)](https://sourceforge.net/projects/openil/)
- [GLee (GL Easy Extension library)](https://sourceforge.net/projects/glee/)
- [FreeGLUT](https://freeglut.sourceforge.net)
- [Nate Robins' GLM](http://user.xmission.com/~nate/tutors.html)

Back then I got a GLM version that makes use of the DevIL library to load textures in other file formats.
I don't remember where I got it and I could not find it on the internet. That "rare" version is included in this project.

# Contact
João Carlos Ferreira Gonçalves
jcfgonc@gmail.com OR jcgonc@dei.uc.pt
