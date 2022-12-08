#include<algorithm>
#include<functional>
#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<numeric>
#include<string>
#include<stack>
#include<vector>
#include <FileTokenizer.h>


struct File
{
    File(const std::string& name, size_t size) : m_name(name), m_size(size)
    {}
    std::string m_name;
    size_t m_size;
};

struct Directory
{
    Directory(const std::string& name) : m_name(name), m_files(), m_dirs(), m_size(0)
    {}
    std::string m_name;
    std::vector<File> m_files;
    std::vector<Directory> m_dirs;
    mutable size_t m_size;
    size_t get_size() const
    {
        if(m_size == 0)
        {
            m_size = std::accumulate(m_files.begin(), m_files.end(), 0,
                        [](size_t acc, const File& file){return acc + file.m_size;});
            m_size += std::accumulate(m_dirs.begin(), m_dirs.end(), 0,
                        [](size_t acc, const Directory& dir){return acc + dir.get_size();});
        }
        return m_size;
    }

    //note ptr only good as long as m_dirs isnt modified
    Directory* get_dir(const std::string& dir_name)
    {
        Directory* dir = nullptr;
        auto dir_iter = std::find_if(m_dirs.begin(), m_dirs.end(),
            [&dir_name](const Directory& d){return d.m_name == dir_name;});
        if(dir_iter != m_dirs.end())
        {
            dir = &*dir_iter;
        }
        return dir;
    }
};

Directory TOP_DIR = Directory("/");
static constexpr const size_t SMALL_DIR_MAX_SIZE = 100000;
static constexpr const size_t TOTAL_FILESYSTEM_SIZE = 70000000;
static constexpr const size_t SPACE_NEEDED = 30000000;

std::vector<std::string> tokenize_string(std::string&& str, const std::string& delimiter)
{
    size_t pos = 0;
    std::vector<std::string> tokens;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        tokens.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    tokens.push_back(str);
    return tokens;
}

void handle_cmd(std::stack<Directory*>& current_path, const std::vector<std::string>& cmd_tokens)
{
    if(cmd_tokens[1] == "cd")
    {
        if(cmd_tokens[2] == "/")
        {
            current_path = std::stack<Directory*>();
            current_path.push(&TOP_DIR);
        }
        else if(cmd_tokens[2] == "..")
        {
            current_path.pop();
        }
        else
        {
            current_path.push(current_path.top()->get_dir(cmd_tokens[2]));
        }
    }
}

//need like a directory path or something
void parse_line(std::stack<Directory*>& current_path, std::string&& cmd_str)
{
    std::vector<std::string> cmd_tokens = tokenize_string(std::move(cmd_str), " ");

    if(cmd_tokens[0] == "$")
    {
        handle_cmd(current_path, cmd_tokens);
    }

    if(cmd_tokens[0] == "dir")
    {
        current_path.top()->m_dirs.emplace_back(cmd_tokens[1]);
    }

    current_path.top()->m_files.emplace_back(cmd_tokens[1], std::strtoul(cmd_tokens[0].c_str(), nullptr, 10));
}

std::vector<const Directory*> find_dirs_if(const Directory& cur_dir,
                                        size_t pred_size_arg,
                                        std::function<bool(const Directory&, size_t)> pred)
{
    std::vector<const Directory*> ret;
    if(pred(cur_dir, pred_size_arg))
    {
        ret.emplace_back(&cur_dir);
    }
    for(const auto& dir : cur_dir.m_dirs)
    {
        auto sub_dir_sizes = find_dirs_if(dir, pred_size_arg, pred);
        std::move(std::begin(sub_dir_sizes), std::end(sub_dir_sizes), std::back_inserter(ret));
    }
    return ret;
}

size_t calc_space_needed(const Directory& dir)
{
    return SPACE_NEEDED - (TOTAL_FILESYSTEM_SIZE - dir.get_size());
}

int main()
{
    auto tokenizer = FileTokenizer("../input/day7.txt", '\n');
//Construct file system
    std::stack<Directory*> current_path;
    current_path.push(&TOP_DIR);
    for(std::string cmd_str = tokenizer.next().value_or("");
         !cmd_str.empty();
         cmd_str = tokenizer.next().value_or(""))
    {
        parse_line(current_path, std::move(cmd_str));
    }

//part 1 stuff
    std::vector<const Directory*> small_dirs = find_dirs_if(TOP_DIR, SMALL_DIR_MAX_SIZE,
         [](const Directory& dir, size_t size){return dir.get_size() < size;});
    
    size_t total = std::accumulate(small_dirs.begin(),small_dirs.end(),0, 
        [](size_t acc, const Directory* dir){return acc + dir->get_size();});
    std::cout << "part1: " << total << std::endl;

//part 2 stuff
    size_t space_needed = calc_space_needed(TOP_DIR);
    std::vector<const Directory*> dirs = find_dirs_if(TOP_DIR, space_needed,
        [](const Directory& dir, size_t size){return dir.get_size() >= size;});

    std::sort(dirs.begin(),dirs.end(),
            [](const Directory* dir1, const Directory* dir2) {return dir1->get_size() < dir2->get_size();});
    std::cout << "part2: " << dirs[0]->get_size() << std::endl;
    return 1;
}