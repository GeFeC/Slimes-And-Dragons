#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "../Util.hpp"

#include <string>

class Creature{
private:
  static constexpr float MAX_HP_BAR_SIZE = 20.f;

public:
  std::string name;
  std::int32_t level = 1;
  std::int32_t hp;
  std::int32_t maxHp = 10;
  std::int32_t damage;

  Creature() = default;

  auto printHpBar() const noexcept -> void;

  auto isDead() const noexcept -> bool;
};

#endif //CREATURE_HPP