# Ray Tracing Complex Objects Using Octrees
This project served as my final project for my Computer Graphics course in Fall 2014.
Authors: Josh Wolper and Jacob Carstenson
Supervisor: Andrew Danner

## Content Description
Throughout the year in this course we worked on a C++ based ray tracer. The issue with ray tracers is that when you add more 
objects to the scene, this greatly reduces the speed of ray tracing because each light ray needs to check with each object in the scene
to see if there is a collision. Our solution to this problem involves implementing an octree. Octrees function similarly to binary trees
but are used to partition 3D space because each parent node is divided once in each dimension giving 2<sup>3</sup>, or 8, children called octants.
Depending on how busy each octanct is, if a certain threshold number of objects is passed then this octant can be subdivided into 8 more
child octants, so on and so forth. Now raytracing is a matter of determining position and then traversing this octree which gives a
significant speedup.

To read more in depth about our project and the inner working of the implementation and testing of the code, please read
our paper located in paper/paper.pdf.
