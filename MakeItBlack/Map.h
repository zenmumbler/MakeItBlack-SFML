// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Map__
#define __MakeItBlack__Map__

#include <vector>
#include <memory>
#include <utility>

// forward decl for rapidxml
namespace rapidxml {
	template<class Ch> class xml_node;
}

using Tile = uint32_t;
using TileVector = std::vector<Tile>;
using TileVectorIt = TileVector::iterator;
using TileRange = std::pair<TileVectorIt, TileVectorIt>;

class MapLayer {
	int width = 0, height = 0;
	TileVector tileData;

public:
	MapLayer(rapidxml::xml_node<char>* node);
	
	uint32_t tileAt(int row, int col);
	void setTileAt(int row, int col, uint32_t tile);
	TileRange rangeOnRow(int row, int fromCol, int tileCount);
	
	int countExposedTiles();
};

using MapLayerRef = std::unique_ptr<MapLayer>;
using MapLayerList = std::vector<MapLayerRef>;


class Map {
	int width = 0, height = 0;
	MapLayerList layers;

public:
	Map(const std::string & fileName);
	
	const MapLayerRef & layer(int index);
	int getWidth() { return width; }
	int getHeight() { return height; }
};

#endif /* defined(__MakeItBlack__Map__) */
