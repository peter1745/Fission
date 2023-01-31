#include "Keyboard.hpp"

#include "TestFramework/Core/Application.hpp"

namespace TestFramework {

	Keyboard& Keyboard::Get()
	{
		static Keyboard s_Instance;
		return s_Instance;
	}
		
	void Keyboard::TransitionHeldKeys()
	{
		for (KeyData& data : m_KeyStates)
		{
			if (data.CurrentState == EButtonState::Pressed)
			{
				data.OldState = data.CurrentState;
				data.CurrentState = EButtonState::Held;
			}
		}
	}

	void Keyboard::ResetReleasedKeys()
	{
		for (KeyData& data : m_KeyStates)
		{
			if (data.CurrentState == EButtonState::Released)
			{
				data.OldState = data.CurrentState;
				data.CurrentState = EButtonState::None;
			}
		}
	}

}
