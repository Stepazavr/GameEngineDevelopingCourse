#pragma once

#include <Core/export.h>
#include <windows.h>

namespace GameEngine::Core
{
	class CORE_API Window final
	{
	public:
		Window() = default;

		void Init(void* instance);

		void* GetWindowHandle() const { return m_WndHndl; }

		uint16_t GetWidth() const { return m_Width; }
		uint16_t GetHeight() const { return m_Height; }
		void Resize(uint16_t newWidth, uint16_t newHeight) { m_Width = newWidth; m_Height = newHeight; }
		float GetAspectRatio() const { return (float)m_Width / (float)m_Height; }

		void UpdateCurrentTime();
		float GetCurrentTime() const { return m_CurrentTime; }


	private:
		uint16_t m_Width = 800;
		uint16_t m_Height = 600;

		float m_StartTime = 0.0f;
		float m_CurrentTime = 0.0f;

		void* m_WndHndl = nullptr;
	};

	extern CORE_API Window* MainWindowsApplication;
}