add_executable(OpenGL-Playground 
main.cpp
Window.cpp
Mesh.cpp
Texture.cpp
Camera.cpp
stb_image.cpp
Button.cpp
State.cpp
MainWindowState.cpp
CreditsWindowState.cpp
GamePlayWindowState.cpp
PauseWindowState.cpp
)

link_directories(${PROJECT_SOURCE_DIR}/Dependencies/assimp-5.0.1/lib/Debug)

target_link_libraries(OpenGL-Playground
PRIVATE
GLAD
IMGUI
glfw
PUBLIC
assimp-vc142-mtd.lib
)

target_include_directories(OpenGL-Playground
 PRIVATE
 "${PROJECT_SOURCE_DIR}/Dependencies"
 "${PROJECT_SOURCE_DIR}/Dependencies/assimp-5.0.1/include"
 #"${PROJECT_SOURCE_DIR}/Dependencies/Assimp/lib/Debug"
)
