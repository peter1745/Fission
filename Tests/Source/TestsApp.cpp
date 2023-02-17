#include "TestsApp.hpp"

#include <TestFramework/Renderer/RenderSettings.hpp>
#include <TestFramework/Core/Timer.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <TestFramework/IO/FileIO.hpp>
#include <TestFramework/IO/Keyboard.hpp>
#include <TestFramework/Renderer/ShaderCompiler.hpp>

#include <TestFramework/Events/WindowEvents.hpp>

#include "Tests/SphereTest.hpp"
#include "Tests/SpheresStressTest.hpp"

#include <Fission/Collision/Shapes/SphereShape.hpp>

namespace FissionTests {

	TestFramework::Unique<TestFramework::ShaderCompiler> Compiler;

	TestsApp::TestsApp(const ApplicationSpecification& specification)
	    : Application(specification, this), m_Camera(specification.WindowWidth, specification.WindowHeight)
	{
	}

	void TestsApp::OnInit()
	{
		m_Renderer = MakeUnique<Renderer>(m_Context.get(), RendererInfo { m_SwapChain.get() });

		TestFramework::GraphicsPipelineInfo PipelineInfo = {};
		PipelineInfo.PipelineShader = Compiler->CompileFromFile("Resources/Shaders/FusionPBR.glsl");
		PipelineInfo.PrimitiveTopology = TestFramework::EPrimitiveTopology::Triangles;
		PipelineInfo.WindingOrder = TestFramework::EWindingOrder::CounterClockwise;
		PipelineInfo.RenderTargetCount = 1;
		PipelineInfo.RenderTargetFormats[0] = TestFramework::EFormat::BGRA8Unorm;
		PipelineInfo.DepthStencilFormat = TestFramework::EFormat::D24UnormS8UInt;
		m_Pipeline = MakeUnique<TestFramework::GraphicsPipeline>(TestFramework::GraphicsContext::Get(), PipelineInfo);

		m_ImGuiContext = ImGuiPlatformContext::Create();
		m_ImGuiContext->Init(GetWindow(), m_Context.get(), m_SwapChain.get());

		m_Camera.SetActive(true);

		RegisterTests();
	}

	void TestsApp::OnUpdate(float DeltaTime)
	{
		m_Camera.OnUpdate(DeltaTime);

		if (m_Tests[m_CurrentTestIndex]->NeedsRestart() && m_Tests[m_CurrentTestIndex]->IsRunning())
		{
			m_Tests[m_CurrentTestIndex]->SetNeedsRestart(false);

			m_Tests[m_CurrentTestIndex]->Stop();
			m_Tests[m_CurrentTestIndex]->Start();
		}

		if (m_Tests[m_CurrentTestIndex]->IsRunning() && !m_IsPaused)
		{
			TestFramework::Timer timer;
			m_Tests[m_CurrentTestIndex]->Simulate(DeltaTime);
			timer.Stop();

			m_SimulationTime = timer.ElapsedSeconds();
		}

		m_Renderer->BeginFrame();

		auto* CommandList = m_Renderer->GetCurrentCommandList();
		m_SwapChain->Bind(CommandList);
		m_SwapChain->Clear(CommandList);

		TestFramework::Viewport viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = GetWindow()->GetWidth();
		viewport.Height = GetWindow()->GetHeight();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		CommandList->SetViewports({ viewport });

		BeginRender();
		Render();
		EndRender();

		m_SwapChain->Unbind(CommandList);

		DrawUI();

		m_Renderer->EndFrame();
	}

	void TestsApp::OnShutdown()
	{
		ShutdownImGui();

		m_Renderer->Release();
		m_Renderer = nullptr;
	}

	void TestsApp::OnEvent([[maybe_unused]] Event& InEvent)
	{
		EventDispatcher dispatcher(InEvent);

		dispatcher.Dispatch<WindowResizeEvent>([&](const WindowResizeEvent& InResizeEvent)
		{
			m_Camera.SetViewportSize(InResizeEvent.GetWidth(), InResizeEvent.GetHeight());
			return false;
		});
	}

	void TestsApp::RegisterTests()
	{
		RegisterTest<SphereTest>("Spheres");
		RegisterTest<SpheresStressTest>("Spheres Stress");

		m_Tests[m_CurrentTestIndex]->Start();
	}

	static bool Dropdown(std::string_view InLabel, size_t& InSelectedValue, const std::vector<std::string>& InOptionLabels)
	{
		const auto& PreviewText = InOptionLabels[InSelectedValue];

		bool ValueChanged = false;

		if (ImGui::BeginCombo(InLabel.data(), PreviewText.c_str()))
		{
			for (size_t i = 0; i < InOptionLabels.size(); i++)
			{
				const auto& OptionLabel = InOptionLabels[i];
				if (ImGui::Selectable(OptionLabel.c_str(), InSelectedValue == i))
				{
					InSelectedValue = i;
					ValueChanged = true;
				}
			}

			ImGui::EndCombo();
		}

		return ValueChanged;
	}

	void TestsApp::DrawUI()
	{
		auto* CommandList = m_Renderer->GetCurrentCommandList();

		m_ImGuiContext->BeginFrame(CommandList);

		if (ImGui::Begin("Fission"))
		{
			size_t oldTestIndex = m_CurrentTestIndex;
			if (Dropdown("Current Test", m_CurrentTestIndex, m_TestNames))
			{
				if (m_Tests[oldTestIndex]->IsRunning())
					m_Tests[oldTestIndex]->Stop();

				m_IsPaused = true;

				m_Tests[oldTestIndex]->Unload();
				m_Tests[m_CurrentTestIndex]->Load();
				m_Tests[m_CurrentTestIndex]->Start();
			}

			auto& currentTest = m_Tests[m_CurrentTestIndex];

			if (ImGui::Button("Restart Test"))
			{
				currentTest->Stop();
				currentTest->Start();
			}

			ImGui::SameLine();

			if (ImGui::Button(m_IsPaused ? "Unpause" : "Pause"))
				m_IsPaused = !m_IsPaused;

			ImGui::Text("Simulation Time: %.4f", m_SimulationTime);

			const auto& stats = currentTest->GetPhysicsWorld()->GetStats();
			ImGui::Text("Bodies: %d", currentTest->GetPhysicsWorld()->GetBodies().size());
			ImGui::Text("Contacts: %d", stats.NumContacts);
			ImGui::Text("Collision Pairs: %d", stats.NumCollisionPairs);

			ImGui::DragFloat("Camera Speed", &m_Camera.GetSpeed(), 1.0f, 1.0f, 1000.0f);

			currentTest->DrawUI();

			ImGui::End();
		}

		ImGui::Render();

		m_ImGuiContext->EndFrame(CommandList);
	}

	void TestsApp::ShutdownImGui()
	{
		m_ImGuiContext->Shutdown();
	}

	void TestsApp::BeginRender()
	{
		m_TransformData.ViewProjection = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix();
		m_TransformData.Transform = Fission::Math::Mat4x4(1.0f);
	}

	void TestsApp::Render()
	{
		uint32_t CurrentFrame = TestFramework::Renderer::GetCurrent().GetCurrentFrame();
		auto* CmdList = TestFramework::Renderer::GetCurrent().GetCurrentCommandList();

		m_Pipeline->Bind(CmdList);

		auto& currentTest = m_Tests[m_CurrentTestIndex];
		auto* currentPhysicsWorld = currentTest->GetPhysicsWorld();

		if (!currentPhysicsWorld)
			return;

		for (const auto& actor : currentTest->GetActors())
		{
			const auto& position = actor.PhysicsBody->GetPosition();
			const auto& rotation = actor.PhysicsBody->GetRotation();

			m_TransformData.Transform = Fission::Math::Mat4x4::CreateTranslation({ position.X, position.Y, position.Z }) * Fission::Math::Mat4x4::FromQuat(rotation);
			CmdList->SetPushConstants(m_Pipeline.get(), TestFramework::EShaderStage::Vertex, sizeof(TransformData), &m_TransformData);

			TestFramework::VertexBufferView View = {};
			View.VertexBuffer = actor.Mesh.VertexBuffer.get();
			View.VertexStride = sizeof(TestFramework::Vertex);
			CmdList->SetVertexBuffer(View);

			TestFramework::IndexBufferView IndexView = {};
			IndexView.IndexBuffer = actor.Mesh.IndexBuffer.get();
			IndexView.IndexFormat = TestFramework::EFormat::R32UInt;
			CmdList->DrawIndexed(IndexView);
		}
	}

	void TestsApp::EndRender()
	{
	}

}

int main([[maybe_unused]] int ArgC, [[maybe_unused]] char** ArgV)
{
	TestFramework::ApplicationSpecification specification = {};
	specification.Title = "Fission Tests";

	auto& RenderSettings = TestFramework::RenderSettings::Get();
	RenderSettings.API = TestFramework::ERendererAPI::Vulkan;

	FissionTests::TestsApp* App = new FissionTests::TestsApp(specification);
	App->Run();
	delete App;

	return 0;
}
