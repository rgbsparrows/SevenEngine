#include "UI/UIContainer.h"

void SUIContainer::EraseElement(SUIElement* _element) noexcept
{
	auto it = std::find(mElememts.begin(), mElememts.end(), _element);
	mElememts.erase(it);

	OnLayoutChange();
}

void SUIContainer::AddElement(SUIElement* _element, Math::SIntRect _rect) noexcept
{
	mElememts.push_back(_element);
	_element->OnAddToContainer(this);
}

void SUIContainer::OnAddToBackPlane(SUIBackPlane* _backPlane) noexcept
{
	SetSortKey(0);
}

void SUIContainer::OnRelease() noexcept
{
	for (auto _ele : mElememts)
		_ele->Release();

	mElememts.clear();

	BaseClass::OnRelease();
}

