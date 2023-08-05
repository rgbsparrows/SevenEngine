#include "UI/Imgui/imgui.h"
#include "WorldContentWindow.h"
#include "Resource/ResourceModule.h"
#include "Render/RenderCommandList.h"
#include "Resource/ResourceType/StaticMeshResource.h"

#include <format>

void SUIWorldContentWindow::OnGui() noexcept
{
	if (mIsWindowOpen == false)
		return;

	static RRenderProxy<RWorld>* renderProxy = new RRenderProxy<RWorld>;
	if (mRenderGraph == nullptr)
	{
		//mRenderGraph = new RWorldForwardRenderGraph;
		//mRenderGraph->Init();
		//GetRenderCommandList()->ConstructRenderGraph(mRenderGraph);

		if (mRT == nullptr)
		{

		mRT = new RRenderProxy<RTexture2D>;
		RTexture2DData texData;
		texData.mDesc.mResourceUsage = ERDIResourceUsage::RenderTarget | ERDIResourceUsage::ShaderResource;
		texData.mDesc.mPixelFormat = ERDIPixelFormat::R8G8B8A8_UNORM;
		texData.mDesc.mSizeX = 1024;
		texData.mDesc.mSizeY = 1024;
		texData.mDesc.mClearColor = Math::SFColor(0.5f, 0, 0, 1);
		GetRenderCommandList()->RefrashStaticTexture2D_I(mRT, std::move(texData));
		}

		if (mImTex == nullptr)
		{
			mImTex = new RRenderProxy<RImguiTexture2D>;
			GetRenderCommandList()->RefrashImTexture2D_I(mRT, mImTex);
		}
	}

	ImGui::Begin(std::format("WorldContentWindow [{0}]", static_cast<const void*>(this)).c_str(), &mIsWindowOpen);

	static Math::STransform transformParent;
	static Math::STransform transformLocal;

	ImGui::Text("ParentTransform");
	Math::SFloat3 pos = static_cast<Math::SFloat3>(transformParent.mPosition);
	ImGui::DragFloat3("ParentPosition", pos.GetData());
	ImGui::DragFloat3("ParentRotation", transformParent.mRotation.GetData());
	ImGui::DragFloat3("ParentScale", transformParent.mScale.GetData());
	transformParent.mPosition = static_cast<Math::SDouble3>(pos);

	ImGui::Text("LocalTransform");
	pos = static_cast<Math::SFloat3>(transformLocal.mPosition);
	ImGui::DragFloat3("LocalPosition", pos.GetData());
	ImGui::DragFloat3("LocalRotation", transformLocal.mRotation.GetData());
	ImGui::DragFloat3("LocalScale", transformLocal.mScale.GetData());
	transformLocal.mPosition = static_cast<Math::SDouble3>(pos);

	Math::SFloat4x4 rotationMatrix = Math::CalcTransformMatrix(transformLocal) * Math::CalcTransformMatrix(transformParent);
	Math::SFloat4x4 rotation2Matrix = Math::CalcTransformMatrix(Math::ApplyTransform(transformParent, transformLocal));

	ImGui::Text("Matrix1");
	ImGui::DragFloat4("m10", rotationMatrix[0]);
	ImGui::DragFloat4("m11", rotationMatrix[1]);
	ImGui::DragFloat4("m12", rotationMatrix[2]);
	ImGui::DragFloat4("m13", rotationMatrix[3]);

	ImGui::Text("Matrix2");
	ImGui::DragFloat4("m20", rotation2Matrix[0]);
	ImGui::DragFloat4("m21", rotation2Matrix[1]);
	ImGui::DragFloat4("m22", rotation2Matrix[2]);
	ImGui::DragFloat4("m23", rotationMatrix[3]);


	//GetRenderCommandList()->RenderWorld(renderProxy, mRT, mRenderGraph);

	ImGui::Image(mImTex, ImGui::GetContentRegionAvail());

	ImGui::End();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(u8"ÎÄ¼þ"))
		{
			if (ImGui::Button(u8"Ìí¼ÓCubeMesh"))
			{
				SStaticMeshRP smr = SStaticMeshRP::New();

				smr->SetVertexSemantic(
					ConvertToEnumFlag({
						EVertexSemantic::Position,
						EVertexSemantic::Color,
						EVertexSemantic::Normal,
						EVertexSemantic::Uv0 }
						)
				);

				smr->ResizeVertexCount(4);
				smr->ResizeTriangleCount(2);

				smr->SetTriangle(SMeshTriangle(0, 1, 2), 0);
				smr->SetTriangle(SMeshTriangle(0, 2, 3), 1);

				SFullVertex v0;
				SFullVertex v1;
				SFullVertex v2;
				SFullVertex v3;
				
				v1.mPosition = Math::SFloat3(-50, 50, 0);
				v2.mPosition = Math::SFloat3(50, 50, 0);
				v3.mPosition = Math::SFloat3(50, -50, 0);

				v0.mColor = Math::SFColor(0, 0, 0, 1);
				v1.mColor = Math::SFColor(1, 0, 0, 1);
				v2.mColor = Math::SFColor(0, 1, 0, 1);
				v3.mColor = Math::SFColor(0, 0, 1, 1);

				v0.mNormal = Math::SFloat3(0, 0, 1);
				v1.mNormal = Math::SFloat3(0, 0, 1);
				v2.mNormal = Math::SFloat3(0, 0, 1);
				v3.mNormal = Math::SFloat3(0, 0, 1);

				v0.mUv[0] = Math::SFloat2(0, 0);
				v1.mUv[0] = Math::SFloat2(0, 1);
				v2.mUv[0] = Math::SFloat2(1, 1);
				v3.mUv[0] = Math::SFloat2(1, 0);

				smr->SetVertex(v0, 0);
				smr->SetVertex(v1, 1);
				smr->SetVertex(v2, 2);
				smr->SetVertex(v3, 3);

				GetResourceModule()->CreateResource(smr, L"Engine/Content/DefaultResource/a.mesh");
				smr->Release();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
