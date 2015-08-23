#include "VoxelObject.hpp"

VoxelObject::VoxelObject(int x, int y, int z, int width, int height, int depth)
{
	voxels.reserve(width * height * depth);	
}

VoxelObject::~VoxelObject()
{
}