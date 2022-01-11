#include "Window.h"

Window* Window::application = NULL;

Window::Window()
{
   width = 1600;
   height = 900;

   for (size_t i = 0; i < 1024; i++)
   {
      keys[i] = 0;
   }
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
   application = this;
   width = windowWidth;
   height = windowHeight;

   for (size_t i = 0; i < 1024; i++)
   {
      keys[i] = 0;
   }
}
int Window::Initialise()
{
   int ret = glfwInit();
   if (!ret)
   {
      std::cout<<"Error initialising GLFW!"<<std::endl;
      glfwTerminate();
      return 1;
   }
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

   mainWindow = glfwCreateWindow(width, height, "Name of the game", NULL, NULL);
   if (!mainWindow)
   {
      glfwTerminate();
      return 1;
   }

   glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

   glfwMakeContextCurrent(mainWindow);
   glfwSwapInterval(1);

   createCallbacks();
   //glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   int gladSucces = 0;
   gladSucces = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
   if (!gladSucces)
   {
      std::cout << "Failed to initialize GLAD\n" << std::endl;
      return -1;
   }

   glEnable(GL_DEPTH_TEST);
   glDepthMask(GL_TRUE);
   glDepthFunc(GL_LESS);

   glViewport(0, 0, bufferWidth, bufferHeight);

   glfwSetWindowUserPointer(mainWindow, this);

   // Setup Dear ImGui context
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO(); (void)io;

   // Setup Dear ImGui style
   ImGui::StyleColorsDark();
   //ImGui::StyleColorsClassic();

   // Setup Platform/Renderer backends
   ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
   ImGui_ImplOpenGL3_Init("#version 150");
}

void Window::createCallbacks()
{
   glfwSetKeyCallback(mainWindow, handleKeys);
   glfwSetCursorPosCallback(mainWindow, MouseCallback);
}

void Window::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
   // for camera movement with right click
   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
   {
      Get()->SetMousePos(window, xpos, ypos);
   }
   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
   {
      Get()->mouseFirstMoved = true;
   }

   // for button click
   glfwGetCursorPos(window, &xpos, &ypos);
   Get()->xPosition = xpos;
   Get()->yPosition = ypos;
}

void Window::SetMousePos(GLFWwindow* window, float x, float  y)
{
   if (mouseFirstMoved)
   {
      lastX = x;
      lastY = y;
      mouseFirstMoved = false;
   }

   float xoffset = x - lastX;
   float yoffset = lastY - y; 

   lastX = x;
   lastY = y;

   this->camera->mouseControl(xoffset, yoffset);
}

void Window::setWindowCamera(Camera* pCamera)
{
   this->camera = pCamera;
}

double Window::getCursorPosX()
{
   return (xPosition- 800);
}
double Window::getCursorPosY()
{
   return (450 - yPosition);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
   Window* theWindow = (Window*)(glfwGetWindowUserPointer(window));
   //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
   //{
   //   glfwSetWindowShouldClose(window, GL_TRUE);
   //}
   if (key >= 0 && key < 1024)
   {
      if (action == GLFW_PRESS)
      {
         theWindow->keys[key] = true;
      }
      else if (action == GLFW_RELEASE)
      {
         theWindow->keys[key] = false;
      }
   }
}

void Window::DrawGUI()
{

}

Window::~Window() 
{
   //glfwTerminate();
}
