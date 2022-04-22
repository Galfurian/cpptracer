/// @file colors.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief ANSI colors.

#pragma once

namespace cpptracer
{

/* FOREGROUND */
#define KRST "\x1B[0m"  ///< Reset: turn off all attributes.
#define KBLD "\x1B[1m"  ///< Bold or bright.
#define KITA "\x1B[2m"  ///< Italic.
#define KUND "\x1B[4m"  ///< Underlined.

#define KRED "\x1B[31m" ///< Sets color to RED.
#define KGRN "\x1B[32m" ///< Sets color to GREEN.
#define KYEL "\x1B[33m" ///< Sets color to YELLOW.
#define KBLU "\x1B[34m" ///< Sets color to BLUE.
#define KMAG "\x1B[35m" ///< Sets color to MAGENTA.
#define KCYN "\x1B[36m" ///< Sets color to CYAN.
#define KWHT "\x1B[37m" ///< Sets color to WHITE.

#define FRED(x) (KRED x KRST) ///< Sets the color of the input text to RED.
#define FGRN(x) (KGRN x KRST) ///< Sets the color of the input text to GREEN.
#define FYEL(x) (KYEL x KRST) ///< Sets the color of the input text to YELLOW.
#define FBLU(x) (KBLU x KRST) ///< Sets the color of the input text to BLUE.
#define FMAG(x) (KMAG x KRST) ///< Sets the color of the input text to MAGENTA.
#define FCYN(x) (KCYN x KRST) ///< Sets the color of the input text to CYAN.
#define FWHT(x) (KWHT x KRST) ///< Sets the color of the input text to WHITE.

#define BOLD(x) (KBLD x KRST) ///< Sets the input text to bold.
#define ITAL(x) (KBLD x KRST) ///< Sets the input text to italic.
#define UNDL(x) (KUND x KRST) ///< Sets the input text to underlined.

} // namespace cpptracer