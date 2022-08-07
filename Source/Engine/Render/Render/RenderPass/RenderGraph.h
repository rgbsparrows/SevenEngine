#pragma once

#include "Core/Misc/Thread.h"
#include "Core/Class/ClassObject.h"
#include "Render/RenderPass/SubRenderContent.h"

#include <vector>
#include <thread>
#include <functional>
#include <initializer_list>

__interface IRDIDevice;
__interface IRDICommandQueue;
__interface IRDICommandList;

class SRenderContent;

class RRenderPassBase
{
	DECLARE_ANCESTOR_CLASSOBJECT_BODY(RRenderPassBase);

public:
	void AddPrecursorRenderPass(std::initializer_list<RRenderPassBase*> _precursorRenderPassList) noexcept;
	void RemovePrecursorRenderPass(std::initializer_list<RRenderPassBase*> _precursorRenderPassList) noexcept;

	std::vector<RRenderPassBase*>& GetPrecursorRenderPassList() noexcept { return mPrecursorRenderPassList; }
	const std::vector<RRenderPassBase*>& GetPrecursorRenderPassList() const noexcept { return mPrecursorRenderPassList; }

	virtual bool IsEmptyCommandRenderPass() const noexcept { return false; }

	virtual void Init(SRenderContent& _renderContent) noexcept {}
	virtual void Clear(SRenderContent& _renderContent) noexcept {}

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

	virtual void PreRender(RRenderingData& _renderSource, SRenderContent& _renderContent) noexcept {}
	virtual void Render(RRenderingData& _renderSource, IRDICommandList* _commandList) noexcept {}
	virtual void PostRender(RRenderingData& _renderSource, SRenderContent& _renderContent) noexcept {}
};

bool CheckRenderGraphValid(const std::vector<RRenderPassBase*>& _renderPassList, uint64_t _renderPassBaseClassHash) noexcept;
bool ReorganizeRenderPass(std::vector<RRenderPassBase*> _renderPassList, uint64_t _renderPassBaseClassHash, std::vector<RRenderPassBase*>& _sortedRenderPassList, std::vector<std::pair<size_t, size_t>>& _renderPackageGroup, std::vector<size_t>& _syncPointForRenderPackage) noexcept;

template<typename _renderingDataType>
class TRenderGraph
{
	DECLARE_ANCESTOR_CLASSOBJECT_BODY(TRenderGraph);

public:
	using RRenderingData = _renderingDataType;
	using RRenderPass = TRenderPass<RRenderingData>;
	using RRenderGraph = TRenderGraph<RRenderingData>;

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

		void Init(SRenderContent& _renderContent) noexcept
		{
			mRDICommandList = _renderContent.AllocateCommandList();
		}

		void Clear(SRenderContent& _renderContent) noexcept
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
		std::vector<size_t> renderPackageList;
		std::vector<size_t> syncPointList;
		bool rereorganizeSuccessed = ReorganizeRenderPass(renderPassList, RRenderPass::StaticGetClassObject()->GetClassHash(), sortedRenderPassList, renderPackageList, syncPointList);
		if (rereorganizeSuccessed)
		{
			std::vector<RRenderPass*> renderPassForPackage;
			for (size_t i = 0; i != sortedRenderPassList.size(); ++i)
			{
				if (renderPackageList[i] == SIZE_MAX)
					continue;

				if (renderPackageList[i] == mRenderPackageList.size())
				{
					if (mRenderPackageList.size() != 0)
						mRenderPackageList.back().Setup(renderPassForPackage);

					renderPassForPackage.resize(0);
					renderPassForPackage.push_back(sortedRenderPassList[i]);
				}

				if (renderPackageList[i] == mRenderPackageList.size())
					renderPassForPackage.push_back(sortedRenderPassList);
			}

			if (mRenderPackageList.size() != 0)
				mRenderPackageList.back().Setup(renderPassForPackage);

			mRenderPassList = sortedRenderPassList;
			mSyncPoint = syncPointList;

			return true;
		}

		return false;
	}

	void Init(SRenderContent* _renderContent) noexcept
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


	void Clear() noexcept
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

	void Render(RRenderingData& _renderingData) noexcept
	{
		for (RRenderPass* renderPass : mRenderPassList)
			renderPass->PreRender(_renderingData, mRenderContent);

		{
			mCurrentRenderingData = &_renderingData;

			mExecuatingRenderPackageCount = 0;
			mExecuatedRenderPackageCount = 0;

			mNewFrameFlag = true;

			Thread::YieldUntilValue(mExecuatedRenderPackageCount, mRenderPackageList.size());

			mNewFrameFlag = false;
		}

		uint32_t currentRenderPackageIndex = 0;
		for (size_t i = 0; i != mSyncPoint.size(); ++i)
		{
			mRenderContent.ExecuteCommandLists(mSyncPoint[i] - currentRenderPackageIndex, &mRenderPackageCommandList[currentRenderPackageIndex]);
			currentRenderPackageIndex = mSyncPoint[i];
		}

		for (RRenderPass* renderPass : mRenderPassList)
			renderPass->PostRender(_renderingData, mRenderContent);
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

private:
	std::wstring mRenderGraphName;
	RRenderingData* mCurrentRenderingData = nullptr;

	std::vector<RRenderPass*> mRenderPassList;
	std::vector<RRenderPackage> mRenderPackageList;
	std::vector<IRDICommandList*> mRenderPackageCommandList;
	std::vector<size_t> mSyncPoint;
	SSubRenderContent mRenderContent;

	std::vector<std::thread> mSubRenderThread;

	bool mRequireExit = false;

	std::atomic<uint32_t> mExecuatingRenderPackageCount = 0;
	std::atomic<uint32_t> mExecuatedRenderPackageCount = 0;

	std::atomic_bool mNewFrameFlag = false;
};
