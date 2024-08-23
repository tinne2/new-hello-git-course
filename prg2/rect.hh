// The main program (provided by the course), COMP.CS.300
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do
// so
//  (for debugging, for example), DO NOT commit any changes to git, but revert
//  all changes later. Otherwise you won't be able to get any updates/fixes to
//  this file from git!)

#ifndef RECT_HH
#define RECT_HH

#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "customtypes.hh"

struct Coord;

class Rectangle
{
public:
    Coord top_left, bottom_right;
    int width, height;

    Rectangle();
    Rectangle(Coord c1, Coord c2);

    std::vector<Coord> getCoords() const;
    std::vector<Coord> getAllCoords() const;
    std::vector<Rectangle> divide(int slices) const;
    std::string str() const;



    static std::vector<Coord> betweens(const std::vector<Rectangle> &rects) {
        std::vector<Coord> coords;

        if (rects.size() < 2) {
            // no rectangles to find 'betweens'
            return coords;
        }

        bool is_horizontal = std::all_of(rects.begin(), rects.end(), [&rects](const Rectangle &r) {
            return r.top_left.y == rects[0].top_left.y;
        });

        if (is_horizontal) {
            int y1 = rects[0].top_left.y;
            int y2 = rects[0].bottom_right.y;
            for (size_t i = 0; i < rects.size() - 1; ++i) {
                int x = rects[i].bottom_right.x + 1;
                for (int y = y1; y <= y2; ++y) {
                    coords.emplace_back(x, y);
                }
            }
        } else {
            int x1 = rects[0].top_left.x;
            int x2 = rects[0].bottom_right.x;
            for (size_t i = 0; i < rects.size() - 1; ++i) {
                int y = rects[i].bottom_right.y + 1;
                for (int x = x1; x <= x2; ++x) {
                    coords.emplace_back(x, y);
                }
            }
        }

        return coords;
    }
    static Rectangle factory(const Rectangle &c)
    {
        Coord newTopLeft = Coord(c.top_left.x+1, c.top_left.y+1);
        Coord newBottomRight = Coord(c.bottom_right.x-1, c.bottom_right.y-1);
        return Rectangle(newTopLeft, newBottomRight);
    }



    static std::vector<std::map<int, std::vector<Coord>>> hilo_factory(
        std::vector<std::map<int, std::vector<Coord>>>& arr,
        Rectangle rect, int z, int max_levels)
    {

        if (rect.width <0 || rect.height <0)
        {
            return arr;
        }
        if (std::abs(z) > max_levels)
        {
            return arr;
        }

        try
        {
            if (std::abs(z) == 1)
            {
                    if (rect.height>2 and rect.width>2){

                        std::vector<Rectangle> peaks = Rectangle::factory(rect).divide(rand() % 4 + 1);
                        for (const auto &peak : peaks)
                        {
                            arr = Rectangle::hilo_factory(arr, peak, z < 0 ? z - 1 : z + 1, max_levels);
                        }
                        if (peaks.size()>0){
                            std::vector<Coord> coords = rect.getCoords(); // Call get_coords once
                            std::vector<Coord> betweens = Rectangle::betweens(peaks); // Call get_coords once
                            coords.insert(coords.end(), betweens.begin(), betweens.end());
                            arr.push_back({{z, coords}});
                            return arr;

                        }
                    }
            }


            bool cont = abs(z)<max_levels&&rect.width>1&&rect.height>1;
            arr.push_back({{z, cont?rect.getCoords():rect.getAllCoords()}});
            if (!cont) return arr;
            z = z < 0 ? z - 1 : z + 1;
            return Rectangle::hilo_factory(arr, Rectangle::factory(rect), z, max_levels); // Pass rect directly
        }
        catch (const std::exception &e)
        {
            return arr;
        }
    }

};

std::vector<Rectangle> getRects(unsigned int nmb_of_hills_and_valleys, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);




#endif // RECT_HH
