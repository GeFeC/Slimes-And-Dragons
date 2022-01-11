#ifndef POTION_HPP
#define POTION_HPP

#include <cstdint>

struct Potion{
  enum class Type{
    NONE,
    CRITICAL,
    STRENGTH,
    XP,
    HEALTH,
    TYPES_COUNT
  } type;

  std::int32_t usages;

  static auto getNameByType(Potion::Type potionType) noexcept{
    switch(potionType){
      case Type::HEALTH: return "Health Potion";
      case Type::STRENGTH: return "Strength Potion";
      case Type::CRITICAL: return "Critical Potion";
      case Type::XP: return "Double XP Potion";
      default: return "";
    }
  }

  static auto getDescriptionByType(Potion::Type potionType) noexcept{
    switch(potionType){
      case Type::HEALTH: return "+30% Max Hp Boost";
      case Type::STRENGTH: return "+30% Damage Boost";
      case Type::CRITICAL: return "50% Critical Hit Chance";
      case Type::XP: return "+100% XP";
      default: return "";
    }
  }

  static auto getPriceByType(Potion::Type potionType) noexcept -> std::int32_t{
    switch(potionType){
      case Type::CRITICAL: return 100;
      case Type::STRENGTH: return 200;
      case Type::XP: return 400;
      case Type::HEALTH: return 500;
      default: return 0;
    }
  }
};

#endif //POTION_HPP