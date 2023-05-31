#pragma once

#include "Engine/Classes/Component.h"
#include "Render/RenderProxy/RenderProxy.h"

class ACRenderComponent : public ACComponent
{
	DECLARE_CLASSOBJECT_BODY(ACRenderComponent, ACComponent)

public:
	void MarkRenderProxyDirty() noexcept
	{
		mRenderProxyDirtyFlag.MarkDirty();
	}

	void LateUpdateRenderProxy() noexcept
	{
		mRenderProxyDirtyFlag.Update();
	}

	bool IsRenderProxyDirty() const noexcept
	{
		return mRenderProxyDirtyFlag.IsDirty();
	}
	
private:
	RDirtyFlag mRenderProxyDirtyFlag;
};
