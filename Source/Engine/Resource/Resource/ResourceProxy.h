#pragma once

#include "Resource/ResourceBase.h"
#include "Core/Util/TemplateUtil.h"

struct SResourceProxyBase
{
	explicit SResourceProxyBase(SResourceBase* _resource) noexcept;

	SResourceProxyBase() noexcept = default;
	SResourceProxyBase(const SResourceProxyBase& _other) noexcept;
	SResourceProxyBase(SResourceProxyBase&& _other) noexcept;
	~SResourceProxyBase() noexcept;

	void operator=(const SResourceProxyBase& _other) noexcept;
	void operator=(SResourceProxyBase&& _other) noexcept;

	SResourceBase& operator*() noexcept { return *Get(); }
	SResourceBase* operator->() noexcept { return Get(); }

	const SResourceBase& operator*() const noexcept { return *Get(); }
	const SResourceBase* operator->() const noexcept { return Get(); }

	bool operator!() const noexcept { return mResource == nullptr; }
	operator bool() const noexcept { return !!*this; }

	SResourceBase* Get() noexcept { return mResource; }
	const SResourceBase* Get() const noexcept { return mResource; }

private:
	SResourceBase* mResource = nullptr;
};

template<std::derived_from<SResourceBase> _resourceType>
struct TResourceProxy : public SResourceProxyBase
{
	using ResourceType = _resourceType;

	template<typename... _argts>
	static TResourceProxy New(_argts&&... _args)noexcept
	{
		return TResourceProxy(new ResourceType(std::forward<_argts...>(_args)...));
	}

	explicit TResourceProxy(ResourceType* _resource) noexcept :SResourceProxyBase(_resource) {}

	TResourceProxy() noexcept = default;
	TResourceProxy(const TResourceProxy& _other) noexcept :SResourceProxyBase(_other) {}
	TResourceProxy(TResourceProxy&& _other) noexcept :SResourceProxyBase(std::forward<TResourceProxy>(_other)) {}
	explicit TResourceProxy(const SResourceProxyBase& _other) noexcept :SResourceProxyBase(_other) {}
	explicit TResourceProxy(SResourceProxyBase&& _other) noexcept :SResourceProxyBase(std::forward<SResourceProxyBase>(_other)) {}

	template<std::derived_from<ResourceType> _subResourceType> TResourceProxy(const TResourceProxy<_subResourceType>& _other) noexcept :SResourceProxyBase(_other) {}
	template<std::derived_from<ResourceType> _subResourceType> TResourceProxy(TResourceProxy<_subResourceType>&& _other) noexcept :SResourceProxyBase(std::forward(_other)) {}

	template<CBaseOf<ResourceType> _baseResourceType> explicit TResourceProxy(const TResourceProxy<_baseResourceType>& _other) noexcept :SResourceProxyBase(_other) {}
	template<CBaseOf<ResourceType> _baseResourceType> explicit TResourceProxy(TResourceProxy<_baseResourceType>&& _other) noexcept :SResourceProxyBase(std::forward(_other)) {}

	void operator=(const SResourceProxyBase& _other) noexcept = delete;
	void operator=(SResourceProxyBase&& _other) noexcept = delete;

	void operator=(const TResourceProxy& _other) noexcept
	{
		~TResourceProxy();
		new(this) TResourceProxy(_other);
	}

	void operator=(TResourceProxy&& _other) noexcept
	{
		~TResourceProxy();
		new(this) TResourceProxy(std::forward<TResourceProxy>(_other));
	}

	template<std::derived_from<ResourceType> _subResourceType> void operator=(const TResourceProxy<_subResourceType>& _other) noexcept
	{
		~TResourceProxy();
		new(this) TResourceProxy(_other);
	}

	template<std::derived_from<ResourceType> _subResourceType> void operator=(TResourceProxy<_subResourceType>&& _other) noexcept
	{
		~TResourceProxy();
		new(this) TResourceProxy(std::forward<TResourceProxy<_subResourceType>>(_other));
	}

	ResourceType& operator*() noexcept { return *Get(); }
	ResourceType* operator->() noexcept { return Get(); }

	const ResourceType& operator*() const noexcept { return *Get(); }
	const ResourceType* operator->() const noexcept { return Get(); }

	ResourceType* Get() noexcept { return static_cast<ResourceType*>(SResourceProxyBase::Get()); }
	const ResourceType* Get() const noexcept { return static_cast<const ResourceType*>(SResourceProxyBase::Get()); }
};
