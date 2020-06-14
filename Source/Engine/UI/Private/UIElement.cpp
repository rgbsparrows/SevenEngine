#include "UIElement.h"
#include "UIContainer.h"
#include "Macros/Assert.h"

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
