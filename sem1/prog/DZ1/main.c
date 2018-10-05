/*
    Составить программу определения такого максимального подотрезка среди элементов заданного целочисленного массива,
    элементы которого увеличиваются по порядку на 1.
*/
#include <stdio.h>
#include <stdlib.h>

/* Стек на односвязном списке */
struct node {
    int data;
    struct node *next;
};

/* Добавляем элемент в стек */
struct node *push(struct node *head, int data) {
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
struct node *pop(struct node *head, int *element) {
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
    int element;
    while (!empty(head)) {
        head = pop(head, &element);
    }
    return head;
}

/* Смотрим последний элемент */
int peek(struct node *head) {
    if (empty(head)) {
        exit(0);
    }
    return head->data;
}

/* Ищем максимальный подотрезок и возвращает его длину, а сам отрезок через указатель-параметр */
size_t find_max_arr(const int arr[], size_t N, struct node **max) {
    struct node *buf = NULL;
    size_t size_buf = 1, size_max = 0;
    int current_buf;
    buf = push(buf, arr[0]);

    for (int i = 1; i < N; i++) {
        current_buf = peek(buf);
        /* если текущий элемент на 1 больше предыдущего */
        if (current_buf + 1 == arr[i]) {
            buf = push(buf, arr[i]);
            size_buf++;
        } else {
            /* Сравниваем размер макс стека и буф стека */
            if (size_max < size_buf) {
                *max = clear(*max);
                size_max = size_buf;
                /* Перекладываем элементы из буфера в макс, тем самым очищаем буфер */
                while (!empty(buf)) {
                    int element;
                    buf = pop(buf, &element);
                    *max = push(*max, element);
                }
            }
            buf = clear(buf);
            buf = push(buf, arr[i]);
            size_buf = 1;
        }
    }

    /* Если вся последовательность возрастала */
    if (size_max < size_buf) {
        *max = clear(*max);
        size_max = size_buf;
        while (!empty(buf)) {
            int element;
            buf = pop(buf, &element);
            *max = push(*max, element);
        }
    }

    /* Если ничего не нашли */
    if (size_max == 1) {
        *max = clear(*max);
        buf = clear(buf);
        free(buf);
        return 0;
    }
    buf = clear(buf);
    free(buf);
    return size_max;
}

/* Выводим ответ */
void print_answer(size_t max_len, struct node **answer) {
    printf("%lu\n", max_len);
    while (!empty(*answer)) {
        int element = 0;
        *answer = pop(*answer, &element);
        printf("%d ", element);
    }
}

/* Получаем входные данные */
int *get_data(size_t *N) {
    int input;
    int element;
    input = scanf("%d", &element);
    if (element <= 0 || !input) {
        return NULL;
    }
    *N = element;

    int *arr = (int *) malloc((*N) * sizeof(int));
    for (int i = 0; i < (*N); i++) {
        input = scanf("%d", &arr[i]);
        if (!input) {
            free(arr);
            return NULL;
        }
    }
    return arr;
}

void mem_free(struct node **answer, int **arr) {
    free(*answer);
    free(*arr);
}

int main() {
    size_t N = 0;

    /* Получаем входные данные */
    int *arr = get_data(&N);
    if (!arr) {
        printf("[error]");
        return 0;
    }

    struct node *answer = NULL;
    size_t max_len = 0;

    /* Получаем размер отрезка max_len и сам отрезок в answer */
    max_len = find_max_arr(arr, N, &answer);

    /* Выводим ответ */
    print_answer(max_len, &answer);

    /* Освобождаем память */
    mem_free(&answer, &arr);
    return 0;
}
