# SimSearch

The `SimSearch` class is designed to perform similarity searches on a dataset. It provides two methods for searching: a brute-force search and a search that uses clustering to accelerate the process. This document provides a detailed explanation of the class, its methods, and their usage.

## Class Definition

The `SimSearch` class is defined in `clustering/cpp/include/simsearch.hpp`.

```cpp
class SimSearch {
 private:
  const Matrix &mat_data;      // data to be processed
  const Matrix &mat_clusters;  // centroids to be used
  std::vector<std::vector<size_t>> cluster_members;

 public:
  SimSearch(const Matrix &data, const Matrix &clusters);
  std::vector<size_t> search_with_clusters(const float *query, size_t top_k);
  std::vector<size_t> search_without(const float *query, size_t top_k);
};
```

### Member Variables

- `mat_data`: A constant reference to a `Matrix` object that holds the dataset.
- `mat_clusters`: A constant reference to a `Matrix` object that holds the cluster centroids.
- `cluster_members`: A `std::vector` of `std::vector<size_t>>` that stores the indices of the data points belonging to each cluster.

## Constructor

The constructor of the `SimSearch` class takes the dataset and the cluster centroids as input and initializes the `cluster_members` vector.

```cpp
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
```

The constructor iterates through each data point in the dataset and assigns it to the closest cluster. The index of the data point is then stored in the `cluster_members` vector at the index corresponding to the cluster it was assigned to.

## Search Methods

The `SimSearch` class provides two methods for performing similarity searches.

### `search_without`

This method performs a brute-force search over the entire dataset.

```cpp
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
```

It calculates the distance between the query vector and every data point in the dataset. It then sorts the distances and returns the indices of the `m` closest data points.

### `search_with_clusters`

This method uses the pre-computed clusters to speed up the search.

```cpp
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
```

This method first finds the closest cluster to the query vector. Then, it performs a search only on the data points that belong to that cluster. This can significantly reduce the search time, especially for large datasets.

## Usage

To use the `SimSearch` class, you first need to create a `Matrix` object for your dataset and your cluster centroids. Then, you can create a `SimSearch` object and use its search methods.

```cpp
// Assuming mat_data and mat_clusters are already initialized
SimSearch sim_search(mat_data, mat_clusters);

// Define a query vector
float query[] = {1.0, 2.0, 3.0};

// Perform a search with clusters
std::vector<size_t> results_with_clusters = sim_search.search_with_clusters(query, 5);

// Perform a search without clusters
std::vector<size_t> results_without_clusters = sim_search.search_without(query, 5);
```
