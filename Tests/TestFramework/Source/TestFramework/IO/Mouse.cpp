#include "Mouse.hpp"

namespace TestFramework {

	Mouse& Mouse::Get()
	{
		static Mouse s_Instance;
		return s_Instance;
	}

	void Mouse::TransitionHeldButtons()
	{
		for (MouseButtonState& data : m_ButtonStates)
		{
			if (data.CurrentState == EButtonState::Pressed)
			{
				data.OldState = data.CurrentState;
				data.CurrentState = EButtonState::Held;
			}
		}
	}

	void Mouse::ResetReleasedButtons()
	{
		for (MouseButtonState& data : m_ButtonStates)
		{
			if (data.CurrentState == EButtonState::Released)
			{
				data.OldState = data.CurrentState;
				data.CurrentState = EButtonState::None;
			}
		}
	}

}
