add_executable(OpenGL-Playground 
main.cpp
Window.cpp
Mesh.cpp
Texture.cpp
Camera.cpp
stb_image.cpp
)

target_link_libraries(OpenGL-Playground
PRIVATE
GLAD
glfw
)

target_include_directories(OpenGL-Playground
 PRIVATE
 "${PROJECT_SOURCE_DIR}/Dependencies"
)
