#include <string>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "mork-timer.h"


namespace {
  constexpr size_t N = 60;
}


  MorkTimer::MorkTimer(const std::string& _name):
    name{_name},
    start_time{},
    measurement_index{0},
    iteration_index{0},
    // durations(N),
    times() {}

  void MorkTimer::start_measurement() {
    start_time = std::chrono::high_resolution_clock::now();
  }

  void MorkTimer::stop_measurement(bool mark_iteration) {
    auto measurement = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
    if (measurement_index < times.size()) {
      times[measurement_index] = measurement;
    } else {
      times.push_back(measurement);
    }
    measurement_index += 1;

    if (mark_iteration) {
      this->mark_iteration();
    }
  }

  void MorkTimer::mark_iteration() {
    
    iteration_index = (iteration_index + 1);

    if (iteration_index == N) {
      
      // for (size_t n = 0; n < N; ++n) {
      //   durations[n] = std::chrono::duration_cast<std::chrono::microseconds>(times[n+1] - times[n]).count();
      // }
      size_t num_measurements = times.size();

      auto mean_duration = std::accumulate(std::begin(times), std::end(times), 0.0) / num_measurements;
      auto min_duration = *std::min_element(std::begin(times), std::end(times));
      auto max_duration = *std::max_element(std::begin(times), std::end(times));

      // Use times to calculate standard deviation.
      for (size_t t = 0; t < num_measurements; ++t) {
        times[t] = std::pow(times[t] - mean_duration, 2.0);
      }

      auto variance = std::accumulate(std::begin(times), std::end(times), 0.0) / num_measurements;
      auto stdev = std::sqrt(variance);

      auto print_us = [](double val){
          std::cout << std::setfill(' ') << std::setw(6) << int(val);
          auto decimals = int(val*100)%100;
          std::cout << ".";
          std::cout << std::setfill('0') << std::setw(2) << decimals;
      };

      std::cout << name
        << " measurements: " << std::setw(8) << std::setfill(' ') << num_measurements << "," 
        << " mean: "; print_us(mean_duration); std::cout << "us," 
        << " stdev="; print_us(stdev); std::cout << "us,"
        << " mean-min="; print_us(mean_duration - min_duration); std::cout << "us,"
        << " max-mean="; print_us(max_duration - mean_duration); std::cout << "us" << std::endl;
   
      iteration_index = 0;
      measurement_index = 0;
    }    
  }