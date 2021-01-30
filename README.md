# QuadTreeTerrain
The first step towards rendering planets in OpenGL. QuadTree LOD based on camera position and an LOD equation.
Credit to Thatcher Ulrich's paper: http://tulrich.com/geekstuff/sig-notes.pdf 
for the LOD equation, as well as general QuadTree rendering pseudocode.

Written in OpenGL and C++. Uses GLEW and GLFW, and the stb_image header only library.
img folder contains two sample heightmaps.

I wrote this as a step towards rendering planets, which will use 6 quadtrees (1 for each cube face)
which will then be projected onto a sphere.
