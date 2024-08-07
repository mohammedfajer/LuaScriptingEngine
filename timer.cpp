#include "timer.h"

// ==============
// Provides high-resolution timing functionality
Timer_Module gTimeInfo;

void update_timer_module()
{
	gTimeInfo.lastFrameTime = gTimeInfo.currentFrameTime;
	gTimeInfo.currentFrameTime = std::chrono::high_resolution_clock::now();

	gTimeInfo.deltaTime = std::chrono::duration<double, std::milli>(
		gTimeInfo.currentFrameTime - gTimeInfo.lastFrameTime
	).count();

	gTimeInfo.frameTimes[gTimeInfo.frameIndex % 60] = gTimeInfo.deltaTime;
	gTimeInfo.frameIndex++;
}


static double getAverageDeltaImpl()
{
	double sum = 0.0;
	int count = std::min(gTimeInfo.frameIndex, 60);
	for (int i = 0; i < count; i++) {
		sum += gTimeInfo.frameTimes[i];
	}
	double averageDelta = (count > 0) ? (sum / count) : 0.0;
	return averageDelta;
}

/**
 * Returns the average delta time over the last second.
 */
static int lua_getAverageDelta(lua_State *L)
{
	double averageDelta =getAverageDeltaImpl();
	lua_pushnumber(L, averageDelta);
	return 1;
}

/**
 * Returns the time between the last two frames.
 */

static int lua_getDelta(lua_State *L)
{
	lua_pushnumber(L, gTimeInfo.deltaTime);
	return 1;
}

/**
 * Returns the current frames per second.
 */

static int lua_getFPS(lua_State *L)
{
	double averageDelta = getAverageDeltaImpl();
	double fps = (averageDelta > 0.0) ? (1000.0 / averageDelta) : 0.0;
	lua_pushnumber(L, fps);
	return 1;
}

/**
 * Returns the precise amount of time since some time in the past.
 */

static int lua_getTime(lua_State *L)
{
	auto now = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double>(now.time_since_epoch()).count();
	lua_pushnumber(L, duration);
	return 1;
}

/**
 * Pauses the current thread for the specified amount of time.
 */

static int lua_sleep(lua_State *L)
{
	int milliseconds = luaL_checkinteger(L, 1);
	SDL_Delay(milliseconds);
	return 0;
}

/**
 * Measures the time between two frames.
 */

static int lua_step(lua_State *L)
{
	update_timer_module();
	return 0;
}

// Interface

extern "C" int luaopen_timer(lua_State * L)
{
	static const luaL_Reg timerLib[] = {
	{"getAverageDelta", lua_getAverageDelta},
	{"getDelta", lua_getDelta},
	{"getFPS", lua_getFPS},
	{"getTime", lua_getTime},
	{"sleep", lua_sleep},
	{"step", lua_step},
	{NULL, NULL}

	};


	luaL_newlib(L, timerLib);
	return 1;
}
