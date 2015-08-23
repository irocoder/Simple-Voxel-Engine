#ifndef VOXEL_HPP
#define VOXEL_HPP

#include "Texture2D.hpp"
#include <GL/glew.h>
#include <array>

static const float BLOCK_USE_VERTEX_COLOR = -1.0f;
static const int VOXEL_FACE_FRONT = 0;
static const int VOXEL_FACE_BACK = 1;
static const int VOXEL_FACE_TOP = 2;
static const int VOXEL_FACE_BOTTOM = 3;
static const int VOXEL_FACE_RIGHT = 4;
static const int VOXEL_FACE_LEFT = 5;

static const int VOXEL_DRAW_STATE_FALSE = 0;
static const int VOXEL_DRAW_STATE_TRUE = 1;

static const int VOXEL_NO_DATA_OFFSET = -1;

class Voxel
{
	public:
		Voxel();
		Voxel(Voxel& voxel);
		Voxel(float x, float y, float z, float sideLength, float r, float g, float b, float a, float textureIndex); //x, y, z in map coordinates
		~Voxel();

		Voxel& operator=(const Voxel& rhs);

		const float getX() const { return x; }
		const float getY() const { return y; }
		const float getZ() const { return z; }

		const float getMapX() const { return x / sideLength; }
		const float getMapY() const { return y / sideLength; }
		const float getMapZ() const { return z / sideLength; }

		const float getSideLength() { return sideLength; }

		const float getR() const { return r; }
		const float getG() const { return g; }
		const float getB() const { return b; }
		const float getA() const { return a; }

		static const std::array<unsigned int, 36> indices;
		static const std::array<unsigned int, 6> quadIndices;	

		void setID(int id) { this->id = id; }
		const int getID() const { return id; }
		void setFaceDrawState(int face, int draw);
		const int getFaceDrawState(int face) const;
		void setFaceTextureIndex(int face, float texIndex);
		const float getFaceTextureIndex(int face) const;
		void setFaceVertexDataOffset(int face, int offset);
		int getFaceVertexDataOffset(int face);
		const bool isAir() const { return isAirBlock; }

		const bool getTransparent() const { return transparent; }

		std::array<int, 6> drawFace = {{ 0, 0, 0, 0, 0, 0 }}; //whether or not the face is drawn
		std::array<float, 6> faceTexIndices = {{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }}; //textureIndex in the texture array of the face
		std::array<int, 6> vertexDataOffsets = {{-1, -1, -1, -1, -1, -1 }}; //offset of each face's vertex data within each map(VoxelObject)'s vector of vertex data
		//intialized as VOXEL_NO_DATA_OFFSET

		int drawableFaceCount;

	protected:
		float x, y, z; //draw coordinates
		float r, g, b, a;
		float sideLength;
		bool transparent;
		bool isAirBlock = false;
		int id; //the id of the voxel, should be its index in the VoxelBatch's vector of voxels
};


#endif