#ifndef MAP_HPP
#define MAP_HPP

#include "engine/Camera.hpp"
#include "engine/TextureArray.hpp"
#include "engine/VoxelBatch.hpp"
#include "engine/ShaderProgram.hpp"
#include <vector>
#include <memory>

class Map
{
	public:
		Map(int width, int height, int depth);
		~Map();
		void draw(Camera* camera);

		GLuint getShaderProgram() const { return shaderProgram->get(); }

	private:
		void place(Voxel& voxel);
		void remove(int x, int y, int z); //removes the voxel at (x, y, z)
		int index3D(int x, int y, int z);
		bool outOfBounds(int x, int y, int z);
		void updateVoxelFaces(int x, int y, int z); //updates block faces at pos x,y,z

		void genStoneHouse(int x, int y, int z);

		int width, height, depth;

		std::unique_ptr<VoxelBatch> voxelBatch;
		std::unique_ptr<ShaderProgram> shaderProgram;
		std::unique_ptr<Texture2D> texture;
		std::unique_ptr<TextureArray> textureArray;

		std::vector<Voxel> table;
};


#endif