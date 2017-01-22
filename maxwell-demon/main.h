#ifndef MAIN_H
#define MAIN_H

#define IN  (1)
#define OUT (0)
#define ON	(1)
#define OFF (0)

unsigned int __stdcall thread_wall_func1(void *a);
unsigned int __stdcall thread_wall_func2(void *a);
unsigned int __stdcall thread_wall_func3(void *a);
unsigned int __stdcall thread_func1(void *dmy);
unsigned int __stdcall thread_func2(void *dmy);
unsigned int __stdcall thread_func3(void *dmy);

#endif