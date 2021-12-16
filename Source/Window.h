#pragma once
#include "stdio.h"
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui-master/imgui.h>
#include <imgui-master/imgui_impl_glfw.h>
#include <imgui-master/imgui_impl_opengl3.h>
#include "Button.h"


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
   double getCursorPosX();
   double getCursorPosY();
   GLFWwindow* getWindow() { return mainWindow; }

   void swapBuffers() { glfwSwapBuffers(mainWindow); }

   ~Window();

private:
   GLFWwindow* mainWindow;

   GLint width, height;
   GLint bufferWidth, bufferHeight;

   bool keys[1024];

   GLfloat lastX, lastY, xChange, yChange;
   double xPosition, yPosition;
   bool mouseFirstMoved;
   
   void createCallbacks();
   static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
   static void handleMouseMove(GLFWwindow* window, double xPos, double yPos);
   static void handleMouseButton(GLFWwindow* window,int, int, int);
};
