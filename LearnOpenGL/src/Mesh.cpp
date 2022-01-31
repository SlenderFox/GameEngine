#include "Mesh.hpp"

namespace Engine
{
	Mesh::Mesh()
	{
		m_vertices = new vector<float>();
		m_indices = new vector<unsigned int>();

		// For testing purposes
		for (int i = 0; i < 288; ++i)
			m_vertices->push_back(m_verticesArr[i]);
		m_vertices->shrink_to_fit();

		for (int i = 0; i < 6; ++i)
			m_indices->push_back(m_indicesArr[i]);
		m_indices->shrink_to_fit();
	}
	
	Mesh::Mesh(vector<float> pVertices, vector<unsigned int> pIndices)
	{
		m_vertices = new vector<float>(pVertices);
		m_indices = new vector<unsigned int>(pIndices);
	}
	
	Mesh::Mesh(vector<float>* pVertices, vector<unsigned int>* pIndices)
	{
		m_vertices = pVertices;
		m_indices = pIndices;
	}

	Mesh::~Mesh()
	{
		m_vertices->clear();
		m_indices->clear();
		delete m_vertices;
		delete m_indices;
	}
	
	Mesh::Mesh(const Mesh& pOther)
	{
		m_vertices = new vector<float>(*pOther.GetVertices());
		m_indices = new vector<unsigned int>(*pOther.GetIndices());
	}

	Mesh::Mesh(Mesh&& pOther)
	{
		m_vertices = new vector<float>(*pOther.GetVertices());
		m_indices = new vector<unsigned int>(*pOther.GetIndices());
	}

	Mesh& Mesh::operator=(const Mesh& pOther)
	{
		Mesh newObj = Mesh(pOther.GetVertices(), pOther.GetIndices());
		return newObj;
	}

	Mesh& Mesh::operator=(Mesh&& pOther)
	{
		Mesh newObj = Mesh(pOther.GetVertices(), pOther.GetIndices());
		return newObj;
	}

	vector<float>* Mesh::GetVertices() const
	{
		return m_vertices;
	}

	void Mesh::SetVertices(vector<float> pVertices)
	{
		m_vertices = new vector<float>(pVertices);
	}

	vector<unsigned int>* Mesh::GetIndices() const
	{
		return m_indices;
	}

	void Mesh::SetIndices(vector<unsigned int> pIndices)
	{
		m_indices = new vector<unsigned int>(pIndices);
	}

	unsigned int* Mesh::GetVAO()
	{
		return &m_idVAO;
	}

	unsigned int* Mesh::GetVBO()
	{
		return &m_idVBO;
	}

	unsigned int* Mesh::GetEBO()
	{
		return &m_idEBO;
	}
}
