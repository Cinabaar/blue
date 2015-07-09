#include "event_dispatch_process.hpp"
#include "game/game.hpp"
#include "event/event_manager.hpp"

namespace process {
  EventDispatchProcess::EventDispatchProcess(game::Game *game) : Process() {
    _game = game;
    event::EventManager::get().addListener(event::EventType::WINDOW_CLOSE, [this](event::IEventDataSPtr& event)
                                         {
                                           end();
                                         });
  }
  void EventDispatchProcess::update(double deltaTime) {
    auto& eventManager = event::EventManager::get();
    eventManager.update(deltaTime);
  }
}
