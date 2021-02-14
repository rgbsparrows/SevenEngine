#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <functional>
#include <stdint.h>

namespace D3DAPIWarp_Impl
{
	uint32_t CreateDXGIFactory_D3DImpl(bool _isDebugMode, void** _dxgiFactory) noexcept;

#pragma region IDXGIFactory
	uint32_t EnumDXGIAdapter_D3DImpl(void* _dxgiFactory, uint32_t _index, void** _adapter) noexcept;
	uint32_t EnumDXGIWarpAdapter_D3DImpl(void* _dxgiFactory, void** _warpAdapter) noexcept;
#pragma endregion

#pragma region IDXGIAdapter
	uint32_t GetDXGIAdapterDesc_D3DImpl(void* _adapter, std::wstring* _description, uint64_t* _dedicatedVideoMemory, uint64_t* _dedicatedSystemMemory, uint64_t* _sharedSystemMemory) noexcept;
	uint32_t EnumDXGIOutput_D3DImpl(void* _adapter, uint32_t _index, void** _output) noexcept;
#pragma endregion

#pragma region IDXGISwapChain
	uint32_t GetContainingOutput_D3DImpl(void* _swapChain, void** _output) noexcept;
	uint32_t Present_D3DImpl(void* _swapChain) noexcept;
#pragma endregion

#pragma  region IDXGIOutput
	uint32_t GetDXGIOutputDesc_D3DImpl(void* _output, std::wstring* _deviceName, int32_t* _left, int32_t* _on, int32_t* _right, int32_t* _down, bool* _attachedToDesktop) noexcept;
#pragma endregion

#pragma region D3DCompile
	uint32_t D3DCompile_D3DImpl(const void* _data, size_t _dataSize, const char* _sourceName, const void* _shaderMacro, const std::function<std::filesystem::path(const std::filesystem::path&)>& _searchIncludeFileFunc, const char* _entryPoint, const char* _target, int32_t flag1, std::vector<uint8_t>& _shaderBlob, std::vector<uint8_t>& _errorBlob) noexcept;
#pragma endregion
}
