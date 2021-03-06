# compile in debug mode
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Add an option to build as shared lib
OPTION(BUILD_MKNIXLIB_SHARED "Build the Mknix libraries shared." ON)

IF(BUILD_MKNIXLIB_SHARED)
  SET(MKNIXLIB_LIBRARY_TYPE SHARED)
ELSE(BUILD_MKNIXLIB_SHARED)
  SET(MKNIXLIB_LIBRARY_TYPE STATIC)
ENDIF(BUILD_MKNIXLIB_SHARED)


# FIND_PACKAGE(VTK)
# IF(VTK_FOUND)
#   INCLUDE(${VTK_USE_FILE})
#   INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIR})
# ELSE(VTK_FOUND)
# #  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
#   FIND_PATH(VTK_DIR VTKConfig.cmake /usr/local/lib/vtk-5.4)
#   INCLUDE(${VTK_USE_FILE})
# #  FIND_PATH(VTK_INCLUDE_DIR vtkActor.h /usr/local/include/vtk-5.4)
# ENDIF(VTK_FOUND)
# 
# FIND_PATH(LMX_INCLUDE_DIR HINTS ${PROJECT_SOURCE_DIR}/LMX-1.0/src NAMES LMX/lmx.h)

OPTION(USE_GMM
	"Allows to use the GMM matrix types"
	ON)
IF(USE_GMM)
	FIND_PATH(GMM_INCLUDE_DIR HINTS ${PROJECT_SOURCE_DIR}/gmm/gmm-3.0/include/ NAMES gmm/gmm.h)
	ADD_DEFINITIONS(-DHAVE_GMM)
ENDIF(USE_GMM)

#SET(LMX_INCLUDE_DIR ${MKNIX_LMX_INCLUDE_DIR} CACHE FILEPATH "")
FIND_PATH(LMX_INCLUDE_DIR HINTS ${PROJECT_SOURCE_DIR}/LMX-1.0/src/ NAMES LMX/lmx.h)

include( CPack )

# Make sure the compiler can find include files from our Hello library.
include_directories (
${MkniX_SOURCE_DIR}/libmknix
${LMX_INCLUDE_DIR}/ 
# ${VTK_INCLUDE_DIR}
${GMM_INCLUDE_DIR}
)

# Make sure the linker can find the Hello library once it is built.
#link_directories (${MkniX_BINARY_DIR}/libs)

SET(MknixLibSrcs 
  analysis.cpp
  analysisthermaldynamic.cpp
  body.cpp
  bodyflex.cpp
  bodyflexglobalgalerkin.cpp
  cell.cpp
  celltriang.cpp
  common.cpp
  elemtriangle.cpp
  gausspoint.cpp
  gausspoint2D.cpp
  loadthermal.cpp
  material.cpp
  node.cpp
  point.cpp
  reader.cpp
  readerflex.cpp
  shapefunction.cpp
  shapefunctiontriangle.cpp
  simulation.cpp
  system.cpp
)


# Add executable called "helloDemo" that is built from the source files
# "demo.cxx" and "demo_b.cxx".  The extensions are automatically found.
add_library (mknixlib ${MKNIXLIB_LIBRARY_TYPE} ${MknixLibSrcs})

INSTALL( TARGETS mknixlib DESTINATION lib )

