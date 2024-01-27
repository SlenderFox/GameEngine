#include <assert.h>
#include <string>
#include "mesh.hpp"
#include "renderer.hpp"

using std::string;
using std::vector;

namespace srender
{
/** Hard coded vertices for a square face. */
constexpr float l_VerticesArr[32] = {
	// Positions          // Normals          // Texture coords
	-0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
	-0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
	 0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f
};

/** Hard coded indices for a square face. */
constexpr uint32_t l_indicesArr[6] = {
	0U, 1U, 2U, // Tri one
	2U, 3U, 0U  // Tri two
};

//Static

vector<mesh::vertex> mesh::generateVertices() noexcept
{
	vector<vertex> verts = vector<vertex>();
	// Makes cube with pos, normal, and texcoord
	for (uint8_t i = 0; i < 4; ++i)
	{
		vertex vert;
		for (uint8_t j = 0; j < 8; ++j)
		{
			switch (j)
			{
			case 0: vert.position.x  = l_VerticesArr[i * 8 + j]; break;
			case 1: vert.position.y  = l_VerticesArr[i * 8 + j]; break;
			case 2: vert.position.z  = l_VerticesArr[i * 8 + j]; break;
			case 3: vert.normal.x    = l_VerticesArr[i * 8 + j]; break;
			case 4: vert.normal.y    = l_VerticesArr[i * 8 + j]; break;
			case 5: vert.normal.z    = l_VerticesArr[i * 8 + j]; break;
			case 6: vert.texCoords.x = l_VerticesArr[i * 8 + j]; break;
			case 7: vert.texCoords.y = l_VerticesArr[i * 8 + j]; break;
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
	for (uint8_t i = 0; i < 6; ++i)
	{	inds.push_back(l_indicesArr[i]); }
	inds.shrink_to_fit();
	return inds;
}

//Member

mesh::mesh(
	std::vector<vertex> *_vertices,
	std::vector<uint32_t> *_indices,
	bool _save
) noexcept
{
	assert(_vertices && _indices);
	renderer::setupMesh(
		&m_idVAO,
		&m_idVBO,
		&m_idEBO,
		&(*_vertices)[0].position[0],
		&(*_indices)[0],
		(uint32_t)_vertices->size() * sizeof(vertex),
		(uint32_t)_indices->size() * sizeof(uint32_t),
		sizeof(vertex),
		offsetof(vertex, normal),
		offsetof(vertex, texCoords)
	);
	if (_save)
	{
		m_vertices = _vertices;
		m_indices = _indices;
	}
	m_count = (uint32_t)_indices->size();
}

mesh::~mesh()
{
	renderer::deleteMesh(m_idVAO, m_idVBO, m_idEBO);
	if (m_vertices)
	{	delete m_vertices; }
	if (m_indices)
	{	delete m_indices; }
}

void mesh::draw() const noexcept
{	renderer::drawElements(m_idVAO, m_count); }

uint32_t mesh::getVAO() const noexcept
{	return m_idVAO; }

uint32_t mesh::getVBO() const noexcept
{	return m_idVBO; }

uint32_t mesh::getEBO() const noexcept
{	return m_idEBO; }
}
