#include "kdtree.h"

BoundingBox::BoundingBox() { }

BoundingBox::BoundingBox(Vector<FLOAT,3> min, Vector<FLOAT,3> max) 
 : min(min), max(max) { }

void BoundingBox::split(BoundingBox & left, BoundingBox & right) {
  // from here
    float length_x = max[0] - min[0];
    float length_y = max[1] - min[1];

    Vector<FLOAT,3> min_left = min;
    Vector<FLOAT,3> max_left;
    Vector<FLOAT,3> min_right;
    Vector<FLOAT,3> max_right = max;

    if (length_x >= length_y) {
        max_left = {length_x / 2, min[1], min[2]}
        min_right = {length_x / 2, max[1], max[2]};
    } else {
        max_left = {min[0], length_y / 2, min[2]}
        min_right = {max[0], length_y / 2, max[2]};
    }

    BoundingBox left = BoundingBox(min_left, max_left);
    BoundingBox right = BoundingBox(min_right, max_right);

    // to here
}

bool BoundingBox::contains(Vector<FLOAT, 3> v) {
  // from here
  // TODO: your code
  return false; // to avoid a warning, delete this in your code
  // to here
}

bool BoundingBox::contains(Triangle<FLOAT> *triangle) {
  // from here
  // TODO: your code
  return false; // to avoid a warning, delete this in your code
  // to here
}

bool BoundingBox::intersects(Vector<FLOAT,3> eye, Vector<FLOAT, 3> direction) {
    // slab test implementation
    FLOAT tmin[3] = { (min[0] - eye[0]) / direction[0],
                      (min[1] - eye[1]) / direction[1],
                      (min[2] - eye[2]) / direction[2] };
    FLOAT tmax[3] = { (max[0] - eye[0]) / direction[0],
                      (max[1] - eye[1]) / direction[1],
                      (max[2] - eye[2]) / direction[2] };
    FLOAT tminimum = std::min(tmin[0], tmax[0]);
    FLOAT tmaximum = std::max(tmin[0], tmax[0]);
    tminimum = std::max(tminimum, std::min(tmin[1], tmax[1]) );
    tmaximum = std::min(tmaximum, std::max(tmin[1], tmax[1]) );
    tminimum = std::max(tminimum, std::min(tmin[2], tmax[2]) );
    tmaximum = std::min(tmaximum, std::max(tmin[2], tmax[2]) );

    return tmaximum >= tminimum;
}


KDTree::~KDTree() {
  delete left;
  delete right;
}

KDTree * KDTree::buildTree(KDTree * tree, std::vector< Triangle<FLOAT> *> & triangles) {
  // from here
  // TODO: your code
  // to here
  return tree;
}

KDTree *  KDTree::buildTree(std::vector< Triangle<FLOAT> *> & triangles)  {
  KDTree * root = new KDTree();
  // from here
  // TODO: your code
  // to here
  return root;
}

bool KDTree::hasNearestTriangle(Vector<FLOAT,3> eye, Vector<FLOAT,3> direction, Triangle<FLOAT> *  & nearest_triangle, FLOAT &t, FLOAT &u, FLOAT &v, FLOAT minimum_t) {
  // from here
  // TODO: your code
  // to here
  return nearest_triangle != nullptr;
}
