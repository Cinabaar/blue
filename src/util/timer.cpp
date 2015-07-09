#include "timer.hpp"


void Timer::start() {
  _previousTime = std::chrono::high_resolution_clock::now();
  _running = true;
}
double Timer::delta() {
  if (_running) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    double delta = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - _previousTime).count();
    _previousTime = currentTime;
    return delta;
  }
  return 0;
}
void Timer::stop() {
  _running = false;
}
bool Timer::isRunning() const {
  return _running;
}
