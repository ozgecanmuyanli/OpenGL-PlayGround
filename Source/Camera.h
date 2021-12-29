#pragma once
//#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera
{
public:
   Camera();
   Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

   void drawCameraGUI();
   void keyControl(bool* keys, GLfloat deltaTime);
   void mouseControl(float xChange, float yChange);

   glm::vec3 getCameraPosition();
   glm::mat4 calculateViewMatrix();
   void ApplyHeadBob(GLfloat deltaTime);

   float accAcc = 0.0;
   float maxAcc = 0.0;
   float maxSpeed = 0.0;

   ~Camera();

private:
   glm::vec3 position;
   glm::vec3 front;
   glm::vec3 up;
   glm::vec3 right;
   glm::vec3 worldUp;

   GLfloat yaw;
   GLfloat pitch;

   GLfloat moveSpeed;
   GLfloat turnSpeed;

   GLfloat velocityW = 0.0f;
   GLfloat velocityA = 0.0f;
   GLfloat velocityS = 0.0f;
   GLfloat velocityD = 0.0f;
   GLfloat velocity = 0.0f;
   void update();
};
