#include "UI/UIBackPlane.h"
#include "UI/UIContainer.h"

void SUIBackPlane::SetContainer(SUIContainer* _container)noexcept
{
	if (mContainer)
	{
		mContainer->Release();
	}

	mContainer = _container;

	if (mContainer)
	{
		mContainer->OnAddToBackPlane(this);
		mContainer->OnResize(GetRect());
	}
}

void SUIBackPlane::OnRelease() noexcept
{
	mContainer->Release();

	BaseClass::OnRelease();
}
