#include "Application.hpp"

#include "TestFramework/IO/Keyboard.hpp"
#include "TestFramework/IO/Mouse.hpp"
#include "TestFramework/Events/WindowEvents.hpp"

#include <minmax.h>

//#include <tracy/Tracy.hpp>

namespace TestFramework {

	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& InSpecification, void* InUserData)
	    : m_Specification(InSpecification)
	{
		s_Application = this;

		WindowSpecification windowSpec = {};
		windowSpec.Title = m_Specification.Title;
		windowSpec.Width = m_Specification.WindowWidth;
		windowSpec.Height = m_Specification.WindowHeight;
		windowSpec.UserData = InUserData;
		m_Window = MakeUnique<Window>(windowSpec);
		m_Window->Init();
		m_Window->SetEventCallback(FUSION_BIND_FUNC(Application::EventCallback));

		m_Context = std::make_unique<GraphicsContext>();

		SwapChainInfo swapChainCreateInfo;
		swapChainCreateInfo.Width = m_Window->GetWidth();
		swapChainCreateInfo.Height = m_Window->GetHeight();
		swapChainCreateInfo.HasDepthBuffer = false;
		m_SwapChain = std::make_unique<SwapChain>(m_Context.get(), swapChainCreateInfo);

		m_Window->Maximize();
	}

	Application::~Application()
	{
		m_Window = nullptr;
	}

	void Application::Run()
	{
		m_Running = true;
		OnInit();
		while (m_Running)
		{
			//FrameMarkStart("MainThread");

			Keyboard::Get().TransitionHeldKeys();
			Mouse::Get().TransitionHeldButtons();
			m_Window->ProcessEvents();

			ExecuteMainThreadQueue();

			OnUpdate(m_TimeStep);

			TimePoint time = std::chrono::high_resolution_clock::now();
			m_FrameTime = std::chrono::duration_cast<std::chrono::duration<float>>(time - m_LastFrameTime).count();
			m_TimeStep = min(m_FrameTime, 0.0333f);
			m_LastFrameTime = time;

			Mouse::Get().ResetReleasedButtons();
			Keyboard::Get().ResetReleasedKeys();

			// End of frame
			//FrameMarkNamed("MainThread");

			// TODO(Peter): Handle with event
			m_Running = !m_Window->ShouldClose();
		}

		OnShutdown();

		m_SwapChain->Release();
		m_Context->GetDevice()->Wait();
		m_Context->Release();
	}

	Application& Application::Get() { return *s_Application; }

	void Application::EventCallback(Event& InEvent)
	{
		EventDispatcher dispatcher(InEvent);
		dispatcher.Dispatch<WindowCloseEvent>([this]([[maybe_unused]] WindowCloseEvent& InCloseEvent)
		{
			m_Running = false;
			return true;
		});

		OnEvent(InEvent);
	}

	void Application::ExecuteMainThreadQueue()
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		for (auto& func : m_MainThreadQueue)
			func();

		m_MainThreadQueue.clear();
	}

}
