#pragma once

#include "Core.hpp"
#include "TestFramework/Events/Event.hpp"

#include <string>
#include <functional>

struct GLFWwindow;

namespace TestFramework {

	struct WindowSpecification
	{
		std::string Title = "";
		uint32_t Width = 0;
		uint32_t Height = 0;
		void* UserData = nullptr;
	};

	using EventCallbackFunc = std::function<void(Event&)>;
	struct WindowData
	{
		WindowSpecification Specification;
		EventCallbackFunc EventCallback = nullptr;
	};

	class Window
	{
	public:
		Window(const WindowSpecification& InSpecification);
		~Window();

		void SetTitle(const std::string& InTitle);

		uint32_t GetWidth() const { return m_WindowData.Specification.Width; }
		uint32_t GetHeight() const { return m_WindowData.Specification.Height; }

		void SetEventCallback(const EventCallbackFunc& InCallbackFunc)
		{
			m_WindowData.EventCallback = InCallbackFunc;
		}

		void* GetWindowHandle() const { return static_cast<void*>(m_NativeWindow); }
		bool ShouldClose() const;

		void Maximize();

	private:
		void Init();
		void ProcessEvents();

	private:
		WindowData m_WindowData;
		GLFWwindow* m_NativeWindow = nullptr;

		friend class Application;
	};

}
