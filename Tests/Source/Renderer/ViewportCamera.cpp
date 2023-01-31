#include "ViewportCamera.hpp"
#include "TestFramework/Core/Application.hpp"
#include "TestFramework/IO/Keyboard.hpp"

#include <Fission/Math/Math.hpp>

#include <iostream>

namespace FissionTests {

	using namespace TestFramework;
	
	// NOTE(Peter): Viewport camera shamelessly stolen from Hazel. Thanks Yan and Karim ;)

	using namespace Fission;

	ViewportCamera::ViewportCamera(uint32_t InWidth, uint32_t InHeight)
	{
		SetViewportSize(InWidth, InHeight);

		m_ViewMatrix = Math::Mat4x4(1.0f);
		m_ViewMatrix.Inverse();
	}

	void ViewportCamera::SetViewportSize(uint32_t InWidth, uint32_t InHeight)
	{
		m_Width = InWidth;
		m_Height = InHeight;
		SetProjectionMatrix(Math::Mat4x4::CreateProjection(Math::Radians(m_VerticalFOV), (float) InWidth, (float) InHeight, 0.01f, 1000.0f));
	}

	void ViewportCamera::OnUpdate(float InDeltaTime)
	{
		if (!m_IsActive)
		{
			Mouse::Get().SetVisibility(EMouseVisibility::Visible);
			return;
		}

		const auto& MousePosition = Mouse::Get().GetPosition();
		float MouseDeltaX = (MousePosition.x - m_InitialMousePosition.x) * 0.002f;
		float MouseDeltaY = (MousePosition.y - m_InitialMousePosition.y) * 0.002f;

		if (Mouse::Get().IsButtonHeld(EMouseButton::Right))
		{
			Mouse::Get().SetVisibility(EMouseVisibility::Locked);

			const float RotationSpeed = 0.7f;

			Math::FVec3 translation(0.0f);

			if (Keyboard::Get().IsKeyHeld(EKeyCode::Q))
				translation.Y -= 1.0f;
			if (Keyboard::Get().IsKeyHeld(EKeyCode::E))
				translation.Y += 1.0f;
			if (Keyboard::Get().IsKeyHeld(EKeyCode::S))
				translation.Z += 1.0f;
			if (Keyboard::Get().IsKeyHeld(EKeyCode::W))
				translation.Z -= 1.0f;
			if (Keyboard::Get().IsKeyHeld(EKeyCode::A))
				translation.X -= 1.0f;
			if (Keyboard::Get().IsKeyHeld(EKeyCode::D))
				translation.X += 1.0f;

			m_Location += m_RotationQ * translation.Normalized() * m_Speed * InDeltaTime;

			m_RotationQ = Math::FQuat(Math::FVec3(0.0f, -1.0f, 0.0f), MouseDeltaX) * m_RotationQ;
			m_RotationQ = m_RotationQ * Math::FQuat(Math::FVec3(-1.0f, 0.0f, 0.0f), MouseDeltaY);
			m_RotationQ.Normalize();

			m_ViewMatrix = Math::Mat4x4::CreateTranslation(m_Location) * Math::Mat4x4::FromQuat(m_RotationQ);
			m_ViewMatrix.Inverse();
		}
		else
		{
			Mouse::Get().SetVisibility(EMouseVisibility::Visible);
		}

		m_InitialMousePosition = MousePosition;
	}

}
