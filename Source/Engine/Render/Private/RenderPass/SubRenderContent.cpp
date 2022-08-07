#include "Core/Util/UtilMacros.h"
#include "Render/RenderPass/SubRenderContent.h"

void SSubRenderContent::Init(SRenderContent* _renderContent, uint32_t _threadCount) noexcept
{
	mRenderContent = _renderContent;
	mRenderContent->EnsureThreadCount(_threadCount);
}

void SSubRenderContent::Clear() noexcept
{
	TODO("SubRenderContent--Clear")
}
