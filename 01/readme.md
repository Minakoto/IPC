# Task 1

Написать программу, вычисляющую площади квадратов с заданной
длиной стороны.
Длины сторон передаются как аргументы запуска.
Расчеты делают родительский и дочерний процессы, разделяя задачи
примерно поровну.

## Make instructions

To compile the program run 'make' in cmd

The executable is named 'main'. To cpecify size of needed square, input the size as a command line argument.
For example './main 50' will calculate area of a square 50x50 in size.


## Result examples

IN: './main 50'
OUT: 'Area: 2500'

IN: './main 600'
OUT: 'Area: 360000'

IN: './main'
OUT: 'No size specified'

IN: './main -1'
OUT: 'Wrong size'