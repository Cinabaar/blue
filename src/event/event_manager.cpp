#include <iostream>
#include "easylogging++.h"
#include "event_manager.hpp"

namespace event {

  Connection EventManager::addListener(EventType eventType, std::function<void(IEventDataSPtr &)> callback)
  {
    return _eventBinding[eventType].connect(callback);
  }

  void EventManager::triggerEvent(IEventDataSPtr eventData)
  {

    if (_eventBinding.find(eventData->getEventType()) != _eventBinding.end())
      _eventBinding[eventData->getEventType()](eventData);
  }

  void EventManager::queueEvent(IEventDataSPtr data)
  {
    _eventQueue[_activeQueue].push(data);
  }

  void EventManager::update(double deltaTime)
  {
    auto dispatchQueue = _activeQueue;
    switchActiveQueue();
    while (!_eventQueue[dispatchQueue].empty()) {
      auto &eventData = _eventQueue[dispatchQueue].front();
      triggerEvent(eventData);
      _eventQueue[dispatchQueue].pop();
    }
  }

  void EventManager::switchActiveQueue()
  {
    _activeQueue = (_activeQueue + 1) % 2;
  }
}
