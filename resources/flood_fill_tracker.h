#ifndef FLOOD_FILL_TRACKER_H
#define FLOOD_FILL_TRACKER_H

#include <unordered_map>
#include <glm/common.hpp>

class FloodFillTracker : public std::unordered_map<int, std::unordered_map<int,char> >
{
public:
    FloodFillTracker();
    ~FloodFillTracker();

    bool tracked(const glm::vec2 & point);
    bool setTracked(const glm::vec2 & point);
};

#endif //FLOOD_FILL_TRACKER_H
