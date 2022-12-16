#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstdint>
#include<cstdlib>
#include<set>
#include<vector>
#include<limits>
#include <FileTokenizer.h>

struct Point
{
    int64_t x;
    int64_t y;
    Point(int _x, int _y) : x(_x), y(_y)
    {}
    int64_t distance_from(const Point& other) const
    {
        return std::labs(x - other.x) + std::labs(y - other.y);
    }
    bool operator == (const Point& other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator < (const Point& other) const
    {
        if(x < other.x)
        {
            return true;
        }
        if(x == other.x && y < other.y)
        {
            return true;
        }
        return false;
    }
    std::string print() const
    {
        char test[254] = {'\0'};
        snprintf(test, sizeof(test), "%ld,%ld", x,y);
        return std::string(test);
    }
};

struct ManhattanCircle
{
    Point center;
    int64_t radius;
    ManhattanCircle(const Point& _center, const Point& edge) : center(_center)
         , radius(center.distance_from(edge))
    {}
    ManhattanCircle(const Point& _center, int64_t _radius) : center(_center)
         , radius(_radius)
    {}
    bool contains(const Point& p) const
    {
        return center.distance_from(p) <= radius;
    }

    std::vector<Point> get_perimeter () const
    {
        std::vector<Point> ret;
        ret.emplace_back(Point(center.x+radius,center.y));
        ret.emplace_back(Point(center.x-radius,center.y));
        ret.emplace_back(Point(center.x,center.y+radius));
        ret.emplace_back(Point(center.x,center.y-radius));

        for(int64_t dx = 1; dx < radius; dx++)
        {
            int64_t dy = radius - dx;
            ret.emplace_back(Point(center.x+dx,center.y+dy));
            ret.emplace_back(Point(center.x-dx,center.y-dy));
            ret.emplace_back(Point(center.x+dx,center.y-dy));
            ret.emplace_back(Point(center.x-dx,center.y+dy));
        }
        return ret;
    }
};

std::vector<Point> beacons;
std::vector<ManhattanCircle> circles;

int64_t LEFT_MOST_X = std::numeric_limits<int64_t>::max();
int64_t RIGHT_MOST_X = std::numeric_limits<int64_t>::min();
int64_t Y = 2000000;

void parse_sensors_and_beacons(std::string&& str)
{
    Point sensor = Point(0,0);
    Point beacon = Point(0,0);
    sscanf(str.c_str(),"Sensor at x=%ld, y=%ld: closest beacon is at x=%ld, y=%ld"
        ,&sensor.x
        ,&sensor.y
        ,&beacon.x
        ,&beacon.y);
    LEFT_MOST_X = std::min(beacon.x, LEFT_MOST_X);
    RIGHT_MOST_X = std::max(beacon.x, RIGHT_MOST_X);
    beacons.emplace_back(beacon);
    circles.emplace_back(sensor, beacon);

}

bool is_valid(const Point& p)
{
    for(const auto& circle : circles)
    {
        if(circle.contains(p))
        {
            return true;
        }
    }
    return false;
}

bool not_a_beacon(const Point& p)
{
    return std::find(beacons.begin(), beacons.end(), p) == beacons.end();
}

std::vector<Point> count_possible_locations(int64_t min_x, int64_t max_x, int64_t min_y,int64_t max_y)
{
    std::vector<Point> ret;
    for(int64_t x = min_x; x <= max_x; x++)
    {
        for(int64_t y = min_y; y <= max_y; y++)
        {
            Point test_point = Point(x, y);
            if(is_valid(test_point) && not_a_beacon(test_point))
            {
                ret.emplace_back(test_point);
                break;
            }
        }
    }
    return ret;
}

int64_t part_2()
{
    std::vector<ManhattanCircle> bigger_circles;
    std::set<Point> to_check;
    for(const auto& circle : circles)
    {
        ManhattanCircle bigger_circle = ManhattanCircle(circle.center,circle.radius+1);
        for(const auto& point : bigger_circle.get_perimeter())
        {
            to_check.emplace(point);
        }
    }
    for(const auto& point : to_check)
    {
        if(point.x >=0 && 
            point.x <= 4000000&&
            point.y >=0 && 
            point.y <= 4000000)
        {
            if(!is_valid(point) && not_a_beacon(point))
            {
                return point.x*4000000 + point.y;
                std::cout << point.print() << std::endl;
            }
        }
    }
    return 0;

}

int main()
{
    auto tokenizer = FileTokenizer("../input/day15.txt", '\n');
    for(std::string str = tokenizer.next().value_or("");
         !str.empty();
         str = tokenizer.next().value_or(""))
    {
        parse_sensors_and_beacons(std::move(str));
    }
    std::cout << "part1: " << count_possible_locations(LEFT_MOST_X, RIGHT_MOST_X,Y,Y).size() << std::endl;
    std::cout << "part2: " << part_2() << std::endl;
    return 1;
}