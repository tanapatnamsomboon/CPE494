add_executable(a1
        main.cpp
        shader.cpp
)

file(COPY shader.vert shader.frag DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(a1 PRIVATE glad glfw)
target_include_directories(a1 PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad/include
        ${CMAKE_SOURCE_DIR}/external/glfw/include
)
