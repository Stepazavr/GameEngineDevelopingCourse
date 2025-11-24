#include <CameraManager.h>

namespace GameEngine::Core
{
	std::unique_ptr<CameraManager> g_CameraManager = nullptr;

	Camera::Ptr CameraManager::CreateCamera()
	{
		Camera::Ptr newCamera = std::make_shared<Camera>();
		newCamera->SetPosition(startCameraPosition);
		newCamera->SetViewDir(startCameraViewDir);

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
			m_CameraList.push_back(static_cast<Camera::WeakPtr>(camera));
			m_CurrCameraIt = std::prev(m_CameraList.end());
			return;
		}

		m_CurrCameraIt = m_CameraList.insert(std::next(m_CurrCameraIt), static_cast<Camera::WeakPtr>(camera));
	}

	Camera::Ptr CameraManager::GetActiveCamera()
	{
		m_CurrCameraIt = FindValidCamera(m_CurrCameraIt);
		assert(m_CurrCameraIt != m_CameraList.end());
		return m_CurrCameraIt->lock();
	}

	void CameraManager::SetActiveCamera(Camera::WeakPtr camera)
	{
		if (camera.expired())
		{
			return;
		}

		CameraList::iterator newCameraIt = std::find_if(
			m_CameraList.begin(),
			m_CameraList.end(),
			[&](const Camera::WeakPtr& p)
			{
				if (!p.expired()) {
					return p.lock() == camera.lock();
				}
				return false;
			}
		);

		if (newCameraIt != m_CameraList.end())
		{
			m_CurrCameraIt = newCameraIt;
		}
	}

	void CameraManager::SwitchNextCamera()
	{
		if (m_CameraList.empty())
		{
			return;
		}

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
		{
			return;
		}

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

	CameraManager::CameraList::iterator CameraManager::FindValidCamera(CameraList::iterator startIt)
	{
		for (CameraList::iterator it = startIt; !m_CameraList.empty(); )
		{
			if (it == m_CameraList.end())
			{
				it = m_CameraList.begin();
			}

			if (!(it->expired()))
			{
				return it;
			}
			it = m_CameraList.erase(it);
		}

		return m_CameraList.end();
	}
}