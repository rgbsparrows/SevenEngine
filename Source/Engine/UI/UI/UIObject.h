#pragma once

#include "Core/Math/Type.h"
#include "Core/Class/ClassObject.h"

REWRITE_WHEN_SE_UIMODULE_EVENT_AVAILABLE("补充事件回调的处理")
class SUIObject
{
	DECLARE_ANCESTOR_CLASSOBJECT_BODY(SUIObject)
public:
	Math::SIntRect GetRect() const noexcept { return mRect; }
	void Release() noexcept;

public:
	REWRITE_WHEN_SE_UIMODULE_EVENT_AVAILABLE("这些事件应该都是private/protected的，而后通过事件机制来进行回调")

	DEPRECATED_WHEN_SE_UIMODULE_EVENT_AVAILABLE("应该通过事件机制来进行回调, 而不是直接调用")
	virtual void OnCreated() noexcept {}
	DEPRECATED_WHEN_SE_UIMODULE_EVENT_AVAILABLE("应该通过事件机制来进行回调, 而不是直接调用")
	virtual void OnResize(Math::SIntRect _rect) noexcept { mRect = _rect; }
	DEPRECATED_WHEN_SE_UIMODULE_EVENT_AVAILABLE("应该通过事件机制来进行回调, 而不是直接调用")
	virtual void OnRelease() noexcept {}

private:
	Math::SIntRect mRect;
};
