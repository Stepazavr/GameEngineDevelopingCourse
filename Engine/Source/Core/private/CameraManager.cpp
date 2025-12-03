#include <CameraManager.h>

namespace GameEngine::Core
{
	std::unique_ptr<CameraManager> g_CameraManager = nullptr;

	Camera* CameraManager::CreateCamera()
	{
		Camera* newCamera = new Camera();
		newCamera->SetPosition(startCameraPosition);
		newCamera->SetViewDir(startCameraViewDir);

		AddCamera(newCamera);
		return newCamera;
	}

	void CameraManager::AddCamera(Camera* camera)
	{
		if (m_CurrCameraIt == m_CameraList.end())
		{
			m_CameraList.push_back(std::unique_ptr<Camera>(camera));
			m_CurrCameraIt = std::prev(m_CameraList.end());
			return;
		}

		m_CurrCameraIt = m_CameraList.insert(std::next(m_CurrCameraIt), std::unique_ptr<Camera>(camera));
	}

	Camera* CameraManager::GetActiveCamera()
	{
		assert(m_CurrCameraIt != m_CameraList.end());
		return m_CurrCameraIt->get();
	}

	void CameraManager::SetActiveCamera(Camera* camera)
	{
		if (!camera)
		{
			return;
		}

		CameraList::iterator newCameraIt = std::find_if(
			m_CameraList.begin(),
			m_CameraList.end(),
			[&](const Camera::Ptr& p)
			{
				return p.get() == camera;
			}
		);

		m_CurrCameraIt = newCameraIt;
	}

	void CameraManager::SwitchNextCamera()
	{
		if (m_CameraList.empty())
		{
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
		{
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

	void CameraManager::DeleteCamera(Camera* camera)
	{
		CameraList::iterator forDelCameraIt = std::find_if(
			m_CameraList.begin(),
			m_CameraList.end(),
			[&](const Camera::Ptr& p)
			{
				return p.get() == camera;
			}
		);

		if (m_CurrCameraIt == forDelCameraIt)
		{
			SwitchNextCamera();
		}
		m_CameraList.erase(forDelCameraIt);
	}

}