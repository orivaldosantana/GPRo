// -------------------------------------------------------------------
//   v-rep SDL2 plugin v1.0
//   Author:  Tobias Benz , tobias.m.benz [a] gmail.com
//   GitHub: https://github.com/To-be-b/v_repExtSDL2
// 
// -------------------------------------------------------------------
// THIS FILE IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// 
// You are free to use/modify/distribute this file for whatever purpose!
// -------------------------------------------------------------------


#include "v_repExtSDL2.h"

#include <SDL.h>

#include <iostream>
#include <vector>

#include "luaFunctionData.h"
#include "v_repLib.h"

// have a look what is really needed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
//#include <inttypes.h>
//#include <stdbool.h>
#include <conio.h>

#ifdef _WIN32
#ifdef QT_COMPIL
#include <direct.h>
#else
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#endif
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
#include <unistd.h>
#endif /* __linux || __APPLE__ */

#ifdef __APPLE__
#define _stricmp strcmp
#endif

#define CONCAT(x,y,z) x y z
#define strConCat(x,y,z)    CONCAT(x,y,z)

#define PLUGIN_VERSION 2 // 2 since version 3.2.1

LIBRARY vrepLib; // the V-REP library that we will dynamically load and bind
// --------------------------------------------------------------------------------------
// Functions of OpenAL
// --------------------------------------------------------------------------------------

class HapticJoystick
{

public:
    bool init_sdl(bool USE_HAPTIC);
    bool quit_sdl();
    bool refresh();
    bool isPressed(int button);
    int numButtons();
    int hatPosition();

    bool createDirectionalEffect(int dir_deg, int level);
    bool createGroovesEffect(int dir_deg, int level, int length);
    bool createRumbleEffect();
    bool playRumbleEffect(int strength, int duration);
    bool updateDirEffect(int dir_deg, int level);
    bool updateGroovesEffect(int dir_deg, int level, int length);
    bool playDirEffect();
    bool playGroovesEffect();
    bool stopRumbleEffect();
    bool destroyDirEffect();
    bool destroyGroovesEffect();
    bool stopDirEffect();
    bool stopGroovesEffect();

    SDL_Joystick *joy = NULL;
    SDL_Haptic *haptic;
    SDL_HapticEffect effectDir;
    SDL_HapticEffect effectGrooves;
    SDL_Event e;

    int effectIDDir;
    int effectIDGrooves;
    std::vector<float> Stick;
};

bool HapticJoystick::init_sdl(bool USE_HAPTIC)
{
    SDL_ClearError();
    if (SDL_Init(SDL_INIT_JOYSTICK) != 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        atexit(SDL_Quit);
        return (false);
    };
    if (SDL_NumJoysticks() > 0) {
        joy = SDL_JoystickOpen(0);
        if (joy != NULL) {
            printf("Open Joystick 0\n");
            printf("Name: %s\n", SDL_JoystickNameForIndex(0));
            printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
            printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
            printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
            printf("Number of Hats: %d\n", SDL_JoystickNumHats(joy));
        }
        else {
            fprintf(stderr, "Could not open Joystick 0: %s\n", SDL_GetError());
            return(false);
        }
    }
    // see if user wants to init haptic and if joystick is capable of haptic
    if (USE_HAPTIC) {
        if (SDL_JoystickIsHaptic(joy) == 1) {
            if (SDL_InitSubSystem(SDL_INIT_HAPTIC) != 0) {
                fprintf(stderr, "Unable to initialize Haptic System: %s\n", SDL_GetError());
                atexit(SDL_Quit);
                return (false);
            }
            else {
                haptic = SDL_HapticOpen(0); // SDL_HapticOpenFromJoystick(joy);
                if (haptic == NULL) {
                    fprintf(stderr, "Unable to initialize Haptic Joystick: %s\n", SDL_GetError());
                    return(false);
                }
                printf("Haptic system initialized \n");
            };

        }
        else {
            printf("Joystick is not haptic \n");
            return(false);
        };
    };

    return(true);
};
bool HapticJoystick::quit_sdl()
{
    if (haptic != NULL) {
        SDL_HapticClose(haptic);
        haptic = NULL;
        if (effectIDDir > 0) {
            SDL_HapticDestroyEffect(haptic, effectIDDir);
        }
        else if (effectIDGrooves > 0) {
            SDL_HapticDestroyEffect(haptic, effectIDGrooves);
        }

        printf("Haptic closed \n");
    }

    if (SDL_JoystickGetAttached(joy)) {
        SDL_JoystickClose(joy);
        joy = NULL;
        printf("Joystick closed \n");
    }

    SDL_Quit();
    printf("SDL closed \n");
    return(true);
}

bool HapticJoystick::refresh()
{
    Stick.clear();
    SDL_ClearError();
    SDL_JoystickUpdate();
    if (joy == NULL) {
        fprintf(stderr, "Unable to access joystick: %s\n", SDL_GetError());
        return (false);
    }
    else {

        for (int i = 0; i < SDL_JoystickNumAxes(joy); i++)
        {
            Stick.push_back(SDL_JoystickGetAxis(joy, i));
        }
        return true;
    }
    return false;
}
int HapticJoystick::numButtons()
{
    SDL_ClearError();
    if (joy == NULL) {
        fprintf(stderr, "Unable to access joystick: %s\n", SDL_GetError());
        return(-999);
    }else{
        return(SDL_JoystickNumButtons(joy));
    }
}
bool HapticJoystick::isPressed(int button)
{
    SDL_ClearError();
    SDL_JoystickUpdate();
    if (joy == NULL) {
        fprintf(stderr, "Unable to access joystick: %s\n", SDL_GetError());
        return (false);
    }
    if (SDL_JoystickGetButton(joy, button) == 1) {
        return(true);
    }
    return(false);
}
int HapticJoystick::hatPosition()
{

    SDL_ClearError();
    SDL_JoystickUpdate();
    if (joy == NULL) {
        fprintf(stderr, "Unable to access joystick: %s\n", SDL_GetError());
        return (-99999);
    }
    else {

        return(SDL_JoystickGetHat(joy, 0));
    }
}
// Haptic Effects
bool HapticJoystick::createDirectionalEffect(int dir_deg, int level)
{
    SDL_ClearError();
    if (haptic == NULL) {
        fprintf(stderr, "No Haptic Joystick initialized : %s\n", SDL_GetError());
        return(false);
    }
    if ((SDL_HapticQuery(haptic) & SDL_HAPTIC_CONSTANT) == 0) {
        SDL_HapticClose(haptic); // No sine effect
        fprintf(stderr, "Joystick does not support that effect");
        return false;
    }
    int dir = dir_deg * 100;
    memset(&effectDir, 0, sizeof(SDL_HapticEffect));
    effectDir.type = SDL_HAPTIC_CONSTANT;
    effectDir.constant.direction.type = SDL_HAPTIC_POLAR; // Polar coordinates
    effectDir.constant.direction.dir[0] = dir;
    effectDir.constant.level = level;
    effectDir.constant.length = 5; //VERSTEHEN DIESES WERTS
    printf("Directional force with magnitude of: %d and direction: %d ° \n", level, dir_deg);
    // Upload the effect
    effectIDDir = SDL_HapticNewEffect(haptic, &effectDir);
    if (effectIDDir < 0) {
        fprintf(stderr, "Effect can not be uploaded: %s\n", SDL_GetError());
        return false;
    }

    return true;
}
bool HapticJoystick::createGroovesEffect(int dir_deg, int level, int length)
{
    SDL_ClearError();
    if (haptic == NULL) {
        fprintf(stderr, "No Haptic Joystick initialized : %s\n", SDL_GetError());
        return(false);
    }

    int dir = dir_deg * 100;
    if ((SDL_HapticQuery(haptic) & SDL_HAPTIC_RAMP) == 0) {
        SDL_HapticClose(haptic);
        fprintf(stderr, "Joystick does not support that effect \n");
        return false;
    }
    memset(&effectGrooves, 0, sizeof(SDL_HapticEffect));
    effectGrooves.type = SDL_HAPTIC_RAMP;
    effectGrooves.ramp.direction.type = SDL_HAPTIC_POLAR; // Polar coordinates
    effectGrooves.ramp.direction.dir[0] = dir;
    effectGrooves.ramp.length = length;
    effectGrooves.ramp.start = level;
    effectGrooves.ramp.end = level;
    printf("Grooves with magnitude of: %d,length: %d ms and direction: %d ° \n", level, length, dir_deg);

    // Upload the effect
    effectIDGrooves = SDL_HapticNewEffect(haptic, &effectGrooves);
    if (effectIDGrooves < 0) {
        fprintf(stderr, "Effect can not be uploaded: %s\n", SDL_GetError());
        return false;
    }

    return true;
}
bool HapticJoystick::createRumbleEffect()
{
    SDL_ClearError();
    if (haptic == NULL) {
        fprintf(stderr, "No Haptic Joystick initialized : %s\n", SDL_GetError());
        return(false);
    }
    if (SDL_HapticRumbleSupported(haptic)) {
        if (SDL_HapticRumbleInit(haptic) != 0) {
            fprintf(stderr, "Failed to initialize rumble : %s\n", SDL_GetError());
        }
    }
    else {
        fprintf(stderr, "Rumble not supported : %s\n", SDL_GetError());
    }
    return(true);
}
bool HapticJoystick::updateDirEffect(int dir_deg, int level)
{
    SDL_ClearError();
    int dir = dir_deg * 100;

    effectDir.constant.direction.dir[0] = dir;
    effectDir.constant.level = level;

    effectDir.ramp.direction.dir[0] = dir;
    /*effect.ramp.start = level;
    effect.ramp.end = level;*/
    effectDir.constant.level = level;

    if (SDL_HapticUpdateEffect(haptic, effectIDDir, &effectDir) != 0) {
        fprintf(stderr, "Effect update failed : %s\n", SDL_GetError());
        return(false);
    }
    return(true);
}
bool HapticJoystick::updateGroovesEffect(int dir_deg, int level, int length)
{
    SDL_ClearError();
    int dir = dir_deg * 100;


    effectGrooves.ramp.direction.dir[0] = dir;
    /*effect.ramp.start = level;
    effect.ramp.end = level;*/
    effectGrooves.constant.level = level;
    effectGrooves.ramp.length = length;

    if (SDL_HapticUpdateEffect(haptic, effectIDGrooves, &effectGrooves) != 0) {
        fprintf(stderr, "Effect update failed : %s\n", SDL_GetError());
        return(false);
    }
    return(true);
}
bool HapticJoystick::playRumbleEffect(int strength, int duration)
{
    SDL_ClearError();
    if (SDL_HapticRumblePlay(haptic, strength, duration) != 0) {
        fprintf(stderr, "Error, rumble could not be played : %s\n", SDL_GetError());
        return(false);
    }
    return(true);
}
bool HapticJoystick::playDirEffect()
{
    SDL_ClearError();
    if (SDL_HapticRunEffect(haptic, effectIDDir, SDL_HAPTIC_INFINITY) != 0) // last integer may be set to SDL_HAPTIC_INFINITY
    {
        fprintf(stderr, "Effect can not be played: %s\n", SDL_GetError());
        return(false);
    }
    return true;
}
bool HapticJoystick::playGroovesEffect()
{
    SDL_ClearError();
    if (SDL_HapticRunEffect(haptic, effectIDGrooves, SDL_HAPTIC_INFINITY) != 0) // last integer may be set to SDL_HAPTIC_INFINITY
    {
        fprintf(stderr, "Effect can not be played: %s\n", SDL_GetError());
        return(false);
    }
    return true;
}
bool HapticJoystick::stopRumbleEffect()
{
    SDL_ClearError();
    if (SDL_HapticRumbleStop(haptic) != 0) {
        fprintf(stderr, "Effect can not be stopped: %s\n", SDL_GetError());
        return false;
    }
    return true;
}
bool HapticJoystick::stopDirEffect()
{
    SDL_ClearError();
    if (SDL_HapticStopEffect(haptic, effectIDDir) != 0) {
        fprintf(stderr, "Effect can not be stopped: %s\n", SDL_GetError());
        return false;
    }
    return true;
}
bool HapticJoystick::stopGroovesEffect()
{
    SDL_ClearError();
    if (SDL_HapticStopEffect(haptic, effectIDGrooves) != 0) {
        fprintf(stderr, "Effect can not be stopped: %s\n", SDL_GetError());
        return false;
    }
    return true;
}
bool HapticJoystick::destroyDirEffect()
{
    SDL_HapticDestroyEffect(haptic, effectIDDir);
    return true;
}
bool HapticJoystick::destroyGroovesEffect()
{
    SDL_HapticDestroyEffect(haptic, effectIDGrooves);
    return true;
}

HapticJoystick joystick;

#define LUA_INIT_SDL_COMMAND "simExtSDL_init"

const int inArgs_INIT_SDL[] = {
    1,
    sim_lua_arg_bool, 1,
};

void LUA_INIT_SDL_CALLBACK(SLuaCallBack *p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    bool res = false;
    bool useOfHaptic = false;
    if (D.readDataFromLua(p, inArgs_INIT_SDL, inArgs_INIT_SDL[0], LUA_INIT_SDL_COMMAND))
    {
        std::vector<CLuaFunctionDataItem>* inData = D.getInDataPtr();
        useOfHaptic = inData->at(0).boolData[0]; // the first argument
        res = joystick.init_sdl(useOfHaptic);
    }

    D.pushOutData(CLuaFunctionDataItem(res));
    D.writeDataToLua(p);
};

#define LUA_QUIT_SDL_COMMAND "simExtSDL_quit"

const int inArgs_QUIT_SDL[] = {
    0,
};

void LUA_QUIT_SDL_CALLBACK(SLuaCallBack *p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    bool res = false;
    if (D.readDataFromLua(p, inArgs_QUIT_SDL, inArgs_QUIT_SDL[0], LUA_QUIT_SDL_COMMAND))
    {
        // no input to work with
    }
    res = joystick.quit_sdl();
    D.pushOutData(CLuaFunctionDataItem(res));
    D.writeDataToLua(p);
};

#define LUA_REFRESH_STATE_COMMAND "simExtSDL_refreshState"

const int inArgs_REFRESH_STATE[] = {
    0,
};

void LUA_REFRESH_STATE_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;

    if (D.readDataFromLua(p, inArgs_REFRESH_STATE, inArgs_REFRESH_STATE[0], LUA_REFRESH_STATE_COMMAND))
    {
        // no inArgs to work with
    }

    bool refreshStateResult = joystick.refresh();

    D.pushOutData(CLuaFunctionDataItem(refreshStateResult));

    D.writeDataToLua(p);
}

#define LUA_GET_JOYSTICK_COORDS_COMMAND "simExtSDL_getJoystickCoords"

const int inArgs_GET_JOYSTICK_COORDS[] = {
    0,
};

void LUA_GET_JOYSTICK_COORDS_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;

    if (D.readDataFromLua(p, inArgs_GET_JOYSTICK_COORDS, inArgs_GET_JOYSTICK_COORDS[0], LUA_GET_JOYSTICK_COORDS_COMMAND))
    {
        // no inArgs to work with
    }
    
    D.pushOutData(CLuaFunctionDataItem(joystick.Stick));

    D.writeDataToLua(p);
}

#define LUA_GET_NUM_BUTTONS_COMMAND "simExtSDL_getNumButtons"

const int inArgs_GET_NUM_BUTTONS[] = {
    0,
};

void LUA_GET_NUM_BUTTONS_CALLBACK (SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    int numButtons = -999;
    if (D.readDataFromLua(p, inArgs_GET_NUM_BUTTONS, inArgs_GET_NUM_BUTTONS[0], LUA_GET_NUM_BUTTONS_COMMAND))
    {
            }
    D.pushOutData(CLuaFunctionDataItem(joystick.numButtons()));
    D.writeDataToLua(p);
}

#define LUA_IS_BUTTON_PRESSED_COMMAND "simExtSDL_isButtonPressed"

const int inArgs_IS_BUTTON_PRESSED[] = {
    1,
    sim_lua_arg_int, 1,
};

void LUA_IS_BUTTON_PRESSED_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    bool buttonState = false;
    if (D.readDataFromLua(p, inArgs_IS_BUTTON_PRESSED, inArgs_IS_BUTTON_PRESSED[0], LUA_IS_BUTTON_PRESSED_COMMAND))
    {
        std::vector<CLuaFunctionDataItem>* inData = D.getInDataPtr();
        int button = inData->at(0).intData[0]; // the first argument
        buttonState = joystick.isPressed(button);
    }
    D.pushOutData(CLuaFunctionDataItem(buttonState));
    D.writeDataToLua(p);
}

#define LUA_HAT_POSITION_COMMAND "simExtSDL_hatPosition"

const int inArgs_HAT_POSITION[] = {
    0,
};

void LUA_HAT_POSITION_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    if (D.readDataFromLua(p, inArgs_HAT_POSITION, inArgs_HAT_POSITION[0], LUA_HAT_POSITION_COMMAND))
    {
        // no input
    }
    D.pushOutData(CLuaFunctionDataItem(joystick.hatPosition()));
    D.writeDataToLua(p);
}

#define LUA_CREATE_DIR_EFFECT_COMMAND "simExtSDL_createDirEffect"

const int inArgs_CREATE_DIR_EFFECT[] = {
    2,
    sim_lua_arg_int, 1,
    sim_lua_arg_int, 1,
};

void LUA_CREATE_DIR_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    bool effectSuccess = false;
    if (D.readDataFromLua(p, inArgs_CREATE_DIR_EFFECT, inArgs_CREATE_DIR_EFFECT[0], LUA_CREATE_DIR_EFFECT_COMMAND))
    {
        std::vector<CLuaFunctionDataItem>* inData = D.getInDataPtr();

        int direction = inData->at(0).intData[0]; // the first argument
        int level = inData->at(1).intData[0];
        effectSuccess = joystick.createDirectionalEffect(direction, level);
    }
    D.pushOutData(CLuaFunctionDataItem(effectSuccess));
    D.writeDataToLua(p);
}

#define LUA_CREATE_GROOVES_EFFECT_COMMAND "simExtSDL_createGroovesEffect"

const int inArgs_CREATE_GROOVES_EFFECT[] = {
    3,
    sim_lua_arg_int, 1,
    sim_lua_arg_int, 1,
    sim_lua_arg_int, 1,
};

void LUA_CREATE_GROOVES_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    bool effectSuccess = false;
    if (D.readDataFromLua(p, inArgs_CREATE_GROOVES_EFFECT, inArgs_CREATE_GROOVES_EFFECT[0], LUA_CREATE_GROOVES_EFFECT_COMMAND))
    {
        std::vector<CLuaFunctionDataItem>* inData = D.getInDataPtr();

        int direction = inData->at(0).intData[0]; // the first argument
        int level = inData->at(1).intData[0];
        int length = inData->at(2).intData[0];
        effectSuccess = joystick.createGroovesEffect(direction, level, length);
    }
    D.pushOutData(CLuaFunctionDataItem(effectSuccess));
    D.writeDataToLua(p);
}

#define LUA_UPDATE_DIR_EFFECT_COMMAND "simExtSDL_updateDirEffect"

const int inArgs_UPDATE_DIR_EFFECT[] = {
    2,
    sim_lua_arg_int, 1,
    sim_lua_arg_int, 1,
};

void LUA_UPDATE_DIR_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    bool effectSuccess = false;
    if (D.readDataFromLua(p, inArgs_UPDATE_DIR_EFFECT, inArgs_UPDATE_DIR_EFFECT[0], LUA_UPDATE_DIR_EFFECT_COMMAND))
    {
        std::vector<CLuaFunctionDataItem>* inData = D.getInDataPtr();

        int direction_update = inData->at(0).intData[0]; // the first argument
        int level_update = inData->at(1).intData[0]; // the first argument
        effectSuccess = joystick.updateDirEffect(direction_update, level_update);
    }
    D.pushOutData(CLuaFunctionDataItem(effectSuccess));
    D.writeDataToLua(p);
}

#define LUA_UPDATE_GROOVES_EFFECT_COMMAND "simExtSDL_updateGroovesEffect"

const int inArgs_UPDATE_GROOVES_EFFECT[] = {
    3,
    sim_lua_arg_int, 1,
    sim_lua_arg_int, 1,
    sim_lua_arg_int, 1,
};

void LUA_UPDATE_GROOVES_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    bool effectSuccess = false;
    if (D.readDataFromLua(p, inArgs_UPDATE_GROOVES_EFFECT, inArgs_UPDATE_GROOVES_EFFECT[0], LUA_UPDATE_GROOVES_EFFECT_COMMAND))
    {
        std::vector<CLuaFunctionDataItem>* inData = D.getInDataPtr();

        int direction_update = inData->at(0).intData[0]; // the first argument
        int level_update = inData->at(1).intData[0];
        int length_update = inData->at(2).intData[0];
        effectSuccess = joystick.updateGroovesEffect(direction_update, level_update, length_update);
    }
    D.pushOutData(CLuaFunctionDataItem(effectSuccess));
    D.writeDataToLua(p);
}

#define LUA_PLAY_DIR_EFFECT_COMMAND "simExtSDL_playDirEffect"

const int inArgs_PLAY_DIR_EFFECT[] = {
    0,
};

void LUA_PLAY_DIR_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    if (D.readDataFromLua(p, inArgs_PLAY_DIR_EFFECT, inArgs_PLAY_DIR_EFFECT[0], LUA_PLAY_DIR_EFFECT_COMMAND))
    {
        // no input
    }
    D.pushOutData(CLuaFunctionDataItem(joystick.playDirEffect()));
    D.writeDataToLua(p);
}

#define LUA_PLAY_GROOVES_EFFECT_COMMAND "simExtSDL_playGroovesEffect"

const int inArgs_PLAY_GROOVES_EFFECT[] = {
    0,
};

void LUA_PLAY_GROOVES_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    if (D.readDataFromLua(p, inArgs_PLAY_GROOVES_EFFECT, inArgs_PLAY_GROOVES_EFFECT[0], LUA_PLAY_GROOVES_EFFECT_COMMAND))
    {
        // no input
    }
    D.pushOutData(CLuaFunctionDataItem(joystick.playGroovesEffect()));
    D.writeDataToLua(p);
}

#define LUA_DESTROY_DIR_EFFECT_COMMAND "simExtSDL_destroyDirEffect"

const int inArgs_DESTROY_DIR_EFFECT[] = {
    0,
};

void LUA_DESTROY_DIR_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    if (D.readDataFromLua(p, inArgs_DESTROY_DIR_EFFECT, inArgs_DESTROY_DIR_EFFECT[0], LUA_DESTROY_DIR_EFFECT_COMMAND))
    {
        // no input
    }
    D.pushOutData(CLuaFunctionDataItem(joystick.destroyDirEffect()));
    D.writeDataToLua(p);
}

#define LUA_DESTROY_GROOVES_EFFECT_COMMAND "simExtSDL_destroyGroovesEffect"

const int inArgs_DESTROY_GROOVES_EFFECT[] = {
    0,
};

void LUA_DESTROY_GROOVES_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    if (D.readDataFromLua(p, inArgs_DESTROY_GROOVES_EFFECT, inArgs_DESTROY_GROOVES_EFFECT[0], LUA_DESTROY_GROOVES_EFFECT_COMMAND))
    {
        // no input
    }
    D.pushOutData(CLuaFunctionDataItem(joystick.destroyGroovesEffect()));
    D.writeDataToLua(p);
}

#define LUA_STOP_DIR_EFFECT_COMMAND "simExtSDL_stopDirEffect"

const int inArgs_STOP_DIR_EFFECT[] = {
    0,
};

void LUA_STOP_DIR_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    if (D.readDataFromLua(p, inArgs_STOP_DIR_EFFECT, inArgs_STOP_DIR_EFFECT[0], LUA_STOP_DIR_EFFECT_COMMAND))
    {
        // no input
    }
    D.pushOutData(CLuaFunctionDataItem(joystick.stopDirEffect()));
    D.writeDataToLua(p);
}

#define LUA_STOP_RUMBLE_EFFECT_COMMAND "simExtSDL_stopRumbleEffect"

const int inArgs_STOP_RUMBLE_EFFECT[] = {
    0,
};

void LUA_STOP_RUMBLE_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    if (D.readDataFromLua(p, inArgs_STOP_RUMBLE_EFFECT, inArgs_STOP_RUMBLE_EFFECT[0], LUA_STOP_RUMBLE_EFFECT_COMMAND))
    {
        // no input
    }
    D.pushOutData(CLuaFunctionDataItem(joystick.stopRumbleEffect()));
    D.writeDataToLua(p);
}


#define LUA_STOP_GROOVES_EFFECT_COMMAND "simExtSDL_stopGroovesEffect"

const int inArgs_STOP_GROOVES_EFFECT[] = {
    0,
};

void LUA_STOP_GROOVES_EFFECT_CALLBACK(SLuaCallBack* p)
{
    p->outputArgCount = 1;
    CLuaFunctionData D;
    if (D.readDataFromLua(p, inArgs_STOP_GROOVES_EFFECT, inArgs_STOP_GROOVES_EFFECT[0], LUA_STOP_GROOVES_EFFECT_COMMAND))
    {
        // no input
    }
    D.pushOutData(CLuaFunctionDataItem(joystick.stopGroovesEffect()));
    D.writeDataToLua(p);
}

#define LUA_INIT_RUMBLE_COMMAND "simExtSDL_createRumbleEffect"

const int inArgs_INIT_RUMBLE[] = {
    0,
};

void LUA_INIT_RUMBLE_CALLBACK(SLuaCallBack* p) {
    p->outputArgCount = 1;
    CLuaFunctionData D;

    if (D.readDataFromLua(p, inArgs_INIT_RUMBLE, inArgs_INIT_RUMBLE[0], LUA_INIT_RUMBLE_COMMAND)) {
        // no Input to work with
    }

    D.pushOutData(CLuaFunctionDataItem(joystick.createRumbleEffect()));
    D.writeDataToLua(p);
}

#define LUA_PLAY_RUMBLE_EFFECT_COMMAND "simExtSDL_playRumbleEffect"

const int inArgs_PLAY_RUMBLE_EFFECT[] = {
    2,
    sim_lua_arg_int,1,
    sim_lua_arg_int,1,
};

void LUA_PLAY_RUMBLE_EFFECT_CALLBACK(SLuaCallBack* p) {
    p->outputArgCount = 1;
    CLuaFunctionData D;
    bool rumbleSuccess = false;
    if (D.readDataFromLua(p, inArgs_PLAY_RUMBLE_EFFECT, inArgs_PLAY_RUMBLE_EFFECT[0], LUA_PLAY_RUMBLE_EFFECT_COMMAND)) {
        std::vector<CLuaFunctionDataItem>* inData = D.getInDataPtr();

        int strength = inData->at(0).intData[0]; // the first argument
        int duration = inData->at(1).intData[0]; // the second argument

        rumbleSuccess = joystick.playRumbleEffect(strength, duration);
    }

    D.pushOutData(CLuaFunctionDataItem(rumbleSuccess));
    D.writeDataToLua(p);
}

// --------------------------------------------------------------------------------------


// This is the plugin start routine (called just once, just after the plugin was loaded):
VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer, int reservedInt)
{
    // Dynamically load and bind V-REP functions:
    // ******************************************
    // 1. Figure out this plugin's directory:
    char curDirAndFile[1024];
#ifdef _WIN32
#ifdef QT_COMPIL
    _getcwd(curDirAndFile, sizeof(curDirAndFile));
#else
    GetModuleFileName(NULL, curDirAndFile, 1023);
    PathRemoveFileSpec(curDirAndFile);
#endif
#elif defined (__linux) || defined (__APPLE__)
    getcwd(curDirAndFile, sizeof(curDirAndFile));
#endif

    std::string currentDirAndPath(curDirAndFile);
    // 2. Append the V-REP library's name:
    std::string temp(currentDirAndPath);
#ifdef _WIN32
    temp += "\\v_rep.dll";
#elif defined (__linux)
    temp += "/libv_rep.so";
#elif defined (__APPLE__)
    temp += "/libv_rep.dylib";
#endif /* __linux || __APPLE__ */
    // 3. Load the V-REP library:
    vrepLib = loadVrepLibrary(temp.c_str());
    if (vrepLib == NULL)
    {
        std::cout << "Error, could not find or correctly load the V-REP library. Cannot start 'PluginSkeleton' plugin.\n";
        return(0); // Means error, V-REP will unload this plugin
    }
    if (getVrepProcAddresses(vrepLib) == 0)
    {
        std::cout << "Error, could not find all required functions in the V-REP library. Cannot start 'PluginSkeleton' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0); // Means error, V-REP will unload this plugin
    }
    // ******************************************

    // Check the version of V-REP:
    // ******************************************
    int vrepVer;
    simGetIntegerParameter(sim_intparam_program_version, &vrepVer);
    if (vrepVer < 30200) // if V-REP version is smaller than 3.02.00
    {
        std::cout << "Sorry, your V-REP copy is somewhat old. Cannot start 'PluginSkeleton' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0); // Means error, V-REP will unload this plugin
    }
    // ******************************************

    std::vector<int> inArgs;

    // Register the new Lua command "simExtSkeleton_getSensorData":
    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_INIT_SDL, inArgs);
    simRegisterCustomLuaFunction(LUA_INIT_SDL_COMMAND, strConCat("boolean status=", LUA_INIT_SDL_COMMAND, "(boolean Haptic Initialization)"), &inArgs[0], LUA_INIT_SDL_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_QUIT_SDL, inArgs);
    simRegisterCustomLuaFunction(LUA_QUIT_SDL_COMMAND, strConCat("boolean status=", LUA_QUIT_SDL_COMMAND, "()"), &inArgs[0], LUA_QUIT_SDL_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_REFRESH_STATE, inArgs);
    simRegisterCustomLuaFunction(LUA_REFRESH_STATE_COMMAND,
        strConCat("boolean refreshState=", LUA_REFRESH_STATE_COMMAND, "()"), &inArgs[0], LUA_REFRESH_STATE_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_GET_JOYSTICK_COORDS, inArgs);
    simRegisterCustomLuaFunction(LUA_GET_JOYSTICK_COORDS_COMMAND,
        strConCat("table StickCoords=", LUA_GET_JOYSTICK_COORDS_COMMAND, "()"), &inArgs[0], LUA_GET_JOYSTICK_COORDS_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_GET_NUM_BUTTONS , inArgs);
    simRegisterCustomLuaFunction(LUA_GET_NUM_BUTTONS_COMMAND,
        strConCat("int numButtons=", LUA_GET_NUM_BUTTONS_COMMAND, "()"), &inArgs[0], LUA_GET_NUM_BUTTONS_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_IS_BUTTON_PRESSED, inArgs);
    simRegisterCustomLuaFunction(LUA_IS_BUTTON_PRESSED_COMMAND,
        strConCat("boolean isButtonPressed=", LUA_IS_BUTTON_PRESSED_COMMAND, "(int Button)"), &inArgs[0], LUA_IS_BUTTON_PRESSED_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_HAT_POSITION, inArgs);
    simRegisterCustomLuaFunction(LUA_HAT_POSITION_COMMAND,
        strConCat("integer hatPosition=", LUA_HAT_POSITION_COMMAND, "()"), &inArgs[0], LUA_HAT_POSITION_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_CREATE_DIR_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_CREATE_DIR_EFFECT_COMMAND,
        strConCat("boolen createDirEffect=", LUA_CREATE_DIR_EFFECT_COMMAND, "(int Polar_Coors_deg, int level(0-32767)"), &inArgs[0], LUA_CREATE_DIR_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_CREATE_GROOVES_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_CREATE_GROOVES_EFFECT_COMMAND,
        strConCat("boolen createGroovesEffect=", LUA_CREATE_GROOVES_EFFECT_COMMAND, "(int Polar_Coors_deg, int level(0-32767), int RampLenght"), &inArgs[0], LUA_CREATE_GROOVES_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_UPDATE_DIR_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_UPDATE_DIR_EFFECT_COMMAND,
        strConCat("boolen updateDirEffect=", LUA_UPDATE_DIR_EFFECT_COMMAND, "(int Polar_Coors_deg, int level(0-32767))"), &inArgs[0], LUA_UPDATE_DIR_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_UPDATE_GROOVES_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_UPDATE_GROOVES_EFFECT_COMMAND,
        strConCat("boolen updateGroovesEffect=", LUA_UPDATE_GROOVES_EFFECT_COMMAND, "(int Polar_Coors_deg, int level(0-32767), int RampLenght)"), &inArgs[0], LUA_UPDATE_GROOVES_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_PLAY_DIR_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_PLAY_DIR_EFFECT_COMMAND,
        strConCat("boolen playDirEffect=", LUA_PLAY_DIR_EFFECT_COMMAND, "()"), &inArgs[0], LUA_PLAY_DIR_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_PLAY_GROOVES_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_PLAY_GROOVES_EFFECT_COMMAND,
        strConCat("boolen playGroovesEffect=", LUA_PLAY_GROOVES_EFFECT_COMMAND, "()"), &inArgs[0], LUA_PLAY_GROOVES_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_STOP_DIR_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_STOP_DIR_EFFECT_COMMAND,
        strConCat("boolen stopDirEffect=", LUA_STOP_DIR_EFFECT_COMMAND, "()"), &inArgs[0], LUA_STOP_DIR_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_STOP_GROOVES_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_STOP_GROOVES_EFFECT_COMMAND,
        strConCat("boolen stopGroovesEffect=", LUA_STOP_GROOVES_EFFECT_COMMAND, "()"), &inArgs[0], LUA_STOP_GROOVES_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_DESTROY_DIR_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_DESTROY_DIR_EFFECT_COMMAND,
        strConCat("boolen destroyDirEffect=", LUA_DESTROY_DIR_EFFECT_COMMAND, "()"), &inArgs[0], LUA_DESTROY_DIR_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_DESTROY_GROOVES_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_DESTROY_GROOVES_EFFECT_COMMAND,
        strConCat("boolen destroyGroovesEffect=", LUA_DESTROY_GROOVES_EFFECT_COMMAND, "()"), &inArgs[0], LUA_DESTROY_GROOVES_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_INIT_RUMBLE, inArgs);
    simRegisterCustomLuaFunction(LUA_INIT_RUMBLE_COMMAND,
        strConCat("boolen initRumbleEffect=", LUA_INIT_RUMBLE_COMMAND, "()"), &inArgs[0], LUA_INIT_RUMBLE_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_PLAY_RUMBLE_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_PLAY_RUMBLE_EFFECT_COMMAND,
        strConCat("boolen playRumbleEffect=", LUA_PLAY_RUMBLE_EFFECT_COMMAND, "(int strengt(0-1), int duration[ms])"), &inArgs[0], LUA_PLAY_RUMBLE_EFFECT_CALLBACK);

    CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_STOP_RUMBLE_EFFECT, inArgs);
    simRegisterCustomLuaFunction(LUA_STOP_RUMBLE_EFFECT_COMMAND,
        strConCat("boolen stopRumbleEffect=", LUA_STOP_RUMBLE_EFFECT_COMMAND, ""), &inArgs[0], LUA_STOP_RUMBLE_EFFECT_CALLBACK);

    return 1;

}

// This is the plugin end routine (called just once, when V-REP is ending, i.e. releasing this plugin):
VREP_DLLEXPORT void v_repEnd()
{
    // Here you could handle various clean-up tasks

    unloadVrepLibrary(vrepLib); // release the library
}

// This is the plugin messaging routine (i.e. V-REP calls this function very often, with various messages):
VREP_DLLEXPORT void* v_repMessage(int message, int* auxiliaryData, void* customData, int* replyData)
{ // This is called quite often. Just watch out for messages/events you want to handle
    // Keep following 5 lines at the beginning and unchanged:
    static bool refreshDlgFlag = true;
    int errorModeSaved;
    simGetIntegerParameter(sim_intparam_error_report_mode, &errorModeSaved);
    simSetIntegerParameter(sim_intparam_error_report_mode, sim_api_errormessage_ignore);
    void* retVal = NULL;

    // Here we can intercept many messages from V-REP (actually callbacks). Only the most important messages are listed here.
    // For a complete list of messages that you can intercept/react with, search for "sim_message_eventcallback"-type constants
    // in the V-REP user manual.

    if (message == sim_message_eventcallback_refreshdialogs)
        refreshDlgFlag = true; // V-REP dialogs were refreshed. Maybe a good idea to refresh this plugin's dialog too

    if (message == sim_message_eventcallback_menuitemselected)
    { // A custom menu bar entry was selected..
        // here you could make a plugin's main dialog visible/invisible
    }

    if (message == sim_message_eventcallback_instancepass)
    {   // This message is sent each time the scene was rendered (well, shortly after) (very often)
        // It is important to always correctly react to events in V-REP. This message is the most convenient way to do so:

        int flags = auxiliaryData[0];
        bool sceneContentChanged = ((flags&(1 + 2 + 4 + 8 + 16 + 32 + 64 + 256)) != 0); // object erased, created, model or scene loaded, und/redo called, instance switched, or object scaled since last sim_message_eventcallback_instancepass message 
        bool instanceSwitched = ((flags & 64) != 0);

        if (instanceSwitched)
        {
            // React to an instance switch here!!
        }

        if (sceneContentChanged)
        { // we actualize plugin objects for changes in the scene

            //...

            refreshDlgFlag = true; // always a good idea to trigger a refresh of this plugin's dialog here
        }
    }

    if (message == sim_message_eventcallback_mainscriptabouttobecalled)
    { // The main script is about to be run (only called while a simulation is running (and not paused!))

    }

    if (message == sim_message_eventcallback_simulationabouttostart)
    { // Simulation is about to start

    }

    if (message == sim_message_eventcallback_simulationended)
    { // Simulation just ended

    }

    if (message == sim_message_eventcallback_moduleopen)
    { // A script called simOpenModule (by default the main script). Is only called during simulation.
        if ((customData == NULL) || (_stricmp("PluginSkeleton", (char*)customData) == 0)) // is the command also meant for this plugin?
        {
            // we arrive here only at the beginning of a simulation
        }
    }

    if (message == sim_message_eventcallback_modulehandle)
    { // A script called simHandleModule (by default the main script). Is only called during simulation.
        if ((customData == NULL) || (_stricmp("PluginSkeleton", (char*)customData) == 0)) // is the command also meant for this plugin?
        {
            // we arrive here only while a simulation is running
        }
    }

    if (message == sim_message_eventcallback_moduleclose)
    { // A script called simCloseModule (by default the main script). Is only called during simulation.
        if ((customData == NULL) || (_stricmp("PluginSkeleton", (char*)customData) == 0)) // is the command also meant for this plugin?
        {
            // we arrive here only at the end of a simulation
        }
    }

    if (message == sim_message_eventcallback_instanceswitch)
    { // We switched to a different scene. Such a switch can only happen while simulation is not running

    }

    if (message == sim_message_eventcallback_broadcast)
    { // Here we have a plugin that is broadcasting data (the broadcaster will also receive this data!)

    }

    if (message == sim_message_eventcallback_scenesave)
    { // The scene is about to be saved. If required do some processing here (e.g. add custom scene data to be serialized with the scene)

    }

    // You can add many more messages to handle here

    if ((message == sim_message_eventcallback_guipass) && refreshDlgFlag)
    { // handle refresh of the plugin's dialogs
        // ...
        refreshDlgFlag = false;
    }

    // Keep following unchanged:
    simSetIntegerParameter(sim_intparam_error_report_mode, errorModeSaved); // restore previous settings
    return(retVal);
}

/*--------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------*/
////Get initialize rumble
//if (SDL_HapticRumbleInit(haptic) < 0)
//{
//  printf("Warning: Unable to initialize rumble! SDL Error: %s\n", SDL_GetError());
//}

//memset(&effect, 0, sizeof(SDL_HapticEffect)); // 0 is safe default
//effect.type = SDL_HAPTIC_SINE;
//effect.periodic.direction.type = SDL_HAPTIC_POLAR; // Polar coordinates
//effect.periodic.direction.dir[0] = 18000; // Force comes from south
//effect.periodic.period = 1000; // 1000 ms
//effect.periodic.magnitude = 20000; // 20000/32767 strength
//effect.periodic.length = 5000; // 5 seconds long
//effect.periodic.attack_length = 1000; // Takes 1 second to get max strength
//effect.periodic.fade_length = 1000; // Takes 1 second to fade away

//// Play rumble at 75 % strenght for 500 milliseconds
//  if (SDL_HapticRumblePlay(haptic, 1, dir) != 0)
//  {
//      printf("Warning: Unable to play rumble! %s\n", SDL_GetError());
//  }