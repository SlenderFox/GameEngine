#pragma once
#include <glm/glm.hpp>

namespace Engine
{
    class GameObject
    {
    public:
        GameObject();
        GameObject(glm::mat4 pValue);

		virtual glm::mat4 GetTransform() const;
        virtual void SetTransform(glm::mat4 pValue);

    protected:
        glm::mat4 m_transform = glm::mat4(1);
    };
}
