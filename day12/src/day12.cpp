#include<algorithm>
#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<limits>
#include<vector>
#include <FileTokenizer.h>


struct HeightMap
{
    std::vector<std::vector<char>> map;
    std::vector<std::vector<bool>> edges;
    std::pair<size_t,size_t> start;
    size_t start_point;
    std::pair<size_t,size_t> end;
    size_t end_point;

    HeightMap()
    {}
    void add_row(const std::string& row_str)
    {
        std::vector<char> row;
        for(size_t i = 0; i < row_str.size(); i++)
        {
            if(row_str[i] == 'S')
            {
                start = std::make_pair(map.size(),i);
                row.emplace_back('a');
            }
            else if(row_str[i] == 'E')
            {
                end = std::make_pair(map.size(),i);
                row.emplace_back('z');
            }
            else
            {
                row.emplace_back(row_str[i]);
            }
        }
        map.emplace_back(std::move(row));
    }
    size_t pair_to_point(size_t first, size_t second) const
    {
        return (first*map[0].size() + second);
    }
    void construct_edges()
    {
        size_t edges_size = map.size() * map[0].size();
        edges.reserve(edges_size);
        for(size_t i = 0; i < edges_size; i++)
        {
            edges.emplace_back(std::vector<bool>{});
        }
        for(auto& vector : edges)
        {
            vector.reserve(edges_size);
            for(size_t i = 0; i < edges_size; i++)
            {
                vector.emplace_back(false);
            }
        }
        for(size_t i = 0; i < map.size(); i++)
        {
            for(size_t j = 0; j < map[i].size(); j++)
            {
                char c = map[i][j];
                size_t first_point = pair_to_point(i,j);
                if(j < map[i].size() - 1 && map[i][j+1] - c >= -1)
                {
                    size_t second_point = pair_to_point(i,j+1);
                    edges[first_point][second_point] = true;
                }
                if(j > 0 && map[i][j-1] - c >= -1)
                {
                    size_t second_point = pair_to_point(i,j-1);
                    edges[first_point][second_point] = true;
                }
                if(i < map.size() - 1 && map[i+1][j] - c >= -1)
                {
                    size_t second_point = pair_to_point(i+1,j);
                    edges[first_point][second_point] = true;
                }
                if(i > 0 && map[i-1][j] - c >= -1)
                {
                    size_t second_point = pair_to_point(i-1,j);
                    edges[first_point][second_point] = true;
                }
            }
        }
        start_point = pair_to_point(start.first, start.second);
        end_point = pair_to_point(end.first, end.second);
    }
};

size_t minDistance(const std::vector<size_t>& distances, const std::vector<bool>& visited )
{
    size_t minimum=std::numeric_limits<size_t>::max();
    size_t ret = 0 ;
    for(size_t i = 0; i < distances.size(); i++) 
    {
        if(!visited[i] && distances[i] <= minimum)
        {
            minimum = distances[i];
            ret = i;
        }
    }
    return ret;
}

std::vector<size_t> dikstraAlgo(const std::vector<std::vector<bool>>& graph_edges, size_t end)
{
    std::vector<size_t> distances;
    std::vector<bool> visited;
    for(size_t i = 0; i < graph_edges.size(); i++)
    {
        distances.emplace_back(std::numeric_limits<size_t>::max());
        visited.emplace_back(false);
    }
    distances[end] = 0;

    for(size_t count = 0; count < distances.size()-1; count++)
    {
        size_t u = minDistance(distances, visited);
        visited[u] = true;
        for(size_t v = 0; v < distances.size(); v++)
        {
            size_t graph_weight = graph_edges[u][v] ? 1 : 0;
            if(!visited[v] && 
                graph_weight &&
                distances[u] != std::numeric_limits<size_t>::max() &&
                distances[u] + graph_weight < distances[v])
                {
                    distances[v] = distances[u] + graph_weight;
                }
        }
    }
    return distances;
}


std::vector<size_t> find_points_with_low_elevation(HeightMap& map)
{
    std::vector<size_t> ret;
    for(size_t i = 0; i < map.map.size(); i++)
    {
        for(size_t j = 0; j < map.map[i].size(); j++)
        {
            if(map.map[i][j] == 'a')
            {
                ret.emplace_back(map.pair_to_point(i,j));
            }
        }
    }
    return ret;
}

int main()
{
    auto tokenizer = FileTokenizer("../input/day12.txt", '\n');
    HeightMap map = HeightMap();
    for(std::string height_str = tokenizer.next().value_or("");
         !height_str.empty();
         height_str = tokenizer.next().value_or(""))
    {
        map.add_row(height_str);
    }
    map.construct_edges();
    auto distances = dikstraAlgo(map.edges,map.end_point);
    std::cout << "part1: " << distances[map.start_point]<< std::endl;

    //part2
    auto posible_start_points = find_points_with_low_elevation(map);
    std::vector<size_t> possible_distances;
    for(const auto& point : posible_start_points)
    {
        possible_distances.push_back(distances[point]);
    }
    std::sort(possible_distances.begin(), possible_distances.end(), std::less<size_t>());
    std::cout << "part2: " <<possible_distances[0] << std::endl;
    return 1;
}