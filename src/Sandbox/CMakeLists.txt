add_executable(sandbox
        main.cpp
)

file(COPY shader.vert shader.frag DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(sandbox PRIVATE glad glfw imgui)
target_include_directories(sandbox PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad/include
        ${CMAKE_SOURCE_DIR}/external/glfw/include
)
