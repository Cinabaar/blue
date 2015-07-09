#include "update_process.hpp"
#include "event/event_manager.hpp"
#include "game/game.hpp"

namespace process {
  UpdateProcess::UpdateProcess(game::Game* game) : Process() {
    _game = game;
    event::EventManager::get().addListener(event::EventType::WINDOW_CLOSE, [this](event::IEventDataSPtr& event)
                                         {
                                           end();
                                         });
 
  }

  void UpdateProcess::update(double deltaTime) {
    _game->getScene()->update(deltaTime);
  }
}
