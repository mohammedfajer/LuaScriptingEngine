#pragma once



#include <SDL.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <chrono>

struct Timer_Module
{
	std::chrono::high_resolution_clock::time_point lastFrameTime;
	std::chrono::high_resolution_clock::time_point currentFrameTime;

	double frameTimes[60]; // Store the last 60 frames times for FPS calculations
	int frameIndex = 0;
	double deltaTime = 0.0;


};

extern Timer_Module gTimeInfo;

void update_timer_module();


extern "C" int luaopen_timer(lua_State * L);