//
//  gon125.c
//  TestDriver
//
//  Created by Geonhyeong LIm on 2021/05/05.
//

#include "gon125.h"

static regex_t regex[REGEX_SIZE];

void setup_regex() {
    regcomp(&regex[COMMENT_REGEX], "\\/\\*.*\\*\\/", 0);
    regcomp(&regex[INT_REGEX], "int", 0);
    regcomp(&regex[FLOAT_REGEX], "float", 0);
    regcomp(&regex[DOUBLE_REGEX], "double", 0);
    regcomp(&regex[CHARACTER_REGEX], "char", 0);
    regcomp(&regex[POINTER_REGEX], "\\*", 0);
    regcomp(&regex[ARRAY_REGEX], "\\[.*\\]", 0);
    regcomp(&regex[VOID_REGEX], "void", 0);
}

void guard(int result, const char* fmt, ...) {
    va_list args;
    if (result == 0) {
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fprintf(stderr, "\n");
        exit(1);
    }
}

void get_next_line(char* buff, int size, FILE* fp) {
    if (feof(fp)) { return; }
    do {
        fgets(buff, BUF_SIZE, fp);
    } while (!feof(fp) && isComment(buff));
}

bool isComment(const char* string) {
    return isMatching(string, COMMENT_REGEX);
}

bool isMatching(const char* string, int regex_type) {
    if (!regexec(&regex[regex_type], string, 0, NULL, 0)) { return true; }
    else return false;
}

Function get_function_from_function_prototype(const char* function_prototype) {
    Function f;
    char* function_pt = strdup(function_prototype);
    int count = 0;
    char* return_type_string = strtok(function_pt, " ");
    f.return_type = string_to_type(return_type_string);
    f.name = strdup(strtok(NULL, "("));
    char* args_string = strtok(NULL, ")");
    char* arg_string = strtok(args_string, ",");
    while (arg_string != NULL) {
        f.arg_types[count] = string_to_type(arg_string);
        arg_string = strtok(NULL, ",");
        count++;
    }
    f.arg_count = count;
    free(function_pt);
    return f;
}

void type_to_string(char* buff, Type type) {
    switch (type.Self) {
        case Int:
            sprintf(buff+strlen(buff), "int");
            break;
        case Float:
            sprintf(buff+strlen(buff), "float");
            break;
        case Double:
            sprintf(buff+strlen(buff), "double");
            break;
        case Character:
            sprintf(buff+strlen(buff), "char");
            break;
        default:
            break;
    }
    if (type.isPointer) { sprintf(buff+strlen(buff), "*"); }
}

Type string_to_type(const char* string) {
    Type t;
    if (isMatching(string, INT_REGEX)) {
        t.Self = Int;
    } else if (isMatching(string, FLOAT_REGEX)) {
        t.Self = Float;
    } else if (isMatching(string, DOUBLE_REGEX)) {
        t.Self = Double;
    } else if (isMatching(string, CHARACTER_REGEX)) {
        t.Self = Character;
    } else {
        t.Self = Void;
    }
    if (isMatching(string, POINTER_REGEX)) {
        t.isPointer = true;
    } else t.isPointer = false;
    if (isMatching(string, ARRAY_REGEX)) {
        t.isArray = true;
    } else t.isArray = false;
    return t;
}

void append_conversion_function_of_type(char* buff, Type type) {
    switch (type.Self) {
        case Int:
            sprintf(buff+strlen(buff), "atoi");
            break;
        case Float:
        case Double:
            sprintf(buff+strlen(buff), "atof");
            break;
        case Character:
            sprintf(buff+strlen(buff), "atoc");
            break;
        default:
            break;
    }
}

char atoc(const char* string) {
    return string[0];
}

char* name_without_extension(const char* string) {
    char* str = strdup(string);
    char* token;
    token = strtok(str, ".");
    return token;
}
