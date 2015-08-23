#ifndef BLOCK_BATCH_HPP
#define BLOCK_BATCH_HPP

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <stack>
#include "RectangularPrism.hpp"
#include "Voxel.hpp"

#define FACE_DATA_SIZE 44
#define VERTEX_DATA_SIZE 11
#define VERTEX_DATA_OFFSET (faceCount * 4 * VERTEX_DATA_SIZE)
#define ELEMENT_DATA_OFFSET (faceCount * 6)
#define ELEMENT_DATA_ADD_VALUE (faceCount * 4)

class VoxelBatch
{
	public:
		//numFaces = number of faces you want to allocate space for
		VoxelBatch(GLint shaderProgram, int numFaces);

		void drawVoxels();

		void addVoxelFace(Voxel* voxel, int face);
		void removeVoxelFace(Voxel* voxel, int face); //removes a face from a voxel
		void removeVoxel(Voxel* voxel); //removes a voxel from vertex data. //change to std::vector<Voxel*> or something
		void addVoxel(Voxel* voxel); //adds a voxel to the vertex data  //are the add/remove functions really needed?

		void open(); //allow vertex data insertions
		void close(); //disallow vertex data insertions
		float* getVertexBuffer(); //returns a pointer to the vertex buffer if 'open' is true
		float* getElementBuffer(); //returns a pointer to the element buffer if 'open' is true

	private:
		//new code
		int vertexDataSize;
		int elementDataSize;
		int* elementBuffer;
		//end new code

		bool isOpen = false;
		bool uploaded = false;

		GLint program; //shaderprogram

		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		GLuint vao;
		GLuint vbo; //vertex buffer object
		GLuint ebo; //element buffer object

		void addVertex(float x, float y, float z, float r, float g, float b, float a, float u, float v, float texIndex);
		void newAddVertex(float* buf, float x, float y, float z, float r, float g, float b, float a, float u, float v, float texIndex);

		void addQuadIndices(int* eBuf, int add); //'add' = value to add to the indices

		int faceCount;
		float* vertexBuffer; //only accessible if the voxelBatch is "open"

		std::vector<Voxel*> voxelLocations;
		std::vector<int> voxelFaceLocations;
		void addFaceVertexData(Voxel* voxel, int face);
};

#endif