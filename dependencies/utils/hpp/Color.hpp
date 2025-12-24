#ifndef COLOR_HPP
#define COLOR_HPP

#include "Vector3.hpp"

#include <iostream>

using color = Vector3;

void write_color(std::ostream& out, const color& pixel_color) ;
#endif