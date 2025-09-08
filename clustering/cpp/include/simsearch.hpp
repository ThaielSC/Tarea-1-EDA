#ifndef _SIM_SEARCH_HPP
#define _SIM_SEARCH_HPP
#include <vector>

#include "matrix.hpp"

/**
 * @class SimSearch
 * @brief A class for performing similarity search on a dataset, with an option to use clustering to speed up the search.
 */
class SimSearch {
 private:
  const Matrix &mat_data;      ///< A constant reference to the data matrix.
  const Matrix &mat_clusters;  ///< A constant reference to the clusters matrix (centroids).
  std::vector<std::vector<size_t>> cluster_members; ///< A vector of vectors to store the members of each cluster.

 public:
  /**
   * @brief Constructs a SimSearch object.
   * @param data The dataset to be searched.
   * @param clusters The centroids of the clusters.
   */
  SimSearch(const Matrix &data, const Matrix &clusters);

  /**
   * @brief Searches for the top_k most similar items to a query vector, using clusters to narrow down the search space.
   * @param query The query vector.
   * @param top_k The number of similar items to return.
   * @return A vector of indices of the top_k most similar items in the dataset.
   */
  std::vector<size_t> search_with_clusters(const float *query, size_t top_k);

  /**
   * @brief Searches for the top_k most similar items to a query vector by scanning the entire dataset.
   * @param query The query vector.
   * @param top_k The number of similar items to return.
   * @return A vector of indices of the top_k most similar items in the dataset.
   */
  std::vector<size_t> search_without(const float *query, size_t top_k);
};

#endif
