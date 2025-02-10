cmake_minimum_required(VERSION 3.30)
project(melee LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(melee src/main.cpp
        src/client/level/node.cpp
        src/client/level/node.h
        src/client/level/level.cpp
        src/client/level/level.h
        src/client/level/chunk.cpp
        src/client/level/chunk.h
        src/client/client.cpp
        src/client/client.h
        src/client/util/game_math.h
        src/client/render/greedy_mesh.cpp
        src/client/render/greedy_mesh.h
        src/client/render/wireframe.h
        src/client/entity/player.cpp
        src/client/entity/player.h
        src/client/controller/player_movement_controller.cpp
        src/client/controller/player_movement_controller.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# raylib requires this additional configuration for macOS
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
endif()