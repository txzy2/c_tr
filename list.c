#include "types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Узел двусвязного списка.
// next/prev — связь в обе стороны, за счёт этого удаление и вставка
// произвольного узла (если есть указатель на него) выполняются за O(1).
struct Node
{
	i32 data;
	struct Node *next, *prev;
};

// Сам список — хранит указатели на оба конца (head/end), чтобы
// вставка/удаление с начала и с конца тоже были за O(1),
// без прохода по всему списку.
typedef struct
{
	i32 size;
	struct Node *head, *end;
} List;

// Добавляет значение в КОНЕЦ списка.
// Возвращает false, если malloc не смог выделить память (список не трогаем).
bool add_to_end_list(List *list, const i32 *val)
{
	struct Node *item = malloc(sizeof(struct Node));
	if (item == NULL)
	{
		return false;
	}
	item->data = *val;
	item->next = item->prev = NULL;

	if (list->end == NULL)
	{
		// Список был пустым — новый узел становится и head, и end одновременно.
		list->head = list->end = item;
	}
	else
	{
		// Обычный случай: подвешиваем новый узел после текущего end
		// и сдвигаем end на него.
		item->prev = list->end;
		list->end->next = item;
		list->end = item;
	}
	list->size++;
	return true;
}

// Добавляет значение в НАЧАЛО списка.
// Возвращает указатель на созданный узел (а не bool) — это специально
// сделано, чтобы потом можно было использовать этот узел, например,
// как аргумент для insert_after/delete_node, не делая повторный find_node.
// NULL при ошибке malloc.
struct Node *add_to_head_list(List *list, const i32 *val)
{
	struct Node *item = malloc(sizeof(struct Node));
	if (item == NULL)
	{
		return NULL;
	}
	item->data = *val;
	item->next = item->prev = NULL;

	if (list->head == NULL)
	{
		// Список был пустым.
		list->head = list->end = item;
	}
	else
	{
		// Новый узел становится head, старый head сдвигается вправо.
		item->next = list->head;
		list->head->prev = item;
		list->head = item;
	}
	list->size++;
	return item;
}

// Вставляет новый узел со значением val СРАЗУ ПОСЛЕ узла node.
//
//   До:     [node] <-> [node->next]
//   После:  [node] <-> [item] <-> [node->next]
//
// ВАЖНО: node->next = item; стоит ВНЕ if — это должно выполняться
// всегда, а не только когда у node был следующий элемент. Раньше здесь
// был баг: если убрать эту строку внутрь if (node->next != NULL),
// то при вставке ПОСЛЕ ХВОСТА (node == list->end) список рвался —
// list->end указывал на новый узел, но старый хвост не знал о нём
// (его next оставался NULL), и обход от head никогда не доходил до
// нового узла.
bool insert_after(List *list, struct Node *node, const i32 val)
{
	if (list == NULL || node == NULL)
	{
		return false;
	}
	struct Node *item = malloc(sizeof(struct Node));
	if (item == NULL)
	{
		// ВНИМАНИЕ: функция объявлена как bool, а тут return NULL.
		// NULL == 0 == false, так что по факту работает как return false,
		// но писать надо return false — так корректно и понятно для читателя.
		return false;
	}
	item->data = val;
	item->next = node->next; // забираем себе "хвост" от node
	item->prev = node;

	if (node->next != NULL)
	{
		// У node был следующий элемент — переключаем его prev на новый узел.
		node->next->prev = item;
	}
	node->next = item; // выполняется ВСЕГДА (см. комментарий выше про баг)

	if (node == list->end)
	{
		// Вставили после последнего элемента — новый узел теперь хвост.
		list->end = item;
	}
	list->size++;
	return true;
}

// Удаляет произвольный узел node из списка за O(1)
// (не ищем "предыдущий" отдельным проходом — у node уже есть prev).
// Корректно обрабатывает все случаи: node — голова, node — хвост,
// node — единственный элемент (голова и хвост одновременно),
// node — элемент где-то в середине.
bool delete_node(List *list, struct Node *node)
{
	if (list == NULL || list->head == NULL || node == NULL)
	{
		return false;
	}

	if (node == list->head)
	{
		// Удаляем голову — сдвигаем head на следующий узел.
		list->head = node->next;
		if (list->head != NULL)
		{
			list->head->prev = NULL;
		}
	}
	else
	{
		// node не голова => у него точно есть node->prev, безопасно
		// перелинковываем соседей друг на друга, "перепрыгивая" через node.
		node->prev->next = node->next;
		if (node->next != NULL)
		{
			node->next->prev = node->prev;
		}
	}

	if (node == list->end)
	{
		// Удаляемый узел был хвостом — сдвигаем end на предыдущий.
		// Если это был единственный элемент, node->prev == NULL,
		// и end корректно станет NULL.
		list->end = node->prev;
	}

	list->size--;
	free(node);
	return true;
}

// Удобная обёртка: удалить головной узел.
bool delete_first_node(List *list)
{
	if (list == NULL || list->head == NULL)
	{
		return false;
	}
	delete_node(list, list->head);
	return true;
}

// Удобная обёртка: удалить хвостовой узел.
bool delete_last_node(List *list)
{
	if (list == NULL || list->end == NULL)
	{
		return false;
	}
	delete_node(list, list->end);
	return true;
}

// Линейный поиск (O(n)) первого узла со значением val, начиная от head.
// Если значение встречается несколько раз — вернётся самое первое
// совпадение по ходу списка. NULL, если не нашли.
struct Node *find_node(const List *list, i32 val)
{
	struct Node *tmp = list->head;
	while (tmp != NULL)
	{
		if (tmp->data == val)
		{
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

// Отладочная печать: адреса head/end/size, затем каждый узел
// со своим адресом и адресами prev/next — удобно, чтобы глазами
// проверить, что связи выставлены правильно (симметрично).
void print_list_debug(const List *list)
{
	printf("head=%p end=%p size=%d\n", (void *)list->head, (void *)list->end, list->size);
	struct Node *cur = list->head;
	while (cur != NULL)
	{
		printf("  [%p] data=%d prev=%p next=%p\n", (void *)cur, cur->data, (void *)cur->prev, (void *)cur->next);
		cur = cur->next;
	}
}

// Освобождает ВСЕ узлы списка и обнуляет сам List (head/end/size).
// next_node сохраняется ДО free(cur), потому что после free
// читать поля cur (в т.ч. cur->next) — use-after-free (UB).
void free_list(List *list)
{
	struct Node *cur = list->head;
	while (cur != NULL)
	{
		struct Node *next_node = cur->next;
		free(cur);
		cur = next_node;
	}
	list->head = list->end = NULL;
	list->size = 0;
}

int main()
{
	List l = {.head = NULL, .end = NULL, .size = 0};
	const i32 val = 5, val2 = 10, val3 = 12;
	i8 result = 0;

	// Строим список: [12] <-> [10] <-> [5]
	add_to_end_list(&l, &val);
	struct Node *val_item2 = add_to_head_list(&l, &val2);
	add_to_head_list(&l, &val3);

	// Вставляем 7 после val_item2 (узел со значением 10):
	// [12] <-> [10] <-> [7] <-> [5]
	insert_after(&l, val_item2, 7);
	print_list_debug(&l);

	// Проверка граничного случая: вставка ПОСЛЕ ХВОСТА (l.end).
	// [12] <-> [10] <-> [7] <-> [5] <-> [100]
	insert_after(&l, l.end, 100);
	print_list_debug(&l);

	// Поиск значения 7.
	struct Node *el = find_node(&l, 7);
	if (el == NULL)
	{
		fprintf(stderr, "EL 7 NOT FOUND\n");
		result = 1;
		goto cleanup; // единая точка очистки — чтобы не забыть free_list
	}
	printf("FOUND EL: %p, VAL: %d\n", (void *)el, el->data);

	// Удаляем узел val_item2 (значение 10) по указателю — O(1).
	if (!delete_node(&l, val_item2))
	{
		fprintf(stderr, "ERROR REMOVE\n");
		result = 1;
		goto cleanup;
	}

	// Удаляем текущую голову списка (значение 12).
	delete_first_node(&l);
	print_list_debug(&l);

cleanup:
	// Единственное место, где освобождается память списка,
	// независимо от того, через какой goto сюда пришли (или дошли естественно).
	free_list(&l);
	return result;
}
