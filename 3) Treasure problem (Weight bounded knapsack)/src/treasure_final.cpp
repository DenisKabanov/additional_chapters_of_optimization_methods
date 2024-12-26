#include <algorithm>
#include <cassert>
#include <iostream>
#include <stack>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

struct Item {
  int64_t weight;
  int64_t price;
  int pos;
};

class Solution {
 public:
  int64_t weight = 0;
  int64_t price = 0;
  vector<int> items_ids;

  Solution() {
    weight = 0;
    price = 0;
    items_ids = vector<int>();
  }

  Solution(int64_t weight_, int64_t price_, vector<int>& items_ids_) {
    weight = weight_;
    price = price_;
    items_ids = items_ids_;
  }

  void update_solution(int64_t weight_new, int64_t price_new,
                       vector<int>& items_ids_new) {
    weight = weight_new;
    price = price_new;
    items_ids = items_ids_new;
  }

  void print_solution() {
    if (items_ids.size() == 0) {
      cout << 0;
    } else {
      cout << items_ids.size() << endl;
      sort(items_ids.begin(), items_ids.end());
      for (int id : items_ids) cout << id << " ";
    }
    cout << endl;
  }

  friend bool operator==(const Solution& lhs, const Solution& rhs) {
    return (lhs.price == rhs.price) && (lhs.weight == rhs.weight) &&
           (lhs.items_ids == rhs.items_ids);
  }
};

void generate_solutions(const vector<Item>& data, vector<Solution>& solutions,
                        int64_t bound) {
  int items_count = data.size();

  int total_combinations = 1 << items_count;

  int64_t solution_weight;
  int64_t solution_price;
  vector<int> solution_items;

  for (int mask = 0; mask < total_combinations; ++mask) {
    solution_weight = 0;
    solution_price = 0;
    solution_items.clear();
    for (int i = 0; i < items_count; ++i) {
      if (mask & (1 << i)) {
        solution_weight += data[i].weight;
        solution_price += data[i].price;
        solution_items.push_back(data[i].pos);
      }
    }

    if (solution_weight > bound) {
      continue;
    } else {
      solutions.emplace_back(
          Solution(solution_weight, solution_price, solution_items));
    }
  }
}

template <typename T>
void print_items_vector(vector<T>& vector) {
  for (auto item : vector) {
    cout << "weight: " << item.weight << ", price: " << item.price
         << ", pos: " << item.pos << " " << endl;
  }
}

void print_solutions_vector(vector<Solution>& vector) {
  cout << "================" << endl;
  for (auto item : vector) {
    cout << "solution weight: " << item.weight
         << ", solution price: " << item.price << " " << endl;
    cout << "Items: ";
    for (auto id : item.items_ids) {
      cout << id << " ";
    }
    cout << endl << "-----------------" << endl;
  }
}

class MaxStack {
 private:
  std::stack<Solution> stack_data;
  std::stack<Solution> stack_max;

 public:
  MaxStack() {}

  ~MaxStack() {}

  void push(Solution value) {
    stack_data.push(value);
    if (stack_max.empty() || value.price >= stack_max.top().price) {
      stack_max.push(value);
    }
  }

  void pop() {
    assert(!stack_data.empty());

    if (stack_data.top() == stack_max.top()) {
      stack_max.pop();
    }
    stack_data.pop();
  }

  Solution top() const {
    assert(!stack_data.empty());

    return stack_data.top();
  }

  Solution get_max() const {
    assert(!stack_max.empty());

    return stack_max.top();
  }

  size_t get_size() const { return stack_data.size(); }

  bool empty() const { return stack_data.empty(); }
};

class MaxQueue {
 private:
  MaxStack stack_head;
  MaxStack stack_tail;

 public:
  MaxQueue() {}

  ~MaxQueue() {}

  void push(Solution value) { stack_tail.push(value); }

  void move_elements() {
    while (!stack_tail.empty()) {
      stack_head.push(stack_tail.top());
      stack_tail.pop();
    }
  }

  void pop() {
    if (stack_head.empty()) {
      assert(!stack_tail.empty());

      this->move_elements();
    }
    stack_head.pop();
  }

  Solution top() {
    if (stack_head.empty()) {
      assert(!stack_tail.empty());

      this->move_elements();
    }
    return stack_head.top();
  }

  Solution get_max() {
    if (stack_head.empty()) {
      assert(!stack_tail.empty());

      this->move_elements();
    }

    Solution max_value_head = stack_head.get_max();
    if (!stack_tail.empty()) {
      Solution max_value_tail = stack_tail.get_max();
      if (max_value_tail.price > max_value_head.price) {
        return max_value_tail;
      }
    }
    return max_value_head;
  }

  size_t get_size() const {
    return stack_head.get_size() + stack_tail.get_size();
  }

  bool empty() const { return stack_head.empty() && stack_tail.empty(); }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int count_items;
  int64_t left_bound, right_bound;
  cin >> count_items >> left_bound >> right_bound;
  vector<Item> items(count_items);
  for (int i = 0; i < count_items; ++i) {
    cin >> items[i].weight >> items[i].price;
    items[i].pos = i + 1;
  }

  Solution best_sol;
  if (count_items == 1) {
    if ((left_bound <= items[0].weight) && (items[0].weight <= right_bound)) {
      vector<int> sol_ = {items[0].pos};
      best_sol.update_solution(items[0].weight, items[0].price, sol_);
    }
    best_sol.print_solution();

    return 0;
  }

  std::sort(items.begin(), items.end(),
            [](auto& left, auto& right) { return left.weight < right.weight; });

  int middle = count_items / 2;
  vector<Item> items_left(items.begin(), items.begin() + middle);
  vector<Item> items_right(items.begin() + middle, items.end());

  vector<Solution> solutions_left, solutions_right;
  solutions_left.reserve(1 << solutions_left.size());
  solutions_left.reserve(1 << solutions_right.size());
  generate_solutions(items_left, solutions_left, right_bound);
  generate_solutions(items_right, solutions_right, right_bound);

  std::sort(solutions_left.begin(), solutions_left.end(),
            [](const Solution& left, const Solution& right) {
              if (left.weight == right.weight) {
                return left.price > right.price;
              }
              return left.weight < right.weight;
            });
  std::sort(solutions_right.begin(), solutions_right.end(),
            [](const Solution& left, const Solution& right) {
              if (left.weight == right.weight) {
                return left.price > right.price;
              }
              return left.weight < right.weight;
            });

  MaxQueue Queue;

  int left_sol_id = static_cast<int>(solutions_left.size()) - 1;
  int64_t consumed_weight = 0;
  for (Solution& right_sol : solutions_right) {
    consumed_weight = right_sol.weight;

    while (left_sol_id > -1) {
      if (solutions_left[left_sol_id].weight + consumed_weight >= left_bound) {
        if (solutions_left[left_sol_id].weight + consumed_weight <=
            right_bound) {
          Queue.push(solutions_left[left_sol_id]);
        }
        left_sol_id--;
      } else {
        break;
      }
    }

    while ((!Queue.empty()) &&
           (Queue.top().weight + consumed_weight > right_bound)) {
      Queue.pop();
    }

    Solution best_left_sol;
    if (!Queue.empty()) {
      best_left_sol = Queue.get_max();
    }

    if ((best_left_sol.weight + consumed_weight >= left_bound) &&
        (best_left_sol.weight + consumed_weight <= right_bound) &&
        (best_left_sol.price + right_sol.price > best_sol.price)) {
      vector<int> items_ids = best_left_sol.items_ids;
      items_ids.insert(items_ids.end(), right_sol.items_ids.begin(),
                       right_sol.items_ids.end());
      best_sol.update_solution(best_left_sol.weight + consumed_weight,
                               best_left_sol.price + right_sol.price,
                               items_ids);
    }
  }
  best_sol.print_solution();

  return 0;
}
