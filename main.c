//
//  main.c
//  TestDriverGenerator
//
//  Created by Geonhyeong Lim on 2021/05/03.
//
#include "gon125.h"

void make_driver_file(FILE* driver, FILE* tc, FILE* sut, const char*, const char*, char*);
void make_shell_script(const char*, const char*, const char*, const char*);
void run_shell_script(const char*);
void print_test_results_report(void);

int main(int argc, const char * argv[]) {
    FILE *fp_tc = NULL, *fp_driver = NULL, *fp_sut = NULL;
    const char* test_cases_file_name = argv[1];
    const char* sut_source_file_name = argv[2];
    const char* shell_script_name = "run_test.sh";
    char driver_file_name[FILENAME_MAX];
    
    setup_regex();
        
    make_driver_file(fp_driver,
                     fp_tc,
                     fp_sut,
                     test_cases_file_name,
                     sut_source_file_name,
                     driver_file_name);
    fclose(fp_tc); fclose(fp_sut); fclose(fp_driver);
    fflush(fp_driver);
    make_shell_script(test_cases_file_name,
                      shell_script_name,
                      sut_source_file_name,
                      driver_file_name);
    run_shell_script(shell_script_name);
    print_test_results_report();
    
    
    return 0;
}

void make_driver_file(FILE* fp_driver, FILE* fp_tc, FILE* fp_sut, const char* test_cases_file_name, const char* sut_source_file_name, char* driver_file_name) {
    // variables
    char buff[BUF_SIZE];
    char* sut_name = NULL;

    // file connection
    fp_tc = fopen(test_cases_file_name, "r");
    fp_sut = fopen(sut_source_file_name, "r");
    guard(fp_tc != NULL && fp_sut != NULL, "Usage: tgen test_cases_file.txt sut_source_file.c");
    
    // naming driver file
    sut_name = name_without_extension(sut_source_file_name);
    guard(sut_name != NULL, "File Extension Not Supported: %s", sut_source_file_name);
    strncpy(driver_file_name, sut_name, FILENAME_MAX-strlen(driver_file_name_suffix)-1);
    strcat(driver_file_name, driver_file_name_suffix);
    fp_driver = fopen(driver_file_name, "w");
    guard(fp_driver != NULL, "Couldn't Make Driver File: %s", driver_file_name);
    
    // make function prototype from tc file
    if (!feof(fp_tc)) {
        get_next_line(buff, BUF_SIZE, fp_tc);
    }
    Function function = get_function_from_function_prototype(buff);
    
    // write to driver source file
    fprintf(fp_driver, "%s\n", include_string);
    fprintf(fp_driver, "%s\n", buff);
    fprintf(fp_driver, "%s", main_header_string);
    
    // write arg declarations
    sprintf(buff, "    ");
    for (int i = 0; i < function.arg_count; i++) {
        type_to_string(buff, function.arg_types[i]);
        sprintf(buff+strlen(buff), " a%d; ", i+1);
    }
    fprintf(fp_driver, "%s\n", buff);
    
    // write expected_output declaration
    sprintf(buff, "    ");
    type_to_string(buff, function.return_type);
    sprintf(buff+strlen(buff), " expected_output;\n");
    fprintf(fp_driver, "%s\n", buff);
    
    // setup and read code
    fprintf(fp_driver, "%s", main_middle_string);
    
    // args assignment
    sprintf(buff, "");
    for (int i = 0; i < function.arg_count; i++) {
        sprintf(buff+strlen(buff), "        a%d = ", i+1);
        append_conversion_function_of_type(buff, function.arg_types[i]);
        sprintf(buff+strlen(buff), "(strtok(NULL, \" \"));\n");
    }
    fprintf(fp_driver, "%s", buff);
    
    // expected_output assignment
    sprintf(buff, "        expected_output = ");
    append_conversion_function_of_type(buff, function.return_type);
    sprintf(buff+strlen(buff), "(strtok(NULL, \" \"));\n");
    fprintf(fp_driver, "%s\n", buff);
    
    // test if else statement
    sprintf(buff, "        if ( ");
    sprintf(buff+strlen(buff), "%s(", function.name);
    for (int i = 0; i < function.arg_count-1; i++) {
            sprintf(buff+strlen(buff), "a%d, ", i+1);
    }
    sprintf(buff+strlen(buff), "a%d) == expected_output ) %s", function.arg_count, ifelse_string);
    fprintf(fp_driver, "%s\n", buff);
    
    fprintf(fp_driver, "%s", main_footer_string);
}

void make_shell_script(const char* test_cases_file_name,
                       const char* shell_scrpit_name,
                       const char* sut_source_name,
                       const char* driver_file_name) {
    static const char* compile_string = "gcc gon125.c %s %s\n./a.out %s";
    //"gcc -o main.o -c %s\ngcc -o sut.o -c %s\ngcc -c gon125.c\ngcc -o a.out main.o gon125.o sut.o\n";
    FILE* fp_sh = fopen(shell_scrpit_name, "w");
    guard(fp_sh != NULL , "Failed to Make Script File: %s", shell_scrpit_name);
    fprintf(fp_sh, compile_string, sut_source_name, driver_file_name, test_cases_file_name);
    //, name_without_extension(sut_source_name), name_without_extension(driver_file_name));
    fclose(fp_sh);
}

void run_shell_script(const char* shell_scrpit_name) {
    char buff[BUF_SIZE];
    sprintf(buff,"./%s", shell_scrpit_name);
    chmod(buff, 0777);
    system(buff);
}

void print_test_results_report() {
    // running shell automatically prints the result.
}
