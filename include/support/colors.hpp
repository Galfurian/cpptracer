/// @file   colors.hpp

#pragma once

/* FOREGROUND */
#define KRST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x KRST
#define FGRN(x) KGRN x KRST
#define FYEL(x) KYEL x KRST
#define FBLU(x) KBLU x KRST
#define FMAG(x) KMAG x KRST
#define FCYN(x) KCYN x KRST
#define FWHT(x) KWHT x KRST

#define BOLD(x) "\x1B[1m" x KRST
#define UNDL(x) "\x1B[4m" x KRST
