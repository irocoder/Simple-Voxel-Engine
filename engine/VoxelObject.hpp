#ifndef VOXEL_OBJECT_HPP
#define VOXEL_OBJECT_HPP

//IDEA!!!
//Have the Voxel Object store the list of vertices, then the VoxelBatch draw method will just take an array of vertices
//it is easy to edit vertex data because all you have to do is find the voxel's position within the table - it is ordered, after all
//and edit the vertex data
//Then simply memcpy to the buffer and you're all set!

//have the map class extend VoxelObject - after all, it's just a bunch of textured voxels
//perhaps create a proprietary file format to store voxel objects? .vxl

#include "Camera.hpp"
#include "TextureArray.hpp"
#include "Voxel.hpp"
#include "ShaderProgram.hpp"
#include <vector>
#include <memory>


class VoxelObject
{
	public:
		VoxelObject(int x, int y, int z, int width, int height, int depth);
		~VoxelObject();

	private:
		int width, height, depth;

		void place(std::unique_ptr<Voxel>);
		int index3D(int x, int y, int z);
		bool outOfBounds(int x, int y, int z);
		void updateVoxelFaces(int x, int y, int z); //updates block faces at pos x,y,z

		std::vector<std::unique_ptr<Voxel>> voxels; //voxelData replaces this?  I think so... but what about textures...
		std::vector<float> voxelData;
		//no, voxelData is kept in parallel with "voxels", thus allowing texture ID lookups as well as fast vertex data transfer
};

#endif