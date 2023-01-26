#include <iostream>
#include <string.h>

#include <Fission/Core/Core.hpp>

#ifdef FISSION_USE_SSE
static constexpr bool s_SSEEnabled = true;
#else
static constexpr bool s_SSEEnabled = false;
#endif

#define PRINT_SSE_USAGE() std::cout << "SSE Enabled: %s" << (s_SSEEnabled ? "true" : "false") << std::endl

// Include All Tests
#include "Vector/Vec3Tests.hpp"
#include "Vector/Vec4Tests.hpp"

const char* TestGroups[] =
{
	"Vector3",
	"Vector4",
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
	PRINT_SSE_USAGE();

	bool AllTestsPassed = true;

	for (auto Group : TestGroups)
		AllTestsPassed &= TestFixture::ExecuteTestGroup(Group, TestFixture::Verbose);

	std::cout << "Result: " << (AllTestsPassed ? "ALL TESTS PASSED" : "TESTS FAILED") << std::endl;
	std::cin.get();
	return AllTestsPassed ? 0 : 1;
}
