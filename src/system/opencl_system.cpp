#include "opencl_system.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <sstream>


namespace gamesystem {
  OpenCLSystem::OpenCLSystem() {
    TryCreateContext();
  }
  bool OpenCLSystem::TryGetPlatforms(std::vector<cl_platform_id> &platforms) {
    cl_uint numPlatforms;
    cl_int errNum;

    errNum = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (errNum != CL_SUCCESS) {
      LOG(ERROR) << "Error " << errNum << " in clGetPlatformIDs Call";
      return false;
    }
    else {
      if (numPlatforms == 0) {
        LOG(ERROR) << "No OpenCL platform found";
        return false;
      }
      else {
        platforms.resize(numPlatforms);
        errNum = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);
        if (errNum != CL_SUCCESS) {
          LOG(ERROR) << "Error " << errNum << " in clGetPlatformIDs Call ";
          return false;
        }
      }
    }
    return true;
  }

  bool OpenCLSystem::TryGetDevices(cl_platform_id platform, std::vector<cl_device_id> &devices)
  {
    cl_int errNum;
    cl_uint device_count;
    errNum = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &device_count);
    if(errNum != CL_SUCCESS || device_count==0)
      return false;
    devices.resize(device_count);
    errNum = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, device_count, &devices[0], NULL);
    return errNum == CL_SUCCESS;
  }

  bool OpenCLSystem::TryCreateContext() {
    std::vector<cl_platform_id> platforms;
    auto& glSystem = OpenGLSystem::get();
    cl_int errNum;
    if(!TryGetPlatforms(platforms))
    {
      LOG(ERROR) << "Unable to get opencl platforms";
      return false;
    }
    else
    {
      for(auto& platform : platforms)
      {
        std::vector<cl_device_id> devices;
        if(TryGetDevices(platform, devices))
        {
          for(cl_device_id& device : devices)
          {
            cl_context_properties props[] =
              {
                CL_GL_CONTEXT_KHR,
                (cl_context_properties) glSystem.getCurrentContext(),
#ifdef _WIN32   // Windows version
                CL_WGL_HDC_KHR,
#else
                CL_GLX_DISPLAY_KHR,
#endif
                (cl_context_properties) glSystem.getCurrentDisplay(),
                CL_CONTEXT_PLATFORM,
                (cl_context_properties)platform, 0
              };
            _context = clCreateContext(props, 1, &device, NULL, NULL, &errNum);
            if (errNum != CL_SUCCESS) {
              LOG(ERROR) << "Platform: "<<platform<<", Error:" << errNum << " clCreateContext Call";
              continue;
            } else {
              _deviceId = device;
              _platformId = platform;
              LOG(INFO) << "Platform: "<<platform<<", clCreateContext succeeded";
              break;
            }
          }
        }
      }
    }
    if(errNum != CL_SUCCESS)
      return false;
    _commandQueue = clCreateCommandQueue(_context, _deviceId, 0, &errNum);
    if (errNum != CL_SUCCESS) {
      LOG(ERROR) << "Error:" << errNum << " clCreateCommandQueue Call";
      return false;
    } else {
      LOG(INFO) << "clCreateCommandQueue succeeded";
      return true;
    }
  }


  bool OpenCLSystem::TryLoadKernel(std::string filePath, std::string kernelName, cl_kernel& kernel)
  {
    if(kernelCache.find(kernelName) != kernelCache.end()) {
      kernel = kernelCache[kernelName];
      return true;
    }

    cl_int errNum;
    std::string kernelSource;
    size_t program_length;
    if(programCache.find(filePath)== programCache.end()) {
      std::ifstream in(filePath, std::ios::in | std::ios::binary);
      std::ostringstream buffer;
      buffer << in.rdbuf();
      kernelSource = buffer.str();
      programCache[filePath]=kernelSource;
    } else {
      kernelSource = programCache[filePath];
    }
    program_length = kernelSource.size();

    LOG(INFO) << kernelName;

    const char* kernelSourceC = &kernelSource[0];
    cl_program program = clCreateProgramWithSource(_context, 1, (const char **) &kernelSourceC, &program_length,
                                                   &errNum);
    
    if (errNum != CL_SUCCESS)
    {
      LOG(ERROR) << "Kernel program load from source error " << errNum;
    }
    //errNum = clBuildProgram(program, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);
    errNum = clBuildProgram(program, 0, NULL, "", NULL, NULL);
        
    if (errNum != CL_SUCCESS)
    {
      LOG(ERROR) << "Kernel program build error " << errNum;
      size_t len;
      clGetProgramBuildInfo(program, _deviceId, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
      vector<char> info;
      info.resize(len);
      clGetProgramBuildInfo(program, _deviceId, CL_PROGRAM_BUILD_LOG, len, &info[0], 0);
      LOG(ERROR) << std::string(info.begin(), info.end());
    }

    // create the kernel
    kernel = clCreateKernel(program, kernelName.c_str(), &errNum);
    if(errNum != CL_SUCCESS)
    {
      LOG(ERROR) << "Failed to create kernel" << errNum;
      return false;
    }
    kernelCache[kernelName] = kernel;
    return true;
  }
 }


