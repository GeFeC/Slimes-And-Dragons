#include "Creature.hpp"

auto Creature::isDead() const noexcept -> bool{
  return this->hp <= 0;
}

auto Creature::printHpBar() const noexcept -> void{
  using namespace util::operator_overloads;
  using namespace std::string_literals;

  const auto currentHpBarSize = std::ceil(Creature::MAX_HP_BAR_SIZE * this->hp / this->maxHp);
  
  util::printNow("Hp: |{}{}|\n", 
    "="s * currentHpBarSize, 
    " "s * (Creature::MAX_HP_BAR_SIZE - currentHpBarSize));
}