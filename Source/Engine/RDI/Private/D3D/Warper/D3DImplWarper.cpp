#include "D3DImplWarper.h"
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#pragma  comment(lib, "d3dcompiler.lib")

using SD3D12NativeDXGIFactoryType = IDXGIFactory;
using SD3D12NativeSwapChainType = IDXGISwapChain;
using SD3D12NativeAdapterType = IDXGIAdapter;
using SD3D12NativeOutputType = IDXGIOutput;

namespace
{
	class D3DInclude : public ID3DInclude
	{
	public:
		D3DInclude(const std::function<std::filesystem::path(const std::filesystem::path&)>& _searchIncludeFileFunc) noexcept
		{
			mSearchIncludeFileFunc = _searchIncludeFileFunc;
		}

		HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
		{
			*ppData = nullptr;
			*pBytes = 0;

			std::filesystem::path filePath = mSearchIncludeFileFunc(pFileName);
			FILE* fp = nullptr;
			errno_t err = _wfopen_s(&fp, filePath.c_str(), L"rb");
			if (fp != nullptr)
			{
				void* fileBuffer = nullptr;
				int fileLen = 0;

				fseek(fp, 0, SEEK_END);
				fileLen = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				fileBuffer = operator new(fileLen, std::nothrow);

				if (fileBuffer == nullptr)
				{
					fclose(fp);
					return S_OK;
				}

				fread_s(fileBuffer, fileLen, fileLen, 1, fp);

				fclose(fp);

				*ppData = fileBuffer;
				*pBytes = fileLen;
			}

			return S_OK;
		}

		HRESULT Close(LPCVOID pData) override
		{
			operator delete(const_cast<void*>(pData), std::nothrow);
			return S_OK;
		}

		std::function<std::filesystem::path(std::filesystem::path)> mSearchIncludeFileFunc;
	};
}

namespace D3DAPIWarp_Impl
{
	uint32_t CreateDXGIFactory_D3DImpl(bool _isDebugMode, void** _dxgiFactory) noexcept
	{
		IDXGIFactory7* dxgiFactory = nullptr;
		uint32_t res = CreateDXGIFactory2(_isDebugMode ? DXGI_CREATE_FACTORY_DEBUG : 0, IID_PPV_ARGS(&dxgiFactory));
		*_dxgiFactory = dxgiFactory;
		return res;
	}

	uint32_t EnumDXGIAdapter_D3DImpl(void* _dxgiFactory, uint32_t _index, void** _adapter) noexcept
	{
		SD3D12NativeDXGIFactoryType* dxgiFactory = static_cast<SD3D12NativeDXGIFactoryType*>(_dxgiFactory);
		IDXGIAdapter* adapter = nullptr;
		uint32_t res = dxgiFactory->EnumAdapters(_index, &adapter);
		*_adapter = static_cast<SD3D12NativeAdapterType*>(adapter);
		return res;
	}

	uint32_t EnumDXGIWarpAdapter_D3DImpl(void* _dxgiFactory, void** _warpAdapter) noexcept
	{
		SD3D12NativeDXGIFactoryType* dxgiFactory = static_cast<SD3D12NativeDXGIFactoryType*>(_dxgiFactory);
		SD3D12NativeAdapterType* adapter = nullptr;
		uint32_t res = static_cast<IDXGIFactory4*>(dxgiFactory)->EnumWarpAdapter(IID_PPV_ARGS(&adapter));
		*_warpAdapter = adapter;
		return res;
	}

	uint32_t GetDXGIAdapterDesc_D3DImpl(void* _adapter, std::wstring* _description, uint64_t* _dedicatedVideoMemory, uint64_t* _dedicatedSystemMemory, uint64_t* _sharedSystemMemory) noexcept
	{
		SD3D12NativeAdapterType* adapter = static_cast<SD3D12NativeAdapterType*>(_adapter);
		DXGI_ADAPTER_DESC desc;
		uint32_t res = adapter->GetDesc(&desc);
		if (_description) *_description = desc.Description;
		if (_dedicatedVideoMemory) *_dedicatedVideoMemory = desc.DedicatedVideoMemory;
		if (_dedicatedVideoMemory) *_dedicatedSystemMemory = desc.DedicatedSystemMemory;
		if (_sharedSystemMemory) *_sharedSystemMemory = desc.SharedSystemMemory;
		return res;
	}

	uint32_t EnumDXGIOutput_D3DImpl(void* _adapter, uint32_t _index, void** _output) noexcept
	{
		SD3D12NativeAdapterType* adapter = static_cast<SD3D12NativeAdapterType*>(_adapter);
		IDXGIOutput* output = nullptr;
		uint32_t res = adapter->EnumOutputs(_index, &output);
		*_output = static_cast<SD3D12NativeOutputType*>(output);
		return res;
	}

	uint32_t GetContainingOutput_D3DImpl(void* _swapChain, void** _output) noexcept
	{
		SD3D12NativeSwapChainType* swapChain = static_cast<SD3D12NativeSwapChainType*>(_swapChain);

		IDXGIOutput* output;
		HRESULT res = swapChain->GetContainingOutput(&output);
		return res;
	}

	uint32_t Present_D3DImpl(void* _swapChain) noexcept
	{
		SD3D12NativeSwapChainType* swapChain = static_cast<SD3D12NativeSwapChainType*>(_swapChain);

		HRESULT res = swapChain->Present(0, 0);
		return res;
	}

	uint32_t GetDXGIOutputDesc_D3DImpl(void* _output, std::wstring* _deviceName, int32_t* _left, int32_t* _on, int32_t* _right, int32_t* _down, bool* _attachedToDesktop) noexcept
	{
		SD3D12NativeOutputType* output = static_cast<SD3D12NativeOutputType*>(_output);
		DXGI_OUTPUT_DESC desc;
		uint32_t res = output->GetDesc(&desc);
		if (_deviceName) *_deviceName = desc.DeviceName;
		if (_left) *_left = desc.DesktopCoordinates.left;
		if (_on) *_on = desc.DesktopCoordinates.top;
		if (_right) *_right = desc.DesktopCoordinates.right;
		if (_down) *_down = desc.DesktopCoordinates.bottom;
		if (_attachedToDesktop) *_attachedToDesktop = desc.AttachedToDesktop;
		return res;
	}

	uint32_t D3DCompile_D3DImpl(const void* _data, size_t _dataSize, const char* _sourceName, const void* _shaderMacro, const std::function<std::filesystem::path(const std::filesystem::path&)>& _searchIncludeFileFunc, const char* _entryPoint, const char* _target, int32_t flag1, std::vector<uint8_t>& _shaderBlob, std::vector<uint8_t>& _errorBlob) noexcept
	{
		_shaderBlob.clear();
		_errorBlob.clear();

		D3DInclude d3DInclude(_searchIncludeFileFunc);

		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		HRESULT res = D3DCompile(_data, _dataSize, _sourceName, static_cast<const D3D_SHADER_MACRO*>(_shaderMacro), &d3DInclude, _entryPoint, _target, flag1, 0, &shaderBlob, &errorBlob);

		if (shaderBlob != nullptr)
		{
			_shaderBlob.resize(shaderBlob->GetBufferSize());
			memcpy_s(&_shaderBlob[0], _shaderBlob.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
			shaderBlob->Release();
		}

		if (errorBlob != nullptr)
		{
			_errorBlob.resize(errorBlob->GetBufferSize());
			memcpy_s(&_errorBlob[0], _errorBlob.size(), errorBlob->GetBufferPointer(), errorBlob->GetBufferSize());
			errorBlob->Release();
		}

		return res;
	}
}
