#include "GrassVoxel.hpp"

GrassVoxel::GrassVoxel(float x, float y, float z)
:Voxel(x, y, z, 16.f, 1.f, 1.f, 1.f, 1.f, 0.f)
{
	isAirBlock = false;

	setFaceTextureIndex(VOXEL_FACE_TOP, 0.f);
	setFaceTextureIndex(VOXEL_FACE_BOTTOM, 4.f);
	setFaceTextureIndex(VOXEL_FACE_RIGHT, 4.f);
	setFaceTextureIndex(VOXEL_FACE_LEFT, 4.f);
	setFaceTextureIndex(VOXEL_FACE_FRONT, 4.f);
	setFaceTextureIndex(VOXEL_FACE_BACK, 4.f);
}