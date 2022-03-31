#pragma once
#include "Transform.hpp"
#include <vector>

using std::vector;

namespace Engine
{
    class Entity : public Transform
    {
    public:
        Entity();
        Entity(Entity* pParent);

        Entity* GetParent() const;
        vector<Entity*>* GetChildren() const;

    private:
        Entity* m_parent = nullptr;
        vector<Entity*>* m_children = nullptr;
    };
}
