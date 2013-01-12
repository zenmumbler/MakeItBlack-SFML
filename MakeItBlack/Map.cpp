// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include <fstream>
#include <iostream>
#include <algorithm>

#include "Map.h"

#include "rapidxml.hpp"
#include "base64.h"

using namespace std;


static std::vector<char> readTextFile(const std::string & fileName) {
	ifstream mapFile(fileName, ios::binary);
	mapFile.unsetf(ios::skipws);
	
	mapFile.seekg(0, ios::end);
	size_t size = mapFile.tellg();
	mapFile.seekg(0);
	
	vector<char> text(size + 1);
	mapFile.read(&text.front(), static_cast<streamsize>(size));
	text[size] = 0;
	
	return text;
}


MapLayer::MapLayer(rapidxml::xml_node<char>* node) {
	for (auto attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
		string name { attr->name() };
		if (name == "width")
			width = stoi(attr->value());
		if (name == "height")
			height = stoi(attr->value());
	}
	
	auto dataVal = node->first_node("data")->value();
	while (isspace(*dataVal))
		dataVal++;
	string tileBytes = base64_decode(dataVal);
	
	int tileCount = tileBytes.size() / 4,
		offset = 0;
	tileData.reserve(tileCount);
	while (tileCount--) {
		Tile tilex = *(reinterpret_cast<Tile*>(&tileBytes[offset]));
		tileData.push_back(tilex & 0x1fffffff); // strip away mirroring bits
		offset += 4;
	}
}

Tile MapLayer::tileAt(int row, int col) {
	if (row < 0 || col < 0 || row >= height || col >= width)
		return 999;
	return tileData[(row * width) + col];
}

void MapLayer::setTileAt(int row, int col, Tile tile) {
	if (row < 0 || col < 0 || row >= height || col >= width)
		return;
	tileData[(row * width) + col] = tile;
}

TileRange MapLayer::rangeOnRow(int row, int fromCol, int tileCount) {
	auto offset = tileData.begin() + (row * width) + fromCol;
	return { offset, offset + tileCount };
}

void MapLayer::eachTile(const std::function<void(int, int, Tile)> & callback) {
	int off = 0;
	for (int row=0; row<height; row++)
		for (int col=0; col<width; col++) {
			if (tileData[off])
				callback(row, col, tileData[off]);
			off++;
		}
}

int MapLayer::countExposedTiles() {
	return 50;
}



Map::Map(const std::string & fileName) {
	using namespace rapidxml;

	auto mapXML = readTextFile(fileName);
	xml_document<char> mapDoc;
	mapDoc.parse<0>(&mapXML.front());
	auto mapNode = mapDoc.first_node("map");

	for (auto attr = mapNode->first_attribute(); attr; attr = attr->next_attribute()) {
		string name { attr->name() };

		if (name == "width")
			width = stoi(attr->value());
		if (name == "height")
			height = stoi(attr->value());
	}
	
	for (auto layerNode = mapNode->first_node(); layerNode; layerNode = layerNode->next_sibling()) {
		string name { layerNode->name() };
		if (name == "layer")
			layers.emplace_back(new MapLayer(layerNode));
	}
}

const MapLayerRef & Map::layer(int index) {
	return layers[index];
}
