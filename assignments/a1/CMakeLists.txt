add_executable(a1
        main.cpp
)

target_link_libraries(a1 PRIVATE common glfw glad glm)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})