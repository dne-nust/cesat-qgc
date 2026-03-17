# custom.cmake — CESAT QGC build integration
# This file is included by the QGC CMake build system via the custom build hook.
# It adds CESAT-specific sources without modifying upstream CMakeLists.txt.

message(STATUS "CESAT: Loading custom CESAT QGC extensions")

# Include CESAT UI module
add_subdirectory(${CMAKE_SOURCE_DIR}/src/CesatUI)

# Include CESAT MAVLink dialect headers
# (Generated from mavlink/cesat_dialect/cesat.xml via pymavlink)
include_directories(${CMAKE_SOURCE_DIR}/mavlink/cesat_dialect/generated/c)

# TODO: Add dialect generation step at M2
# find_package(Python3 REQUIRED)
# add_custom_command(OUTPUT ...)
