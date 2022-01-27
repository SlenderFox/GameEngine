#include "GameObject.hpp"

using glm::mat4;

namespace Engine
{
    GameObject::GameObject()
    { }

    GameObject::GameObject(mat4 pValue)
    {
        m_transform = pValue;
    }

    mat4 GameObject::GetTransform() const
    {
        return m_transform;
    }

    void GameObject::SetTransform(mat4 pValue)
    {
        m_transform = pValue;
    }
}
