#ifndef __CLASS_ENGINE_DEF_H__
#define __CLASS_ENGINE_DEF_H__

#pragma once
#include <string>
#include <map>

struct SInitParams 
{
	SInitParams()
		: fullscreen(false), show_fps(false), exclusiveModeinMouse(false), x(0), y(0), w(0), h(0), max_fps(0),
		  fonts_path(""), language_def(""), actions_path(""), gui_path(""), lua_path(""), sounds_path(""),
		  animatedmodels_path(""), effects_path(""), ROT_path(""), scenerenderercommands_path(""),
		  shadows_type("")
	{
	}

    bool fullscreen;
	bool show_fps;
	bool vsync;
	bool exclusiveModeinMouse;
	int x;
    int y;
    int w;
    int h;
	unsigned int max_fps;
	std::string fonts_path;
	std::string language_def;
	std::map<std::string, std::string> language_desc;
	std::string actions_path;
	std::string gui_path;
	std::string lua_path;
	std::string sounds_path;
	std::string animatedmodels_path;
	std::string effects_path;
	std::string ROT_path;
	std::string scenerenderercommands_path;
	std::string shadows_type;//OFF, PREDEFINED, LOW, MEDIUM, HIGH, ULTRA
};

#endif