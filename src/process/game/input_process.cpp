#include "input_process.hpp"
#include "game/game.hpp"
#include "event/event_manager.hpp"

namespace process {
  InputProcess::InputProcess(game::Game* game) : Process() {
    _game = game;
    event::EventManager::get().addListener(event::EventType::WINDOW_CLOSE, [this](event::IEventDataSPtr& event)
                                         {
                                           end();
                                         });
 
  }

  void InputProcess::update(double deltaTime) {
    _game->getWindow()->pollEvents();
  }
}
