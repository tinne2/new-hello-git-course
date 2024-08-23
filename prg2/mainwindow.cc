// The main program (provided by the course), COMP.CS.300
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do
// so
//  (for debugging, for example), DO NOT commit any changes to git, but revert
//  all changes later. Otherwise you won't be able to get any updates/fixes to
//  this file from git!)

#include "mainprogram.hh"
// Qt generated main window code
#ifdef GRAPHICAL_GUI
#include <QBrush>
#include <QColor>
#include <QCoreApplication>
#include <QGraphicsTextItem>

#include <QDir>
#include <QFileDialog>
#include <QFont>
#include <QGraphicsItem>
#include <QPen>
#include <QVariant>

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

#include <QDir>
#include <QProcess>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPainter>
#include <QString>
#include <QTimer>
#include <QFontDatabase>

// Needed to be able to store BiteID in QVariant (in QGraphicsItem)
Q_DECLARE_METATYPE(BiteID)
// The same for Coords (currently a pair of ints)
Q_DECLARE_METATYPE(Coord)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  mainprg_.setui(this);
  load_emoji_font(); // Load the emoji font during initialization

  CustomGraphicsView *customView =
      qobject_cast<CustomGraphicsView *>(ui->graphics_view_maze);
  if (customView)
  {
    customView->setMouseTracking(true); // Enable mouse tracking for the view

    connect(customView, &CustomGraphicsView::mouseMoved, this,
            &MainWindow::onMouseMoved);
    connect(customView, &CustomGraphicsView::mouseLeft, this,
            &MainWindow::onMouseLeft); // Connect mouseLeft signal
    connect(customView, &CustomGraphicsView::mouseClicked, this,
            &MainWindow::onMouseClicked); // Connect mouseClicked signal
  }
  // Ensure the coordinate text item is updated when the view is scrolled
  connect(ui->graphics_view_maze->horizontalScrollBar(),
          &QScrollBar::valueChanged, this, &MainWindow::updateCoordinateText);
  connect(ui->graphics_view_maze->verticalScrollBar(),
          &QScrollBar::valueChanged, this, &MainWindow::updateCoordinateText);

  setMouseTracking(true);                  // Enable mouse tracking
  centralWidget()->setMouseTracking(true); // If you have a central widget

  connect(ui->maze_toggle, &QCheckBox::toggled, this, &MainWindow::toggle_maze);

  // Disable most buttons at start

  ui->maze_toggle->setChecked(DEF_CREATE_MAZE);

  // Initialize algorithm selection

  // Initialize graphics scene & view
  gscenep_ = new QGraphicsScene(this);
  ui->graphics_view_maze->setScene(gscenep_);

  // Initialize pixmap in view
  labypixmapitemp_ = gscenep_->addPixmap(labypixmap_);

  // // Start scene update timer
  scene_update_timer_.start();

  // Execute line
  connect(ui->execute_button, &QPushButton::pressed, this,
          &MainWindow::execute_line);

  // Line edit
  connect(ui->lineEdit, &QLineEdit::returnPressed, this,
          &MainWindow::execute_line);

  // File selection
  connect(ui->file_button, &QPushButton::pressed, this,
          &MainWindow::select_file);

  std::sort(mainprg_.cmds_.begin(), mainprg_.cmds_.end(),
            [](auto const &l, auto const &r)
            { return l.cmd < r.cmd; });
  for (auto &cmd : mainprg_.cmds_)
  {
    ui->cmd_select->addItem(QString::fromStdString(cmd.cmd));
  }
  connect(ui->cmd_select,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this,
          &MainWindow::cmd_selected);

  // Number selection
  for (auto i = 0; i <= 20; ++i)
  {
    ui->number_select->addItem(QString("%1").arg(i));
  }
  connect(ui->number_select,
          static_cast<void (QComboBox::*)(QString const &)>(
              &QComboBox::currentTextChanged),
          this, &MainWindow::number_selected);

  // Output box
  QFont monofont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  monofont.setLetterSpacing(QFont::AbsoluteSpacing, 0); // Adjust letter spacing for the monospaced font

  monofont.setStyleHint(QFont::TypeWriter);
  ui->output->setFont(monofont);

  QFont emojiFont = emojiFont_;
  emojiFont.setPointSize(10); // Ensure the point size matches the mono font
  ui->output->setFont(emojiFont_);

  // Initialize graphics scene & view
  gscene_ = new QGraphicsScene(this);

  // Selecting graphics items by mouse
  connect(gscene_, &QGraphicsScene::selectionChanged, this,
          &MainWindow::scene_selection_change);

  // Zoom slider changes graphics view scale
  connect(ui->zoom_plus, &QToolButton::clicked,
          [this]
          { this->ui->graphics_view_maze->scale(1.1, 1.1); });
  connect(ui->zoom_minus, &QToolButton::clicked,
          [this]
          { this->ui->graphics_view_maze->scale(1 / 1.1, 1 / 1.1); });
  connect(ui->zoom_1, &QToolButton::clicked,
          [this]
          { this->ui->graphics_view_maze->resetTransform(); });
  connect(ui->zoom_fit, &QToolButton::clicked, this, &MainWindow::fit_view);

  // Changing checkboxes updates view
  connect(ui->bites_checkbox, &QCheckBox::clicked, this,
          &MainWindow::update_view);
  connect(ui->contours_checkbox, &QCheckBox::clicked, this,
          &MainWindow::update_view);

  // Clear input button
  connect(ui->clear_input_button, &QPushButton::clicked, this,
          &MainWindow::clear_input_line);

  // Stop button
  connect(ui->stop_button, &QPushButton::clicked,
          [this]()
          { this->stop_pressed_ = true; });
  // QTimer::singleShot(0, this, &MainWindow::create_labyrinth);

  clear_input_line();
  show_area();
  update_view_maze();
  QTimer::singleShot(0, this, &MainWindow::fit_view);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::create_labyrinth() {}

void MainWindow::toggle_maze()
{
  update_view();
}

void MainWindow::load_emoji_font()
{
#ifdef Q_OS_WIN
  emojiFont_ = QFont("Segoe UI Emoji");
#elif defined(Q_OS_LINUX)
  int fontId = QFontDatabase::addApplicationFont(":/NotoColorEmoji.ttf");
  if (fontId != -1)
  {
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (!fontFamilies.isEmpty())
    {
      emojiFont_ = QFont(fontFamilies.at(0));
      emojiFont_.setPixelSize(DEF_CELL_SIZE - 2);
    }
  }
#else
  emojiFont_ = QFont("Arial"); // Fallback for other OSes
#endif
  emojiFont_.setLetterSpacing(QFont::AbsoluteSpacing, 0); // Adjust letter spacing
}
void MainWindow::add_char_to_maze(unsigned int x, unsigned int y,
                                  QString emoji)
{
  QPainter painter(&labypixmap_);

  auto skip = DEF_CELL_SIZE + DEF_WALL_WIDTH;

  if (emoji != " ")
  {
    painter.setPen(QPen(Qt::blue));

    painter.setFont(emojiFont_);

    QString text(emoji);
    painter.drawText(skip * x + DEF_WALL_WIDTH - DEF_WALL_WIDTH / 2,
                     skip * y + DEF_WALL_WIDTH - DEF_WALL_WIDTH / 2, skip, skip,
                     Qt::AlignCenter, text);
  }
}

void MainWindow::add_height_to_maze(unsigned int x, unsigned int y, double z)
{
  QPainter painter(&labypixmap_);

  auto skip = DEF_CELL_SIZE + DEF_WALL_WIDTH;

  // Define colors for the minimum, middle, and maximum heights
  static const QColor DEF_MAZE_MIN_HEIGHTColor(
      10, 10, 20, 255); // Minimum height color (darkest blue)
  static const QColor zeroHeightColor(194, 178, 128,
                                      255); // 0 height color (ecru, sand)
  static const QColor DEF_MAZE_MAX_HEIGHTColor(248, 255, 246,
                                               255); // Maximum height color (mint cream)
  static const QColor maxNegativeColor(84, 166, 255,
                                       255); // -1 height color (lightest blue)
  static const QColor minPositiveColor(31, 79, 15,
                                       255); // 1 height color (darkest green)

  // Ensure z is within the desired range
  if (z < DEF_MAZE_MIN_HEIGHT)
    z = DEF_MAZE_MIN_HEIGHT;
  if (z > DEF_MAZE_MAX_HEIGHT)
    z = DEF_MAZE_MAX_HEIGHT;

  QColor color;
  if (z < 0)
  {
    // Interpolate between DEF_MAZE_MIN_HEIGHTColor and zeroHeightColor for negative
    // heights
    double t = (z - DEF_MAZE_MIN_HEIGHT) / (0 - DEF_MAZE_MIN_HEIGHT);
    color = QColor(
        static_cast<int>(DEF_MAZE_MIN_HEIGHTColor.red() +
                         t * (maxNegativeColor.red() - DEF_MAZE_MIN_HEIGHTColor.red())),
        static_cast<int>(
            DEF_MAZE_MIN_HEIGHTColor.green() +
            t * (maxNegativeColor.green() - DEF_MAZE_MIN_HEIGHTColor.green())),
        static_cast<int>(DEF_MAZE_MIN_HEIGHTColor.blue() +
                         t * (maxNegativeColor.blue() - DEF_MAZE_MIN_HEIGHTColor.blue())),
        255);
  }
  else if (z > 0)
  {
    // Interpolate between zeroHeightColor and DEF_MAZE_MAX_HEIGHTColor for positive
    // heights
    double t = z / DEF_MAZE_MAX_HEIGHT;
    color = QColor(
        static_cast<int>(minPositiveColor.red() +
                         t * (DEF_MAZE_MAX_HEIGHTColor.red() - minPositiveColor.red())),
        static_cast<int>(
            minPositiveColor.green() +
            t * (DEF_MAZE_MAX_HEIGHTColor.green() - minPositiveColor.green())),
        static_cast<int>(minPositiveColor.blue() +
                         t * (DEF_MAZE_MAX_HEIGHTColor.blue() - minPositiveColor.blue())),
        255);
  }
  else
  {
    // z == 0
    color = zeroHeightColor;
  }

  painter.setPen(QPen(Qt::NoPen));
  painter.setBrush(QBrush(color));
  painter.drawRect(skip * x, skip * y, skip + DEF_WALL_WIDTH, skip + DEF_WALL_WIDTH);
}

void MainWindow::redraw_walls_for_maze()
{
  auto width = DEF_MAZE_WIDTH;
  auto height = DEF_MAZE_HEIGHT;
  for (unsigned int x = 0; x < width; ++x)
  {
    for (unsigned int y = 0; y < height; ++y)
    {

      bool west_wall = (x == 0);
      bool east_wall = (x == width - 1);
      bool north_wall = (y == 0);
      bool south_wall = (y == height - 1);
      add_walls_to_maze_cell(x, y, east_wall, west_wall, north_wall, south_wall,
                             false);
    }
  }
}

void MainWindow::onMouseMoved(QMouseEvent *event)
{
  QPointF point = ui->graphics_view_maze->mapToScene(event->pos());
  auto x = point.x();
  auto y = point.y();

  auto skip = DEF_CELL_SIZE + DEF_WALL_WIDTH;
  int cell_x = static_cast<int>(x) / skip;
  int cell_y = static_cast<int>(y) / skip;

  if (cell_x >= 0 && cell_x < static_cast<int>(contours_.size()) &&
      cell_y >= 0 && cell_y < static_cast<int>(contours_[0].size()))
  {

    if (!coordTextItem)
    {
      coordTextItem = new QGraphicsTextItem();
      gscenep_->addItem(coordTextItem);
    }

    coordTextItem->setPlainText(QString("(%1, %2)").arg(cell_x).arg(cell_y));
    coordTextItem->setPos(point.x() + 10, point.y() + 10);
    coordTextItem->setDefaultTextColor(Qt::red);

    // Retrieve the current scale factor of the view
    qreal scale =
        ui->graphics_view_maze->transform().m11(); // Assuming uniform scaling

    // Set the font size based on the scale factor
    QFont font = coordTextItem->font();
    font.setPointSize(
        static_cast<int>(10 / scale)); // Adjust the base size (50) as needed
    coordTextItem->setFont(font);
  }
}
void MainWindow::updateCoordinateText()
{
  QPoint cursorPos = ui->graphics_view_maze->mapFromGlobal(QCursor::pos());
  QMouseEvent event(
      QEvent::MouseMove,
      cursorPos,
      cursorPos, // Add global position for compatibility
      Qt::NoButton,
      Qt::NoButton,
      Qt::NoModifier);
  onMouseMoved(&event);
}
void MainWindow::onMouseClicked(QMouseEvent *event)
{
  try
  {
    QPointF point = ui->graphics_view_maze->mapToScene(event->pos());
    int cell_x = static_cast<int>(point.x()) / (DEF_CELL_SIZE + DEF_WALL_WIDTH);
    int cell_y = static_cast<int>(point.y()) / (DEF_CELL_SIZE + DEF_WALL_WIDTH);

    if (cell_x >= 0 && cell_x < static_cast<int>(contours_.size()) &&
        cell_y >= 0 && cell_y < static_cast<int>(contours_[0].size()))
    {

      Coord coord{cell_x, cell_y};
      BiteID bite = mainprg_.ds_.find_bite_with_coord(coord);

      if (bite != NO_BITE)
      {
        // Get the name
        Name name = mainprg_.ds_.get_bite_name(bite);
        // also show details
        ui->output->appendPlainText(QString("Clicked  %1 (id: %2) at (%3, %4).")
                                        .arg(QString::fromStdString(name))
                                        .arg(bite)
                                        .arg(cell_x)
                                        .arg(cell_y));
      }
    }
  }
  catch (NotImplemented const &e)
  {

    std::cerr << std::endl
              << "NotImplemented while updating graphics: " << e.what()
              << std::endl;
    ui->output->appendPlainText(
        QString("NotImplemented while updating graphics: %1").arg(e.what()));
  }
}

void MainWindow::onMouseLeft()
{
  if (coordTextItem)
  {
    gscenep_->removeItem(coordTextItem);
    delete coordTextItem;
    coordTextItem = nullptr;
  }
}

void MainWindow::add_walls_to_maze_cell(unsigned int x, unsigned int y,
                                        bool east_wall, bool west_wall,
                                        bool north_wall, bool south_wall,
                                        bool fill_maze, bool add_path_dot)
{
  QPainter painter(&labypixmap_);
  auto skip = DEF_CELL_SIZE + DEF_WALL_WIDTH;

  if (fill_maze)
  {
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(QColor(Qt::black)));
    painter.drawRect(skip * x, skip * y, skip + DEF_WALL_WIDTH,
                     skip + DEF_WALL_WIDTH);

    // Uncomment to Apply the cross-hatch pattern
    // QBrush crossHatchBrush(Qt::white, Qt::BDiagPattern);
    // painter.setBrush(crossHatchBrush);
    // painter.drawRect(skip * x, skip * y, skip + DEF_WALL_WIDTH,
    //  skip + DEF_WALL_WIDTH);
  }
  else
  {
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(QColor(Qt::black)));

    if (west_wall)
    {
      painter.drawRect(skip * x, skip * y, DEF_WALL_WIDTH, skip + DEF_WALL_WIDTH);
    }
    if (east_wall)
    {
      painter.drawRect(skip * (x + 1), skip * y, DEF_WALL_WIDTH,
                       skip + DEF_WALL_WIDTH);
    }
    if (north_wall)
    {
      painter.drawRect(skip * x, skip * y, skip + DEF_WALL_WIDTH, DEF_WALL_WIDTH);
    }
    if (south_wall)
    {
      painter.drawRect(skip * x, skip * (y + 1), skip + DEF_WALL_WIDTH,
                       DEF_WALL_WIDTH);
    }
  }

  if (add_path_dot)
  {
    painter.setBrush(QBrush(QColor(Qt::red)));
    painter.drawEllipse(skip * x + DEF_WALL_WIDTH + skip / 2 - 2,
                        skip * y + DEF_WALL_WIDTH + skip / 2 - 2, 4, 4);
  }
}

void MainWindow::update_view_maze()
{
  if (scene_update_timer_.hasExpired(DEF_SCENE_UPDATE))
  {
    always_update_view();
  }
}

void MainWindow::always_update_view()
{
  labypixmapitemp_->setPixmap(labypixmap_);
  scene_update_timer_.restart();
}

void MainWindow::show_bites(std::unordered_set<std::string> &errorset)
{
  auto bites = mainprg_.ds_.all_bites();

  if (bites.size() == 1 && bites.front() == NO_BITE)
  {
    errorset.insert("all_bites() returned error {NO_BITE}");
    bites.clear();
  }

  try
  {
    for (auto BiteID : bites)
    {
      // show the id of the bite in output
      auto name = mainprg_.ds_.get_bite_name(BiteID);
      if (name == NO_NAME)
      {
        errorset.insert("get_bite_name() returned error {NO_NAME}");
      }
      auto xy = mainprg_.ds_.get_bite_coord(BiteID);
      auto [x, y] = xy;
      if (x == NO_VALUE || y == NO_VALUE)
      {
        errorset.insert("get_bite_coord() returned error NO_COORD/NO_VALUE");
        x = 0;
        y = 0;
      }

      if (!name.empty())
      {
        auto key =
            "(x: " + std::to_string(x) + ", y: " + std::to_string(y) + ")";
        bites_[key] = QString::fromStdString(name);
        auto name_qstring = QString::fromStdString(name);

        add_char_to_maze(x, y, name_qstring);
      }
    }
  }
  catch (NotImplemented const &e)
  {
    errorset.insert(std::string("NotImplemented while updating graphics: ") +
                    e.what());
    std::cerr << std::endl
              << "NotImplemented while updating graphics: " << e.what()
              << std::endl;
  }
}

void MainWindow::show_contours(std::unordered_set<std::string> &errorset)
{

  auto contourids = mainprg_.ds_.all_contours();

  if (contourids.size() == 1 && contourids.front() == NO_CONTOUR)
  {
    errorset.insert("all_contours() returned error {NO_CONTOUR}");
  }

  std::unordered_map<Coord, int> coord_contour_map;
  for (auto contourid : contourids)
  {
    if (contourid != NO_CONTOUR)
    {
      auto coords = mainprg_.ds_.get_contour_coords(contourid);
      auto height = mainprg_.ds_.get_contour_height(contourid);
      if (coords.size() < 3)

        if (std::find(coords.begin(), coords.end(), NO_COORD) !=
            coords.end())
        {
          errorset.insert("get_contour_coords() returned error NO_COORD");
        }
      if (height == NO_CONTOUR_HEIGHT)
      {
        errorset.insert("get_contour_height() returned error NO_VALUE");
        height = 1;
      }
      for (auto coord : coords)
      {
        if (coord.x < static_cast<int>(DEF_MAZE_WIDTH) && coord.y < static_cast<int>(DEF_MAZE_HEIGHT))
        {
          contours_[coord.x][coord.y] = height;
          add_height_to_maze(coord.x, coord.y, height);
          bool west_wall = (coord.x == 0);
          bool east_wall = (coord.x == DEF_MAZE_WIDTH - 1);
          bool north_wall = (coord.y == 0);
          bool south_wall = (coord.y == DEF_MAZE_HEIGHT - 1);
          add_walls_to_maze_cell(coord.x, coord.y, east_wall, west_wall,
                                 north_wall, south_wall, false, false);
        }
      }
    }
  }
}

void MainWindow::show_area()
{
  auto width = DEF_MAZE_WIDTH;
  auto height = DEF_MAZE_HEIGHT;

  auto skip = DEF_CELL_SIZE + DEF_WALL_WIDTH;

  labypixmap_ =
      QPixmap(skip * width + DEF_WALL_WIDTH, skip * height + DEF_WALL_WIDTH);
  labypixmap_.fill();

  ui->graphics_view_maze->setSceneRect(gscenep_->itemsBoundingRect());

  // initialize a vector of the maze cells, where only the height of each cell
  // is. Example: maze[0][0] = 1. We will populate this vector with the
  // heights of the contours in the coming for loops Use width and height to
  // generate a plain field with no contours, walls or bites
  //  initially, height of each cell is 1
  for (unsigned int x = 0; x < width; ++x)
  {
    for (unsigned int y = 0; y < height; ++y)
    {
      contours_[x][y] = 0;
      add_height_to_maze(x, y, 0);
      // if its one of the corner or edge cells, add a wall to the edg
      bool west_wall = (x == 0);
      bool east_wall = (x == width - 1);
      bool north_wall = (y == 0);
      bool south_wall = (y == height - 1);
      add_walls_to_maze_cell(x, y, east_wall, west_wall, north_wall, south_wall,
                             false, false);
    }
  }
  labypixmapitemp_->setPixmap(labypixmap_);
}

void MainWindow::show_maze(std::unordered_set<std::string> &errorset)
{
  std::unordered_set<Coord> all_path_coords;

  auto coord_paths =
      std::unordered_map<Coord, std::unordered_map<std::string, bool>>();

  if (!bite_path_distance_.empty())
  {
    unsigned long int distance = 0;
    // There's always atleast 2 bites, and the next bite is always the next
    // bite in the path
    for (std::vector<std::pair<Coord, long unsigned int>>::size_type i = 0;
         i < bite_path_distance_.size() - 1; i++)
    {

      Coord currentCoord = bite_path_distance_[i].first;
      Coord nextCoordCorner = bite_path_distance_[i + 1].first;

      if (currentCoord == nextCoordCorner)
      {
        continue;
      }

      if (currentCoord.x == nextCoordCorner.x)
      {
        int y_step = (nextCoordCorner.y > currentCoord.y) ? 1 : -1;
        for (int y = currentCoord.y; y != nextCoordCorner.y + y_step;
             y += y_step)
        {
          Coord current_coord = Coord(currentCoord.x, y);
          all_path_coords.insert(current_coord);
        }
      }
      else
      {
        int x_step = (nextCoordCorner.x > currentCoord.x) ? 1 : -1;
        for (int x = currentCoord.x; x != nextCoordCorner.x + x_step;
             x += x_step)
        {

          Coord current_coord = Coord(x, currentCoord.y);
          all_path_coords.insert(current_coord);
        }
      }
    }
    distance = bite_path_distance_.back().second;
    ui->pathlength->display(QString::number(distance));
  }

  auto bites = mainprg_.ds_.all_bites();

  if (bites.size() == 1 && bites.front() == NO_BITE)
  {
    errorset.insert("show_maze: all_bites() returned error {NO_BITE}. ");
    return;
  }

  // Iterate through all bites
  for (auto BiteID : bites)
  {
    auto start_xy = mainprg_.ds_.get_bite_coord(BiteID);
    if (coord_paths.find(start_xy) == coord_paths.end())
    {
      coord_paths[start_xy] = {
          {"east_wall", true},
          {"west_wall", true},
          {"north_wall", true},
          {"south_wall", true},
          {"path_coord", false},
      };
    }
    auto nextBites = mainprg_.ds_.get_next_bites_from(BiteID);
    for (auto nextBite : nextBites)
    {
      auto end_xy = mainprg_.ds_.get_bite_coord(nextBite);
      std::vector<ConnectionID> connectionIDs =
          mainprg_.ds_.get_connections(BiteID, nextBite);

      if (connectionIDs.empty())
      {
        continue;
      }
      for (auto connectionID : connectionIDs)
      {
        bool is_path_coord = false;

        std::vector<Coord> path_points =
            mainprg_.ds_.get_connection_coords(BiteID, connectionID);
        if (path_points.empty())
        {
          errorset.insert(
              "show_maze: get_connection_coords() returned no path points. ");
          continue;
        }
        path_points.insert(path_points.begin(), start_xy);
        path_points.push_back(end_xy);
        std::vector<Coord> path_visited_coords;

        for (size_t i = 0; i < path_points.size() - 1; i++)
        {
          Coord current_path_point = path_points[i];
          Coord next_path_point = path_points[i + 1];
          // Show in the ui the current path point and the next path point
          if (current_path_point.x == next_path_point.x)
          {
            int y_step = (next_path_point.y > current_path_point.y) ? 1 : -1;
            for (int y = current_path_point.y; y != next_path_point.y + y_step;
                 y += y_step)
            {
              Coord current_coord = Coord(current_path_point.x, y);
              // If current_coord is in all_path_coords, then set
              // is_path_coord to true
              if (all_path_coords.find(current_coord) !=
                  all_path_coords.end())
              {
                is_path_coord = true;
              }
              else
              {
                is_path_coord = false;
              }
              // IF the next y is the same as the next path point, then use
              // the next path point as the next coord
              Coord next_coord = Coord(current_path_point.x, y + y_step);
              if (y + y_step >= next_path_point.y)
              {
                // show in the ui that it gets here
                next_coord = next_path_point;
              }
              // if next_coord and current_coord are equal, then continue
              if (next_coord == current_coord)
              {
                continue;
              }
              // Coord next_coord = Coord(current_path_point.x, y + y_step);
              update_coord_paths(coord_paths, current_coord, next_coord,
                                 path_visited_coords, is_path_coord);
            }
          }
          else
          {
            int x_step = (next_path_point.x > current_path_point.x) ? 1 : -1;
            for (int x = current_path_point.x; x != next_path_point.x + x_step;
                 x += x_step)
            {
              Coord current_coord = Coord(x, current_path_point.y);
              if (all_path_coords.find(current_coord) !=
                  all_path_coords.end())
              {
                is_path_coord = true;
              }
              else
              {
                is_path_coord = false;
              }
              Coord next_coord = Coord(x + x_step, current_path_point.y);
              if (x + x_step >= next_path_point.x)
              {
                next_coord = next_path_point;
              }
              if (next_coord == current_coord)
              {
                continue;
              }
              update_coord_paths(coord_paths, current_coord, next_coord,
                                 path_visited_coords, is_path_coord);
            }
          }
        }
      }
    }
  }
  // initially, make everything inside the maze black
  for (unsigned int x = 0; x < DEF_MAZE_WIDTH; ++x)
  {
    for (unsigned int y = 0; y < DEF_MAZE_HEIGHT; ++y)
    {
      Coord current_coord = Coord(x, y);
      if (coord_paths.find(current_coord) == coord_paths.end())
      {
        add_walls_to_maze_cell(x, y, true, true, true, true, true, false);
      }
      else
      {
        add_walls_to_maze_cell(x, y, coord_paths[current_coord]["east_wall"],
                               coord_paths[current_coord]["west_wall"],
                               coord_paths[current_coord]["north_wall"],
                               coord_paths[current_coord]["south_wall"], false,
                               coord_paths[current_coord]["path_coord"]);
      }
    }
  }
}

void MainWindow::update_coord_paths(
    std::unordered_map<Coord, std::unordered_map<std::string, bool>>
        &coord_paths,
    const Coord &current_coord, const Coord &next_coord,
    std::vector<Coord> &path_visited_coords, bool is_path_coord)
{
  // In the ui, print the last visited coord, the current coord and the next
  // coord
  bool east_wall = true, west_wall = true, north_wall = true, south_wall = true;
  if (!path_visited_coords.empty())
  {
    auto last_visited_coord = path_visited_coords.back();
    if (last_visited_coord.x < current_coord.x)
      west_wall = false;
    if (last_visited_coord.x > current_coord.x)
      east_wall = false;
    if (last_visited_coord.y < current_coord.y)
      north_wall = false;
    if (last_visited_coord.y > current_coord.y)
      south_wall = false;
  }
  if (next_coord.x < current_coord.x)
    west_wall = false;
  if (next_coord.x > current_coord.x)
    east_wall = false;
  if (next_coord.y < current_coord.y)
    north_wall = false;
  if (next_coord.y > current_coord.y)
    south_wall = false;

  if (coord_paths.find(current_coord) == coord_paths.end())
  {
    coord_paths[current_coord] = {{"east_wall", true},
                                  {"west_wall", true},
                                  {"north_wall", true},
                                  {"south_wall", true},
                                  {"path_coord", false}};
  }
  coord_paths[current_coord] = {
      {"east_wall",
       !east_wall ? east_wall : coord_paths[current_coord]["east_wall"]},
      {"west_wall",
       !west_wall ? west_wall : coord_paths[current_coord]["west_wall"]},
      {"north_wall",
       !north_wall ? north_wall : coord_paths[current_coord]["north_wall"]},
      {"south_wall",
       !south_wall ? south_wall : coord_paths[current_coord]["south_wall"]},
      {"path_coord", is_path_coord ? is_path_coord
                                   : coord_paths[current_coord]["path_coord"]}};
  path_visited_coords.push_back(current_coord);
}

void MainWindow::update_view()
{

  std::unordered_set<std::string> errorset;
  try
  {
    gscene_->clear();
    // Remove all bites
    bites_.clear();
    show_area();

    if (ui->contours_checkbox->isChecked())
    {
      show_contours(errorset);
    }
    if (ui->maze_toggle->isChecked())
    {
      show_maze(errorset);
    }
    if (ui->bites_checkbox->isChecked())
    {
      show_bites(errorset);
    }
    update_view_maze();
  }
  catch (NotImplemented const &e)
  {
    errorset.insert(std::string("NotImplemented while updating graphics: ") +
                    e.what());
    std::cerr << std::endl
              << "NotImplemented while updating graphics: " << e.what()
              << std::endl;
  }

  if (!errorset.empty())
  {
    std::ostringstream errorstream;
    for (auto const &errormsg : errorset)
    {
      errorstream << "Error from GUI: " << errormsg << std::endl;
    }
    output_text(errorstream);
    output_text_end();
  }
}

void MainWindow::output_text(ostringstream &output)
{
  string outstr = output.str();
  if (!outstr.empty())
  {
    if (outstr.back() == '\n')
    {
      outstr.pop_back();
    } // Remove trailing newline
    ui->output->appendPlainText(QString::fromStdString(outstr));
    ui->output->ensureCursorVisible();
    ui->output->repaint();
  }

  output.str(""); // Clear the stream, because it has already been output
}

void MainWindow::update_path_distance(
    std::vector<std::pair<Coord, Distance>> &bite_path_distance)
{
  bite_path_distance_ = bite_path_distance;
}

void MainWindow::clear_path_distance() { bite_path_distance_.clear(); }

void MainWindow::output_text_end()
{
  ui->output->moveCursor(QTextCursor::End);
  ui->output->ensureCursorVisible();
  ui->output->repaint();
}

bool MainWindow::check_stop_pressed() const
{
  QCoreApplication::processEvents();
  return stop_pressed_;
}

/**
 * @brief Execute the command in the input line
 * @details Parse the command from the input line, execute it and update the
 * view
 */
void MainWindow::execute_line()
{
  try
  {
    auto line = ui->lineEdit->text();
    // Remove previous path distance
    clear_path_distance();
    clear_input_line();
    ui->output->appendPlainText(QString::fromStdString(MainProgram::PROMPT) +
                                line);

    ostringstream output;
    bool cont = mainprg_.command_parse_line(line.toStdString(), output);
    ui->lineEdit->clear();
    output_text(output);
    output_text_end();

    ui->stop_button->setEnabled(false);
    ui->execute_button->setEnabled(true);
    stop_pressed_ = false;

    ui->lineEdit->setFocus();

    update_view();

    if (!cont)
    {
      close();
    }
  }
  catch (std::exception const &e)
  {
    std::ostringstream errorstream;
    errorstream << "Error: " << e.what() << std::endl;
    output_text(errorstream);
    output_text_end();
  }
}

void MainWindow::cmd_selected(int idx)
{
  ui->lineEdit->insert(QString::fromStdString(mainprg_.cmds_[idx].cmd + " "));
  ui->cmd_info_text->setText(QString::fromStdString(
      mainprg_.cmds_[idx].cmd + " " + mainprg_.cmds_[idx].info));

  ui->lineEdit->setFocus();
}

void MainWindow::number_selected(QString const &number)
{
  ui->lineEdit->insert(number + " ");

  ui->lineEdit->setFocus();
}

void MainWindow::select_file()
{
  QFileDialog dialog(this, "Select file", QDir::currentPath(),
                     "Command list (*.txt)");
  dialog.setFileMode(QFileDialog::ExistingFiles);
  if (dialog.exec())
  {
    auto filenames = dialog.selectedFiles();
    for (auto &i : filenames)
    {
      auto filename = QDir("").relativeFilePath(i);
      ui->lineEdit->insert("\"" + filename + "\" ");
    }
  }

  ui->lineEdit->setFocus();
}

void MainWindow::clear_input_line()
{
  ui->cmd_info_text->clear();
  ui->lineEdit->clear();
  ui->lineEdit->setFocus();
}

void MainWindow::fit_view()
{

  ui->graphics_view_maze->fitInView(gscenep_->itemsBoundingRect(),
                                    Qt::KeepAspectRatio);
}

void MainWindow::scene_selection_change()
{
  auto items = gscene_->selectedItems();
  if (!items.empty())
  {
    for (auto i : items)
    {
      auto data = i->data(0);
      if (!selection_clear_in_progress)
      {
        ostringstream output;
        output << "*click* ";
        std::string ui_str;
        if (data.canConvert<BiteID>())
        {
          auto biteid = data.value<BiteID>();
          ui_str = mainprg_.print_bite(biteid, output);
        }
        if (data.canConvert<ContourID>())
        {
          auto contourid = data.value<ContourID>();
          ui_str = mainprg_.print_contour(contourid, output);
        }
        else if (data.canConvert<Coord>())
        {
          auto coord = data.value<Coord>();
          ui_str = mainprg_.print_coord(coord, output);
        }
        if (!ui_str.empty())
        {
          ui->lineEdit->insert(QString::fromStdString(ui_str + " "));
        }
        output_text(output);
        output_text_end();
      }
      i->setSelected(false);
      selection_clear_in_progress = !selection_clear_in_progress;
    }
  }
}

void MainWindow::clear_selection() { gscene_->clearSelection(); }

#include <QApplication>

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    auto status = a.exec();
    std::cerr << "Program ended normally." << std::endl;
    return status;
  }
  else
  {
    return MainProgram::mainprogram(argc, argv);
  }
}

#else

int main(int argc, char *argv[])
{
  return MainProgram::mainprogram(argc, argv);
}
#endif
