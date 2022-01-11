#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdint>
#include <sstream>
#include <random>
#include <algorithm>

namespace util{
  using namespace std::chrono_literals;
  using namespace std::string_literals;

  static constexpr std::int32_t TEXT_PRINT_DELAY = 5;

  template<typename T>
  static auto toInt(T value){
    return static_cast<std::int32_t>(value);
  }

  static auto sleep(std::int32_t milliseconds){
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
  }

  static auto format(const std::string& text){
    return text;
  }

  template<typename T, typename... Targs>
  static auto format(const std::string& text, const T& t, const Targs&... args) -> std::string{
    if (auto pos = text.find("{}"); pos != std::string::npos){
      auto stream = std::stringstream{};
      stream << text.substr(0, pos) << t << text.substr(pos + 2);

      return util::format(stream.str(), args...);
    }

    return text;
  }

  template<typename... Targs>
  static auto print(const std::string& text, const Targs&... args){
    const auto formattedText = util::format(text, args...);

    for (const auto e : formattedText){
      std::cout << e;
      util::sleep(util::TEXT_PRINT_DELAY);
    }
  }

  template<typename... Targs>
  static auto printNow(const std::string& text, const Targs&... args){
    const auto formattedText = util::format(text, args...);

    std::cout << formattedText;
  }

  template<typename... Targs>
  static auto printAndWait(const std::string& text, const Targs&... args){
    util::print(text, args...);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
  }

  static auto getCurrentTimePoint(){
    return std::chrono::steady_clock::now();
  }

  static auto getCurrentTimestamp(){
    return getCurrentTimePoint().time_since_epoch().count();
  }

  static std::mt19937 randomNumberGenerator(static_cast<std::mt19937::result_type>(getCurrentTimestamp()));

  static auto getRandomValue(std::int32_t min, std::int32_t max){
    auto engine = std::uniform_int_distribution(min, max);
    return engine(randomNumberGenerator);
  }

  //A function that returns true with chance of (percent)%
  //E.g: passWithChanceOf(20) returns true with chance of 20%
  static auto passWithChanceOf(std::int32_t percent) noexcept -> bool{
    return util::getRandomValue(1, 100) <= percent;
  }

  static auto getSumOfRandomValues(std::int32_t count, std::int32_t min, std::int32_t max){
    auto sum = std::int32_t{};
    for (std::int32_t i = 0; i < count; ++i){
      sum += util::getRandomValue(min, max);
    }

    return sum;
  }
  
  template<typename T = std::string>
  static auto getInput(const std::string& textBeforeInput = ""){
    util::print(textBeforeInput);
    
    auto value = T{};
    std::cin >> value;
    return value;
  }

  static auto resizeText(const std::string& string, std::int32_t size){
    auto resizedString = ""s;
    resizedString.resize(size);
    std::fill(resizedString.begin(), resizedString.end(), ' ');

    for (std::int32_t i = 0; i < string.size(); ++i){
      resizedString[i] = string[i];
    }

    return resizedString;
  }

  namespace operator_overloads{
    static auto operator*(const std::string& string, std::int32_t multiplier){
      auto result = ""s;
      
      while(multiplier--){
        result += string;
      }

      return result;
    }
  };
}

#endif //GAME_HPP