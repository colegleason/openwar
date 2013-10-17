class Model {
 public:
    void translate(int dx, int dy);
    void scale(int scaleAmount);
    void rotate(int angle);
 private:
    void *modelData;
    void *textureData;
};
