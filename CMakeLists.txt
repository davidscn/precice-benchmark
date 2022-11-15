##
#  CMake script for the coupled laplace problem tutorial program:
##

# Set the name of the project and target:
SET(TARGET "test")

# Declare all source files the target consists of. Here, this is only
# the one step-X.cc file, but as you expand your project you may wish
# to add other source files as well. If your project becomes much larger,
# you may want to either replace the following statement by something like
#    FILE(GLOB_RECURSE TARGET_SRC  "source/*.cc")
#    FILE(GLOB_RECURSE TARGET_INC  "include/*.h")
#    SET(TARGET_SRC ${TARGET_SRC}  ${TARGET_INC})
# or switch altogether to the large project CMakeLists.txt file discussed
# in the "CMake in user projects" page accessible from the "User info"
# page of the documentation.
SET(TARGET_SRC
  ${TARGET}.cc
  )
SET(CMAKE_CXX_STANDARD 17)
# Usually, you will not need to modify anything beyond this point...

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(${TARGET})
ADD_EXECUTABLE(${TARGET} ${TARGET_SRC})
FIND_PACKAGE(precice REQUIRED
  HINTS ${PRECICE_DIR}
  )
MESSAGE(STATUS "Using the preCICE version found at ${precice_DIR}")

include(FindPkgConfig)
TARGET_LINK_LIBRARIES(${TARGET} precice::precice)
find_package(benchmark REQUIRED)
target_link_libraries(${TARGET} benchmark::benchmark)
target_link_libraries(${TARGET} benchmark::benchmark_main)

