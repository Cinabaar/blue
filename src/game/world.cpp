#include "Actor/Actor.hpp"
#include "World.hpp"
#include "Game/World/Actor/Components/Basic/BasicRendererComponent.hpp"
#include <fstream>
#include <sstream>
#include <Game/World/Actor/Components/TransformComponent.hpp>
#include "log.hpp"
#include <Game/World/Actor/Components/Camera/CameraLogicComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Game/World/Actor/Components/Background/BackgroundRendererComponent.hpp>
#include <Game/World/Actor/Components/Basic/BasicModelComponent.hpp>
#include <Game/World/Actor/Components/Camera/CameraModelComponent.hpp>
#include <Game/World/Actor/Components/Water/WaterModelComponent.hpp>
#include <Game/World/Actor/Components/Water/WaterRendererComponent.hpp>
#include <Game/World/Actor/Components/Water/WaterLogicComponent.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Event/EventManager.hpp>

namespace game {

        World::World() {

        auto& eventManager = event::EventManager::getInstance();
        eventManager.AddListener(event::EventType::MOUSE_RMB_PRESSED, [this](event::IEventDataSPtr& eventData)
        {
            _waterSPtr = std::make_shared<Actor>();
            auto waterTransformSPtr = std::make_shared<TransformComponent>(_waterSPtr);
            waterTransformSPtr->MoveBy(glm::vec3(0, 0.5f, 0));
            auto waterModelSPtr = std::make_shared<WaterModelComponent>(32, 0.5, 2, 1);
            auto waterRendererSPtr = std::make_shared<WaterRendererComponent>(_waterSPtr, waterShaderProgram);
            _waterSPtr->AddComponent(waterTransformSPtr);
            _waterSPtr->AddComponent(waterModelSPtr);
            _waterSPtr->AddComponent(waterRendererSPtr);
            _waterSPtr->ClearComponent(ComponentType::LOGIC_COMPONENT);
        });
        eventManager.AddListener(event::EventType::MOUSE_RMB_RELEASED, [this](event::IEventDataSPtr& eventData)
        {
            auto waterLogicSPtr = std::make_shared<WaterLogicComponent>(_waterSPtr);
            _waterSPtr->AddComponent(waterLogicSPtr);
        });



        auto vert = get_file_contents("Shader/shader_vert.glsl");
        auto frag = get_file_contents("Shader/shader_frag.glsl");
        const char* basicVertexShader = vert.c_str();
        const char* basicFragmentShader = frag.c_str();

        GLuint vs = glCreateShader (GL_VERTEX_SHADER);
        glShaderSource (vs, 1, &basicVertexShader, NULL);
        glCompileShader (vs);
        GetShaderCompileLog(vs);
        GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
        glShaderSource (fs, 1, &basicFragmentShader, NULL);
        glCompileShader (fs);
        GetShaderCompileLog(fs);
        GLuint basicShaderProgram = glCreateProgram ();
        glAttachShader (basicShaderProgram, fs);
        glAttachShader (basicShaderProgram, vs);
        glLinkProgram (basicShaderProgram);
        GetProgramLinkLog(basicShaderProgram);

        vert = get_file_contents("Shader/watershader_vert.glsl");
        frag = get_file_contents("Shader/watershader_frag.glsl");
        auto geom = get_file_contents("Shader/watershader_geom.glsl");
        const char* waterVertexShader = vert.c_str();
        const char* waterFragmentShader = frag.c_str();
        const char* waterGeometryShader = geom.c_str();

        GLuint wvs = glCreateShader (GL_VERTEX_SHADER);
        glShaderSource (wvs, 1, &waterVertexShader, NULL);
        glCompileShader (wvs);
        GetShaderCompileLog(wvs);

        GLuint wgs = glCreateShader (GL_GEOMETRY_SHADER);
        glShaderSource (wgs, 1, &waterGeometryShader, NULL);
        glCompileShader (wgs);
        GetShaderCompileLog(wgs);

        GLuint wfs = glCreateShader (GL_FRAGMENT_SHADER);
        glShaderSource (wfs, 1, &waterFragmentShader, NULL);
        glCompileShader (wfs);
        GetShaderCompileLog(wfs);

        waterShaderProgram = glCreateProgram ();
        glAttachShader (waterShaderProgram, wfs);
        glAttachShader (waterShaderProgram, wgs);
        glAttachShader (waterShaderProgram, wvs);
        glLinkProgram (waterShaderProgram);
        GetProgramLinkLog(waterShaderProgram);

        _cameraSPtr = std::make_shared<Actor>();
        auto cameraTransformSPtr = std::make_shared<TransformComponent>(_cameraSPtr);
        cameraTransformSPtr->MoveBy(glm::vec3(0, 0.5f, 8));
        cameraTransformSPtr->SetRotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0)));
        cameraTransformSPtr->RotateBy(M_PI, glm::vec3(0, 1, 0));
        _cameraSPtr->AddComponent(cameraTransformSPtr);
        auto cameraSPtr = std::make_shared<CameraModelComponent>(_cameraSPtr);
        cameraSPtr->Load(basicShaderProgram);
        cameraSPtr->Load(waterShaderProgram);
        _cameraSPtr->AddComponent(cameraSPtr);
        auto cameraLogicSPtr = std::make_shared<CameraLogicComponent>(_cameraSPtr);
        _cameraSPtr->AddComponent(cameraLogicSPtr);


        _backgroundSPtr = std::make_shared<Actor>();
        auto backgroundRendererSPtr = std::make_shared<BackgroundRendererComponent>(_backgroundSPtr, basicShaderProgram);
        _backgroundSPtr->AddComponent(backgroundRendererSPtr);


        _floorSPtr = std::make_shared<Actor>();
        auto floorTransformSPtr = std::make_shared<TransformComponent>(_floorSPtr);
        auto floorModelSPtr = std::make_shared<BasicModelComponent>(_floorSPtr);
        floorModelSPtr->Load("assets/floor.obj");
        auto floorRendererSPtr = std::make_shared<BasicRendererComponent>(_floorSPtr, basicShaderProgram);
        _floorSPtr->AddComponent(floorTransformSPtr);
        _floorSPtr->AddComponent(floorModelSPtr);
        _floorSPtr->AddComponent(floorRendererSPtr);

        /*_boxSPtr = std::make_shared<Actor>();
        auto boxTransformSPtr = std::make_shared<TransformComponent>(_boxSPtr);
        boxTransformSPtr->MoveBy(glm::vec3(0, 0.5f, 0));
        auto boxModelSPtr = std::make_shared<BasicModelComponent>(_boxSPtr);
        boxModelSPtr->Load("assets/box.obj");
        auto boxRendererSPtr = std::make_shared<BasicRendererComponent>(_boxSPtr, basicShaderProgram);
        _boxSPtr->AddComponent(boxTransformSPtr);
        _boxSPtr->AddComponent(boxModelSPtr);
        _boxSPtr->AddComponent(boxRendererSPtr);*/

        _waterSPtr = std::make_shared<Actor>();
    }

    void World::Render() {
        auto backgroundRenderComponent = std::dynamic_pointer_cast<BackgroundRendererComponent>(_backgroundSPtr->getComponent(ComponentType::RENDER_COMPONENT));
        backgroundRenderComponent->Render();
        /*  renderComponent = std::dynamic_pointer_cast<BasicRendererComponent>(_boxSPtr->getComponent(ComponentType::RENDER_COMPONENT));
        renderComponent->Render();*/
        auto renderComponent = std::dynamic_pointer_cast<BasicRendererComponent>(_floorSPtr->getComponent(ComponentType::RENDER_COMPONENT));
        renderComponent->Render();
        if(_waterSPtr->hasComponent(ComponentType::RENDER_COMPONENT)) {
            auto waterRenderComponent = std::dynamic_pointer_cast<WaterRendererComponent>(_waterSPtr->getComponent(ComponentType::RENDER_COMPONENT));
            waterRenderComponent->Render();
        }
    }

    void World::Update(double deltaTime) {
        auto cameraLogicComponent = std::dynamic_pointer_cast<CameraLogicComponent>(_cameraSPtr->getComponent(ComponentType::LOGIC_COMPONENT));
        cameraLogicComponent->Update(deltaTime);
        static double skipTimer = 0;
        skipTimer += deltaTime;
        if(skipTimer >= _updateStep) {
            if(_waterSPtr->hasComponent(ComponentType::LOGIC_COMPONENT)) {
                auto waterLogicComponent = std::dynamic_pointer_cast<WaterLogicComponent>(_waterSPtr->getComponent(ComponentType::LOGIC_COMPONENT));
                waterLogicComponent->Update(_updateStep);
                skipTimer = 0;
            }
        }
    }
}
