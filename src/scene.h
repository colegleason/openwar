#include "marker.h"
#include <vector>
using namespace std;

class Scene {
 public:
    Marker addTrackingMarker(Marker marker);
    bool activate();
    bool deactivate();
 private:
    vector<Marker> markers;
    bool active = false;
};
