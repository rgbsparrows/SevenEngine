#pragma once

#include "Core/Misc/Thread.h"
#include "Core/Class/ClassObject.h"
#include "RDI/Interface/RDICommandList.h"
#include "Render/RenderGraph/SubRenderContent.h"

#include <vector>
#include <thread>
#include <functional>
#include <initializer_list>

__interface IRDIDevice;
__interface IRDICommandQueue;
__interface IRDICommandList;

class SSubRenderContent;

class RRenderPassBase
{
	DECLARE_ANCESTOR_CLASSOBJECT_BODY(RRenderPassBase);

public:
	void AddPrecursorRenderPass(std::initializer_list<RRenderPassBase*> _precursorRenderPassList) noexcept;
	void RemovePrecursorRenderPass(std::initializer_list<RRenderPassBase*> _precursorRenderPassList) noexcept;

	std::vector<RRenderPassBase*>& GetPrecursorRenderPassList() noexcept { return mPrecursorRenderPassList; }
	const std::vector<RRenderPassBase*>& GetPrecursorRenderPassList() const noexcept { return mPrecursorRenderPassList; }

	virtual bool IsEmptyCommandRenderPass() const noexcept { return false; }

	virtual void Init(SSubRenderContent& _renderContent) noexcept {}
	virtual void Clear(SSubRenderContent& _renderContent) noexcept {}

private:
	std::vector<RRenderPassBase*> mPrecursorRenderPassList;
};

template<typename _renderingDataType>
class TRenderPass : public RRenderPassBase
{
	DECLARE_CLASSOBJECT_BODY(TRenderPass, RRenderPassBase);

public:
	using RRenderingData = _renderingDataType;
	using RRenderPass = TRenderPass<RRenderingData>;

	virtual void Init(SSubRenderContent& _renderContent) noexcept {}
	virtual void Clear(SSubRenderContent& _renderContent) noexcept {}

	virtual void PreRender(RRenderingData& _renderSource, SSubRenderContent& _renderContent) noexcept {}
	virtual void Render(RRenderingData& _renderSource, IRDICommandList* _commandList) noexcept {}
	virtual void PostRender(RRenderingData& _renderSource, SSubRenderContent& _renderContent) noexcept {}
};

bool CheckRenderGraphValid(const std::vector<RRenderPassBase*>& _renderPassList, SClassIdentifier _renderPassBaseClassHash) noexcept;
bool ReorganizeRenderPass(std::vector<RRenderPassBase*> _renderPassList, SClassIdentifier _renderPassBaseClassHash, std::vector<RRenderPassBase*>& _sortedRenderPassList, std::vector<uint32_t>& _renderPackageGroup, std::vector<uint32_t>& _syncPointForRenderPackage) noexcept;

class RRenderGraphBase
{
	DECLARE_ANCESTOR_CLASSOBJECT_BODY(RRenderGraphBase);

public:
	virtual void Init_RenderThread(SRenderContent* _renderContent) noexcept = 0;
	virtual void Clear_RenderThread() noexcept = 0;
};

template<typename _rawRenderingDataType, typename _renderingDataType = _rawRenderingDataType>
class TRenderGraph : public RRenderGraphBase
{
	DECLARE_CLASSOBJECT_BODY(TRenderGraph, RRenderGraphBase);

public:
	using RRawRenderingData = _rawRenderingDataType;
	using RRenderingData = _renderingDataType;
	using RRenderPass = TRenderPass<RRenderingData>;
	using RRenderGraph = TRenderGraph<RRawRenderingData, RRenderingData>;

	class RRenderPackage
	{
	public:
		using RRenderingData = _renderingDataType;
		using RRenderPassBase = TRenderPass<RRenderingData>;

		IRDICommandList* GetCommandList() noexcept { return mRDICommandList; }

		void Setup(std::vector<RRenderPassBase*> _renderPassList) noexcept
		{
			mRenderPassList = _renderPassList;
		}

		void Init(SSubRenderContent& _renderContent) noexcept
		{
			mRDICommandList = _renderContent.AllocateCommandList();
			mRDICommandList->Close();
		}

		void Clear(SSubRenderContent& _renderContent) noexcept
		{
			mRDICommandList->Release();
		}

		void Render(RRenderingData& _renderingData, IRDICommandAllocator* _commandAllocator) noexcept
		{
			if (mRenderPassList.empty())
				return;

			mRDICommandList->Reset(_commandAllocator);

			for (RRenderPassBase* renderPass : mRenderPassList)
				renderPass->Render(_renderingData, mRDICommandList);

			mRDICommandList->Close();
		}

	private:
		std::vector<RRenderPassBase*> mRenderPassList;
		IRDICommandList* mRDICommandList = nullptr;
	};

	void Init_RenderThread(SRenderContent* _renderContent) noexcept override
	{
		uint32_t subThreadCount = 0;

		uint32_t currentRenderPackageIndex = 0;
		for (size_t i = 0; i != mSyncPoint.size(); ++i)
		{
			subThreadCount = std::max(subThreadCount, mSyncPoint[i] - currentRenderPackageIndex);
			currentRenderPackageIndex = mSyncPoint[i];
		}

		subThreadCount = std::min(subThreadCount, std::thread::hardware_concurrency() - 2);

		mRenderContent.Init(_renderContent, subThreadCount);

		for (RRenderPass* renderPass : mRenderPassList)
			renderPass->Init(mRenderContent);

		for (RRenderPackage& renderPackage : mRenderPackageList)
			renderPackage.Init(mRenderContent);

		for (RRenderPackage& renderPackage : mRenderPackageList)
			mRenderPackageCommandList.push_back(renderPackage.GetCommandList());

		for (uint32_t threadIndex = 0; threadIndex != subThreadCount; ++threadIndex)
		{
			std::thread subThread = std::thread(std::mem_fn(&RRenderGraph::SubRenderThreadMain), this, threadIndex);
			mSubRenderThread.push_back(std::move(subThread));
		}
	}

	void Clear_RenderThread() noexcept override
	{
		{
			mRequireExit = true;
			mNewFrameFlag = true;
		}

		for (std::thread& subThread : mSubRenderThread)
			subThread.join();

		for (RRenderPackage& renderPackage : mRenderPackageList)
			renderPackage.Clear(mRenderContent);

		mRenderContent.Clear();
	}

	void Render(RRawRenderingData&& _renderingData) noexcept
	{
		RRenderingData renderingData = ProcessRawRenedrData(std::forward<RRawRenderingData>(_renderingData));

		for (RRenderPass* renderPass : mRenderPassList)
			renderPass->PreRender(renderingData, mRenderContent);

		PrepareRenderPassResource(renderingData, mRenderContent);

		{
			mCurrentRenderingData = &renderingData;

			mExecuatingRenderPackageCount = 0;
			mExecuatedRenderPackageCount = 0;

			mNewFrameFlag = true;

			Thread::YieldUntilValue(mExecuatedRenderPackageCount, static_cast<uint32_t>(mRenderPackageList.size()));

			mNewFrameFlag = false;

			mCurrentRenderingData = nullptr;
		}

		size_t currentRenderPackageIndex = 0;
		for (size_t i = 0; i != mSyncPoint.size(); ++i)
		{
			mRenderContent.ExecuteCommandLists(static_cast<uint32_t>(mSyncPoint[i] - currentRenderPackageIndex), &mRenderPackageCommandList[currentRenderPackageIndex]);
			currentRenderPackageIndex = mSyncPoint[i];
		}

		for (RRenderPass* renderPass : mRenderPassList)
			renderPass->PostRender(renderingData, mRenderContent);
	}

	void SubRenderThreadMain(uint32_t _threadIndex)
	{
		Thread::SetCurrentThreadName(std::format(L"RenderGraph [{0}] : {1}", mRenderGraphName, _threadIndex));
		while (true)
		{
			Thread::YieldUntilValue(mNewFrameFlag, true);

			if (mRequireExit)
				break;

			while (true)
			{
				uint32_t renderPackageIndex = mExecuatingRenderPackageCount++;
				if (renderPackageIndex >= mRenderPackageList.size())
					break;

				mRenderPackageList[renderPackageIndex].Render(*mCurrentRenderingData, mRenderContent.GetCommandAllocator(_threadIndex));
				++mExecuatedRenderPackageCount;
			}
		}
	}

protected:
	static bool CheckRenderGraphValid(const std::vector<RRenderPass*>& _renderPassList) noexcept
	{
		std::vector<RRenderPassBase*> renderPassList;
		for (RRenderPass* renderPass : _renderPassList)
			renderPassList.push_back(renderPass);
		return CheckRenderGraphValid(renderPassList, RRenderPass::StaticGetClassObject()->GetClassHash());
	}

	bool Setup(std::vector<RRenderPass*> _renderPassList) noexcept
	{
		std::vector<RRenderPassBase*> renderPassList;
		for (RRenderPass* renderPass : _renderPassList)
			renderPassList.push_back(renderPass);

		std::vector<RRenderPassBase*> sortedRenderPassList;
		std::vector<uint32_t> renderPackageList;
		std::vector<uint32_t> syncPointList;
		std::wstring_view a111 = RRenderPass::StaticGetClassObject()->GetClassFullName();
		bool rereorganizeSuccessed = ReorganizeRenderPass(renderPassList, RRenderPass::StaticGetClassObject()->GetClassHash(), sortedRenderPassList, renderPackageList, syncPointList);
		if (rereorganizeSuccessed)
		{
			std::vector<RRenderPass*> renderPassForPackage;
			for (size_t i = 0; i != sortedRenderPassList.size(); ++i)
			{
				if (renderPackageList[i] == SIZE_MAX)
					continue;

				if (renderPackageList[i] == mRenderPackageList.size() + 1)
				{
					if (renderPackageList.size() != 0)
					{
						mRenderPackageList.push_back(RRenderPackage());
						mRenderPackageList.back().Setup(renderPassForPackage);
					}

					renderPassForPackage.resize(0);
					renderPassForPackage.push_back(static_cast<RRenderPass*>(sortedRenderPassList[i]));
				}

				if (renderPackageList[i] == mRenderPackageList.size())
					renderPassForPackage.push_back(static_cast<RRenderPass*>(sortedRenderPassList[i]));
			}

			if (renderPassForPackage.size() != 0)
			{
				mRenderPackageList.push_back(RRenderPackage());
				mRenderPackageList.back().Setup(renderPassForPackage);
			}

			for (size_t i = 0; i != sortedRenderPassList.size(); ++i)
				mRenderPassList.push_back(static_cast<RRenderPass*>(sortedRenderPassList[i]));

			mSyncPoint = syncPointList;

			return true;
		}

		return false;
	}

	virtual RRenderingData ProcessRawRenedrData(RRawRenderingData&& _renderingData) noexcept
	{
		if constexpr (std::is_same_v<RRenderingData, RRawRenderingData>)
			return _renderingData;

		return RRenderingData();
	}

	virtual void PrepareRenderPassResource(RRenderingData& _renderingData, SSubRenderContent& _renderContent) noexcept {}
	virtual void PostRender(RRenderingData& _renderingData, SSubRenderContent& _renderContent) noexcept {}

private:
	std::wstring mRenderGraphName;
	RRenderingData* mCurrentRenderingData = nullptr;

	std::vector<RRenderPass*> mRenderPassList;
	std::vector<RRenderPackage> mRenderPackageList;
	std::vector<IRDICommandList*> mRenderPackageCommandList;
	std::vector<uint32_t> mSyncPoint;
	SSubRenderContent mRenderContent;

	std::vector<std::thread> mSubRenderThread;

	bool mRequireExit = false;

	std::atomic<uint32_t> mExecuatingRenderPackageCount = 0;
	std::atomic<uint32_t> mExecuatedRenderPackageCount = 0;

	std::atomic_bool mNewFrameFlag = false;
};
