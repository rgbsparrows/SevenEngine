#pragma once

#include "UI/UIObject.h"

class SUIContainer;

class SUIBackPlane : public SUIObject
{
	DECLARE_CLASSOBJECT_BODY(SUIBackPlane, SUIObject);

public:
	void SetContainer(SUIContainer* _container)noexcept;

	SUIContainer* GetContainer()noexcept { return mContainer; }
	const SUIContainer* GetContainer()const noexcept { return mContainer; }

public:
	DEPRECATED_WHEN_SE_UIMODULE_EVENT_AVAILABLE("应该通过事件机制来进行回调, 而不是直接调用")
	virtual void OnRelease() noexcept;

private:
	SUIContainer* mContainer = nullptr;
};
