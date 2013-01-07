// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Map__
#define __MakeItBlack__Map__

#include <vector>
#include <memory>

namespace rapidxml {
	template<class Ch> class xml_node;
}

class MapLayer {
	int width = 0, height = 0;

public:
	MapLayer(rapidxml::xml_node<char>* node);
};

using MapLayerRef = std::unique_ptr<MapLayer>;
using MapLayerList = std::vector<MapLayerRef>;


class Map {
	int width = 0, height = 0;
	MapLayerList layers;

public:
	Map(const std::string & fileName);
};

#endif /* defined(__MakeItBlack__Map__) */
