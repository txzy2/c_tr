#ifndef BOOKS_H
#define BOOKS_H

#include "files.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct Book
 * @brief Структура, представляющая книгу.
 *
 * @var Book::id
 * Уникальный идентификатор книги.
 *
 * @var Book::title
 * Название книги (макс. 99 символов).
 *
 * @var Book::author
 * Имя автора (макс. 49 символов).
 *
 * @var Book::year
 * Год издания.
 *
 * @var Book::available
 * Доступна ли книга (true — да, false — нет).
 */
typedef struct
{
	int id;
	char title[100];
	char author[50];
	int year;
	bool available;
} Book;

/**
 * @struct Vector
 * @brief Динамический массив указателей на Book.
 *
 * @var Vector::b
 * Массив указателей на книги.
 *
 * @var Vector::size
 * Текущее количество элементов.
 *
 * @var Vector::capacity
 * Вместимость массива.
 */
typedef struct
{
	Book **b;
	size_t size;
	size_t capacity;
} Vector;

/**
 * @brief Инициализирует вектор с заданной начальной вместимостью.
 *
 * @param v        Указатель на вектор.
 * @param capacity Начальная вместимость (должна быть > 0).
 * @return true при успехе, false если v == NULL или capacity == 0.
 */
bool vector_init(Vector *v, size_t capacity);

/**
 * @brief Освобождает память, выделенную под вектор и все книги внутри.
 *
 * @param v Указатель на вектор.
 */
void free_vec(Vector *v);

/**
 * @brief Добавляет книгу в конец вектора.
 *
 * При нехватке места вместимость удваивается (реаллокация).
 *
 * @param v Указатель на вектор.
 * @param b Указатель на книгу.
 * @return true при успехе, false если v или b равны NULL.
 */
bool push_back(Vector *v, Book *b);

/**
 * @brief Выводит меню пользователю в stdout.
 */
void print_menu();

/**
 * @brief Обрабатывает ввод пользователя и выполняет соответствующее действие.
 *
 * @param input Указатель на выбор пользователя (1 — добавить, 2 — удалить, 0 — выход).
 * @param v     Указатель на вектор книг.
 * @return EXIT_SUCCESS при успехе, EXIT_FAILURE при ошибке.
 */
bool validate_input(const int *input, Vector *v);

/**
 * @brief Считывает целое число из stdin.
 *
 * @param input Указатель на переменную для сохранения результата.
 * @return true при успешном чтении, false при ошибке.
 */
bool read_input(int *input);

/**
 * @brief Удаляет книгу по ID из вектора и перезаписывает хранилище.
 *
 * @param v  Указатель на вектор.
 * @param id ID книги для удаления.
 * @return true если книга найдена и удалена, false если не найдена.
 */
bool delete_book(Vector *v, int id);

/**
 * @brief Создаёт книгу с указанными полями.
 *
 * @param id        Идентификатор книги.
 * @param title     Название книги.
 * @param author    Имя автора.
 * @param year      Год издания.
 * @param available Доступность книги.
 * @return Указатель на созданную книгу или NULL при ошибке выделения памяти.
 */
Book *create_book(int id, const char *title, const char *author, int year, bool available);

/**
 * @brief Записывает данные книги в файл хранилища.
 *
 * Формат строки: id;title;author;year;available
 *
 * @param b    Указатель на книгу.
 * @param mode Режим открытия файла ("w" — перезапись, "a" — дописывание).
 * @return true при успехе, false при ошибке форматирования или записи.
 */
bool move_into_storage(Book *b, const char *mode);

#endif
