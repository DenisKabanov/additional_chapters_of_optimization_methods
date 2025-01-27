# Лабораторная работа #3

## Task:
Дочь короля Флатландии собирается выйти за прекрасного принца. Принц хочет подарить принцессе сокровища, но он не уверен какие именно бриллианты из своей коллекции выбрать.

В коллекции принца ***n*** бриллиантов, каждый характеризуется весом $w_i$ и стоимостью $v_i$. Принц хочет подарить наиболее дорогие бриллианты, однако король умен и не примет бриллиантов суммарного веса больше **R**. С другой стороны, принц будет считать себя жадным всю оставшуюся жизнь, если подарит бриллиантов суммарным весом меньше **L**.

Помогите принцу выбрать набор бриллиантов наибольшей суммарной стоимости, чтобы суммарный вес был в отрезке ***[L,R]***.

***Формат ввода:***
Первая строка содержит число ***n*** (1≤n≤32), ***L*** и ***R*** (0≤L≤R≤$10^{18}$).
Следующие ***n*** строк описывают бриллианты и содержат по два числа $w_i$ и $v_i$ — вес и стоимость соответствующего бриллианта (1≤$w_i$, $v_i$≤$10^{15}$).

***Формат вывода:***
Первая строка вывода должна содержать ***k*** — количество бриллиантов, которые нужно подарить королю. Вторая строка должна содержать номера даримых бриллиантов.
Бриллианты нумеруются от *1 до n* в порядке появляние во входных даных. Если составить подарок королю невозможно, то выведите 0 в первой строке вывода.