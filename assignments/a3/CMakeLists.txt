add_executable(a3
        main.cpp
        game.cpp
        player.cpp
        object.cpp
        entity/ground_plane.cpp
        entity/static_object.cpp
        follow_camera.cpp
)

target_link_libraries(a3
        PRIVATE
        common
        glfw
        glad
        glm
        stb
        imgui
)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
        COMMENT "Copying assets to build directory..."

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMENT "Copying assets to build directory..."
)

add_dependencies(a3 copy_assets)

if (UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(a3 PRIVATE
            X11 Xrandr Xinerama Xcursor Xi
            dl pthread
    )
endif ()