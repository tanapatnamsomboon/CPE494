add_library(common STATIC
        src/Model.cpp
        src/Mesh.cpp
        src/Shader.cpp
)

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(common PUBLIC
    glad
)
