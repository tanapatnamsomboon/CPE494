add_library(common STATIC
        src/shader.cpp
        src/texture.cpp
        src/camera.cpp
        src/utils.cpp
        src/shape.cpp
)

target_include_directories(common
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(common
        PRIVATE
        glfw
        glad
        glm
        stb
)