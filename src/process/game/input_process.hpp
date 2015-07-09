#pragma once

#include "process/process.hpp"
namespace game {
  class Game;
}
namespace process {
  class InputProcess : public Process {
  private:
    game::Game* _game;

  public:
    InputProcess(game::Game *game);
    virtual ~InputProcess() {}
    virtual void update(double deltaTime) override;
  };
}
