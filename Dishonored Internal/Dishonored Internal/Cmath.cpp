#include "Cmath.hpp"

bool WorldToScreen(vec3_t pos, vec2 &screen, float matrix[16], int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	vec4 clipCoords;
	clipCoords.x = pos.x*matrix[0] + pos.y*matrix[1] + pos.z*matrix[2] + matrix[3];
	clipCoords.y = pos.x*matrix[4] + pos.y*matrix[5] + pos.z*matrix[6] + matrix[7];
	clipCoords.z = pos.x*matrix[8] + pos.y*matrix[9] + pos.z*matrix[10] + matrix[11];
	clipCoords.w = pos.x*matrix[12] + pos.y*matrix[13] + pos.z*matrix[14] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	vec3_t NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

float DotProduct(vec3_t src, vec3_t dst)
{
	return src.x * dst.x + src.y * dst.y + src.z * dst.z;
}

float Magnitude(vec3_t vec)
{
	return sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

vec3_t Divide(vec3_t src, float num)
{
	vec3_t vec;
	vec.x = src.x / num;
	vec.y = src.y / num;
	vec.z = src.z / num;

	return vec;
}

vec3_t Subtract(vec3_t src, vec3_t dst)
{
	vec3_t diff;
	diff.x = src.x - dst.x;
	diff.y = src.y - dst.y;
	diff.z = src.z - dst.z;
	return diff;
}