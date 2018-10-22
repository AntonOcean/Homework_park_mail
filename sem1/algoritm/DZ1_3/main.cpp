/*
Реализовать очередь с динамическим зацикленным буфером.
Обрабатывать команды push back и pop front.

 https://contest.yandex.ru/contest/9367/run-report/12462127/

 * */
#include <iostream>

using namespace std;

/* Очередь на динамическом массиве зацикленный */
class Queue {
 public:
  explicit Queue(int buffer_size);
  ~Queue() { delete[] buffer; }
  Queue(const Queue &) = delete;
  Queue(Queue &&) = delete;
  Queue &operator=(const Queue &) = delete;
  Queue &operator=(Queue &&) = delete;

  void push_back(int element);
  int pop_front();
  bool is_empty() const { return head == tail; }
  bool is_full() const { return (tail+1)%buffer_size == head; }
  void resize();

 private:
  int* buffer;
  size_t buffer_size;
  int head; // индекс головы
  int tail; // индекс хвоста
};

/* Инициализируем очередь */
Queue::Queue(int buffer_size) :
  buffer_size( buffer_size ),
  head( 0 ),
  tail( 0 )
{
  buffer = new int[buffer_size];
}

/* Добавляем элемент в конец массива */
void Queue::push_back(int element) {
  if (is_full()) {
    resize();
  }
    buffer[tail] = element;
    tail = (tail + 1) % buffer_size; // двигаем конец вперед по циклу
}

/* Извлекаем первый элемент массива */
int Queue::pop_front() {
  if (is_empty()) {
    return -1;
  }
  int result = buffer[head];
  head = (head+1)%buffer_size;
  return result;
}

void Queue::resize() {
  int* tmp_buffer = new int[buffer_size*2];
  int i=0;
  for(;i<buffer_size;i++) {
    tmp_buffer[i]= pop_front();
  }
  buffer = nullptr;
  delete[] buffer;
  buffer = tmp_buffer;
  tail = i-1;
  head = 0;
  buffer_size*=2;
  tmp_buffer = nullptr;
  delete[] tmp_buffer;
}

bool action_command(int number, int value, Queue* queue) {
  switch (number) {
    int tmp;
    case 2:
      tmp = queue->pop_front();
      if (tmp != value) {
        return false;
      }
      break;
    case 3:
      queue->push_back(value);
      break;
    default:
      return false;
  }
  return true;
}


int main() {
  int command_count = 0, number, value;
  cin >> command_count;
  if (command_count <= 0) {
    return 0;
  }
  Queue queue(2);

  for(int i = 0; i < command_count; i++) {
    cin >>number>>value;
    if (!action_command(number, value, &queue)) {
      printf("NO");
      return 0;
    }
  }

  printf("YES");

  return 0;
}