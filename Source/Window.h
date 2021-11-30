#include "stdio.h"
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui-master/imgui.h>
#include <imgui-master/imgui_impl_glfw.h>
#include <imgui-master/imgui_impl_opengl3.h>


class Window
{
public:
   Window();
   Window(GLint windowWidth, GLint windowHeight);

   int Initialise();
   GLint getBufferWidth() { return bufferWidth; }
   GLint getBufferHeight() { return bufferHeight; }

   bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

   bool* getsKeys() { return keys; }
   GLfloat getXChange();
   GLfloat getYChange();

   void swapBuffers() { glfwSwapBuffers(mainWindow); }

   ~Window();

private:
   GLFWwindow* mainWindow;

   GLint width, height;
   GLint bufferWidth, bufferHeight;

   bool keys[1024];

   GLfloat lastX, lastY, xChange, yChange;
   bool mouseFirstMoved;

   void createCallbacks();
   static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
   static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};
