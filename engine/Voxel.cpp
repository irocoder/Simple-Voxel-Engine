#include "Voxel.hpp"

const std::array<unsigned int, 36> Voxel::indices =
{	
	0, 1, 2, 0, 2, 3, //front face
	3, 2, 6, 3, 6, 7, //bottom face
	1, 0, 4, 1, 4, 5, //top face
	5, 4, 7, 5, 7, 6, //back face
	2, 1, 5, 2, 5, 6, //right face
	0, 3, 7, 0, 7, 4  //left face
};

const std::array<unsigned int, 6> Voxel::quadIndices = { 0, 1, 2, 0, 2, 3 };

Voxel::Voxel()
{
	isAirBlock = true;
}	

Voxel::Voxel(Voxel& voxel)
{
	*this = voxel;
}

Voxel::Voxel(float x, float y, float z, float sideLength, float r, float g, float b, float a, float textureIndex)
{
	this->x = x * sideLength;
	this->y = y * sideLength;
	this->z = z * sideLength;
	this->sideLength = sideLength;
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
	this->transparent = false;

	id = 0;
	drawableFaceCount = 6;

	for(int i = 0; i < 6; i++)
	{
		faceTexIndices[i] = textureIndex;
	}
}

Voxel::~Voxel()
{
}

Voxel& Voxel::operator=(const Voxel& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	this->sideLength = rhs.sideLength;
	this->r = rhs.r;
	this->g = rhs.g;
	this->b = rhs.b;
	this->a = rhs.a;
	this->transparent = rhs.transparent;
	this->isAirBlock = rhs.isAirBlock;

	id = rhs.id;
	drawableFaceCount = rhs.drawableFaceCount;

	for (int i = 0; i < 6; i++)
	{
		faceTexIndices[i] = rhs.faceTexIndices[i];
		drawFace[i] = rhs.drawFace[i];
		vertexDataOffsets[i] = rhs.vertexDataOffsets[i];
	}

	return *this;
}

void Voxel::setFaceDrawState(int face, int draw)
{
	if(drawFace[face] != draw)
	{
		switch(draw)
		{
			case VOXEL_DRAW_STATE_FALSE:
			{
				drawableFaceCount--;
			} break;

			case VOXEL_DRAW_STATE_TRUE:
			{
				drawableFaceCount++;
			} break;
		}

		drawFace[face] = draw;
	}
}

const int Voxel::getFaceDrawState(int face) const
{
	return drawFace[face];
}

const float Voxel::getFaceTextureIndex(int face) const
{
	return faceTexIndices[face];
}

void Voxel::setFaceTextureIndex(int face, float texIndex)
{
	faceTexIndices[face] = texIndex;
}

void Voxel::setFaceVertexDataOffset(int face, int offset)
{
	vertexDataOffsets[face] = offset;
}

int Voxel::getFaceVertexDataOffset(int face)
{
	return vertexDataOffsets[face];
}