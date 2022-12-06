#include<iostream>
#include<array>
#include<cstdint>
#include<cstdlib>
#include<deque>
#include<string>
#include <FileTokenizer.h>

static constexpr const size_t START_OF_PACKET_SIZE = 4;
static constexpr const size_t START_OF_MESSAGE_SIZE = 14;
struct Device
{
    std::string m_traffic_stream;
    size_t m_start_of_packet_marker_index;
    size_t m_start_of_message_marker_index;

    Device(std::string&& traffic_stream) : m_traffic_stream(traffic_stream)
    {
        m_start_of_packet_marker_index = find_start_of(START_OF_PACKET_SIZE);
        m_start_of_message_marker_index = find_start_of(START_OF_MESSAGE_SIZE);
    }

    bool unique_deque(const std::array<uint32_t, 26>& seen, const std::deque<char>& dq) const
    {
        bool ret = true;
        for(const auto& item: dq)
        {
            ret &= (seen[item - 'a'] ==1);
        }
        return ret;
    }

    size_t find_start_of(size_t num_unique_characters) const
    {
        std::deque<char> current_set;
        std::array<uint32_t, 26> seen_characters = {0};

        for(size_t i = 0; i < m_traffic_stream.size(); i++)
        {
            char c = m_traffic_stream[i];
            if(current_set.size() < num_unique_characters)
            {
                current_set.push_back(c);
                seen_characters[c-'a']++;
            }
            else
            {
                current_set.push_back(c);
                seen_characters[c-'a']++;
                seen_characters[current_set.front()-'a']--;
                current_set.pop_front();
                if(unique_deque(seen_characters, current_set))
                {
                    return i+1;
                }
            }
        }
        return 0;
    }
};
int main()
{
    auto tokenizer = FileTokenizer("../input/day6.txt", '\n');
    Device device = Device( tokenizer.next().value_or("") );

    std::cout << "part1: " << device.m_start_of_packet_marker_index << std::endl;
    std::cout << "part2: " << device.m_start_of_message_marker_index << std::endl;
    return 1;
}