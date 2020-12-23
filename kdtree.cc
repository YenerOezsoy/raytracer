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
        max_left = {length_x / 2, min[1], min[2]};
        min_right = {length_x / 2, max[1], max[2]};
    } else {
        max_left = {min[0], length_y / 2, min[2]};
        min_right = {max[0], length_y / 2, max[2]};
    }

    left = BoundingBox(min_left, max_left);
    right = BoundingBox(min_right, max_right);

    // to here
}

bool BoundingBox::contains(Vector<FLOAT, 3> v) {
  // from here
  if (v[0] < min[0] && max[0] < v[0]) {
      return false;
  } else if (v[1] < min[1] && max[1] < v[1]) {
      return false;
  } else if (v[2] < min[2] && max[2] < v[2]) {
      return false;
  }
  return true; // to avoid a warning, delete this in your code
  // to here
}

bool BoundingBox::contains(Triangle<FLOAT> *triangle) {
  // from here
    if(contains(triangle->p1) || contains(triangle->p2) || contains(triangle->p3)) {
        return true;
    }
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
  tree->triangles = triangles;
  if (triangles.size() > MAX_TRIANGLES_PER_LEAF) {
      BoundingBox left_box, right_box;
      tree->box.split(left_box, right_box);

      KDTree left_tree;
      KDTree right_tree;

      tree->left = &left_tree;
      tree->right = &right_tree;

      left_tree.box = left_box;
      right_tree.box = right_box;

      std::vector<Triangle<FLOAT> *> triangles_left;
      std::vector<Triangle<FLOAT> *> triangles_right;

      for (auto it = triangles.begin(); it != triangles.end(); it++) {
          if (left_box.contains(*it) && right_box.contains(*it)) {
              //do nothing, triangle will be stored in this node
          } else if (left_box.contains(*it)) {
              triangles_left.push_back(*it);
              triangles.erase(it--);
          } else if (right_box.contains(*it)) {
              triangles_right.push_back(*it);
              triangles.erase(it--);
          }
      }

      buildTree(tree, triangles_left);
      buildTree(tree, triangles_right);

  }
  // to here
  return tree;
}

KDTree * KDTree::buildTree(std::vector< Triangle<FLOAT> *> & triangles)  {
  KDTree *root = new KDTree();
  // from here
  root->buildTree(root,triangles);
  // to here
  return root;
}

bool KDTree::hasNearestTriangle(Vector<FLOAT,3> eye, Vector<FLOAT,3> direction, Triangle<FLOAT> *  & nearest_triangle, FLOAT &t, FLOAT &u, FLOAT &v, FLOAT minimum_t) {
  // from here
  std::vector<Triangle<FLOAT>*> temp_triangles;
  KDTree *actual_node = this;
  while(actual_node->left != nullptr && actual_node->right != nullptr) {
      temp_triangles.insert(std::end(temp_triangles), std::begin(actual_node->triangles), std::end(actual_node->triangles));
      if (actual_node->left->box.intersects(eye,direction)) {
          actual_node = actual_node->left;
      } else if (actual_node->right->box.intersects(eye,direction)) {
          actual_node = actual_node->right;
      }
  }
  temp_triangles.insert(std::end(temp_triangles), std::begin(actual_node->triangles), std::end(actual_node->triangles));
  minimum_t = INFINITY;
  for (Triangle<FLOAT> * triangle : temp_triangles) {
      if (triangle->intersects(eye,direction,t,u,v,minimum_t)) {
          if (t < minimum_t) {
              minimum_t = t;
              nearest_triangle = triangle;
          }
      }
  }
  // to here
  return nearest_triangle != nullptr;
}
