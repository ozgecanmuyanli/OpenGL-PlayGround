#include "Window.h"


Window::Window()
{
   width = 1600;
   height = 900;

   for (size_t i = 0; i < 1024; i++)
   {
      keys[i] = 0;
   }
   xChange = 0.0f;
   yChange = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
   width = windowWidth;
   height = windowHeight;

   for (size_t i = 0; i < 1024; i++)
   {
      keys[i] = 0;
   }
   xChange = 0.0f;
   yChange = 0.0f;
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
   glfwSetCursorPosCallback(mainWindow, handleMouseMove);
   glfwSetMouseButtonCallback(mainWindow, handleMouseButton);
}

GLfloat Window::getXChange()
{
   GLfloat thChange = xChange;
   xChange = 0.0f;
   return thChange;
}

GLfloat Window::getYChange()
{
   GLfloat thChange = yChange;
   yChange = 0.0f;
   return thChange;
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
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, GL_TRUE);
   }
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

void Window::handleMouseMove(GLFWwindow* window, double xPos, double yPos)
{
   Window* theWindow = (Window*)(glfwGetWindowUserPointer(window));
   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
   {
      if (theWindow->mouseFirstMoved)
      {
         theWindow->lastX = xPos;
         theWindow->lastY = yPos;
         theWindow->mouseFirstMoved = false;
      }
      theWindow->xChange = xPos - theWindow->lastX;
      theWindow->yChange = theWindow->lastY - yPos;
      theWindow->lastX = xPos;
      theWindow->lastY = yPos;
   }

   glfwGetCursorPos(window, &xPos, &yPos);
   theWindow->xPosition = xPos;
   theWindow->yPosition = yPos;

}

void Window::handleMouseButton(GLFWwindow* window, int, int, int)
{
   Window* theWindow = (Window*)(glfwGetWindowUserPointer(window));

   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
   {
      theWindow->mouseFirstMoved = true;
   }
   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
   {
      theWindow->isButtonClicked = true;
   }
   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
   {
      theWindow->isButtonClicked = false;
   }
}

Window::~Window() 
{
   //glfwTerminate();
}
