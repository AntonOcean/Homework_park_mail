/*
 Даны два массива целых чисел одинаковой длины A[0..n-1] и B[0..n-1].
 Необходимо найти первую пару индексов i0 и j0, i0 <= j0,
 такую что A[i0] + B[j0] = max {A[i] + B[j], где 0 <= i < n, 0 <= j < n, i <= j}.
 Время работы - O(n). n ≤ 100000.

 https://contest.yandex.ru/contest/9367/run-report/12408016/

 * */
#include <iostream>

using namespace std;

void solve(const int *arr_a, const int *arr_b, size_t N, int *maxi, int *maxj) {
  int max_a = arr_a[0], maxi_current = 0;
  int max = arr_a[0] + arr_b[0];

  for (int i = 1; i < N; i++) {
    if (max_a < arr_a[i]) {
      max_a = arr_a[i];
      maxi_current = i;
    }
    if (max < max_a + arr_b[i]) {
      max = max_a + arr_b[i];
      *maxj = i;
      *maxi = maxi_current;
    }
  }
}

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

int main() {
  size_t N = get_size();
  int *arr_a = get_data(N);
  int *arr_b = get_data(N);
  int maxi = 0, maxj = 0;
  solve(arr_a, arr_b, N, &maxi, &maxj);
  cout << maxi << " " << maxj << endl;
  delete[] arr_a;
  delete[] arr_b;
  return 0;
}