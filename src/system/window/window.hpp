#pragma once

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/util.hpp"

namespace gamesystem {

  using std::unique_ptr;
  
  class Window : public NonCopyable<Window> {
  private:
    class InputManager;

    GLFWwindow *_glfwWindowPtr;
    unique_ptr<InputManager> _inputManagerUPtr;

  public:
    Window(int width, int height, std::string title, bool fullScreen = false);
    ~Window();
    void swapBuffers();
    void pollEvents();
    void clear();

  private:
    class InputManager {
    private:

    public:
      static void onWindowClosed(GLFWwindow *glfwWindow);
      /*
        [in]	focused	GL_TRUE if the window was focused, or GL_FALSE if it was defocused.
      */
      static void onWindowFocused(GLFWwindow *glfwWindow, int focused) {}
      /*
        [in]	width	The new width, in pixels, of the framebuffer.
        [in]	height	The new height, in pixels, of the framebuffer.
      */
      static void onWindowFramebufferResized(GLFWwindow *glfwWindow, int width, int height);
      /*
        [in]	iconified	GL_TRUE if the window was iconified, or GL_FALSE if it was restored.
      */
      static void onWindowIconify(GLFWwindow *glfwWindow, int iconified) {}
      /*
        [in]	xpos	The new x-coordinate, in screen coordinates, of the upper-left corner of the client area of the window.
        [in]	ypos	The new y-coordinate, in screen coordinates, of the upper-left corner of the client area of the window.
      */
      static void onWindowPositionChanged(GLFWwindow *glfwWindow, int x_pos, int y_pox) {}
      static void onWindowRefreshed(GLFWwindow *glfwWindow) {}
      /*
        [in]	width	The new width, in screen coordinates, of the window.
        [in]	height	The new height, in screen coordinates, of the window.
      */
      static void onWindowResized(GLFWwindow *glfwWindow, int width, int height);
      /*
        [in]	key	The keyboard key that was pressed or released.
        [in]	scancode	The system-specific scancode of the key.
        [in]	action	GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
        [in]	mods	Bit field describing which modifier keys were held down.
      */
      static void onKeyPressed(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods);
      /*
        [in]	button	The mouse button that was pressed or released.
        [in]	action	One of GLFW_PRESS or GLFW_RELEASE.
        [in]	mods	Bit field describing which modifier keys were held down.
      */
      static void onMouseButton(GLFWwindow *glfwWindow, int button, int action, int mods);
      /*
        [in]	xoffset	The scroll offset along the x-axis.
        [in]	yoffset	The scroll offset along the y-axis.
      */
      static void onScroll(GLFWwindow *glfwWindow, double x_offset, double y_offset) {}
      /*
        [in]	entered	GL_TRUE if the cursor entered the window's client area, or GL_FALSE if it left it.
      */
      static void onCursorEnter(GLFWwindow *glfwWindow, int entered) {}
      /*
        [in]	xpos	The new x-coordinate, in screen coordinates, of the cursor.
        [in]	ypos	The new y-coordinate, in screen coordinates, of the cursor.
      */
      static void onCursorPositionChanged(GLFWwindow *glfwWindow, double xPos, double yPos);
    };
    static void onError(int errorCode, const char *description);
  };

}
