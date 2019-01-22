/*
 * Требуется написать программу, которая способна вычислять арифметические выражения.
Выражения могут содержать:
1) знаки операций '+', '-', '/', '*'
2) Скобки '(', ')'
3) Целые и вещественные числа, в нотации '123', '123.345', все операции должны быть вещественны,
 а результаты выведены с точностю до двух знаков после запятой в том числе целые '2.00'
4) необходимо учитывать приоритеты операций, и возможность унарного минуса, пробелы ничего не значат
5) Если в выражении встретилась ошибка требуется вывести в стандартный поток вывода "[error]" (без кавычек)
 (1+2)*3/6.5 -> 1.38
*/

#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define minus '-'
#define plus '+'
#define open_bracket '('
#define close_bracket ')'
#define unary_minus '~'
#define division '/'
#define mul '*'
#define space ' '

/* Стек для чисел типа double */
struct node_digit {
  double data;
  struct node_digit *next;
};

/* Добавляем элемент в стек */
struct node_digit *push_digit(struct node_digit *head, double data) {
  struct node_digit *tmp = (struct node_digit *) malloc(sizeof(struct node_digit));
  if (tmp == NULL) {
    return NULL;
  }
  tmp->data = data;
  tmp->next = head;
  head = tmp;
  return head;
}

/* Удаляем элемент из стека */
struct node_digit *pop_digit(struct node_digit *head, double *element) {
  struct node_digit *tmp = head;
  *element = head->data;
  head = head->next;
  free(tmp);
  return head;
}

/* Если стек пуст 1, иначе 0 */
int empty_digit(struct node_digit *head) {
  return head == NULL ? TRUE : FALSE;
}

/* Очищаем стек */
struct node_digit *clear_digit(struct node_digit *head) {
  double element;
  while (!empty_digit(head)) {
    head = pop_digit(head, &element);
  }
  return head;
}

/* Стек для операций: + - * / ( ) */
struct node {
  char data;
  struct node *next;
};

/* Добавляем элемент в стек */
struct node *push(struct node *head, char data) {
  struct node *tmp = (struct node *) malloc(sizeof(struct node));
  if (tmp == NULL) {
    return NULL;
  }
  tmp->data = data;
  tmp->next = head;
  head = tmp;
  return head;
}

/* Удаляем элемент из стека */
struct node *pop(struct node *head, char *element) {
  struct node *tmp = head;
  *element = head->data;
  head = head->next;
  free(tmp);
  return head;
}

/* Если стек пуст 1, иначе 0 */
int empty(struct node *head) {
  return head == NULL ? TRUE : FALSE;
}

/* Очищаем стек */
struct node *clear(struct node *head) {
  char element;
  while (!empty(head)) {
    head = pop(head, &element);
  }
  return head;
}

/* Смотрим последний элемент */
char peek(struct node *head) {
  if (empty(head)) {
    return 0;
  }
  return head->data;
}

/* Очередь для сбора больших чисел FIFO */
struct node_q {
  char data;
  struct node_q *next;
};
typedef struct node_q node_q;

struct queue {
  size_t count;
  node_q *front;
  node_q *rear;
};
typedef struct queue queue;

/* Инициализируем очередь */
void initialize(queue *q) {
  q->count = 0;
  q->front = NULL;
  q->rear = NULL;
}

/* Проверка очереди на пустоту */
int isempty_queue(queue *q) {
  return (q->rear == NULL);
}

/* Добавляем элемент в очередь */
void enqueue(queue *q, char value) {
  node_q *tmp;
  tmp = malloc(sizeof(node_q));
  tmp->data = value;
  tmp->next = NULL;
  if (!isempty_queue(q)) {
    q->rear->next = tmp;
    q->rear = tmp;
  } else {
    q->front = q->rear = tmp;
  }
  q->count++;
}

/* Берем элемент из очереди */
char dequeue(queue *q) {
  node_q *tmp;
  char n = q->front->data;
  tmp = q->front;
  q->front = q->front->next;
  q->count--;
  free(tmp);
  tmp = NULL;
  return (n);
}

/* Чистим память */
void clear_memory(struct node **operators, struct node_digit **digits, queue **digit) {
  free(*digit);
  *operators = clear(*operators);
  *digits = clear_digit(*digits);
  free(*digits);
  free(*operators);
}

/* Получаем приоритет операции, если символ не валидный, то 0 */
int get_priority(char ch) {
  int result;
  switch (ch) {
    case plus:result = 1;
      break;
    case minus:result = 1;
      break;
    case mul:result = 2;
      break;
    case division:result = 2;
      break;
    case unary_minus:result = 3;
      break;
    case open_bracket:result = -1;
      break;
    default:result = 0;
  }
  return result;
}

/* Выполняем операцию бинарную */
_Bool action_bin(double left_operand, char operator, double right_operand, double *result) {
  switch (operator) {
    case plus:*result = left_operand + right_operand;
      break;
    case mul:*result = left_operand * right_operand;
      break;
    case minus:*result = left_operand - right_operand;
      break;
    case division:
      if (!right_operand)
        return FALSE;
      *result = left_operand / right_operand;
      break;
    default:return FALSE;
  }
  return TRUE;
}

/* Выполняем операцию унарную */
void action_un(double right_operand, double *result) {
  *result = -right_operand;
}

/* Вычисляем со стека */
_Bool calc_from_stack(struct node **operators, struct node_digit **digits) {
  char operator;
  double left_operand, right_operand, result;
  if (empty(*operators) || empty_digit(*digits))
    return FALSE;
  *operators = pop(*operators, &operator);
  *digits = pop_digit(*digits, &right_operand);
  if (operator != unary_minus) {

    if (empty_digit(*digits))
      return FALSE;
    *digits = pop_digit(*digits, &left_operand);

    if (!action_bin(left_operand, operator, right_operand, &result))
      return FALSE;

  } else {
    action_un(right_operand, &result);
  }

  *digits = push_digit(*digits, result);
  return TRUE;
}

_Bool calc_operators(struct node **operators, struct node_digit **digits, char current_char, int priority) {
  while (TRUE) {
    if (empty(*operators)) {
      *operators = push(*operators, current_char);
      break;
    }
    char current_operator = peek(*operators);
    int current_priority = get_priority(current_operator);
    if (!current_priority)
      return FALSE;
    /* Операторы кладем в стек по возрастанию приоритета операции,
     * иначе вычисляем, пока приоритет не будет меньше,
     * */
    if (current_priority < priority) {
      *operators = push(*operators, current_char);
      break;
    } else {
      if (!calc_from_stack(operators, digits))
        return FALSE;
    }
  }
  return TRUE;
}

/* Это конец */
_Bool char_is_end(char ch) {
  return ch == '\n' || ch == '\0' || ch == EOF;
}

_Bool char_is_digit(char ch) {
  return ch >= 48 && ch <= 57;
}

_Bool handler_close_backet(struct node **operators, struct node_digit **digits) {
  /* выполняем операции со стека пока не встретим ), когда она встретится мы ее забираем и идем далее */
  char operator = space;
  operator = peek(*operators);
  while (operator != open_bracket) {
    if (!calc_from_stack(operators, digits))
      return FALSE;
    operator = peek(*operators);
  }
  /* Убираем ) скобку */
  *operators = pop(*operators, &operator);
  return TRUE;
}

_Bool handler_end_calculate(struct node **operators, struct node_digit **digits) {
  while (!empty(*operators)) {
    if (!calc_from_stack(operators, digits))
      return FALSE;
  }
  return TRUE;
}

void handler_digit(struct node_digit **digits, queue **digit, size_t digit_size) {
  size_t size_tmp = digit_size;
  char *tmp = (char *) malloc(size_tmp * sizeof(char) + 1);
  for (int j = 0; j < size_tmp; j++) {
    tmp[j] = dequeue(*digit);
  }
  tmp[size_tmp] = '\0';
  double element = atof(tmp);
  *digits = push_digit(*digits, element);
  free(tmp);
  initialize(*digit);
}

_Bool calculate(const char *expression, size_t expr_size, double *result) {
  _Bool success = TRUE;
  char char_before = space;
  queue *digit = (queue *) malloc(sizeof(queue)); // собираем большие числа тип 12324.43242 по символам
  initialize(digit);
  int priority = 0;
  struct node *operators = NULL; // стек операторов
  struct node_digit *digits = NULL; // стек чисел

  for (int i = 0; i <= expr_size; i++) {
    char current_char = expression[i];

    /* Игнорируем пробел */
    if (current_char == space)
      continue;

    /* Выявялем унарный минус */
    if ((current_char == minus && i == 0) || (char_before == open_bracket && current_char == minus)) {
      current_char = unary_minus;
    }
    char_before = current_char;

    /* Собираем большое число тип 12234.4342 */
    if (current_char == '.' || char_is_digit(current_char)) {
      enqueue(digit, current_char);
      continue;
    }

    /* Записываем наше собранное большое число в стек чисел */
    if (digit->count)
      handler_digit(&digits, &digit, digit->count);

    /* Обрабатываем открывающую скобочку */
    if (empty(operators) || current_char == open_bracket) {
      operators = push(operators, current_char);
      continue;
    }

    /* Обрабатываем закрывающую скобочку */
    if (current_char == close_bracket) {
      if (!handler_close_backet(&operators, &digits)) {
        success = FALSE;
        break;
      }
      continue;
    }

    /* Если ввод выражения закончен */
    if (char_is_end(current_char)) {
      /* вычисляем что осталось в стеке пока операции не закончатся */
      if (!handler_end_calculate(&operators, &digits)) {
        success = FALSE;
        break;
      }
      continue;
    }

    /* Узнаем приоритет текущей операции */
    priority = get_priority(current_char);
    if (!priority) {
      success = FALSE;
      break;
    }

    /* Вычисляем операторы со стека */
    if (!calc_operators(&operators, &digits, current_char, priority)) {
      success = FALSE;
      break;
    }
  }

  if (success)
    digits = pop_digit(digits, result);

  clear_memory(&operators, &digits, &digit);

  return success;
}

char input_char() {
  char c = '\0';
  c = getchar();
  return c;
}

char *input_string(size_t *string_size) {
  struct buffer {
    char *string;
    size_t size;
    size_t capacity;
  } buf = {NULL, 0, 0};
  char c = '\0';
  while (c = input_char(), c != EOF && c != '\n' && c!='\0') {
    if (buf.size + 1 >= buf.capacity) {
      size_t new_capacity = !buf.capacity ? 1 : buf.capacity * 2;
      char *tmp = (char *) malloc((new_capacity + 1) * sizeof(char));
      if (!tmp) {
        if (buf.string) {
          free(buf.string);
        }
        return NULL;
      }
      if (buf.string) {
        tmp = strcpy(tmp, buf.string);
        free(buf.string);
      }
      buf.string = tmp;
      buf.capacity = new_capacity;
    }
    buf.string[buf.size] = c;
    buf.string[buf.size + 1] = '\0';
    ++buf.size;
  }
  *string_size = buf.size;
  return buf.string;
}

int main() {
  char *string = NULL;
  size_t string_size = 0;

  string = input_string(&string_size);
  if (!string) {
    printf("[error]");
    return 0;
  }

  double result = 0;

  if (calculate(string, string_size, &result)) {
    printf("%.2lf", result);
  } else {
    printf("[error]");
  }

  free(string);
  return 0;
}
