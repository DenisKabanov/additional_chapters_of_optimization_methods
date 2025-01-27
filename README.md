# Дополнительные главы методов оптимизации
Сборник лабораторных работ по предмету "Дополнительные главы методов оптимизации" (магистратура ИАД, 2 курс).

**Используемые языки:**
* C++
* CMake

**Содержание работ:**
1) ***Bounded knapsack problem***. Имеется N типов вещей со своими стоимостями, весами и доступным количеством для взятия. Необходимо собрать рюкзак наибольшей стоимости из представленных предметов.
2) ***Corruption problem (2-SAT)***. Имеется N участников соревнования и M судий. Каждый судья предоставляет вердикт, который имеет вид *±i*, где *+i* значит, что участник с номером *i* проходит в следующий этап, а *-i* значит, что участник с номером *i* не должен пройти в следующий этап. При этом каждый судья ожидает, что как минимум один из его вердиктов будет удовлетворён, иначе соревнование будет считаться купленным (формула 2-SAT не будет выполнена). Требуется проверить, разрешими ли требования всех судей одновременно (с дополнительным усложнением, что первый участник должен пройти ~ $x_1$ = 1).
3) ***Treasure problem***. Имеется *N* вещей со своими стоимостями и весами. Необходимо собрать рюкзак максимальной стоимости, при этом его суммарный вес должен быть в отрезке *[L, R]*.
4) ***Monotone problem***. Имеется *N* булевых векторов размерности *k*. Один вектор является предшествующим по отношению к другому, если они различны и каждая координата первого вектора не больше чем соответствующая координата второго. Функция называется монотонной, если ее значение на каждом векторе не меньше чем на любом предшествующем векторе. Требуется переопределить функцию в минимальном количество векторов, чтобы она стала монотонной.  