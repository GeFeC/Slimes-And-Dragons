#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "../Creature/Creature.hpp"
#include <vector>
#include <cstdint>

class Monster :public Creature{
public:
  enum class Type{
    SLIME,
    ZOMBIE,
    ORC,
    OGRE,
    DRAGON,
    TYPES_COUNT
  };

  std::int32_t droppedGoldCount;

  Monster(Type type, std::int32_t level) noexcept;
  Monster() noexcept {};

  auto getXpAfterDefeat() noexcept -> std::int32_t;
  
  static auto getNameByType(Type type) noexcept -> std::string;
};

#endif //MONSTER_HPP