using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using OpenWARExampleComp;
using Microsoft.Devices;
using Microsoft.Xna.Framework.Media;
using System.Windows.Input;
using System.Windows.Media;
using System.Threading;

namespace OpenWARExample
{
    public partial class MainPage : PhoneApplicationPage
    {
        // Variables
        private int savedCounter = 0;
        PhotoCamera cam;
        MediaLibrary library = new MediaLibrary();
        private Direct3DInterop m_d3dInterop = null;
        private Thread FramesThread;
        private bool pumpFrames;
        private static ManualResetEvent pauseFramesEvent = new ManualResetEvent(true);

        // Holds the current flash mode.
        private string currentFlashMode;

        // Holds the current resolution index.
        int currentResIndex = 0;

        // Constructor
        public MainPage()
        {
            InitializeComponent();
        }


        //Code for initialization, capture completed, image availability events; also setting the source for the viewfinder.
        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {

            // Check to see if the camera is available on the device.
            if ((PhotoCamera.IsCameraTypeSupported(CameraType.Primary) == true))
            {

                // use standard camera on back of device.
                cam = new Microsoft.Devices.PhotoCamera(CameraType.Primary);

                // Event is fired when the PhotoCamera object has been initialized.
                cam.Initialized += new EventHandler<Microsoft.Devices.CameraOperationCompletedEventArgs>(cam_Initialized);

                // The event is fired when auto-focus is complete.
                cam.AutoFocusCompleted += new EventHandler<CameraOperationCompletedEventArgs>(cam_AutoFocusCompleted);

                // The event is fired when the viewfinder is tapped (for focus).
                viewfinderCanvas.Tap += new EventHandler<GestureEventArgs>(focus_Tapped);


                //Set the VideoBrush source to the camera.
                viewfinderBrush.SetSource(cam);

                pumpFrames = true;
                FramesThread = new System.Threading.Thread(PumpFrames);

                // Start pump.
                FramesThread.Start();
            }
            else
            {
                // The camera is not supported on the device.
                this.Dispatcher.BeginInvoke(delegate()
                {
                    // Write message.
                    txtDebug.Text = "A Camera is not available on this device.";
                });

                // Disable UI.
                AFButton.IsEnabled = false;
                ResButton.IsEnabled = false;
            }
        }

        protected override void OnNavigatingFrom(System.Windows.Navigation.NavigatingCancelEventArgs e)
        {
            if (cam != null)
            {
                // Dispose camera to minimize power consumption and to expedite shutdown.
                cam.Dispose();

                // Release memory, ensure garbage collection.
                cam.Initialized -= cam_Initialized;
                cam.AutoFocusCompleted -= cam_AutoFocusCompleted;
            }
        }

        // Update the UI if initialization succeeds.
        void cam_Initialized(object sender, Microsoft.Devices.CameraOperationCompletedEventArgs e)
        {
            if (e.Succeeded)
            {
                this.Dispatcher.BeginInvoke(delegate()
                {
                    // Write message.
                    txtDebug.Text = "Camera initialized.";

                });
            }
        }

        // Ensure that the viewfinder is upright in LandscapeRight.
        protected override void OnOrientationChanged(OrientationChangedEventArgs e)
        {
            if (cam != null)
            {
                // LandscapeRight rotation when camera is on back of device.
                int landscapeRightRotation = 180;

                // Change LandscapeRight rotation for front-facing camera.
                if (cam.CameraType == CameraType.FrontFacing) landscapeRightRotation = -180;

                // Rotate video brush from camera.
                if (e.Orientation == PageOrientation.LandscapeRight)
                {
                    // Rotate for LandscapeRight orientation.
                    viewfinderBrush.RelativeTransform =
                        new CompositeTransform() { CenterX = 0.5, CenterY = 0.5, Rotation = landscapeRightRotation };
                }
                else
                {
                    // Rotate for standard landscape orientation.
                    viewfinderBrush.RelativeTransform =
                        new CompositeTransform() { CenterX = 0.5, CenterY = 0.5, Rotation = 0 };
                }
            }

            base.OnOrientationChanged(e);
        }


        // Provide auto-focus in the viewfinder.
        private void focus_Clicked(object sender, System.Windows.RoutedEventArgs e)
        {
            if (cam.IsFocusSupported == true)
            {
                //Focus when a capture is not in progress.
                try
                {
                    cam.Focus();
                }
                catch (Exception focusError)
                {
                    // Cannot focus when a capture is in progress.
                    this.Dispatcher.BeginInvoke(delegate()
                    {
                        txtDebug.Text = focusError.Message;
                    });
                }
            }
            else
            {
                // Write message to UI.
                this.Dispatcher.BeginInvoke(delegate()
                {
                    txtDebug.Text = "Camera does not support programmable auto focus.";
                });
            }
        }

        void cam_AutoFocusCompleted(object sender, CameraOperationCompletedEventArgs e)
        {
            Deployment.Current.Dispatcher.BeginInvoke(delegate()
            {
                // Write message to UI.
                txtDebug.Text = "Auto focus has completed.";

                // Hide the focus brackets.
                focusBrackets.Visibility = Visibility.Collapsed;

            });
        }

        // Provide touch focus in the viewfinder.
        void focus_Tapped(object sender, GestureEventArgs e)
        {
            if (cam != null)
            {
                if (cam.IsFocusAtPointSupported == true)
                {
                    try
                    {
                        // Determine location of tap.
                        Point tapLocation = e.GetPosition(viewfinderCanvas);

                        // Position focus brackets with estimated offsets.
                        focusBrackets.SetValue(Canvas.LeftProperty, tapLocation.X - 30);
                        focusBrackets.SetValue(Canvas.TopProperty, tapLocation.Y - 28);

                        // Determine focus point.
                        double focusXPercentage = tapLocation.X / viewfinderCanvas.Width;
                        double focusYPercentage = tapLocation.Y / viewfinderCanvas.Height;

                        // Show focus brackets and focus at point
                        focusBrackets.Visibility = Visibility.Visible;
                        cam.FocusAtPoint(focusXPercentage, focusYPercentage);

                        // Write a message to the UI.
                        this.Dispatcher.BeginInvoke(delegate()
                        {
                            txtDebug.Text = String.Format("Camera focusing at point: {0:N2} , {1:N2}", focusXPercentage, focusYPercentage);
                        });
                    }
                    catch (Exception focusError)
                    {
                        // Cannot focus when a capture is in progress.
                        this.Dispatcher.BeginInvoke(delegate()
                        {
                            // Write a message to the UI.
                            txtDebug.Text = focusError.Message;
                            // Hide focus brackets.
                            focusBrackets.Visibility = Visibility.Collapsed;
                        });
                    }
                }
                else
                {
                    // Write a message to the UI.
                    this.Dispatcher.BeginInvoke(delegate()
                    {
                        txtDebug.Text = "Camera does not support FocusAtPoint().";
                    });
                }
            }
        }

        private void changeRes_Clicked(object sender, System.Windows.RoutedEventArgs e)
        {
            // Variables
            IEnumerable<Size> resList = cam.AvailableResolutions;
            int resCount = resList.Count<Size>();
            Size res;

            // Poll for available camera resolutions.
            for (int i = 0; i < resCount; i++)
            {
                res = resList.ElementAt<Size>(i);
            }

            // Set the camera resolution.
            res = resList.ElementAt<Size>((currentResIndex + 1) % resCount);
            cam.Resolution = res;
            currentResIndex = (currentResIndex + 1) % resCount;

            // Update the UI.
            txtDebug.Text = String.Format("Setting capture resolution: {0}x{1}", res.Width, res.Height);
            ResButton.Content = "R" + res.Width;
        }

        private void DrawingSurface_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_d3dInterop == null)
            {
                m_d3dInterop = new Direct3DInterop();

                // Set window bounds in dips
                m_d3dInterop.WindowBounds = new Windows.Foundation.Size(
                    (float)DrawingSurface.ActualWidth,
                    (float)DrawingSurface.ActualHeight
                    );

                // Set native resolution in pixels
                m_d3dInterop.NativeResolution = new Windows.Foundation.Size(
                    (float)Math.Floor(DrawingSurface.ActualWidth * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f),
                    (float)Math.Floor(DrawingSurface.ActualHeight * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f)
                    );

                // Set render resolution to the full native resolution
                m_d3dInterop.RenderResolution = m_d3dInterop.NativeResolution;

                // Hook-up native component to DrawingSurface
                DrawingSurface.SetContentProvider(m_d3dInterop.CreateContentProvider());
                DrawingSurface.SetManipulationHandler(m_d3dInterop);
            }
        }

        // frame pump
        void PumpFrames()
        {
            // Create capture buffer.
            byte[] YPx = new byte[(int)cam.PreviewResolution.Width * (int)cam.PreviewResolution.Height];

            try
            {
                PhotoCamera phCam = (PhotoCamera)cam;

                while (pumpFrames)
                {
                    pauseFramesEvent.WaitOne();

                    // Copies the current viewfinder frame into a buffer for further manipulation.
                    phCam.GetPreviewBufferY(YPx);

                    // DO STUFF HERE

                    pauseFramesEvent.Reset();
                }

            }
            catch (Exception e)
            {
                this.Dispatcher.BeginInvoke(delegate()
                {
                    // Display error message.
                    txtDebug.Text = e.Message;
                });
            }
        }
    }
}