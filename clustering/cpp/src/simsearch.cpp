#include "simsearch.hpp"

#include <vector>

#include "utils.hpp"

SimSearch::SimSearch(const Matrix &data, const Matrix &clusters)
    : mat_data(data), mat_clusters(clusters) {}

std::vector<size_t> SimSearch::search_without(const float *query, size_t m) {
  size_t n = mat_data.getN();
  size_t dim = mat_data.getDim();
  std::vector<float> dist(n);

  for (size_t i = 0; i < n; ++i) {
    const float *row = mat_data.getRow(i);
    dist[i] = vec_compute_distance(query, row, dim);
  }

  size_t *sorted_indices = argsort(dist.data(), n);

  std::vector<size_t> result;
  for (size_t i = 0; i < m && i < n; ++i) {
    result.push_back(sorted_indices[i]);
  }
  delete[] sorted_indices;
  return result;
}