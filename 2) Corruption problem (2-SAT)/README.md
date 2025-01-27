# Лабораторная работа #2

## Task:
В четвертьфинале чемпионата мира по акробатической кулинарии выступают N участников. Каждому участнику предоставляется 15 минут, чтобы впечатлить судей. После выступления всех участников каждый судья выносит **ровно два вердикта**. Каждый вердикт имеет вид *±i*, где *+i* значит, что участник с номером *i* проходит в следующий этап, а *-i* значит, что участник с номером *i* не должен пройти в следующий этап.

Вердикты всех судей поступают почётному президенту соревнований. На их основании он принимает решение, кто из участников проходит в следующий этап, а кто — нет. При этом каждый судья ожидает, что как минимум один из его вердиктов будет удовлетворён. Если это не так, судья решает, что результаты соревнований куплены, устраивает скандал, и никто из участников не проходит в следующий этап. Например, пусть вердиктами первого судьи были *+1* и *-2*, вердиктами второго — *-1* и *-2*, а почётный президент объявил, что в следующий этап прошли участники *1* и *2*. Тогда первый судья ничего не заподозрит, так как один из его вердиктов выполнен (1-й участник прошёл дальше), а второй устроит скандал, потому что, по его мнению, ни первый, ни второй участники не должны были попасть в следующий этап.

В этом году в соревнованиях **под номером 1 выступает племянница почётного президента, и он очень хочет, чтобы она прошла в следующий этап**. Помогите ему узнать, может ли он огласить такой список прошедших в следующий этап, чтобы в него вошёл участник под номером 1, и ни один судья не устроил скандал.

***Формат ввода:***
Первая строка ввода содержит два целых числа **N** и **M** — число участников и судей соответственно (2 ≤ N ≤ 1000, 1 ≤ M ≤ 2000). Каждая из следующих M строк содержит по два целых числа a и b (1 ≤ |a|, |b| ≤ N, |a| ≠ |b|) — вердикты судей. Положительное число x означает, что участник с номером x должен пройти в следующий этап, отрицательное число x означает, что участник с номером |x| не должен пройти в следующий этап.

***Формат вывода:***
Выведите "yes", если почётный президент может огласить такой список прошедших в следующий этап, чтобы в него вошёл участник под номером 1, и ни один судья не устроил скандал. В противном случае выведите "no".