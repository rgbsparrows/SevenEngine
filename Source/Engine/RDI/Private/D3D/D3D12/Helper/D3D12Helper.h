#pragma once

#include <stdint.h>

#pragma comment(lib, "d3d12.lib")

constexpr uint32_t D3D12_TEXTURE_RESOLUTION = 1 << 13;
constexpr uint16_t D3D12_MIPMAPCOUNT = 13;
constexpr uint16_t D3D12_BACKBUFFER_COUNT = 8;
constexpr uint16_t D3D12_VERTEX_BUFFER_SLOT_COUNT = 16;
constexpr uint16_t D3D12_EXECUTE_COMMAND_LIST_COUNT = 32;
constexpr uint16_t D3D12_COMMAND_LIST_ALLOCATOR_COUNT = 8;
