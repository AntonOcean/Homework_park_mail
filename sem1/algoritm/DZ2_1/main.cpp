/**
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми.
Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество,
удаления строки из множества и проверки принадлежности данной строки множеству.

 Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
*/
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#define BUFFER_SIZE 8
#define HASH_CONST 17

using namespace std;

int hash_function(const string &key, int size, int probe) {
  int hash = 0;
  for (int i = static_cast<int>(key.size()); i >= 0; i--) {
    hash = (HASH_CONST * hash + key[i]) % size;
  }
  int c1 = 1;
  int c2 = 3;
  return (hash + c1 * probe + c2 * probe * probe) % size;
}

// элемент хеш-таблицы
struct HashTableNode {
  string key;
  bool deleted;
  explicit HashTableNode(string key) : key(move(key)), deleted(false) {}
};

// хеш-таблица
class HashTable {
 public:
  explicit HashTable() : buffer(BUFFER_SIZE), current_size(0) {
  }

  HashTable(const HashTable &) = delete;
  HashTable(HashTable &&) = delete;
  HashTable &operator=(const HashTable &) = delete;
  HashTable &operator=(HashTable &&) = delete;

  bool add(const string &key);
  bool remove(const string &key);
  bool has(const string &key) const;

 private:
  void rehash();

  vector<shared_ptr<HashTableNode>> buffer;
  int current_size;
};

bool HashTable::add(const string &key) {


  int hash;
  int insert_pos = -1;

  for (int probe = 0; probe < buffer.size(); ++probe) {
    hash = hash_function(key, static_cast<int>(buffer.size()), probe);

    if (buffer[hash] != nullptr && buffer[hash]->key == key && !buffer[hash]->deleted) {
      return false;
    } else if (buffer[hash] == nullptr) {
      if (insert_pos == -1)
        insert_pos = hash;
      break;
    } else if (buffer[hash]->deleted) {
      if (insert_pos == -1)
        insert_pos = hash;
    }
  }

  if (insert_pos == -1)
    return false;

  if (buffer[insert_pos] != nullptr)
    buffer[insert_pos].reset();

  buffer[insert_pos] = make_shared<HashTableNode>(key);
  current_size++;
// cout<<current_size<<"   "<< static_cast<int>(buffer.size())<<" cd "<< static_cast<int>(buffer.size()) * 3 / 4;
  double a = (double)current_size / (double) buffer.size();

  if (a >= 0.75) {
    rehash();

  }


  return true;
}

bool HashTable::remove(const string &key) {

  int hash = 0;

  for (int probe = 0; probe < buffer.size(); ++probe) {
    hash = hash_function(key, static_cast<int>(buffer.size()), probe);

    if (buffer[hash] != nullptr && buffer[hash]->key == key && !buffer[hash]->deleted) break;
    else if (buffer[hash] == nullptr) return false;
  }

  if (buffer[hash] == nullptr)
    return false;

  buffer[hash]->deleted = true;
  current_size--;

  return true;

}

bool HashTable::has(const string &key) const {
  for (int i = 0; i < buffer.size(); i++) {
    int hash = hash_function(key, static_cast<int>(buffer.size()), i);

    if (buffer[hash] == nullptr)
      break;
    if (buffer[hash]->key == key && !buffer[hash]->deleted)
      return true;

  }
  return false;
}

void HashTable::rehash() {
  vector<shared_ptr<HashTableNode>> old_buffer = buffer;
  buffer.assign(buffer.size() * 2, nullptr);
  current_size = 0;

  for (auto &i : old_buffer)
    if (i != nullptr && !i->deleted) {
      add(i->key);
      i.reset();
    }
}

int main() {
  HashTable table;

  char command = '\0';
  string key;

  bool result;
  while (cin >> command >> key) {
    switch (command) {
      case '+':
        result = table.add(key);
        break;
      case '?':
        result = table.has(key);
        break;
      case '-':
        result = table.remove(key);
        break;
      default:
        continue;
    }
    cout << (result ? "OK" : "FAIL") << endl;
  }

  return 0;
}
