#pragma once

#include <RenderObject.h>
#include <RenderThread.h>
#include <Vector.h>

namespace GameEngine
{

	enum class ObjectType : uint16_t
	{
		Jumping = 0,
		Moving,
		Ñontrolled,
		Unknown
	};


	class GameObject final
	{
	public:
		GameObject(ObjectType type, Math::Vector3f pos = Math::Vector3f::Zero(), 
			       Math::Vector3f vel = Math::Vector3f::Zero())
			: m_Position{ pos }, m_StartPosition{ pos }, m_Velosity{ vel }, 
			  m_StartVelosity{ vel }, m_Type{ type } { };

	public:
		Render::RenderObject** GetRenderObjectRef() { return &m_RenderObject; }

		void SetPosition(Math::Vector3f position, size_t frame)
		{
			m_Position = position;

			if (m_RenderObject) [[likely]]
			{
				m_RenderObject->SetPosition(position, frame);
			}
		}

		void SetVelosity(Math::Vector3f velosity)
		{
			m_Velosity = velosity;
		}

		Math::Vector3f GetPosition()
		{
			return m_Position;
		}
		Math::Vector3f GetStartPosition()
		{
			return m_StartPosition;
		}

		Math::Vector3f GetVelosity()
		{
			return m_Velosity;
		}

		Math::Vector3f GetStartVelosity()
		{
			return m_StartVelosity;
		}

		void Move(Math::Vector3f dir)
		{
			m_CurrentMoveDir = m_CurrentMoveDir + dir;
		}
		Math::Vector3f GetMoveDir()
		{
			return m_CurrentMoveDir;
		}
		void SetMoveDir(Math::Vector3f dir)
		{
			m_CurrentMoveDir = dir;
		}
		ObjectType GetType()
		{
			return m_Type;
		}

	protected:
		Render::RenderObject* m_RenderObject = nullptr;
		ObjectType m_Type;

		Math::Vector3f m_Position;
		Math::Vector3f m_StartPosition;
		Math::Vector3f m_Velosity;
		Math::Vector3f m_StartVelosity;
		Math::Vector3f m_CurrentMoveDir;
	};
}