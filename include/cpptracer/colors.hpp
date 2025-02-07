/// @file colors.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief ANSI colors.

#pragma once

namespace cpptracer
{
/// @brief ANSI escape codes.
namespace ansi
{
/// @brief Foreground colors.
namespace fg
{
// Normal colors.
constexpr const char *black          = "\33[30m"; ///< ANSI foreground color black.
constexpr const char *red            = "\33[31m"; ///< ANSI foreground color red.
constexpr const char *green          = "\33[32m"; ///< ANSI foreground color green.
constexpr const char *yellow         = "\33[33m"; ///< ANSI foreground color yellow.
constexpr const char *blue           = "\33[34m"; ///< ANSI foreground color blue.
constexpr const char *magenta        = "\33[35m"; ///< ANSI foreground color magenta.
constexpr const char *cyan           = "\33[36m"; ///< ANSI foreground color cyan.
constexpr const char *white          = "\33[37m"; ///< ANSI foreground color white.
// Bright colors.
constexpr const char *bright_black   = "\33[30;1m"; ///< ANSI foreground color bright black.
constexpr const char *bright_red     = "\33[31;1m"; ///< ANSI foreground color bright red.
constexpr const char *bright_green   = "\33[32;1m"; ///< ANSI foreground color bright green.
constexpr const char *bright_yellow  = "\33[33;1m"; ///< ANSI foreground color bright yellow.
constexpr const char *bright_blue    = "\33[34;1m"; ///< ANSI foreground color bright blue.
constexpr const char *bright_magenta = "\33[35;1m"; ///< ANSI foreground color bright magenta.
constexpr const char *bright_cyan    = "\33[36;1m"; ///< ANSI foreground color bright cyan.
constexpr const char *bright_white   = "\33[37;1m"; ///< ANSI foreground color bright white.
} // namespace fg
/// @brief Background colors.
namespace bg
{
// Normal colors.
constexpr const char *black          = "\33[40m"; ///< ANSI background color black.
constexpr const char *red            = "\33[41m"; ///< ANSI background color red.
constexpr const char *green          = "\33[42m"; ///< ANSI background color green.
constexpr const char *yellow         = "\33[43m"; ///< ANSI background color yellow.
constexpr const char *blue           = "\33[44m"; ///< ANSI background color blue.
constexpr const char *magenta        = "\33[45m"; ///< ANSI background color magenta.
constexpr const char *cyan           = "\33[46m"; ///< ANSI background color cyan.
constexpr const char *white          = "\33[47m"; ///< ANSI background color white.
// Bright colors.
constexpr const char *bright_black   = "\33[40;1m"; ///< ANSI background color bright black.
constexpr const char *bright_red     = "\33[41;1m"; ///< ANSI background color bright red.
constexpr const char *bright_green   = "\33[42;1m"; ///< ANSI background color bright green.
constexpr const char *bright_yellow  = "\33[43;1m"; ///< ANSI background color bright yellow.
constexpr const char *bright_blue    = "\33[44;1m"; ///< ANSI background color bright blue.
constexpr const char *bright_magenta = "\33[45;1m"; ///< ANSI background color bright magenta.
constexpr const char *bright_cyan    = "\33[46;1m"; ///< ANSI background color bright cyan.
constexpr const char *bright_white   = "\33[47;1m"; ///< ANSI background color bright white.
} // namespace bg
/// @brief Utility escape codes.
namespace util
{
constexpr const char *reset     = "\33[0m\033[49m"; ///< Reset all styles.
constexpr const char *bold      = "\33[1m";         ///< Turns on the bold text.
constexpr const char *underline = "\33[4m";         ///< Turns on the underline text.
constexpr const char *reverse   = "\33[7m";         ///< Turns on the reverse text.
constexpr const char *clear     = "\33[2J";         ///< Clears entire screen.
constexpr const char *clearline = "\33[2K";         ///< Clears entire line.
constexpr const char *up        = "\33[1A";         ///< Moves the cursor up by one position.
constexpr const char *down      = "\33[1B";         ///< Moves the cursor down by one position.
constexpr const char *right     = "\33[1C";         ///< Moves the cursor right by one position.
constexpr const char *left      = "\33[1D";         ///< Moves the cursor left by one position.
constexpr const char *nextline  = "\33[1E";         ///< Move cursor to beginning of line, 1 line down.
constexpr const char *prevline  = "\33[1F";         ///< Move cursor to beginning of line, 1 line up.
} // namespace util
} // namespace ansi
} // namespace cpptracer
