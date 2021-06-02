//
//  gon125.h
//  TestDriver
//
//  Created by Geonhyeong LIm on 2021/05/05.
//

#ifndef gon125_h
#define gon125_h
#include <stdarg.h>
#include <stdlib.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#define BUF_SIZE 4096
#define ARG_MAX 50
#define REGEX_SIZE 10
#define COMMENT_REGEX 0
#define INT_REGEX 1
#define FLOAT_REGEX 2
#define DOUBLE_REGEX 3
#define CHARACTER_REGEX 4
#define POINTER_REGEX 5
#define ARRAY_REGEX 6
#define VOID_REGEX 7

typedef enum T {Int, Float, Double, Character, Void} T;

typedef struct Type {
    T Self;
    bool isPointer;
    bool isArray;
} Type;

typedef struct Function {
    char* name;
    int arg_count;
    Type return_type;
    Type arg_types[ARG_MAX];
} Function;

void guard(int, const char* fmt, ...);
void get_next_line(char* buff, int size, FILE* fp);
bool isComment(const char* string);
bool isMatching(const char* string, int regex_type);
void setup_regex(void);
Function get_function_from_function_prototype(const char* function_prototype);
Type string_to_type(const char*);
void type_to_string(char* buff, Type type);
char* name_without_extension(const char* string);
void append_conversion_function_of_type(char* buff, Type type);
char atoc(const char* string);

static const char* driver_file_name_suffix = "_driver.c";
static const char* include_string = "#include \"gon125.h\"";

static const char* main_header_string = "int main(int argc, const char * argv[]) {\n    FILE* fp_tc; char buff[BUF_SIZE];\n    const char* test_cases_file_name = argv[1];\n    fp_tc = fopen(test_cases_file_name, \"r\");\n    if (fp_tc == NULL) { fprintf(stderr, \"Usage: ./a.out test_cases_file_name\\n\"); exit(1); }\n    int tc_num;\n";
static const char* main_middle_string =  "    setup_regex();\n    // remove function prototype\n    if (!feof(fp_tc)) {\n        get_next_line(buff, BUF_SIZE, fp_tc);\n    }\n    while(!feof(fp_tc)) {\n        get_next_line(buff, BUF_SIZE, fp_tc);\n        char* token = strtok(buff, \" \");\n        tc_num = atoi(token);\n";
//"    setup_regex();\n
static const char* main_footer_string = "    }\n    fclose(fp_tc);\n    return 0;\n}";

static const char* ifelse_string = "printf(\"test case %d: pass\\n\", tc_num);\n        else printf(\"test case %d : Fail\\n\", tc_num);";

#endif /* gon125_h */
