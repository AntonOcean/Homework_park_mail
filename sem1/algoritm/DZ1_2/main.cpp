/*Дан !отсортированный! массив различных целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
 * Для каждого элемента массива B[i] найдите минимальный индекс элемента массива A[k],
 * ближайшего по значению к B[i]. n ≤ 110000, m ≤ 1000.
 * Время работы поиска для каждого элемента B[i]: O(log(k)).
 *
 * https://contest.yandex.ru/contest/9367/run-report/12444874/
 *
 * */
#include <iostream>

using namespace std;

int *get_data(size_t N) {
  int *tmp = new int[N];
  for (int i = 0; i < N; i++) {
    cin >> tmp[i];
  }
  return tmp;
}

size_t get_size() {
  size_t tmp;
  cin >> tmp;
  return tmp;
}

int abs_diff(int a, int b) {
  int result = a - b;
  return (result >= 0) ? result : -result;
}

void get_border(int *left_border, int *right_border, int current_b, const int *arr_a, size_t N) {
  for (int i = 1; i < N; i *= 2) {
    if (current_b < arr_a[i]) {
      *right_border = i;
      break;
    }
    *left_border = i;
  }
}

int find_bro_idx(int current_b, int *arr_a, size_t N) {
  int right_border = (int) N - 1;
  int left_border = 0;

  get_border(&left_border, &right_border, current_b, arr_a, N);

  int middle_pos;
  while (left_border < right_border) {
    middle_pos = (left_border + right_border) / 2;
    int diff_left = abs_diff(arr_a[middle_pos], current_b);
    int diff_right = abs_diff(arr_a[middle_pos + 1], current_b);

    if (diff_right < diff_left) {
      left_border = middle_pos + 1;

    } else {
      right_border = middle_pos;
    }

  }

  return left_border;
}

int main() {
  size_t N = get_size();
  int *arr_a = get_data(N);
  size_t M = get_size();
  int *arr_b = get_data(M);

  for (int k = 0; k < M; k++) {
    int current_b = arr_b[k];
    int bro_idx = find_bro_idx(current_b, arr_a, N);
    cout << bro_idx << " ";
  }

  delete[] arr_a;
  delete[] arr_b;
  return 0;
}