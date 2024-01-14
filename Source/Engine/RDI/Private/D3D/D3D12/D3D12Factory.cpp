#include "D3D/D3DUtil.h"
#include "D3D/D3D12/D3D12Factory.h"
#include "D3D/Helper/D3DEnumConvertor.h"
#include "Core/ProgramConfiguation/BasicPath.h"
#include "Core/ProgramConfiguation/BuildConfiguation.h"
#include "Core/ProgramConfiguation/ProgramConfiguation.h"

#pragma comment(lib, "dxgi.lib")

#if WITH_DEBUG_CODE
#pragma comment(lib, "dxguid.lib")
#endif

bool SD3D12Factory::Init() noexcept
{
	//DebugLayer
	{
		if constexpr (SBuildConfiguation::GIsDebugMode)
		{
			VERIFY_D3D_RETURN(D3D12GetDebugInterface(IID_PPV_ARGS(&mD3D12DebugNativePtr)));
			GetD3D12DebugNativePtr()->EnableDebugLayer();
			GetD3D12DebugNativePtr()->SetEnableGPUBasedValidation(true);
			GetD3D12DebugNativePtr()->SetEnableSynchronizedCommandQueueValidation(true);
			GetD3D12DebugNativePtr()->SetGPUBasedValidationFlags(D3D12_GPU_BASED_VALIDATION_FLAGS_NONE);
			VERIFY_D3D_RETURN(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&mDXGIDebugNativePtr)));
		}
	}

	//DXGIFactory
	{
		
		VERIFY_D3D_RETURN(CreateDXGIFactory2(SBuildConfiguation::GIsDebugMode ? DXGI_CREATE_FACTORY_DEBUG : 0,IID_PPV_ARGS(&mDXGIFactoryNativePtr)));
	}

	//Adapter
	{
		//Hardware Adapter
		{
			uint32_t res = 0;
			uint32_t index = 0;
			IDXGIAdapter* adapterNativePtr = nullptr;

			for(void(); mDXGIFactoryNativePtr->EnumAdapters(index, &adapterNativePtr) == S_OK; ++index)
			{
				mAdpaters.push_back(SD3D12Adapter());
				mAdpaters.back().Init(adapterNativePtr);
			}
		}

		//Warp Adapter
		if (mAdpaters.empty())
		{
			IDXGIAdapter* warpAdapterNativePtr = nullptr;
			VERIFY_D3D_RETURN(mDXGIFactoryNativePtr->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapterNativePtr)));

			mAdpaters.push_back(SD3D12Adapter());
			mAdpaters.back().Init(warpAdapterNativePtr);
		}

		mCachedAdapters.resize(mAdpaters.size());
		for (size_t i = 0; i != mAdpaters.size(); ++i)
			mCachedAdapters[i] = &mAdpaters[i];
	}

	//Device
	{
		ID3D12Device* d3d12DeviceNativePtr = nullptr;
		D3D12CreateDevice(mAdpaters[0].GetNativePtr(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&d3d12DeviceNativePtr));

		mD3D12Device.Init(d3d12DeviceNativePtr, &mAdpaters[0], this);
	}

	// Shader Compile
	{
		mShaderCompileFlag = D3DCOMPILE_AVOID_FLOW_CONTROL | D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ALL_RESOURCES_BOUND;

		if (SProgramConfiguation::UseDebugShader())
			mShaderCompileFlag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
		else
			mShaderCompileFlag |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

		mD3DInclude.Init(
			[](const std::filesystem::path& _includeFile)
			{
				auto filePath = SBasicPath::GetEngineShaderPath() / _includeFile;
				if (std::filesystem::exists(filePath))
					return filePath;

				filePath = SBasicPath::GetProjectShaderPath() / _includeFile;
				if (std::filesystem::exists(filePath))
					return filePath;

				return std::filesystem::path();
			}
		);
	}

	return true;
}

void SD3D12Factory::Release() noexcept
{
	mDXGIFactoryNativePtr->Release();

	for (SD3D12Adapter& adapter : mAdpaters)
		adapter.Clear();

	mD3D12Device.Clear();

	if constexpr (SBuildConfiguation::GIsDebugMode)
	{
		mDXGIDebugNativePtr->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		mDXGIDebugNativePtr->Release();
	}
}

SBlob SD3D12Factory::CompileVertexShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	return CompileShader(_hlslShader, ED3DShaderTarget::VS, _shaderMacro, _shaderCompileError);
}

SBlob SD3D12Factory::CompileHullShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	return CompileShader(_hlslShader, ED3DShaderTarget::HS, _shaderMacro, _shaderCompileError);
}

SBlob SD3D12Factory::CompileDomainShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	return CompileShader(_hlslShader, ED3DShaderTarget::DS, _shaderMacro, _shaderCompileError);
}

SBlob SD3D12Factory::CompileGeometryShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	return CompileShader(_hlslShader, ED3DShaderTarget::GS, _shaderMacro, _shaderCompileError);
}

SBlob SD3D12Factory::CompilePixelShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	return CompileShader(_hlslShader, ED3DShaderTarget::PS, _shaderMacro, _shaderCompileError);
}

SBlob SD3D12Factory::CompileComputeShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	return CompileShader(_hlslShader, ED3DShaderTarget::CS, _shaderMacro, _shaderCompileError);
}

SBlob SD3D12Factory::CompileShader(SConstBufferView _hlslShader, ED3DShaderTarget _shaderTarget, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept
{
	static D3D_SHADER_MACRO shaderMacro[64] = {};
	static char macroBuffer[64][128] = {};
	static SRDIShaderMacro emptyShaderMacros;

	SBlob shaderBlob;

	if (_shaderMacro == nullptr)
		_shaderMacro = &emptyShaderMacros;

	for (size_t i = 0; i != _shaderMacro->mDefinedMacro.size(); ++i)
	{
		std::string_view macro = _shaderMacro->mDefinedMacro[i];

		for (size_t j = 0; j != macro.size(); ++j)
			macroBuffer[i][j] = macro[j];
		macroBuffer[i][macro.size()] = '\0';

		shaderMacro[i].Name = macroBuffer[i];
		shaderMacro[i].Definition = nullptr;
	}

	shaderMacro[_shaderMacro->mDefinedMacro.size()].Name = nullptr;
	shaderMacro[_shaderMacro->mDefinedMacro.size()].Definition = nullptr;

	ID3DBlob* shaderD3DBlob;
	ID3DBlob* errorD3DBlob;
	HRESULT res = D3DCompile(_hlslShader.GetBuffer(), _hlslShader.GetBufferSize(), nullptr, shaderMacro, &mD3DInclude, GetShaderEntryPoint(_shaderTarget), ConvertShaderTargetToStr(_shaderTarget), mShaderCompileFlag, 0, &shaderD3DBlob, &errorD3DBlob);

	if (res == 0)
		shaderBlob = SBlob(shaderD3DBlob->GetBufferPointer(), shaderD3DBlob->GetBufferSize());
	else
		GenerateErrorInfo(errorD3DBlob, _shaderCompileError);

	if (shaderD3DBlob != nullptr)
		shaderD3DBlob->Release();
	if (errorD3DBlob != nullptr)
		errorD3DBlob->Release();

	return shaderBlob;
}
