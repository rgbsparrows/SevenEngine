#include "ResourceModuleImpl.h"
#include "Resource/ResourceBase.h"

void SResourceBase::Save() noexcept
{
	GetResourceModuleImpl()->SaveResource(this);
	mIsDirty = false;
}

void SResourceBase::Release() noexcept
{
	if (mRefCount == 0)
		OnRelease();
	else
		--mRefCount;
}

void SResourceBase::OnRelease() noexcept
{
	if(mResourcePath.empty() == false)
		GetResourceModuleImpl()->RemoveResource(mResourcePath);

	delete this;
}
