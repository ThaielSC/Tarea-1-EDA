#include <vector>

#include "simsearch.hpp"
#include "utils.hpp"

SimSearch::SimSearch(const Matrix &data, const Matrix &clusters)
    : mat_data(data), mat_clusters(clusters) {
  size_t num_data_points = mat_data.getN();
  size_t num_clusters = mat_clusters.getN();
  size_t dim = mat_data.getDim();
  this->cluster_members.resize(num_clusters);

  for (size_t i = 0; i < num_data_points; ++i) {
    const float *data_point = mat_data.getRow(i);
    float min_dist = -1;
    size_t best_cluster_idx = 0;
    for (size_t j = 0; j < num_clusters; ++j) {
      const float *centroid = mat_clusters.getRow(j);
      float dist = vec_compute_distance(data_point, centroid, dim);
      if (min_dist < 0 || dist < min_dist) {
        min_dist = dist;
        best_cluster_idx = j;
      }
    }
    this->cluster_members[best_cluster_idx].push_back(i);
  }
}

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

std::vector<size_t> SimSearch::search_with_clusters(const float *query,
                                                    size_t top_k) {
  size_t num_clusters = mat_clusters.getN();
  size_t dim = mat_clusters.getDim();
  float min_dist = -1;
  size_t best_cluster_idx = 0;

  for (size_t i = 0; i < num_clusters; ++i) {
    const float *centroid = mat_clusters.getRow(i);
    float dist = vec_compute_distance(query, centroid, dim);
    if (min_dist < 0 || dist < min_dist) {
      min_dist = dist;
      best_cluster_idx = i;
    }
  }

  const std::vector<size_t> &candidate_indices =
      this->cluster_members[best_cluster_idx];
  size_t num_candidates = candidate_indices.size();
  if (num_candidates == 0) {
    return {};
  }

  std::vector<float> candidate_dists(num_candidates);
  for (size_t i = 0; i < num_candidates; ++i) {
    const float *row = mat_data.getRow(candidate_indices[i]);
    candidate_dists[i] = vec_compute_distance(query, row, dim);
  }

  size_t *sorted_candidate_indices_ptr =
      argsort(candidate_dists.data(), num_candidates);

  std::vector<size_t> result;
  for (size_t i = 0; i < top_k && i < num_candidates; ++i) {
    result.push_back(candidate_indices[sorted_candidate_indices_ptr[i]]);
  }

  delete[] sorted_candidate_indices_ptr;
  return result;
}
