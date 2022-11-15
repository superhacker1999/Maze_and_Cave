#include "cave.h"

s21::Cave::Cave(size_t rows, size_t cols, __limits limit, int birth_chance)
    : m_cave_(std::make_unique<s21::Matrix<bool>>(rows, cols)), m_limits_(limit), m_birth_chance_(birth_chance) { ; }

void s21::Cave::GetCaveFromFile(const std::string& file_path) {
  std::string buffer;
  std::vector<bool> cave;
  std::fstream file;
  file.open(file_path);
  if (file.is_open()) {
    while (getline(file, buffer)) {
      while (buffer.size() > 0) {
        cave.push_back(stoi(buffer));
        buffer.erase(0, buffer.find_first_of(' ') + 1);
      }
    }
  }
  cave.erase(cave.begin());
  cave.erase(cave.begin());
  m_cave_ref_ = s21::Matrix<bool>::VectorToMatrix(cave);
}

void s21::Cave::InitializeCave() {
  for (int i = 0; i < m_cave_.get()->GetRows(); ++i)
    for (int j = 0; j < m_cave_.get()->GetCols(); ++j) {
      if (GetRandomNumber_() <= m_birth_chance_)
        m_cave_ref_(i, j) = kALIVE;
      else
        m_cave_ref_(i, j) = kDEAD;
    }
}

int s21::Cave::GetRandomNumber_() {
  std::mt19937 engine;
  std::random_device device;
  engine.seed(device());
  int num = engine() % 100 - 0;
  return num;
}

bool s21::Cave::Transform() {
  bool changed = false;
  s21::Matrix<bool> tmp_cave(m_cave_.get()->GetRows(), m_cave_.get()->GetCols());
  for (int i = 0; i < m_cave_.get()->GetRows(); ++i) {
    for (int j = 0; j < m_cave_.get()->GetCols(); ++j) {
      int alive_count = GetAliveNeighboursCount_(i, j);
      if (m_cave_ref_(i, j) == kALIVE && alive_count < m_limits_.__death_limit) {
        tmp_cave(i, j) = kDEAD;
        changed = true;
      } else if (m_cave_ref_(i, j) == kDEAD && alive_count > m_limits_.__birth_limit) {
        tmp_cave(i, j) = kALIVE;
        changed = true;
      } else {
        tmp_cave(i, j) = m_cave_ref_(i, j);
      }
    }
  }
  m_cave_ref_ = tmp_cave;
  return changed;
}

void s21::Cave::TransformCycle() {
  // пока поле меняется, запускаем трансформацию
  while (Transform()) {}
}

int s21::Cave::GetAliveNeighboursCount_(int i, int j) {
  int counter = 0;
  for (int buf_i = i - 1; buf_i < i + 2; ++buf_i) {
    for (int buf_j = j - 1; buf_j < j + 2; ++buf_j) {
      if (buf_i != i || buf_j != j) {
        try {
          if (m_cave_ref_(buf_i, buf_j) == kALIVE)
            counter++;
        } catch (std::out_of_range& err) {  // если клетки нет, считаем ее живой
          counter++;
        }
      }
    }
  }
  return counter;
}