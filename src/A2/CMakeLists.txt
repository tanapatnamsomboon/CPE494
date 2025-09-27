add_executable(A2
        main.cpp
)

file(COPY shader.vert shader.frag DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(A2 PRIVATE
        glad
        glfw
        imgui
        glm::glm
        assimp::assimp
        common
)
