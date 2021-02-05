#include "Entity.hpp"

Entity::Entity(uintptr_t entAddy)
{
	this->gameModule = (uintptr_t)GetModuleHandle(L"Dishonored.exe");
	this->entity = *(uintptr_t*)(gameModule + entAddy);
}

int* Entity::GetCurrentHealth()
{
	return (int*)(this->entity + 0x344);
}

void Entity::SetCurrentHealth(int value)
{
	*(int*)(this->entity + 0x344) = value;
}

int* Entity::GetMaxHealth()
{
	return (int*)(this->entity + 0x348);
}

//To Set Max Health, you need to OP the instruction (NOTE: AALL ENTITIES USE THIS FUNCTION):
//89 9E ? ? ? ? 7F
//\x89\x9E\x00\x00\x00\x00\x7F xx????x
void Entity::SetMaxHealth(int value)
{
	*(int*)(this->entity + 0x348) = value;
}

Vector Entity::GetEntityPosition()
{
	Vector pos;
	pos.x = *(float*)(this->entity + 0xC4);
	pos.z = *(float*)(this->entity + 0xC8);
	pos.y = *(float*)(this->entity + 0xCC);
	return pos;
}

void Entity::SetEntityPosition(Vector pos)
{
	*(float*)(this->entity + 0xC4) = pos.x;
	*(float*)(this->entity + 0xC8) = pos.y;
	*(float*)(this->entity + 0xCC) = pos.z;
}

Vector Entity::GetEntityViewAngle()
{
	Vector angle = Vector();
	angle.x = *(float*)(this->entity + 0x50);
	angle.y = *(float*)(this->entity + 0x54);
	return angle;
}

void Entity::SetEntityViewAngle(Vector angle)
{
	*(float*)(this->entity + 0x50) = angle.x;
	*(float*)(this->entity + 0x54) = angle.y;
}