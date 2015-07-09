#pragma once
#include <memory>
#include "util/util.hpp"
#include "util/timer.hpp"
#include "system/window/window.hpp"
#include "process/process_manager.hpp"
#include "scene.hpp"

namespace game {
  using gamesystem::Window;
  using std::unique_ptr;
  using process::ProcessManager;
  using process::Process;
  
  class Game : public NonCopyable<Game>
  {
  private:
    Window* _windowPtr;
    unique_ptr<process::ProcessManager> _processManagerUPtr;
    unique_ptr<Scene> _sceneUPtr;
    unique_ptr<Timer> _timerUPtr;

  public:
    Game();
    void run();

    Scene* getScene() const {
      return _sceneUPtr.get();
    }

    Window* getWindow() const {
      return _windowPtr;
    }

    Timer *getTimer() const {
      return _timerUPtr.get();
    }
  };
}
