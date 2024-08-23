// The main program (provided by the course), COMP.CS.300
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do
// so
//  (for debugging, for example), DO NOT commit any changes to git, but revert
//  all changes later. Otherwise you won't be able to get any updates/fixes to
//  this file from git!)

#ifndef MAINPROGRAM_HH
#define MAINPROGRAM_HH

#ifdef QT_CORE_LIB
#ifndef NO_GRAPHICAL_GUI
#define GRAPHICAL_GUI
#endif
#endif

#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <codecvt>
#include <functional>
#include <iostream>
#include <locale>
#include <random>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "customtypes.hh"
#include "datastructures.hh"
#include "rect.hh"

class MainWindow; // In case there's UI

const Coord DEFAULT_MIN_COORD = {1, 1};
const Coord DEFAULT_MAX_COORD = {10000, 10000};
const int DEFAULT_MAX_HEIGHT = 100;
const int DEFAULT_MIN_HEIGHT = 1;
const double ROOT_BIAS_MULTIPLIER = 0.05;
const double LEAF_BIAS_MULTIPLIER = 0.5;

class MainProgram
{
public:
  MainProgram();

  class Stopwatch;

  enum class PromptStyle
  {
    NORMAL,
    NO_ECHO,
    NO_NESTING
  };
  enum class TestStatus
  {
    NOT_RUN,
    NO_DIFFS,
    DIFFS_FOUND
  };

  bool command_parse_line(std::string input, std::ostream &output);
  void command_parser(std::istream &input, std::ostream &output,
                      PromptStyle promptstyle);

  void setui(MainWindow *ui);

  void flush_output(std::ostream &output);
  bool check_stop() const;

  static int mainprogram(int argc, char *argv[]);

private:
  Datastructures ds_;
  MainWindow *ui_ = nullptr;

  static std::string const PROMPT;

  std::minstd_rand rand_engine_;

  static std::array<unsigned long int, 20> const primes1;
  static std::array<unsigned long int, 20> const primes2;
  unsigned long int prime1_ =
      0; // Will be initialized to random value from above
  unsigned long int prime2_ =
      0;                                     // Will be initialized to random value from above
  unsigned long int random_bites_added_ = 0; // Counter for random nodes added
  unsigned long int random_contours_added_ =
      0; // Counter for random contours added
  unsigned long int random_coords_added_ = 0;
  unsigned long int random_connections_added_ = 0;
  const unsigned int NAME_LENGTH = 8;
  const unsigned int BiteID_LENGTH = 20;
  const unsigned long long int MAX_CONTOURID =
      std::numeric_limits<unsigned long int>::max();

/**
 * ========================
 * == PRG2 CONSTANTS =====
 * ========================
 */

  const unsigned long int MAX_CONNECTIONID =
      std::numeric_limits<unsigned long int>::max();


  Coord min = DEFAULT_MIN_COORD;
  Coord max = DEFAULT_MAX_COORD;
  int min_height = DEFAULT_MIN_HEIGHT;
  int max_height = DEFAULT_MAX_HEIGHT;
  void init_primes();
  Name n_to_name(unsigned long int n);
  BiteID n_to_BiteID(unsigned long int n);
  ContourID n_to_contourid(unsigned long int n);
  Coord n_to_coord(unsigned long int n);

  /**
   * ========================
   * == PRG2 N FUNCTIONS ====
   * ========================
   */
  ConnectionID n_to_connectionid(unsigned long int n);

  std::unordered_set<BiteID> valid_bite_ids_;
  std::unordered_set<ContourID> valid_contour_ids_;
  std::unordered_multiset<Coord> valid_coords_;
  std::unordered_set<ConnectionID> valid_connection_ids_;
  std::vector<BiteID> bite_ids_;
  std::vector<ContourID> contour_ids_;
  std::vector<Coord> coords_;
  std::vector<ConnectionID> connection_ids_;

  std::unordered_multiset<Name> valid_bite_names_;
  void clear_test_cache();

  ContourID random_root_contour();
  ContourID random_leaf_contour();
  BiteID random_valid_bite();
  ContourID random_valid_contour();
  Coord random_valid_coord();
  ConnectionID random_valid_connection();

  enum class StopwatchMode
  {
    OFF,
    ON,
    NEXT
  };
  StopwatchMode stopwatch_mode = StopwatchMode::OFF;

  enum class ResultType
  {
    NOTHING,
    IDLIST,
    NAMELIST,
    HEIGHTLIST,
    COORDLIST,
    ROUTE,
    ID
  };
  using CmdResultID = std::pair<BiteID, ContourID>;
  using CmdResultIDs = std::tuple<std::vector<ContourID>, std::vector<BiteID>,
                                  std::vector<ConnectionID>>;
  using CmdResultNames = std::pair<std::pair<BiteID, ContourID>, Name>;
  using CmdResultHeights =
      std::pair<std::pair<BiteID, ContourID>, ContourHeight>;
  using CmdResultCoords =
      std::pair<std::pair<BiteID, ContourID>, std::vector<Coord>>;
  using CmdResultRoute = std::vector<std::tuple<Coord, Coord, Distance>>;
  using CmdResult =
      std::pair<ResultType,
                std::variant<CmdResultIDs, CmdResultNames, CmdResultHeights,
                             CmdResultCoords, CmdResultID, CmdResultRoute>>;
  CmdResult prev_result;
  bool view_dirty = true;

  TestStatus test_status_ = TestStatus::NOT_RUN;

  using MatchIter = std::smatch::const_iterator;
  struct CmdInfo
  {
    std::string cmd;
    std::string info;
    std::string param_regex_str;
    CmdResult (MainProgram::*func)(std::ostream &output, MatchIter begin,
                                   MatchIter end);
    bool (MainProgram::*testfunc)(Stopwatch &stopwatch);
    std::regex param_regex = {};
  };
  static std::vector<CmdInfo> cmds_;
  // Regex objects and their initialization
  std::regex cmds_regex_;
  std::regex coords_regex_;
  std::regex nodetimes_regex_;
  std::regex times_regex_;
  std::regex commands_divide_regex_;
  std::regex timeouts_regex_;
  std::regex commands_regex_;
  std::regex sizes_regex_;
  std::regex range_regex_;
  std::regex is_range_notation_regex_;
  void init_regexs();

  CmdResult cmd_get_bite_count(std::ostream &output, MatchIter begin,
                               MatchIter end);
  CmdResult cmd_clear_all(std::ostream &output, MatchIter begin, MatchIter end);
  CmdResult cmd_all_bites(std::ostream &output, MatchIter begin, MatchIter end);
  CmdResult cmd_add_bite(std::ostream &output, MatchIter begin, MatchIter end);

  CmdResult cmd_get_bite_name(std::ostream &output, MatchIter begin,
                              MatchIter end);
  CmdResult cmd_get_contour_name(std::ostream &output, MatchIter begin,
                                 MatchIter end);
  CmdResult cmd_get_bite_coord(std::ostream &output, MatchIter begin,
                               MatchIter end);
  CmdResult cmd_get_contour_coords(std::ostream &output, MatchIter begin,
                                   MatchIter end);

  CmdResult cmd_find_bite_with_coord(std::ostream &output, MatchIter begin,
                                     MatchIter end);
  CmdResult cmd_get_contour_height(std::ostream &output, MatchIter begin,
                                   MatchIter end);

  CmdResult cmd_add_contour(std::ostream &output, MatchIter begin,
                            MatchIter end);
  CmdResult cmd_all_contours(std::ostream &output, MatchIter begin,
                             MatchIter end);
  CmdResult cmd_add_subcontour_to_contour(std::ostream &output, MatchIter begin,
                                          MatchIter end);
  CmdResult cmd_add_bite_to_contour(std::ostream &output, MatchIter begin,
                                    MatchIter end);
  CmdResult cmd_get_bite_in_contours(std::ostream &output, MatchIter begin,
                                     MatchIter end);
  CmdResult cmd_all_subcontours_of_contour(std::ostream &output,
                                           MatchIter begin, MatchIter end);
  CmdResult cmd_get_bites_closest_to(std::ostream &output, MatchIter begin,
                                     MatchIter end);
  CmdResult cmd_remove_bite(std::ostream &output, MatchIter begin,
                            MatchIter end);
  CmdResult cmd_get_closest_common_ancestor_of_contours(std::ostream &output,
                                                        MatchIter begin,
                                                        MatchIter end);

  CmdResult help_command(std::ostream &output, MatchIter begin, MatchIter end);
  CmdResult cmd_randseed(std::ostream &output, MatchIter begin, MatchIter end);
  CmdResult cmd_random_add(std::ostream &output, MatchIter begin,
                             MatchIter end);
  CmdResult cmd_read(std::ostream &output, MatchIter begin, MatchIter end);
  CmdResult cmd_testread(std::ostream &output, MatchIter begin, MatchIter end);
  CmdResult cmd_stopwatch(std::ostream &output, MatchIter begin, MatchIter end);
  CmdResult cmd_perftest(std::ostream &output, MatchIter begin, MatchIter end);
  CmdResult cmd_comment(std::ostream &output, MatchIter begin, MatchIter end);

  bool test_all_bites(Stopwatch &stopwatch);
  void test_get_functions(BiteID id);
  bool test_find_bite_with_coord(Stopwatch &stopwatch);
  bool test_get_bite_in_contours(Stopwatch &stopwatch);
  bool test_all_subcontours_of_contour(Stopwatch &stopwatch);
  bool test_get_bites_closest_to(Stopwatch &stopwatch);
  bool test_remove_bite(Stopwatch &stopwatch);
  bool test_get_closest_common_ancestor_of_contours(Stopwatch &stopwatch);
  bool test_random_bites(Stopwatch &stopwatch);
  bool test_get_contour_height(Stopwatch &stopwatch);
  bool test_get_bite_count(Stopwatch &stopwatch);
  bool test_clear_all(Stopwatch &stopwatch);
  bool test_add_bite(Stopwatch &stopwatch);
  bool test_get_bite_name(Stopwatch &stopwatch);
  bool test_get_bite_coord(Stopwatch &stopwatch);
  bool test_add_contour(Stopwatch &stopwatch);
  bool test_add_n_contours(Stopwatch &stopwatch, int x1, int y1, int x2, int y2);
  bool test_all_contours(Stopwatch &stopwatch);
  bool test_get_contour_name(Stopwatch &stopwatch);
  bool test_get_contour_coords(Stopwatch &stopwatch);
  bool test_add_subcontour_to_contour(Stopwatch &stopwatch);
  bool test_add_coord_to_contour(Stopwatch &stopwatch);

  bool test_get_bites_alphabetically(Stopwatch &stopwatch);
  bool test_get_bites_distance_increasing(Stopwatch &stopwatch);
  BiteID random_bite_id();
  Name random_name();
  std::string random_emoji();
  Coord random_coord();
  int random_contour_height();
  ContourID random_contour_id();
  ConnectionID random_connection_id();
  bool init_perftest(Stopwatch &stopwatch, std::string testcmd, int n);
  int add_random_bites_and_contours(Stopwatch &stopwatch, int nmb_of_hills_and_valleys, int x1, int y1, int x2, int y2);

  int add_random_bites_to_given_contours(Stopwatch &stopwatch, int nmb_of_bites, const std::vector<int> contour_ids);
  int add_random_bites(Stopwatch &stopwatch, int nmb_of_bites);
  std::pair<std::vector<int>, bool>  add_random_contours(Stopwatch &stopwatch, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int max_levels=MAX_CONTOUR_HEIGHT);
  std::pair<std::vector<int>, bool>  add_random_contours2(Stopwatch &stopwatch, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int max_levels=MAX_CONTOUR_HEIGHT);
  int add_bite_with_info(Stopwatch &stopwatch, int bite_counter, int nmb_of_bites, int contour_id, Coord &bite_pos, Datastructures &ds_);


  bool add_random_coord_connections(Stopwatch &stopwatch,
                                    unsigned int size, int x1, int y1, int x2, int y2);
  Distance calc_distance(Coord c1, Coord c2);
  std::string print_bite(BiteID id, std::ostream &output, bool nl = true);
  std::string print_bite_brief(BiteID id, std::ostream &output, bool nl = true);
  std::string print_coord_brief(Coord coord, std::ostream &output,
                                bool nl = true);
  std::string print_contour(ContourID id, std::ostream &output, bool nl = true);
  std::string print_connection(ConnectionID id, std::ostream &output,
                               bool nl = true);
  std::string print_bite_name(BiteID id, std::ostream &output, bool nl = true);
  std::string print_coord(Coord coord, std::ostream &output, bool nl = true);
  template <typename Type>
  Type random(Type start, Type end);
  template <typename To>
  static To convert_string_to(std::string from);
  template <typename From>
  static std::string convert_to_string(From from);

  template <BiteID (Datastructures::*MFUNC)()>
  CmdResult NoParNodeCmd(std::ostream &output, MatchIter begin, MatchIter end);

  template <std::vector<BiteID> (Datastructures::*MFUNC)()>
  CmdResult NoParListCmd(std::ostream &output, MatchIter begin, MatchIter end);

  template <BiteID (Datastructures::*MFUNC)()>
  void NoParNodeTestCmd();

  template <std::vector<BiteID> (Datastructures::*MFUNC)()>
  void NoParListTestCmd();

  /**
   * ========================
   * = PRG2 FUNCTIONS BELOW =
   * ========================
   */

  CmdResult cmd_add_connection(std::ostream &output, MatchIter begin,
                               MatchIter end);
  CmdResult cmd_clear_connections(std::ostream &output, MatchIter begin,
                                  MatchIter end);
  CmdResult cmd_next_bites_from(std::ostream &output, MatchIter begin,
                                MatchIter end);
  CmdResult cmd_get_connections(std::ostream &output, MatchIter begin,
                                MatchIter end);
  CmdResult cmd_get_connection_coords(std::ostream &output, MatchIter begin,
                                      MatchIter end);
  CmdResult cmd_path_any(std::ostream &output, MatchIter begin, MatchIter end);

  CmdResult cmd_path_shortest(std::ostream &output, MatchIter begin,
                              MatchIter end);

  CmdResult cmd_path_least_bites(std::ostream &output, MatchIter begin,
                                 MatchIter end);

  CmdResult cmd_path_least_uphill(std::ostream &output, MatchIter begin,
                                  MatchIter end);

  bool test_add_connection(Stopwatch &stopwatch);
  bool test_clear_connections(Stopwatch &stopwatch);
  bool test_next_bites_from(Stopwatch &stopwatch);
  bool test_get_connections(Stopwatch &stopwatch);
  bool test_get_connection_coords(Stopwatch &stopwatch);
  bool test_path_any(Stopwatch &stopwatch);
  bool test_path_shortest(Stopwatch &stopwatch);
  bool test_path_least_bites(Stopwatch &stopwatch);
  bool test_path_least_uphill(Stopwatch &stopwatch);

  friend class MainWindow;
};

template <typename Type>
Type MainProgram::random(Type start, Type end)
{
  auto range = end - start;
  assert(range != 0 && "random() with zero range!");

  auto num = std::uniform_int_distribution<unsigned long int>(0, range - 1)(
      rand_engine_);

  return static_cast<Type>(start + num);
}

template <typename To>
To MainProgram::convert_string_to(std::string from)
{
  std::istringstream istr(from);
  To result;
  istr >> std::noskipws >> result;
  if (istr.fail() || !istr.eof())
  {
    throw std::invalid_argument("Cannot convert string to required type");
  }
  return result;
}

template <typename From>
std::string MainProgram::convert_to_string(From from)
{
  std::ostringstream ostr;
  ostr << from;
  if (ostr.fail())
  {
    throw std::invalid_argument("Cannot convert type to string");
  }
  return ostr.str();
}

template <BiteID (Datastructures::*MFUNC)()>
MainProgram::CmdResult MainProgram::NoParNodeCmd(std::ostream & /*output*/,
                                                 MatchIter /*begin*/,
                                                 MatchIter /*end*/)
{
  auto result = (ds_.*MFUNC)();
  return {ResultType::IDLIST, CmdResultIDs{{}, {result}, {}}};
}

template <std::vector<BiteID> (Datastructures::*MFUNC)()>
MainProgram::CmdResult MainProgram::NoParListCmd(std::ostream & /*output*/,
                                                 MatchIter /*begin*/,
                                                 MatchIter /*end*/)
{
  auto result = (ds_.*MFUNC)();
  CmdResultIDs cmdResultIds =
      std::make_tuple(std::vector<ContourID>{}, std::vector<BiteID>(result),
                      std::vector<ConnectionID>{});

  return {ResultType::IDLIST, cmdResultIds};
}

template <BiteID (Datastructures::*MFUNC)()>
void MainProgram::NoParNodeTestCmd()
{
  (ds_.*MFUNC)();
}

template <std::vector<BiteID> (Datastructures::*MFUNC)()>
void MainProgram::NoParListTestCmd()
{
  (ds_.*MFUNC)();
}

#ifdef USE_PERF_EVENT
extern "C"
{
#include <unistd.h>
#ifdef _WIN32
// Windows-specific headers and code
#else
// Unix-specific headers and code
#endif
#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>

  // #include <sys/ioctl.h>

  static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                              int cpu, int group_fd, unsigned long flags)
  {
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    return ret;
  }
}
#endif

class MainProgram::Stopwatch
{
public:
  using Clock = std::chrono::high_resolution_clock;

  Stopwatch(bool use_counter = false) : use_counter_(use_counter)
  {
#ifdef USE_PERF_EVENT
    if (use_counter_)
    {
      memset(&pe_, 0, sizeof(pe_));
      pe_.type = PERF_TYPE_HARDWARE;
      pe_.size = sizeof(pe_);
      pe_.config = PERF_COUNT_HW_INSTRUCTIONS;
      pe_.disabled = 1;
      pe_.exclude_kernel = 1;
      pe_.exclude_hv = 1;

      fd_ = perf_event_open(&pe_, 0, -1, -1, 0);
      if (fd_ == -1)
      {
        throw "Couldn't open perf events!";
      }
    }
#endif
    reset();
  }

  ~Stopwatch()
  {
#ifdef USE_PERF_EVENT
    if (use_counter_)
    {
      close(fd_);
    }
#endif
  }

  void start()
  {
    running_ = true;
    starttime_ = Clock::now();
#ifdef USE_PERF_EVENT
    if (use_counter_)
    {
      ioctl(fd_, PERF_EVENT_IOC_RESET, 0);
      read(fd_, &startcount_, sizeof(startcount_));
      ioctl(fd_, PERF_EVENT_IOC_ENABLE, 0);
    }
#endif
  }

  void stop()
  {
    running_ = false;
#ifdef USE_PERF_EVENT
    if (use_counter_)
    {
      ioctl(fd_, PERF_EVENT_IOC_DISABLE, 0);
      long long c;
      read(fd_, &c, sizeof(c));
      counter_ += (c - startcount_);
    }
#endif
    elapsed_ += (Clock::now() - starttime_);
  }

  void reset()
  {
    running_ = false;
#ifdef USE_PERF_EVENT
    if (use_counter_)
    {
      ioctl(fd_, PERF_EVENT_IOC_DISABLE, 0);
      ioctl(fd_, PERF_EVENT_IOC_RESET, 0);
      counter_ = 0;
    }
#endif
    elapsed_ = elapsed_.zero();
  }

  double elapsed()
  {
    if (!running_)
    {
      return static_cast<std::chrono::duration<double>>(elapsed_).count();
    }
    else
    {
      auto total = elapsed_ + (Clock::now() - starttime_);
      return static_cast<std::chrono::duration<double>>(total).count();
    }
  }

#ifdef USE_PERF_EVENT
  long long count()
  {
    if (use_counter_)
    {
      if (!running_)
      {
        return counter_;
      }
      else
      {
        long long c;
        read(fd_, &c, sizeof(c));
        return counter_ + (c - startcount_);
      }
    }
    else
    {
      assert(!"perf_event not enabled during StopWatch creation!");
    }
  }
#endif

private:
  std::chrono::time_point<Clock> starttime_;
  Clock::duration elapsed_ = Clock::duration::zero();
  bool running_ = false;

  bool use_counter_;
#ifdef USE_PERF_EVENT
  struct perf_event_attr pe_;
  long long startcount_ = 0;
  long long counter_ = 0;
  int fd_ = 0;
#endif
};

#endif // MAINPROGRAM_HH
