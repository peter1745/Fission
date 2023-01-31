#pragma once

#include "TestBase.hpp"

class SpheresStressTest : public TestBase
{
public:
	void DrawUI() override;

	void OnStart() override;
	void OnStop() override;
};
