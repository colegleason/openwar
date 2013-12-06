#include "pch.h"
#include "Direct3DInterop.h"
#include "Direct3DContentProvider.h"

using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Microsoft::WRL;
using namespace Windows::Phone::Graphics::Interop;
using namespace Windows::Phone::Input::Interop;

namespace OpenWARExampleComp
{

Direct3DInterop::Direct3DInterop() :
	m_timer(ref new BasicTimer())
{
}

IDrawingSurfaceContentProvider^ Direct3DInterop::CreateContentProvider()
{
	ComPtr<Direct3DContentProvider> provider = Make<Direct3DContentProvider>(this);
	return reinterpret_cast<IDrawingSurfaceContentProvider^>(provider.Get());
}

// IDrawingSurfaceManipulationHandler
void Direct3DInterop::SetManipulationHost(DrawingSurfaceManipulationHost^ manipulationHost)
{
	manipulationHost->PointerPressed +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerPressed);

	manipulationHost->PointerMoved +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerMoved);

	manipulationHost->PointerReleased +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerReleased);
}

void Direct3DInterop::RenderResolution::set(Windows::Foundation::Size renderResolution)
{
	if (renderResolution.Width  != m_renderResolution.Width ||
		renderResolution.Height != m_renderResolution.Height)
	{
		m_renderResolution = renderResolution;

		if (m_marker)
		{
			m_marker->UpdateForRenderResolutionChange(m_renderResolution.Width, m_renderResolution.Height);
			RecreateSynchronizedTexture();
		}
	}
}

// Event Handlers
void Direct3DInterop::OnPointerPressed(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
}

void Direct3DInterop::OnPointerMoved(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
}

void Direct3DInterop::OnPointerReleased(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
}

// Interface With Direct3DContentProvider
HRESULT Direct3DInterop::Connect(_In_ IDrawingSurfaceRuntimeHostNative* host)
{
	m_marker = ref new Marker();
	m_marker->Initialize();
	m_model = ref new Model();
	m_marker->AddModel(m_model);
	m_model2 = ref new Model();
	m_marker->AddModel(m_model2);

	m_marker->UpdateForWindowSizeChange(WindowBounds.Width, WindowBounds.Height);
	m_marker->UpdateForRenderResolutionChange(m_renderResolution.Width, m_renderResolution.Height);

	// Restart timer after renderer has finished initializing.
	m_timer->Reset();

	return S_OK;
}

void Direct3DInterop::Disconnect()
{
	m_marker = nullptr;
	m_model = nullptr;
	m_model2 = nullptr;
}

HRESULT Direct3DInterop::PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty)
{
	*contentDirty = true;

	return S_OK;
}

HRESULT Direct3DInterop::GetTexture(_In_ const DrawingSurfaceSizeF* size, _Inout_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Inout_ DrawingSurfaceRectF* textureSubRectangle)
{
	m_timer->Update();

	m_model->ResetModelMatrix();
	m_model->Scale(0.5,0.5,0.5);
	m_model->Rotate(0, 1, 0, m_timer->Total * DirectX::XM_PIDIV4);
	m_model->Rotate(1, 0, 1, 0.2f);
	m_model->Translate(-1, 0, -2);

	m_model2->ResetModelMatrix();
	m_model2->Scale(0.5,0.5,0.5);
	m_model2->Rotate(0, 1, 0, (2*DirectX::XM_PIDIV4) - (m_timer->Total * DirectX::XM_PIDIV4));
	m_model2->Rotate(-1, 0, 1, 0.2f);
	m_model2->Translate(1, 0, -2);

	m_marker->Update(m_timer->Total, m_timer->Delta);
	m_marker->Render();

	RequestAdditionalFrame();

	return S_OK;
}

ID3D11Texture2D* Direct3DInterop::GetTexture()
{
	return m_marker->GetTexture();
}

}
