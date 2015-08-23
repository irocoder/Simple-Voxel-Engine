#include "AirVoxel.hpp"

AirVoxel::AirVoxel(float x, float y, float z)
:Voxel(x, y, z, 16.f, 1.f, 0.f, 0.f, 0.f, BLOCK_USE_VERTEX_COLOR)
{
	for(int i = 0; i < 6; i++)
	{
		drawFace[i] = VOXEL_DRAW_STATE_FALSE;
	}

	isAirBlock = true;
	transparent = true;
	drawableFaceCount = 0;
}