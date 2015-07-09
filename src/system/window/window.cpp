#include <string>
#include <easylogging++.h>
#include "window.hpp"
#include "event/event_manager.hpp"

namespace gamesystem {

  using std::string;
  class Game;

  Window::Window(int width, int height, string title, bool fullScreen) :
    _inputManagerUPtr(std::unique_ptr<InputManager>(new InputManager())) {
    glfwSetErrorCallback(onError);
    glfwInit();
    //glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (!fullScreen)
      _glfwWindowPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    else
      _glfwWindowPtr = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);

    glfwMakeContextCurrent(_glfwWindowPtr);

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


    glfwSetWindowUserPointer(_glfwWindowPtr, this);
    glfwSetWindowCloseCallback(_glfwWindowPtr, _inputManagerUPtr->onWindowClosed);
    glfwSetWindowFocusCallback(_glfwWindowPtr, _inputManagerUPtr->onWindowFocused);
    glfwSetWindowSizeCallback(_glfwWindowPtr, _inputManagerUPtr->onWindowResized);
    glfwSetWindowIconifyCallback(_glfwWindowPtr, _inputManagerUPtr->onWindowIconify);
    glfwSetWindowPosCallback(_glfwWindowPtr, _inputManagerUPtr->onWindowPositionChanged);
    glfwSetFramebufferSizeCallback(_glfwWindowPtr, _inputManagerUPtr->onWindowFramebufferResized);
    glfwSetWindowRefreshCallback(_glfwWindowPtr, _inputManagerUPtr->onWindowRefreshed);
    glfwSetKeyCallback(_glfwWindowPtr, _inputManagerUPtr->onKeyPressed);
    glfwSetMouseButtonCallback(_glfwWindowPtr, _inputManagerUPtr->onMouseButton);
    glfwSetScrollCallback(_glfwWindowPtr, _inputManagerUPtr->onScroll);
    glfwSetCursorEnterCallback(_glfwWindowPtr, _inputManagerUPtr->onCursorEnter);
    glfwSetCursorPosCallback(_glfwWindowPtr, _inputManagerUPtr->onCursorPositionChanged);

    _inputManagerUPtr->onWindowResized(_glfwWindowPtr, width, height);

    auto& eventManager = event::EventManager::get();
    eventManager.addListener(event::EventType::WINDOW_FULLSCREEN, [this](event::IEventDataSPtr& eventData)
                             {
                               const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                               auto window_width = mode->width;
                               auto window_height = mode->height;
                               glfwSetWindowSize(_glfwWindowPtr, window_width, window_height);
                             });
    eventManager.addListener(event::EventType::WINDOW_CLOSE, [this](event::IEventDataSPtr& eventData)
                             {
                               glfwSetWindowShouldClose(_glfwWindowPtr, 1);
                             });
  }


  Window::~Window() {
    glfwTerminate();
  }

  void Window::onError(int errorCode, const char *description) {
    LOG(ERROR)<<errorCode<<" "<<description;
  }

  void Window::swapBuffers() {
    glfwSwapBuffers(_glfwWindowPtr);
  }

  void Window::pollEvents() {
    glfwPollEvents();
  }

  void Window::clear() {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}
