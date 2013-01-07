// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include <fstream>
#include <iostream>

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
	string tileData = base64_decode(dataVal);
	std::cout << "tileData size = " << tileData.size() << '\n';
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
