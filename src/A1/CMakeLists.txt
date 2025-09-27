add_executable(A1
        main.cpp
)

file(COPY shader.vert shader.frag DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(A1 PRIVATE glad glfw common)
