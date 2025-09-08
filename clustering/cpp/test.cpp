#include <cstddef>
#include <iostream>
#include <ostream>
#include <vector>

#include "cluster.hpp"
#include "matrix.hpp"
#include "simsearch.hpp"
#include "utils.hpp"

void print_vector(const std::vector<size_t> &v) {
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << v[i] << ",";
  }
  std::cout << std::endl;
}

int main(int args, char *vargs[]) {
  // Matrix mat(2, 2);
  // float x[] = {1, 2};
  // mat.show();
  // vec_add(x, x, 2);
  // vec_divide(x, 3, 2);
  // mat.setRow(0, x);
  // mat.show();
  // return 0;
  std::string str_data("../../data_eda.npy");
  Matrix mat(str_data);
  const float *row = mat.getRow(999);

  // print_array(row, mat.getDim());
  Cluster cluster(mat, 8);
  cluster.applyClustering();
  std::cout << "--- print clusters" << std::endl;
  cluster.print_clusters();
  // std::vector<size_t> inds_0 = cluster.getInds(0);
  // std::vector<size_t> inds_1 = cluster.getInds(1);
  // print_vector(inds_0);
  // print_vector(inds_1);

  int k = 8;
  Cluster c = Cluster(mat, k);
  c.applyClustering();
  const float *j = c.getCentroid(1);
  SimSearch s = SimSearch(mat, k);
  std::vector<size_t> a = s.search_without(mat.getRow(0), 100);
  print_vector(a);

  printf("\n");

  std::cout << j[0] << "\n" << j[1] << "\n" << j[16] << std::endl;

  return 0;
}
