#pragma once

#include "Core/Util/TemplateUtil.h"

#include <filesystem>
#include <functional>

#include "Core/Misc/PreWindowsApi.h"
#include <d3dcompiler.h>
#include "Core/Misc/PostWindowsApi.h"

#pragma comment(lib, "d3dcompiler.lib")

struct SRDIErrorInfo;

void GenerateErrorInfo(ID3DBlob* _errorBlob, SRDIErrorInfo* _errorInfo);

enum class ED3DShaderTarget
{
	VS,
	HS,
	DS,
	GS,
	PS,
	CS,

	Num,
	Error = TErrorEnumValue<ED3DShaderTarget>
};

class D3DInclude : public ID3DInclude
{
public:
	void Init(const std::function<std::filesystem::path(const std::filesystem::path&)>& _searchIncludeFileFunc) noexcept
	{
		mSearchIncludeFileFunc = _searchIncludeFileFunc;
	}

	HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
	{
		*ppData = nullptr;
		*pBytes = 0;

		std::filesystem::path filePath = mSearchIncludeFileFunc(pFileName);

		if (std::filesystem::exists(filePath))
		{
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
		}
		return S_OK;
	}

	HRESULT Close(LPCVOID pData) override
	{
		operator delete(const_cast<void*>(pData), std::nothrow);
		return S_OK;
	}

	std::function<std::filesystem::path(const std::filesystem::path&)> mSearchIncludeFileFunc;
};
