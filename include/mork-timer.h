#ifndef MORK_TIMER_H
#define MORK_TIMER_H

#include <string>
#include <chrono>
#include <numeric>
#include <ranges>
#include <limits>
#include <vector>

class MorkTimer {
public:
  MorkTimer(const std::string& _name);

  void start_measurement();
  void stop_measurement(bool mark_iteration=true);
  void mark_iteration();

private:
  std::string name;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  size_t measurement_index;
  size_t iteration_index;

  // std::vector<double> durations;
  std::vector<double> times;
};

#endif