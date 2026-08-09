#ifndef COREUTILS_H
#define COREUTILS_H

/* Общая сигнатура для всех coreutils —
   совпадает с тем, что ожидает диспетчер команд BlShell */
typedef int (*coreutil_fn_t)(int argc, char **argv);

#endif // COREUTILS_H