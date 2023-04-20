#include <assert.h>
#include <string>
#include "mesh.hpp"
#include "renderer.hpp"

using std::string;
using std::vector;

namespace srender
{
	//Static

	vector<vertex> mesh::generateVertices() noexcept
	{
		vector<vertex> verts = vector<vertex>();

		// Makes cube with pos, normal, and texcoord
		for (uint8_t i = 0; i < 36; ++i)
		{
			vertex vert;
			for (uint8_t j = 0; j < 8; ++j)
			{
				switch (j)
				{
				case 0: vert.position.x  = s_cubeVerticesArr[i * 8 + j]; break;
				case 1: vert.position.y  = s_cubeVerticesArr[i * 8 + j]; break;
				case 2: vert.position.z  = s_cubeVerticesArr[i * 8 + j]; break;
				case 3: vert.normal.x    = s_cubeVerticesArr[i * 8 + j]; break;
				case 4: vert.normal.y    = s_cubeVerticesArr[i * 8 + j]; break;
				case 5: vert.normal.z    = s_cubeVerticesArr[i * 8 + j]; break;
				case 6: vert.texCoords.x = s_cubeVerticesArr[i * 8 + j]; break;
				case 7: vert.texCoords.y = s_cubeVerticesArr[i * 8 + j]; break;
				}
			}
			verts.push_back(vert);
		}
		verts.shrink_to_fit();
		return verts;
	}

	vector<uint32_t> mesh::generateIndices() noexcept
	{
		vector<uint32_t> inds = vector<uint32_t>();

		for (uint8_t i = 0; i < 36; ++i)
			inds.push_back(s_indicesArr[i]);
		inds.shrink_to_fit();
		return inds;
	}

	//Member

	mesh::mesh(
		const std::vector<vertex> *_vertices,
		const std::vector<uint32_t> *_indices
	) noexcept
	{
		m_vertices = new vector<vertex>(_vertices ? *_vertices : generateVertices());
		m_indices = new vector<uint32_t>(_indices ? *_indices : generateIndices());
		renderer::setupMesh(
			&m_idVAO,
			&m_idVBO,
			&m_idEBO,
			&(*m_vertices)[0].position[0],
			&(*m_indices)[0],
			(uint32_t)m_vertices->size() * sizeof(vertex),
			(uint32_t)m_indices->size() * sizeof(uint32_t),
			sizeof(vertex),
			offsetof(vertex, normal),
			offsetof(vertex, texCoords)
		);
	}

	mesh::~mesh()
	{
		renderer::deleteMesh(m_idVAO, m_idVBO, m_idEBO);
		delete m_vertices;
		delete m_indices;
	}

	void mesh::draw() const noexcept
	{	renderer::drawElements(m_idVAO, (uint32_t)m_indices->size()); }

	uint32_t mesh::getVAO() const noexcept
{	return m_idVAO; }

	uint32_t mesh::getVBO() const noexcept
{	return m_idVBO; }

	uint32_t mesh::getEBO() const noexcept
{	return m_idEBO; }
}
