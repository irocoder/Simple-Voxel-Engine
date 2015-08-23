#include "StoneVoxel.hpp"

StoneVoxel::StoneVoxel(float x, float y, float z)
:Voxel(x, y, z, 16.f, 1.f, 1.f, 1.f, 1.f, 7.f)
{
	isAirBlock = false;
}