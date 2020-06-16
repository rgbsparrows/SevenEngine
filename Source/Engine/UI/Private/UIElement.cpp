#include "UI/UIElement.h"
#include "UI/UIContainer.h"
#include "Core/Macros/Assert.h"

void SUIElement::SetSortKey(uint64_t _sortKey) noexcept
{
	ASSERT(_sortKey < 16);

	if (mContainer != nullptr)
		mSortKey = (_sortKey << (mContainer->GetDepthLayer() * 4)) + mContainer->GetSortKey();
	else mSortKey = _sortKey;
}

void SUIElement::OnAddToContainer(SUIContainer* _container) noexcept
{
	mContainer = _container;
}
