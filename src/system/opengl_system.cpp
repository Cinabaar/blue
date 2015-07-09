#include "opengl_system.hpp"
#undef None
#include "format.h"
namespace gamesystem {
  OpenGLSystem::OpenGLSystem() {
    _windowUPtr = std::unique_ptr<Window>(new Window(800, 600, "SPH", false));
  }

  GLuint OpenGLSystem::createShaderProgram(std::vector<std::string> shaders)
  {
    GLuint shaderProgram;
    std::vector<GLuint> s(shaders.size());
    std::string error;

    for(auto& shader : shaders)
    {
      int i=&shader-&shaders[0];
      GLenum shaderType;
      if(shader.find(".vert")!=std::string::npos)
      {
        shaderType = GL_VERTEX_SHADER;
      }
      else if(shader.find(".frag")!=std::string::npos)
      {
        shaderType = GL_FRAGMENT_SHADER;
      }
      else if(shader.find(".geom")!=std::string::npos)
      {
        shaderType = GL_GEOMETRY_SHADER;
      }
      if(!tryLoadShader(shader, shaderType, s[i], error))
      {
        LOG(ERROR) << fmt::format("Unable to load {0}\n{1}", shader, error);
      }
      else
      {
        LOG(INFO) << fmt::format("Shader {0} loaded succesfuly", shader);
      }
    }
    
    if(!tryCreateShaderProgram(s, shaderProgram, error ))
    {
      LOG(ERROR) << fmt::format("Unable to link program\n{0}", error);
    }
    else
    {
      LOG(INFO) << fmt::format("Program linked succesfully");
    }
    return shaderProgram;
  }
  
  bool OpenGLSystem::tryCreateShaderProgram(std::vector<GLuint> shaders, GLuint& shaderProgram, std::string& error)
  {
    shaderProgram = glCreateProgram();
    for(auto& shader : shaders)
    {
      glAttachShader(shaderProgram, shader);
    }
    glLinkProgram(shaderProgram);
    GLint isLinked = GL_FALSE;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);
    if(isLinked == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
      glDeleteProgram(shaderProgram);
      error = std::string(&infoLog[0]);
      for(auto& shader : shaders)
      {
        glDeleteShader(shader);
      }
      return false;
    }
    for(auto& shader : shaders)
    {
      glDetachShader(shaderProgram, shader);
    }
    return true;
  }

  bool OpenGLSystem::tryLoadShader(std::string fileName, GLenum shaderType, GLuint& shader, std::string& error)
  {
    std::ifstream in(fileName, std::ios::in | std::ios::binary);
    if (in)
    {
      std::string contents;
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();
      const char* shaderText = contents.c_str();
    
      LOG(INFO) << std::endl << shaderText;

      shader = glCreateShader(shaderType);
      glShaderSource(shader, 1, &shaderText, NULL);
      glCompileShader(shader);
     
      GLint isCompiled = 0;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

      if(isCompiled == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
 
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
        glDeleteShader(shader);
        error = std::string(&infoLog[0]);
        return false;
      }
    }
    return true;
  }

  void OpenGLSystem::printShaderProgramInfo(GLuint shaderProgram)
  {
    int value;
    glGetProgramiv(shaderProgram, GL_ATTACHED_SHADERS, &value);
    LOG(INFO) << "Attached shaders: " << value;
    glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &value);
    LOG(INFO) << "Active attributes: " << value;
    for(GLuint i=0; i<(GLuint)value; i++)
    {
      char name[64];
      int maxLength = 64;
      int actualLength = 0;
      int size = 0;
      GLenum type;
      glGetActiveAttrib(shaderProgram, i, maxLength, &actualLength, &size, &type, name);
      if(size > 1)
      {
        for(int j=0; j<size; j++)
        {
          std::string longName = fmt::format("{0}[{1}]", name, j);
          int location = glGetAttribLocation(shaderProgram, longName.c_str());
          LOG(INFO) << fmt::format("{0}) type: {1}, name: {2}, location: {3}", i, glEnumToString(type), longName, location);
        }
      }
      else
      {
        int location = glGetAttribLocation(shaderProgram, name);
        LOG(INFO) << fmt::format("{0}) type: {1}, name: {2}, location: {3}", i, glEnumToString(type), name, location);
      }
    }
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &value);
    LOG(INFO) << "Active uniforms: " << value;

    for(GLuint i=0; i<(GLuint)value; i++)
    {
      char name[64];
      int maxLength = 64;
      int actualLength = 0;
      int size = 0;
      GLenum type;
      glGetActiveUniform(shaderProgram, i, maxLength, &actualLength, &size, &type, name);
      if(size > 1)
      {
        for(int j=0; j<size; j++)
        {
          std::string longName = fmt::format("{0}[{1}]", name, j);
          int location = glGetUniformLocation(shaderProgram, longName.c_str());
          LOG(INFO) << fmt::format("{0}) type: {1}, name: {2}, location: {3}", i, glEnumToString(type), longName, location);
        }
      }
      else
      {
        int location = glGetUniformLocation(shaderProgram, name);
        LOG(INFO) << fmt::format("{0}) type: {1}, name: {2}, location: {3}", i, glEnumToString(type), name, location);
      }
    }
  }
  
  std::string OpenGLSystem::glEnumToString(GLenum glenum)
  {
    switch (glenum) {
    case GL_BOOL: return "bool";
    case GL_INT: return "int";
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
    default: return "other";
    }
  }
}
