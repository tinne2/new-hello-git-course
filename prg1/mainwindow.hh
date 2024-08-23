// The main program (provided by the course), COMP.CS.300
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do
// so
//  (for debugging, for example), DO NOT commit any changes to git, but revert
//  all changes later. Otherwise you won't be able to get any updates/fixes to
//  this file from git!)
// The main program (provided by the course), COMP.CS.300
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do
// so
//  (for debugging, for example), DO NOT commit any changes to git, but revert
//  all changes later. Otherwise you won't be able to get any updates/fixes to
//  this file from git!)

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "datastructures.hh"
#include "mainprogram.hh"
#include <QElapsedTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView> // Include QGraphicsView here
#include <QMainWindow>
#include <QPixmap>
#include <QProcess>
#include <QScrollBar> // Include QScrollBar here
#include <QMouseEvent>

namespace Ui
{
  class MainWindow;
}

class CustomGraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  CustomGraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent)
  {
    setMouseTracking(true); // Enable mouse tracking
  }

signals:
  void mouseMoved(QMouseEvent *event);
  void mouseLeft();                      // Signal for mouse leave event
  void mouseClicked(QMouseEvent *event); // Signal for mouse click event

protected:
  void mouseMoveEvent(QMouseEvent *event) override
  {
    emit mouseMoved(event);
    QGraphicsView::mouseMoveEvent(
        event); // Ensure default behavior is preserved
  }

  void mousePressEvent(QMouseEvent *event) override
  {
    emit mouseClicked(event); // Emit mouse click signal
    QGraphicsView::mousePressEvent(
        event); // Ensure default behavior is preserved
  }

  void mouseReleaseEvent(QMouseEvent *event) override
  {
    QGraphicsView::mouseReleaseEvent(
        event); // Ensure default behavior is preserved
  }

  void leaveEvent(QEvent *event) override
  {
    emit mouseLeft();
    QGraphicsView::leaveEvent(event); // Ensure default behavior is preserved
  }
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  // Constants
  static constexpr double DEF_MAZE_MAX_HEIGHT = 9.0;
  static constexpr double DEF_MAZE_MIN_HEIGHT = -9.0;
  static unsigned int const DEF_CELL_SIZE = 20;
  static unsigned int const DEF_WALL_WIDTH = 5;
  static unsigned int const DEF_REMOVE_WALLS = 0;
  static bool const DEF_CREATE_MAZE = false;
  static unsigned int const DEF_SCENE_UPDATE = 20; // ms

  void create_labyrinth();
  void toggle_maze();

  virtual void update_view_maze();

  void always_update_view();

  explicit MainWindow(
      QWidget *parent = nullptr); // in maze solver:  explicit
                                  // MainWindow(QWidget *parent = 0);
  ~MainWindow();

  // Pass the errorset as a parameter
  void show_area();
  void show_bites(std::unordered_set<std::string> &errorset);
  void show_contours(std::unordered_set<std::string> &errorset);


  void update_coord_paths(
      std::unordered_map<Coord, std::unordered_map<std::string, bool>>
          &coord_paths,
      const Coord &current_coord, const Coord &next_coord,
      std::vector<Coord> &path_points, bool is_path_coord);

  void update_view();
  void output_text(std::ostringstream &output);
  void output_text_end();
  void update_path_distance(
      std::vector<std::pair<Coord, Distance>> &bite_path_distance);
  void clear_path_distance();

  bool check_stop_pressed() const;

public slots:
  void execute_line();
  void cmd_selected(int idx);
  void number_selected(const QString &number);
  void select_file();
  void clear_input_line();
  void fit_view();
  void scene_selection_change();
  void clear_selection();
  void onMouseMoved(QMouseEvent *event);
  void updateCoordinateText();
  void onMouseLeft();                      // Slot for mouse leave event
  void onMouseClicked(QMouseEvent *event); // Slot for mouse click event

private:
  void add_char_to_maze(unsigned int x, unsigned int y, QString emoji);
  void add_height_to_maze(unsigned int x, unsigned int y, double z);
  void add_walls_to_maze_cell(unsigned int x, unsigned int y, bool east_wall,
                              bool west_wall, bool north_wall, bool south_wall,
                              bool fill_maze, bool add_path_dot = false);
  void redraw_walls_for_maze();

  void load_emoji_font();
  QFont emojiFont_; // Member variable to store the emoji font

  Ui::MainWindow *ui;

  std::vector<std::vector<int>> contours_ = std::vector<std::vector<int>>(
      DEF_MAZE_WIDTH, std::vector<int>(DEF_MAZE_HEIGHT, 0));

  std::unordered_map<std::string, QString> bites_ =
      std::unordered_map<std::string, QString>();

  // Stores the bites on the path, and the distance between the start and end of
  // the path
  std::vector<std::pair<Coord, Distance>> bite_path_distance_;
  QGraphicsTextItem *coordTextItem = nullptr; // Coordinate text item

  QGraphicsScene *gscenep_;
  QGraphicsPixmapItem *labypixmapitemp_;
  QPixmap labypixmap_;
  QElapsedTimer scene_update_timer_;

  QGraphicsScene *gscene_ = nullptr;

  MainProgram mainprg_;

  bool stop_pressed_ = false;

  bool selection_clear_in_progress = false;
};

#endif // MAINWINDOW_HH

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "datastructures.hh"
#include "mainprogram.hh"
#include <QElapsedTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView> // Include QGraphicsView here
#include <QMainWindow>
#include <QPixmap>
#include <QProcess>
#include <QScrollBar> // Include QScrollBar here
#include <QMouseEvent>

namespace Ui
{
  class MainWindow;
}

class CustomGraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  CustomGraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent)
  {
    setMouseTracking(true); // Enable mouse tracking
  }

signals:
  void mouseMoved(QMouseEvent *event);
  void mouseLeft();                      // Signal for mouse leave event
  void mouseClicked(QMouseEvent *event); // Signal for mouse click event

protected:
  void mouseMoveEvent(QMouseEvent *event) override
  {
    emit mouseMoved(event);
    QGraphicsView::mouseMoveEvent(
        event); // Ensure default behavior is preserved
  }

  void mousePressEvent(QMouseEvent *event) override
  {
    emit mouseClicked(event); // Emit mouse click signal
    QGraphicsView::mousePressEvent(
        event); // Ensure default behavior is preserved
  }

  void mouseReleaseEvent(QMouseEvent *event) override
  {
    QGraphicsView::mouseReleaseEvent(
        event); // Ensure default behavior is preserved
  }

  void leaveEvent(QEvent *event) override
  {
    emit mouseLeft();
    QGraphicsView::leaveEvent(event); // Ensure default behavior is preserved
  }
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  // Constants
  static constexpr double DEF_MAZE_MAX_HEIGHT = 9.0;
  static constexpr double DEF_MAZE_MIN_HEIGHT = -9.0;
  static unsigned int const DEF_CELL_SIZE = 20;
  static unsigned int const DEF_WALL_WIDTH = 5;
  static unsigned int const DEF_REMOVE_WALLS = 0;
  static bool const DEF_CREATE_MAZE = false;
  static unsigned int const DEF_SCENE_UPDATE = 20; // ms

  void create_labyrinth();
  void toggle_maze();

  virtual void update_view_maze();

  void always_update_view();

  explicit MainWindow(
      QWidget *parent = nullptr); // in maze solver:  explicit
                                  // MainWindow(QWidget *parent = 0);
  ~MainWindow();

  // Pass the errorset as a parameter
  void show_area();
  void show_bites(std::unordered_set<std::string> &errorset);
  void show_contours(std::unordered_set<std::string> &errorset);
  void show_maze(std::unordered_set<std::string> &errorset);
  void update_coord_paths(
      std::unordered_map<Coord, std::unordered_map<std::string, bool>>
          &coord_paths,
      const Coord &current_coord, const Coord &next_coord,
      std::vector<Coord> &path_points, bool is_path_coord);

  void update_view();
  void output_text(std::ostringstream &output);
  void output_text_end();
  void update_path_distance(
      std::vector<std::pair<Coord, Distance>> &bite_path_distance);
  void clear_path_distance();

  bool check_stop_pressed() const;

public slots:
  void execute_line();
  void cmd_selected(int idx);
  void number_selected(const QString &number);
  void select_file();
  void clear_input_line();
  void fit_view();
  void scene_selection_change();
  void clear_selection();
  void onMouseMoved(QMouseEvent *event);
  void updateCoordinateText();
  void onMouseLeft();                      // Slot for mouse leave event
  void onMouseClicked(QMouseEvent *event); // Slot for mouse click event

private:
  void add_char_to_maze(unsigned int x, unsigned int y, QString emoji);
  void add_height_to_maze(unsigned int x, unsigned int y, double z);
  void add_walls_to_maze_cell(unsigned int x, unsigned int y, bool east_wall,
                              bool west_wall, bool north_wall, bool south_wall,
                              bool fill_maze, bool add_path_dot = false);
  void redraw_walls_for_maze();

  Ui::MainWindow *ui;

  std::vector<std::vector<int>> contours_ = std::vector<std::vector<int>>(
      DEF_MAZE_WIDTH, std::vector<int>(DEF_MAZE_HEIGHT, 0));

  std::unordered_map<std::string, QString> bites_ =
      std::unordered_map<std::string, QString>();

  // Stores the bites on the path, and the distance between the start and end of
  // the path
  std::vector<std::pair<Coord, Distance>> bite_path_distance_;
  QGraphicsTextItem *coordTextItem = nullptr; // Coordinate text item

  QGraphicsScene *gscenep_;
  QGraphicsPixmapItem *labypixmapitemp_;
  QPixmap labypixmap_;
  QElapsedTimer scene_update_timer_;

  QGraphicsScene *gscene_ = nullptr;

  MainProgram mainprg_;

  bool stop_pressed_ = false;

  bool selection_clear_in_progress = false;
};

#endif // MAINWINDOW_HH
