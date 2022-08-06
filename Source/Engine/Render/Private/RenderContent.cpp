#include "Render/RenderContent.h"
#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDIDevice.h"

void SRenderContent::Init(IRDIDevice* _device, IRDICommandQueue* _commandQueue, size_t _threadCount) noexcept
{
	_threadCount = std::min<size_t>(1, _threadCount);

	mRDIDevice = _device;
	mRDICommandQueue = _commandQueue;

	for (size_t i = 0; i != _threadCount; ++i)
		mCommandAllocatorList.push_back(mRDIDevice->CreateCommandAllocator(ERDICommandListType::Direct));
}
