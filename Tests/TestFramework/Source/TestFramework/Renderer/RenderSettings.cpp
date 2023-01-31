#include "RenderSettings.hpp"

namespace TestFramework {

	RenderSettings& RenderSettings::Get()
	{
		static RenderSettings s_Settings;
		return s_Settings;
	}

}
