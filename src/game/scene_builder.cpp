#include "scene_builder.hpp"
#include "easylogging++.h"
#include "components/components.hpp"
#include "format.h"
#include "system/opengl_system.hpp"
#include "game/entity.hpp" 
#include "SOIL.h"

namespace game
{
 
  using std::shared_ptr;
  using std::vector;
  using std::string;
  using std::make_shared;
  using namespace glm;
  using namespace gamesystem;
  
  vector<float> cube = {
  -10.0f,  10.0f, -10.0f,
  -10.0f, -10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
   10.0f,  10.0f, -10.0f,
  -10.0f,  10.0f, -10.0f,
 
  -10.0f, -10.0f,  10.0f,
  -10.0f, -10.0f, -10.0f,
  -10.0f,  10.0f, -10.0f,
  -10.0f,  10.0f, -10.0f,
  -10.0f,  10.0f,  10.0f,
  -10.0f, -10.0f,  10.0f,
  
   10.0f, -10.0f, -10.0f,
   10.0f, -10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
   
  -10.0f, -10.0f,  10.0f,
  -10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f, -10.0f,  10.0f,
  -10.0f, -10.0f,  10.0f,
  
  -10.0f,  10.0f, -10.0f,
   10.0f,  10.0f, -10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
  -10.0f,  10.0f,  10.0f,
  -10.0f,  10.0f, -10.0f,
  
  -10.0f, -10.0f, -10.0f,
  -10.0f, -10.0f,  10.0f,
   10.0f, -10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
  -10.0f, -10.0f,  10.0f,
   10.0f, -10.0f,  10.0f
};
 
  SceneUPtr SceneBuilder::build()
  {
    SceneUPtr scene(new Scene());
    auto& openglSystem = OpenGLSystem::get();
    
    //////CAMERA//////
    auto cam = std::make_shared<Entity>("camera");
    auto camTransform = make_shared<TransformComponent>(cam);
    auto camera = std::make_shared<CameraComponent>(cam);
    auto camLogic = make_shared<CameraLogicComponent>(cam);
    cam->addComponent(camTransform);
    cam->addComponent(camera);
    cam->addComponent(camLogic);
    cam->init();
    
    camTransform->lookAt(vec3(0, 5, 1), vec3(0, 5, 0), vec3(0, 1, 0));
    camera->updatePerspective(4.0/3.0);
    camera->updateView();
    scene->addEntity(cam);
    scene->setActiveCamera(cam);
    
    //////SKYBOX/////
    auto skybox = make_shared<Entity>("skybox");
    auto skyboxModel = make_shared<ModelComponent>(skybox);
    auto skyboxRenderer = make_shared<SkyboxRenderComponent>(skybox);
    skybox->addComponent(skyboxModel);
    skybox->addComponent(skyboxRenderer);
    skybox->init();

    skyboxModel->setVertices(cube);
    std::vector<string> skybox_shaders = {"shaders/skybox.vert", "shaders/skybox.frag"};
    auto shaderProgram = openglSystem.createShaderProgram(skybox_shaders);
    skyboxRenderer->setShaderProgram(shaderProgram);
    camera->loadIntoProgram(shaderProgram);
    GLuint texture = SOIL_load_OGL_single_cubemap("assets/skybox_texture.png", "ESWNUD", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    skyboxModel->setTexture(texture);
    scene->addEntity(skybox);
    
    /////GROUND/////
    auto ground = make_shared<Entity>("ground");
    auto groundModel = make_shared<ModelComponent>(ground);
    auto groundRenderer = make_shared<RenderComponent>(ground);
    auto groundTransform = make_shared<TransformComponent>(ground);
    ground->addComponent(groundModel);
    ground->addComponent(groundRenderer);
    ground->addComponent(groundTransform);
    ground->init();
    
    vector<float> points = {{
        -99999, 0, -99999,
        99999, 0, 99999,
        -99999, 0, 99999,
        99999, 0, -99999,
        99999, 0, 99999,
        -99999, 0, -99999
      }};
    groundModel->setVertices(points);
    groundTransform->setPosition(vec3(0,0,0));
    std::vector<string> ground_shaders = {"shaders/shader.vert", "shaders/shader.frag"};
    shaderProgram = openglSystem.createShaderProgram(ground_shaders);
    groundRenderer->setShaderProgram(shaderProgram);
    camera->loadIntoProgram(shaderProgram);
    scene->addEntity(ground);

    /////WATER////
    auto water = make_shared<Entity>("water");
    auto waterModel = make_shared<WaterModelComponent>(water, 32, 0.5, 2, 1);
    auto waterRenderer = make_shared<WaterRenderComponent>(water);
    auto waterLogic = make_shared<WaterLogicComponent>(water);
    auto waterTransform = make_shared<TransformComponent>(water);
    water->addComponent(waterModel);
    water->addComponent(waterLogic);
    water->addComponent(waterRenderer);
    water->addComponent(waterTransform);
    water->init();
    
    waterTransform->setPosition(vec3(0, 4, -5));
    std::vector<string> water_shaders = {"shaders/watershader.vert", "shaders/watershader.geom", "shaders/watershader.frag"};
    shaderProgram = openglSystem.createShaderProgram(water_shaders);
    waterRenderer->setShaderProgram(shaderProgram);
    camera->loadIntoProgram(shaderProgram);

    scene->addEntity(water);

    
    return scene;
  }
}
