#pragma once

// This header includes all files inside the framework project.
// It is meant for use by game projects ONLY.
// As new files are created, include them here so the game has easy access to them.
// It should **not** be included by any files inside the framework project.

#if GAME_PROJECT

#include "Core/CoreHeaders.h"

#include "Core/FWCore.h"
#include "Core/FWWindows.h"

#include "Camera.h"
#include "GameCore.h"
#include "Math/vec2.h"
#include"Math/ivec2.h"
#include "Resources/Color.h"
#include "Resources/Mesh.h"
#include "Resources/ShaderProgram.h"
#include "Resources/Texture.h"
#include "Resources/Utility.h"

#endif // GAME_PROJECT
