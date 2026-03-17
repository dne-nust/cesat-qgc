set(CESAT_SOURCES
    CesatMavlinkHandler.cpp   # TODO: Implement at M2
    CesatMavlinkHandler.h
    # QML panels (registered as Qt resources)
    # cesat_fpga_status.qml   # TODO: M2
    # cesat_imu_status.qml    # TODO: M2
    # cesat_algorithm_status.qml  # TODO: M3
    # cesat_estimator_state.qml   # TODO: M3
)

target_sources(${QGC_APP_NAME} PRIVATE ${CESAT_SOURCES})
target_include_directories(${QGC_APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
