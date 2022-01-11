#include "Game.hpp"

auto Game::run() noexcept -> void{
  this->_printTitle();
  this->_askForPlayerName();
  this->_askForShowingRules();

  while(true)
    this->_handleMainInterface();
}

auto Game::_printTitle() const noexcept -> void{
  util::print("SLIMES AND DRAGONS\n");
  util::print("By: GeFeC\n");
}

auto Game::_askForPlayerName() noexcept -> void{
  util::print("Enter your name: ");
  this->player.name = util::getInput();

  util::print("Welcome, {}!\n", this->player.name);
}

auto Game::_askForShowingRules() const noexcept -> void{
  const auto choice = util::getInput<char>(
    "\nDo You want to read the rules?\n"
    "(Note: You can always see them by using 'rules' command!)\n"
    "Y - Yes, OTHER - No: "
  );

  if (choice == 'Y' || choice == 'y')
    this->_printRules();
}

auto Game::_printRules() const noexcept -> void{
  util::printAndWait(
    "THE MAIN ASSUMPTION\n"
    "'Slimes And Dragons' is a simple Command Line RPG where you control player\n"
    "by using specific commands. Your main task is to defeat as many monsters as\n"
    "You can and not to be defeated yourself. The game ends after defeating the\n"
    "final boss which spawns after reaching lvl 20. The chance of spawning is\n"
    "equal to player's level, so the higher the level, the higher the chance for\n"
    "the final boss to spawn. GOOD LUCK :D\n"
  );

  util::printAndWait(
    "COMMANDS\n"
    "stats - displays all of your statistics.\n"
  );

  util::printAndWait(
    "attack - displays a list of 6-12 randomly generated monsters. Then You choose\n"
    "  the number of the monster you want to fight and the battle begins.\n"
  );

  util::printAndWait(
    "rest - lets You regain Your HP. After using this command the game starts sleeping.\n"
    "  In order to stop resting, just press any key. The more seconds You wait the more\n"
    "  HP you will regain. However, there is a 30% chance of being attacked by random\n"
    "  monster after the rest\n"
  );

  util::printAndWait(
    "heal - lets You regain Your HP for Your gold. 1HP costs 5 gold\n"
  );

  util::printAndWait(
    "shop - opens potion shop\n"
  );

  util::printAndWait("You can always see the list of available commands by using 'commands'\n");

  util::print(
    "MONSTERS:\n"
    "Here's the list of all monsters in game, from the weakest to the strongest: \n\n"
  );

  for (std::int32_t i = 0; i < util::toInt(Monster::Type::TYPES_COUNT); ++i){
    util::print("{}\n", Monster::getNameByType(static_cast<Monster::Type>(i)));
  }

  util::printAndWait(
    "\nFighting Slimes and Zombies at the beginning of the game is a good idea\n"
    "but attacking a Dragon is suicide ;)\n"
  );

  util::print("THE GAME BEGINS NOW! GOOD LUCK!\n");
}

auto Game::_handleMainInterface() noexcept -> void{
  this->player.printBasicStats();
  const auto command = util::getInput();

  if (command == "stats"){
    this->player.printStats();
    return;
  }
  if (command == "attack"){
    this->_handleAttack();
    return;
  }
  if (command == "rest"){
    this->_handlePlayerRest();
    return;
  }
  if (command == "heal"){
    this->player.handleHeal();
    return;
  }
  if (command == "shop"){
    this->player.handleShop();
    return;
  }
  if (command == "rules"){
    this->_printRules();
    return;
  }
  if (command == "commands"){
    this->_printCommandsList();
    return;
  }
}

auto Game::_handleAttack() noexcept -> void{
  if (this->_didBossSpawn()){
    this->_handleBossEncountering();
    return;
  }

  this->_handleMonsterEncountering();
}

auto Game::_didBossSpawn() const noexcept -> bool{
  const auto chanceOfBossSpawning = this->player.level;

  return 
    this->player.level >= Game::MIN_PLAYER_LEVEL_FOR_BOSS_SPAWNING && 
    util::passWithChanceOf(chanceOfBossSpawning);
}

auto Game::_handleBossEncountering() noexcept -> void{
  util::print("THE FINAL BOSS HAS SPAWNED!\n\n");
  auto boss = this->_generateBoss();

  this->_handleFightScene(boss);
  util::print("CONGRATULATIONS! YOU DEFEATED THE FINAL BOSS AND WON THE GAME!\n\n");
  while(true);
}

auto Game::_generateBoss() const noexcept -> Monster{
  auto boss = Monster{};

  boss.name = "ROCK GOLEM";
  boss.level = this->player.level;
  boss.maxHp = boss.hp = this->player.maxHp + Game::BOSS_MAX_HP_BOOST;
  boss.damage = this->player.damage + Game::BOSS_DAMAGE_BOOST;
  boss.droppedGoldCount = Game::BOSS_DROPPED_GOLD_COUNT;

  return boss;
}

auto Game::_handleMonsterEncountering() noexcept -> void{
  this->_spawnMonsters();

  util::print("There are {} monsters nearby: \n", this->currentlyEncounteredMonsters.size());
  this->_printCurrentlyEncounteredMonsters();

  util::print("\nChoose monster to fight: ");
  const auto monsterId = this->_getMonsterIdFromInput(); 

  this->_handleFightScene(this->currentlyEncounteredMonsters[monsterId]);

  this->currentlyEncounteredMonsters.clear();
}

auto Game::_spawnMonsters() noexcept -> void{
  const auto monstersCount = util::getRandomValue(
    Game::MIN_SPAWNED_MONSTERS_COUNT, 
    Game::MAX_SPAWNED_MONSTERS_COUNT
  );

  for (std::int32_t i = 0; i < monstersCount; ++i){
    const auto [monsterType, monsterLevel] = this->_getRandomMonster();

    this->currentlyEncounteredMonsters.emplace_back(monsterType, monsterLevel);
  }
}

auto Game::_getRandomMonster() noexcept -> std::pair<Monster::Type, std::int32_t>{
  const auto monsterTypeId = util::getRandomValue(0, static_cast<std::int32_t>(Monster::Type::TYPES_COUNT) - 1);
  const auto monsterLevel = util::getRandomValue(
    (this->player.level < 6) ? 1 : this->player.level - 5, 
    1 + this->player.level + (this->player.level / 3)
  );
  
  return { static_cast<Monster::Type>(monsterTypeId), monsterLevel };
};

auto Game::_printCurrentlyEncounteredMonsters() const noexcept -> void{
  auto i = 1;
  for (const auto& monster : this->currentlyEncounteredMonsters){
    util::print("[{}] {} lvl:{}\n", i, monster.name, monster.level);
    ++i;
  }
}

auto Game::_getMonsterIdFromInput() const noexcept -> std::int32_t{
  auto monsterId = util::getInput<std::int32_t>() - 1;

  while(monsterId < 0 || monsterId >= this->currentlyEncounteredMonsters.size()){
    monsterId = util::getInput<std::int32_t>("Invalid value! Try again: ");
  }

  return monsterId;
}

auto Game::_handleFightScene(Monster& attackedMonster) noexcept -> void{
  const auto potionBoost = this->player.addDrunkPotionEffectBeforeBattle(); 

  while(!attackedMonster.isDead()){
    this->_printStatsDuringBattle(attackedMonster);

    const auto choice = util::getInput<char>("Type 'r' to run, or any other key to fight: ");
    const auto playerWantsToFlee = (choice == 'r' || choice == 'R');
    const auto successfullyFled = util::getRandomValue(0, 1) == 0;

    if (playerWantsToFlee){
      if (successfullyFled){
        util::print("You fled successfully!\n");
        break;
      }
    }
    else{
      if (this->player.handleAttack(attackedMonster) == Player::KILLED_MONSTER)
        break;
    }

    this->_handleMonsterAttack(attackedMonster);
  }

  if (this->player.drunkPotion.type != Potion::Type::NONE){
    this->player.restoreStatsToStateBeforeDrinkingPotion(potionBoost);
    this->player.decreaseDrunkPotionUsage();
  }
}

auto Game::_printStatsDuringBattle(Monster& attackedMonster) const noexcept -> void{
  util::printNow("{} lvl:{}\n", attackedMonster.name, attackedMonster.level);
  attackedMonster.printHpBar();
  util::printNow("\n");

  util::printNow("{} lvl:{}\n", this->player.name, this->player.level);
  this->player.printHpBar();
  util::printNow("\n");
}

auto Game::_handleMonsterAttack(Monster& attackingMonster) noexcept -> bool{
  util::print("{} hits you for {} damage!\n\n", attackingMonster.name, attackingMonster.damage);
  this->player.hp -= attackingMonster.damage;

  if (this->player.isDead()){
    util::print("GAME OVER!\n");
    while(true);
    
    return Game::MONSTER_KILLED_PLAYER;
  }

  return Game::MONSTER_DIDNT_KILL_PLAYER;
}

auto Game::_handlePlayerRest() noexcept -> void{
  this->player.handleRest();

  if (util::getRandomValue(0, 9) < 3){
    util::print("Some monster attacks you while you're resting!\n\n");
    const auto [monsterType, monsterId] = this->_getRandomMonster();

    this->currentlyEncounteredMonsters.emplace_back(monsterType, monsterId);

    this->_handleFightScene(this->currentlyEncounteredMonsters[0]);
  }
}

auto Game::_printCommandsList() const noexcept -> void{
  util::print("Here's the list of all available commands:\n");
  util::print("stats\n");
  util::print("attack\n");
  util::print("rest\n");
  util::print("heal\n");
  util::print("shop\n");
  util::print("rules\n");
  util::print("commands\n");
}