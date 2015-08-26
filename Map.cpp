#include "Map.hpp"
#include "voxel_types/StoneVoxel.hpp"
#include "voxel_types/GrassVoxel.hpp"
#include "voxel_types/AirVoxel.hpp"
#include <iostream>

Map::Map(int width, int height, int depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;

	shaderProgram = std::make_unique<ShaderProgram>("source/map_shader/vertex.glsl", "source/map_shader/fragment.glsl");
	shaderProgram->bind();
	voxelBatch = std::make_unique<VoxelBatch>(shaderProgram->get(), 500000);
	
	//INIT TEXTURE HERE
	textureArray = std::make_unique<TextureArray>("source/resource/new_tiles.png", 16, 16);
	//textureArray->bind();

	table.resize(width * height * depth);

	voxelBatch->open();

	for(int z = 0; z < depth; z++)
	{
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				int r = std::rand();

				if(r % 7 == 0)
					place(GrassVoxel(x, y, z));
				else
					place(AirVoxel(x, y, z));
			}
		}
	}

	genStoneHouse(5, 1, 5);
	place(StoneVoxel(5, 0, 5));
	
	voxelBatch->close();
}

Map::~Map()
{
	std::cout << "DESTRUCTING MAP!" << std::endl;
	table.clear();
	std::cout << "MAP DESTRUCTION DONE!" << std::endl;
}

void Map::draw(Camera* camera)
{
	shaderProgram->bind();
	textureArray->bind();

	camera->update();
	voxelBatch->drawVoxels();
	textureArray->unbind();

	shaderProgram->unbind();
}

void Map::place(Voxel& block)
{
	table[index3D(block.getMapX(), block.getMapY(), block.getMapZ())] = block;

	Voxel* b = &table[index3D(block.getMapX(), block.getMapY(), block.getMapZ())];
	int x = b->getMapX();
	int y = b->getMapY();
	int z = b->getMapZ();

	if(&table[index3D(x, y, z)] != nullptr)
	{
		voxelBatch->removeVoxel(&table[index3D(x, y, z)]);
	}

	if (!b->isAir())
	{
		voxelBatch->addVoxel(b);

		if (!outOfBounds(x + 1, y, z) && !table[index3D(x + 1, y, z)].isAir())
		{
			voxelBatch->removeVoxelFace(b, VOXEL_FACE_RIGHT);
			voxelBatch->removeVoxelFace(&table[index3D(x + 1, y, z)], VOXEL_FACE_LEFT);
		}

		if (!outOfBounds(x - 1, y, z) && !table[index3D(x - 1, y, z)].isAir())
		{
			voxelBatch->removeVoxelFace(b, VOXEL_FACE_LEFT);
			voxelBatch->removeVoxelFace(&table[index3D(x - 1, y, z)], VOXEL_FACE_RIGHT);
		}

		if (!outOfBounds(x, y + 1, z) && !table[index3D(x, y + 1, z)].isAir())
		{
			voxelBatch->removeVoxelFace(b, VOXEL_FACE_TOP);
			voxelBatch->removeVoxelFace(&table[index3D(x, y + 1, z)], VOXEL_FACE_BOTTOM);
		}

		if (!outOfBounds(x, y - 1, z) && !table[index3D(x, y - 1, z)].isAir())
		{
			voxelBatch->removeVoxelFace(b, VOXEL_FACE_BOTTOM);
			voxelBatch->removeVoxelFace(&table[index3D(x, y - 1, z)], VOXEL_FACE_TOP);
		}

		if (!outOfBounds(x, y, z + 1) && !table[index3D(x, y, z + 1)].isAir())
		{
			voxelBatch->removeVoxelFace(b, VOXEL_FACE_FRONT);
			voxelBatch->removeVoxelFace(&table[index3D(x, y, z + 1)], VOXEL_FACE_BACK);
		}

		if (!outOfBounds(x, y, z - 1) && !table[index3D(x, y, z - 1)].isAir())
		{
			voxelBatch->removeVoxelFace(b, VOXEL_FACE_BACK);
			voxelBatch->removeVoxelFace(&table[index3D(x, y, z - 1)], VOXEL_FACE_FRONT);
		}
	}
}

void Map::remove(int x, int y, int z)
{
	voxelBatch->removeVoxel(&table[index3D(x, y, z)]);

	if (!outOfBounds(x + 1, y, z) && !table[index3D(x + 1, y, z)].isAir())
		voxelBatch->addVoxelFace(&table[index3D(x + 1, y, z)], VOXEL_FACE_LEFT);
	
	if (!outOfBounds(x - 1, y, z) && !table[index3D(x - 1, y, z)].isAir())
		voxelBatch->addVoxelFace(&table[index3D(x - 1, y, z)], VOXEL_FACE_RIGHT);

	if (!outOfBounds(x, y + 1, z) && !table[index3D(x, y + 1, z)].isAir())
		voxelBatch->addVoxelFace(&table[index3D(x, y + 1, z)], VOXEL_FACE_BOTTOM);

	if (!outOfBounds(x, y - 1, z) && !table[index3D(x, y - 1, z)].isAir())
		voxelBatch->addVoxelFace(&table[index3D(x, y - 1, z)], VOXEL_FACE_TOP);

	if (!outOfBounds(x, y, z + 1) && !table[index3D(x, y, z + 1)].isAir())
		voxelBatch->addVoxelFace(&table[index3D(x, y, z + 1)], VOXEL_FACE_BACK);

	if (!outOfBounds(x, y, z - 1) && !table[index3D(x, y, z - 1)].isAir())
		voxelBatch->addVoxelFace(&table[index3D(x, y, z - 1)], VOXEL_FACE_FRONT);

	place(AirVoxel(x, y, z));
}

int Map::index3D(int x, int y, int z)
{
	return (y * width) + (z * width * height) + x;
}

bool Map::outOfBounds(int x, int y, int z)
{
	if (x < 0 || x > width - 1)
		return true;

	if (y < 0 || y > height - 1)
		return true;

	if (z < 0 || z > depth - 1)
		return true;

	return false;
}

void Map::genStoneHouse(int x, int y, int z)
{
	int houseWidth = 7;
	int houseDepth = 7;
	int houseHeight = 10;

	for(int m = 0; m < houseHeight; m += houseHeight - 1) //height
	{
		for(int j = 0; j < houseWidth; j++) //width
		{
			for(int k = 0; k < houseDepth; k++) //depth
			{
				place(StoneVoxel(x + j, y + m, z + k));
			}
		}
	}

	for(int m = 0; m < houseHeight; m++)
	{
		for(int j = 0; j < houseWidth; j += houseWidth - 1)
		{
			for(int k = 0 ; k < houseDepth; k += houseDepth - 1)
			{
				place(StoneVoxel(x + j, y + m, z + k));
			}
		} 
	}
}
