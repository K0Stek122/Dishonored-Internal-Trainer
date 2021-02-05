#include "Player.hpp"

Player::Player()
{
	this->gameModule = (uintptr_t)GetModuleHandle("Dishonored.exe");
	this->entity = *(uintptr_t*)(gameModule + 0x1052DE8);
}

int* Player::GetCurrentMana()
{
	return (int*)(this->entity + 0xA60);
}

void Player::SetCurrentMana(int value)
{
	*(int*)(this->entity + 0xA60) = value;
}

int* Player::GetMaxMana()
{
	return (int*)(this->entity + 0xA64);
}

//To Set Max Mana, you need to OP the instruction:
//89 86 ? ? ? ? 7E ? 8B C1 8D 8E
//\x89\x86\x00\x00\x00\x00\x7E\x00\x8B\xC1\x8D\x8E xx????x?xxxx
void Player::SetMaxMana(int value)
{
	*(int*)(this->entity + 0xA64) = value;
}

int* Player::GetRunes()
{
	return (int*)Functions::FindDMAAddy(this->gameModule + 0x1052DE8, { 0x270, 0x59C, 0x10, 0xC8, 0x4C });
}

void Player::SetRunes(int value)
{
	*(int*)Functions::FindDMAAddy(this->gameModule + 0x1052DE8, { 0x270, 0x59C, 0x10, 0xC8, 0x4C }) = value;
}

int* Player::GetCoins()
{
	return (int*)Functions::FindDMAAddy(this->gameModule + 0x1052DE8, { 0x270, 0x59C, 0x10, 0xC8, 0x40 });
}

void Player::SetCoins(int value)
{
	*(int*)Functions::FindDMAAddy(this->gameModule + 0x1052DE8, { 0x270, 0x59C, 0x10, 0xC8, 0x40 }) = value;
}

void Player::Update()
{
	this->entity = *(uintptr_t*)(gameModule + 0x1052DE8);
}

int* Player::GetAmmoAmount(AmmoTypes type)
{
	uintptr_t offset = type;
	return (int*)Functions::FindDMAAddy(this->gameModule + 0x1052DE8, { 0x270, 0x59C, 0x10, 0xBC, offset });
}

void Player::SetAmmoAmount(int value, AmmoTypes type)
{
	uintptr_t offset = type;
	*(int*)Functions::FindDMAAddy(this->gameModule + 0x1052DE8, { 0x270, 0x59C, 0x10, 0xBC, offset }) = value;
}

int* Player::GetHealthPotionAmount()
{
	return (int*)Functions::FindDMAAddy(this->gameModule + 0x1052DE8, { 0x270, 0x59C, 0x10, 0xD4 });
}

void Player::SetHealthPotionAmount(int value)
{
	*(int*)Functions::FindDMAAddy(this->gameModule + 0x1052DE8, { 0x270, 0x59C, 0x10, 0xD4 }) = value;
}

int* Player::GetManaPotionAmount()
{
	return (int*)Functions::FindDMAAddy(this->gameModule + 0x1052DE8, { 0x270, 0x59C, 0x10, 0xD8 });
}

void Player::SetManaPotionAmount(int value)
{
	*(int*)Functions::FindDMAAddy(this->gameModule + 0x1052DE8, { 0x270, 0x59C, 0x10, 0xD8 }) = value;
}