#include "Mesh.hpp"
#include "Renderer.hpp"

namespace Engine
{
	Mesh::Mesh()
	{
		m_vertices = make_unique<vector<float>>();
		m_indices = make_unique<vector<unsigned int>>();

		// For testing purposes
		for (int i = 0; i < 288; ++i)
			m_vertices->push_back(m_verticesArr[i]);
		m_vertices->shrink_to_fit();

		for (int i = 0; i < 6; ++i)
			m_indices->push_back(m_indicesArr[i]);
		m_indices->shrink_to_fit();

		m_shaderRef = new Shader("../Assets/shaders/cube.vert", "../Assets/shaders/cube.frag");
		m_textureRef = new Texture();
		m_textureRef->LoadImages();
		//m_shaderRef->Use();
		m_shaderRef->SetInt("texture0", 0);
		m_shaderRef->SetInt("texture1", 1);
		m_shaderRef->SetInt("texture2", 2);
	}
	
	Mesh::Mesh(vector<float> pVertices, vector<unsigned int> pIndices)
	{
		m_vertices = make_unique<vector<float>>(pVertices);
		m_indices = make_unique<vector<unsigned int>>(pIndices);
	}
	
	Mesh::Mesh(unique_ptr<vector<float>> pVertices, unique_ptr<vector<unsigned int>> pIndices)
	{
		m_vertices = make_unique<vector<float>>(*pVertices);
		m_indices = make_unique<vector<unsigned int>>(*pIndices);
	}
	
	Mesh::Mesh(const Mesh& pOther)
	{
		m_vertices = make_unique<vector<float>>(*pOther.GetVertices());
		m_indices = make_unique<vector<unsigned int>>(*pOther.GetIndices());
	}

	Mesh::Mesh(Mesh&& pOther)
	{
		m_vertices = make_unique<vector<float>>(*pOther.GetVertices());
		m_indices = make_unique<vector<unsigned int>>(*pOther.GetIndices());
	}

	Mesh& Mesh::operator=(const Mesh& pOther)
	{
		Mesh* newObj = new Mesh(*pOther.GetVertices(), *pOther.GetIndices());
		return *newObj;
	}

	Mesh& Mesh::operator=(Mesh&& pOther)
	{
		Mesh* newObj = new Mesh(*pOther.GetVertices(), *pOther.GetIndices());
		return *newObj;
	}

	void Mesh::Destroy(bool pValidate)
	{
		if (m_shaderRef != nullptr)
			m_shaderRef->Destroy(pValidate);

		m_vertices->clear();
		m_indices->clear();
		delete m_shaderRef;
		delete m_textureRef;
		delete m_idVAO;
		delete m_idVBO;
		delete m_idEBO;
	}
	
	vector<float>* Mesh::GetVertices() const
	{
		return m_vertices.get();
	}

	void Mesh::SetVertices(vector<float>* pVertices)
	{
		m_vertices = make_unique<vector<float>>(*pVertices);
	}

	vector<unsigned int>* Mesh::GetIndices() const
	{
		return m_indices.get();
	}

	void Mesh::SetIndices(vector<unsigned int>* pIndices)
	{
		m_indices = make_unique<vector<unsigned int>>(*pIndices);
	}

	unsigned int* Mesh::GetVAO() const
	{
		return m_idVAO;
	}

	unsigned int* Mesh::GetVBO() const
	{
		return m_idVBO;
	}

	unsigned int* Mesh::GetEBO() const
	{
		return m_idEBO;
	}

	Shader* Mesh::GetShader() const
	{
		return m_shaderRef;
	}
	
	Texture* Mesh::GetTexture() const
	{
		return m_textureRef;
	}
}
