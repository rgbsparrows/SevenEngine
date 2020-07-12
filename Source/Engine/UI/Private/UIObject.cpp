#include "UI/UIObject.h"
#include "UIModuleImpl.h"

void SUIObject::Release() noexcept 
{
	OnRelease();
	GetUIModuleImpl()->ReleaseUIObject(this);
}
