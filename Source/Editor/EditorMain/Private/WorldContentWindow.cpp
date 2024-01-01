#include "Engine/World.h"
#include "Engine/Engine.h"
#include "WorldContentWindow.h"
#include "Renderer/BaseRenderer.h"
#include "Resource/ResourceModule.h"
#include "Render/RenderCommandList.h"
#include "Resource/ResourceType/StaticMeshResource.h"

#include <format>
#include "imgui.h"
#include "Core/Clock/Clock.h"

SUIWorldContentWindow::SUIWorldContentWindow() noexcept
{
	mRT = GetRenderCommandList()->CreateRenderProxy<RTexture2D>();
	mImTex = GetRenderCommandList()->CreateRenderProxy<RImguiTexture2D>();

	mRenderGraph = GetRenderCommandList()->CreateRenderGraph<RBaseRenderer>();

	mCamera.mPosition = Math::SFloat3(-10.f, 0.f, 0.f);
}

void SUIWorldContentWindow::OnGui() noexcept
{
	if (mIsWindowOpen == false)
		return;

	ImGui::Begin(std::format("WorldContentWindow [{0}] ### WorldContentWindow", static_cast<const void*>(this)).c_str(), &mIsWindowOpen);
	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(u8"工具"))
		{
			if (ImGui::MenuItem(u8"构造世界"))
			{
				ConstructWorld();
			}

			if (ImGui::MenuItem(u8"生成新Mesh"))
			{
				MakeNewMesh();
			}

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}

	HandleWindowResize();

	if (ImGui::GetIO().MouseDown[1])
	{
		if (ImGui::IsMouseDragging(1))
		{
			ImVec2 delta = ImGui::GetIO().MouseDelta;
			mCamera.mRotation[1] += delta[0];
			mCamera.mRotation[2] += delta[1];
		}

		Math::SFloat3 forward = Math::CalcForwardDirection(mCamera.mRotation);
		Math::SFloat3 right = Math::CalcRightDirection(mCamera.mRotation);
		Math::SFloat3 up = Math::CalcUpDirection(mCamera.mRotation);

		if (ImGui::IsKeyDown('W'))
			mCamera.mPosition += forward * SClock::Get().GetDeltaTime();

		if (ImGui::IsKeyDown('S'))
			mCamera.mPosition -= forward * SClock::Get().GetDeltaTime();

		if (ImGui::IsKeyDown('A'))
			mCamera.mPosition -= right * SClock::Get().GetDeltaTime() * Math::PI / 180.f;

		if (ImGui::IsKeyDown('D'))
			mCamera.mPosition += right * SClock::Get().GetDeltaTime() * Math::PI / 180.f;
	}

	RenderWorld();

	ImGui::Image(mImTex, ImGui::GetContentRegionAvail());

	ImGui::End();
}

void SUIWorldContentWindow::HandleWindowResize()
{
	ImVec2 size = ImGui::GetWindowSize();
	if (size.x != mLastWindowSize[0] || size.y != mLastWindowSize[1])
	{
		mLastWindowSize[0] = size.x;
		mLastWindowSize[1] = size.y;
		OnResize();
	}
}

void SUIWorldContentWindow::OnResize()
{
	ImVec2 windowSize = ImGui::GetWindowSize();

	if (windowSize.x > 0 && windowSize.y > 0)
	{
		RTexture2DData texData;
		texData.mDesc.mResourceUsage = ERDIResourceUsage::RenderTarget | ERDIResourceUsage::ShaderResource;
		texData.mDesc.mPixelFormat = ERDIPixelFormat::R8G8B8A8_UNORM;
		texData.mDesc.mSizeX = static_cast<uint32_t>(windowSize.x);
		texData.mDesc.mSizeY = static_cast<uint32_t>(windowSize.y);
		texData.mDesc.mClearColor = Math::SFColor(0.5f, 0, 0, 1);
		GetRenderCommandList()->RefrashStaticTexture2D_I(mRT, std::move(texData));
		GetRenderCommandList()->RefrashImTexture2D_I(mRT, mImTex);
	}
}

void SUIWorldContentWindow::ConstructWorld()
{
	SWorld* world = SEngine::Get().GetMainWorld();
	if (world == nullptr)
		return;

	AActor* actor = world->SpawnActor<AActor>();
	ACComponent* component = actor->SpawnComponent(nullptr, ACStaticMesh::StaticGetClassObject()->GetClassHash());
	ACStaticMesh* smComponent = static_cast<ACStaticMesh*>(component);

	SStaticMeshRP defaultMesh = GetResourceModule()->LoadResource<SStaticMeshResource>(L"Engine/Content/DefaultResource/a.mesh");

	smComponent->SetStaticMesh(defaultMesh);
}

void SUIWorldContentWindow::MakeNewMesh()
{
	//SStaticMeshRP smr = SStaticMeshRP::New();

	//smr->SetVertexSemantic(
	//	ConvertToEnumFlag({
	//		EVertexSemantic::Position,
	//		EVertexSemantic::Color,
	//		EVertexSemantic::Normal,
	//		EVertexSemantic::Uv0 }
	//		)
	//);

	//smr->ResizeVertexCount(4);
	//smr->ResizeTriangleCount(2);

	//smr->SetTriangle(SMeshTriangle(0, 1, 2), 0);
	//smr->SetTriangle(SMeshTriangle(0, 2, 3), 1);

	//SFullVertex v0;
	//SFullVertex v1;
	//SFullVertex v2;
	//SFullVertex v3;

	//v1.mPosition = Math::SFloat3(-50, 50, 0);
	//v2.mPosition = Math::SFloat3(50, 50, 0);
	//v3.mPosition = Math::SFloat3(50, -50, 0);

	//v0.mColor = Math::SFColor(0, 0, 0, 1);
	//v1.mColor = Math::SFColor(1, 0, 0, 1);
	//v2.mColor = Math::SFColor(0, 1, 0, 1);
	//v3.mColor = Math::SFColor(0, 0, 1, 1);

	//v0.mNormal = Math::SFloat3(0, 0, 1);
	//v1.mNormal = Math::SFloat3(0, 0, 1);
	//v2.mNormal = Math::SFloat3(0, 0, 1);
	//v3.mNormal = Math::SFloat3(0, 0, 1);

	//v0.mUv[0] = Math::SFloat2(0, 0);
	//v1.mUv[0] = Math::SFloat2(0, 1);
	//v2.mUv[0] = Math::SFloat2(1, 1);
	//v3.mUv[0] = Math::SFloat2(1, 0);

	//smr->SetVertex(v0, 0);
	//smr->SetVertex(v1, 1);
	//smr->SetVertex(v2, 2);
	//smr->SetVertex(v3, 3);

	//smr->ResizeSubMeshCount(1);
	//smr->SetSubMesh(SSubMesh(SRange(0, 6), u8"Cube"), 0);

	//GetResourceModule()->CreateResource(smr, "Engine/Content/DefaultResource/a.mesh");

	//smr->Release();

	SStaticMeshRP defaultMesh = GetResourceModule()->LoadResource<SStaticMeshResource>(L"Engine/Content/DefaultResource/a.mesh");
	defaultMesh->ResizeSubMeshCount(1);
	defaultMesh->SetSubMesh(SSubMesh(SRange(0, 6), u8"Cube"), 0);
	defaultMesh->Save();
}

void SUIWorldContentWindow::RenderWorld()
{
	if (SWorld* World = SEngine::Get().GetMainWorld())
	{
		GetRenderCommandList()->RenderWorld_D(World->GetRenderProxy(), mCamera, mRT, mRenderGraph);
	}
}
