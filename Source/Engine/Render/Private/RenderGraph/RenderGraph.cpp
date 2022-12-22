#include "Core/Util/Algorithm.h"
#include "Render/RenderGraph/RenderGraph.h"

void RRenderPassBase::AddPrecursorRenderPass(std::initializer_list<RRenderPassBase*> _precursorRenderPassList) noexcept
{
	Append(mPrecursorRenderPassList, _precursorRenderPassList);
	Sort(mPrecursorRenderPassList);
	DedupForSortedRange(mPrecursorRenderPassList);
}

void RRenderPassBase::RemovePrecursorRenderPass(std::initializer_list<RRenderPassBase*> _precursorRenderPassList) noexcept
{
	for (auto it = _precursorRenderPassList.begin(); it != _precursorRenderPassList.end(); ++it)
	{
		auto eraseIt = Find(mPrecursorRenderPassList, *it);
		if (eraseIt != mPrecursorRenderPassList.end())
			mPrecursorRenderPassList.erase(eraseIt);
	}
}

struct SRenderPackageInfo;

struct SRenderPassInfo
{
	RRenderPassBase* mRenderPass = nullptr;
	SRenderPackageInfo* mRenderPackae = nullptr;
	std::vector<RRenderPassBase*> mPrecursorRenderPass;
	std::vector<RRenderPassBase*> mSuccessorRenderPass;
};

struct SRenderPackageInfo
{
	std::vector<SRenderPassInfo*> mRenderPassList;
	std::vector<SRenderPackageInfo*> mPrecursorRenderPackage;
	std::vector<SRenderPackageInfo*> mSuccessorRenderPackage;
};

bool CheckRenderGraphValid(const std::vector<RRenderPassBase*>& _renderPassList, SClassIdentifier _renderPassBaseClassHash) noexcept
{
	if (_renderPassList.size() == 0)
		return true;

	if (Has(_renderPassList, nullptr))
		return false;

	if (HasIf(_renderPassList, [&](RRenderPassBase* _renderPass) { return IsInstanceOf(_renderPass, _renderPassBaseClassHash) == false; }))
		return false;

	//检查前继RenderPass是否都在列表内
	for (RRenderPassBase* _renderPass : _renderPassList)
	{
		for (RRenderPassBase* precursorRenderPass : _renderPass->GetPrecursorRenderPassList())
			if (Has(_renderPassList, precursorRenderPass) == false)
				return false;
	}

	//检查是否存在重复的RenderPassBaseClass
	{
		std::vector<RRenderPassBase*> tempRenderPassBase = _renderPassList;
		Sort(tempRenderPassBase);
		DedupForSortedRange(tempRenderPassBase);
		if (tempRenderPassBase.size() != _renderPassList.size())
			return false;
	}

	//拓扑排序（这不是正确的拓扑排序算法，但写起来更方便，并且也可以验证图中是否存在环）
	{
		std::vector<SRenderPassInfo> renderPassInfoList(_renderPassList.size());
		for (size_t i = 0; i != _renderPassList.size(); ++i)
		{
			renderPassInfoList[i].mRenderPass = _renderPassList[i];
			renderPassInfoList[i].mPrecursorRenderPass = _renderPassList[i]->GetPrecursorRenderPassList();
		}

		do
		{
			for (auto it = renderPassInfoList.begin(); it != renderPassInfoList.end(); void())
			{
				if (it->mPrecursorRenderPass.size() == 0)
				{
					for (SRenderPassInfo& renderPassInfo : renderPassInfoList)
						Erase(renderPassInfo.mPrecursorRenderPass, it->mRenderPass);
					it = renderPassInfoList.erase(it);
				}
				else
					++it;
			}

		} while (HasIf(renderPassInfoList, [](const SRenderPassInfo& _renderPassInfo) {return _renderPassInfo.mPrecursorRenderPass.empty(); }));

		if (renderPassInfoList.size() != 0)
			return false;
	}

	return true;
}

bool ReorganizeRenderPass(std::vector<RRenderPassBase*> _renderPassList, SClassIdentifier _renderPassBaseClassHash, std::vector<RRenderPassBase*>& _sortedRenderPassList, std::vector<uint32_t>& _renderPackageGroup, std::vector<uint32_t>& _syncPointForRenderPackage) noexcept
{
	TODO("当需要构建包含子Pass的RenderGraph时，需要追加拆解RenedrGraph的函数，但函数主体内容不需要变更");
	_sortedRenderPassList.resize(0);
	_syncPointForRenderPackage.resize(0);
	_renderPackageGroup.resize(0);

	if (_renderPassList.empty())
		return true;

	if (CheckRenderGraphValid(_renderPassList, _renderPassBaseClassHash) == false)
		return false;

	std::vector<SRenderPassInfo> renderPassInfoList(_renderPassList.size());
	for (size_t i = 0; i != _renderPassList.size(); ++i)
	{
		renderPassInfoList[i].mRenderPass = _renderPassList[i];
		renderPassInfoList[i].mPrecursorRenderPass = _renderPassList[i]->GetPrecursorRenderPassList();
	}

	//拓扑排序
	{
		std::vector<SRenderPassInfo> tempRenderPassInfoList = renderPassInfoList;
		std::vector<RRenderPassBase*> tempRenderPassList;
		do
		{
			std::vector<RRenderPassBase*> removedRenderPass;

			for (auto it = tempRenderPassInfoList.begin(); it != tempRenderPassInfoList.end(); void())
			{
				if (it->mPrecursorRenderPass.size() == 0)
				{
					tempRenderPassList.push_back(it->mRenderPass);
					removedRenderPass.push_back(it->mRenderPass);
					it = tempRenderPassInfoList.erase(it);
				}
				else
					++it;
			}

			for (RRenderPassBase* _renderPass : removedRenderPass)
			{
				for (SRenderPassInfo& renderPassInfo : tempRenderPassInfoList)
					Erase(renderPassInfo.mPrecursorRenderPass, _renderPass);
			}

		} while (tempRenderPassInfoList.empty() == false);
		Sort(renderPassInfoList, [&](const SRenderPassInfo& _left, const SRenderPassInfo& _right) {return FindIndex(tempRenderPassList, _left.mRenderPass) < FindIndex(tempRenderPassList, _right.mRenderPass); });
	}

	//化简
	{
		for (SRenderPassInfo& _renderPassInfo : renderPassInfoList)
		{
			std::vector<RRenderPassBase*> precursorRenderPassList = _renderPassInfo.mPrecursorRenderPass;
			for (RRenderPassBase* _precursorRenderPass : _renderPassInfo.mPrecursorRenderPass)
			{
				std::vector<RRenderPassBase*>& ppRenderPassList = _precursorRenderPass->GetPrecursorRenderPassList();
				Append(precursorRenderPassList, ppRenderPassList);
			}
			_renderPassInfo.mPrecursorRenderPass = precursorRenderPassList;
		}

		for (SRenderPassInfo& _renderPassInfo : renderPassInfoList)
		{
			Sort(_renderPassInfo.mPrecursorRenderPass);
			DedupForSortedRange(_renderPassInfo.mPrecursorRenderPass);
		}

		for (auto it = renderPassInfoList.rbegin(); it != renderPassInfoList.rend(); ++it)
		{
			SRenderPassInfo& renderPassInfo = *it;
			std::vector<RRenderPassBase*> fullPpRenderPassList;

			for (RRenderPassBase* precursorPass : renderPassInfo.mPrecursorRenderPass)
			{
				std::vector<RRenderPassBase*>& ppRenderPassList = precursorPass->GetPrecursorRenderPassList();
				Append(fullPpRenderPassList, ppRenderPassList);
			}

			Sort(fullPpRenderPassList);
			DedupForSortedRange(fullPpRenderPassList);

			EraseIf(renderPassInfo.mPrecursorRenderPass, [&](RRenderPassBase* _renderPass) {return Has(fullPpRenderPassList, _renderPass); });
		}

		for (SRenderPassInfo& renderPassInfo : renderPassInfoList)
		{
			Sort(renderPassInfo.mPrecursorRenderPass);
			DedupForSortedRange(renderPassInfo.mPrecursorRenderPass);
		}
	}

	//生成后继Pass
	for (SRenderPassInfo& renderPassInfo : renderPassInfoList)
	{
		for (RRenderPassBase* precursorRenderPass : renderPassInfo.mPrecursorRenderPass)
		{
			auto precursorRenderPassInfo = FindIf(renderPassInfoList, [&](SRenderPassInfo& _renderPassInfo) {return _renderPassInfo.mRenderPass == precursorRenderPass; });
			precursorRenderPassInfo->mSuccessorRenderPass.push_back(renderPassInfo.mRenderPass);
		}
	}

	//生成RenderPackage
	std::vector<SRenderPackageInfo> renderPackageInfoList;
	{
		for (size_t i = 0; i != renderPassInfoList.size(); ++i)
		{
			if (renderPassInfoList[i].mRenderPackae != nullptr)
				continue;

			auto getSuccessorRenderPassIndex = [&](SRenderPassInfo& _renderPassInfo)
			{
				if (_renderPassInfo.mSuccessorRenderPass.size() != 1)
					return SIZE_MAX;

				size_t successorRenderPassIndex = FindIndexIf(renderPassInfoList, [&](const SRenderPassInfo& _renderPassInfo) {return _renderPassInfo.mRenderPass == _renderPassInfo.mSuccessorRenderPass[0]; });
				SRenderPassInfo& successorRenderPassInfo = renderPassInfoList[successorRenderPassIndex];

				if (successorRenderPassInfo.mPrecursorRenderPass.size() != 1)
					return SIZE_MAX;

				return successorRenderPassIndex;
			};

			renderPackageInfoList.push_back(SRenderPackageInfo());
			for (size_t renderPassIndex = i; renderPassIndex != SIZE_MAX; renderPassIndex = getSuccessorRenderPassIndex(renderPassInfoList[renderPassIndex]))
			{
				renderPackageInfoList.back().mRenderPassList.push_back(&renderPassInfoList[renderPassIndex]);
				renderPassInfoList[renderPassIndex].mRenderPackae = &renderPackageInfoList.back();
			}
		}

		for (SRenderPackageInfo& renderPackageInfo : renderPackageInfoList)
		{
			std::vector<RRenderPassBase*> precursorRenderPassList;
			std::vector<RRenderPassBase*> successorRenderPassList;

			for (SRenderPassInfo* renderPassInfo : renderPackageInfo.mRenderPassList)
			{
				Append(precursorRenderPassList, renderPassInfo->mPrecursorRenderPass);
				Append(successorRenderPassList, renderPassInfo->mSuccessorRenderPass);
			}

			for (RRenderPassBase* renderPass : precursorRenderPassList)
			{
				auto it = FindIf(
					renderPackageInfoList,
					[&](SRenderPackageInfo& _renderPackageInfo)
					{
						return HasIf(
							_renderPackageInfo.mRenderPassList,
							[&](SRenderPassInfo* _renderPassInfo)
							{
								return _renderPassInfo->mRenderPass == renderPass;
							});
					});

				renderPackageInfo.mPrecursorRenderPackage.push_back(&*it);
			}

			for (RRenderPassBase* renderPass : successorRenderPassList)
			{
				auto it = FindIf(
					renderPackageInfoList,
					[&](SRenderPackageInfo& _renderPackageInfo)
					{
						return HasIf(
							_renderPackageInfo.mRenderPassList,
							[&](SRenderPassInfo* _renderPassInfo)
							{
								return _renderPassInfo->mRenderPass == renderPass;
							});
					});

				renderPackageInfo.mSuccessorRenderPackage.push_back(&*it);
			}

			Erase(renderPackageInfo.mPrecursorRenderPackage, &renderPackageInfo);
			Erase(renderPackageInfo.mSuccessorRenderPackage, &renderPackageInfo);

			Sort(renderPackageInfo.mPrecursorRenderPackage);
			Sort(renderPackageInfo.mSuccessorRenderPackage);

			DedupForSortedRange(renderPackageInfo.mPrecursorRenderPackage);
			DedupForSortedRange(renderPackageInfo.mSuccessorRenderPackage);
		}
	}

	//对RenderPackage进行拓扑排序
	std::vector<std::vector<SRenderPackageInfo*>> renderGroupList;
	{
		std::vector<SRenderPackageInfo*> tempRenderPackageInfoList;

		for (SRenderPackageInfo& _renderPackageInfo : renderPackageInfoList)
			tempRenderPackageInfoList.push_back(&_renderPackageInfo);

		do
		{
			renderGroupList.push_back(std::vector<SRenderPackageInfo*>());

			std::vector<SRenderPackageInfo*> removedRenderPackage;

			for (auto it = tempRenderPackageInfoList.begin(); it != tempRenderPackageInfoList.end(); void())
			{
				if ((*it)->mPrecursorRenderPackage.size() == 0)
				{
					renderGroupList.back().push_back(*it);
					removedRenderPackage.push_back(*it);
					it = tempRenderPackageInfoList.erase(it);
				}
				else
					++it;
			}

			for (SRenderPackageInfo* renderPackageInfo : removedRenderPackage)
			{
				for (SRenderPackageInfo& renderPackageInfo : renderPackageInfoList)
					Erase(renderPackageInfo.mPrecursorRenderPackage, &renderPackageInfo);
			}

			EraseIf(tempRenderPackageInfoList, [&](SRenderPackageInfo* _renderPackageInfo) {return Has(removedRenderPackage, _renderPackageInfo); });

		} while (tempRenderPackageInfoList.empty() == false);
	}

	//缩略无用的RenderPackage，生成紧凑的同步点
	{
		uint32_t renderPackageIndex = 0;
		for (std::vector<SRenderPackageInfo*>& renderGroup : renderGroupList)
		{
			for (SRenderPackageInfo* renderPackage : renderGroup)
			{
				bool hasGpuRenderPass = false;

				for (SRenderPassInfo* renderPassInfo : renderPackage->mRenderPassList)
				{
					_sortedRenderPassList.push_back(renderPassInfo->mRenderPass);

					bool isEmptyCommandPass = renderPassInfo->mRenderPass->IsEmptyCommandRenderPass();
					_renderPackageGroup.push_back(isEmptyCommandPass ? UINT32_MAX : renderPackageIndex);
					hasGpuRenderPass = hasGpuRenderPass || !isEmptyCommandPass;
				}

				if (hasGpuRenderPass)
					++renderPackageIndex;
			}

			_syncPointForRenderPackage.push_back(renderPackageIndex);
		}

		DedupForSortedRange(_syncPointForRenderPackage);
	}

	return true;
}
