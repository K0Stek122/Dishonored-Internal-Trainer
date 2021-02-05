#pragma once
#include <iostream>
#include <Windows.h>

#include "../Entity/Entity.hpp"
#include "../../Functions/Functions.hpp"
#include "../AmmoTypes/AmmoTypes.hpp"

class Player : public Entity
{
public:
	Player();

	int* GetCurrentMana();
	void SetCurrentMana(int value);

	int* GetMaxMana();
	void SetMaxMana(int value);

	int* GetRunes();
	void SetRunes(int value);

	int* GetCoins();
	void SetCoins(int value);

	int* GetAmmoAmount(AmmoTypes type);
	void SetAmmoAmount(int value, AmmoTypes type);

	int* GetHealthPotionAmount();
	void SetHealthPotionAmount(int value);

	int* GetManaPotionAmount();
	void SetManaPotionAmount(int value);

	void Update();
};