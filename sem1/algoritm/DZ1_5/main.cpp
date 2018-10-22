/*На числовой прямой окрасили N отрезков.
  Известны координаты левого и правого концов каждого отрезка [Li, Ri]. Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.
  N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 109].
 *
 * https://contest.yandex.ru/contest/9367/run-report/13067058/
 *
 * */

#include <iostream>
#include <cstring>

using namespace std;

struct Point {
  int value;
  bool is_start;

  explicit Point(int _first = 0, bool _second = false) : value(_first), is_start(_second) {}
};

// сравнение точек
bool comparsion(const Point &point_1, const Point &point_2) {
  return point_1.value < point_2.value;
}

Point *merge(Point *arr_a, size_t size_a, Point *arr_b, size_t size_b) {
  auto *result = new Point[size_a + size_b];
  int idx_a = 0, idx_b = 0, idx_result = 0;

  /* Извлекаем крайнии элементы, пока один из массивов не опустеет */
  while (idx_a < size_a && idx_b < size_b)
    if (comparsion(arr_a[idx_a], arr_b[idx_b])) {
      result[idx_result++] = arr_a[idx_a++];
    } else {
      result[idx_result++] = arr_b[idx_b++];
    }

  /* Если arr_b опустел, то копируем arr_a */
  while (idx_a < size_a)
    result[idx_result++] = arr_a[idx_a++];

  /* Если arr_a опустел, то копируем arr_b */
  while (idx_b < size_b)
    result[idx_result++] = arr_b[idx_b++];
  return result;
}

void merge_sort(Point *a, size_t len) {
  if (len <= 1) {
    return;
  }
  size_t firstLen = len / 2;
  size_t secondLen = len - firstLen;

  merge_sort(a, firstLen);
  merge_sort(a + firstLen, secondLen);

  Point *c = merge(a, firstLen, a + firstLen, secondLen);

  memcpy(a, c, sizeof(Point) * len);
  delete[] c;
}

void get_arr(Point *arr, size_t size_arr) {
  int idx = 0;
  int value1, value2;
  while (idx < size_arr) {
    cin >> value1 >> value2;
    arr[idx++] = Point(value1, true);
    arr[idx++] = Point(value2, false);
  }
}

int get_sum_len_by_layer(Point *arr, size_t size_arr, int layer) {
  int depth = 1;
  int start_point = arr[0].value;
  int end_point = 0;
  int sum = 0;

  for (int tmp_idx = 1; tmp_idx < size_arr; tmp_idx++) {
    end_point = arr[tmp_idx].value;
    if (depth == layer) {
      int lenght = end_point - start_point;
      sum += lenght;
    }
    start_point = end_point;
    if (arr[tmp_idx].is_start) {
      depth++;
    } else {
      depth--;
    }
  }

  return sum;
}

int main() {
  size_t count_point = 0;
  if (!(cin >> count_point))
    return 0;

  size_t size_arr = count_point * 2;
  auto *arr = new Point[size_arr];

  get_arr(arr, size_arr);

  merge_sort(arr, size_arr);

  int layer = 1;
  int result = get_sum_len_by_layer(arr, size_arr, layer);
  cout << result;

  delete[] arr;
  return 0;
}