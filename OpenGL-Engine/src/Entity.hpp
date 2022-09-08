#pragma once
#include "Model.hpp"

using std::vector;

namespace Engine
{
	class Entity : public Transform
	{
	// Static
		Entity* s_root;

	// Member
	private:
		Entity* m_parent = nullptr;
		unique_ptr<vector<Entity*>> m_children = nullptr;

	public:
		Entity();
		Entity(Entity* pParent);

		Entity* GetParent() const;
		vector<Entity*>* GetChildren() const;
	};
}
