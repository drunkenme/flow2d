#pragma once

#define NS_FLOW2D_BEGIN namespace flow2d {
#define NS_FLOW2D_END }

// PRECOMPILED HEADERS
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <bitset>
#include <vector>
#include <array>
#include <list>
#include <unordered_map>
#include <algorithm>

NS_FLOW2D_BEGIN

#define INLINE inline

/// GLOBAL CONFIGS
static const size_t kDbgMaxTracebackFrames  = 63;
static const size_t kCacheDefaultThreshold  = 1024;
static const size_t kEntFirstChunksSize     = 1024;
static const size_t kEntGrowChunkSize       = 1024;
static const size_t kEntMaxComponents       = 64;
static const size_t kGfxMaxTextures         = 8;
static const size_t kGfxMaxPrograms         = 8;
static const size_t kGfxMaxAttributes       = 8;
static const size_t kGfxMaxUniforms         = 16;
static const size_t kGfxMaxBatchVertices    = 1024;

static const float kGfxDefaultScreenWidth   = 256.f;
static const float kGfxDefaultScreenHeight  = 256.f;

/// FORWARD DECLARATIONS
extern void ABORT(const char* file, int line, const char* format, ...);
extern void LOGI(const char* format, ...);
extern void LOGW(const char* format, ...);
extern void LOGE(const char* format, ...);

#define ASSERT(condition, format, ...) do { \
    if( !(condition) ) { \
        ABORT(__FILE__, __LINE__, "\nAssertion failed: %s, " format "\n", #condition, ##__VA_ARGS__);\
    } \
} while(0)

#define FATAL(format, ...) ASSERT(false, format, ##__VA_ARGS__)
#define ENSURE(condition) ASSERT(condition, "")

//- ENTITY COMPONENT SYSTEM
template<typename T> struct ComponentHandle;

struct Entity;
struct EntityIterator;
struct EntityView;
template<typename ...T> struct EntityViewTrait;
struct EntityManager;

struct System;
template<typename T> struct SystemTrait;
struct SystemManager;

//- EVENT DISPATCH SYSTEM
struct Event;
template<typename T> struct EventTrait;
struct EventManager;

//- RESOURCE CACHE SYSTEM
struct DataStream;
struct FilesystemArchive;
struct ArchiveManager;
struct ResourceCacheManager;

//- SCENE GRAPH SYSTEM
struct Transform;
struct SceneSystem;
struct ActionSystem;
struct TransformComponent;

NS_FLOW2D_END