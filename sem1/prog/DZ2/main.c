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


/* Стек для чисел типа double */
struct node_digit {
    double data;
    struct node_digit *next;
};

/* Добавляем элемент в стек */
struct node_digit *push_digit(struct node_digit *head, double data) {
    struct node_digit *tmp = (struct node_digit *) malloc(sizeof(struct node_digit));
    if (tmp == NULL) {
        exit(0);
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
    return head == NULL ? 1 : 0;
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
        exit(0);
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
    return head == NULL ? 1 : 0;
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
//        printf("[error]");
        return 0;
    }
    return head->data;
}

/* Получаем приоритет операции, если символ не валидный, то 0 */
int get_priority(char ch) {
    int result;
    switch (ch) {
        case '+':
            result = 1;
            break;
        case '-':
            result = 1;
            break;
        case '*':
            result = 2;
            break;
        case '/':
            result = 2;
            break;
        case '~':
            result = 3;
            break;
        case '(':
            result = -1;
            break;
        default:
            result = 0;
    }
    return result;
}

/* Выполняем операцию бинарную */
double action_bin(double a, char operator, double b, _Bool *success) {
    double result = 0;
    switch (operator) {
        case '+':
            result = a + b;
            break;
        case '*':
            result = a * b;
            break;
        case '-':
            result = a - b;
            break;
        case '/':
            if (!b) {
                (*success) = 0;
                break;
            }
            result = a / b;
            break;
        default:
            (*success) = 0;
            break;
    }
    return result;
}

/* Выполняем операцию унарную */
double action_un(char operator, double b, _Bool *success) {
    double result = 0;
    if (operator == '~') {
        result = -b;
    } else {
        (*success) = 0;
    }
    return result;
}


/* Вычисляем со стека */
_Bool calc(struct node **operators, struct node_digit **digits) {
    _Bool success = 1;
    char operator;
    double digit_1, digit_2, result;
    if (empty(*operators) || empty_digit(*digits)) {
        return 0;
    }
    *operators = pop(*operators, &operator);
    *digits = pop_digit(*digits, &digit_2);
    if (operator != '~') {
        if (empty_digit(*digits)) {
            return 0;
        }
        *digits = pop_digit(*digits, &digit_1);

        result = action_bin(digit_1, operator, digit_2, &success);
    } else {
        result = action_un(operator, digit_2, &success);
    }
    if (!success) {
        return 0;
    }
    *digits = push_digit(*digits, result);
    return 1;
}

/* Чистим память */
void down(struct node **operators, struct node_digit **digits, char **digit) {
    free(*digit);
    *operators = clear(*operators);
    *digits = clear_digit(*digits);
    free(*digits);
    free(*operators);
}

/* Очередь для сбора больших чисел FIFO */
struct node_q
{
    char data;
    struct node_q *next;
};
typedef struct node_q node_q;

struct queue
{
    int count;
    node_q *front;
    node_q *rear;
};
typedef struct queue queue;

void initialize(queue *q)
{
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

int isempty_queue(queue *q)
{
    return (q->rear == NULL);
}

void enqueue(queue *q, char value)
{
    node_q *tmp;
    tmp = malloc(sizeof(node_q));
    tmp->data = value;
    tmp->next = NULL;
    if(!isempty_queue(q))
    {
        q->rear->next = tmp;
        q->rear = tmp;
    }
    else
    {
        q->front = q->rear = tmp;
    }
    q->count++;
}

int dequeue(queue *q)
{
    node_q *tmp;
    char n = q->front->data;
    tmp = q->front;
    q->front = q->front->next;
    q->count--;
    free(tmp);
    return(n);
}





int main() {
    char old_ch=' ';
    char ch, current_operator; // текущий символ строки
    _Bool in_begin = 1;
    int priority = 0, current_priority = 0;
    size_t MAX = 50;
    struct node *operators = NULL; // стек операторов
    struct node_digit *digits = NULL; // стек чисел
    char *digit = (char *) malloc(MAX * sizeof(char)+1); // собираем большие числа тип 12324.43242 по символам
    int idx = 0; // кол-во цифр в числе

    do {
        scanf("%c", &ch);
        /* Пробелы игнорируем */
        if (ch == ' ') {
            continue;
        }
        /* Выявялем унарный минус */
        if ((ch == '-' && in_begin) || (old_ch == '(' && ch == '-')) {
            ch = '~';
        }
        in_begin = 0;
        old_ch = ch;
        /* Собираем большое число тип 12234.4342 */
        if (ch == '.' || (ch >= 48 && ch <= 57)) {
            if (idx >= MAX) {
                printf("[error]");
                return 0;
            }
            digit[idx] = ch;
            idx++;
            continue;
        }
        /* Записываем наше собранное большое число в стек чисел */
        if (idx) {
            digit[idx] = '\0';
            double element = atof(digit);
            digits = push_digit(digits, element);
            idx = 0;
            free(digit);
            digit = NULL;
            digit = (char *) malloc(MAX * sizeof(char)+1);
        }

        if (empty(operators) || ch == '(') {
            operators = push(operators, ch);
            continue;
        }
        /* Обрабатываем закрывающую скобочку */
        if (ch == ')') {
            /* выполняем операции со стека пока не встретим ), когда она встретится мы ее забираем и идем далее */
            char operator;
            operator = peek(operators);
            while (operator != '(') {
                if (!calc(&operators, &digits)) {
                    printf("[error]");
                    down(&operators, &digits, &digit);
                    return 0;
                }
                operator = peek(operators);
            }
            /* Убираем ) скобку */
            operators = pop(operators, &operator);
            continue;
        }
        /* Если ввод выражения закончен */
        if (ch == '\n' || ch == '\0' || ch==EOF) {
            /* вычисляем что осталось в стеке пока операции не закончатся */
            while (!empty(operators)) {
                if (!calc(&operators, &digits)) {
                    printf("[error]");
                    down(&operators, &digits, &digit);
                    return 0;
                }
            }
            continue;
        }
        /* Узнаем приоритет текущей операции */
        priority = get_priority(ch);
        if (!priority) {
            printf("[error]");
            down(&operators, &digits, &digit);
            return 0;
        }
        do {
            if (empty(operators)) {
                operators = push(operators, ch);
                break;
            }
            current_operator = peek(operators);
            current_priority = get_priority(current_operator);
            if (!current_priority) {
                printf("[error]");
                down(&operators, &digits, &digit);
                return 0;
            }
            /* Операторы кладем в стек по возрастанию приоритета операции,
             * иначе вычисляем, пока приоритет не будет меньше,
             * */
            if (current_priority < priority) {
                operators = push(operators, ch);
                break;
            } else {
                if (!calc(&operators, &digits)) {
                    printf("[error]");
                    down(&operators, &digits, &digit);
                    return 0;
                }
            }
        } while (1);
    } while (ch != '\n' && ch != '\0' && ch!=EOF);

    double result;
    digits = pop_digit(digits, &result);
    printf("%.2lf", result);

    down(&operators, &digits, &digit);
    return 0;
}
