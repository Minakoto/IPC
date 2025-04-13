# Task 4

Написать программу, порождающую дочерний процесс и
использующую однонаправленный обмен данными.
Процесс-потомок генерирует случайные числа и отправляет родителю.
Родитель выводит числа на экран и в файл.
Количество чисел задается в параметрах запуска приложения.

## Make instructions

To compile the program run 'make' in cmd

The executable is named 'main'. To cpecify number of randomized elements, input the number as a command line argument.
For example './main 5' will randomize 5 numbers

## Execution examples

IN: './main -1'
OUT: 'Wrong input'

IN: './main 3'
OUT: '679.000 479.000 3378.000'