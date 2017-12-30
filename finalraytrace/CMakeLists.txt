include_directories(PNG_INCLUDE_DIR)
find_package(PNG REQUIRED)
find_package(Qt5Core)
find_package(Qt5Gui)

set(HDRS common.h png_reader.h png_writer.h
  rgbColor.h rgbImage.h ray.h shape.h 
  material.h view.h light.h
  parser.h raytracer.h sphere.h scene.h material.h plane.h rectangle.h triangle.h octree.h objparser.h
)

set(LIBSRCS common.cpp png_reader.cpp png_writer.cpp 
  rgbImage.cpp ray.cpp parser.cpp raytracer.cpp sphere.cpp plane.cpp rectangle.cpp triangle.cpp octree.cpp shape.cpp objparser.cpp
)

#  add these to library above when ready
#  sphere.h sphere.cpp
#  rectangle.h rectangle.cpp
#  triangle.h triangle.cpp
  
add_library(rtrace ${HDRS} ${LIBSRCS})

add_executable(makescene ${HDRS} makescene.cpp)

set(ALLLIBS rtrace ${EXT_LIBS} ${PNG_LIBRARIES})
target_link_libraries(makescene ${ALLLIBS} Qt5::Core Qt5::Gui)

symlink_data(raytracer *.txt)
symlink_data(objects *.obj)
