#include "CRTFunctionDeclear.h"
#include "CRTLibraryImplWarper.h"

namespace CRTLibraryWarp_Impl
{
	void Assert(const wchar_t * _message, const wchar_t * _file, uint32_t _line) noexcept
	{
		_wassert(_message, _file, _line);
	}
}
