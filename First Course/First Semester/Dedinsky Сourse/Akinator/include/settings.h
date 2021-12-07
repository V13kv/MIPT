#ifndef AKINATOR_SETTINGS
#define AKINATOR_SETTINGS

#define READ_LINE_UNTIL_CORRECT_INPUT(readLineFunc) \
    do                                              \
    {                                               \
    } while (readLineFunc != EXIT_CODES::NO_ERRORS)            

#define treeNodeElem_t                          char
#define TO_STR(object)                          STRINGIFY(object)
#define CLEAR_CONSOLE_COMMAND                   "cls"
#define DEFAULT_CHAR_VALUE                      'E'
#define BRACKET_REGEXP                          " %1[{}]"
#define DEFAULT_LINE_READ_REGEXP(maxLineLength) " %" TO_STR(maxLineLength) "[^\n]%n"
#define OUTPUT_DUMP_FILE_EXTENSION              ".dot"
#define COMMAND_FOR_GRAPH_DUMP_CONSTRUCTION     "dot.exe -Tsvg %s -o %s.svg"
#define NODE_DUMP_W_BOTH_BRANCHES_MESSAGE       "\tNode_%p [shape=\"record\", label=\"{ <left> %p | %s | <right> %p }\"];\n"
#define NODE_DUMP_W_ONLY_LEFT_BRANCH_MESSAGE    "\tNode_%p [shape=\"record\", label=\"{ <left> %p | %s | <right> NaN }\"];\n"
#define NODE_DUMP_W_ONLY_RIGHT_BRANCH_MESSAGE   "\tNode_%p [shape=\"record\", label=\"{ <left> NaN | %s | <right> %p }\"];\n"
#define NODE_DUMP_WO_BRANCHES_MESSAGE           "\tNode_%p [shape=\"record\", label=\"{ %s }\"];\n"
#define NODE_LEFT_BRANCH_DUMP_MESSAGE           "\tNode_%p:<left> -> Node_%p [label=\"No\"];\n"
#define NODE_RIGHT_BRANCH_DUMP_MESSAGE          "\tNode_%p:<right> -> Node_%p [label=\"Yes\"];\n"
#define Y_CHAR                                  "Y"
#define N_CHAR                                  "N"
#define YES_NO_INPUT                            "[" TURN_TO_YELLOW(Y_CHAR) "es/" TURN_TO_YELLOW(N_CHAR) "o] >> "
#define GUESS_NODE_QUESTION_MESSAGE             "Is it %s?\n" YES_NO_INPUT
#define WINNING_MESSAGE                         "Yep, that is what i meant :)\n"  
#define QUESTION_INPUT_MESSAGE                  "Enter the question after %s question.\n[QUESTION] >> "
#define QUESTION_CREATE_MESSAGE                 "OK, do you want to create new question after %s question?\n" YES_NO_INPUT
#define ANSWER_INPUT_MESSAGE                    "Ohhh, what is the answer then?\n"
#define INCORRECT_GUESS_INPUT_CHOICE            "Did not understand you. Please, repeat once again.\n"
#define TREE_SEARCH_FOR_INPUT_MESSAGE           "What do you want to search for?\n[INPUT] >> "
#define MAX_TREE_NODE_VALUE_STR_LENGTH          50
#define MAX_LINE_STR_LENGTH                     150

const int POISON                                = -663;
const int MAX_STRING_READ_VALUE                 = 100;
const int DEFAULT_MEMSET_VALUE                  = 0;
const int MAX_COMMAND_LENGTH                    = 100;

const int NODE_HAS_NOT_ANY_BRANCHES             = 0b000;
const int NODE_HAS_LEFT_BRANCH                  = 0b100;
const int NODE_HAS_RIGHT_BRANCH                 = 0b001;

#endif  // AKINATOR_SETTINGS
