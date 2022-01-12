#ifndef GAME_HPP
#define GAME_HPP

#include "../Monster/Monster.hpp"
#include "../Player/Player.hpp"
#include "../Util.hpp"

#include <vector>
#include <map>
#include <functional>

class Game{
private:
  using Commands_t = std::map<std::string, std::function<void(void)>>;

  static constexpr float MAX_HP_BAR_SIZE = 20.f;

  static constexpr bool MONSTER_KILLED_PLAYER = true;
  static constexpr bool MONSTER_DIDNT_KILL_PLAYER = false;

  static constexpr std::int32_t MIN_PLAYER_LEVEL_FOR_BOSS_TO_SPAWN = 20;
  static constexpr std::int32_t BOSS_DROPPED_GOLD_COUNT = 10000;
  static constexpr std::int32_t BOSS_MAX_HP_BOOST = 50;
  static constexpr std::int32_t BOSS_DAMAGE_BOOST = 20;

  static constexpr std::int32_t MIN_SPAWNED_MONSTERS_COUNT = 6;
  static constexpr std::int32_t MAX_SPAWNED_MONSTERS_COUNT = 12;

  static constexpr std::int32_t CHANCE_FOR_PLAYER_TO_FLEE = 50;
  static constexpr std::int32_t CHANCE_OF_BEING_ATTACKED_AFTER_REST = 30;

  bool gameOver = false;
  Player player;

  std::vector<Monster> currentlyEncounteredMonsters;

  auto _askForPlayerName() noexcept -> void;
  auto _askForShowingRules() const noexcept -> void;

  auto _printTitle() const noexcept -> void;
  auto _printCurrentlyEncounteredMonsters() const noexcept -> void;
  auto _printStatsDuringBattle(Monster& attackedMonster) const noexcept -> void;
  auto _printRules() const noexcept -> void;
  auto _printCommandsList() const noexcept -> void;

  auto _spawnMonsters() noexcept -> void;

  auto _handleAttack() noexcept -> void;
  auto _handleMonsterEncountering() noexcept -> void;
  auto _handleBossEncountering() noexcept -> void;
  auto _handleFightScene(Monster& attackedMonster) noexcept -> void;
  auto _handlePlayerRest() noexcept -> void;
  auto _handleMainInterface() noexcept -> void;

  auto _handleMonsterAttack(Monster& attackingMonster) noexcept -> bool; 

  auto _getRandomMonster() noexcept -> std::pair<Monster::Type, std::int32_t>;
  auto _getMonsterIdFromInput() const noexcept -> std::int32_t;

  auto _generateBoss() const noexcept -> Monster;

  auto _didBossSpawn() const noexcept -> bool;

public:
  auto run() noexcept -> void;
};

#endif //GAME_HPP