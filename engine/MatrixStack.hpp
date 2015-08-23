#ifndef MATRIX_STACK_HPP
#define MATRIX_STACK_HPP

#include <stack>
#include <glm/glm.hpp>

class MatrixStack
{
	public:
		glm::mat4& top() { return curMatrix; }
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
		void rotate(float pitch, float yaw, float roll);

		void scale(float x, float y, float z);
		void translate(float x, float y, float z);

		void push();
		void pop();
		void clear();

	private:
		glm::mat4 createRotationX(float angle);
		glm::mat4 createRotationY(float angle);
		glm::mat4 createRotationZ(float angle);
		glm::mat4 createRotation(float pitch, float yaw, float roll);
		glm::mat4 createScale(float x, float y, float z);
		glm::mat4 createTranslation(float x, float y, float z);

		glm::mat4 curMatrix;
		std::stack<glm::mat4> matrices;
};

#endif