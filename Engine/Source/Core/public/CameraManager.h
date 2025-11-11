#pragma once

#include <Core/export.h>
#include <Matrix.h>
#include <Vector.h>
#include <Camera.h>
#include <list>

namespace GameEngine
{
	namespace Core
	{
		class CORE_API CameraManager final
		{
		public:
			using CameraList = std::list<Camera*>;

			CameraManager()
				: m_CurrCameraIt(m_CameraList.end())
			{}


			void CreateCamera();
			Camera* GetCamera();
			void SwitchNextCamera();
			void SwitchPrevCamera();

		private:
			void AddCamera(Camera* camera);

		private:
			CameraList m_CameraList;
			CameraList::iterator m_CurrCameraIt;
		};

		extern CORE_API CameraManager* g_CameraManager;
	}
}