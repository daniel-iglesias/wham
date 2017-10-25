IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Make sure the compiler can find include files.
include_directories (
${MkniX_SOURCE_DIR}/src 
${MkniX_SOURCE_DIR}/libmknix 
${LMX_INCLUDE_DIR}/ 
# ${VTK_INCLUDE_DIR}
${GMM_INCLUDE_DIR}
)

# Make sure the linker can find the Hello library once it is built.
link_directories (${MkniX_BINARY_DIR}/libmknix)

add_executable (wham wham.cpp)
add_executable (wham-files wham-files.cpp)


# link_directories ( ${VTK_DIR})
# Link the executable to the VTK libraries.
# target_link_libraries ( mknixlib
#   vtkRendering
#   vtkGraphics
#   vtkIO
#   vtkCommon
# )

link_directories ( ${PROJECT_BINARY_DIR})
# Link the executable to the VTK libraries.
target_link_libraries ( wham
   mknixlib
)
target_link_libraries ( wham-files
   mknixlib
)
