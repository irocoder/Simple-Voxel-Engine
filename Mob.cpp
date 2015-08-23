#include "Mob.hpp"
#include <iostream>
#include <glfw3.h>

Mob::Mob(float x, float y, float z, VoxelBatch* primitiveBatch)
:body(x, y, z, 1.0f, 1.7f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
 upperLeftArm(x + 1.05f, y, z, 0.35f, 1.2f, 0.35f, 1.0f, 0.0f, 0.0f, 1.0f),
 upperRightArm(x + 1.05f, y, z, 0.35f, 1.2f, 0.35f, 1.0f, 0.0f, 0.0f, 1.0f),
 leftLeg(x, y, z, 0.5f, 1.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
 rightLeg(x, y, z, 0.5f, 1.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
 head(x, y, z, 0.65f, 0.6f, 0.55f, 0.0f, 1.0f, 0.0f, 1.0f)
{
	this->primitiveBatch = primitiveBatch;
	this->x = x;
	this->y = y;
	this->z = z;
}

void Mob::draw()
{
	//in matrix stack, perform all matrix operations in the inverse order as one would on the GPU
	//always perform rotations in XYZ order
	angle += 0.1;

	//body matrix
	matrices.translate(x, y, z);

	//matrices.rotateY(angle);
	matrices.push(); //push onto stack

	body.setOffset(0.5f, -0.85f, -0.25f);
	body.setTransform(matrices.top());

	//this->primitiveBatch->drawRectangularPrism(body);
		
	// ----------------- LEFT ARM ------------------- //
	matrices.translate(0.55f, 0.65f, 0.1f);
	matrices.rotate(-angle * 4, 0.f, 0.f);
	matrices.push(); //push left arm matrix onto stack

	//upper left arm dim: w: 0.25, h: 0.8, d: 0.25
	upperLeftArm.setOffset(upperLeftArm.getCenterX(), -0.2f, upperLeftArm.getCenterZ());
	upperLeftArm.setTransform(matrices.top());

	//this->primitiveBatch->drawRectangularPrism(upperLeftArm);

	matrices.pop(); //current matrix is now left arm matrix


	// ----------------- RIGHT ARM ------------------- //
	matrices.translate(-0.55f, 0.65f, 0.1f);
	matrices.rotateX(-30.f);
	matrices.push(); //push right arm matrix onto stack

	//upper left arm dim: w: 0.25, h: 0.8, d: 0.25
	upperRightArm.setOffset(upperRightArm.getCenterX(), -0.2f, upperRightArm.getCenterZ());
	upperRightArm.setTransform(matrices.top());

	//this->primitiveBatch->drawRectangularPrism(upperRightArm);
	matrices.pop();


	// ----------------- LEFT LEG ------------------- //
	matrices.translate(0.25f, -1.0f, 0.0f);
	matrices.rotateX(std::sin(glfwGetTime() * 6) * 30);
	matrices.push();

	leftLeg.setOffset(leftLeg.getCenterX(), -0.2f, leftLeg.getCenterZ());
	leftLeg.setTransform(matrices.top());
	//primitiveBatch->drawRectangularPrism(leftLeg);
	matrices.pop();

	// ----------------- RIGHT LEG ------------------- //
	matrices.translate(-0.25f, -1.0f, 0.0f);
	matrices.rotateX(std::cos(glfwGetTime() * 6) * 30);
	matrices.push();

	rightLeg.setOffset(rightLeg.getCenterX(), -0.2f, rightLeg.getCenterZ());
	rightLeg.setTransform(matrices.top());
	//primitiveBatch->drawRectangularPrism(rightLeg);
	matrices.pop();

	// ----------------- HEAD ------------------- //
	matrices.translate(0.0f, 1.2f, 0.0f);
	matrices.push();

	head.setOffset(head.getCenterX(), -0.2f, head.getCenterZ());
	head.setTransform(matrices.top());
	//primitiveBatch->drawRectangularPrism(head);
	matrices.pop();

	matrices.clear();
}