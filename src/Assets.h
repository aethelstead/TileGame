#ifndef ASSETS_H
#define ASSETS_H

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "Utilities/UniqueID.h"
#include "Platform/Platform.h"
#include "Rendering/Texture.h"

#define MAP_PATH "assets/tmx/"
#define IMG_PATH "assets/img/"
#define TTF_PATH "assets/font/"
#define MUS_PATH "assets/music/"
#define SFX_PATH "assets/sfx/"

using TextureMap = std::unordered_map<UniqueID, std::unique_ptr<Gin::Texture>>;
using MusicMap = std::unordered_map<UniqueID, Gin::Platform::Music*>;
using SfxMap = std::unordered_map<UniqueID, Gin::Platform::Chunk*>;

#endif
