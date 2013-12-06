#pragma once

#include "DirectXHelper.h"
#include "Model.h"
#include <vector>

using namespace std;

ref class Marker sealed
{
public:
	Marker(void);

	void Initialize();
	void CreateDeviceResources();
	void CreateWindowSizeDependentResources();
	void UpdateForRenderResolutionChange(float width, float height);
	void UpdateForWindowSizeChange(float width, float height);

	void Render();
	void Update(float timeTotal, float timeDelta);

	void AddModel(Model^ nmodel);

internal:
	ID3D11Texture2D* GetTexture()
	{
		return m_renderTarget.Get();
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTarget;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	// Cached renderer properties.
	D3D_FEATURE_LEVEL m_featureLevel;
	Windows::Foundation::Size m_renderTargetSize;
	Windows::Foundation::Rect m_windowBounds;

	vector<Model^> m_ModelList;
};

