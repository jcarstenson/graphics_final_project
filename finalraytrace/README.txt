Our project uses the same input file style from the midterm project. See our file, "finalobjtest.txt" for an example of how to write input files. The new commands are:

"obj" - used as: obj <.obj file>
"oct" - simply used as: oct <origin defined as x y z> <halfDim>

Please note that using the oct command does make the rendering faster BUT does not always render complete images.

In addition, in our octree.h one of the first defined values is maxData and this determines how many shapes can be entered into a given node of the octree. If this is set too low the scene will definitely have issues and that's what we don't know how to fix.

The CMakeLists.txt in ~/cs40/projects/ was changed to build this lab. "add_subdirectory(raytrace)" was commented out and "add_subdirectory(final/finalraytrace)" was added.

In order to render objects on the image, we must also have the correct translations, scales, and rotations set up in our object parser. Here are some settings for the dragon

        
Dragon Settings in objparser.cpp -- use finalobjtest.txt as an input file and make sure you have the following settings set up in objparser.cpp:
        v1 *= 5;
        v2 *= 5;
        v3 *= 5;

        v1 = v1;
        float temp = v2;
        v2 = v3;
        v3 = temp * -1;

        v2 -= 4.5;

        v1 -= 4.5;

