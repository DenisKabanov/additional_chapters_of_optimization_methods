# Лабораторная работа #4

## Task:
Пусть есть некоторая булевая функция, определенная на некотором множестве булевых век- торов размерности K. Один вектор является предшествующим по отношению к другому, если они различны и каждая координата первого вектора не больше чем соответствующая координата второго. Функция называется монотонной, если ее значение на каждом векторе не меньше чем на любом предшествующем векторе.

Требуется переопределить функцию в минимальном количество векторов, чтобы она стала монотонной.

***Формат ввода:***
Два числа ***N*** и ***k*** (*1 ≤ N ≤ 1000, 1 ≤ K ≤ 10*) — количество векторов и размерность. Далее ***N*** строк по ***k + 1*** числу в каждой — булевый вектор и значение функции в нем. Все числа — 0 или 1. Все вектора различны.

***Формат вывода:***
В первой строке минимальное количество векторов, значение функции в которых необходимо изменить, чтобы она стала монотонной. Во второй строке номера этих векторов без повторений в любом порядке.