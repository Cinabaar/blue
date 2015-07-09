#pragma once

#include "process/process.hpp"

namespace game {
  class Game;
}
namespace process {

  class EventDispatchProcess : public Process {
  private:
    game::Game *_game;
  public:
    EventDispatchProcess(game::Game* game);
    virtual ~EventDispatchProcess() {}
    virtual void update(double deltaTime) override;
  };
}
