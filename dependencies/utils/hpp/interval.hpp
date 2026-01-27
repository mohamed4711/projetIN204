#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits> // for std::numeric_limits

class interval {
  public:
    double min, max;

    // Use standard infinity for empty interval
    interval() : min(+std::numeric_limits<double>::infinity()), 
                 max(-std::numeric_limits<double>::infinity()) {}

    interval(double min, double max) : min(min), max(max) {}

    // Added a constructor to combine two intervals (useful for BVH)
    interval(const interval& a, const interval& b) {
        min = a.min < b.min ? a.min : b.min;
        max = a.max > b.max ? a.max : b.max;
    }

    double size() const { return max - min; }

    bool contains(double x) const { return min <= x && x <= max; }

    bool surrounds(double x) const { return min < x && x < max; }

    // Utility to clamp a value to the interval
    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty, universe;
};

// Correct initialization of static constants
inline const interval interval::empty    = interval(+std::numeric_limits<double>::infinity(), 
                                             -std::numeric_limits<double>::infinity());
inline const interval interval::universe = interval(-std::numeric_limits<double>::infinity(), 
                                             +std::numeric_limits<double>::infinity());

#endif