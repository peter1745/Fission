#pragma once

#include <TestFramework/Core/Application.hpp>
#include <TestFramework/Renderer/Renderer.hpp>
#include <TestFramework/Renderer/Mesh.hpp>
#include <TestFramework/Renderer/GraphicsPipeline.hpp>

#include "UI/ImGuiPlatformContext.hpp"

#include "Renderer/ViewportCamera.hpp"

#include "Tests/TestBase.hpp"

namespace FissionTests {

	using namespace TestFramework;

	class TestsApp : public Application
	{
	public:
		TestsApp(const ApplicationSpecification& specification);

		virtual void OnInit() override;
		virtual void OnUpdate(float DeltaTime) override;
		virtual void OnShutdown() override;

	private:
		virtual void OnEvent([[maybe_unused]] Event& InEvent) override;

	private:
		void RegisterTests();

		template<typename TTest>
		void RegisterTest(const std::string& InName)
		{
			auto test = std::make_unique<TTest>();
			m_Tests.push_back(std::move(test));
			m_TestNames.push_back(InName);
		}

		void DrawUI();
		void ShutdownImGui();

		void BeginRender();
		void Render();
		void EndRender();

		struct TransformData
		{
			Fission::Math::Mat4x4 ViewProjection;
			Fission::Math::Mat4x4 Transform;
		} m_TransformData;

	private:
		ViewportCamera m_Camera;

		Unique<Renderer> m_Renderer = nullptr;
		TestFramework::Unique<TestFramework::GraphicsPipeline> m_Pipeline = nullptr;
		std::unique_ptr<ImGuiPlatformContext> m_ImGuiContext = nullptr;

		std::vector<std::unique_ptr<TestBase>> m_Tests;
		std::vector<std::string> m_TestNames;
		TestBase* m_CurrentTest = nullptr;
		size_t m_CurrentTestIndex = 0;

		bool m_IsPaused = true;

		float m_SimulationTime = 0.0f;
	};

}
