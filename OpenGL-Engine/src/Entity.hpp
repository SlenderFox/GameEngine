#pragma once
#include "Model.hpp"

using std::vector;

namespace Engine
{
	class Entity : public Transform
	{
	private:
		Entity* m_parent = nullptr;
		vector<Entity*>* m_children = nullptr;

	public:
		Entity();
		Entity(Entity* pParent);

		Entity* GetParent() const;
		vector<Entity*>* GetChildren() const;
	};
}
