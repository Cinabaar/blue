#pragma once

#include "process/process.hpp"

namespace game {
  class Game;
}

namespace process {

  class RenderProcess : public Process {
  private:
    game::Game *_game;
  public:
    RenderProcess(game::Game *game);
    virtual ~RenderProcess() {}
    virtual void update(double deltaTime) override;
  };
}
