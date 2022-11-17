#ifndef SRC_MODEL_MAZE_MAZE_H_
#define SRC_MODEL_MAZE_MAZE_H_

#include <memory>

#include "../../helpers/matrix.h"

/*
первое значение из пары ячейки матрицы:
  true - справа от ячейки есть стенка
  false - справа от ячейки нет стенки

второе значение из пары ячейки матрицы:
  true - снизу от ячейки есть стенка
  false - снизу от ячейки нет стенки
*/

namespace s21 {
//#define right_wall first
//#define bottom_wall second
struct  walls {
    bool right_wall;
    bool bottom_wall;
};
constexpr int kEmpty = 0;

class Maze {
 public:
  using MatrixPtr = std::unique_ptr<s21::Matrix<walls>>;
  using MazeMatrix = s21::Matrix<walls>;

  explicit Maze(const std::string& file_path);
  explicit Maze(int rows, int cols);
  void outputMaze();

 private:
  void generateMaze();

  void fillEmptyValues();
  void assignUniqueSet();
  void addRightWall(int row);
  void mergeSet(int index, int element);
  void addBottomWall(int row);
  int calculateUniqueSet(int element);
  void checkBottomWall(int row);
  int calculateBottomWalls(int element, int row);
  void prepareNewLine(int row);
  void addEndLine();
  void checkEndLine();

  MazeMatrix getMazeFromFile(const std::string& file_path);

  void getMazeSize(std::fstream& file);
  MazeMatrix fillMazeMatrix(const std::string &file_path);
  void fillRightWall(std::fstream& file, std::vector<walls>& maze);
  void fillBottomWall(std::fstream& file, std::vector<walls>& maze);
  bool isWall(const int& state);
  void removePrevState(std::string& buffer);

  void getError() const;

  MatrixPtr m_maze_;  // указатель на матрицу, представляющую лабиринт
  int rows_;
  int cols_;
  std::vector<int> side_line_;
  int counter_;
  bool reading_error_;

};  // class Maze
}  // namespace s21

#endif  // SRC_MODEL_MAZE_MAZE_H_