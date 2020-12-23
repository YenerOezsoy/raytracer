#include "kdtree.h"

BoundingBox::BoundingBox() { }

BoundingBox::BoundingBox(Vector<FLOAT,3> min, Vector<FLOAT,3> max) 
 : min(min), max(max) { }

void BoundingBox::split(BoundingBox & left, BoundingBox & right) {
  // from here
    float length_x = max[0] - min[0];
    float length_y = max[1] - min[1];
    float length_z = max[2] - min[2];

    Vector<FLOAT,3> min_left = min;
    Vector<FLOAT,3> max_left;
    Vector<FLOAT,3> min_right;
    Vector<FLOAT,3> max_right = max;

    float mid;

    if (length_x >= length_y && length_x >= length_z) {
        mid = min[0] + (length_x / 2);
        max_left = {mid, max[1], max[2]};
        min_right = {mid, min[1], min[2]};
    } else if (length_y >= length_x && length_y >= length_z) {
        mid = min[1] + (length_y / 2);
        max_left = {max[0], mid, max[2]};
        min_right = {min[0], mid, min[2]};
    } else {
        mid = min[2] + (length_z / 2);
        max_left = {max[0], max[1], mid};
        min_right = {min[0], min[1], mid};
    }

    left = BoundingBox(min_left, max_left);
    right = BoundingBox(min_right, max_right);

    // to here
}

bool BoundingBox::contains(Vector<FLOAT, 3> v) {
  // from here
  return v[0] >= min[0] && v[0] <= max[0] &&
          v[1] >= min[1] && v[1] <= max[1] &&
          v[2] >= min[2] && v[2] <= max[2];
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
      KDTree *left_tree = new KDTree();
      KDTree *right_tree = new KDTree();

      tree->box.split(left_tree->box, right_tree->box);

      tree->left = left_tree;
      tree->right = right_tree;

      std::vector<Triangle<FLOAT> *> triangles_left;
      std::vector<Triangle<FLOAT> *> triangles_right;

      for (auto it = triangles.begin(); it != triangles.end(); it++) {
          if (left_tree->box.contains(*it) && right_tree->box.contains(*it)) {
              //do nothing, triangle will be stored in this node
          } else if (left_tree->box.contains(*it)) {
              triangles_left.push_back(*it);
              triangles.erase(it--);
          } else if (right_tree->box.contains(*it)) {
              triangles_right.push_back(*it);
              triangles.erase(it--);
          }
      }

      buildTree(left_tree, triangles_left);
      buildTree(right_tree, triangles_right);

  }
  // to here
  return tree;
}

KDTree * KDTree::buildTree(std::vector< Triangle<FLOAT> *> & triangles)  {
  KDTree *root = new KDTree();
  // from here
  Vector<FLOAT, 3> min, max = {0, 0, 0};
  for (Triangle<FLOAT> * triangle : triangles) {
      for (int i = 0; i < 3; i++) {
          if (min[i] > triangle->p1[i]) {
              min[i] = triangle->p1[i];
          }
          if (min[i] > triangle->p2[i]) {
              min[i] = triangle->p2[i];
          }
          if (min[i] > triangle->p3[i]) {
              min[i] = triangle->p3[i];
          }
          if (max[i] < triangle->p1[i]) {
              max[i] = triangle->p1[i];
          }
          if (max[i] < triangle->p2[i]) {
              max[i] = triangle->p2[i];
          }
          if (max[i] < triangle->p3[i]) {
              max[i] = triangle->p3[i];
          }
      }
  }
  root->box = *(new BoundingBox(min, max));
  root->buildTree(root,triangles);
  // to here
  return root;
}

bool KDTree::hasNearestTriangle(Vector<FLOAT,3> eye, Vector<FLOAT,3> direction, Triangle<FLOAT> *  & nearest_triangle, FLOAT &t, FLOAT &u, FLOAT &v, FLOAT minimum_t) {
  // from here
  std::vector<Triangle<FLOAT>*> temp_triangles;
  KDTree *actual_node = this;
  if (actual_node->box.intersects(eye, direction)) {
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
          stats.no_ray_triangle_intersection_tests++;
          if (triangle->intersects(eye,direction,t,u,v,minimum_t)) {
              stats.no_ray_triangle_intersections_found++;
              minimum_t = t;
              nearest_triangle = triangle;
          }
      }
  }

  // to here
  return nearest_triangle != nullptr;
}
