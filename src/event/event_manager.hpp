#pragma once

#include <queue>
#include <map>

#include "event_data.hpp"
#include "util/util.hpp"
#include "nod/nod.hpp"

namespace event {

  enum class EventType;
  using EventSignal = nod::unsafe_signal<void(IEventDataSPtr&)>;
  using Connection = nod::connection;
  class EventManager : NonCopyable<EventManager> {
  private:
    int _activeQueue;
    std::queue<IEventDataSPtr> _eventQueue[2];
    // _event_queue[_active_queue] - new events are stored here.
    // _event_queue[(_active_queue+1)%2] - dispatching is done from this queue.
    
    std::map<EventType, EventSignal> _eventBinding;

  private:
    EventManager() : _activeQueue(0) {
    }
    void switchActiveQueue();

  public:
    static EventManager& get() {
      static EventManager eventManager;
      return eventManager;
    }
    Connection addListener(EventType eventType, std::function<void(IEventDataSPtr &)> callback);
    void triggerEvent(IEventDataSPtr eventData);
    void queueEvent(IEventDataSPtr eventData);
    void update(double deltaTime);
  };
}
