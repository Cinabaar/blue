#pragma once

#include <memory>
#include "Game/Process/ProcessManager.hpp"
#include "Game/World/Actor/Actor.hpp"
#include <GL/glew.h>
#ifndef M_PI
#define M_PI 3.1415
#endif

namespace game {

    class World {

    private:
        std::unique_ptr<process::ProcessManager> _processManagerUPtr;
        std::shared_ptr<Actor> _cameraSPtr;
        std::shared_ptr<Actor> _floorSPtr;
        std::shared_ptr<Actor> _boxSPtr;
        std::shared_ptr<Actor> _backgroundSPtr;
        std::shared_ptr<Actor> _waterSPtr;
        GLuint waterShaderProgram;

        double _updateStep = 1.0 / 60.0;
    public:
        World();

        void Render();

        void Update(double deltaTime);



    };
}