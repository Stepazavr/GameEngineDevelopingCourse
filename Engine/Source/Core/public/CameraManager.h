#pragma once

#include <Core/export.h>
#include <Matrix.h>
#include <Vector.h>
#include <Camera.h>
#include <list>
#include <memory>

namespace GameEngine
{
	namespace Core
	{
		class CORE_API CameraManager final
		{
		public:
			using CameraList = std::list<Camera::Ptr>;

			CameraManager()
				: m_CurrCameraIt(m_CameraList.end())
			{}

			Camera::Ptr CreateCamera();
			Camera::Ptr GetActiveCamera();
			void SwitchNextCamera();
			void SwitchPrevCamera();

		private:
			void AddCamera(Camera::Ptr camera);

		private:
			CameraList m_CameraList;
			CameraList::iterator m_CurrCameraIt;
		};

		extern CORE_API CameraManager* g_CameraManager;
	}
}