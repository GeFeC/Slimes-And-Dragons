#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../Creature/Creature.hpp"
#include "../Monster/Monster.hpp"
#include "../Util.hpp"
#include "../Potion/Potion.hpp"

#include <cstdint>
#include <string>
#include <limits>

class Player :public Creature{
private:
  static constexpr float STRENGTH_POTION_BOOST = 0.3;
  static constexpr float HP_POTION_BOOST = 0.3;

  static constexpr std::int32_t ONE_HP_COST = 5;
  static constexpr std::int32_t GOLD_RECEIVED_PER_LEVEL = 20;
  static constexpr std::int32_t INITIAL_POTION_USAGES_COUNT = 3;

  static auto _printShop() noexcept -> void;

public:
  static constexpr bool KILLED_MONSTER = true;
  static constexpr bool DIDNT_KILL_MONSTER = false;

  std::string name;
  std::int32_t xp = 0;
  std::int32_t neededXpForLevelUp = 20;
  std::int32_t gold = 0;

  std::int32_t damageBoostPerLevel = 2;
  std::int32_t hpBoostPerLevel = 5;

  Potion drunkPotion = { Potion::Type::NONE };

  Player() : Creature(name, 20, 5, 0) {}

  auto handleLevelUp() noexcept -> void;
  auto handleRest() noexcept -> void;
  auto handleHeal() noexcept -> void;
  auto handleShop() noexcept -> void;

  auto handleAttack(Monster& attackedMonster) noexcept -> bool;
  auto handleReceivingRewardForKillingMonster(Monster& monster) noexcept -> void;

  auto addDrunkPotionEffectBeforeBattle() noexcept -> std::int32_t;

  //boostValue - amount of either Hp or Damage added due to boost
  auto restoreStatsToStateBeforeDrinkingPotion(std::int32_t oldValue) noexcept -> void;
  auto clearPotionEffect() noexcept -> void;
  auto decreaseDrunkPotionUsage() noexcept -> void;

  auto printBasicStats() const noexcept -> void;
  auto printStats() const noexcept -> void;
};

#endif //PLAYER_HPP