#include "D3DHelper.h"
#include "RDI/RDIFunctionHelper.h"

void GenerateErrorInfo(ID3DBlob* _errorBlob, SRDIErrorInfo* _errorInfo)
{
	if (_errorInfo == nullptr)
		return;

	if (_errorBlob == nullptr || _errorBlob->GetBufferSize() == 0)
	{
		_errorInfo->mParsedErrorString.clear();
		_errorInfo->mErrorString.clear();
		return;
	}

	std::string compiledError = reinterpret_cast<const char*>(_errorBlob->GetBufferPointer());
	_errorInfo->mErrorString = compiledError;
	const char* errorStr = _errorInfo->mErrorString.c_str();

	size_t begin = 0;
	while (true)
	{
		size_t end = _errorInfo->mErrorString.find_first_of(L'\n', begin);
		if (end == std::string::npos)
			break;

		_errorInfo->mParsedErrorString.push_back(std::string_view(errorStr + begin, errorStr + end));
		begin = end + 1;
	}
}
