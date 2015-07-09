#pragma once

#include <chrono>

class Timer
{
private:
  std::chrono::high_resolution_clock::time_point _previousTime;
  bool _running;
public:
  bool isRunning() const;
  void start();
  double delta();
  void stop();
};
