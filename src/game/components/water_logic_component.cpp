#include "water_logic_component.hpp"
#include "water_model_component.hpp"
#include "easylogging++.h"
#include "event/event_manager.hpp"
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <CL/cl_platform.h>
#include "system/opencl_system.hpp"
#include "clpp.h"

namespace game
{ 
  struct WaterLogicComponent::WaterLogicComponentImpl
  {
    std::shared_ptr<WaterModelComponent> waterModel;
 
    cl_kernel hash_particles_kernel;
    clppContext cont;
    clppSort* sort;
    cl_kernel sort_post_pass_kernel;
    cl_kernel index_kernel;
    cl_kernel index_post_pass_kernel;
    cl_kernel find_voxel_neighbours_kernel;
    cl_kernel neighbour_map_kernel;
    cl_kernel compute_density_pressure_kernel;
    cl_kernel compute_acceleration_kernel;
    cl_kernel integrate_kernel;

    std::vector<cl_float4> positions;
    cl_mem position_cl;
    std::vector<cl_float4> velocities;
    cl_mem velocity_cl;
    std::vector<cl_float4> accelerations;
    cl_mem acceleration_cl;
    std::vector<cl_float4> sorted_positions;
    cl_mem sorted_position_cl;
    std::vector<cl_float4> sorted_velocities;
    cl_mem sorted_velocity_cl;
    std::vector<cl_int> grid_voxel_indices;
    cl_mem grid_voxel_index_cl;
    std::vector<cl_int> neighbour_map;
    cl_mem neighbour_map_cl;
    std::vector<cl_int2> voxel_positions;
    cl_mem voxel_positions_cl;
    std::vector<cl_int> voxel_neighbours;
    cl_mem voxel_neighbours_cl;
    std::vector<cl_float2> density_pressure;
    cl_mem density_pressure_cl;

    int neighbour_count{64};

    unsigned int particle_count;
  };
  
  WaterLogicComponent::WaterLogicComponent(EntityWPtr entityWPtr) :
    LogicComponent(entityWPtr),
    pimpl(std::unique_ptr<WaterLogicComponentImpl>(new WaterLogicComponentImpl()))
  { }

  void WaterLogicComponent::init()
  {
    pimpl->waterModel = entityWPtr_.lock()->getComponent<WaterModelComponent>(ComponentType::MODEL_COMPONENT);
    auto& model = pimpl->waterModel;
    auto& openCLSystem = gamesystem::OpenCLSystem::get();
    if (!openCLSystem.TryLoadKernel("kernels/sph.cl", "hash_particles", pimpl->hash_particles_kernel)) {
      LOG(ERROR) << "Failed to load hash_particles kernel";
    }
    if (!openCLSystem.TryLoadKernel("kernels/sph.cl", "sort_post_pass", pimpl->sort_post_pass_kernel)) {
      LOG(ERROR) << "Failed to load sort_post_pass kernel";
    }
    if (!openCLSystem.TryLoadKernel("kernels/sph.cl", "indexx", pimpl->index_kernel)) {
      LOG(ERROR) << "Failed to load indexx kernel";
    }
    if (!openCLSystem.TryLoadKernel("kernels/sph.cl", "index_post_pass", pimpl->index_post_pass_kernel)) {
      LOG(ERROR) << "Failed to load index_post_pass kernel";
    }
    if (!openCLSystem.TryLoadKernel("kernels/sph.cl", "neighbour_map", pimpl->neighbour_map_kernel)) {
      LOG(ERROR) << "Failed to load neighbour_map kernel";
    }
    if (!openCLSystem.TryLoadKernel("kernels/sph.cl", "find_voxel_neighbours", pimpl->find_voxel_neighbours_kernel)) {
      LOG(ERROR) << "Failed to load find_voxel_neighbours kernel";
    }

    if (!openCLSystem.TryLoadKernel("kernels/sph.cl", "compute_density_pressure", pimpl->compute_density_pressure_kernel)) {
      LOG(ERROR) << "Failed to load compute_density_pressure kernel";
    }
    if (!openCLSystem.TryLoadKernel("kernels/sph.cl", "compute_acceleration", pimpl->compute_acceleration_kernel)) {
      LOG(ERROR) << "Failed to load compute_acceleration kernel";
    }
    if (!openCLSystem.TryLoadKernel("kernels/sph.cl", "integrate", pimpl->integrate_kernel)) {
      LOG(ERROR) << "Failed to load integrate kernel";
    }

    auto context = openCLSystem.getContext();
    cl_int errNum = 0;

    pimpl->particle_count = model->particle_count();
    
    cl_float4 lbf{0,0,0,0};
    cl_float4 rtb{2,2,2,0};

    cl_float h = 0.1;
    cl_float ro0 = 90;
    cl_float m = 0.02;


    cl_float4 g{0,-20,0,0};
    cl_float mi{8};
    cl_float k{4};
    cl_float speed_loss{1};

    unsigned int voxelsX = (unsigned int) (fabs((float)((rtb.s[0] - lbf.s[0])/(2*h))) + 0.5);
    unsigned int voxelsY = (unsigned int) (fabs((float)((rtb.s[1] - lbf.s[1])/(2*h))) + 0.5);
    unsigned int voxelsZ = (unsigned int) (fabs((float)((rtb.s[2] - lbf.s[2])/(2*h))) + 0.5);
    unsigned voxelCount = voxelsX * voxelsY * voxelsZ;

    clppProgram::setBasePath("kernels");
    auto& cont = pimpl->cont;
    cont.clContext = openCLSystem.getContext();
    cont.clDevice = openCLSystem.getDevice();
    cont.clPlatform = openCLSystem.getPlatform();
    cont.clQueue = openCLSystem.getCommandQueue();

    pimpl->positions.resize(pimpl->particle_count);
    pimpl->position_cl = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, model->position_vbo, &errNum);
    pimpl->velocities.resize(pimpl->particle_count);
    pimpl->velocity_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4) * pimpl->velocities.size(), &pimpl->velocities[0], &errNum);
    pimpl->accelerations.resize(pimpl->particle_count);
    pimpl->acceleration_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4) * pimpl->accelerations.size(), &pimpl->accelerations[0], &errNum);
    pimpl->sorted_positions.resize(pimpl->particle_count);
    pimpl->sorted_position_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4) * pimpl->sorted_positions.size(), &pimpl->sorted_positions[0], &errNum);
    pimpl->sorted_velocities.resize(pimpl->particle_count);
    pimpl->sorted_velocity_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4) * pimpl->sorted_velocities.size(), &pimpl->sorted_velocities[0], &errNum);
    pimpl->grid_voxel_indices.resize(voxelCount + 1);
    pimpl->grid_voxel_indices.back() = pimpl->particle_count;
    pimpl->grid_voxel_index_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * pimpl->grid_voxel_indices.size(), &pimpl->grid_voxel_indices[0], &errNum);
    pimpl->neighbour_map.resize(pimpl->neighbour_count*pimpl->particle_count);
    pimpl->neighbour_map_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * pimpl->neighbour_map.size(), &pimpl->neighbour_map[0], &errNum);
    pimpl->voxel_positions.resize(pimpl->particle_count);
    pimpl->voxel_positions_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int2) * pimpl->voxel_positions.size(), &pimpl->voxel_positions[0], &errNum);
    pimpl->voxel_neighbours.resize(64*voxelCount);
    pimpl->voxel_neighbours_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * pimpl->voxel_neighbours.size(), &pimpl->voxel_neighbours[0], &errNum);
    pimpl->density_pressure.resize(pimpl->particle_count);
    pimpl->density_pressure_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float2) * pimpl->density_pressure.size(), &pimpl->density_pressure[0], &errNum);

    pimpl->sort = clpp::createBestSortKV(&cont, pimpl->voxel_positions.size(), false);
    pimpl->sort->pushCLDatas(pimpl->voxel_positions_cl, pimpl->voxel_positions.size());

    
    
    errNum = clSetKernelArg(pimpl->hash_particles_kernel, 0, sizeof(cl_mem), &pimpl->position_cl);
    errNum = clSetKernelArg(pimpl->hash_particles_kernel, 1, sizeof(cl_mem), &pimpl->voxel_positions_cl);
    errNum = clSetKernelArg(pimpl->hash_particles_kernel, 2, sizeof(cl_float4), &lbf);
    errNum = clSetKernelArg(pimpl->hash_particles_kernel, 3, sizeof(cl_float4), &rtb);
    errNum = clSetKernelArg(pimpl->hash_particles_kernel, 4, sizeof(cl_float), &h);

    errNum = clSetKernelArg(pimpl->sort_post_pass_kernel, 0, sizeof(cl_mem), &pimpl->position_cl);
    errNum = clSetKernelArg(pimpl->sort_post_pass_kernel, 1, sizeof(cl_mem), &pimpl->velocity_cl);
    errNum = clSetKernelArg(pimpl->sort_post_pass_kernel, 2, sizeof(cl_mem), &pimpl->sorted_position_cl);
    errNum = clSetKernelArg(pimpl->sort_post_pass_kernel, 3, sizeof(cl_mem), &pimpl->sorted_velocity_cl);
    errNum = clSetKernelArg(pimpl->sort_post_pass_kernel, 4, sizeof(cl_mem), &pimpl->voxel_positions_cl);

    errNum = clSetKernelArg(pimpl->index_kernel, 0, sizeof(cl_mem), &pimpl->grid_voxel_index_cl);
    errNum = clSetKernelArg(pimpl->index_kernel, 1, sizeof(cl_mem), &pimpl->voxel_positions_cl);
    errNum = clSetKernelArg(pimpl->index_kernel, 2, sizeof(cl_uint), &pimpl->particle_count);

    errNum = clSetKernelArg(pimpl->index_post_pass_kernel, 0, sizeof(cl_mem), &pimpl->grid_voxel_index_cl);

    errNum = clSetKernelArg(pimpl->neighbour_map_kernel, 0, sizeof(cl_mem), &pimpl->neighbour_map_cl);
    errNum = clSetKernelArg(pimpl->neighbour_map_kernel, 1, sizeof(cl_mem), &pimpl->voxel_positions_cl);
    errNum = clSetKernelArg(pimpl->neighbour_map_kernel, 2, sizeof(cl_mem), &pimpl->grid_voxel_index_cl);
    errNum = clSetKernelArg(pimpl->neighbour_map_kernel, 3, sizeof(cl_mem), &pimpl->sorted_position_cl);
    errNum = clSetKernelArg(pimpl->neighbour_map_kernel, 4, sizeof(cl_mem), &pimpl->voxel_neighbours_cl);
    errNum = clSetKernelArg(pimpl->neighbour_map_kernel, 5, sizeof(cl_int), &pimpl->neighbour_count);
    errNum = clSetKernelArg(pimpl->neighbour_map_kernel, 6, sizeof(cl_float4), &lbf);
    errNum = clSetKernelArg(pimpl->neighbour_map_kernel, 7, sizeof(cl_float4), &rtb);
    errNum = clSetKernelArg(pimpl->neighbour_map_kernel, 8, sizeof(cl_float), &h);

    errNum = clSetKernelArg(pimpl->find_voxel_neighbours_kernel, 0, sizeof(cl_mem), &pimpl->voxel_neighbours_cl);
    errNum = clSetKernelArg(pimpl->find_voxel_neighbours_kernel, 1, sizeof(cl_float4), &lbf);
    errNum = clSetKernelArg(pimpl->find_voxel_neighbours_kernel, 2, sizeof(cl_float4), &rtb);
    errNum = clSetKernelArg(pimpl->find_voxel_neighbours_kernel, 3, sizeof(cl_float), &h);

    auto commandQueue = openCLSystem.getCommandQueue();
    size_t voxelWorkSize = pimpl->grid_voxel_indices.size() - 1;
    errNum = clEnqueueNDRangeKernel(commandQueue, pimpl->find_voxel_neighbours_kernel, 1, NULL, &voxelWorkSize, NULL, 0,0,0 );
    clFinish(commandQueue);

    errNum = clSetKernelArg(pimpl->compute_density_pressure_kernel, 0, sizeof(cl_mem), &pimpl->sorted_position_cl);
    errNum = clSetKernelArg(pimpl->compute_density_pressure_kernel, 1, sizeof(cl_mem), &pimpl->neighbour_map_cl);
    errNum = clSetKernelArg(pimpl->compute_density_pressure_kernel, 2, sizeof(cl_mem), &pimpl->density_pressure_cl);
    errNum = clSetKernelArg(pimpl->compute_density_pressure_kernel, 3, sizeof(cl_int), &pimpl->neighbour_count);
    errNum = clSetKernelArg(pimpl->compute_density_pressure_kernel, 4, sizeof(cl_float), &m);
    errNum = clSetKernelArg(pimpl->compute_density_pressure_kernel, 5, sizeof(cl_float), &h);
    errNum = clSetKernelArg(pimpl->compute_density_pressure_kernel, 6, sizeof(cl_float), &k);
    errNum = clSetKernelArg(pimpl->compute_density_pressure_kernel, 7, sizeof(cl_float), &ro0);

    errNum = clSetKernelArg(pimpl->compute_acceleration_kernel, 0, sizeof(cl_mem), &pimpl->sorted_position_cl);
    errNum = clSetKernelArg(pimpl->compute_acceleration_kernel, 1, sizeof(cl_mem), &pimpl->sorted_velocity_cl);
    errNum = clSetKernelArg(pimpl->compute_acceleration_kernel, 2, sizeof(cl_mem), &pimpl->acceleration_cl);
    errNum = clSetKernelArg(pimpl->compute_acceleration_kernel, 3, sizeof(cl_mem), &pimpl->density_pressure_cl);
    errNum = clSetKernelArg(pimpl->compute_acceleration_kernel, 4, sizeof(cl_mem), &pimpl->neighbour_map_cl);
    errNum = clSetKernelArg(pimpl->compute_acceleration_kernel, 5, sizeof(cl_float4), &g);
    errNum = clSetKernelArg(pimpl->compute_acceleration_kernel, 6, sizeof(cl_float), &m);
    errNum = clSetKernelArg(pimpl->compute_acceleration_kernel, 7, sizeof(cl_float), &h);
    errNum = clSetKernelArg(pimpl->compute_acceleration_kernel, 8, sizeof(cl_float), &mi);
    errNum = clSetKernelArg(pimpl->compute_acceleration_kernel, 9, sizeof(cl_int), &pimpl->neighbour_count);

    errNum = clSetKernelArg(pimpl->integrate_kernel, 0, sizeof(cl_mem), &pimpl->position_cl);
    errNum = clSetKernelArg(pimpl->integrate_kernel, 1, sizeof(cl_mem), &pimpl->velocity_cl);
    errNum = clSetKernelArg(pimpl->integrate_kernel, 2, sizeof(cl_mem), &pimpl->acceleration_cl);
    errNum = clSetKernelArg(pimpl->integrate_kernel, 3, sizeof(cl_mem), &pimpl->voxel_positions_cl);
    errNum = clSetKernelArg(pimpl->integrate_kernel, 4, sizeof(cl_float4), &lbf);
    errNum = clSetKernelArg(pimpl->integrate_kernel, 5, sizeof(cl_float4), &rtb);
    errNum = clSetKernelArg(pimpl->integrate_kernel, 6, sizeof(cl_float), &speed_loss);
  }
  
  
  void WaterLogicComponent::update(double deltaTime)
  {
    glFinish();

    deltaTime = 0.005;
    cl_int errNum = CL_SUCCESS;
    auto& clSystem = gamesystem::OpenCLSystem::get();
    cl_command_queue commandQueue = clSystem.getCommandQueue();
    errNum = clEnqueueAcquireGLObjects(commandQueue, 1, &pimpl->position_cl, 0,0,0);

    //hash particles
    size_t particlesWorkSize = pimpl->particle_count;
    size_t voxelWorkSize = pimpl->grid_voxel_indices.size() - 1;

    errNum = clEnqueueNDRangeKernel(commandQueue, pimpl->hash_particles_kernel, 1, NULL, &particlesWorkSize, NULL, 0,0,0 );
    clFinish(commandQueue);
    pimpl->sort->sort();
    pimpl->sort->waitCompletion();
    errNum = clEnqueueNDRangeKernel(commandQueue, pimpl->sort_post_pass_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);
    errNum = clEnqueueNDRangeKernel(commandQueue, pimpl->index_kernel, 1, NULL, &voxelWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);

    errNum = clEnqueueNDRangeKernel(commandQueue, pimpl->index_post_pass_kernel, 1, NULL, &voxelWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);

    errNum = clEnqueueNDRangeKernel(commandQueue, pimpl->neighbour_map_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);

    errNum = clEnqueueReadBuffer(commandQueue, pimpl->neighbour_map_cl, CL_TRUE, 0, pimpl->neighbour_map.size() * sizeof(cl_int), &pimpl->neighbour_map[0], 0, 0, NULL);
   
    errNum = clEnqueueNDRangeKernel(commandQueue, pimpl->compute_density_pressure_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);
    
    errNum = clEnqueueNDRangeKernel(commandQueue, pimpl->compute_acceleration_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);

    float castedDelta = deltaTime;
    errNum = clSetKernelArg(pimpl->integrate_kernel, 7, sizeof(cl_float), &castedDelta);
    errNum = clEnqueueNDRangeKernel(commandQueue, pimpl->integrate_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);

    errNum = clEnqueueReleaseGLObjects(commandQueue, 1, &pimpl->position_cl, 0,0,0);
    clFinish(commandQueue);
  };

  WaterLogicComponent::~WaterLogicComponent() {

    clReleaseMemObject(pimpl->position_cl);
    clReleaseMemObject(pimpl->velocity_cl);
    clReleaseMemObject(pimpl->acceleration_cl);
    clReleaseMemObject(pimpl->sorted_position_cl);
    clReleaseMemObject(pimpl->sorted_velocity_cl);
    clReleaseMemObject(pimpl->grid_voxel_index_cl);
    clReleaseMemObject(pimpl->neighbour_map_cl);
    clReleaseMemObject(pimpl->voxel_positions_cl);
    clReleaseMemObject(pimpl->voxel_neighbours_cl);
    clReleaseMemObject(pimpl->density_pressure_cl);
    delete pimpl->sort;
  }
}
