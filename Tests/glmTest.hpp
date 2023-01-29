#pragma once

#include "simpletest.h"

#include <glm/glm.hpp>

DEFINE_TEST_G(SIMDOPS, GLM)
{
	glm::mat4 Matrix(1.0f);
	glm::mat4 InverseMatrix = glm::inverse(Matrix);
	TEST(true == true);
}
