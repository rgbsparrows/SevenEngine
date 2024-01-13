#include "UI/UIModule.h"
#include "FbxImporterWindow.h"
#include "Core/Misc/Localize.h"
#include "Core/Util/UtilMacros.h"
#include "Resource/ResourceModule.h"
#include "EditorMain/MessageBoxWindow.h"

#include "imgui.h"
#include "fbxsdk.h"
#include <intsafe.h>

#pragma comment(lib, "libfbxsdk.lib")

DECLEAR_DEFAULT_WINDOW(SUIFbxImporterMenuWindow, , "FbxImporterMenu");

void SUIFbxImporterMenuWindow::OnGui() noexcept
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::MenuItem(u8"加载Mesh"))
		{
			GetUIModule()->FindOrAddWindow<SUIFbxImporterWindow>(u8"Mesh加载");
		}
		ImGui::EndMainMenuBar();
	}
}

void SUIFbxImporterWindow::OnGui() noexcept
{
	if (mIsWindowOpen == false)
		return;

	ImGui::Begin(u8"Mesh加载", &mIsWindowOpen);

	std::string fbxPath = mFbxPath.u8string();
	if (ImGui::InputText(u8"FBX路径", fbxPath, 512))
		mFbxPath = fbxPath;
	std::string destResourcePath = mDestResourcePath.u8string();
	if (ImGui::InputText(u8"生成资源路径", destResourcePath, 512))
		mDestResourcePath = destResourcePath;

	if (ImGui::Button(u8"加载"))
	{
		bool res = LoadFbxMesh(mFbxPath, mDestResourcePath);
		if (res == false)
			MessageBox(u8"加载失败", u8"加载失败");
	}

	ImGui::BeginChild(u8"Mesh列表");

	for (size_t i = 0; i != mLoadedMesh.size(); ++i)
	{
		ImGui::Text(mLoadedMesh[i].mName.c_str());
		ImGui::SameLine();
		std::string savePath = mLoadedMesh[i].mSavePath.u8string();
		if (ImGui::InputText(std::format(u8"###保存路径{0}", i).c_str(), savePath, 512))
			mLoadedMesh[i].mSavePath = savePath;
		ImGui::SameLine();
		if (ImGui::Button(std::format(u8"导出###导出{0}", i).c_str()))
			GetResourceModule()->CreateResource(mLoadedMesh[i].mStaticMesh, mLoadedMesh[i].mSavePath);
		ImGui::SameLine();
		if (ImGui::Button(std::format(u8"删除###删除{0}", i).c_str()))
		{
			mLoadedMesh.erase(mLoadedMesh.begin() + i);
			--i;
		}
	}

	ImGui::EndChild();

	ImGui::End();
}

bool SUIFbxImporterWindow::LoadFbxMesh(const std::filesystem::path& fbxPath, const std::filesystem::path& destResourcePath) noexcept
{
	if (std::filesystem::exists(fbxPath) == false)
		return false;

	fbxsdk::FbxManager* fbxManager = fbxsdk::FbxManager::Create();
	fbxsdk::FbxIOSettings* fbxIOSettings = fbxsdk::FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(fbxIOSettings);

	fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(fbxManager, "");
	if (importer->Initialize(Locale::ConvertWstringToString(fbxPath.wstring(), Locale::ECodePage::GBK).c_str(), -1, fbxManager->GetIOSettings()) == false)
		return false;

	fbxsdk::FbxScene* fbxScene = fbxsdk::FbxScene::Create(fbxManager, "Scene");
	importer->Import(fbxScene);

	RecursiveProcessFbxNode(fbxScene->GetRootNode());

	importer->Destroy();

	return true;
}

void SUIFbxImporterWindow::RecursiveProcessFbxNode(fbxsdk::FbxNode* _node)
{
	if (_node == nullptr)
		return;

	for (int32_t i = 0; i != _node->GetNodeAttributeCount(); ++i)
	{
		fbxsdk::FbxNodeAttribute* nodeAttribute = _node->GetNodeAttributeByIndex(i);

		switch (nodeAttribute->GetAttributeType())
		{
		case fbxsdk::FbxNodeAttribute::eMesh:
		{
			if (fbxsdk::FbxMesh* fbxMesh = _node->GetMesh())
				ProcessFbxMesh(fbxMesh);
			break;
		}
		}
	}

	for (int32_t i = 0; i != _node->GetChildCount(); ++i)
		RecursiveProcessFbxNode(_node->GetChild(i));
}

template<typename _ElementType>
auto GetFbxVertexElement(const fbxsdk::FbxMesh* _mesh, int32_t _polyIndex, int32_t _controlPointIndex, int32_t _vertexIndex, const fbxsdk::FbxLayerElementTemplate<_ElementType>* _meshElementList)
{
	using ElementType = _ElementType;

	if (_meshElementList == nullptr)
		return ElementType();

	switch (_meshElementList->GetMappingMode())
	{
	case fbxsdk::FbxGeometryElement::eByControlPoint:
	{
		switch (_meshElementList->GetReferenceMode())
		{
		case fbxsdk::FbxGeometryElement::eDirect:
			return _meshElementList->GetDirectArray().GetAt(_controlPointIndex);
		case fbxsdk::FbxGeometryElement::eIndexToDirect:
		{
			int32_t index = _meshElementList->GetIndexArray().GetAt(_controlPointIndex);
			return _meshElementList->GetDirectArray().GetAt(index);
		}
		}
		break;
	}
	case fbxsdk::FbxGeometryElement::eByPolygonVertex:
	{
		switch (_meshElementList->GetReferenceMode())
		{
		case fbxsdk::FbxGeometryElement::eDirect:
			return _meshElementList->GetDirectArray().GetAt(_vertexIndex);
		case fbxsdk::FbxGeometryElement::eIndexToDirect:
		{
			int32_t index = _meshElementList->GetIndexArray().GetAt(_vertexIndex);
			return _meshElementList->GetDirectArray().GetAt(index);
		}
		}
		break;
	}
	case fbxsdk::FbxGeometryElement::eByPolygon:
	{
		switch (_meshElementList->GetReferenceMode())
		{
		case fbxsdk::FbxGeometryElement::eDirect:
			return _meshElementList->GetDirectArray().GetAt(_polyIndex);
		case fbxsdk::FbxGeometryElement::eIndexToDirect:
		{
			int32_t index = _meshElementList->GetIndexArray().GetAt(_polyIndex);
			return _meshElementList->GetDirectArray().GetAt(index);
		}
		}
		break;
	}
	}

	return ElementType();
}

void SUIFbxImporterWindow::ProcessFbxMesh(fbxsdk::FbxMesh* _mesh)
{
	SStaticMeshRP staticMeshRP = SStaticMeshRP::New();

	struct SSubMeshFullInfo
	{
		std::vector<SFullVertex> mVertexs;
		std::vector<SMeshTriangle> mTriangles;
		fbxsdk::FbxSurfaceMaterial* mMaterial = nullptr;
	};

	std::vector<SSubMeshFullInfo> allSubMeshFullInfos;

	EVertexSemanticFlag vertexSemanticFlag = ConvertToEnumFlag(EVertexSemantic::Position);

	int xxas = _mesh->GetElementMaterialCount();

	if (_mesh->GetElementVertexColorCount() != 0)
		vertexSemanticFlag |= ConvertToEnumFlag(EVertexSemantic::Color);

	if (_mesh->GetElementNormalCount() != 0)
		vertexSemanticFlag |= ConvertToEnumFlag(EVertexSemantic::Normal);

	if (_mesh->GetElementTangentCount() != 0 && _mesh->GetElementBinormalCount() != 0)
		vertexSemanticFlag |= ConvertToEnumFlag(EVertexSemantic::Tangent);

	for (int32_t i = 0; i != _mesh->GetElementUVCount() && i != 8; ++i)
		vertexSemanticFlag |= ConvertToEnumFlag(static_cast<EVertexSemantic>(EnumToInt(EVertexSemantic::Uv0) + i));

	for (int32_t polyIndex = 0, vertexIndex = 0; polyIndex != _mesh->GetPolygonCount(); ++polyIndex)
	{
		if (_mesh->GetPolygonSize(polyIndex) < 3)
			continue;

		fbxsdk::FbxSurfaceMaterial* currentMaterial = GetFbxVertexElement(_mesh, polyIndex, _mesh->GetPolygonVertex(polyIndex, 0), vertexIndex, _mesh->GetElementMaterial());

		size_t subMeshIndex = FindIndexIf(allSubMeshFullInfos, [&](const SSubMeshFullInfo& _subMeshInfo) { return _subMeshInfo.mMaterial == currentMaterial; });
		if (subMeshIndex == SIZE_T_ERROR)
		{
			SSubMeshFullInfo newSubMeshFullInfo;
			newSubMeshFullInfo.mMaterial = currentMaterial;

			allSubMeshFullInfos.push_back(newSubMeshFullInfo);
			subMeshIndex = allSubMeshFullInfos.size() - 1;
		}

		std::vector<SFullVertex>& vertexs = allSubMeshFullInfos[subMeshIndex].mVertexs;
		std::vector<SMeshTriangle>& triangles = allSubMeshFullInfos[subMeshIndex].mTriangles;

		for (int32_t vertexInPolyIndex = 2; vertexInPolyIndex != _mesh->GetPolygonSize(polyIndex); ++vertexInPolyIndex)
		{
			uint32_t baseVertexIndex = static_cast<uint32_t>(vertexs.size());
			triangles.push_back(SMeshTriangle(baseVertexIndex, baseVertexIndex + vertexInPolyIndex - 1, baseVertexIndex + vertexInPolyIndex));
		}

		for (int32_t vertexInPolyIndex = 0; vertexInPolyIndex != _mesh->GetPolygonSize(polyIndex); ++vertexInPolyIndex)
		{
			SFullVertex newVertex;

			int32_t controlPointIndex = _mesh->GetPolygonVertex(polyIndex, vertexInPolyIndex);

			{
				fbxsdk::FbxVector4 vertexPosition = _mesh->GetControlPointAt(controlPointIndex);
				newVertex.mPosition = Math::SFloat3(vertexPosition);
			}

			if ((vertexSemanticFlag | ConvertToEnumFlag(EVertexSemantic::Color)) == ConvertToEnumFlag(EVertexSemantic::Color))
			{
				fbxsdk::FbxColor vertexColor = GetFbxVertexElement(_mesh, polyIndex, controlPointIndex, vertexIndex, _mesh->GetElementVertexColor());
				newVertex.mColor = Math::SFColor(vertexColor);
			}

			if ((vertexSemanticFlag | ConvertToEnumFlag(EVertexSemantic::Normal)) == ConvertToEnumFlag(EVertexSemantic::Normal))
			{
				fbxsdk::FbxVector4 vertexNormal = GetFbxVertexElement(_mesh, polyIndex, controlPointIndex, vertexIndex, _mesh->GetElementNormal());
				newVertex.mNormal = Math::SFloat3(vertexNormal);
			}

			if ((vertexSemanticFlag | ConvertToEnumFlag(EVertexSemantic::Tangent)) == ConvertToEnumFlag(EVertexSemantic::Tangent))
			{
				fbxsdk::FbxVector4 vertexTangent = GetFbxVertexElement(_mesh, polyIndex, controlPointIndex, vertexIndex, _mesh->GetElementTangent());
				fbxsdk::FbxVector4 vertexBinormal = GetFbxVertexElement(_mesh, polyIndex, controlPointIndex, vertexIndex, _mesh->GetElementBinormal());

				bool inverseBinormal = Math::Dot(Math::Cross(newVertex.mNormal, Math::SFloat3(vertexTangent)), Math::SFloat3(vertexBinormal));
				Math::SFloat3 tangent = Math::SFloat3(vertexTangent);
				newVertex.mTangent = Math::SFloat4(tangent[0], tangent[1], tangent[2], inverseBinormal ? -1.f : 1.f);
			}

			for (int32_t i = 0; i != _mesh->GetElementUVCount() && i != 8; ++i)
			{
				EVertexSemanticFlag uvFlag = ConvertToEnumFlag(static_cast<EVertexSemantic>(EnumToInt(EVertexSemantic::Uv0) + i));
				if ((vertexSemanticFlag | uvFlag) == uvFlag)
				{
					fbxsdk::FbxVector2 vertexUv = GetFbxVertexElement(_mesh, polyIndex, controlPointIndex, vertexIndex, _mesh->GetElementUV(i));
					newVertex.mUv[i] = Math::SFloat2(vertexUv);
				}
			}

			vertexs.push_back(newVertex);

			++vertexIndex;
		}
	}

	Sort(allSubMeshFullInfos,
		[](const SSubMeshFullInfo& _left, const SSubMeshFullInfo& _right)
		{
			if (_left.mMaterial && _right.mMaterial)
				return strcmp(_left.mMaterial->GetName(), _right.mMaterial->GetName()) > 0;
			else return _right.mMaterial != nullptr;
		});

	std::vector<SFullVertex> finalFullVertexs;
	std::vector<SMeshTriangle> finalTriangles;
	std::vector<SSubMesh> finalSubMeshes;
	for (SSubMeshFullInfo& subMeshFullInfo : allSubMeshFullInfos)
	{
		uint32_t rawTriangleCount = static_cast<uint32_t>(finalTriangles.size());

		MergeRepeatedVertex(subMeshFullInfo.mVertexs, subMeshFullInfo.mTriangles);
		ApplyTriangleIndexOffset(subMeshFullInfo.mTriangles, static_cast<uint32_t>(finalFullVertexs.size()));

		Append(finalFullVertexs, subMeshFullInfo.mVertexs);
		Append(finalTriangles, subMeshFullInfo.mTriangles);

		uint32_t newTriangleCount = static_cast<uint32_t>(subMeshFullInfo.mTriangles.size());

		const char* materialName = subMeshFullInfo.mMaterial == nullptr ? u8"未命名材质" : subMeshFullInfo.mMaterial->GetName();
		finalSubMeshes.push_back(SSubMesh(SRange(rawTriangleCount * 3, newTriangleCount * 3), materialName));
	}

	staticMeshRP->SetVertexSemantic(vertexSemanticFlag);

	staticMeshRP->ResizeVertexCount(finalFullVertexs.size());
	staticMeshRP->ResizeTriangleCount(finalTriangles.size());
	staticMeshRP->ResizeSubMeshCount(finalSubMeshes.size());

	for (size_t i = 0; i != finalFullVertexs.size(); ++i)
		staticMeshRP->SetVertex(finalFullVertexs[i], i);

	for (size_t i = 0; i != finalTriangles.size(); ++i)
		staticMeshRP->SetTriangle(finalTriangles[i], i);

	for (size_t i = 0; i != finalSubMeshes.size(); ++i)
		staticMeshRP->SetSubMesh(finalSubMeshes[i], i);

	std::filesystem::path savePath = std::filesystem::path("Engine/Content/Mesh") / _mesh->GetName();

	mLoadedMesh.push_back(SLoadedStaticMeshInfo{ _mesh->GetName(), savePath, staticMeshRP});
}

void SUIFbxImporterWindow::MergeRepeatedVertex(std::vector<SFullVertex>& _vertexs, std::vector<SMeshTriangle>& _triangles, size_t _maxSearchSpan)
{
	std::vector<SFullVertex> newVertexs;
	std::vector<SMeshTriangle> newTriangles;

	for (size_t i = 0; i != _triangles.size(); ++i)
	{
		{
			SFullVertex vertex[3];
			for (size_t j = 0; j != 3; ++j)
				vertex[j] = _vertexs[_triangles[i].mVertexIndex[j]];

			Math::SFloat3 crossResult = Math::Cross(vertex[1].mPosition - vertex[0].mPosition, vertex[2].mPosition - vertex[0].mPosition);
			if (Math::Dot(crossResult, crossResult) == 0.f)
				continue;
		}

		SMeshTriangle newTriangle;

		for (size_t j = 0; j != 3; ++j)
		{
			SFullVertex vertex = _vertexs[_triangles[i].mVertexIndex[j]];
			size_t vertexIndex = FindIndex(newVertexs, vertex);
			if (vertexIndex == SIZE_T_ERROR || newVertexs.size() - vertexIndex > _maxSearchSpan)
			{
				newVertexs.push_back(vertex);
				vertexIndex = newVertexs.size() - 1;
			}

			newTriangle[j] = static_cast<uint32_t>(vertexIndex);
		}

		newTriangles.push_back(newTriangle);
	}

	_vertexs = std::move(newVertexs);
	_triangles = std::move(newTriangles);
}

void SUIFbxImporterWindow::ApplyTriangleIndexOffset(std::vector<SMeshTriangle>& _triangles, uint32_t _offset) noexcept
{
	for (size_t i = 0; i != _triangles.size(); ++i)
	{
		for (size_t j = 0; j != 3; ++j)
			_triangles[i].mVertexIndex[j] += _offset;
	}
}
