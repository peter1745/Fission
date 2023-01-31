#pragma once

#include <stdint.h>

#include <Fission/Math/FQuat.hpp>
#include <Fission/Math/Mat4x4.hpp>

#include <TestFramework/IO/Mouse.hpp>

namespace FissionTests {

	// NOTE(Peter): Viewport camera shamelessly stolen from Hazel. Thanks Yan and Karim ;)

	class ViewportCamera
	{
	public:
		ViewportCamera(uint32_t InWidth, uint32_t InHeight);

		void SetViewportSize(uint32_t InWidth, uint32_t InHeight);

		void OnUpdate(float InDeltaTime);
		void SetActive(bool InActive) { m_IsActive = InActive; }

		void SetProjectionMatrix(const Fission::Math::Mat4x4& InProjectionMatrix) { m_ProjectionMatrix = InProjectionMatrix; }
		const Fission::Math::Mat4x4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		const Fission::Math::Mat4x4& GetViewMatrix() const { return m_ViewMatrix; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		float GetFieldOfView() const { return m_VerticalFOV; }
		float GetNearDistance() const { return m_NearPlane; }
		float GetFarDistance() const { return m_FarPlane; }

		float& GetSpeed() { return m_Speed; }

	private:

		float m_Speed = 50.0f;

		Fission::Math::Mat4x4 m_ProjectionMatrix = Fission::Math::Mat4x4(1.0f);
		Fission::Math::Mat4x4 m_ViewMatrix = Fission::Math::Mat4x4(1.0f);
		Fission::Math::FVec3 m_Location = Fission::Math::FVec3(0.0f);

		uint32_t m_Width, m_Height;

		Fission::Math::FQuat m_RotationQ;

		TestFramework::MousePos m_InitialMousePosition = { 0.0f, 0.0f };

		float m_VerticalFOV = 70.0f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 1000.0f;

		bool m_IsActive = false;
	};

}
