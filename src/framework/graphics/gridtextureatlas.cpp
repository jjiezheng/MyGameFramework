#include "gridtextureatlas.h"

#include "texture.h"

GridTextureAtlas::GridTextureAtlas(uint textureWidth, uint textureHeight, uint tileWidth, uint tileHeight, uint tileBorder, float texCoordEdgeOffset)
	: TextureAtlas(textureWidth, textureHeight, texCoordEdgeOffset)
{
	GenerateGrid(tileWidth, tileHeight, tileBorder);
}

GridTextureAtlas::GridTextureAtlas(Texture *source, uint tileWidth, uint tileHeight, uint tileBorder, float texCoordEdgeOffset)
	: TextureAtlas(source, texCoordEdgeOffset)
{
	GenerateGrid(tileWidth, tileHeight, tileBorder);
}

GridTextureAtlas::~GridTextureAtlas()
{
}

void GridTextureAtlas::GenerateGrid(uint tileWidth, uint tileHeight, uint tileBorder)
{
	m_tileWidth = tileWidth;
	m_tileHeight = tileHeight;

	tileWidth += tileBorder;
	tileHeight += tileBorder;

	uint tilesX = (GetWidth() - tileBorder) / (m_tileWidth + tileBorder);
	uint tilesY = (GetHeight() - tileBorder) / (m_tileHeight + tileBorder);

	m_tiles.resize(tilesX * tilesY);

	for (uint y = 0; y < tilesY; ++y)
	{
		for (uint x = 0; x < tilesX; ++x)
		{
			TextureAtlasTile *current = &m_tiles[(y * tilesX) + x];

			// set pixel location/dimensions
			current->dimensions.left = tileBorder + x * tileWidth;
			current->dimensions.top = tileBorder + y * tileHeight;
			current->dimensions.right = current->dimensions.left + tileWidth - tileBorder;
			current->dimensions.bottom = current->dimensions.top + tileHeight - tileBorder;

			// set texture coordinates
			// HACK: subtract GetTexCoordEdgeOffset() from the bottom right edges to 
			//       get around floating point rounding errors (adjacent tiles will 
			//       slightly bleed in otherwise)
			current->texCoords.left = (current->dimensions.left - tileBorder + GetTexCoordEdgeOffset()) / (float)GetWidth();
			current->texCoords.top = (current->dimensions.top - tileBorder + GetTexCoordEdgeOffset()) / (float)GetHeight();
			current->texCoords.right = ((float)current->dimensions.right + tileBorder - GetTexCoordEdgeOffset()) / (float)GetWidth();
			current->texCoords.bottom = ((float)current->dimensions.bottom + tileBorder - GetTexCoordEdgeOffset()) / (float)GetHeight();
		}
	}
}
