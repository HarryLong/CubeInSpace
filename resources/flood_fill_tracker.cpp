#include "flood_fill_tracker.h"
#include <cstring>
#include <QDebug>

FloodFillTracker::FloodFillTracker() : m_tracker(NULL), m_size_x(0), m_size_y(0)
{

}

FloodFillTracker::~FloodFillTracker()
{

}

void FloodFillTracker::reset(int size_x, int size_y)
{
    m_size_x = size_x;
    m_size_y = size_y;
    int sz(m_size_x*m_size_y*sizeof(bool));
    if(m_tracker != NULL)
    {
        free(m_tracker);
    }
    m_tracker = (bool *) std::malloc(sz);

    std::memset(m_tracker, 0, sz);
}

int FloodFillTracker::get_index(const glm::ivec2 & point)
{
    return (point.y*m_size_x)+point.x;
}

bool FloodFillTracker::tracked(const glm::ivec2 & point)
{
    return m_tracker[get_index(point)];
}

bool FloodFillTracker::setTracked(const glm::ivec2 & point)
{
    m_tracker[get_index(point)] = true;
}
