#include "../framework/debug.h"

#include "statictilemesh.h"
#include "../framework/assets/static/staticmesh.h"
#include "../framework/assets/static/staticmeshsubset.h"
#include "../framework/graphics/vertexbuffer.h"
#include "../framework/math/mathhelpers.h"
#include "../framework/math/rectf.h"

StaticTileMesh::StaticTileMesh(const StaticMesh *mesh, const RectF *textureAtlasTileBoundaries, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, bool alpha, float translucency, const Color &color, const StaticMesh *collisionMesh)
{
	// only work with the first mesh subset
	StaticMeshSubset *subset = mesh->GetSubset(0);

	// copy the source buffer
	m_vertices = new VertexBuffer();
	ASSERT(m_vertices != NULL);
	m_vertices->Initialize(subset->GetVertices());

	SetOpaque(opaqueSides);
	SetAlpha(alpha);
	SetColor(color);
	SetTranslucency(translucency);
	SetLight(lightValue);

	// adjust texture coordinates, which are likely all within the full range 0.0f to 1.0f
	// to fit into the texture atlas tile's texture coordinate boundaries
	for (uint i = 0; i < m_vertices->GetNumElements(); ++i)
	{
		Vector2 texCoord = m_vertices->GetTexCoord(i);
		texCoord.x = ScaleRange(texCoord.x, 0.0f, 1.0f, textureAtlasTileBoundaries->left, textureAtlasTileBoundaries->right);
		texCoord.y = ScaleRange(texCoord.y, 0.0f, 1.0f, textureAtlasTileBoundaries->top, textureAtlasTileBoundaries->bottom);
		m_vertices->SetTexCoord(i, texCoord);
	}

	SetupCollisionVertices(collisionMesh);
}

StaticTileMesh::StaticTileMesh(const StaticMesh *mesh, const RectF *textureAtlasTileBoundaries, uint numTiles, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, bool alpha, float translucency, const Color &color, const StaticMesh *collisionMesh)
{
	ASSERT(numTiles > 0);
	ASSERT(mesh->GetNumSubsets() >= numTiles);

	// count up all the vertices needed for the number of subsets we're going to use
	uint numVertices = 0;
	for (uint i = 0; i < numTiles; ++i)
		numVertices += mesh->GetSubset(i)->GetVertices()->GetNumElements();

	// create a copy of the mesh's vertex attributes as an array
	const VertexBuffer *meshVertices = mesh->GetSubset(0)->GetVertices();
	VERTEX_ATTRIBS *attribs = new VERTEX_ATTRIBS[meshVertices->GetNumAttributes()];
	for (uint i = 0; i < meshVertices->GetNumAttributes(); ++i)
		attribs[i] = meshVertices->GetAttributeInfo(i)->standardType;
	
	// create the vertex buffer using the same attribs as the source mesh
	// (assuming all subsets have the same attribs in the mesh)
	m_vertices = new VertexBuffer();
	ASSERT(m_vertices != NULL);
	m_vertices->Initialize(attribs, meshVertices->GetNumAttributes(), numVertices, BUFFEROBJECT_USAGE_STATIC);

	SetOpaque(opaqueSides);
	SetAlpha(alpha);
	SetColor(color);
	SetTranslucency(translucency);
	SetLight(lightValue);

	uint currentVertex = 0;
	for (uint i = 0; i < numTiles; ++i)
	{
		StaticMeshSubset *subset = mesh->GetSubset(i);
		const RectF *tileBoundaries = &textureAtlasTileBoundaries[i];

		// copy all this subset's vertices
		m_vertices->Copy(subset->GetVertices(), currentVertex);

		// now we need to adjust the copied texture coordinates to match the tile boundaries
		for (uint t = 0; t < subset->GetVertices()->GetNumElements(); ++t)
		{
			uint position = t + currentVertex;

			Vector2 texCoord = m_vertices->GetTexCoord(position);
			texCoord.x = ScaleRange(texCoord.x, 0.0f, 1.0f, tileBoundaries->left, tileBoundaries->right);
			texCoord.y = ScaleRange(texCoord.y, 0.0f, 1.0f, tileBoundaries->top, tileBoundaries->bottom);
			m_vertices->SetTexCoord(position, texCoord);
		}

		currentVertex += subset->GetVertices()->GetNumElements();
	}

	SetupCollisionVertices(collisionMesh);
}

void StaticTileMesh::SetupCollisionVertices(const StaticMesh *collisionMesh)
{
	const VertexBuffer *srcCollisionVertices;
	if (collisionMesh != NULL)
		srcCollisionVertices = collisionMesh->GetSubset(0)->GetVertices();
	else
		srcCollisionVertices = m_vertices;

	m_numCollisionVertices = srcCollisionVertices->GetNumElements();

	m_collisionVertices = new Vector3[m_numCollisionVertices];
	ASSERT(m_collisionVertices != NULL);

	for (uint i = 0; i < m_numCollisionVertices; ++i)
		m_collisionVertices[i] = srcCollisionVertices->GetPosition3(i);
}

StaticTileMesh::~StaticTileMesh()
{
	SAFE_DELETE(m_vertices);
	SAFE_DELETE(m_collisionVertices);
}
