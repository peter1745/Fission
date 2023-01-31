#pragma once

#include <chrono>

namespace TestFramework {

	class Timer
	{
		using TimePoint = std::chrono::high_resolution_clock::time_point;

	public:
		
		Timer()
		{
			Start();
		}

		void Start()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		void Stop()
		{
			TimePoint now = std::chrono::high_resolution_clock::now();
			m_ElapsedMilliseconds = std::chrono::duration_cast<std::chrono::duration<float>>(now - m_Start).count();
		}

		float ElapsedSeconds() const { return m_ElapsedMilliseconds * 1000.0f; }
		float ElapsedMilliseconds() const { return m_ElapsedMilliseconds; }

	private:
		TimePoint m_Start;
		float m_ElapsedMilliseconds = 0.0f;

	};

}
