// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Sound__
#define __MakeItBlack__Sound__

#include <functional>
#include <vector>
#include <unordered_map>
#include <SFML/Audio.hpp>

class Sound {
	std::vector<sf::SoundBuffer*> buffers;
	std::unordered_map<std::string, sf::Sound> sounds;

	void loadSound(const std::string & name, const std::string & fileName);
	
public:
	Sound();

	void load(const std::function<void()> done);
	void play(const std::string & name);
};


#endif /* defined(__MakeItBlack__Sound__) */
