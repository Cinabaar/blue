#pragma once

#include "process/process.hpp"

namespace game
{
  class Game;
}
namespace process {

  class UpdateProcess : public Process {
  private:
    game::Game *_game;

  public:
    UpdateProcess(game::Game* game);
    virtual ~UpdateProcess() {}
    virtual void update(double deltaTime) override;
  };
}
