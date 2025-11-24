#pragma once

#include <Core/export.h>
#include <Matrix.h>
#include <Vector.h>
#include <Camera.h>

namespace GameEngine
{
    namespace Core
    {
        class CORE_API CameraManager final
        {
        public:
            using CameraList = std::list<Camera::WeakPtr>;

            CameraManager()
                : m_CurrCameraIt(m_CameraList.end())
            {}

            Camera::Ptr CreateCamera();
            Camera::Ptr GetActiveCamera();
            void SwitchNextCamera();
            void SwitchPrevCamera();
            void SetActiveCamera(Camera::WeakPtr camera);

        private:
            void AddCamera(Camera::Ptr camera);
            //CameraList::iterator FindValidFrom(CameraList::iterator startIt, int direction = 1);

        private:
            CameraList m_CameraList;
            CameraList::iterator m_CurrCameraIt;

            inline static Math::Vector3f startCameraPosition = Math::Vector3f(0.0f, 12.0f, -10.0f);
            inline static Math::Vector3f startCameraViewDir = Math::Vector3f(0.0f, -6.0f, 12.0f);
        };

        extern CORE_API std::unique_ptr<CameraManager> g_CameraManager;
    }
}