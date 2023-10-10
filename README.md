# Shadow Mapping Engine

This project was written in C/C++ and is a simple modular 3D engine.
The main focus was providing a base engine with shadow mapping, multi-texturing,
transparency support, Wavefront 3D object loading, multiple viewports,
and “first-person-shooter” controls for further works. The work supports
cubic interpolation (Catmull-Rom spline) which is used for controlling the
path and direction of both cameras and lights composing the scene. The shadow
mapping algorithm is done using the fixed-pipeline with a three pass rendering,
where the first pass creates the depth buffer from the light view, and both the
second and third passes render the scene with the objects either in the shadow
or lit. In order to show detailed shadows the engine renders the first pass
in a Frame Buffer Object and then uses the depth texture in the following passes.

# Future work:
- Reduce shadow algorithm to two passes;
- Use cube mapping to produce shadows for entire scene.

Help and tutorials from:
	http://www.paulsprojects.net/opengl/shadowmap/shadowmap.html
	http://fabiensanglard.net/shadowmapping/index.php

# Controls
Press space to enable/disable automatic camera.
Keys AWSD and arrows for movement, Q/E for rolling, Page Up/Down for moving up/down, C to debug camera position and ESC to quit.


(C) 2007-2023 João Carlos Ferreira Gonçalves
jcfgonc@gmail.com OR jcgonc@dei.uc.pt
