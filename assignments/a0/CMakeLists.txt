add_executable(a0
        main.cpp
)

target_link_libraries(a0 PRIVATE
        common
        glfw
        glad
        glm
)

file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})