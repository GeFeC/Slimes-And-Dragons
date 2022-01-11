#include "Player.hpp"

auto Player::printBasicStats() const noexcept -> void{
  util::print("\n<{}lvl {}/{}hp {}/{}xp {}gold> ", 
    this->level, this->hp, this->maxHp, this->xp, this->neededXpForLevelUp, this->gold);
}

auto Player::printStats() const noexcept -> void{
  util::print("\nName: {}\n", this->name);
  util::print("Level: {}\n", this->level);
  util::print("Xp: {}/{}\n\n", this->xp, this->neededXpForLevelUp);

  util::print("HP: {}/{}\n", this->hp, this->maxHp);
  util::print("Damage: {}\n", this->damage);
  util::print("Damage Boost per level: {}\n", this->damageBoostPerLevel);
  util::print("Max Hp Boost per level: {}\n", this->hpBoostPerLevel);
}

auto Player::handleRest() noexcept -> void{
  const auto restStart = util::getCurrentTimePoint();

  util::printAndWait("\nTaking a rest... Press any key to stop\n");

  const auto restStop = util::getCurrentTimePoint();

  auto regainedHp = std::chrono::duration_cast<std::chrono::seconds>(restStop - restStart).count();
 
  if (this->hp + regainedHp > this->maxHp)
    regainedHp = this->maxHp - this->hp;

  this->hp += regainedHp;
  util::print("You regained {} hp!\n", regainedHp);
}

auto Player::handleHeal() noexcept -> void{
  util::print("\n1hp -> 5gold\n");
  auto wantedHp = util::getInput<std::int32_t>("How much health do you want to regain? 0 for full: ");

  const auto userEnteredTooBigValue = this->hp + wantedHp > this->maxHp;

  if (wantedHp == 0 || userEnteredTooBigValue){
    wantedHp = this->maxHp - this->hp;
  }

  if (wantedHp * Player::ONE_HP_COST > this->gold){
    util::print("You don't have enough gold!\n");
    return;
  }

  util::print("You regained {} health!\n", wantedHp);

  this->gold -= wantedHp * Player::ONE_HP_COST;
  this->hp += wantedHp;
}

auto Player::handleShop() noexcept -> void{
  if (this->drunkPotion.type != Potion::Type::NONE){
    util::print("\nYou cannot buy more than one potion at a time!\n");
    return;
  }

  Player::_printShop();

  util::printNow("All potions work for next 3 battles!\n\n");

  util::print("[OTHER] Exit\n");
  const auto choice = util::getInput<std::int32_t>("Your choice: ");

  const auto potionTypesCount = util::toInt(Potion::Type::TYPES_COUNT);
  if (choice >= 1 && choice < potionTypesCount){
    const auto type = static_cast<Potion::Type>(choice);
    const auto name = Potion::getNameByType(type);
    const auto price = Potion::getPriceByType(type);

    if (price > this->gold){
      util::print("You don't have enough gold!\n");
      return;
    }

    this->gold -= price;
    this->drunkPotion.type = type;
    this->drunkPotion.usages = Player::INITIAL_POTION_USAGES_COUNT;

    util::print("You purchased {} Potion for {} gold!", name, price);
  }
}

auto Player::_printShop() noexcept -> void{
  const auto potionTypesCount = util::toInt(Potion::Type::TYPES_COUNT);

  for (std::int32_t i = 1; i < potionTypesCount; ++i){
    const auto potionType = static_cast<Potion::Type>(i);
    const auto potionName = util::resizeText(Potion::getNameByType(potionType), 20);
    const auto potionDescription = util::resizeText(Potion::getDescriptionByType(potionType), 30);
    const auto potionPrice = Potion::getPriceByType(potionType);

    util::printNow("[{}] {} {} {}gold\n", 
      i,
      potionName, 
      potionDescription,
      potionPrice
    );
  }
}

auto Player::addDrunkPotionEffectBeforeBattle() noexcept -> std::int32_t{
  auto boostValue = std::int32_t{};
  
  switch(this->drunkPotion.type){
    case Potion::Type::HEALTH:
      boostValue = this->maxHp * Player::HP_POTION_BOOST;

      this->hp = this->maxHp += boostValue;
      util::print("HEALTH POTION BOOSTS YOUR HP TO {}!\n\n", this->maxHp);

      return boostValue;

    case Potion::Type::STRENGTH:
      boostValue = this->damage * Player::STRENGTH_POTION_BOOST;

      this->damage += boostValue;
      util::print("STRENGTH POTION BOOSTS YOUR DAMAGE TO {}!\n\n", this->damage);

      return boostValue;

    case Potion::Type::CRITICAL:
      util::print("CRITICAL POTION BOOSTS CHANCE OF A CRITICAL HIT TO 50%!\n\n");
      return 0;

    default:
      return 0;
  }
}

auto Player::restoreStatsToStateBeforeDrinkingPotion(std::int32_t boostValue) noexcept -> void{
  switch(this->drunkPotion.type){
    case Potion::Type::HEALTH:
      this->maxHp -= boostValue;
      this->hp = this->maxHp;
      break;

    case Potion::Type::STRENGTH:
      this->damage -= boostValue;
      break;

    default:
      return;
  }
}

auto Player::decreaseDrunkPotionUsage() noexcept -> void{
  --this->drunkPotion.usages;

  if (this->drunkPotion.usages == 0)
    this->clearPotionEffect();
}

auto Player::handleAttack(Monster& attackedMonster) noexcept -> bool{
  auto damage = this->damage;

  util::print("\n");

  const auto randomValue = util::getRandomValue(0, 9);
  const auto criticalHitOccured = 
    (this->drunkPotion.type == Potion::Type::CRITICAL) ? 
    randomValue < 5 : randomValue < 1;

  if (criticalHitOccured){
    util::print("CRITICAL HIT!\n");
    damage *= 2;
  }

  util::print("You hit {} for {} damage!\n\n", attackedMonster.name, damage);
  attackedMonster.hp -= damage;

  if (attackedMonster.isDead()){
    this->handleReceivingRewardForKillingMonster(attackedMonster);
    
    return Player::KILLED_MONSTER;
  }

  return Player::DIDNT_KILL_MONSTER;
}

auto Player::handleReceivingRewardForKillingMonster(Monster& killedMonster) noexcept -> void{
  const auto xpMultiplier = (this->drunkPotion.type == Potion::Type::XP) ? 2 : 1;

  const auto gainedXp = killedMonster.getXpAfterDefeat() * xpMultiplier;

  util::print("You killed {}!\n", killedMonster.name);
  util::print("You gained {} gold!\n", killedMonster.droppedGoldCount);
  util::print("You gained {} xp!\n", gainedXp);
  
  this->gold += killedMonster.droppedGoldCount;

  this->xp += gainedXp;

  while (this->xp >= this->neededXpForLevelUp)
    this->handleLevelUp();

  if (this->drunkPotion.usages == 0)
    this->clearPotionEffect();
}

auto Player::handleLevelUp() noexcept -> void{
  ++this->level;

  if (this->level % 3 == 0){
    this->hpBoostPerLevel += 2;
    ++this->damageBoostPerLevel;
  }

  this->maxHp += this->hpBoostPerLevel;
  this->hp = this->maxHp;
  this->damage += this->damageBoostPerLevel;
  this->gold += this->level * Player::GOLD_RECEIVED_PER_LEVEL;

  this->xp -= this->neededXpForLevelUp;
  this->neededXpForLevelUp += (this->level % 2 == 0 ? 20 : 30) * (this->level / 5 + 1);

  util::print("\nYou advanced to level {}!\n", this->level);
  util::print("You gain {} gold!\n", this->level * Player::GOLD_RECEIVED_PER_LEVEL);
}

auto Player::clearPotionEffect() noexcept -> void{
  if (this->drunkPotion.type == Potion::Type::NONE)
    return;

  this->drunkPotion.type = Potion::Type::NONE;
  util::print("POTION EFFECT WORE OFF!\n");
}