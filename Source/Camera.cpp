# include "Camera.h"
#include <imgui-master/imgui.h>
#include <imgui-master/imgui_impl_glfw.h>
#include <imgui-master/imgui_impl_opengl3.h>

Camera::Camera() { }

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
   position = startPosition;
   worldUp = startUp;
   yaw = startYaw;
   pitch = startPitch;
   front = glm::vec3(0.0f, 0.0f, -1.0f);

   moveSpeed = startMoveSpeed;
   turnSpeed = startTurnSpeed;

   accAcc = 0.02;
   maxAcc = 5.3f;
   maxSpeed = 0.6f;

   update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
   static float a = 0.0f; 

   if (keys[GLFW_KEY_W])
   {
      a += accAcc;
      if (a >= maxAcc)
      {
         a = maxAcc;
      }
      velocityW += a;
      if (velocityW >= maxSpeed)
      {
         velocityW = maxSpeed;
      }

      position.x += front.x * (velocityW)*deltaTime;
      position.z += front.z * (velocityW)*deltaTime;
   }
   else
   {
      a -= accAcc;
      if (a <= 0.0f)
      {
         a = 0.015f;
      }
      velocityW -= a;
      if (velocityW <= 0.0f)
      {
         velocityW = 0.0f;
      }

      position.x += front.x * velocityW * deltaTime;
      position.z += front.z * velocityW * deltaTime;
   }
   if (keys[GLFW_KEY_S])
   {
      a += accAcc;
      if (a >= maxAcc)
      {
         a = maxAcc;
      }
      velocityS += a;
      if (velocityS >= maxSpeed)
      {
         velocityS = maxSpeed;
      }
 
      position.x -= front.x * velocityS * deltaTime;
      position.z -= front.z * velocityS * deltaTime;
   }
   else
   {
      a -= accAcc;
      if (a <= 0.0f)
      {
         a = 0.015f;
      }
      velocityS -= a;
      if (velocityS <= 0.0f)
      {
         velocityS = 0.0f;
      }

      position.x -= front.x * velocityS * deltaTime;
      position.z -= front.z * velocityS * deltaTime;
   }
   if (keys[GLFW_KEY_A])
   {
      a += accAcc;
      if (a >= maxAcc)
      {
         a = maxAcc;
      }
      velocityA += a;
      if (velocityA >= maxSpeed)
      {
         velocityA = maxSpeed;
      }

      position.x -= right.x * velocityA * deltaTime;
      position.z -= right.z * velocityA * deltaTime;
   }
   else
   {
      a -= accAcc;
      if (a <= 0.0f)
      {
         a = 0.015f;
      }
      velocityA -= a;
      if (velocityA <= 0.0f)
      {
         velocityA = 0.0f;
      }

      position.x -= right.x * velocityA * deltaTime;
      position.z -= right.z * velocityA * deltaTime;
   }
   if (keys[GLFW_KEY_D])
   {
      a += accAcc;
      if (a >= maxAcc)
      {
         a = maxAcc;
      }
      velocityD += a;
      if (velocityD >= maxSpeed)
      {
         velocityD = maxSpeed;
      }

      position.x += right.x * velocityD * deltaTime;
      position.z += right.z * velocityD * deltaTime;
   }
   else
   {
      a -= accAcc;
      if (a <= 0.0f)
      {
         a = 0.015f;
      }
      velocityD -= a;
      if (velocityD <= 0.0f)
      {
         velocityD = 0.0f;
      }

      position.x += right.x * velocityD * deltaTime;
      position.z += right.z * velocityD * deltaTime;
   }

   if (keys[GLFW_KEY_Q])
   {
      position += up * 1.25f * deltaTime;
   }
   if (keys[GLFW_KEY_E])
   {
      position -= up * 1.25f * deltaTime;
   }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
   xChange *= turnSpeed;
   yChange *= turnSpeed;

   yaw += xChange;
   pitch += yChange;

   if (pitch > 30.0f)
   {
      pitch = 30.0f;
   }
   if (pitch < -30.0f)
   {
      pitch = -30.0f;
   }

   update();
}

glm::mat4 Camera::calculateViewMatrix()
{
   return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
   return position;
}

void Camera::ApplyHeadBob(GLfloat deltaTime)
{
   float bobOscillate;
   
   if ((velocityW > 0.01f) || (velocityA > 0.01f) || (velocityS > 0.01f) || (velocityD > 0.01f))
   {
      velocity = maxSpeed;
      bobOscillate = sin((float)glfwGetTime() * velocity * (2 * 3.14f)) * 0.0005f;
      position.y += bobOscillate;
   }
}

void Camera::update()
{
   front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   front.y = sin(glm::radians(pitch));
   front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   front = glm::normalize(front);

   right = glm::normalize(glm::cross(front, worldUp));
   up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {}
