add_executable(a2
        main.cpp
        terrain.cpp
)

target_link_libraries(a2
        PRIVATE
        common
        glfw
        glad
        glm
        stb
        imgui
)

file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

if (UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(a2 PRIVATE
            X11 Xrandr Xinerama Xcursor Xi
            dl pthread
    )
endif ()