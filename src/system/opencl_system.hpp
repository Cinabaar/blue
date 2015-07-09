#pragma once


#include "easylogging++.h"

#include <GL/glew.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <map>
#include <vector>

#include "opengl_system.hpp"
#include "util/util.hpp"

namespace gamesystem
{
  using std::vector;
  using std::map;
  using std::string;
  
  class OpenCLSystem : public NonCopyable<OpenCLSystem>
  {
  private:
    OpenCLSystem();
  private:
    cl_context _context;
    cl_device_id _deviceId;
    cl_platform_id _platformId;
    cl_command_queue _commandQueue;
    map<string, string> programCache;
    map<string, cl_kernel> kernelCache;
    bool TryCreateContext();
    bool TryGetPlatforms(vector<cl_platform_id> &platforms);
  public:
    static OpenCLSystem& get() {
      static OpenCLSystem instance;
      return instance;
    }
    cl_context getContext() const {
      return _context;
    }
    cl_device_id getDevice() const {
      return _deviceId;
    }
    cl_command_queue getCommandQueue() const {
      return _commandQueue;
    }
    cl_platform_id getPlatform() const {
      return _platformId;
    }
    bool TryLoadKernel(string filePath, string kernelName, cl_kernel& kernel);
    bool TryGetDevices(cl_platform_id platform, vector <cl_device_id> &devices);
  };

}
