#pragma once

#include "DirectXHelper.h"
#include "Model.h"
#include <vector>

using namespace std;

ref class Marker sealed
{
public:
	Marker(void);

	void AddModel(Model^ nmodel);

	// Direct3DBase methods.
	void Initialize(Windows::UI::Core::CoreWindow^ window);
	void HandleDeviceLost(); 
	
	void CreateDeviceResources();
	void CreateWindowSizeDependentResources();

	void UpdateForWindowSizeChange();
	void ReleaseResourcesForSuspending();
	void Render();
	void Present();
	float ConvertDipsToPixels(float dips);
	
	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);

private:
	// Direct3D Objects.
	Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	// Cached renderer properties.
	D3D_FEATURE_LEVEL m_featureLevel;
	Windows::Foundation::Size m_renderTargetSize;
	Windows::Foundation::Rect m_windowBounds;
	Platform::Agile<Windows::UI::Core::CoreWindow> m_window;

	vector<Model^> m_ModelList;
};

