#pragma once

#include "event/event_data.hpp"
#include "event/event_type.hpp"
#include "glm/glm.hpp"

namespace event {
  class OnWindowResized : public IEventData {
  private:
    const int width_;
    const int height_;

  public:
    OnWindowResized(int width, int height) : width_(width), height_(height) {};
    virtual EventType getEventType() const override {return EventType::WINDOW_RESIZED;};
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
  };

  class OnCursorPositionChanged : public IEventData {
  private:
    glm::vec2 moveVec_;
  public:
    OnCursorPositionChanged(glm::vec2 moveVec): moveVec_(moveVec) {}
    virtual EventType getEventType() const override {return EventType::MOUSE_CURSOR_POS;};
    const glm::vec2 getVec() const { return moveVec_; }
  };

  
}
