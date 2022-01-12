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

  static auto sleep(std::int32_t milliseconds) noexcept{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
  }

  static auto getCurrentTimePoint() noexcept{
    return std::chrono::steady_clock::now();
  }

  static auto getCurrentTimestamp() noexcept{
    return getCurrentTimePoint().time_since_epoch().count();
  }

  static std::mt19937 randomNumberGenerator(static_cast<std::mt19937::result_type>(util::getCurrentTimestamp()));

  static auto getRandomValue(std::int32_t min, std::int32_t max) noexcept{
    auto engine = std::uniform_int_distribution(min, max);
    return engine(util::randomNumberGenerator);
  }

  template<typename T>
  static auto toInt(T t) noexcept{
    return static_cast<std::int32_t>(t);
  }

  static auto format(const std::string& text) noexcept{
    return text;
  }

  template<typename T, typename... Targs>
  static auto format(const std::string& text, const T& t, const Targs&... args) noexcept -> std::string{
    if (auto pos = text.find("{}"); pos != std::string::npos){
      auto stream = std::stringstream{};
      stream << text.substr(0, pos) << t << text.substr(pos + 2);

      return util::format(stream.str(), args...);
    }

    return text;
  }

  template<typename... Targs>
  static auto print(const std::string& text, const Targs&... args) noexcept{
    const auto formattedText = util::format(text, args...);

    for (const auto e : formattedText){
      std::cout << e;
      util::sleep(util::TEXT_PRINT_DELAY);
    }
  }

  template<typename... Targs>
  static auto printNow(const std::string& text, const Targs&... args) noexcept{
    const auto formattedText = util::format(text, args...);

    std::cout << formattedText;
  }

  template<typename... Targs>
  static auto printAndWait(const std::string& text, const Targs&... args) noexcept{
    util::print(text, args...);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
  }

  //A function that returns true with chance of (percent)%
  //E.g: passWithChanceOf(20) returns true with chance of 20%
  static auto passWithChanceOf(std::int32_t percent) noexcept{
    return util::getRandomValue(1, 100) <= percent;
  }

  //Returns the sum of [count] numbers from [min] to [max]
  static auto getSumOfRandomValues(std::int32_t count, std::int32_t min, std::int32_t max) noexcept{
    auto sum = std::int32_t{};
    for (std::int32_t i = 0; i < count; ++i){
      sum += util::getRandomValue(min, max);
    }

    return sum;
  }
  
  template<typename T = std::string>
  static auto getInput(const std::string& textBeforeInput = "") noexcept -> T{
    util::print(textBeforeInput);
    
    auto value = T{};

    if(!(std::cin >> value)){
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      return util::getInput<T>("Entered Invalid input! Try Again: ");
    }
    return value;
  }

  static auto resizeText(const std::string& string, std::int32_t size) noexcept{
    auto resizedString = ""s;
    resizedString.resize(size);
    std::fill(resizedString.begin(), resizedString.end(), ' ');

    for (std::int32_t i = 0; i < string.size(); ++i){
      resizedString[i] = string[i];
    }

    return resizedString;
  }

  namespace operator_overloads{
    static auto operator*(const std::string& text, std::int32_t multiplier) noexcept{
      auto result = ""s;
      
      while(multiplier--){
        result += text;
      }

      return result;
    }
  };
}

#endif //GAME_HPP