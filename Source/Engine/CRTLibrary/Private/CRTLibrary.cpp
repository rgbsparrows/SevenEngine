#include "CRTLibraryImplWarper.h"
#include "CRTLibrary/CRTLibrary.h"


void SCRTLibrary::Assert(const wchar_t* _message, const wchar_t* _file, uint32_t _line) noexcept
{
	CRTLibraryWarp_Impl::Assert(_message, _file, _line);
}
