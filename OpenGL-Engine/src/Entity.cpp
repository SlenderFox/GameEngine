#include "Entity.hpp"

namespace Engine
{
	Entity* Engine::s_root = new Entity();

	Entity::Entity()
	{
		m_children = make_unique<vector<Entity*>>();
	}

	Entity::Entity(Entity* pParent)
	{
		m_parent = pParent;
		m_children = make_unique<vector<Entity*>>();
	}

	Entity* Entity::GetParent() const
	{
		return m_parent;
	}

	vector<Entity*>* Entity::GetChildren() const
	{
		return m_children.get();
	}
}
