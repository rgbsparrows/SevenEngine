 #pragma once

#include "UIObject.h"

class SUIContainer;

class SUIElement : public SUIObject
{
	DECLARE_CLASSOBJECT_BODY(SUIElement, SUIObject)
public:
	const SUIContainer* GetContainer()const noexcept { return mContainer; }
	uint64_t GetSortKey()const noexcept { return mSortKey; }

	void SetSortKey(uint64_t _sortKey) noexcept;

public:
	REWRITE_WHEN_SE_UIMODULE_EVENT_AVAILABLE("这些事件应该都是private/protected的，而后通过事件机制来进行回调")
	
	DEPRECATED_WHEN_SE_UIMODULE_EVENT_AVAILABLE("应该通过事件机制来进行回调, 而不是直接调用")
	void OnAddToContainer(SUIContainer* _container) noexcept;

private:
	SUIContainer* mContainer = nullptr;

	uint64_t mSortKey = 0;
};
