/*
 * На вокзале есть некоторое количество тупиков, куда прибывают электрички.
 * Этот вокзал является их конечной станцией.
 * Дано расписание движения электричек, в котором для каждой электрички указано время ее прибытия,
 * а также время отправления в следующий рейс.
 * Электрички в расписании упорядочены по времени прибытия.
 * Когда электричка прибывает, ее ставят в свободный тупик с минимальным номером.
 * При этом если электричка из какого-то тупика отправилась в момент времени X,
 * то электричку, которая прибывает в момент времени X, в этот тупик ставить нельзя,
 * а электричку, прибывающую в момент X+1 — можно.
 * В данный момент на вокзале достаточное количество тупиков для работы по расписанию.
 * Напишите программу, которая по данному расписанию определяет,
 * какое минимальное количество тупиков требуется для работы вокзала.
 *
 * https://contest.yandex.ru/contest/9367/run-report/13021091/
 *
 * */

#include <iostream>

using namespace std;

struct Train {
  int come_time;
  int out_time;

  Train() : come_time(0), out_time(0) {}
  Train(int _first, int _second) : come_time(_first), out_time(_second) {}
};

/* Очередь на динамическом массиве, поддержка мин кучи, на вершине поезд который раньше всех уезжает */
class Queue {
 public:
  explicit Queue(size_t buffer_size);
  ~Queue() { delete[] buffer; }
  Queue(const Queue &) = delete;
  Queue(Queue &&) = delete;
  Queue &operator=(const Queue &) = delete;
  Queue &operator=(Queue &&) = delete;

  void enqueue(Train element);
  void dequeue();
  bool is_empty() const { return buffer_top < 0; };
  Train peek_min() const;
  int get_current_size() const { return buffer_top + 1; };

 private:
  void resize();
  void sift_up(int index);
  void sift_down(int index);
  void swap(Train &one, Train &two);
  bool is_full() const { return buffer_top + 1 >= buffer_size; };

  Train *buffer;
  size_t buffer_size;
  int buffer_top;
};

/* Инициализируем очередь */
Queue::Queue(size_t buffer_size) :
    buffer_size(buffer_size),
    buffer_top(-1) {
  buffer = new Train[buffer_size];
}

/* Добааляем элемент */
void Queue::enqueue(Train element) {
  if (is_full())
    resize();

  buffer_top++;
  buffer[buffer_top] = element;

  sift_up(buffer_top);
}

/* Удаляем элемент */
void Queue::dequeue() {
  if (is_empty())
    exit(1);

  buffer[0] = buffer[buffer_top];
  sift_down(0);
  buffer_top--;
}
void Queue::resize() {
  auto *tmp_buffer = new Train[buffer_size * 2];
  int i = 0;
  for (; i < buffer_size; i++) {
    tmp_buffer[i] = buffer[i];
  }
  buffer = nullptr;
  delete[] buffer;
  buffer = tmp_buffer;
  buffer_size *= 2;
  tmp_buffer = nullptr;
  delete[] tmp_buffer;
}

void Queue::swap(Train &one, Train &two) {
  Train temp = one;
  one = two;
  two = temp;
}

// Проталкиваем наверх
void Queue::sift_up(int index) {
  while (index > 0) {
    int parent = (index - 1) / 2;
    if (buffer[index].out_time >= buffer[parent].out_time)
      return;
    swap(buffer[index], buffer[parent]);
    index = parent;
  }
}

void Queue::sift_down(int index) {
  int left = 2 * index + 1;
  int right = 2 * index + 2;
  int current_size = buffer_top + 1;
  int largest = index;

  // Ищем самый первый поезд, который уедет
  if (left < current_size && buffer[left].out_time < buffer[index].out_time)
    largest = left;
  if (right < current_size && buffer[right].out_time < buffer[largest].out_time)
    largest = right;
  // Если такой поезд есть, проталкиваем его в корень
  if (largest != index) {
    swap(buffer[index], buffer[largest]);
    sift_down(largest);
  }
}

Train Queue::peek_min() const {
  return buffer[0];
}


int find_min_station(int command_count) {
  int count_min_station = 0;
  Queue station(static_cast<size_t>(command_count / 2));

  for (int i = 0; i < command_count; i++) {
    int come_time, out_time;
    cin >> come_time >> out_time;
    if (come_time >= out_time)
      exit(0);

    Train tmp_train(come_time, out_time);

    if (!station.is_empty()) {
      /* Пока на станции есть поезд который уезжает раньше, то убираем его */
      while (station.peek_min().out_time < tmp_train.come_time && !station.is_empty())
        station.dequeue();
    }

    station.enqueue(tmp_train);

    int current_size = station.get_current_size();

    if (current_size > count_min_station)
      count_min_station = current_size;

  }
  return count_min_station;
}

int main() {
  int command_count;
  if (!(cin >> command_count))
    return 0;

  cout << find_min_station(command_count);

  return 0;
}