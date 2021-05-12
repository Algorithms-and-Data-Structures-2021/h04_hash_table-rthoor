#include "hash_table.hpp"

#include <stdexcept>

namespace itis {

  int HashTable::hash(int key) const {
    return utils::hash(key, static_cast<int>(buckets_.size()));
  }

  HashTable::HashTable(int capacity, double load_factor) : load_factor_{load_factor} {
    if (capacity <= 0) {
      throw std::logic_error("hash table capacity must be greater than zero");
    }

    if (load_factor <= 0.0 || load_factor > 1.0) {
      throw std::logic_error("hash table load factor must be in range [0...1]");
    }

    // Tip: allocate hash-table buckets
    buckets_.resize(capacity);
    num_keys_ = 0;
  }

  std::optional<std::string> HashTable::Search(int key) const {
    // Tip: compute hash code (index) and use linear search

      int index = hash(key);
      for (auto pair : buckets_[index]){
          if (pair.first == key) {
              return pair.second;
          }
      }
      return std::nullopt;
  }

  void HashTable::Put(int key, const std::string &value) {
    // Tip 1: compute hash code (index) to determine which bucket to use
    // Tip 2: consider the case when the key exists (read the docs in the header file)

    int index = hash(key);
    std::pair <int, std::string> pair (key, value);
    if(index > buckets_.size()-1){
        buckets_.resize(index+1);
    }
    buckets_[index].push_back(pair);
    num_keys_++;



    if (static_cast<double>(num_keys_) / buckets_.size() >= load_factor_) {
      // Tip 3: recompute hash codes (indices) for key-value pairs (create a new hash-table)
      // Tip 4: use utils::hash(key, size) to compute new indices for key-value pairs

        auto newBuckets = new std::vector<Bucket>[buckets_.size()*kGrowthCoefficient];
        for(int j = 0; j < buckets_.size(); j++){
            for (auto pair : buckets_[j]){
                int index = hash(pair.first);
                newBuckets[index][0].push_back(pair);
            }
        }
        buckets_.clear();
        buckets_ = *newBuckets;
    }
  }

  std::optional<std::string> HashTable::Remove(int key) {
    // Tip 1: compute hash code (index) to determine which bucket to use
    // TIp 2: find the key-value pair to remove and make a copy of value to return

    int index = hash(key);
    for (auto pair : buckets_[index]){
        if(pair.first == key){
            std::pair <int, std::string> pair (key, pair.second);
            //std::optional<std::string> res = make_optional(pair.second);
            buckets_[index].remove(pair);
            return pair.second;
        }
    }
    return std::nullopt;
  }

  bool HashTable::ContainsKey(int key) const {
    // Note: uses Search(key) which is not initially implemented
    return Search(key).has_value();
  }

  bool HashTable::empty() const {
    return size() == 0;
  }

  int HashTable::size() const {
    return num_keys_;
  }

  int HashTable::capacity() const {
    return static_cast<int>(buckets_.size());
  }

  double HashTable::load_factor() const {
    return load_factor_;
  }

  std::unordered_set<int> HashTable::keys() const {
    std::unordered_set<int> keys(num_keys_);
    for (const auto &bucket : buckets_) {
      for (const auto &[key, _] : bucket) {
        keys.insert(key);
      }
    }
    return keys;
  }

  std::vector<std::string> HashTable::values() const {
    std::vector<std::string> values;
    for (const auto &bucket : buckets_) {
      for (const auto &[_, value] : bucket) {
        values.push_back(value);
      }
    }
    return values;
  }

}  // namespace itis