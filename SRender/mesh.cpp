#include <assert.h>
#include <string>
#include "mesh.hpp"
#include "renderer.hpp"

using std::string;
using std::vector;

namespace srender
{
/** Hard coded vertices for a cube
 * @deprecated No longer used since Model class
 */
constexpr float l_cubeVerticesArr[288] = {
	// Positions				// Normals				  // Texture coords
	-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
};

/** Hard coded indices for a cube
 * @deprecated No longer used since Model class
 */
constexpr uint32_t l_indicesArr[36] = {
	0U,	1U,	2U,	3U,	4U,	5U,	// Face 1
	6U, 	7U,	8U,	9U,	10U,	11U,	// Face 2
	12U,	13U,	14U,	15U,	16U,	17U,	// Face 3
	18U,	19U,	20U,	21U,	22U,	23U,	// Face 4
	24U,	25U,	26U,	27U,	28U,	29U,	// Face 5
	30U,	31U,	32U,	33U,	34U,	35U	// Face 6
};

//Static

vector<mesh::vertex> mesh::generateVertices() noexcept
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
			case 0: vert.position.x  = l_cubeVerticesArr[i * 8 + j]; break;
			case 1: vert.position.y  = l_cubeVerticesArr[i * 8 + j]; break;
			case 2: vert.position.z  = l_cubeVerticesArr[i * 8 + j]; break;
			case 3: vert.normal.x    = l_cubeVerticesArr[i * 8 + j]; break;
			case 4: vert.normal.y    = l_cubeVerticesArr[i * 8 + j]; break;
			case 5: vert.normal.z    = l_cubeVerticesArr[i * 8 + j]; break;
			case 6: vert.texCoords.x = l_cubeVerticesArr[i * 8 + j]; break;
			case 7: vert.texCoords.y = l_cubeVerticesArr[i * 8 + j]; break;
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
	{	inds.push_back(l_indicesArr[i]); }
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
