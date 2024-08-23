// The main program (provided by the course), COMP.CS.300
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do
// so
//  (for debugging, for example), DO NOT commit any changes to git, but revert
//  all changes later. Otherwise you won't be able to get any updates/fixes to
//  this file from git!)

#include <string>
using std::getline;
using std::string;

#include <iostream>
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::flush;
using std::noskipws;

#include <iomanip>
using std::setfill;
using std::setw;

#include <istream>
using std::istream;

#include <ostream>
using std::ostream;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::istringstream;
using std::ostringstream;
using std::stringstream;

#include <iomanip>
using std::setw;

#include <tuple>
using std::get;
using std::make_tuple;
using std::tie;
using std::tuple;

#include <regex>
using std::regex;
using std::regex_match;
using std::regex_search;
using std::smatch;
using std::sregex_token_iterator;

#include <algorithm>
using std::binary_search;
using std::find;
using std::find_if;
using std::max;
using std::max_element;
using std::min;
using std::shuffle;
using std::sort;

#include <random>
using std::minstd_rand;
using std::uniform_int_distribution;

#include <chrono>

#include <functional>
using std::equal_to;
using std::function;

#include <vector>
using std::vector;

#include <set>
using std::set;

#include <array>
using std::array;

#include <bitset>
using std::bitset;

#include <iterator>
using std::next;

#include <ctime>
using std::time;

#include <memory>
using std::move;

#include <utility>
using std::make_pair;
using std::pair;

#include <cmath>
using std::abs;

#include <cstdlib>
using std::div;

#include <algorithm>
using std::transform;

#include <iterator>
using std::back_inserter;

#include <cassert>
#include <cstddef>

#include "mainprogram.hh"

#include "datastructures.hh"
#include "rect.hh"
#include "customtypes.hh"

#ifdef GRAPHICAL_GUI
#include "mainwindow.hh"
#endif

// for perftests, to execute only the needed things
const std::set<std::string> bite_tests = {"get_bite_name", "get_bite_coord", "all_bites", "get_bites_alphabetically", "get_bites_distance_increasing", "get_bites_closest_to"};
const std::set<std::string> contour_tests = {"get_contour_coords", "get_contour_name", "all_subcontours_of_contour", "get_closest_common_ancestor_of_contours"};
const std::set<std::string> bite_contour_tests = {"find_bite_with_coord", "change_bite_coord", "remove_bite"};

int BITE_COUNTER = 1;
int CONTOUR_COUNTER = 1;



string const MainProgram::PROMPT = "> ";

template <typename C, typename F>
size_t remove_if_from(C &container, F fun)
{
  auto old_size = container.size();
  for (auto first = container.begin(); first != container.end();)
  {
    if (fun(*first))
    {
      first = container.erase(first);
    }
    else
    {
      ++first;
    }
  }
  return old_size - container.size();
}

template <typename T>
void unstable_vector_erase(std::vector<T> &vector, const T &to_remove)
{
  auto findit = std::find(vector.begin(), vector.end(), to_remove);
  if (findit == vector.end())
  {
    return;
  }
  std::swap(*findit, vector.back());
  vector.pop_back();
}

int suggest_number_of_hilos(int width, int height)
{
  return int((width + height) / 2 * 0.3);
}

void MainProgram::test_get_functions(BiteID id)
{
  ds_.get_bite_name(id);
  ds_.get_bite_coord(id);
}

MainProgram::CmdResult
MainProgram::cmd_add_bite(ostream &output, MatchIter begin, MatchIter end)
{

  BiteID id = std::stoull(begin->str());
  ++begin;
  string name = *begin++;
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  std::u32string u32_name = converter.from_bytes(name);
  // name can be either 1 char long or and emoji. Abort if name is too long
  if (u32_name.size() > 1)
  {
    output << "Name too long!" << endl;
    return {};
  }
  string xstr = *begin++;
  string ystr = *begin++;
  assert(begin == end && "Invalid number of parameters!");

  int x = convert_string_to<int>(xstr);
  int y = convert_string_to<int>(ystr);

  bool success = ds_.add_bite(id, name, {x, y});

  view_dirty = true;
  return {ResultType::IDLIST,
          std::make_tuple(std::vector<ContourID>{},
                          std::vector<BiteID>{success ? id : NO_BITE},
                          std::vector<ConnectionID>{})};
}

MainProgram::CmdResult MainProgram::cmd_get_bite_name(std::ostream & /*output*/,
                                                      MatchIter begin,
                                                      MatchIter end)
{
  BiteID id = std::stoull(begin->str());
  ++begin; // Move to the next match
  assert(begin == end && "Invalid number of parameters!");
  Name node_name = ds_.get_bite_name(id);
  return {ResultType::NAMELIST, CmdResultNames{{id, NO_CONTOUR}, node_name}};
}

MainProgram::CmdResult
MainProgram::cmd_get_contour_name(std::ostream & /*output*/, MatchIter begin,
                                  MatchIter end)
{
  ContourID id = convert_string_to<ContourID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");
  Name contour_name = ds_.get_contour_name(id);
  return {ResultType::NAMELIST, CmdResultNames{{NO_BITE, id}, contour_name}};
}

MainProgram::CmdResult
MainProgram::cmd_get_contour_height(std::ostream & /*output*/, MatchIter begin,
                                    MatchIter end)
{
  ContourID id = convert_string_to<ContourID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");
  ContourHeight height = ds_.get_contour_height(id);
  return {ResultType::HEIGHTLIST, CmdResultHeights{{NO_BITE, id}, height}};
}

MainProgram::CmdResult
MainProgram::cmd_get_bite_coord(std::ostream & /*output*/, MatchIter begin,
                                MatchIter end)
{
  BiteID id = std::stoull(begin->str());
  ++begin; // Move to the next match
  assert(begin == end && "Invalid number of parameters!");
  Coord node_coordinates = ds_.get_bite_coord(id);

  return {ResultType::COORDLIST,
          CmdResultCoords{{id, NO_CONTOUR}, {node_coordinates}}};
}

MainProgram::CmdResult
MainProgram::cmd_get_contour_coords(std::ostream & /*output*/, MatchIter begin,
                                    MatchIter end)
{
  ContourID id = convert_string_to<ContourID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");
  std::vector<Coord> contour_coordinates = ds_.get_contour_coords(id);
  return {ResultType::COORDLIST,
          CmdResultCoords{{NO_BITE, id}, contour_coordinates}};
}


MainProgram::CmdResult
MainProgram::cmd_add_subcontour_to_contour(std::ostream &output,
                                           MainProgram::MatchIter begin,
                                           MainProgram::MatchIter end)
{
  ContourID subcontourid = convert_string_to<ContourID>(*begin++);
  ContourID parentid = convert_string_to<ContourID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");

  bool ok = ds_.add_subcontour_to_contour(subcontourid, parentid);
  if (ok)
  {
    try
    {
      auto subcontourname = ds_.get_contour_name(subcontourid);
      auto parentname = ds_.get_contour_name(parentid);
      output << "Added '" << subcontourname << "' as a subcontour of '"
             << parentname << "'" << endl;
    }
    catch (NotImplemented &)
    {
      output << "Added a subcontour to contour." << endl;
    }

    return {ResultType::IDLIST,
            std::make_tuple(std::vector<ContourID>{subcontourid, parentid},
                            std::vector<BiteID>{},
                            std::vector<ConnectionID>{})};
  }
  else
  {
    output << "Adding a subcontour failed!" << endl;
    return {};
  }
}

MainProgram::CmdResult
MainProgram::cmd_add_bite_to_contour(std::ostream &output, MatchIter begin,
                                     MatchIter end)
{
  BiteID biteId = std::stoull(begin->str());
  ++begin; // Move to the next match
  ContourID contourId = convert_string_to<ContourID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");

  bool ok = ds_.add_bite_to_contour(biteId, contourId);
  if (ok)
  {
    try
    {
      auto bitename = ds_.get_bite_name(biteId);
      auto contourname = ds_.get_contour_name(contourId);
      output << "Added '" << bitename << "' to contour '" << contourname << "'"
             << endl;
    }
    catch (NotImplemented &)
    {
      output << "Added a bite to contour." << endl;
    }
    return {ResultType::IDLIST,
            CmdResultIDs(std::make_tuple(std::vector<ContourID>{contourId},
                                         std::vector<BiteID>{biteId},
                                         std::vector<ConnectionID>{}))};
  }
  else
  {
    output << "Adding a bite to contour failed!" << endl;
    return {};
  }
}

MainProgram::CmdResult
MainProgram::cmd_get_bites_closest_to(std::ostream &output, MatchIter begin,
                                      MatchIter end)
{
  // Extract parameters from iterators
  string xstr = *begin++;
  string ystr = *begin++;
  assert(begin == end && "Invalid number of parameters!");

  // Convert strings to integers
  int x = convert_string_to<int>(xstr);
  int y = convert_string_to<int>(ystr);

  // Get closest nodes
  auto nodes = ds_.get_bites_closest_to({x, y});
  if (nodes.empty())
  {
    output << "No bites!" << endl;
  }

  // Explicitly create CmdResultIDs using std::make_tuple
  CmdResultIDs cmdResultIds = std::make_tuple(
      std::vector<ContourID>{}, std::vector<BiteID>(nodes.begin(), nodes.end()),
      std::vector<ConnectionID>{});

  // Return the result as a pair of ResultType and CmdResultIDs
  return {ResultType::IDLIST, cmdResultIds};
}

MainProgram::CmdResult MainProgram::cmd_get_closest_common_ancestor_of_contours(
    std::ostream &output, MatchIter begin, MatchIter end)
{
  ContourID contourid1 = convert_string_to<ContourID>(*begin++);
  ContourID contourid2 = convert_string_to<ContourID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");

  auto contourid =
      ds_.get_closest_common_ancestor_of_contours(contourid1, contourid2);
  if (contourid == NO_CONTOUR)
  {
    output << "No common parent contour found." << endl;
  }

  return {
      ResultType::IDLIST,
      std::make_tuple(std::vector<ContourID>{contourid1, contourid2, contourid},
                      std::vector<BiteID>{}, std::vector<ConnectionID>{})};
}

MainProgram::CmdResult
MainProgram::cmd_get_bite_in_contours(std::ostream &output,
                                      MainProgram::MatchIter begin,
                                      MainProgram::MatchIter end)
{
  BiteID id = std::stoull(begin->str());
  ++begin; // Move to the next match
  assert(begin == end && "Invalid number of parameters!");

  auto result = ds_.get_bite_in_contours(id);
  if (result.empty())
  {
    output << "Bite does not belong to any contour." << std::endl;
  }
  // return {ResultType::IDLIST, CmdResultIDs{result, {id}, {}}};
  return {ResultType::IDLIST,
          std::make_tuple(std::vector<ContourID>{result.begin(), result.end()},
                          std::vector<BiteID>{id},
                          std::vector<ConnectionID>{})};
}

bool MainProgram::test_get_bite_in_contours(Stopwatch &stopwatch)
{
  if (random_bites_added_ < 1)
  {
    return true;
  }

  auto id = random_valid_bite();
  stopwatch.start();
  auto result = ds_.get_bite_in_contours(id);
  stopwatch.stop();
  std::unordered_set<ContourID> unique(result.begin(), result.end());
  if (unique.size() != result.size())
  {
    return false;
  }
  for (const auto &rid : result)
  {
    if (valid_contour_ids_.find(rid) == valid_contour_ids_.end())
    {
      return false;
    }
  }

  return true;
}

bool MainProgram::test_all_subcontours_of_contour(Stopwatch &stopwatch)
{

  if (random_contours_added_ < 1)
  {
    return true;
  }

  auto id = random_root_contour();
  stopwatch.start();
  auto result = ds_.all_subcontours_of_contour(id);
  stopwatch.stop();

  if (random_contours_added_ > 1 &&
      (result.size() + 1) *
              (pow(2, ROOT_BIAS_MULTIPLIER * random_contours_added_)) <
          random_contours_added_)
  {
    return false;
  }
  std::unordered_set<ContourID> unique(result.begin(), result.end());
  if (result.size() != unique.size())
  {
    return false;
  }
  // check validity of each id
  for (const auto &rid : result)
  {
    if (valid_contour_ids_.find(rid) == valid_contour_ids_.end())
    {
      return false;
    }
  }

  return true;
}

bool MainProgram::test_get_contour_height(Stopwatch &stopwatch)
{
  if (random_contours_added_ < 1)
  {
    return {};
  }

  auto id = random_valid_contour();
  stopwatch.start();
  auto result = ds_.get_contour_height(id);
  stopwatch.stop();
  return result != NO_CONTOUR_HEIGHT;
}

MainProgram::CmdResult
MainProgram::cmd_remove_bite(ostream &output, MatchIter begin, MatchIter end)
{
  BiteID id = std::stoull(begin->str());
  ++begin; // Move to the next match
  assert(begin == end && "Invalid number of parameters!");

  auto name = ds_.get_bite_name(id);
  bool success = ds_.remove_bite(id);
  if (success)
  {
    output << name << " removed." << endl;
    view_dirty = true;
    return {};
  }
  else
  {
    return {ResultType::IDLIST, std::make_tuple(std::vector<ContourID>{},
                                                std::vector<BiteID>{NO_BITE},
                                                std::vector<ConnectionID>{})};
  }
}

bool MainProgram::test_remove_bite(Stopwatch &stopwatch)
{
  if (random_bites_added_ < 1)
  {
    return true;
  }

  BiteID BiteID = random_valid_bite();
  Name name = ds_.get_bite_name(BiteID);
  if (name == NO_NAME)
  {
    return false;
  }
  Coord coords = ds_.get_bite_coord(BiteID);
  if (coords == NO_COORD)
  {
    return false;
  }
  stopwatch.start();
  ds_.remove_bite(BiteID);
  stopwatch.stop();

  // remove from caches
  valid_bite_ids_.erase(BiteID);

  unstable_vector_erase(bite_ids_, BiteID);

  --random_bites_added_;
  return true;
}

bool add_subcontour(unsigned int child_id, unsigned int parent_id, Datastructures &ds_)
{
  return ds_.add_subcontour_to_contour(child_id, parent_id);
}

int MainProgram::add_bite_with_info(Stopwatch &stopwatch, int bite_counter, int nmb_of_bites, int contour_id, Coord &bite_pos, Datastructures &ds_)
{
  if (bite_counter >= nmb_of_bites)
    return nmb_of_bites;

  // int id = static_cast<int>(random_bites_added_);
  int id = BITE_COUNTER;

  Name name = random_emoji();
  stopwatch.start();
  const bool bite_added = ds_.add_bite(id, name, bite_pos);
  stopwatch.stop();
  if (bite_added)
  {
    try
    {
      stopwatch.start();
      ds_.add_bite_to_contour(bite_counter, contour_id);
      ++BITE_COUNTER;
      stopwatch.stop();
    }
    catch (...)
    {
    }
    ++random_bites_added_;
    valid_bite_ids_.insert(id);
    bite_ids_.push_back(id);
    valid_coords_.insert(bite_pos);
    coords_.push_back(bite_pos);
    valid_bite_names_.insert(name);
    return ++bite_counter;
  }
  return bite_counter;
}

std::vector<int> distribute(int nmb_of_bites, int size)
{
  std::vector<int> bite_distribution(size, 0);
  int bite_counter = 0;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, size - 1);

  while (bite_counter < nmb_of_bites)
  {
    int idx = dis(gen);
    bite_distribution[idx]++;
    bite_counter++;
  }
  return bite_distribution;
}

int MainProgram::add_random_bites(Stopwatch &stopwatch, int n)
{
  if (n == 0)
    return 0;

  // Generate rnd coordinates
  std::vector<Coord> coords;
  for (int i = 0; i < n; ++i)
    coords.emplace_back(i, random_in_range(0, n));

  for (int i = 0; i < n; ++i)
  {
    int id = static_cast<int>(random_bites_added_);
    Name name = random_emoji();
    Coord bite_pos = coords[i];
    stopwatch.start();
    bool status = ds_.add_bite(id, name, bite_pos);
    stopwatch.stop();
    if (status)
    {

      ++random_bites_added_;
      valid_bite_ids_.insert(id);
      bite_ids_.push_back(id);
      valid_coords_.insert(bite_pos);
      coords_.push_back(bite_pos);
      valid_bite_names_.insert(name);
    }
  }
  return n;
}

int MainProgram::add_random_bites_to_given_contours(Stopwatch &stopwatch, int nmb_of_bites, std::vector<int> contour_ids)
{
  if (nmb_of_bites == 0 || contour_ids.empty())
    return 0;

  int size = contour_ids.size();
  std::vector<int> bites = distribute(nmb_of_bites, size);
  std::random_device rd;
  std::mt19937 gen(rd());

  int bite_counter = 0;
  int counter = 0; // not that elegant solution to prevent from inf.loop if coords are less than nmb_of_bites
  while (bite_counter < nmb_of_bites && ++counter < 5)
  {
    // distribute the bites randomly
    std::vector<int> bites = distribute(nmb_of_bites, size);
    std::mt19937 gen(rd());

    for (int i = 0; i < size; i++)
    {
      int nmb_of_coords = bites[i];
      if (nmb_of_coords == 0)
        continue;

      int contour_id = contour_ids[i];
      std::vector<Coord> coords = ds_.get_contour_coords(contour_id);
      std::vector<Coord> random_coords;
      // shuffle and pick random coordinates
      std::sample(coords.begin(), coords.end(), std::back_inserter(random_coords), nmb_of_coords, gen);

      int limit = random_coords.size(); // can differ from nmb_of_coords
      for (int j = 0; j < limit; j++)
      {
        Coord bite_pos = random_coords[j];
        bite_counter = add_bite_with_info(stopwatch, bite_counter, nmb_of_bites, contour_id, bite_pos, ds_);
      }
    }
  }
  return bite_counter;
}

std::pair<std::vector<int>, bool> MainProgram::add_random_contours2(Stopwatch &stopwatch, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int max_levels)
{
  const unsigned int w = (x2 - x1);
  const unsigned int h = (y2 - y1);

  const int N = int((w + h) / 2 * 1.2);
  int z = 1;
  int n = std::ceil(std::sqrt(N));
  int contour_id = 1;
  int parent_id = contour_id;

  stopwatch.start();
  for (int i = 0; i < n + 2; ++i)
  {
    for (int j = 0; j < n + 2; ++j)
    {
      int tmp_z = z % max_levels;
      bool isOK = ds_.add_contour(contour_id, "⛰️", tmp_z, {Coord((i * n) % w + x1, (j * n) % h + y1)});

      if (isOK){
        valid_contour_ids_.insert(contour_id);
        contour_ids_.push_back(contour_id);
        ++random_contours_added_;
      }
      if (contour_id >= N)
        break;
      if (isOK)
      {
        if (z % max_levels > 2)
        {
          ds_.add_subcontour_to_contour(contour_id, contour_id - 1);
        }
        else
        {
          ds_.add_subcontour_to_contour(contour_id, parent_id);
        }
        contour_id++;



        while (++z % max_levels < 2)
          ;

      }
    }
    if (contour_id >= N)
      break;
  }
  stopwatch.stop();

  std::vector<int> v(N);
  std::iota(v.begin(), v.end(), 1);
  return std::make_pair(v, true);
}

std::pair<std::vector<int>, bool> MainProgram::add_random_contours(Stopwatch &stopwatch, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int max_levels)
{
  const unsigned int w = x2 - x1;
  if (w > DEF_MAZE_WIDTH)
    return add_random_contours2(stopwatch, x1, y1, x2, y2, max_levels);

  const unsigned int h = y2 - y1;
  unsigned int nmb_of_hills_and_valleys = suggest_number_of_hilos(w, h);

  std::vector<int> contour_ids = {};

  std::vector<std::vector<std::map<int, std::vector<Coord>>>> hilos;
  //  rectangles to fill the area (x1,y1)..(x2, y2)=> either a hill or a valley
  auto rects = getRects(nmb_of_hills_and_valleys, x1, y1, x2, y2);

  for (const auto &rect : rects)
  {
    std::vector<std::map<int, std::vector<Coord>>> arr;
    arr = Rectangle::hilo_factory(arr, rect, rand() % 2 == 0 ? 1 : -1, max_levels);
    hilos.push_back(arr);
  }

  // Populate the 2D vector with data from hilos
  int contour_id = CONTOUR_COUNTER;
  bool isOK = true;
  bool retval = false;
  for (const auto &hilo : hilos)
  {
    for (const auto &z_dict : hilo)
    {
      for (const auto &pair : z_dict)
      {
        int z = pair.first;
        const std::vector<Coord> &coords = pair.second;

        bool is_hill = (z > 0);
        int height = z;
        string name = (is_hill ? "hill_" : "valley_") + std::to_string(contour_id);

        // Outprint the contour_id and name
        stopwatch.start();
        retval = ds_.add_contour(contour_id, name, height, coords);
        stopwatch.stop();

        if (retval)
        {
          ++CONTOUR_COUNTER;
          valid_contour_ids_.insert(contour_id);
          contour_ids_.push_back(contour_id);
          ++random_contours_added_;
          contour_ids.push_back(contour_id++);

        }
      }
    }
  }
  return std::make_pair(contour_ids, isOK);
}

int MainProgram::add_random_bites_and_contours(Stopwatch &stopwatch, int nmb_of_bites,
                                               int x1, int y1, int x2, int y2)
{

  int real_nmb_added = 0;
  if (nmb_of_bites < 0)
  {
    std::cerr << "Invalid argument: nmb_of_bites " << nmb_of_bites << " cannot be negative" << std::endl;
    return real_nmb_added;
  }

  int w = x2 - x1;
  if (w < 0)
  {
    std::cerr << "Invalid argument: width " << w << " cannot be negative" << std::endl;
    return real_nmb_added;
  }

  int h = y2 - y1;
  if (h < 0)
  {
    std::cerr << "Invalid argument: height " << h << " cannot be negative" << std::endl;
    return real_nmb_added;
  }

  try
  {
    std::vector<int> contour_ids = {};
    try
    {
      std::pair<std::vector<int>, bool> result = add_random_contours(stopwatch, x1, y1, x2, y2);
      contour_ids = result.first;
    }
    catch (...)
    {
      std::cerr << "Error" << std::endl;
    }
    return add_random_bites_to_given_contours(stopwatch, nmb_of_bites, contour_ids);
  }
  catch (const std::invalid_argument &e)
  {
    std::cerr << "Invalid argument: " << e.what() << std::endl;
    return real_nmb_added;
  }
}

MainProgram::CmdResult
MainProgram::cmd_random_add(ostream &output, MatchIter begin, MatchIter end)
{

  string sizestr = *begin++;
  string minxstr = *begin++;
  string minystr = *begin++;
  string maxxstr = *begin++;
  string maxystr = *begin++;
  assert(begin == end && "Invalid number of parameters!");

  unsigned int nmb_of_bites = convert_string_to<unsigned int>(sizestr);

  if (!minxstr.empty() && !minystr.empty() && !maxxstr.empty() &&
      !maxystr.empty())
  {
    min.x = convert_string_to<unsigned int>(minxstr);
    min.y = convert_string_to<unsigned int>(minystr);
    max.x = convert_string_to<unsigned int>(maxxstr);
    max.y = convert_string_to<unsigned int>(maxystr);
  }
  else
  {
    min.x = 0;
    min.y = 0;
    //    if (nmb_of_bites<=100){
    if (false)
    {
      max.x = DEF_MAZE_WIDTH;
      max.y = DEF_MAZE_HEIGHT;
    }
    else
    {
      max.x = nmb_of_bites;
      max.y = nmb_of_bites;
    }
  }

  Stopwatch dummy;
  test_add_n_contours(dummy, min.x, min.y, max.x, max.y);
  add_random_coord_connections(dummy, nmb_of_bites, min.x, min.y, max.x, max.y);
  int real_nmb_added = nmb_of_bites;
  output << "Added: " << real_nmb_added << " bites. Yay!" << endl;

  view_dirty = true;

  return {};
}

bool MainProgram::add_random_coord_connections(Stopwatch &stopwatch,
                                               unsigned int size, int x1, int y1, int x2, int y2)
{

  min.x = x1;
  min.y = y1;
  max.x = x2;
  max.y = y2;

  // Add the wanted number of bites
  for (unsigned int i = 0; i < size; ++i)
  {
    bool retval = test_add_bite(stopwatch);
    if (!retval)
    {
      return false;
    }
  }

  // add connections between the bites
  if (random_bites_added_ > 1) // Don't do anything if there's no bites
  {
    for (; random_connections_added_ < random_bites_added_ / 2;)
    {
      if (!test_add_connection(stopwatch))
      {
        return false;
      }
    }
  }

  return true;
}


/**
 * Just bites - no contours
 * If also contours use test_randomadd instead
 *
 */
bool MainProgram::test_random_bites(Stopwatch &stopwatch)
{
  return add_random_coord_connections(stopwatch, 1, 0, 0, max.x, max.y);
}

bool MainProgram::test_get_bite_count(Stopwatch &stopwatch)
{
  stopwatch.start();
  auto result = ds_.get_bite_count();
  stopwatch.stop();
  return result == random_bites_added_;
}

bool MainProgram::test_clear_all(Stopwatch &stopwatch)
{
  stopwatch.start();
  ds_.clear_all();
  stopwatch.stop();
  clear_test_cache();
  return true;
}

bool MainProgram::test_add_bite(Stopwatch &stopwatch)
{
  BiteID BiteID = n_to_BiteID(random_bites_added_);
  Name nodename = n_to_name(random_bites_added_);
  Coord newcoord = n_to_coord(random_bites_added_);
  stopwatch.start();
  bool retval = ds_.add_bite(BiteID, nodename, newcoord);
  stopwatch.stop();
  ++random_bites_added_;
  return retval;
}

bool MainProgram::test_get_bite_name(Stopwatch &stopwatch)
{
  if (random_bites_added_ < 1)
  {
    return true;
  }

  BiteID id = random_valid_bite();
  stopwatch.start();
  Name retval = ds_.get_bite_name(id);
  stopwatch.stop();
  return retval != NO_NAME;
}

bool MainProgram::test_get_bite_coord(Stopwatch &stopwatch)
{
  if (random_bites_added_ < 1)
  {
    return true;
  }
  BiteID id = random_valid_bite();
  stopwatch.start();
  Coord retval = ds_.get_bite_coord(id);
  stopwatch.stop();
  return retval != NO_COORD;
}

bool MainProgram::test_all_contours(Stopwatch &stopwatch)
{
  stopwatch.start();
  auto result = ds_.all_contours();
  stopwatch.stop();
  if (result.size() != random_contours_added_)
  {
    return false;
  }
  std::unordered_set<ContourID> unique(result.begin(), result.end());
  if (unique.size() != result.size())
  {
    return false;
  }
  for (const auto &id : result)
  {
    if (valid_contour_ids_.find(id) == valid_contour_ids_.end())
    {
      return false;
    }
  }
  return true;
}

bool MainProgram::test_get_contour_name(Stopwatch &stopwatch)
{
  if (random_contours_added_ < 1)
  {
    return true;
  }

  ContourID contourid = random_valid_contour();

  stopwatch.start();
  auto result = ds_.get_contour_name(contourid);
  stopwatch.stop();
  return result != NO_NAME;
}

bool MainProgram::test_get_contour_coords(Stopwatch &stopwatch)
{
  ContourID contourid = random_valid_contour();

  stopwatch.start();
  auto result = ds_.get_contour_coords(contourid);
  stopwatch.stop();

  if (result.size() < 1)
  {
    return false;
  }
  for (const auto &coord : result)
  {
    if (coord == NO_COORD)
    {
      return false;
    }
  }
  return true;
}

bool MainProgram::test_add_contour(Stopwatch &stopwatch)
{

  auto contourid = n_to_contourid(random_contours_added_);

  auto height = random_contour_height();

  vector<Coord> coords;
  // possible to change the number of coords by random chance
  for (int j = 0; j < 3; ++j)
  {
    coords.push_back(random_coord());
  }
  Name contourname = convert_to_string(contourid);
  stopwatch.start();
  bool retval =
      ds_.add_contour(contourid, contourname, height, std::move(coords));
  stopwatch.stop();
  ++random_contours_added_;
  return retval;
}

bool MainProgram::test_add_n_contours(Stopwatch &stopwatch, int x1, int y1, int x2, int y2)
{

  int w = x2 - x1;
  if (w < 0)
  {
    std::cerr << "Invalid argument: width " << w << " cannot be negative" << std::endl;
    return false;
  }

  int h = y2 - y1;
  if (h < 0)
  {
    std::cerr << "Invalid argument: height " << h << " cannot be negative" << std::endl;
    return false;
  }

  int max_levels = MAX_CONTOUR_HEIGHT;

  unsigned int nmb_of_hills_and_valleys = suggest_number_of_hilos(w, h);

  try
  {
    // Seed random number generator
    std::srand(std::time(0));

    // Generate rectangles and matrix
    auto rects = getRects(nmb_of_hills_and_valleys, x1, y1, x2, y2);

    std::vector<int> contour_ids = {};
    try
    {
      // contour_ids, status
      std::pair<std::vector<int>, bool> result = add_random_contours(stopwatch, x1, y1, x2, y2, max_levels);
      bool status = result.second;
      return status;
    }
    catch (...)
    {
      std::cerr << "Error" << std::endl;
      return false;
    }
  }
  catch (const std::invalid_argument &e)
  {
    std::cerr << "Invalid argument: " << e.what() << std::endl;
    return false;
  }
}

bool MainProgram::test_add_subcontour_to_contour(Stopwatch &stopwatch)
{
  Stopwatch dummy;
  // add a new subcontour to continue making binary tree with
  unsigned int i = random_contours_added_;
  if (!test_add_contour(dummy))
  {
    return false;
  }

  auto contourid = n_to_contourid(i);
  auto parentid = n_to_contourid(i / 2);
  stopwatch.start();
  bool retval = ds_.add_subcontour_to_contour(contourid, parentid);
  stopwatch.stop();
  return retval;
}

bool MainProgram::test_add_coord_to_contour(Stopwatch &stopwatch)
{
  Stopwatch dummy;
  if (random_contours_added_ < 1)
  {
    if (!test_add_contour(dummy))
    {
      return false;
    }
  }
  auto contourid = random_valid_contour();
  int i = random_bites_added_;
  if (!test_add_bite(dummy))
  {
    return false;
  }
  auto BiteID = n_to_BiteID(i);
  stopwatch.start();
  bool retval = ds_.add_bite_to_contour(BiteID, contourid);
  stopwatch.stop();
  return retval;
}

bool MainProgram::test_get_bites_alphabetically(Stopwatch &stopwatch)
{
  // get the ordered thing

  stopwatch.start();
  auto result = ds_.get_bites_alphabetically();
  stopwatch.stop();
  if (result.size() != random_bites_added_)
  {
    return false;
  }
  std::unordered_set<BiteID> unique(result.begin(), result.end());
  if (unique.size() != result.size())
  {
    return false;
  }
  std::vector<Name> names(result.size());
  size_t i = 0;
  for (const auto &id : result)
  {
    auto name = ds_.get_bite_name(id);
    if (name == NO_NAME ||
        (valid_bite_names_.find(name) == valid_bite_names_.end()))
    {
      return false;
    }
    names[i++] = name;
  }
  // check for the order
  return std::is_sorted(names.begin(), names.end());
}

bool MainProgram::test_get_bites_distance_increasing(Stopwatch &stopwatch)
{
  // get the ordered thing

  stopwatch.start();
  auto result = ds_.get_bites_distance_increasing();
  stopwatch.stop();
  if (result.size() != random_bites_added_)
  {
    return false;
  }
  std::unordered_set<BiteID> unique(result.begin(), result.end());
  if (unique.size() != result.size())
  {
    return false;
  }
  std::vector<unsigned long long int> something(result.size());
  size_t i = 0;
  for (const auto &id : result)
  {
    auto coord = ds_.get_bite_coord(id);
    if (coord == NO_COORD ||
        (valid_coords_.find(coord) == valid_coords_.end()))
    {
      return false;
    }

    something[i++] = abs(coord.x) + abs(coord.y);
  }

  // check for the order
  return std::is_sorted(something.begin(), something.end());
}

MainProgram::CmdResult MainProgram::cmd_get_bite_count(ostream &output,
                                                       MatchIter begin,
                                                       MatchIter end)
{
  assert(begin == end && "Invalid number of parameters!");

  output << "Number of bites: " << ds_.get_bite_count() << endl;

  return {};
}

MainProgram::CmdResult
MainProgram::cmd_all_bites(ostream &output, MatchIter begin, MatchIter end)
{
  assert(begin == end && "Invalid number of parameters!");

  auto nodes = ds_.all_bites();
  if (nodes.empty())
  {
    output << "No bites!" << endl;
  }

  std::sort(nodes.begin(), nodes.end());
  return {ResultType::IDLIST,
          std::make_tuple(std::vector<ContourID>{},
                          std::vector<BiteID>{nodes.begin(), nodes.end()},
                          std::vector<ConnectionID>{})};
}

MainProgram::CmdResult MainProgram::cmd_add_contour(std::ostream & /*output*/,
                                                    MatchIter begin,
                                                    MatchIter end)
{
  ContourID id = convert_string_to<ContourID>(*begin++);
  string name = *begin++;
  int height = convert_string_to<int>(*begin++);
  string coordsstr = *begin++;
  assert(begin == end && "Invalid number of parameters!");

  vector<Coord> coords;
  smatch coord;
  auto sbeg = coordsstr.cbegin();
  auto send = coordsstr.cend();
  for (; regex_search(sbeg, send, coord, coords_regex_);
       sbeg = coord.suffix().first)
  {
    coords.push_back(
        {convert_string_to<int>(coord[1]), convert_string_to<int>(coord[2])});
  }

  bool success = ds_.add_contour(id, name, height, coords);

  view_dirty = true;
  return {ResultType::IDLIST,
          std::make_tuple(std::vector<ContourID>{success ? id : NO_CONTOUR},
                          std::vector<BiteID>{}, std::vector<ConnectionID>{})};
}

MainProgram::CmdResult MainProgram::cmd_all_contours(std::ostream &output,
                                                     MatchIter begin,
                                                     MatchIter end)
{
  assert(begin == end && "Invalid number of parameters!");

  auto contours = ds_.all_contours();
  if (contours.empty())
  {
    output << "No contours!" << endl;
  }

  std::sort(contours.begin(), contours.end());
  return {
      ResultType::IDLIST,
      std::make_tuple(std::vector<ContourID>{contours.begin(), contours.end()},
                      std::vector<BiteID>{}, std::vector<ConnectionID>{})};
}

bool MainProgram::test_all_bites(Stopwatch &stopwatch)
{
  stopwatch.start();
  auto result = ds_.all_bites();
  stopwatch.stop();

  // checks for result validity:
  if (result.size() != random_bites_added_)
  {
    return false;
  }

  std::unordered_set<BiteID> unique(result.begin(), result.end());
  if (unique.size() != result.size())
  {
    return false;
  }

  for (const auto &id : result)
  {
    if (valid_bite_ids_.find(id) == valid_bite_ids_.end())
    {
      return false;
    }
  }
  return true;
}

MainProgram::CmdResult
MainProgram::cmd_all_subcontours_of_contour(std::ostream &output,
                                            MatchIter begin, MatchIter end)
{
  ContourID contourid = convert_string_to<ContourID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");

  auto contours = ds_.all_subcontours_of_contour(contourid);
  if (contours.empty())
  {
    output << "No contours!" << endl;
  }

  std::sort(contours.begin(), contours.end());
  contours.insert(contours.begin(),
                  contourid); // Add parameter as the first contour
  return {
      ResultType::IDLIST,
      std::make_tuple(std::vector<ContourID>{contours.begin(), contours.end()},
                      std::vector<BiteID>{}, std::vector<ConnectionID>{})};
}

Distance MainProgram::calc_distance(Coord c1, Coord c2)
{
  if (c1 == NO_COORD || c2 == NO_COORD)
  {
    return NO_DISTANCE;
  }

  long long int deltax = c1.x - c2.x;
  long long int deltay = c1.y - c2.y;
  return static_cast<Distance>(std::sqrt(deltax * deltax + deltay * deltay));
}

MainProgram::CmdResult
MainProgram::cmd_clear_all(ostream &output, MatchIter begin, MatchIter end)
{
  assert(begin == end && "Invalid number of parameters");

  ds_.clear_all();
  init_primes();

  output << "Cleared all bites" << endl;

  view_dirty = true;

  return {};
}

string MainProgram::print_bite(BiteID id, ostream &output, bool nl)
{
  try
  {
    if (id != NO_BITE)
    {
      auto name = ds_.get_bite_name(id);
      auto xy = ds_.get_bite_coord(id);
      if (!name.empty())
      {
        output << name << ": ";
      }
      else
      {
        output << "*: ";
      }

      output << "pos=";
      print_coord(xy, output, false);
      output << ", id=" << id;
      if (nl)
      {
        output << endl;
      }

      ostringstream retstream;
      retstream << id;
      return retstream.str();
    }
    else
    {
      output << "--NO_BITE--";
      if (nl)
      {
        output << endl;
      }
      return "";
    }
  }
  catch (NotImplemented const &e)
  {
    output << endl
           << "NotImplemented while printing node : " << e.what() << endl;
    std::cerr << endl
              << "NotImplemented while printing node : " << e.what() << endl;
    return "";
  }
}

string MainProgram::print_bite_brief(BiteID id, std::ostream &output, bool nl)
{
  try
  {
    if (id != NO_BITE)
    {
      auto name = ds_.get_bite_name(id);
      if (!name.empty())
      {
        output << name << " ";
      }
      else
      {
        output << "* ";
      }

      output << "(" << id << ")";
      if (nl)
      {
        output << endl;
      }

      ostringstream retstream;
      retstream << id;
      return retstream.str();
    }
    else
    {
      output << "--NO_BITE--";
      if (nl)
      {
        output << endl;
      }
      return "";
    }
  }
  catch (NotImplemented const &e)
  {
    output << endl
           << "NotImplemented while printing bite info : " << e.what() << endl;
    std::cerr << endl
              << "NotImplemented while printing bite info : " << e.what()
              << endl;
    return "";
  }
}

string MainProgram::print_coord_brief(Coord c, std::ostream &output, bool nl)
{
  if (c == NO_COORD)
  {
    output << "--NO_COORD--";
    if (nl)
    {
      output << endl;
    }
    return "";
  }

  output << "(" << c.x << "," << c.y << ")";
  if (nl)
  {
    output << endl;
  }

  ostringstream retstream;
  retstream << c.x << " " << c.y;
  return retstream.str();
}

string MainProgram::print_contour(ContourID id, std::ostream &output, bool nl)
{
  try
  {
    if (id != NO_CONTOUR)
    {
      auto name = ds_.get_contour_name(id);
      //            auto xy = ds_.get_contour_coords(id);
      if (!name.empty())
      {
        output << name << ": ";
      }
      else
      {
        output << "*: ";
      }

      output << "id=" << id;
      if (nl)
      {
        output << endl;
      }

      ostringstream retstream;
      retstream << id;
      return retstream.str();
    }
    else
    {
      output << "--NO_CONTOUR--";
      if (nl)
      {
        output << endl;
      }
      return "";
    }
  }
  catch (NotImplemented const &e)
  {
    output << endl
           << "NotImplemented while printing contour : " << e.what() << endl;
    std::cerr << endl
              << "NotImplemented while printing contour : " << e.what() << endl;
    return "";
  }
}

string MainProgram::print_connection(ConnectionID id, std::ostream &output,
                                     bool nl)
{
  try
  {
    if (id != NO_CONNECTION)
    {
      output << "id=" << id;
      if (nl)
      {
        output << endl;
      }

      ostringstream retstream;
      retstream << id;
      return retstream.str();
    }
    else
    {
      output << "--NO_CONNECTION--";
      if (nl)
      {
        output << endl;
      }
      return "";
    }
  }
  catch (NotImplemented const &e)
  {
    output << endl
           << "NotImplemented while printing connection : " << e.what() << endl;
    std::cerr << endl
              << "NotImplemented while printing connection : " << e.what()
              << endl;
    return "";
  }
}

MainProgram::CmdResult
MainProgram::cmd_find_bite_with_coord(ostream & /*output*/, MatchIter begin,
                                      MatchIter end)
{
  string xstr = *begin++;
  string ystr = *begin++;
  assert(begin == end && "Invalid number of parameters!");

  int x = convert_string_to<int>(xstr);
  int y = convert_string_to<int>(ystr);

  auto bite = ds_.find_bite_with_coord({x, y});
  return {ResultType::ID, CmdResultID{bite, NO_CONTOUR}};
}

bool MainProgram::test_find_bite_with_coord(Stopwatch &stopwatch)
{
  if (random_bites_added_ < 1)
  {
    return true;
  }
  Coord testcoord = random_valid_coord();
  stopwatch.start();
  auto result = ds_.find_bite_with_coord(testcoord);
  stopwatch.stop();
  if (result == NO_BITE)
  {
    return true;
  }
  return valid_bite_ids_.find(result) != valid_bite_ids_.end();
}

bool MainProgram::test_get_bites_closest_to(Stopwatch &stopwatch)
{
  auto searchcoord = random_coord();
  stopwatch.start();
  auto result = ds_.get_bites_closest_to(searchcoord);
  stopwatch.stop();

  unsigned int size = std::min((unsigned long int)3, random_bites_added_);
  if (result.size() != size)
  {
    return false;
  }
  std::unordered_set<BiteID> unique(result.begin(), result.end());
  if (unique.size() != result.size())
  {
    return false;
  }
  for (const auto &id : result)
  {
    if (valid_bite_ids_.find(id) == valid_bite_ids_.end())
    {
      return false;
    }
  }
  return true;
}

bool MainProgram::test_get_closest_common_ancestor_of_contours(
    Stopwatch &stopwatch)
{
  if (random_contours_added_ < 1)
  {
    return true;
  }

  ContourID id1 = random_valid_contour();
  ContourID id2 = random_valid_contour();
  stopwatch.start();
  ContourID result = ds_.get_closest_common_ancestor_of_contours(id1, id2);
  stopwatch.stop();

  return result == NO_CONTOUR ||
         (valid_contour_ids_.find(result) != valid_contour_ids_.end());
}

MainProgram::CmdResult MainProgram::cmd_randseed(std::ostream &output,
                                                 MatchIter begin,
                                                 MatchIter end)
{
  string seedstr = *begin++;
  assert(begin == end && "Invalid number of parameters");

  unsigned long int seed = convert_string_to<unsigned long int>(seedstr);

  rand_engine_.seed(seed);
  init_primes();

  output << "Random seed set to " << seed << endl;

  return {};
}

MainProgram::CmdResult MainProgram::cmd_read(std::ostream &output,
                                             MatchIter begin, MatchIter end)
{
  string filename = *begin++;
  string silentstr = *begin++;
  assert(begin == end && "Invalid number of parameters!");

  bool silent = !silentstr.empty();
  ostream *new_output = &output;

  ostringstream dummystr; // Given as output if "silent" is specified, the
                          // output is discarded
  if (silent)
  {
    new_output = &dummystr;
  }

  ifstream input(filename);
  if (input)
  {
    output << "** Commands from '" << filename << "'" << endl;
    command_parser(input, *new_output, PromptStyle::NORMAL);
    if (silent)
    {
      output << "...(output discarded in silent mode)..." << endl;
    }
    output << "** End of commands from '" << filename << "'" << endl;
  }
  else
  {
    output << "Cannot open file '" << filename << "'1!" << endl;
  }

  return {};
}

/**
 * =====================
 * = PRG2 COMMANDS    =
 * =====================
 */

MainProgram::CmdResult MainProgram::cmd_add_connection(std::ostream &,
                                                       MatchIter begin,
                                                       MatchIter end)
{
  // TODO: UNTESTED
  // It takes the following parameters: connection_id, bite_id1, bite_id2 and
  // the coordinates of the connection.
  ConnectionID connection_id = convert_string_to<ConnectionID>(*begin++);
  vector<BiteID> bite_ids;
  bite_ids.push_back(convert_string_to<BiteID>(*begin++));
  bite_ids.push_back(convert_string_to<BiteID>(*begin++));
  string coordsstr = *begin++;
  assert(begin == end && "Invalid number of parameters!");
  vector<Coord> coords;
  smatch coord;
  auto sbeg = coordsstr.cbegin();
  auto send = coordsstr.cend();
  for (; regex_search(sbeg, send, coord, coords_regex_);
       sbeg = coord.suffix().first)
  {
    coords.push_back(
        {convert_string_to<int>(coord[1]), convert_string_to<int>(coord[2])});
  }

  // Coord list can be empty
  bool success = ds_.add_connection(connection_id, bite_ids[0],bite_ids[1], coords);

  return {ResultType::IDLIST,
          std::make_tuple(std::vector<ContourID>{}, std::vector<BiteID>{},
                          std::vector<ConnectionID>{success ? connection_id
                                                            : NO_CONNECTION})};
}

bool MainProgram::test_add_connection(Stopwatch &stopwatch)
{
  // It takes the following parameters: connection_id, bite_id1, bite_id2 and
  // the coordinates of the connection.
  ConnectionID connection_id = n_to_connectionid(random_connections_added_++);
  BiteID bite_id1 = random_valid_bite();
  BiteID bite_id2 = random_valid_bite();
  vector<BiteID> bite_ids = {bite_id1, bite_id2};

  Coord coord1 = ds_.get_bite_coord(bite_id1);
  Coord coord2 = ds_.get_bite_coord(bite_id2);
  vector<Coord> coords;
  if (coord1.x == coord2.x || coord1.y == coord2.y)
  {
    coords = {};
  }
  else
  {
    Coord middle = {coord1.x, coord2.y};
    coords = {middle};
  }
  stopwatch.start();
  bool success = ds_.add_connection(connection_id, bite_id1,bite_id2, coords);
  stopwatch.stop();

  return success;
}

MainProgram::CmdResult MainProgram::cmd_clear_connections(std::ostream &output,
                                                          MatchIter begin,
                                                          MatchIter end)
{
  assert(begin == end && "Invalid number of parameters");

  ds_.clear_connections();
  output << "Cleared all connections" << endl;

  return {};
}

bool MainProgram::test_clear_connections(Stopwatch &stopwatch)
{
  stopwatch.start();
  ds_.clear_connections();
  stopwatch.stop();
  return true;
}

MainProgram::CmdResult MainProgram::cmd_next_bites_from(std::ostream &output,
                                                        MatchIter begin,
                                                        MatchIter end)
{
  // gets one parameter: the bite_id
  auto bite_id = convert_string_to<BiteID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");

  auto bites = ds_.get_next_bites_from(bite_id);
  if (bites.empty())
  {
    output << "No bites!" << endl;
  }

  return {ResultType::IDLIST,
          std::make_tuple(std::vector<ContourID>{},
                          std::vector<BiteID>{bites.begin(), bites.end()},
                          std::vector<ConnectionID>{})};
}

bool MainProgram::test_next_bites_from(Stopwatch &stopwatch)
{

  if (random_bites_added_ < 1)
  {
    return true;
  }
  auto id = random_valid_bite();
  stopwatch.start();
  auto result = ds_.get_next_bites_from(id);
  stopwatch.stop();
  for (const auto &rid : result)
  {
    if (valid_bite_ids_.find(rid) == valid_bite_ids_.end())
    {
      return false;
    }
  }
  return true;
}

MainProgram::CmdResult MainProgram::cmd_get_connections(std::ostream &output,
                                                        MatchIter begin,
                                                        MatchIter /*end*/)
{
  auto bite_id1 = NO_BITE;
  auto bite_id2 = NO_BITE;
  try
  {
    bite_id1 = convert_string_to<BiteID>(*begin++);
    bite_id2 = convert_string_to<BiteID>(*begin++);
  }
  catch (std::invalid_argument const &e)
  {
    // No bite_id given, it's ok
  }

  auto connections = ds_.get_connections(bite_id1, bite_id2);

  if (connections.empty())
  {
    output << "No connections!" << endl;
  }

  std::sort(connections.begin(), connections.end());
  return {ResultType::IDLIST,
          std::make_tuple(std::vector<ContourID>{}, std::vector<BiteID>{},
                          std::vector<ConnectionID>{connections.begin(),
                                                    connections.end()})};
}

bool MainProgram::test_get_connections(Stopwatch &stopwatch)
{
  if (random_bites_added_ < 2)
  {
    return true;
  }
  auto id1 = random_valid_bite();
  auto id2 = random_valid_bite();
  stopwatch.start();
  auto result = ds_.get_connections(id1, id2);
  stopwatch.stop();
  for (const auto &rid : result)
  {
    if (valid_connection_ids_.find(rid) == valid_connection_ids_.end())
    {
      return false;
    }
  }
  return true;
}

MainProgram::CmdResult
MainProgram::cmd_get_connection_coords(std::ostream &output, MatchIter begin,
                                       MatchIter end)
{
  // It should get two parameters: bite_id and connection_id
  auto bite_id = convert_string_to<BiteID>(*begin++);
  auto connection_id = convert_string_to<ConnectionID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");

  auto coords = ds_.get_connection_coords(bite_id, connection_id);

  if (coords.empty())
  {
    output << "No coordinates!" << endl;
  }

  return {ResultType::COORDLIST, CmdResultCoords{{}, coords}};
}

bool MainProgram::test_get_connection_coords(Stopwatch &stopwatch)
{
  if (random_bites_added_ < 1)
  {
    return true;
  }
  auto id = random_valid_bite();
  auto connection_id = random_valid_connection();
  stopwatch.start();
  auto result = ds_.get_connection_coords(id, connection_id);
  stopwatch.stop();
  for (const auto &coord : result)
  {
    if (valid_coords_.find(coord) == valid_coords_.end())
    {
      return false;
    }
  }
  return true;
}

MainProgram::CmdResult MainProgram::cmd_path_any(std::ostream &output,
                                                 MatchIter begin,
                                                 MatchIter end)
{
  // Should receive two params: from_bite_id and to_bite_id
  auto from_bite_id = convert_string_to<BiteID>(*begin++);
  auto to_bite_id = convert_string_to<BiteID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");

  std::vector<std::pair<Coord, Distance>> steps =
      ds_.path_any(from_bite_id, to_bite_id);

#ifdef GUI
  ui_->update_path_distance(steps);
#endif

  CmdResultRoute result;

  if (steps.empty())
  {
    output << "Path empty!" << endl;
  }
  else if (steps.front() == make_pair(NO_COORD, NO_DISTANCE))
  {
    output << "Path not found!" << endl;
  }
  else
  {
    auto [coord, dist] = steps.front();
    for (auto iter = steps.begin() + 1; iter != steps.end(); ++iter)
    {
      auto &[ncoord, ndist] = *iter;
      result.emplace_back(coord, ncoord, dist);
      coord = ncoord;
      dist = ndist;
    }
    result.emplace_back(coord, NO_COORD, dist);
  }
  return {ResultType::ROUTE, result};
}

bool MainProgram::test_path_any(Stopwatch &stopwatch)
{
  // Should receive two params: from_bite_id and to_bite_id
  if (random_bites_added_ < 2)
  {
    return true;
  }
  auto from_bite_id = random_valid_bite();
  auto to_bite_id = random_valid_bite();
  stopwatch.start();
  auto result = ds_.path_any(from_bite_id, to_bite_id);
  stopwatch.stop();
  return true;
}

MainProgram::CmdResult MainProgram::cmd_path_shortest(std::ostream &output,
                                                      MatchIter begin,
                                                      MatchIter end)
{
  // Should receive two params: from_bite_id and to_bite_id
  auto from_bite_id = convert_string_to<BiteID>(*begin++);
  auto to_bite_id = convert_string_to<BiteID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");

  std::vector<std::pair<Coord, Distance>> steps =
      ds_.path_shortest(from_bite_id, to_bite_id);

#ifdef GUI

  ui_->update_path_distance(steps);
#endif

  CmdResultRoute result;

  if (steps.empty())
  {
    output << "Path empty!" << endl;
  }
  else if (steps.front() == make_pair(NO_COORD, NO_DISTANCE))
  {
    output << "Path not found!" << endl;
  }
  else
  {
    auto [coord, dist] = steps.front();
    for (auto iter = steps.begin() + 1; iter != steps.end(); ++iter)
    {
      auto &[ncoord, ndist] = *iter;
      result.emplace_back(coord, ncoord, dist);
      coord = ncoord;
      dist = ndist;
    }
    result.emplace_back(coord, NO_COORD, dist);
  }
  return {ResultType::ROUTE, result};
}

bool MainProgram::test_path_shortest(Stopwatch &stopwatch)
{
  // Should receive two params: from_bite_id and to_bite_id
  if (random_bites_added_ < 2)
  {
    return true;
  }
  auto from_bite_id = random_valid_bite();
  auto to_bite_id = random_valid_bite();
  stopwatch.start();
  auto result = ds_.path_shortest(from_bite_id, to_bite_id);
  stopwatch.stop();
  return true;
}

MainProgram::CmdResult MainProgram::cmd_path_least_bites(std::ostream &output,
                                                         MatchIter begin,
                                                         MatchIter end)
{
  // Should receive two params: from_bite_id and to_bite_id
  auto from_bite_id = convert_string_to<BiteID>(*begin++);
  auto to_bite_id = convert_string_to<BiteID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");

  std::vector<std::pair<Coord, Distance>> steps =
      ds_.path_least_bites(from_bite_id, to_bite_id);

#ifdef GUI
  ui_->update_path_distance(steps);
#endif

  CmdResultRoute result;

  if (steps.empty())
  {
    output << "Path empty!" << endl;
  }
  else if (steps.front() == make_pair(NO_COORD, NO_DISTANCE))
  {
    output << "Path not found!" << endl;
  }
  else
  {
    auto [coord, dist] = steps.front();
    for (auto iter = steps.begin() + 1; iter != steps.end(); ++iter)
    {
      auto &[ncoord, ndist] = *iter;
      result.emplace_back(coord, ncoord, dist);
      coord = ncoord;
      dist = ndist;
    }
    result.emplace_back(coord, NO_COORD, dist);
  }
  return {ResultType::ROUTE, result};
}

bool MainProgram::test_path_least_bites(Stopwatch &stopwatch)
{
  // Should receive two params: from_bite_id and to_bite_id
  if (random_bites_added_ < 2)
  {
    return true;
  }
  auto from_bite_id = random_valid_bite();
  auto to_bite_id = random_valid_bite();
  stopwatch.start();
  auto result = ds_.path_least_bites(from_bite_id, to_bite_id);
  stopwatch.stop();
  return true;
}

MainProgram::CmdResult MainProgram::cmd_path_least_uphill(std::ostream &output,
                                                          MatchIter begin,
                                                          MatchIter end)
{
  // Should receive two params: from_bite_id and to_bite_id
  auto from_bite_id = convert_string_to<BiteID>(*begin++);
  auto to_bite_id = convert_string_to<BiteID>(*begin++);
  assert(begin == end && "Invalid number of parameters!");

  std::vector<std::pair<Coord, Distance>> steps =
      ds_.path_least_uphill(from_bite_id, to_bite_id);

#ifdef GUI
  ui_->update_path_distance(steps);
#endif

  CmdResultRoute result;

  if (steps.empty())
  {
    output << "Path empty!" << endl;
  }
  else if (steps.front() == make_pair(NO_COORD, NO_DISTANCE))
  {
    output << "Path not found!" << endl;
  }
  else
  {
    auto [coord, dist] = steps.front();
    for (auto iter = steps.begin() + 1; iter != steps.end(); ++iter)
    {
      auto &[ncoord, ndist] = *iter;
      result.emplace_back(coord, ncoord, dist);
      coord = ncoord;
      dist = ndist;
    }
    result.emplace_back(coord, NO_COORD, dist);
  }
  return {ResultType::ROUTE, result};
}

bool MainProgram::test_path_least_uphill(Stopwatch &stopwatch)
{
  // Should receive two params: from_bite_id and to_bite_id
  if (random_bites_added_ < 2)
  {
    return true;
  }
  auto from_bite_id = random_valid_bite();
  auto to_bite_id = random_valid_bite();
  stopwatch.start();
  auto result = ds_.path_least_uphill(from_bite_id, to_bite_id);
  stopwatch.stop();
  return true;
}


MainProgram::CmdResult MainProgram::cmd_testread(std::ostream &output,
                                                 MatchIter begin,
                                                 MatchIter end)
{
  string infilename = *begin++;
  string outfilename = *begin++;
  assert(begin == end && "Invalid number of parameters!");

  ifstream input(infilename);
  if (input)
  {
    ifstream expected_output(outfilename);
    if (output)
    {
      stringstream actual_output;
      command_parser(input, actual_output, PromptStyle::NO_NESTING);

      vector<string> actual_lines;
      while (actual_output)
      {
        string line;
        getline(actual_output, line);
        if (!actual_output)
        {
          break;
        }
        actual_lines.push_back(line);
      }

      vector<string> expected_lines;
      while (expected_output)
      {
        string line;
        getline(expected_output, line);
        if (!expected_output)
        {
          break;
        }
        expected_lines.push_back(line);
      }

      string heading_actual = "Actual output";
      unsigned int actual_max_length = heading_actual.length();
      auto actual_max_iter = max_element(
          actual_lines.cbegin(), actual_lines.cend(),
          [](string s1, string s2)
          { return s1.length() < s2.length(); });
      if (actual_max_iter != actual_lines.cend())
      {
        actual_max_length = actual_max_iter->length();
      }

      string heading_expected = "Expected output";
      unsigned int expected_max_length = heading_expected.length();
      auto expected_max_iter = std::max_element(
          expected_lines.cbegin(), expected_lines.cend(),
          [](string s1, string s2)
          { return s1.length() < s2.length(); });
      if (expected_max_iter != expected_lines.cend())
      {
        expected_max_length = expected_max_iter->length();
      }

      auto pos_actual = actual_lines.cbegin();
      auto pos_expected = expected_lines.cbegin();
      output << "  " << heading_actual
             << string(actual_max_length - heading_actual.length(), ' ')
             << " | " << heading_expected << endl;
      output << "--" << string(actual_max_length, '-') << "-|-"
             << string(expected_max_length, '-') << endl;

      bool lines_ok = true;
      while (pos_expected != expected_lines.cend() ||
             pos_actual != actual_lines.cend())
      {
        if (pos_expected != expected_lines.cend())
        {
          if (pos_actual != actual_lines.cend())
          {
            bool ok = (*pos_expected == *pos_actual);
            output << (ok ? ' ' : '?') << ' ' << *pos_actual
                   << string(actual_max_length - pos_actual->length(), ' ')
                   << " | " << *pos_expected << endl;
            lines_ok = lines_ok && ok;
            ++pos_actual;
          }
          else
          { // Actual output was too short
            output << "? " << string(actual_max_length, ' ') << " | "
                   << *pos_expected << endl;
            lines_ok = false;
          }
          ++pos_expected;
        }
        else
        { // Actual output was too long
          output << "? " << *pos_actual
                 << string(actual_max_length - pos_actual->length(), ' ')
                 << " | " << endl;
          lines_ok = false;
          ++pos_actual;
        }
      }
      if (lines_ok)
      {
        output << "**No differences in output.**" << endl;
        if (test_status_ == TestStatus::NOT_RUN)
        {
          test_status_ = TestStatus::NO_DIFFS;
        }
      }
      else
      {
        output << "**Differences found! (Lines beginning with '?')**" << endl;
        test_status_ = TestStatus::DIFFS_FOUND;
      }
    }
    else
    {
      output << "Cannot open file '" << outfilename << "'!!!!" << endl;
    }
  }
  else
  {
    output << "Cannot open file '" << infilename << "'!!!" << endl;
  }

  return {};
}

MainProgram::CmdResult MainProgram::cmd_stopwatch(std::ostream &output,
                                                  MatchIter begin,
                                                  MatchIter end)
{
  string on = *begin++;
  string off = *begin++;
  string next = *begin++;
  assert(begin == end && "Invalid number of parameters");

  if (!on.empty())
  {
    stopwatch_mode = StopwatchMode::ON;
    output << "Stopwatch: on" << endl;
  }
  else if (!off.empty())
  {
    stopwatch_mode = StopwatchMode::OFF;
    output << "Stopwatch: off" << endl;
  }
  else if (!next.empty())
  {
    stopwatch_mode = StopwatchMode::NEXT;
    output << "Stopwatch: on for the next command" << endl;
  }
  else
  {
    assert(!"Invalid stopwatch mode!");
  }

  return {};
}

std::string MainProgram::print_bite_name(BiteID id, std::ostream &output,
                                         bool nl)
{
  try
  {
    if (id != NO_BITE)
    {
      auto name = ds_.get_bite_name(id);
      if (!name.empty())
      {
        output << name;
      }
      else
      {
        output << "*";
      }

      ostringstream retstream;
      retstream << name;
      if (nl)
      {
        output << endl;
      }
      return retstream.str();
    }
    else
    {
      output << "--NO_BITE--";
      if (nl)
      {
        output << endl;
      }
      return "";
    }
  }
  catch (NotImplemented const &e)
  {
    output << endl
           << "NotImplemented while printing bite name : " << e.what() << endl;
    std::cerr << endl
              << "NotImplemented while printing bite name : " << e.what()
              << endl;
    return "";
  }
}

std::string MainProgram::print_coord(Coord coord, std::ostream &output,
                                     bool nl)
{
  if (coord != NO_COORD)
  {
    output << "(" << coord.x << "," << coord.y << ")";
    ostringstream retstream;
    retstream << "(" << coord.x << "," << coord.y << ")";
    if (nl)
    {
      output << endl;
    }
    return retstream.str();
  }
  else
  {
    output << "(--NO_COORD--)";
    if (nl)
    {
      output << endl;
    }
    return "";
  }
}

// REGEX
string const BiteIDx = "([a-zA-Z0-9-]+)";
// Create optionalBiteIdx for bite ids that are optional:
string const OptionalBiteIdx = "([a-zA-Z0-9-]*)";
string const contouridx = "([0-9]+)";
string const connectionidx = "([0-9]+)";
string const namex = "([ a-zA-Z0-9-]+)";
string const timex = "((?:[01][0-9][0-5][0-9])|(?:2[0-3][0-5][0-9]))";
string const numx = "([0-9]+)";
string const optcoordx =
    "\\([[:space:]]*[0-9]+[[:space:]]*,[[:space:]]*[0-9]+[[:space:]]*\\)";
string const coordx =
    "\\([[:space:]]*([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)[[:space:]]*\\)";
string const wsx = "[[:space:]]+";
string const optionalwsx = "[[:space:]]*";
string const optnodetimeidx =
    "[a-zA-Z0-9-]+:(?:(?:[01][0-9][0-5][0-9])|(?:2[0-3][0-5][0-9]))";

string const perftest_cmds_listx =
    "((?:[0-9]+\\*)?[0-9a-zA-Z_]+(?:;(?:[0-9]+\\*)?[0-9a-zA-Z_]+)*(?:(?::(?:[0-"
    "9]+\\*)?[0-9a-zA-Z_]+)(?:;(?:[0-9]+\\*)?[0-9a-zA-Z_]+)*(?::[0-9]+)?)?)";
string const perftest_timeoutx = "([0-9]+(?::[0-9]+)?)";
string const perftest_n_values_or_rangex =
    "([0-9]+(?:(?::[0-9]+){1,2}|(?:;[0-9]+)+)?)";
string const perftest_logx = "(?:[[:space:]]+(log))?";
string const add_contour_pattern = "([-0-9]+)\\s+\"([a-zA-Z0-9_-]+)\"\\s+(-?[0-9]+)\\s+((?:\\(\\s*-?\\s*[0-9]+\\s*,\\s*-?\\s*[0-9]+\\s*\\)\\s*)+)";

string const add_bite_pattern = "([a-zA-Z0-9-]+)\\s+\"([^\"]+)\"\\s+\\(\\s*([0-"
                                "9]+)\\s*,\\s*([0-9]+)\\s*\\)";
string const add_connection_pattern =
    "([a-zA-Z0-9]+)\\s+([a-zA-Z0-9]+)\\s+([a-zA-Z0-9]+)\\s*((?:\\(\\s*-?[0-9]+"
    "\\s*,\\s*-?[0-9]+\\s*\\)\\s*)*)\\s*";

vector<MainProgram::CmdInfo> MainProgram::cmds_ = {
    {"get_bite_count", "", "", &MainProgram::cmd_get_bite_count,
     &MainProgram::test_get_bite_count},
    {"clear_all", "", "", &MainProgram::cmd_clear_all,
     &MainProgram::test_clear_all},
    {"all_bites", "", "", &MainProgram::cmd_all_bites,
     &MainProgram::test_all_bites},
    {"add_bite", "BiteID \"Emoji\" (x,y)", add_bite_pattern,
     &MainProgram::cmd_add_bite, &MainProgram::test_add_bite},
    {"get_bite_name", "BiteID", BiteIDx, &MainProgram::cmd_get_bite_name,
     &MainProgram::test_get_bite_name},
    {"get_bite_coord", "BiteID", BiteIDx, &MainProgram::cmd_get_bite_coord,
     &MainProgram::test_get_bite_coord},
    {"get_bites_alphabetically", "", "",
     &MainProgram::NoParListCmd<&Datastructures::get_bites_alphabetically>,
     &MainProgram::test_get_bites_alphabetically},
    {"get_bites_distance_increasing", "", "",
     &MainProgram::NoParListCmd<&Datastructures::get_bites_distance_increasing>,
     &MainProgram::test_get_bites_distance_increasing},
    {"find_bite_with_coord", "(x,y)", coordx,
     &MainProgram::cmd_find_bite_with_coord,
     &MainProgram::test_find_bite_with_coord},
    {"add_contour", "ContourID \"Name\" height (x,y) (x,y)...",
     add_contour_pattern, &MainProgram::cmd_add_contour,
     &MainProgram::test_add_contour},
    {"all_contours", "", "", &MainProgram::cmd_all_contours,
     &MainProgram::test_all_contours},
    {"get_contour_name", "ContourID", contouridx,
     &MainProgram::cmd_get_contour_name, &MainProgram::test_get_contour_name},
    {"get_contour_height", "ContourID", contouridx,
     &MainProgram::cmd_get_contour_height, &MainProgram::test_get_contour_height},
    {"get_contour_coords", "ContourID", contouridx,
     &MainProgram::cmd_get_contour_coords,
     &MainProgram::test_get_contour_coords},
    {"add_subcontour_to_contour", "SubcontourID ContourID",
     contouridx + wsx + contouridx, &MainProgram::cmd_add_subcontour_to_contour,
     &MainProgram::test_add_subcontour_to_contour},
    {"add_bite_to_contour", "BiteID ContourID", BiteIDx + wsx + contouridx,
     &MainProgram::cmd_add_bite_to_contour,
     &MainProgram::test_add_coord_to_contour},
    {"get_bite_in_contours", "BiteID", BiteIDx,
     &MainProgram::cmd_get_bite_in_contours,
     &MainProgram::test_get_bite_in_contours},
    {"all_subcontours_of_contour", "ContourID", contouridx,
     &MainProgram::cmd_all_subcontours_of_contour,
     &MainProgram::test_all_subcontours_of_contour},
    {"get_bites_closest_to", "(x,y)", coordx,
     &MainProgram::cmd_get_bites_closest_to,
     &MainProgram::test_get_bites_closest_to},
    {"remove_bite", "BiteID", BiteIDx, &MainProgram::cmd_remove_bite,
     &MainProgram::test_remove_bite},
    {"get_closest_common_ancestor_of_contours", "ContourID1 ContourID2",
     contouridx + wsx + contouridx,
     &MainProgram::cmd_get_closest_common_ancestor_of_contours,
     &MainProgram::test_get_closest_common_ancestor_of_contours},
    // PRG2:
    {"add_connection", "ConnectionID BiteID1 BiteID2 (x,y) (x,y)...",
     add_connection_pattern, &MainProgram::cmd_add_connection,
     &MainProgram::test_add_connection},
    {"clear_connections", "", "", &MainProgram::cmd_clear_connections,
     &MainProgram::test_clear_connections},
    {"get_next_bites_from", "BiteID", BiteIDx,
     &MainProgram::cmd_next_bites_from, &MainProgram::test_next_bites_from},
    {"get_connections", "(Optional:BiteID1 BiteID2)",
     OptionalBiteIdx + optionalwsx + OptionalBiteIdx, &MainProgram::cmd_get_connections,
     &MainProgram::test_get_connections},
    {"get_contour_height", "ContourID", contouridx,
     &MainProgram::cmd_get_contour_height,
     &MainProgram::test_get_contour_height},
    {"get_connection_coords", "BiteID ConnectionID",
     BiteIDx + wsx + connectionidx, &MainProgram::cmd_get_connection_coords,
     &MainProgram::test_get_connection_coords},
    {"path_any", "BiteID1 BiteID2", BiteIDx + wsx + BiteIDx,
     &MainProgram::cmd_path_any, &MainProgram::test_path_any},
    {"path_shortest", "BiteID1 BiteID2", BiteIDx + wsx + BiteIDx,
     &MainProgram::cmd_path_shortest, &MainProgram::test_path_shortest},
    {"path_least_uphill", "BiteID1 BiteID2", BiteIDx + wsx + BiteIDx,
     &MainProgram::cmd_path_least_uphill, &MainProgram::test_path_least_uphill},
    {"path_least_bites", "BiteID1 BiteID2", BiteIDx + wsx + BiteIDx,
     &MainProgram::cmd_path_least_bites, &MainProgram::test_path_least_bites},
    {"quit", "", "", nullptr, nullptr},
    {"help", "", "", &MainProgram::help_command, nullptr},
    {"random_add",
     "number_of_nodes_to_add  (minx,miny) (maxx,maxy) (coordinates "
     "optional)",
     numx + "(?:" + wsx + coordx + wsx + coordx + ")?",
     &MainProgram::cmd_random_add, &MainProgram::test_random_bites},
    {"read", "\"in-filename\" [silent]",
     "\"([-a-zA-Z0-9 ./:_]+)\"(?:" + wsx + "(silent))?", &MainProgram::cmd_read,
     nullptr},
    {"testread", "\"in-filename\" \"out-filename\"",
     "\"([-a-zA-Z0-9 ./:_]+)\"" + wsx + "\"([-a-zA-Z0-9 ./:_]+)\"",
     &MainProgram::cmd_testread, nullptr},
    {"perftest",
     "[REPEAT*]cmd1[;[REPEAT*]cmd2...][:[REPEAT*]extracmd1[;[REPEAT*]"
     "extracmd2."
     "..]:extra_every_n_test_entry] timeout[:extra_timeout] "
     "range_start:range_end[:step]|n1[;n2...] [log] (parts in [] are "
     "optional)",
     perftest_cmds_listx + wsx + perftest_timeoutx + wsx +
         perftest_n_values_or_rangex + perftest_logx,
     &MainProgram::cmd_perftest, nullptr},
    {"stopwatch", "on|off|next (alternatives separated by |)",
     "(?:(on)|(off)|(next))", &MainProgram::cmd_stopwatch, nullptr},
    {"random_seed", "new-random-seed-integer", numx, &MainProgram::cmd_randseed,
     nullptr},
    {"#", "comment text", ".*", &MainProgram::cmd_comment, nullptr},
};
/*
 * regex cmds;cmds:cmds;cmds
 * ([0-9a-zA-Z_]+(?:;[0-9a-zA-Z_]+)*(?:(?::[0-9a-zA-Z_]+)(?:;[0-9a-zA-Z_]+)*)?)
 *
 *
 * timeoutit
 * ([0-9]+(?::[0-9]+)?)
 *
 * extra_cmds_after
 * ([0-9]+)
 *
 * N ja ranget siihen
 * ([0-9]+(?::[0-9]+){0,2}(?:;[0-9]+(?::[0-9]+){0,2})*)
 *
 * log
 * ((?:log)?)
 */

MainProgram::CmdResult MainProgram::help_command(std::ostream &output,
                                                 MatchIter /*begin*/,
                                                 MatchIter /*end*/)
{
  output << "Commands:" << endl;
  for (auto &i : cmds_)
  {
    output << "  " << i.cmd << " " << i.info << endl;
  }

  return {};
}

bool MainProgram::init_perftest(Stopwatch &stopwatch, std::string testcmd, int n)
{

  if (bite_tests.find(testcmd) != bite_tests.end())
  {
    add_random_bites(stopwatch, n);
  }
  else if (contour_tests.find(testcmd) != contour_tests.end())
  {
    add_random_contours(stopwatch, 0, 0, n, n);
  }
  else if (bite_contour_tests.find(testcmd) != bite_contour_tests.end())
  {
    add_random_bites_and_contours(stopwatch, n, 0, 0, n, n);
  }
  else
  {
    // std::cout << "unknown command " << testcmd;
  }
  return true;
}

MainProgram::CmdResult MainProgram::cmd_perftest(std::ostream &output,
                                                 MatchIter begin,
                                                 MatchIter end)
{
#ifdef _GLIBCXX_DEBUG
  output << "WARNING: Debug STL enabled, performance will be worse than "
            "expected (maybe also asymptotically)!"
         << endl;
#endif // _GLIBCXX_DEBUG

  string commandstr = *begin++;
  string timeoutstr = *begin++;
  string n_rangestr = *begin++;
  string logstr = *begin++;
  assert(begin == end && "Invalid number of parameters");

  // parse the commands and extra commands
  vector<pair<unsigned int, string>> testcmds;
  vector<pair<unsigned int, string>> extracmds;
  unsigned int n_extra = 1;
  // to reduce the number of unnecessary data in ram
  {
    vector<string> test_extrastr = {"", "", ""};
    auto cbeg = commandstr.cbegin();
    auto cend = commandstr.cend();
    size_t i = 0;
    for (smatch sma; regex_search(cbeg, cend, sma, commands_divide_regex_);
         cbeg = sma.suffix().first)
    {
      test_extrastr[i++] = sma[1];
    }
    // first find the regular commands
    cbeg = test_extrastr[0].cbegin();
    cend = test_extrastr[0].cend();
    for (smatch scmd; regex_search(cbeg, cend, scmd, commands_regex_);
         cbeg = scmd.suffix().first)
    {
      unsigned int repeat_count =
          scmd[1] == "" ? 1 : convert_string_to<unsigned int>(scmd[1]);
      testcmds.push_back({repeat_count, scmd[2]});
    }
    // then add the extra commands
    cbeg = test_extrastr[1].cbegin();
    cend = test_extrastr[1].cend();
    for (smatch scmd; regex_search(cbeg, cend, scmd, commands_regex_);
         cbeg = scmd.suffix().first)
    {
      unsigned int repeat_count =
          scmd[1] == "" ? 1 : convert_string_to<unsigned int>(scmd[1]);
      extracmds.push_back({repeat_count, scmd[2]});
    }
    if (test_extrastr[1] != "" && test_extrastr[2] == "")
    {
      output << "using extra commands requires every n parameter" << endl;
      return {};
    }
    n_extra = test_extrastr[2] == ""
                  ? 1
                  : convert_string_to<unsigned int>(test_extrastr[2]);
    if (n_extra < 1)
    {
      output << "extra commands can not be run every 0 normal command listing"
             << endl;
      return {};
    }
  }
  vector<unsigned int> timeouts;
  {
    auto cbeg = timeoutstr.cbegin();
    auto cend = timeoutstr.cend();
    for (smatch timeoutsmatch;
         regex_search(cbeg, cend, timeoutsmatch, timeouts_regex_);
         cbeg = timeoutsmatch.suffix().first)
    {
      timeouts.push_back(convert_string_to<unsigned int>(timeoutsmatch[1]));
    }
  }
  if (!extracmds.empty() && timeouts.size() == 1)
  {
    output << "Using extra commands requires timeout for extra commands"
           << endl;
    return {};
  }

  bool is_range = false;
  {
    auto cbeg = n_rangestr.cbegin();
    auto cend = n_rangestr.cend();
    smatch isrange;
    regex_search(cbeg, cend, isrange, is_range_notation_regex_);
    is_range = isrange[1] != "";
  }
  vector<unsigned int> init_ns;
  if (is_range)
  {
    vector<unsigned int> range_limits;
    auto sbeg = n_rangestr.cbegin();
    auto send = n_rangestr.cend();
    for (smatch range; regex_search(sbeg, send, range, range_regex_);
         sbeg = range.suffix().first)
    {
      range_limits.push_back(convert_string_to<unsigned int>(range[1]));
    }
    if (range_limits[0] >= range_limits[1])
    {
      output << "Lower bound of range must be less than upper bound" << endl;
      return {};
    }
    if (logstr.empty())
    {
      unsigned int step = 1;
      if (range_limits.size() == 3)
      {
        step = range_limits[2];
      }
      if (step < 1)
      {
        output << "Step for linear range can not be < 1" << endl;
        return {};
      }
      // linear range
      for (unsigned int n = range_limits[0]; n <= range_limits[1]; n += step)
      {
        init_ns.push_back(n);
      }
    }
    else
    {
      unsigned int step = 10;
      if (range_limits.size() == 3)
      {
        step = range_limits[2];
      }
      if (step < 2)
      {
        output << "Base for log range can not be < 2" << endl;
        return {};
      }
      unsigned int overflow_boundary_check =
          std::numeric_limits<unsigned int>::max() / step;

      for (unsigned int n = range_limits[0]; n <= range_limits[1]; n *= step)
      {
        init_ns.push_back(n);
        if (overflow_boundary_check < n)
        {
          break;
        }
      }
    }
  }
  else
  {
    auto sbeg = n_rangestr.cbegin();
    auto send = n_rangestr.cend();
    for (smatch size; regex_search(sbeg, send, size, sizes_regex_);
         sbeg = size.suffix().first)
    {
      init_ns.push_back(convert_string_to<unsigned int>(size[1]));
    }
  }

  try
  {

    output << "Timeout for each N is " << timeouts[0]
           << " sec (for ADD and test functions)";
    // perftest
    // nodes_distance_increasing;nodes_distance_increasing:nodes_distance_increasing
    // 1:1 1 1000
    if (!extracmds.empty())
    {
      output << " + " << timeouts[1] << " sec (for extra functions)";
    }
    output << ". " << endl;
    output << "For each N perform command(s):" << endl;

    // Initialize test functions
    vector<pair<unsigned int, bool (MainProgram::*)(Stopwatch &)>> testfuncs;
    vector<pair<unsigned int, bool (MainProgram::*)(Stopwatch &)>> extrafuncs;

    for (auto &i : testcmds)
    {
      auto pos = find_if(cmds_.begin(), cmds_.end(),
                         [&i](auto const &cmd)
                         { return cmd.cmd == i.second; });
      if (pos != cmds_.end() && pos->testfunc)
      {
        output << i.second << " ";
        testfuncs.push_back({i.first, pos->testfunc});
      }
      else
      {
        output << "(cannot test " << i.second << ") ";
      }
    }
    if (!extracmds.empty())
    {
      output << endl
             << "with extra functions" << endl;

      for (auto &i : extracmds)
      {
        auto pos = find_if(cmds_.begin(), cmds_.end(), [&i](auto const &cmd)
                           { return cmd.cmd == i.second; });
        if (pos != cmds_.end() && pos->testfunc)
        {
          output << i.second << " ";
          extrafuncs.push_back({i.first, pos->testfunc});
        }
        else
        {
          output << "(cannot add extra function " << i.second << ") ";
        }
      }
      output << endl
             << "after every " << n_extra << " entries in test functions"
             << endl;
    }
    output << endl
           << endl;

    if (testfuncs.empty())
    {
      output << "No commands to test!" << endl;
      return {};
    }

#ifdef USE_PERF_EVENT
    output << setw(7) << "N"
           << " , " << setw(12) << "add (sec)"
           << " , " << setw(12) << "add (count)"
           << " , " << setw(12) << "cmds (sec)"
           << " , " << setw(12) << "cmds (count)"
           << " , " << setw(12) << "total (sec)"
           << " , " << setw(12) << "total (count)" << endl;
#else

    output << setw(7) << "N"
           << " , " << setw(12) << "add (sec)"
           << " , " << setw(12) << "cmds (sec)"
           << " , " << setw(12) << "total (sec)" << endl;
#endif

    flush_output(output);

    auto stop = false;

    for (unsigned int n : init_ns)
    {
      if (stop)
      {
        break;
      }

      output << setw(7) << n << " , " << flush;
      ds_.clear_all();
      init_primes();

      Stopwatch stopwatch(true); // Use also instruction counting, if enabled
      Stopwatch extrasw;

      // Add a random contour and n random nodes with connections
      test_add_n_contours(stopwatch, 0, 0, std::sqrt(n) * 2, std::sqrt(n) * 2);

      for (unsigned int i = 0; i < n / 1000; ++i)
      {
        add_random_coord_connections(stopwatch, 1000, 0, 0, std::sqrt(n) * 2, std::sqrt(n) * 2);

        if (stopwatch.elapsed() >= timeouts[0])
        {
          output << "ADD Timeout!" << endl;
          stop = true;
          break;
        }
        if (check_stop())
        {
          output << "Stopped!" << endl;
          stop = true;
          break;
        }
      }

      if (stop)
      {
        break;
      }

      if (n % 1000 != 0)
      {
        add_random_coord_connections(stopwatch, n % 1000, 0, 0, std::sqrt(n) * 2, std::sqrt(n) * 2);
      }

#ifdef USE_PERF_EVENT
      auto addcount = stopwatch.count();
#endif
      auto addsec = stopwatch.elapsed();

#ifdef USE_PERF_EVENT
      output << setw(12) << addsec << " , " << setw(12) << addcount << " , "
             << flush;
#else
      output << setw(12) << addsec << " , " << flush;
#endif

      if (addsec >= timeouts[0])
      {
        output << "ADD Timeout!" << endl;
        stop = true;
        break;
      }

      unsigned int extra = 0;
      for (auto pair : testfuncs)
      {
        auto cmdpos = pair.second;
        for (unsigned int i = 0; i < pair.first; ++i)
        {
          if (!(this->*cmdpos)(stopwatch))
          {
            output << "Failed check!" << endl;
            stop = true;
            break;
          }
          if (stopwatch.elapsed() >= timeouts[0])
          {
            output << "Timeout!" << endl;
            stop = true;
            break;
          }
          if (check_stop())
          {
            output << "Stopped!" << endl;
            stop = true;
            break;
          }
        }
        if (stop)
        {
          break;
        }
        // run extra functions every "n_extra" elements in the command list
        if ((++extra) % (n_extra < 1 ? 1 : n_extra) == 0 &&
            extra < testfuncs.size())
        {
          for (auto extrapair : extrafuncs)
          {
            auto cmdpos = extrapair.second;
            for (unsigned int i = 0; i < extrapair.first; ++i)
            {
              if (!(this->*cmdpos)(extrasw))
              {
                output << "Extrafunc Failed check!" << endl;
                stop = true;
                break;
              }
              if (extrasw.elapsed() >= timeouts[1])
              {
                output << "Extrafunc Timeout!" << endl;
                stop = true;
                break;
              }
              if (check_stop())
              {
                output << "Stopped!" << endl;
                stop = true;
                break;
              }
            }
          }
        }
        if (stop)
        {
          break;
        }
      }
      if (stop)
      {
        break;
      }

#ifdef USE_PERF_EVENT
      auto totalcount = stopwatch.count();
#endif
      auto totalsec = stopwatch.elapsed();

#ifdef USE_PERF_EVENT
      output << setw(12) << totalsec - addsec << " , " << setw(12)
             << totalcount - addcount << " , " << setw(12) << totalsec << " , "
             << setw(12) << totalcount;
#else
      output << setw(12) << totalsec - addsec << " , " << setw(12) << totalsec;
#endif
      output << endl;
      flush_output(output);
    }

    ds_.clear_all();
    init_primes();
  }
  catch (NotImplemented const &)
  {
    // Clean up after NotImplemented
    ds_.clear_all();
    init_primes();
    throw;
  }

#ifdef _GLIBCXX_DEBUG
  output << "WARNING: Debug STL enabled, performance will be worse than "
            "expected (maybe also asymptotically)!"
         << endl;
#endif // _GLIBCXX_DEBUG

  return {};
}

MainProgram::CmdResult MainProgram::cmd_comment(std::ostream & /*output*/,
                                                MatchIter /*begin*/,
                                                MatchIter /*end*/)
{
  return {};
}

bool MainProgram::command_parse_line(string inputline, ostream &output)
{

  if (inputline.empty())
  {
    return true;
  }

  smatch match;
  bool matched = regex_match(inputline, match, cmds_regex_);
  if (matched)
  {
    assert(match.size() == 3);
    string cmd = match[1];
    string params = match[2];
    // show cmd and params in output

    auto pos = find_if(cmds_.begin(), cmds_.end(),
                       [cmd](CmdInfo const &ci)
                       { return ci.cmd == cmd; });
    assert(pos != cmds_.end());

    smatch match2;
    bool matched2 = regex_match(params, match2, pos->param_regex);
    if (matched2)
    {
      if (pos->func)
      {
        assert(!match2.empty());

        Stopwatch stopwatch;
        bool use_stopwatch = (stopwatch_mode != StopwatchMode::OFF);
        // Reset stopwatch mode if only for the next command
        if (stopwatch_mode == StopwatchMode::NEXT)
        {
          stopwatch_mode = StopwatchMode::OFF;
        }

        TestStatus initial_status = test_status_;
        test_status_ = TestStatus::NOT_RUN;

        if (use_stopwatch)
        {
          stopwatch.start();
        }

        CmdResult result;
        try
        {
          result =
              (this->*(pos->func))(output, ++(match2.begin()), match2.end());
        }
        catch (NotImplemented const &e)
        {
          output << endl
                 << "NotImplemented from cmd " << pos->cmd << " : " << e.what()
                 << endl;
          std::cerr << endl
                    << "NotImplemented from cmd " << pos->cmd << " : "
                    << e.what() << endl;
        }

        if (use_stopwatch)
        {
          stopwatch.stop();
        }

        switch (result.first)
        {
        case ResultType::NOTHING:
        {
          break;
        }
        case ResultType::IDLIST:
        {
          auto &[contours, bites, connections] =
              std::get<CmdResultIDs>(result.second);
          if (bites.size() == 1 && bites.front() == NO_BITE)
          {
            output << "Failed (NO_BITE returned)!" << std::endl;
          }
          else
          {
            if (!bites.empty())
            {
              if (bites.size() == 1)
              {
                output << "Bite:" << std::endl;
              }
              else
              {
                output << "Bites:" << std::endl;
              }

              unsigned int num = 0;
              for (BiteID id : bites)
              {
                ++num;
                if (bites.size() > 1)
                {
                  output << num << ". ";
                }
                else
                {
                  output << "   ";
                }
                print_bite(id, output, true);
              }
            }
          }

          if (contours.size() == 1 && contours.front() == NO_CONTOUR)
          {
            output << "Failed (NO_CONTOUR returned)!" << std::endl;
          }
          else
          {
            if (!contours.empty())
            {
              if (contours.size() == 1)
              {
                output << "Contour:" << std::endl;
              }
              else
              {
                output << "Contours:" << std::endl;
              }

              unsigned int num = 0;
              for (ContourID id : contours)
              {
                ++num;
                if (contours.size() > 1)
                {
                  output << num << ". ";
                }
                else
                {
                  output << "   ";
                }
                print_contour(id, output);
              }
            }
          }
          if (connections.size() == 1 && connections.front() == NO_CONNECTION)
          {
            output << "Failed (NO_CONNECTION returned)!" << std::endl;
          }
          else
          {
            if (!connections.empty())
            {
              if (connections.size() == 1)
              {
                output << "Connection:" << std::endl;
              }
              else
              {
                output << "Connections:" << std::endl;
              }

              unsigned int num = 0;
              for (ConnectionID id : connections)
              {
                ++num;
                if (connections.size() > 1)
                {
                  output << num << ". ";
                }
                else
                {
                  output << "   ";
                }
                print_connection(id, output);
              }
            }
          }
          break;
        }
        case ResultType::NAMELIST:
        {
          auto &resulttype = std::get<CmdResultNames>(result.second);
          auto &sid = resulttype.first.first;
          auto rid = convert_to_string(resulttype.first.second);
          auto &name = resulttype.second;
          std::string type = sid != NO_BITE ? "bite" : "contour";
          if (name == NO_NAME)
          {
            output << "Failed (NO_NAME returned)!" << std::endl;
            break;
          }
          output << "Name of " << type << " with id "
                 << (sid != NO_BITE ? std::to_string(sid) : rid);
          output << " is " << name << endl;
          break;
        }
        case ResultType::HEIGHTLIST:
        {
          auto &resulttype = std::get<CmdResultHeights>(result.second);
          auto &sid = resulttype.first.first;
          auto rid = convert_to_string(resulttype.first.second);
          auto &height = resulttype.second;
          std::string type = sid != NO_BITE ? "bite" : "contour";
          if (height == NO_CONTOUR_HEIGHT)
          {
            output << "Failed (NO_HEIGHT returned)!" << std::endl;
            break;
          }
          output << "Height of " << type << " with id "
                 << (sid != NO_BITE ? std::to_string(sid) : rid);
          output << " is " << height << endl;
          break;
        }
        case ResultType::COORDLIST:
        {
          auto &resulttype = std::get<CmdResultCoords>(result.second);
          auto &sid = resulttype.first.first;
          auto rid = convert_to_string(resulttype.first.second);
          auto &coordlist = resulttype.second;
          // Set the type of the output using a switch case: Bite, connection or
          // contour

          std::string type = sid != NO_BITE ? "Bite " : "Contour ";
          if (coordlist.size() == 1 && coordlist.front() == NO_COORD)
          {
            output << "Failed (NO_COORD returned)!" << std::endl;
            break;
          }
          output << "Coordinates:" << endl;
          for (const auto &coord : coordlist)
          {
            print_coord(coord, output);
          }
          break;
        }
        case ResultType::ID:
        {
          auto &id = std::get<CmdResultID>(result.second).first;
          print_bite(id, output, true);
          break;
        }
        case ResultType::ROUTE:
        {
          auto &route = std::get<CmdResultRoute>(result.second);
          if (!route.empty())
          {
            if (route.size() == 1 && get<1>(route.front()) == NO_COORD)
            {
              output << "Failed (...NO_BITE... returned)!" << std::endl;
            }
            else
            {
              unsigned int num = 1;
              for (auto &r : route)
              {
                // auto [NO_BITE, BiteID1, BiteID2, time, dist] = r;
                auto [coord1, coord2, dist] = r;
                output << num << ". ";
                if (coord1 != NO_COORD)
                {
                  print_coord_brief(coord1, output, false);
                }
                if (coord2 != NO_COORD)
                {
                  output << " -> ";
                  print_coord_brief(coord2, output, false);
                }
                if (dist != NO_DISTANCE)
                {
                  output << " (distance " << dist << ")";
                }
                output << endl;

                ++num;
              }
            }
          }
          break;
        }
        default:
        {
          assert(false && "Unsupported result type!");
        }
        }

        if (result != prev_result)
        {
          prev_result = std::move(result);
          view_dirty = true;
        }

        if (use_stopwatch)
        {
          output << "Command '" << cmd << "': " << stopwatch.elapsed()
                 << " sec";
          output << endl;
        }

        if (test_status_ != TestStatus::NOT_RUN)
        {
          output << "Testread-tests have been run, "
                 << ((test_status_ == TestStatus::DIFFS_FOUND)
                         ? "differences found!"
                         : "no differences found.")
                 << endl;
        }
        if (test_status_ == TestStatus::NOT_RUN ||
            (test_status_ == TestStatus::NO_DIFFS &&
             initial_status == TestStatus::DIFFS_FOUND))
        {
          test_status_ = initial_status;
        }
      }
      else
      { // No function to run = quit command
        return false;
      }
    }
    else
    {
      output << "Invalid parameters for command '" << cmd << "'!" << endl;
      //   show the parameters that were expected, and what were given
      output << "Given: " << params << endl;
      output << "Expected: " << pos->param_regex_str << endl;
    }
  }
  else
  {
    output << "Unknown command!" << endl;
  }

  return true; // Signal continuing
}

void MainProgram::command_parser(istream &input, ostream &output,
                                 PromptStyle promptstyle)
{
  string line;
  do
  {
    //        output << string(nesting_level, '>') << " ";
    output << PROMPT;
    getline(input, line, '\n');

    if (promptstyle != PromptStyle::NO_ECHO)
    {
      output << line << endl;
    }

    if (!input)
    {
      break;
    }

    bool cont = command_parse_line(line, output);
    view_dirty = false; // No need to keep track of individual result changes
    if (!cont)
    {
      break;
    }
  } while (input);

  view_dirty = true; // To be safe, assume that results have been changed
}

void MainProgram::setui(MainWindow *ui) { ui_ = ui; }

#ifdef GRAPHICAL_GUI
void MainProgram::flush_output(std::ostream &output)
{
  if (ui_)
  {
    if (auto soutput = dynamic_cast<ostringstream *>(&output))
    {
      ui_->output_text(*soutput);
    }
  }
}
#else
void MainProgram::flush_output(std::ostream & /*output*/) {}
#endif

bool MainProgram::check_stop() const
{
#ifdef GRAPHICAL_GUI
  if (ui_)
  {
    return ui_->check_stop_pressed();
  }
#endif
  return false;
}

std::array<unsigned long int, 20> const MainProgram::primes1{
    4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999, 5003,
    5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087};
std::array<unsigned long int, 20> const MainProgram::primes2{
    81031, 81041, 81043, 81047, 81049, 81071, 81077, 81083, 81097, 81101,
    81119, 81131, 81157, 81163, 81173, 81181, 81197, 81199, 81203, 81223};

MainProgram::MainProgram()
{
  rand_engine_.seed(time(nullptr));

  //    startmem = get<0>(mempeak());

  init_primes();
  init_regexs();
}

int MainProgram::mainprogram(int argc, char *argv[])
{
  vector<string> args(argv, argv + argc);

  if (args.size() < 1 || args.size() > 2)
  {
    cerr << "Usage: " + ((args.size() > 0) ? args[0] : "<program name>") +
                " [<command file>]"
         << endl;
    return EXIT_FAILURE;
  }

  MainProgram mainprg;

  if (args.size() == 2 && args[1] != "--console")
  {
    string filename = args[1];
    ifstream input(filename);
    if (input)
    {
      mainprg.command_parser(input, cout, MainProgram::PromptStyle::NORMAL);
    }
    else
    {
      cout << "Cannot open file '" << filename << "'!" << endl;
    }
  }
  else
  {
    mainprg.command_parser(cin, cout, MainProgram::PromptStyle::NO_ECHO);
  }

  cerr << "Program ended normally." << endl;
  if (mainprg.test_status_ == TestStatus::DIFFS_FOUND)
  {
    return EXIT_FAILURE;
  }
  else
  {
    return EXIT_SUCCESS;
  }
}

void MainProgram::init_primes()
{
  // Initialize id generator
  prime1_ = primes1[random<int>(0, primes1.size())];
  prime2_ = primes2[random<int>(0, primes2.size())];
  random_bites_added_ = 0;
  random_contours_added_ = 0;
  random_connections_added_ = 0;
  random_coords_added_ = 0;
  clear_test_cache();
}

std::string MainProgram::random_emoji()
{
  // Make emoji_ranges_ static to avoid reinitializing it every time the
  // function is called
  static const std::vector<std::pair<int, int>> emoji_ranges_ = {
      {0x1F600, 0x1F64F}, // Emoticons
      {0x2700, 0x27BF},   // Dingbats
      {0x1F680, 0x1F6FF}, // Transport and Map Symbols
      {0x1F300, 0x1F5FF}, // Miscellaneous Symbols and Pictographs
      {0x1F900, 0x1F9FF}  // Supplemental Symbols and Pictographs
  };

  // Select a random range
  std::uniform_int_distribution<int> range_distr(0, emoji_ranges_.size() - 1);
  auto selected_range = emoji_ranges_[range_distr(rand_engine_)];

  // Generate a random code point within the selected range
  std::uniform_int_distribution<int> codepoint_distr(selected_range.first,
                                                     selected_range.second);
  int codepoint = codepoint_distr(rand_engine_);

  // Convert the code point to a UTF-8 string
  std::string emoji;
  if (codepoint <= 0x7F)
  {
    emoji += static_cast<char>(codepoint);
  }
  else if (codepoint <= 0x7FF)
  {
    emoji += static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F));
    emoji += static_cast<char>(0x80 | (codepoint & 0x3F));
  }
  else if (codepoint <= 0xFFFF)
  {
    emoji += static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F));
    emoji += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
    emoji += static_cast<char>(0x80 | (codepoint & 0x3F));
  }
  else if (codepoint <= 0x10FFFF)
  {
    emoji += static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07));
    emoji += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
    emoji += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
    emoji += static_cast<char>(0x80 | (codepoint & 0x3F));
  }

  return emoji;
}

Name MainProgram::random_name()
{
  Name id;
  // there might be changes in this function on grader

  // all perftest Names are 8 characters long -> (27*2)^20 possibilities
  // this results in pretty low chance of collisions when using <=10^6 ids

  // first get charcode
  auto chardistr = std::uniform_int_distribution<int>('A', 'Z' + ('z' - 'a'));
  for (unsigned int i = 0; i < 1; ++i)
  {
    auto charint = chardistr(rand_engine_);
    // if charcode is above uppercase, add offset for lowercase charcodes
    id += (char)(charint > 'Z' ? charint + ('a' - 'Z') : charint);
  }
  return id;
}

Coord MainProgram::random_coord()
{
  int x = random<int>(min.x, max.x);
  int y = random<int>(min.y, max.y);
  Coord newcoord{x, y};
  return newcoord;
}

int MainProgram::random_contour_height()
{
  return random<int>(min_height, max_height);
}

ContourID MainProgram::random_contour_id()
{
  return random<ContourID>(0, MAX_CONTOURID);
}


ConnectionID MainProgram::random_connection_id()
{
  return random<ConnectionID>(0, MAX_CONNECTIONID);
}


Name MainProgram::n_to_name(unsigned long n)
{

  try
  {
    if (n < valid_bite_names_.size())
    {
      // this WILL result in different names for same n since valid_names_ is
      // unordered set
      return *std::next(valid_bite_names_.begin(), n);
    }

    Name name;
    name = random_emoji();
    valid_bite_names_.insert(name);
    return name;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Exception caught: " << e.what() << " on line " << __LINE__
              << std::endl;
    throw; // Re-throw the exception after logging it
  }
  catch (...)
  {
    std::cerr << "Unknown exception caught on line " << __LINE__ << std::endl;
    throw; // Re-throw the unknown exception after logging it
  }
}

BiteID MainProgram::random_bite_id()
{
  std::uniform_int_distribution<long long> distribution(
      0, std::numeric_limits<long long>::max());
  BiteID id = distribution(rand_engine_);
  return id;
}

BiteID MainProgram::n_to_BiteID(unsigned long n)
{
  if (n < bite_ids_.size())
  {
    return *std::next(bite_ids_.begin(), n);
  }
  BiteID id;
  for (id = random_bite_id(); valid_bite_ids_.find(id) != valid_bite_ids_.end();
       id = random_bite_id())
  {
  }
  valid_bite_ids_.insert(id);
  bite_ids_.push_back(id);
  return id;
}

ContourID MainProgram::n_to_contourid(unsigned long n)
{
  if (n < contour_ids_.size())
  {
    return *std::next(contour_ids_.begin(), n);
  }
  ContourID id;
  for (id = random_contour_id();
       valid_contour_ids_.find(id) != valid_contour_ids_.end();
       id = random_contour_id())
  {
  }
  valid_contour_ids_.insert(id);
  contour_ids_.push_back(id);
  return id;
}

Coord MainProgram::n_to_coord(unsigned long n)
{
  if (n < coords_.size())
  {
    return *std::next(coords_.begin(), n);
  }
  Coord xy;
  for (xy = random_coord(); valid_coords_.find(xy) != valid_coords_.end();
       xy = random_coord())
  {
  }
  valid_coords_.insert(xy);
  coords_.push_back(xy);
  return xy;
}

/**
 * =========================
 * === PRG2 N FUNCTIONS ====
 * =========================
 */

ConnectionID MainProgram::n_to_connectionid(unsigned long n)
{
  if (n < connection_ids_.size())
  {
    return *std::next(connection_ids_.begin(), n);
  }
  ConnectionID id;
  for (id = random_connection_id();
       valid_connection_ids_.find(id) != valid_connection_ids_.end();
       id = random_connection_id())
  {
  }
  valid_connection_ids_.insert(id);
  connection_ids_.push_back(id);
  return id;
}


void MainProgram::clear_test_cache()
{
  valid_bite_ids_.clear();
  valid_contour_ids_.clear();
  valid_connection_ids_.clear();
  valid_coords_.clear();
  bite_ids_.clear();
  contour_ids_.clear();
  coords_.clear();
  valid_bite_names_.clear();
}

ContourID MainProgram::random_root_contour()
{
  if (random_contours_added_ < 2)
  {
    return n_to_contourid(0);
  }
  unsigned long end = ROOT_BIAS_MULTIPLIER * random_contours_added_;
  if (end == 0)
  {
    return n_to_contourid(0);
  }
  return n_to_contourid(random<decltype(random_contours_added_)>(0, end));
}

ContourID MainProgram::random_leaf_contour()
{
  if (random_contours_added_ < 2)
  {
    return n_to_contourid(0);
  }
  unsigned long start = LEAF_BIAS_MULTIPLIER * random_contours_added_;
  if (start == random_contours_added_)
  {
    start = 0;
  }
  return n_to_contourid(
      random<decltype(random_contours_added_)>(start, random_contours_added_));
}

BiteID MainProgram::random_valid_bite()
{
  return n_to_BiteID(
      random<decltype(random_bites_added_)>(0, random_bites_added_));
}

ContourID MainProgram::random_valid_contour()
{
  return n_to_contourid(
      random<decltype(random_contours_added_)>(0, random_contours_added_));
}

Coord MainProgram::random_valid_coord()
{
  return *std::next(coords_.begin(),
                    random<decltype(coords_.size())>(0, coords_.size()));
}

ConnectionID MainProgram::random_valid_connection()
{
  return n_to_connectionid(
      random<decltype(connection_ids_.size())>(0, connection_ids_.size()));
}


void MainProgram::init_regexs()
{
  // Create regex <whitespace>(cmd1|cmd2|...)<whitespace>(.*)
  string cmds_regex_str = "[[:space:]]*(";
  bool first = true;
  for (auto &cmd : cmds_)
  {
    cmds_regex_str += (first ? "" : "|") + cmd.cmd;
    first = false;

    cmd.param_regex = regex(cmd.param_regex_str + "[[:space:]]*",
                            std::regex_constants::ECMAScript |
                                std::regex_constants::optimize);
  }
  cmds_regex_str += ")(?:[[:space:]]*$|" + wsx + "(.*))";
  cmds_regex_ = regex(cmds_regex_str, std::regex_constants::ECMAScript |
                                          std::regex_constants::optimize);
  coords_regex_ =
      regex(coordx + "[[:space:]]?",
            std::regex_constants::ECMAScript | std::regex_constants::optimize);
  nodetimes_regex_ =
      regex(BiteIDx + ":" + timex + "[[:space:]]?",
            std::regex_constants::ECMAScript | std::regex_constants::optimize);
  times_regex_ =
      regex(wsx + "([0-9][0-9]):([0-9][0-9]):([0-9][0-9])",
            std::regex_constants::ECMAScript | std::regex_constants::optimize);
  commands_divide_regex_ =
      regex("([0-9a-zA-Z_;\\*]+):?",
            std::regex_constants::ECMAScript | std::regex_constants::optimize);
  timeouts_regex_ = regex("([0-9]+):?", std::regex_constants::ECMAScript |
                                            std::regex_constants::optimize);
  commands_regex_ =
      regex("(?:([0-9]+)\\*)?([0-9a-zA-Z_]+);?",
            std::regex_constants::ECMAScript | std::regex_constants::optimize);
  sizes_regex_ = regex(numx + ";?", std::regex_constants::ECMAScript |
                                        std::regex_constants::optimize);
  range_regex_ = regex(numx + ":?", std::regex_constants::ECMAScript |
                                        std::regex_constants::optimize);
  is_range_notation_regex_ =
      regex("([0-9]+(?::[0-9]+){1,2})",
            std::regex_constants::ECMAScript | std::regex_constants::optimize);
}
