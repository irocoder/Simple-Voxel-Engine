#ifndef RECTANGULAR_PRISM_HPP
#define RECTANGULAR_PRISM_HPP

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <array>

class RectangularPrism
{
	public:
	RectangularPrism();
	RectangularPrism(float x, float y, float z, float width, float height, float depth, float r, float g, float b, float a);

	float getWidth() const { return width; }
	float getHeight() const { return height; }
	float getDepth() const { return depth; }
	float getR() const { return r; }
	float getG() const { return g; }
	float getB() const { return b; }
	float getA() const { return a; }

	float getCenterX() { return width / 2.0f; }
	float getCenterY() { return -height / 2.0f; }
	float getCenterZ() { return -depth / 2.0f; }

	static const std::array<unsigned short, 36> indices; 

	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void rotate(float x, float y, float z);

	glm::vec3 getPosition() { return glm::vec3(transform[3][0], transform[3][1], transform[3][2]); }

	glm::vec3 getOffset() { return offset; }
	float* getTransform() { return glm::value_ptr(transform); }

	void setColor(glm::vec4& color);
	void setColor(float r, float g, float b, float a);
	void setOffset(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

	void setTransform(glm::mat4& transform);

	void update();

	private:

	glm::mat4 transform;
	glm::vec3 offset;

	float x, y, z, width, height, depth, r, g, b, a;
	float pitch, yaw, roll;
};

#endif