#include<algorithm>
#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<vector>
#include <FileTokenizer.h>

using Tree = int;

size_t get_viewing_distance_left_up(size_t tree_idx, const std::vector<Tree>& trees)
{
    int tree_size = trees[tree_idx];
    size_t viewing_distance = 0;
    for(ssize_t i = tree_idx-1; i >=0; i--)
    {
        viewing_distance++;
        if(tree_size <= trees[i])
        {
            break;
        }
    }

    return viewing_distance;
}

size_t get_viewing_distance_right_down(size_t tree_idx, const std::vector<Tree>& trees)
{
    int tree_size = trees[tree_idx];
    size_t viewing_distance = 0;
    for(size_t i = tree_idx+1; i < trees.size(); i++)
    {
        viewing_distance++;
        if(tree_size <= trees[i])
        {
            break;
        }
    }
    return viewing_distance;
}

bool is_visible(size_t tree_row_idx, 
        size_t tree_col_idx,
        const std::vector<Tree>& tree_row,
        const std::vector<Tree>& tree_col)
{
    if(tree_row_idx == 0 ||
         tree_col_idx == 0 ||
         tree_row_idx == tree_row.size()-1 ||
         tree_col_idx == tree_col.size()-1)
    {
        return true;
    }
    return (get_viewing_distance_left_up(tree_row_idx, tree_row) == tree_row_idx && tree_row[tree_row_idx] > tree_row[0]) ||
        ( (get_viewing_distance_right_down(tree_row_idx, tree_row) == tree_row.size() - tree_row_idx - 1) && tree_row[tree_row_idx] > tree_row.back()) ||
        (get_viewing_distance_left_up(tree_col_idx, tree_col) == tree_col_idx && tree_col[tree_col_idx] > tree_col[0]) ||
        ( (get_viewing_distance_right_down(tree_col_idx, tree_col) ==  tree_col.size() - tree_col_idx - 1) && tree_col[tree_col_idx] > tree_col.back());
}

size_t get_scenic_score(size_t tree_row_idx, 
        size_t tree_col_idx,
        const std::vector<Tree>& tree_row,
        const std::vector<Tree>& tree_col)
{
    return get_viewing_distance_left_up(tree_row_idx, tree_row) *
     get_viewing_distance_right_down(tree_row_idx, tree_row)  *
     get_viewing_distance_left_up(tree_col_idx, tree_col) *
     get_viewing_distance_right_down(tree_col_idx, tree_col) ;
}


std::vector<Tree> get_col(const std::vector<std::vector<Tree>>& tree_map, size_t col_index)
{
    std::vector<Tree> col;
    for(const auto& tree_row : tree_map)
    {
        col.emplace_back(tree_row[col_index]);
    }
    return col;
}

std::vector<Tree> parse_tree_row( const std::string& row_str)
{
    std::vector<Tree> row;
    for(const auto& tree_char : row_str)
    {
        row.emplace_back(tree_char-'0');
    }

    return row;
}

int main()
{
    auto tokenizer = FileTokenizer("../input/day8.txt", '\n');
    std::vector<std::vector<Tree>> tree_map;
    for(std::string tree_row_str = tokenizer.next().value_or("");
         !tree_row_str.empty();
         tree_row_str = tokenizer.next().value_or(""))
    {
       tree_map.emplace_back(parse_tree_row(tree_row_str));
    }

    std::vector<std::vector<Tree>> tree_col_map;
    for(size_t i = 0; i < tree_map.size(); i++)
    {
        tree_col_map.emplace_back(get_col(tree_map, i));
    }

    size_t total_visible = 0;
    std::vector<size_t> scenic_scores;
    for(size_t i = 0; i < tree_map.size(); i++)
    {
        for(size_t j = 0; j < tree_map[i].size(); j++)
        {
            if(is_visible(j,i,tree_map[i], tree_col_map[j]))
            {
                total_visible++;
            }
            scenic_scores.emplace_back(get_scenic_score(j,i,tree_map[i], tree_col_map[j]));
        }
    }
    std::sort(scenic_scores.begin(),scenic_scores.end(), std::greater<size_t>());
    std::cout << "part1: " << total_visible << std::endl;
    std::cout << "part2: " << scenic_scores[0] << std::endl;
    return 1;
}