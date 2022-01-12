#include "Monster.hpp"
#include "../Util.hpp"

Monster::Monster(Type type, std::int32_t level) noexcept{
  this->name = Monster::getNameByType(type);
  this->level = level;

  switch(type){
    case Type::SLIME:
      this->droppedGoldCount = util::getRandomValue(10, 20) + util::getSumOfRandomValues(level, 3, 4);
      this->hp = this->maxHp = 10 + util::getSumOfRandomValues(level, 3, 4);
      this->damage = util::getSumOfRandomValues(level, 2, 3);
      break;

    case Type::ZOMBIE:
      this->droppedGoldCount = util::getRandomValue(20, 30) + util::getSumOfRandomValues(level, 4, 5);
      this->hp = this->maxHp = 10 + util::getSumOfRandomValues(level, 4, 5);
      this->damage = util::getSumOfRandomValues(level, 2, 3);
      break;

    case Type::ORC:
      this->droppedGoldCount = util::getRandomValue(30, 40) + util::getSumOfRandomValues(level, 6, 7);
      this->hp = this->maxHp = 20 + util::getSumOfRandomValues(level, 6, 7);
      this->damage = util::getSumOfRandomValues(level, 3, 4);
      break;

    case Type::OGRE:
      this->droppedGoldCount = util::getRandomValue(40, 50) + util::getSumOfRandomValues(level, 8, 9);
      this->hp = this->maxHp = 20 + util::getSumOfRandomValues(level, 8, 9);
      this->damage = util::getSumOfRandomValues(level, 4, 5);
      break;

    case Type::DRAGON:
      this->droppedGoldCount = util::getRandomValue(50, 60) + util::getSumOfRandomValues(level, 10, 12);
      this->hp = this->maxHp = 30 + util::getSumOfRandomValues(level, 10, 12);
      this->damage = util::getSumOfRandomValues(level, 5, 6);
      break;

    default: break;
  }
}

auto Monster::getNameByType(Type type) noexcept -> std::string{
  switch(type){
    case Type::SLIME:
      return "SLIME";
    
    case Type::ZOMBIE:
      return "ZOMBIE";

    case Type::ORC:
      return "ORC";

    case Type::OGRE:
      return "OGRE";

    case Type::DRAGON:
      return "DRAGON";

    default:
      return "";
  }
}

auto Monster::getXpAfterDefeat() noexcept -> std::int32_t{
  return 
    util::getRandomValue(this->maxHp - 2, this->maxHp + 2) +
    util::getRandomValue(this->damage - 1, this->damage + 1);
}