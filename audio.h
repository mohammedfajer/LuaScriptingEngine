#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <sol/sol.hpp>
#include <map>
#include <string>


// Function to play a sound from a file path
void playSound(const std::string &filepath);


void bind_audio_module(sol::state &lua);

#endif // AUDIO_H