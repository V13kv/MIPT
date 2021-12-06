#ifndef SETTINGS_H
#define SETTINGS_H


#define treeNodeElem_t                          char
#define TO_STR(object)                          STRINGIFY(object)
#define CLEAR_CONSOLE_COMMAND                   "cls"
#define DEFAULT_CHAR_VALUE                      'E'
#define BRACKET_REGEXP                          " %1[{}]"
#define OUTPUT_DUMP_FILE_EXTENSION              ".dot"
#define COMMAND_FOR_GRAPH_DUMP_CONSTRUCTION     "dot.exe -Tsvg %s -o %s"
#define DEFAULT_LINE_READ_REGEXP(maxLineLength) " %" TO_STR(maxLineLength) "[^\n]%n"
#define NODE_DUMP_W_LEFT_BRANCH_REGEXP          "\tNode_%p [shape=\"record\", label=\"{ <left> %p | %s | <right> NaN }\"];\n"
#define NODE_DUMP_W_RIGHT_BRANCH_REGEXP         "\tNode_%p [shape=\"record\", label=\"{ <left> NaN | %s | <right> %p }\"];\n"
#define NODE_DUMP_WO_BRANCHES_REGEXP            "\tNode_%p [shape=\"record\", label=\"{ %s }\"];\n"
#define NODE_LEFT_BRANCH_DUMP_REGEXP            "\tNode_%p:<left> -> Node_%p [label=\"No\"];\n"
#define NODE_RIGHT_BRANCH_DUMP_REGEXP           "\tNode_%p:<right> -> Node_%p [label=\"Yes\"];\n"
#define MAX_TREE_NODE_VALUE_STR_LENGTH          50
#define MAX_LINE_STR_LENGTH                     150

const int POISON                                = -663;
const int MAX_STRING_READ_VALUE                 = 100;
const int DEFAULT_MEMSET_VALUE                  = 0;
const int MAX_COMMAND_LENGTH                    = 100;

const int NODE_HAS_NOT_ANY_BRANCHES             = 0b000;
const int NODE_HAS_LEFT_BRANCH                  = 0b100;
const int NODE_HAS_RIGHT_BRANCH                 = 0b001;

#endif  // SETTINGS_H
