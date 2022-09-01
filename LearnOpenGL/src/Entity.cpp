#include "Entity.hpp"

namespace Engine
{
	Entity::Entity()
	{
		m_children = new vector<Entity*>();
	}

	Entity::Entity(Entity* pParent)
	{
		m_parent = pParent;
		m_children = new vector<Entity*>();
	}

	Entity* Entity::GetParent() const
	{
		return m_parent;
	}

	vector<Entity*>* Entity::GetChildren() const
	{
		return m_children;
	}
}
