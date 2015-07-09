#include "render_process.hpp"
#include "game/game.hpp"
#include "event/event_manager.hpp"

namespace process {
  RenderProcess::RenderProcess(game::Game* game) : Process() {
    _game = game;
    event::EventManager::get().addListener(event::EventType::WINDOW_CLOSE, [this](event::IEventDataSPtr& event)
                                         {
                                           end();
                                         });
  }

  void RenderProcess::update(double deltaTime) {
    _game->getWindow()->clear();
    _game->getScene()->render();
    _game->getWindow()->swapBuffers();
  }
}
