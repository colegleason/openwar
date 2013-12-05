#pragma once

#include <D3D11.h>
#include "DirectXHelper.h"

using namespace std;

struct ModelViewProjectionConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

struct VertexPositionColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};

struct Vertex	//Overloaded Vertex Structure
{
	Vertex(){};
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x,y,z), texCoord(u, v), normal(nx, ny, nz){};

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
};

//Create material structure
struct SurfaceMaterial
{
	std::wstring matName;
	DirectX::XMFLOAT4 difColor;
	int texArrayIndex;
	bool hasTexture;
	bool transparent;
};

ref class Model sealed
{
public:
	Model(void);

	void Initialize(Windows::UI::Core::CoreWindow^ window);
	void SetDeviceAndContext(int device, int context);

private:
	void HandleDeviceLost(); 
	void CreateDeviceResources();
	void CreateWindowSizeDependentResources();
	void UpdateForWindowSizeChange();
	void ReleaseResourcesForSuspending();

public:
	void Render();
	void Update(float timeTotal, float timeDelta);

	void ResetModelMatrix();
	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z, float theta);
	void Scale(float x, float y, float z);

//private:
//	bool LoadObjModel(std::wstring filename, 
//		std::vector<int>& subsetIndexStart,
//		std::vector<int>& subsetMaterialArray,
//		std::vector<SurfaceMaterial>& material, 
//		int& subsetCount,
//		bool isRHCoordSys,
//		bool computeNormals);

private:
	bool m_loadingComplete;

	// Direct3D Objects.
	Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;

	// Cached renderer properties.
	Windows::Foundation::Rect m_windowBounds;
	Platform::Agile<Windows::UI::Core::CoreWindow> m_window;

	// Buffers and Shaders
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// Model info
	vector<float> m_modelVertices, m_modelNormals, m_modelTexCoords;
	vector<unsigned short> m_vertIndices, m_normIndices, m_texIndices;
	vector<float> m_modelInfo;

	uint32 m_indexCount;
	ModelViewProjectionConstantBuffer m_constantBufferData;
	DirectX::XMMATRIX m_ModelTransform;
};
