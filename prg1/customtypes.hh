#include <exception>
#include <limits>
#include <string>
#include <tuple>
#include <random>

#ifndef CUSTOMTYPES_HH
#define CUSTOMTYPES_HH

// Types for IDs
using BiteID = long long int;
using ContourID = long long int;
using ConnectionID = long long int;
using ContourHeight = int;

using Name = std::string;


static unsigned int const DEF_MAZE_WIDTH = 100;
static unsigned int const DEF_MAZE_HEIGHT = 100;



// Return values for cases where required thing was not found
static BiteID const NO_BITE = -1;
static ContourID const NO_CONTOUR = -2;
static ConnectionID const NO_CONNECTION = -3;
static ContourHeight const NO_CONTOUR_HEIGHT = 0;
static ContourHeight const MAX_CONTOUR_HEIGHT =9;
static Name const NO_NAME = "!NO_NAME!";






// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

extern std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type> Type random_in_range(Type start, Type end) {
  auto range = end - start;
  ++range;

  auto num = std::uniform_int_distribution<unsigned long int>(0, range - 1)(
      rand_engine);

  return static_cast<Type>(start + num);
}



// Type for a coordinate (x, y)
struct Coord {
  int x = NO_VALUE;
  int y = NO_VALUE;

  // equality operator for usage as unordered_map key or unordered_set item
  bool operator==(Coord c2) const { return x == c2.x && y == c2.y; }
  bool operator!=(Coord c2) const { return !(Coord{x, y} == c2); }

  Coord() = default;

  Coord(int x, int y) : x{x}, y{y} {}
  // Comparison function(s) to allow usage as map key and set item
  // please notice that these will not result the required order for
  // distance_increasing
  bool operator<(Coord c2) const {
    return std::tie(y, x) < std::tie(c2.y, c2.x);
  }
  bool operator>(Coord c2) const {
    return std::tie(y, x) > std::tie(c2.y, c2.x);
  }

  bool operator>=(Coord c2) const { return !(c2 < Coord{x, y}); }
  bool operator<=(Coord c2) const { return !(Coord{x, y} > c2); }
};

// hash function for Coordinate so that it can be used as unordered_map key or
// unordered_set item
template <> struct std::hash<Coord> {
  size_t operator()(const Coord &xy) const {
    auto hasher = std::hash<int>();
    auto xhash = hasher(xy.x);
    auto yhash = hasher(xy.y);
    // Combine hash values (magic!)
    return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
  }
};

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

const Coord DEFAULT_MIN_COORD = {1, 1};
const Coord DEFAULT_MAX_COORD = {10000, 10000};
const int DEFAULT_MAX_HEIGHT = 100;
const int DEFAULT_MIN_HEIGHT = 1;
const double ROOT_BIAS_MULTIPLIER = 0.05;
const double LEAF_BIAS_MULTIPLIER = 0.5;



// Type for a distance (in metres)
using Distance = unsigned long int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception {
public:
  NotImplemented() : msg_{} {}
  explicit NotImplemented(std::string const &msg)
      : msg_{msg + " not implemented"} {}

  virtual const char *what() const noexcept override { return msg_.c_str(); }

private:
  std::string msg_;
};

#endif // CUSTOMTYPES_HH
