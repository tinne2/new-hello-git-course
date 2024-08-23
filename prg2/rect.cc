// The main program (provided by the course), COMP.CS.300
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do
// so
//  (for debugging, for example), DO NOT commit any changes to git, but revert
//  all changes later. Otherwise you won't be able to get any updates/fixes to
//  this file from git!)

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include "customtypes.hh"
#include "datastructures.hh"
#include "rect.hh"


std::minstd_rand rand_engine; // Definition of rand_engine


Rectangle::Rectangle(Coord c1, Coord c2)
{
    if (c1.x > c2.x || c1.y > c2.y )
    {
        throw std::invalid_argument("Not a valid rect with these coords");
    }
    top_left = Coord(std::min(c1.x, c2.x), std::min(c1.y, c2.y));
    bottom_right = Coord(std::max(c1.x, c2.x), std::max(c1.y, c2.y));
    width = bottom_right.x - top_left.x;
    height = bottom_right.y - top_left.y;
}

std::vector<Rectangle> Rectangle::divide(int slices) const
{
    const int threshold = 3;

    if (width < threshold && height < threshold)
    {
        return {*this};
    }
    if (slices <= 0)
    {
        throw std::invalid_argument("Number of slices must be a positive integer");
    }

    std::vector<Rectangle> divided_pairs;
    if (width >= height)
    {
        int slice_width = width / slices;
        for (int i = 0; i < slices; ++i)
        {
            Coord c1(top_left.x + i * slice_width, top_left.y);
            Coord c2(top_left.x + (i + 1) * slice_width - 2, bottom_right.y);
            if (i == slices - 1)
            {
                c2 = bottom_right;
            }
            divided_pairs.emplace_back(c1, c2);
        }
    }
    else
    {
        int slice_height = height / slices;
        for (int i = 0; i < slices; ++i)
        {
            Coord c1(top_left.x, top_left.y + i * slice_height);
            Coord c2(bottom_right.x, top_left.y + (i + 1) * slice_height - 2);
            if (i == slices - 1)
            {
                c2 = bottom_right;
            }
            divided_pairs.emplace_back(c1, c2);
        }
    }

    return divided_pairs;
}

std::vector<Coord> Rectangle::getCoords() const
{
    std::vector<Coord> all_coords;
    for (int y : {top_left.y, bottom_right.y})
    {
        for (int x = top_left.x; x <= bottom_right.x; ++x)
        {
            all_coords.emplace_back(x, y);
        }
    }
    for (int x : {top_left.x, bottom_right.x})
    {
        for (int y = top_left.y; y <= bottom_right.y; ++y)
        {
            all_coords.emplace_back(x, y);
        }
    }
    all_coords.emplace_back(bottom_right.x, bottom_right.y);
    return all_coords;
}


std::vector<Coord> Rectangle::getAllCoords() const
{
    std::vector<Coord> coords;

    for (int x = top_left.x; x <= bottom_right.x; ++x)
    {
        for (int y = top_left.y; y <= bottom_right.y; ++y)
        {
            coords.emplace_back(x, y);
        }
    }

    return coords;
}

std::string Rectangle::str() const {
    // integers to strings
    return std::to_string(top_left.x) + "," + std::to_string(top_left.y) +
           " (" + std::to_string(bottom_right.x) + "," + std::to_string(bottom_right.y) + ")";
}



std::vector<Rectangle> getRects(unsigned int nmb_of_contours, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    int w = x2 - x1, h = y2 - y1;
    double s = std::sqrt(nmb_of_contours);

    int ok = random_in_range(0,1);
    unsigned int n = ok ? std::ceil(s) : std::floor(s);
    unsigned int m = (n == std::ceil(s)) ? std::floor(s) : std::ceil(s);
    while (n * m < nmb_of_contours) {
        if (std::rand() % 2) n++;
        else m++;
    }

    int w0 = w / n, h0 = h / m;
    std::vector<Rectangle> rects;
    for (unsigned int y = 0; y < m; ++y) {
        for (unsigned int x = 0; x < n; ++x) {
            rects.emplace_back(
                Coord(x1 + (w0 * x) + 1, y1 + (h0 * y) + 1),
                Coord(x1 + (w0 * (x + 1) - 1), y1 + (h0 * (y + 1) - 1))
            );
        }
    }

    int diff = n * m - nmb_of_contours;
    if (diff == 0) return rects;
    std::vector<int> del_arr(nmb_of_contours, 1);
    del_arr.resize(n * m, 0);
    std::shuffle(del_arr.begin(), del_arr.end(), std::mt19937{std::random_device{}()});

    std::vector<Rectangle> filtered_rects;
    for (size_t i = 0; i < rects.size(); ++i) {
        if (del_arr[i]) filtered_rects.push_back(rects[i]);
    }
    return filtered_rects;
}


