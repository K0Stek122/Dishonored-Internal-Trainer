#pragma once
#include <Windows.h>
#include <iostream>

#define PI 3.1415927f

struct vec3_t
{
	float x, y, z;
};

struct vec2
{
	float x, y;
};

struct vec4
{
	float x, y, z, w;
};

vec3_t Subtract(vec3_t src, vec3_t dst);

vec3_t Divide(vec3_t src, float num);

float Magnitude(vec3_t vec);

float DotProduct(vec3_t src, vec3_t dst);

bool WorldToScreen(vec3_t pos, vec2 &screen, float matrix[16], int windowWidth, int windowHeight);