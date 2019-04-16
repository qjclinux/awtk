#ifndef PXIMAN_LOG_H
#define PXIMAN_LOG_H

#ifdef HAS_NO_STDIO
#define pixman_log(...)
#define pixman_logf(...)
#define stderr NULL
#define stdout NULL
#else
#include <stdio.h>
#define pixman_log printf
#define pixman_logf fprintf
#endif/*HAS_NO_STDIO*/

#endif/*PIXMAN_LOG_H*/

