#ifndef MOB_HPP
#define MOB_HPP

#include "engine/MatrixStack.hpp"
#include "engine/RectangularPrism.hpp"
#include "engine/VoxelBatch.hpp"

//create with smart pointer
class Mob
{
	public:
		Mob(float x, float y, float z, VoxelBatch* primitiveBatch);

		void draw();
		float x, y, z;

	private:
		RectangularPrism body;
		RectangularPrism upperLeftArm;
		RectangularPrism upperRightArm;
		RectangularPrism leftLeg;
		RectangularPrism rightLeg;
		RectangularPrism head;
		//RectangularPrism lowerLeftArm;

		MatrixStack matrices;
		VoxelBatch* primitiveBatch;
		float angle = 0;

};


#endif