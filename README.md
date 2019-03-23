﻿# Лабораторные работы по курсу "Разработка програмных систем"
# Преподаватель: Федорук Владимир Геннадьевич

Лабораторная работа №1: Многопроцессное программирование
Вариант №8

# Задание:
Разработать программу, моделирующую в реальном времени работу лифтовой системы десятиэтажного дома, состоящей из блока управления БУ, пассажирского ЛП и грузового ЛГ лифтов.
БУ отслеживает текущее состояние лифтов, принимает вызовы пассажиров на этажах, диспетчеризует движение лифтов (выбирает "ближайший", направляет к пассажирам, тормозит на промежуточных этажах и т.п.), индицирует состояние лифтов.
Лифты исполняют команды находящихся в них пассажиров и команды БУ. Скорость ЛП в два раза выше скорости ЛГ.
Работа каждого элемента системы (БН, ЛП и ЛГ) моделируется отдельным процессом.
Клавиши "1"..."0" используются для имитации клавиш вызова лифта на этажах здания. Следующий ряд клавиш "q"..."p" имитирует кнопки внутри ЛП, а ряд "a"...":" - внутри ЛГ. Для обработки нажатий клавиш использовать неканонический режим.
Визуализация перемещения лифтов може выглядить, например, след. образом.

ЛП - - - - 2 - - - - -

ЛГ - - - - - - - - 0 -
Здесь цифры индицируют количество пассажиров в лифте.

