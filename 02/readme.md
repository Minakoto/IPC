# Task 2

Написать программу, похожую на командный интерпретатор.
При запуске выводится приглашение, и пользователь вводит имя и
аргументы программы, которую желает запустить.
Программы для запуска написать самостоятельно и поместить в тот же
каталог (например, программа для вычисления суммы аргументов,
«склеивания» строк, поиска наибольшего значения или наибольшей длины
строки и т.д.).

## Make instructions

To compile the program run 'make' in cmd

All available for execution functions are in 'exec' directory and are the first line of output.

Numbers of parameners needed for proper executions of funcitons:
'func' 'num_pars' 'arg1' 'arg2' '...'

'max {min} 2 a1 a2'
'rnd {min} 1'
'square 1 a1'
'sum {min} 2 a1 a2'

## Result examples

IN: 'sum 2 1 5'
OUT: 'Sum: 6.000'

IN: 'max 4 50 2 -1 20'
OUT: 'Max: 50.000'

IN: 'max 4 50 2 -1 20'
OUT: 'Max: 50.000'

IN: 'rnd 1 50'
OUT: 'Random number, enjoy):26'

IN: 'square 1 625'
OUT: '390625.000'

