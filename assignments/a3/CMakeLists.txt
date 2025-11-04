add_executable(a3
        src/main.cpp
        src/game.cpp
        src/player.cpp
        src/object.cpp
        src/entity/ground_plane.cpp
        src/entity/static_object.cpp
        src/follow_camera.cpp
        src/systems/item_database.cpp
        src/systems/inventory.cpp
        src/entity/item_pickup.cpp
        src/systems/item_spawner.cpp
        src/systems/world_spawner.cpp
        src/entity/gatherable.cpp
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

target_include_directories(a3
        PRIVATE
        src
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