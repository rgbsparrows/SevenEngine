#include "Resource/ResourceProxy.h"

SResourceProxyBase::SResourceProxyBase(SResourceBase* _resource) noexcept
{
	mResource = _resource;
	if (mResource)
		mResource->Internal_AddRef();
}

SResourceProxyBase::SResourceProxyBase(const SResourceProxyBase& _other) noexcept
{
	mResource = _other.mResource;
	if (mResource)
		mResource->Internal_AddRef();
}

SResourceProxyBase::SResourceProxyBase(SResourceProxyBase&& _other) noexcept
{
	mResource = _other.mResource;
	_other.mResource = nullptr;
}

SResourceProxyBase::~SResourceProxyBase() noexcept
{
	if (mResource)
	{
		mResource->Release();
		mResource = nullptr;
	}
}

void SResourceProxyBase::operator=(const SResourceProxyBase& _other) noexcept
{
	this->~SResourceProxyBase();
	new(this) SResourceProxyBase(_other);
}

void SResourceProxyBase::operator=(SResourceProxyBase&& _other) noexcept
{
	this->~SResourceProxyBase();
	new(this) SResourceProxyBase(std::forward<SResourceProxyBase>(_other));
}
