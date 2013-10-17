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
    enum type { TYPE1, TYPE2 };
    void *trackingData;
    vector<Model> models;
};
