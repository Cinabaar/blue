#include "game.hpp"
#include "system/opengl_system.hpp"
#include "process/game/render_process.hpp"
#include "process/game/event_dispatch_process.hpp"
#include "process/game/input_process.hpp"
#include "process/game/update_process.hpp"
#include "scene_builder.hpp"

namespace game {
  Game::Game()
  {
    _windowPtr = gamesystem::OpenGLSystem::get().getWindow();
    _processManagerUPtr = unique_ptr<ProcessManager>(new ProcessManager());
    SceneBuilder sceneBuilder;
    _sceneUPtr = unique_ptr<Scene>(sceneBuilder.build());
    _timerUPtr = unique_ptr<Timer>(new Timer());

    _processManagerUPtr->attachProcess(unique_ptr<Process>(new process::RenderProcess(this)));
    _processManagerUPtr->attachProcess(unique_ptr<Process>(new process::EventDispatchProcess(this)));
    _processManagerUPtr->attachProcess(unique_ptr<Process>(new process::UpdateProcess(this)));
    _processManagerUPtr->attachProcess(unique_ptr<Process>(new process::InputProcess(this)));
  }

  void Game::run() {
    _timerUPtr->start();
    while (!_processManagerUPtr->ended()) {
      _processManagerUPtr->update(_timerUPtr->delta());
    }
  }
}
