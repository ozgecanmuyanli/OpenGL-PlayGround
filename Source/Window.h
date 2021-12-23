#pragma once
#include "stdio.h"
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui-master/imgui.h>
#include <imgui-master/imgui_impl_glfw.h>
#include <imgui-master/imgui_impl_opengl3.h>
#include "Button.h"
#include "Camera.h"

class Window
{
public:
   GLFWwindow* mainWindow;

   Window();
   Window(GLint windowWidth, GLint windowHeight);
   GLint getBufferWidth() { return bufferWidth; }
   GLint getBufferHeight() { return bufferHeight; }
   bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
   void swapBuffers() { glfwSwapBuffers(mainWindow); }
   int Initialise();

   void setWindowCamera(Camera* pCamera);
   bool* getsKeys() { return keys; }
   double getCursorPosX();
   double getCursorPosY();
   GLFWwindow* getWindow() { return mainWindow; }
   static Window* Get() { return application; };
   static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
   void SetMousePos(GLFWwindow* window, float x, float  y);
   void DrawGUI();

   ~Window();

protected:
   static Window* application;
   bool mouseFirstMoved;
   double lastX;
   double lastY;

private:
   Camera* camera;

   GLint width, height;
   GLint bufferWidth, bufferHeight;
   bool keys[1024];
   double xPosition, yPosition; 
   
   void createCallbacks();
   static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
};
