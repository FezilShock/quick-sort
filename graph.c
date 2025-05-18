#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Функция возвращающая динамическую строку
char *dynstring(char c)
{
    int l = 0;
    int size = 10;
    char *string = (char *)malloc(size * sizeof(char));
    char ch = getchar();
    ch = getchar();
    while (ch != c)
    {
        string[(l)++] = ch;
        if (l >= size)
        {
            size *= 2;
            string = (char *)realloc(string, size * sizeof(char));
        }
        ch = getchar();
    }
    string[l] = '\0';
    return string;
}

// Структура вершин графа
typedef struct n
{
    char val;
    struct n *next;
    int power;
} node;

// Структура, описывающая множество вершин
typedef struct
{
    node *first_node;
    node *now;
    int size;
} set;

node *moveSet(set *set, int n)
{
    int i;
    node *tmp = set->first_node;
    for (i = 0; i < n; i++)
        tmp = tmp->next;
    return tmp;
}

/**
 * Функция которая выделяет память под новую ноду и возвращает указатель на неё.
 * Параметр `val` используется для записи значения в ноду
 */
node *create_node(char val)
{
    node *new_node = (node *)malloc(sizeof(node));
    new_node->val = val;
    new_node->next = NULL;
    new_node->power = 0;
    return new_node;
}

// Функция выделяет память под множество вершин и возвращает указатель на множество
set *createSet(void)
{
    set *new_set = (set *)malloc(sizeof(set));
    new_set->first_node = NULL;
    new_set->size = 0;
    return new_set;
}

void swap(node *a, node *b)
{
    int tmpvar = a->val;
    int tmppower = a->power;
    a->val = b->val;
    a->power = b->power;
    b->val = tmpvar;
    b->power = tmppower;
}

int partition(set *arr, int l, int r)
{
    node *pivot = moveSet(arr, r);
    int i = l - 1;
    for (int j = l; j <= r - 1; j++)
    {
        if ((moveSet(arr, j)->power < pivot->power) || ((moveSet(arr, j)->val < pivot->val) && (moveSet(arr, j)->power == pivot->power)))
        {
            i++;
            swap(moveSet(arr, i), moveSet(arr, j));
        }
    }

    swap(moveSet(arr, i + 1), moveSet(arr, r));
    return i + 1;
}

void quicksort(set *arr, int l, int r)
{
    if (l < r)
    {
        int q = partition(arr, l, r);
        quicksort(arr, l, q - 1);
        quicksort(arr, q + 1, r);
    }
}

/*
 * Функция для проверки существования элемента
 * Возвращает NULL, если элемент не найден
 */
bool setSearch(set *set, double var)
{
    if (set->size)
    {
        node *curNode = set->first_node;
        while (curNode != NULL)
        {
            if (curNode->val == var)
            {
                curNode->power++;
                return curNode;
            }
            curNode = curNode->next;
        }
        return NULL;
    }
    else
        return NULL;
}

/*
 * Функция добавления элементов в множество вержин
 * Параметр `set` - указатель на множество
 * Параметр `new_var` - добавляемое значение
 */
void setPush(set *set, char new_var)
{
    if (new_var != '\n' && new_var != ' ' && new_var != EOF)
    {
        if (set->first_node == NULL)
        {
            node *tmp = create_node(new_var);
            set->first_node = tmp;
            set->now = tmp;
            ++set->size;
        }
        else
        {
            if (!setSearch(set, new_var))
            {
                node *tmp = create_node(new_var);
                set->now->next = tmp;
                set->now = tmp;
                ++set->size;
                setPush(set, new_var - 1);
            }
        }
    }
}

/*
 * Функция вывода множества вершин в файл
 * Через пробел выводит все элементы множества
 * Параметр `set` - указатель на множество
 * Параметр 'FILE' - указатель на файловый поток
 */
void setOutput(set *set)
{
    if (set->size)
    {
        node *curNode = set->first_node;
        while (curNode != NULL)
        {
            printf("%c %d\n", curNode->val, curNode->power);
            curNode = curNode->next;
        }
    }
}

/*
 * Функция освобождения памяти, занятой множеством вершин
 * Параметр `set` - указатель на множество
 */
void freeset(set *set)
{
    if (set->size)
    {
        node *curNode = set->first_node;
        while (curNode != NULL)
        {
            node *delNode = curNode;
            curNode = curNode->next;
            free(delNode);
        }
        free(curNode);
    }
    free(set);
}

/*
 * Функция чтения файла
 * Параметр `edges` - указатель на множество ребер
 * Параметр `tops` - указатель на множество вершин
 * Параметр `*fp` - указатель на файловый поток
 */
int readfile(FILE *fp, set *tops)
{
    char byte;
    if (fp == NULL)
    {
        return 1;
    }

    do
    {
        char ch1 = fgetc(fp);
        fgetc(fp);
        char ch2 = fgetc(fp);
        if (ch1 == ch2)
            setPush(tops, ch1);
        else
        {
            setPush(tops, ch1);
            setPush(tops, ch2);
        }
    } while ((byte = fgetc(fp)) != EOF);
    fclose(fp);
    return 0;
}

// Функция получения и сортировки графа
int gettops(void)
{
    set *tops = createSet();
    char *filename = dynstring('\n');

    char *filepath = (char *)malloc(sizeof(filename) * strlen(filename) * 2);
    strcat(filepath, "input/");
    strcat(filepath, filename);
    strcat(filepath, ".txt");

    FILE *fp = fopen(filepath, "r");
    if (readfile(fp, tops) == 0)
    {
        if (tops->size)
        {
            node *curNode = moveSet(tops, tops->size - 1);
            curNode->power++;
            puts("Unsorted tops:");
            setOutput(tops);
        }
        clock_t start, end;
        double cpu_time_used;
        start = clock();
        quicksort(tops, 0, tops->size - 1);
        end = clock();
        printf("Sort time = %fs\n", ((double)(end - start)) / CLOCKS_PER_SEC);
        if (tops->size)
        {
            node *curNode = tops->first_node;
            puts("Sorted tops:");
            setOutput(tops);
        }
    }
    else
    {
        perror(filepath);
        return 1;
    }
    freeset(tops);
    free(filepath);

    return 0;
}

void help(void)
{
    printf("Commands for work with program:\n1 - Get list of top from file.\n0 - Exit from program.\n");
}

int main(void)
{
    int menu = 0;
    int input;
    char *filename;

    help();
    do
    {
        menu = -1;
        puts("Input commands: ");
        input = scanf("%d", &menu);
        switch (menu)
        {
        case 1:
            system("cd input && ls");
            puts("Input filename without extention");
            gettops();
            break;
        case 0:

            return 0;
        case -1:
            puts("Input error!!");
            return 0;
        default:
            menu = 0;
            help();
        }
    } while (input);
    return 0;
}