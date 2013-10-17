#include "model.h"

#include <vector>
using namespace std;

class Marker {
 public:
   Model addModel(Model model);
   void translate(float dx, float dy);
   void scale(float scaleAmount);
   void rotate(float angle);
 private:
   enum type { AR_MARKER, QR_CODE, IMAGE };
    void *trackingData;
    vector<Model> models;
};
