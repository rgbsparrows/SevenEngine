#pragma once

#include "UI/UIElement.h"

#include <vector>

class SUIBackPlane;

class SUIContainer : public SUIElement
{
	DECLARE_CLASSOBJECT_BODY(SUIContainer, SUIElement)
public:
	const std::vector<SUIElement*>& GetElements()const noexcept { return mElememts; };
	uint64_t GetDepthLayer()const noexcept { return mDepthLayer; }
	void EraseElement(SUIElement* _element) noexcept;

protected:
	//子类扩展其他的AddElement方法，并调用这个AddElement以完成相应的实现，这个AddElement并不是一个通用的功能
	//例如现在扩展一个新的container，其以九宫格的方式组织控件，则显然这个AddElement函数并不适用
	void AddElement(SUIElement* _element, Math::SIntRect _rect) noexcept;
	std::vector<SUIElement*>& GetElements()noexcept { return mElememts; }

public:
	REWRITE_WHEN_SE_UIMODULE_EVENT_AVAILABLE("这些事件应该都是private/protected的，而后通过事件机制来进行回调")

	DEPRECATED_WHEN_SE_UIMODULE_EVENT_AVAILABLE("应该通过事件机制来进行回调, 而不是直接调用")
	void OnAddToBackPlane(SUIBackPlane* _backPlane) noexcept;

	DEPRECATED_WHEN_SE_UIMODULE_EVENT_AVAILABLE("应该通过事件机制来进行回调, 而不是直接调用")
	void OnLayoutChange() noexcept {}

	DEPRECATED_WHEN_SE_UIMODULE_EVENT_AVAILABLE("应该通过事件机制来进行回调, 而不是直接调用")
	virtual void OnResize(Math::SIntRect _rect) noexcept { BaseClass::OnResize(_rect); }

	DEPRECATED_WHEN_SE_UIMODULE_EVENT_AVAILABLE("应该通过事件机制来进行回调, 而不是直接调用")
	virtual void OnRelease() noexcept;

private:
	std::vector<SUIElement*> mElememts;

	uint64_t mDepthLayer = 0;
};
