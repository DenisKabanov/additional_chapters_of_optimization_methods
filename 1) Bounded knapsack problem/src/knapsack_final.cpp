#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

struct Item {
  int64_t weight;
  int64_t price;
  int64_t count;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int64_t items_count;
  int64_t knapsack_max_weight;
  std::cin >> items_count >> knapsack_max_weight;
  std::vector<Item> items(items_count);
  for (int64_t i = 0; i < items_count; ++i) {
    std::cin >> items[i].weight >> items[i].price >> items[i].count;
  }

  std::vector<Item> expanded_items;
  for (const Item& item : items) {
    int64_t count = item.count;
    int64_t k_ = 1;

    while (count > 0) {
      int64_t take = std::min(k_, count);
      int64_t weight_ = item.weight * take;
      int64_t price_ = item.price * take;

      expanded_items.emplace_back(Item{weight_, price_, 1});
      count -= take;
      k_ *= 2;
    }
  }

  std::vector<int64_t> DP(knapsack_max_weight + 1);
  for (const Item& item : expanded_items) {
    for (int64_t weight = knapsack_max_weight; weight >= item.weight; --weight) {
      DP[weight] = std::max(DP[weight], DP[weight - item.weight] + item.price);
    }
  }
  std::cout << DP[knapsack_max_weight];

  return 0;
}
