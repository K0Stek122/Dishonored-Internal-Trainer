#pragma once
#include <iostream>
#include <Windows.h>

#include "../../Vector/Vector.h"

class Entity
{
protected:
	uintptr_t gameModule;
	uintptr_t entity;

public:
	Entity(uintptr_t entAddy);
	Entity() = default;

	int* GetCurrentHealth();
	void SetCurrentHealth(int value);

	int* GetMaxHealth();
	void SetMaxHealth(int value);

	Vector GetEntityPosition();
	void SetEntityPosition(Vector pos);

	Vector GetEntityViewAngle();
	void SetEntityViewAngle(Vector angle);
};