#include "Renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::mat4;

namespace Engine
{
	bool Renderer::Init()
	{
        m_shaderRef = new Shader("../Assets/shaders/shader.vert", "../Assets/shaders/shader.frag");
        m_textureRef = new Texture();
        m_textureRef->LoadImages();
        //m_shaderRef->Use();
        m_shaderRef->SetInt("texture0", 0);
        m_shaderRef->SetInt("texture1", 1);
        m_shaderRef->SetInt("texture2", 2);
        
		// Enables the use of the depth buffer
		glEnable(GL_DEPTH_TEST);

        // Creates and assigns to an id the Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
        glGenVertexArrays(1, &m_idVAO);
        glGenBuffers(1, &m_idVBO);
        glGenBuffers(1, &m_idEBO);

        // Binds the vertex array so that the VBO and EBO are neatly stored within
        glBindVertexArray(m_idVAO);

        // GL_ARRAY_BUFFER effectively works like a pointer, using the id provided to point to the buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_idVBO);
        // Loads the vertices to the VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

        /*GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
        * GL_STATIC_DRAW: the data is set only once and used many times.
        *GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
        */

        // This buffer stores the indices that reference the elements of the VBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

        /*Tells the shader how to use the vertex data provided
        * p1: Which vertex attribute we want to configure in the vertex shader (location = 0)
        * p2: Vertex size (vec3)
        * p3: The type of data (vec is using floats)
        * p4: Whether we want to normalise the data
        * p5: Stride, because the data is tightly packed this can be 0
        * p6: Offset, for some reason a void*
        */
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Colour attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Texcoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Unbinds the vertex array
        glBindVertexArray(0);
        // Unbinds the GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Unbinds the GL_ELEMENT_ARRAY_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}

    void Renderer::Destroy(bool pValidate)
    {
        if (pValidate)
        {
            glDeleteVertexArrays(1, &m_idVAO);
            glDeleteBuffers(1, &m_idVBO);
            glDeleteBuffers(1, &m_idEBO);

            if (m_shaderRef != nullptr)
                m_shaderRef->Destroy(pValidate);
        }

        delete m_textureRef;
        delete m_shaderRef;     // This might be superfluous
    }

	void Renderer::Draw(glm::mat4 pCamera, double pTime)
	{
        // Rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Must be set to the current context
        glBindVertexArray(m_idVAO);

        m_shaderRef->Use();

        m_shaderRef->SetMat4("camera", pCamera);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        for (unsigned int i = 0; i < 10; i++)
        {
            mat4 model = mat4(1.0f);
            model = glm::translate(model, m_cubePositions[i]);
            float angle = (float)pTime * 30.0f * ((i + 1) / (i * 0.2f + 1));
            model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
            m_shaderRef->SetMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
	}
}
