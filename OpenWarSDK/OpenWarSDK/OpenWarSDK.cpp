#include "pch.h"
#include "OpenWarSDK.h"
#include "BasicTimer.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace concurrency;

OpenWarSDK::OpenWarSDK() :
	m_windowClosed(false),
	m_windowVisible(true)
{
}

void OpenWarSDK::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &OpenWarSDK::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &OpenWarSDK::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &OpenWarSDK::OnResuming);

	m_marker = ref new Marker();
	m_model = ref new Model();
	m_model2 = ref new Model();
}

void OpenWarSDK::SetWindow(CoreWindow^ window)
{
	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &OpenWarSDK::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &OpenWarSDK::OnWindowClosed);

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &OpenWarSDK::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &OpenWarSDK::OnPointerMoved);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &OpenWarSDK::OnPointerReleased);

	m_marker->Initialize(CoreWindow::GetForCurrentThread());
	m_marker->AddModel(m_model);
	m_marker->AddModel(m_model2);
}

void OpenWarSDK::Load(Platform::String^ entryPoint)
{
}

void OpenWarSDK::Run()
{
	BasicTimer^ timer = ref new BasicTimer();

	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			timer->Update();
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
			m_model->ResetModelMatrix();
			m_model->Scale(0.5,0.5,0.5);
			m_model->Rotate(0, 1, 0, timer->Total * DirectX::XM_PIDIV4);
			m_model->Rotate(1, 0, 1, 0.2f);
			m_model->Translate(-1, 0, -2);
			m_model2->ResetModelMatrix();
			m_model2->Scale(0.5,0.5,0.5);
			m_model2->Rotate(0, 1, 0, (2*DirectX::XM_PIDIV4) - (timer->Total * DirectX::XM_PIDIV4));
			m_model2->Rotate(-1, 0, 1, 0.2f);
			m_model2->Translate(1, 0, -2);
			m_marker->Update(timer->Total, timer->Delta);
			m_marker->Render();
			m_marker->Present(); // This call is synchronized to the display frame rate.
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void OpenWarSDK::Uninitialize()
{
}

void OpenWarSDK::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void OpenWarSDK::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

void OpenWarSDK::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
}

void OpenWarSDK::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
}

void OpenWarSDK::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
}

void OpenWarSDK::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void OpenWarSDK::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();
	m_marker->ReleaseResourcesForSuspending();

	create_task([this, deferral]()
	{
		// Insert your code here.

		deferral->Complete();
	});
}
 
void OpenWarSDK::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.
	 m_marker->CreateWindowSizeDependentResources();
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new OpenWarSDK();
}

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}