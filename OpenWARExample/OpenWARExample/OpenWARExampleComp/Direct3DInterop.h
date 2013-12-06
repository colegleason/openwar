#pragma once

#include "pch.h"
#include "BasicTimer.h"
#include <DrawingSurfaceNative.h>
#include "Marker.h"
#include "Model.h"
#include "Mat.h"
#include "Feature2d.h"
#include "ImgOps.h"

#define NUM_MATCH_THRESHOLD 7
#define MATCH_TOLERANCE 0.7
#define RANSAC_THRESHOLD 10

namespace OpenWARExampleComp
{

public delegate void RequestAdditionalFrameHandler();
public delegate void RecreateSynchronizedTextureHandler();

[Windows::Foundation::Metadata::WebHostHidden]
public ref class Direct3DInterop sealed : public Windows::Phone::Input::Interop::IDrawingSurfaceManipulationHandler
{
public:
	Direct3DInterop();

	Windows::Phone::Graphics::Interop::IDrawingSurfaceContentProvider^ CreateContentProvider();

	// IDrawingSurfaceManipulationHandler
	virtual void SetManipulationHost(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ manipulationHost);

	event RequestAdditionalFrameHandler^ RequestAdditionalFrame;
	event RecreateSynchronizedTextureHandler^ RecreateSynchronizedTexture;

	property Windows::Foundation::Size WindowBounds;
	property Windows::Foundation::Size NativeResolution;
	property Windows::Foundation::Size RenderResolution
	{
		Windows::Foundation::Size get(){ return m_renderResolution; }
		void set(Windows::Foundation::Size renderResolution);
	}

	void update(byte frame[], int w, int h);
	bool searchScene(Mat m, Marker mark, Feature2d::KeyPoints kp, Feature2d::KeyPoints mkp, Feature2d::Descriptors desc, Feature2d::Descriptors mdesc);

protected:
	// Event Handlers
	void OnPointerPressed(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);
	void OnPointerMoved(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);
	void OnPointerReleased(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);

internal:
	HRESULT STDMETHODCALLTYPE Connect(_In_ IDrawingSurfaceRuntimeHostNative* host);
	void STDMETHODCALLTYPE Disconnect();
	HRESULT STDMETHODCALLTYPE PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty);
	HRESULT STDMETHODCALLTYPE GetTexture(_In_ const DrawingSurfaceSizeF* size, _Inout_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Inout_ DrawingSurfaceRectF* textureSubRectangle);
	ID3D11Texture2D* GetTexture();

private:
	Marker^ m_marker;
	Model^ m_model;
	Model^ m_model2;
	BasicTimer^ m_timer;
	Windows::Foundation::Size m_renderResolution;
};

}
