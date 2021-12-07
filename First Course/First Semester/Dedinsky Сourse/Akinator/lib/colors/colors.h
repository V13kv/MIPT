#ifndef COLORS_H
#define COLORS_H


#define RED                     "\033[0;31m"
#define GREEN                   "\033[0;32m"
#define YELLOW                  "\033[0;33m"
#define RESET                   "\033[0m"

#define TURN_TO_YELLOW(text)    YELLOW text RESET
#define TURN_TO_RED(text)       RED text RESET
#define TURN_TO_GREEN(text)     GREEN text RESET


#endif  // COLORS_H
