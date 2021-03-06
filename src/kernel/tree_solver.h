#ifndef TREE_SOLVER_H
#define TREE_SOLVER_H

/// \file kernel/tree_solver.h
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef int uid;

struct tree_node
{
  uid ed_year_id = 0;
  int group_id = 0;
  int teacher_id = 0;

  tree_node (int ed_year_uid, int g_id = -1, int t_id = -1)
    : ed_year_id (ed_year_uid), group_id (g_id), teacher_id (t_id)
  {
  }

  bool is_neighbour (const tree_node &rhs) const
  {
    return ((group_id == rhs.group_id) || (teacher_id == rhs.teacher_id));
  }
};

class tree_solver
{
public:
  std::vector<std::unordered_set<uid>> calculate (const std::vector<tree_node> &input_data);

private:
  void construct_adjacency_list (const std::vector<tree_node> &input_data);
  void set_colors ();
  std::vector<std::unordered_set<uid>> get_result_timetable_set ();

  /// Debug info
  ///
  void print_vector (const std::vector<tree_node> &vector);

private:
  std::unordered_map<uid, std::vector<tree_node>> m_adjacency_list;
  std::unordered_map<uid, int> m_colors;
  int m_colors_count;
};

#endif // TREE_SOLVER_H
