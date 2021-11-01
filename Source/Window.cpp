# include "Window.h"

Window::Window()
{
   width = 800;
   height = 600;

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
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

   mainWindow = glfwCreateWindow(width, height, "My window", NULL, NULL);
   if (!mainWindow)
   {
      glfwTerminate();
      return 1;
   }

   glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

   glfwMakeContextCurrent(mainWindow);
   //glfwSwapInterval(1);

   createCallbacks();
   glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
}

void Window::createCallbacks()
{
   glfwSetKeyCallback(mainWindow, handleKeys);
   glfwSetCursorPosCallback(mainWindow, handleMouse);
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
void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
   Window* theWindow = (Window*)(glfwGetWindowUserPointer(window));
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
Window::~Window() 
{
   glfwTerminate();
}
