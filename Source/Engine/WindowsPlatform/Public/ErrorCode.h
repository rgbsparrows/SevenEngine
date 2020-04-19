#pragma once

#include <stdint.h>
#include <string>

enum class EErrorCode : uint32_t
{
	ERROR_SUCCESS = 0,

};

struct SErrorCode final
{
	SErrorCode() noexcept :mErrorCode(EErrorCode::ERROR_SUCCESS) {}
	SErrorCode(EErrorCode _errorCode) noexcept :mErrorCode(_errorCode) {}

	std::wstring GetDesc()const noexcept;
	EErrorCode GetErrorCode()const noexcept { return mErrorCode; }
	void Clear() noexcept { mErrorCode = EErrorCode::ERROR_SUCCESS; }
	bool IsSuccess()const noexcept { return mErrorCode == EErrorCode::ERROR_SUCCESS; }

	EErrorCode mErrorCode = EErrorCode::ERROR_SUCCESS;
};
