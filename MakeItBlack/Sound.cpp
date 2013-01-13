// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "Sound.h"


Sound::Sound() {
	
}

void Sound::loadSound(const std::string & name, const std::string & fileName) {
	auto buf = new sf::SoundBuffer();
	buf->LoadFromFile("data/" + fileName);
	buffers.push_back(buf);
	
	sf::Sound sound;
	sound.SetBuffer(*buffers.back());
	sounds[name] = sound;
}

void Sound::load(const std::function<void()> done) {
	loadSound("die", "die.wav");
	loadSound("fuzzledie", "fuzzledie2.wav");
	loadSound("jump", "jump.wav");
	loadSound("land", "land.wav");
	loadSound("pet", "pet.wav");
	loadSound("hit", "playerhit.wav");
	loadSound("splat", "splat.wav");
	
	done();
}

void Sound::play(const std::string & name) {
	sounds[name].Play();
}
