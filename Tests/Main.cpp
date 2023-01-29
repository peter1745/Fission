#include <iostream>
#include <string.h>

// Include All Tests
#include "glmTest.hpp"

const char* TestGroups[] =
{
	"GLM",
	"Finale"
};

DEFINE_TEST_G(FinalTest, Finale)
{
	for (auto Group : TestGroups)
	{
		if (strcmp(Group, "Finale") == 0)
			return;

		for (auto test = GetFirstTest(); test; test = test->GetNextTest())
		{
			if (strcmp(test->TestGroup(), Group) == 0)
			{
				TEST_NEQ(test->NumTests(), 0);
			}
		}
	}
}

int main(int ArgC, char** ArgV)
{
	bool AllTestsPassed = true;

	for (auto Group : TestGroups)
		AllTestsPassed &= TestFixture::ExecuteTestGroup(Group, TestFixture::Verbose);

	std::cout << "Result: " << (AllTestsPassed ? "ALL TESTS PASSED" : "TESTS FAILED") << std::endl;
	std::cin.get();
	return AllTestsPassed ? 0 : 1;
}
