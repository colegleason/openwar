#include "marker.h"

/**
 * The Marker class represents some object we are attempting to track
 * and the models associated with that object.
 */
Marker::Marker(Marker::type type, void* trackingData) {
    this.type = type;
    this.trackingData = trackingData
}

/**
 * Adds a model to this tracking marker.
 */
Model Marker::addModel(Model model) {
    this.models.push_back(model);
    return model;
}

/**
 * Translate all models attached to this marker by dx, dy.
 */
void translate(float dx, float dy) {
    vector<Model>::iterator curr;
    for(curr = this.models.begin(); curr != this.models.end(); curr++) {
	curr.translate(dx, dy);
    }
}

/**
 * Scale all models attached to this marker by scaleAmount.
 */
void scale(float scaleAmount) {
    vector<Model>::iterator curr;
    for(curr = this.models.begin(); curr != this.models.end(); curr++) {
	curr.scale(scaleAmount);
    }
}

/**
 * Rotate all models about the marker's origin by angle.'
 */
void rotate(float angle) {
    vector<Model>::iterator curr;
    for(curr = this.models.begin(); curr != this.models.end(); curr++) {
	// TODO: Rotate around origin of Marker
	curr.rotate(angle);
    }
}
