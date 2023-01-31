#include "Window.hpp"
#include "TestFramework/IO/Mouse.hpp"
#include "TestFramework/IO/Keyboard.hpp"
#include "TestFramework/IO/GLFWKeyMappings.hpp"

#include "TestFramework/Events/WindowEvents.hpp"
#include "TestFramework/Events/KeyboardEvents.hpp"

#include <GLFW/glfw3.h>

namespace TestFramework {

	Window::Window(const WindowSpecification& InSpecification)
	{
		m_WindowData.Specification = InSpecification;
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_NativeWindow);
		glfwTerminate();
	}
	
	void Window::SetTitle(const std::string& InTitle)
	{
		glfwSetWindowTitle(m_NativeWindow, InTitle.c_str());
		m_WindowData.Specification.Title = InTitle;
	}

	void Window::Init()
	{
		glfwInit();

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_NativeWindow = glfwCreateWindow(int(m_WindowData.Specification.Width), int(m_WindowData.Specification.Height), m_WindowData.Specification.Title.c_str(), NULL, NULL);

		glfwSetWindowUserPointer(m_NativeWindow, &m_WindowData);

		glfwSetWindowSizeCallback(m_NativeWindow, [](GLFWwindow* InNativeWindow, int32_t InWidth, int32_t InHeight)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(InNativeWindow));
			data->Specification.Width = InWidth;
			data->Specification.Height = InHeight;

			if (data->EventCallback)
			{
				WindowResizeEvent resizeEvent(InWidth, InHeight);
				data->EventCallback(resizeEvent);
			}
		});

		glfwSetCursorPosCallback(m_NativeWindow, []([[maybe_unused]] GLFWwindow* InNativeWindow, double InPosX, double InPosY)
		{
			Mouse::Get().m_Position = { static_cast<float>(InPosX), static_cast<float>(InPosY) };
		});

		glfwSetMouseButtonCallback(m_NativeWindow, []([[maybe_unused]] GLFWwindow* InNativeWindow, int InButton, int InAction, [[maybe_unused]] int InMods)
		{
			MouseButtonState& buttonState = Mouse::Get().m_ButtonStates[InButton];
			buttonState.OldState = buttonState.CurrentState;
			
			switch (InAction)
			{
			case GLFW_PRESS:
			{
				buttonState.CurrentState = EButtonState::Pressed;
				break;
			}
			case GLFW_RELEASE:
			{
				buttonState.CurrentState = EButtonState::Released;
				break;
			}
			}
		});

		glfwSetKeyCallback(m_NativeWindow, [](GLFWwindow* InNativeWindow, int InKey, [[maybe_unused]] int InScanCode, int InAction, [[maybe_unused]] int InMods)
		{
			if (InKey == -1)
				return;

			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(InNativeWindow));

			KeyData& keyInfo = Keyboard::Get().GetKeyData(GLFWKeyMappings.at(InKey));
			keyInfo.OldState = keyInfo.CurrentState;

			switch (InAction)
			{
			case GLFW_PRESS:
			{
				keyInfo.CurrentState = EButtonState::Pressed;

				KeyPressedEvent keyEvent(GLFWKeyMappings.at(InKey));
				data->EventCallback(keyEvent);
				break;
			}
			case GLFW_REPEAT:
			{
				// NOTE(Peter): This isn't *technically* necessary, since we manually transition pressed keys to the held state
				keyInfo.CurrentState = EButtonState::Held;
				break;
			}
			case GLFW_RELEASE:
			{
				keyInfo.CurrentState = EButtonState::Released;

				KeyReleasedEvent keyEvent(GLFWKeyMappings.at(InKey));
				data->EventCallback(keyEvent);
				break;
			}
			}
		});
	}

	void Window::ProcessEvents()
	{
		EMouseVisibility currentVisibility = static_cast<EMouseVisibility>(glfwGetInputMode(m_NativeWindow, GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
		if (currentVisibility != Mouse::Get().GetVisibility())
			glfwSetInputMode(m_NativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL + static_cast<int32_t>(Mouse::Get().GetVisibility()));

		glfwPollEvents();
	}

	bool Window::ShouldClose() const { return glfwWindowShouldClose(m_NativeWindow); }

	void Window::Maximize()
	{
		glfwMaximizeWindow(m_NativeWindow);
	}

}
