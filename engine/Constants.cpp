#include "Constants.hpp"

void printMat(glm::mat4& m)
{
	std::cout << "MATRIX: " << std::endl;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			std::cout << "[" << m[x][y] << "]";
		}

		std::cout << std::endl;
	}
}