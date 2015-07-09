//#include <easylogging++.h>
#include "window.hpp"
#include "event/window/window_events.hpp"
#include "event/event_type.hpp"
#include "event/event_manager.hpp"
#include <memory>

namespace gamesystem {

  using namespace event;
  using std::make_shared;
  
  void Window::InputManager::onWindowClosed(GLFWwindow *glfwWindow) {
  }

  void Window::InputManager::onWindowFramebufferResized(GLFWwindow *glfwWindow, int width, int height) {
    glViewport(0, 0, width, height);
    EventManager::get().queueEvent(make_shared<OnWindowResized>(width, height));
  }

  void Window::InputManager::onWindowResized(GLFWwindow *glfwWindow, int width, int height) {
  }

  void Window::InputManager::onKeyPressed(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods) {
    IEventDataSPtr eventDataSPtr = nullptr;
    if(key == GLFW_KEY_W || key == GLFW_KEY_UP) {
      if(action == GLFW_PRESS) {
        eventDataSPtr = make_shared<EventData>(EventType::KEYBOARD_UP_PRESSED);
      } else if(action == GLFW_RELEASE) {
        eventDataSPtr = make_shared<EventData>(EventType::KEYBOARD_UP_RELEASED);
      }
    } else if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
      if(action == GLFW_PRESS) {
        eventDataSPtr = make_shared<EventData>(EventType::KEYBOARD_LEFT_PRESSED);
      } else if(action == GLFW_RELEASE) {
        eventDataSPtr = make_shared<EventData>(EventType::KEYBOARD_LEFT_RELEASED);
      }
    } else if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
      if(action == GLFW_PRESS) {
        eventDataSPtr = make_shared<EventData>(EventType::KEYBOARD_RIGHT_PRESSED);
      } else if(action == GLFW_RELEASE) {
        eventDataSPtr = make_shared<EventData>(EventType::KEYBOARD_RIGHT_RELEASED);
      }
    } else if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
      if(action == GLFW_PRESS) {
        eventDataSPtr = make_shared<EventData>(EventType::KEYBOARD_DOWN_PRESSED);
      } else if(action == GLFW_RELEASE) {
        eventDataSPtr = make_shared<EventData>(EventType::KEYBOARD_DOWN_RELEASED);
      }
    } else if (key == GLFW_KEY_ESCAPE) {
      if(action == GLFW_PRESS) {
        eventDataSPtr = make_shared<EventData>(EventType::WINDOW_CLOSE);
      }
    } else if (key == GLFW_KEY_ENTER && (mods & GLFW_MOD_ALT)) {
      if(action == GLFW_PRESS) {
        eventDataSPtr = make_shared<EventData>(EventType::WINDOW_FULLSCREEN);
      }
    }
    if(eventDataSPtr!= nullptr)
      EventManager::get().queueEvent(eventDataSPtr);
  }

  void Window::InputManager::onMouseButton(GLFWwindow *glfwWindow, int button, int action, int mods) {
    IEventDataSPtr eventDataSPtr = nullptr;

    if(button == GLFW_MOUSE_BUTTON_LEFT) {
      if(action == GLFW_PRESS) {
        eventDataSPtr = make_shared<EventData>(EventType::MOUSE_LMB_PRESSED);
      } else if(action == GLFW_RELEASE) {
        eventDataSPtr = make_shared<EventData>(EventType::MOUSE_LMB_RELEASED);
      }
    } else if( button == GLFW_MOUSE_BUTTON_RIGHT) {
      if(action == GLFW_PRESS) {
        eventDataSPtr = make_shared<EventData>(EventType::MOUSE_RMB_PRESSED);
      } else if(action == GLFW_RELEASE) {
        eventDataSPtr = make_shared<EventData>(EventType::MOUSE_RMB_RELEASED);
      }
    }

    if(eventDataSPtr!= nullptr)
      EventManager::get().queueEvent(eventDataSPtr);
  }

  void Window::InputManager::onCursorPositionChanged(GLFWwindow *glfwWindow, double xPos, double yPos) {
    int width, height;
    glfwGetWindowSize(glfwWindow, &width, &height);
    int centerX = width >> 1, centerY = height >> 1;
    IEventDataSPtr eventDataSPtr = make_shared<OnCursorPositionChanged>(glm::vec2((xPos - centerX)/(width), (centerY - yPos)/(height)));
    glfwSetCursorPos(glfwWindow, centerX, centerY);
    EventManager::get().queueEvent(eventDataSPtr);
  }
}
