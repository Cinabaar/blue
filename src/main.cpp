#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "system/opengl_system.hpp"
#include "system/opencl_system.hpp"
#include "game/game.hpp"

#define GLM_FORCE_RADIANS

int main(int argc, char** argv)
{

    auto& glSystem = gamesystem::OpenGLSystem::get();
    auto& clSystem = gamesystem::OpenCLSystem::get();

    game::Game game;
    game.run();
}
