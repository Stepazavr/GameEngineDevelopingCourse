#include <CameraManager.h>
#include <cassert>
#include <iterator>

namespace GameEngine::Core
{
	CameraManager* g_CameraManager = nullptr;

	Camera::Ptr CameraManager::CreateCamera()
	{
		auto newCamera = std::make_shared<Camera>();
		newCamera->SetPosition(Math::Vector3f(0.0f, 12.0f, -10.0f));
		newCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 12.0f));

		AddCamera(newCamera);
		return newCamera;
	}

	void CameraManager::AddCamera(Camera::Ptr camera)
	{
		if (m_CameraList.empty())
		{
			m_CameraList.push_back(camera);
			m_CurrCameraIt = m_CameraList.begin();
			return;
		}

		if (m_CurrCameraIt == m_CameraList.end())
		{
			m_CameraList.push_back(camera);
			m_CurrCameraIt = std::prev(m_CameraList.end());
			return;
		}

		m_CurrCameraIt = m_CameraList.insert(std::next(m_CurrCameraIt), camera);
	}

	Camera::Ptr CameraManager::GetActiveCamera()
	{
		assert(m_CurrCameraIt != m_CameraList.end());
		return *m_CurrCameraIt;
	}

	void CameraManager::SwitchNextCamera()
	{
		if (m_CameraList.empty())
			return;

		if (m_CurrCameraIt == m_CameraList.end())
		{
			m_CurrCameraIt = m_CameraList.begin();
			return;
		}

		++m_CurrCameraIt;
		if (m_CurrCameraIt == m_CameraList.end())
		{
			m_CurrCameraIt = m_CameraList.begin();
		}
	}

	void CameraManager::SwitchPrevCamera()
	{
		if (m_CameraList.empty())
			return;

		if (m_CurrCameraIt == m_CameraList.end())
		{
			m_CurrCameraIt = std::prev(m_CameraList.end());
			return;
		}

		if (m_CurrCameraIt == m_CameraList.begin())
		{
			m_CurrCameraIt = std::prev(m_CameraList.end());
		}
		else
		{
			--m_CurrCameraIt;
		}
	}
}