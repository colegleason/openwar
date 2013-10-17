#include "scene.h"

Marker Scene::addTrackingMarker(Marker marker) {
    this.markers.push_back(marker);
    return marker;
}

bool Scene::activate() {
    this.active = true;
}

bool Scene::deactivate() {
    this.active = false;
}
