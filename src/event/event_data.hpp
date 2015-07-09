#pragma once

#include "event_type.hpp"
#include <memory>
#include <string>


namespace event {
  class IEventData
  {
  public:
    virtual EventType getEventType() const = 0;
  };
  
  class EventData : public IEventData
  {
  private:
    EventType eventType_;
  public:
    EventData(EventType eventType) : eventType_(eventType) {}
    virtual EventType getEventType() const { return eventType_;}
  };

  using IEventDataSPtr = std::shared_ptr<IEventData>;
}
