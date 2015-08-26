#include "VoxelBatch.hpp"
#include <iostream>
#include <iterator>
#include <chrono>

VoxelBatch::VoxelBatch(GLint shaderProgram, int numFaces)
{
	std::cout << "PRIMITIVE_BATCH!" << std::endl;	

	//vertices.reserve(10000);
	//indices.reserve(10000);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//stride = number of elements in vertex data
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(4 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	faceCount = 0;

	program = shaderProgram;

	//NEW CODE

	//map is 100x20x100 (x * y * z), so we will allocate that much space
	//that makes... 200,000 cubes, * 24 vertices for each cube... * 11 floats for each vertex
	//so... 52,800,000 * sizeof(float) - 4 bytes for each float = 211 MB - holy jesus.  I need to add in an "air chunk" or something like that

	//indices = 6 indices per face * 6 faces * 200000 cubes = 7,200,000 ints

	//allocate space
	//vertexDataSize = 52800000;
	//elementDataSize = 7200000;

	vertexDataSize = numFaces * FACE_DATA_SIZE;
	elementDataSize = numFaces * 6;

	glBufferData(GL_ARRAY_BUFFER, vertexDataSize * sizeof(float), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementDataSize * sizeof(float), nullptr, GL_STATIC_DRAW);
}

void VoxelBatch::open()
{
	isOpen = true;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	vertexBuffer = (float*)glMapBufferRange(GL_ARRAY_BUFFER, 0, vertexDataSize * sizeof(float), GL_MAP_WRITE_BIT);
	elementBuffer = (int*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, elementDataSize * sizeof(int), GL_MAP_WRITE_BIT);
}

void VoxelBatch::close()
{
	isOpen = false;
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	vertexBuffer = nullptr;
	elementBuffer = nullptr;

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void VoxelBatch::drawVoxels()
{
	glClearColor(0.191f, 0.300f, 0.473f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glDrawElements(GL_TRIANGLES, faceCount * 6, GL_UNSIGNED_INT, nullptr);	
}

void VoxelBatch::addFaceVertexData(Voxel* voxel, int face)
{
	if(isOpen && !voxel->getTransparent())
	{
		float x = voxel->getX();
		float y = voxel->getY();
		float z = voxel->getZ();

		float sideLength = voxel->getSideLength();

		float r = voxel->getR();
		float g = voxel->getG();
		float b = voxel->getB();
		float a = voxel->getA();

		switch(face)
		{
			case VOXEL_FACE_FRONT:
			{					
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET], x, y, z, r, g, b, a, 0.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_FRONT));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + VERTEX_DATA_SIZE], x, y - sideLength, z, r, g, b, a, 0.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_FRONT));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 2)], x + sideLength, y - sideLength, z, r, g, b, a, 1.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_FRONT));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 3)], x + sideLength, y, z, r, g, b, a, 1.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_FRONT));

				addQuadIndices(&elementBuffer[ELEMENT_DATA_OFFSET], ELEMENT_DATA_ADD_VALUE);

				voxel->setFaceVertexDataOffset(VOXEL_FACE_FRONT, VERTEX_DATA_OFFSET);
				faceCount++;

			} break;

			case VOXEL_FACE_BACK:
			{
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET], x + sideLength, y, z - sideLength, r, g, b, a, 0.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_BACK));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + VERTEX_DATA_SIZE], x + sideLength, y - sideLength, z - sideLength, r, g, b, a, 0.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_BACK));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 2)], x, y - sideLength, z - sideLength, r, g, b, a, 1.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_BACK));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 3)], x, y, z - sideLength, r, g, b, a, 1.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_BACK));
				
				addQuadIndices(&elementBuffer[ELEMENT_DATA_OFFSET], ELEMENT_DATA_ADD_VALUE);

				voxel->setFaceVertexDataOffset(VOXEL_FACE_BACK, VERTEX_DATA_OFFSET);
				faceCount++;

			} break;

			case VOXEL_FACE_LEFT:
			{
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET], x, y, z - sideLength, r, g, b, a, 0.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_LEFT));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + VERTEX_DATA_SIZE], x, y - sideLength, z - sideLength, r, g, b, a, 0.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_LEFT));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 2)], x, y - sideLength, z, r, g, b, a, 1.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_LEFT));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 3)], x, y, z, r, g, b, a, 1.0, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_LEFT));

				addQuadIndices(&elementBuffer[ELEMENT_DATA_OFFSET], ELEMENT_DATA_ADD_VALUE);

				voxel->setFaceVertexDataOffset(VOXEL_FACE_LEFT, VERTEX_DATA_OFFSET);
				faceCount++;

			} break;

			case VOXEL_FACE_RIGHT:
			{
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET], x + sideLength, y, z, r, g, b, a, 0.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_RIGHT));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + VERTEX_DATA_SIZE], x + sideLength, y - sideLength, z, r, g, b, a, 0.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_RIGHT));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 2)], x + sideLength, y - sideLength, z - sideLength, r, g, b, a, 1.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_RIGHT));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 3)], x + sideLength, y, z - sideLength, r, g, b, a, 1.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_RIGHT));

				addQuadIndices(&elementBuffer[ELEMENT_DATA_OFFSET], ELEMENT_DATA_ADD_VALUE);

				voxel->setFaceVertexDataOffset(VOXEL_FACE_RIGHT, VERTEX_DATA_OFFSET);
				faceCount++;

			} break;

			case VOXEL_FACE_TOP:
			{
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET], x + sideLength, y, z, r, g, b, a, 0.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_TOP));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + VERTEX_DATA_SIZE], x + sideLength, y, z - sideLength, r, g, b, a, 0.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_TOP));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 2)], x, y, z - sideLength, r, g, b, a, 1.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_TOP));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 3)], x, y, z, r, g, b, a, 1.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_TOP));

				addQuadIndices(&elementBuffer[ELEMENT_DATA_OFFSET], ELEMENT_DATA_ADD_VALUE);

				voxel->setFaceVertexDataOffset(VOXEL_FACE_TOP, VERTEX_DATA_OFFSET);
				faceCount++;
			
			} break;

			case VOXEL_FACE_BOTTOM:
			{
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET], x, y - sideLength, z, r, g, b, a, 0.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_BOTTOM));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + VERTEX_DATA_SIZE], x, y - sideLength, z - sideLength, r, g, b, a, 0.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_BOTTOM));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 2)], x + sideLength, y - sideLength, z - sideLength, r, g, b, a, 1.0f, 1.0f, voxel->getFaceTextureIndex(VOXEL_FACE_BOTTOM));
				newAddVertex(&vertexBuffer[VERTEX_DATA_OFFSET + (VERTEX_DATA_SIZE * 3)], x + sideLength, y - sideLength, z, r, g, b, a, 1.0f, 0.0f, voxel->getFaceTextureIndex(VOXEL_FACE_BOTTOM));

				addQuadIndices(&elementBuffer[ELEMENT_DATA_OFFSET], ELEMENT_DATA_ADD_VALUE);

				voxel->setFaceVertexDataOffset(VOXEL_FACE_BOTTOM, VERTEX_DATA_OFFSET);
				faceCount++;

			} break;
		}
	}
	else
	{
		if(!isOpen)
			std::cout << "Cannot add vertex data while batch is closed." << "\n";
	}
}

void VoxelBatch::addVertex(float x, float y, float z, float r, float g, float b, float a, float u, float v, float texIndex)
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
	vertices.push_back(1.0f); //w

	vertices.push_back(r);
	vertices.push_back(g);
	vertices.push_back(b);
	vertices.push_back(a);

	vertices.push_back(u);
	vertices.push_back(v);

	vertices.push_back(texIndex); //texture index
}

void VoxelBatch::newAddVertex(float* buf, float x, float y, float z, float r, float g, float b, float a, float u, float v, float texIndex)
{
	buf[0] = x;		buf[1] = y;		buf[2] = z;		buf[3] = 1.0f; //w
	buf[4] = r;		buf[5] = g;		buf[6] = b;		buf[7] = a;
	buf[8] = u;		buf[9] = v;		buf[10] = texIndex;
}

void VoxelBatch::addQuadIndices(int* eBuf, int add)
{
	for (int i = 0; i < 6; i++)
	{
		eBuf[i] = add + Voxel::quadIndices[i];
	}
}

float* VoxelBatch::getVertexBuffer()
{
	if(isOpen)
		return vertexBuffer;
	else
	{
		//add an exception
		return nullptr;
	}
}

void VoxelBatch::addVoxel(Voxel* voxel)
{
	if(isOpen)
	{	
		for (int k = 0; k < 6; k++)
		{
			addVoxelFace(voxel, k);
		}

	}
	else
		std::cout << "Voxelbatch must be open to add voxels." << std::endl;
}

void VoxelBatch::removeVoxel(Voxel* voxel)
{
	if(isOpen)
	{

		for (int k = 0; k < 6; k++) //for each face in the voxel
		{
			removeVoxelFace(voxel, k);
		}
	}
	else
		std::cout << "VOXEL BATCH MUST BE OPEN TO REMOVE VOXELS." << std::endl;
}

void VoxelBatch::addVoxelFace(Voxel* voxel, int face)
{
	if(isOpen && voxel->getFaceDrawState(face) == VOXEL_DRAW_STATE_FALSE)
	{
		voxelLocations.push_back(voxel);
		voxelFaceLocations.push_back(face);

		addFaceVertexData(voxel, face); //increments faceCount and sets the vertex data offset
		voxel->setFaceDrawState(face, VOXEL_DRAW_STATE_TRUE);
	}
}

void VoxelBatch::removeVoxelFace(Voxel* voxel, int face)
{
	if (isOpen)
	{
		if (voxel->getFaceDrawState(face) == VOXEL_DRAW_STATE_TRUE)
		{
			if (voxel->getFaceVertexDataOffset(face) != VOXEL_NO_DATA_OFFSET)
			{
				int offset = voxel->getFaceVertexDataOffset(face);
				int index = faceCount - 1;

				//replace the vertex data
				for (int i = offset; i < offset + FACE_DATA_SIZE; i++)
				{
					vertexBuffer[i] = vertexBuffer[VERTEX_DATA_OFFSET - FACE_DATA_SIZE + (i - offset)];
				}

				//set the new vertex data offsets of the last element in voxelLocations and voxelFaceLocations, 
				//to prepare it for the switch to the removed voxel face location.
				voxelLocations[index]->setFaceVertexDataOffset(voxelFaceLocations[index], voxel->getFaceVertexDataOffset(face));
				voxelLocations[voxel->getFaceVertexDataOffset(face) / FACE_DATA_SIZE] = voxelLocations[index];
				
				voxelFaceLocations[voxel->getFaceVertexDataOffset(face) / FACE_DATA_SIZE] = voxelFaceLocations[index];

				voxelLocations.pop_back();
				voxelFaceLocations.pop_back();

				voxel->setFaceVertexDataOffset(face, VOXEL_NO_DATA_OFFSET);
				voxel->setFaceDrawState(face, VOXEL_DRAW_STATE_FALSE);

				faceCount--;
			}
		}

	}
	else
		std::cout << "Cannot remove voxel faces in a closed VoxelBatch!" << std::endl;
}