#include "flood_fill_tracker.h"

FloodFillTracker::FloodFillTracker()
{

}

FloodFillTracker::~FloodFillTracker()
{

}

bool FloodFillTracker::tracked(const glm::vec2 & point)
{
    auto x_it(find(point.x));
    if(x_it != end())
    {
        auto y_it(x_it->second.find(point.y));
        return (y_it != x_it->second.end());
    }
    return false;
}

bool FloodFillTracker::setTracked(const glm::vec2 & point)
{
    (*this)[point.x][point.y] = 0x01;
}
