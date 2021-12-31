target_sources(MVRE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SpaceObject.cpp)
target_sources(MVRE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SpaceManager.cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Ship)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Asteroids)