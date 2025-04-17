# Task 5

Изменить программу 04 так, чтобы дочерний процесс выводил
информацию из файла.
Если родительский процесс собирается изменить файл, то он
отправляет сигнал SIGUSR1 (блокировка доступа к файлу).
Когда родительский процесс завершил модификацию файла, он
отправляет сигнал SIGUSR2 (разрешение доступа к файлу).
Дочерний процесс отправляет новое число родительскому после того,
как прочитал файл.

## Make instructions

To compile the program run 'make' in cmd

The executable is named 'main'. To cpecify number of randomized elements, input the number as a command line argument.
For example './main 5' will randomize 5 numbers
Items are wtitten in 'ttt.txt', file access logs are written in 'log.txt'

## Execution examples

IN: './main -1'
OUT: 'Wrong input'

IN: './main 3'
OUT: '679.000 479.000 3378.000'