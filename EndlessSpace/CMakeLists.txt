add_executable(EndlessSpace src/Main.cpp)

add_subdirectory(src)

add_custom_command(TARGET EndlessSpace PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/EndlessSpace/src/Data $<TARGET_FILE_DIR:EndlessSpace>/Data)

target_link_libraries(EndlessSpace glfw)
target_link_libraries(EndlessSpace OpenGL::GL)
target_link_libraries(EndlessSpace ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(EndlessSpace OpenCL)

target_link_libraries(EndlessSpace GameEngine)

target_link_libraries(EndlessSpace ${FREETYPE_LIBRARIES})
target_include_directories(EndlessSpace PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_compile_options(EndlessSpace PUBLIC ${FREETYPE_CGLAGS_OTHER})