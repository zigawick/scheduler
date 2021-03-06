/// \file kernel/tree_solver.cpp

#include "src/kernel/tree_solver.h"

#include <iostream>

std::vector<std::unordered_set<uid>> tree_solver::calculate (const std::vector<tree_node> &input_data)
{
  construct_adjacency_list (input_data);
  set_colors ();

  return get_result_timetable_set ();
}

void tree_solver::construct_adjacency_list (const std::vector<tree_node> &input_data)
{
  auto vector_size = input_data.size ();
  for (size_t curr_node_idx = 0; curr_node_idx < vector_size; curr_node_idx++)
    {
      std::vector<tree_node> adjacted_nodes;
      tree_node current_element = input_data[curr_node_idx];
      for (size_t other_node_idx = 0; other_node_idx < vector_size; other_node_idx++)
        {
          auto node = input_data[other_node_idx];
          if (other_node_idx != curr_node_idx && current_element.is_neighbour (node))
            adjacted_nodes.push_back (node);
        }
      m_adjacency_list[current_element.ed_year_id] = adjacted_nodes;
      m_colors[current_element.ed_year_id] = -1;
    }
}

void tree_solver::set_colors ()
{
  std::vector<bool> availible_colors (m_adjacency_list.size (), false);
  unsigned int color = 0;
  for (const auto &element : m_adjacency_list)
    {
      // set color of element
      for (const auto &neighbour_elem : element.second)
        {
          int neighbour_color = m_colors[neighbour_elem.ed_year_id];
          if (neighbour_color != -1)
            availible_colors[neighbour_color] = true;

          for (color = 0; color < availible_colors.size (); color++)
            {
              if (!availible_colors[color])
                break;
            }
        }
      m_colors[element.first] = color;
    }
  m_colors_count = color + 1;
//  /// DEBUG INFO
//  for (const auto &elem : m_adjacency_list)
//    {
//      std::cout << "(" << elem.first.group_id << " " << elem.first.room_id << " " << elem.first.teacher_id << ") Color="
//                << m_colors[elem.first] << std::endl;
//      print_vector (elem.second);
//    }
}

std::vector<std::unordered_set<uid>> tree_solver::get_result_timetable_set ()
{
  std::vector<std::unordered_set<uid>> result (m_colors_count);
  for (const auto &it : m_colors)
    result[it.second].insert (it.first);
  return result;
}
