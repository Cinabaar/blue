#pragma once


#include "easylogging++.h"
#include "window/window.hpp"
#include "util/util.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif
#include <memory>

namespace gamesystem
{
  class OpenGLSystem : public NonCopyable<OpenGLSystem>
  {
  private:
    std::unique_ptr<Window> _windowUPtr;
  public:
    OpenGLSystem();
    static OpenGLSystem& get() {
      static OpenGLSystem instance;
      return instance;
    }
    Window* getWindow() const {
      return _windowUPtr.get();
    };
    const void* getCurrentDisplay() {
#ifdef _WIN32
      return wglGetCurrentDC();
#else
      return glXGetCurrentDisplay();
#endif
    }
    const void* getCurrentContext() {
#ifdef _WIN32
      return wglGetCurrentContext();
#else
      return glXGetCurrentContext();
#endif
    }
    GLuint createShaderProgram(std::vector<std::string> shaders);
    bool tryCreateShaderProgram(std::vector<GLuint> shaders, GLuint& shaderProgram, std::string& error);
    bool tryLoadShader(std::string fileName, GLenum shaderType, GLuint& shader, std::string& error);
    void printShaderProgramInfo(GLuint shaderProgram);
    std::string glEnumToString(GLenum glenum);
  };
}
