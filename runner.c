/*
 * ============================================================
 *   MINI-C COMPILER — Project Runner
 *   File: runner.c
 *   Purpose: Interactive terminal interface to manage and
 *            execute compiler phases and test cases.
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#define CLEAR_SCREEN "cls"
#else
#include <unistd.h>
#define SLEEP(ms) usleep((ms) * 1000)
#define CLEAR_SCREEN "clear"
#endif

/* ── Configuration ── */
#define TEST_DIR "test"
#define COMPILER_EXE "minicc.exe"
#define MAX_TEST_FILES 100

/* ── ANSI Color Codes ── */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

/* ── Global State ── */
char test_files[MAX_TEST_FILES][256];
int test_file_count = 0;

/* ── Helper: Sort filenames ── */
int compare_names(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

/* ──────────────────────────────────────────────
 * Helper: Load all .c files from test directory
 * ────────────────────────────────────────────── */
void load_test_files() {
    DIR *d;
    struct dirent *dir;
    d = opendir(TEST_DIR);
    test_file_count = 0;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            /* Only match .c files, skip hidden files */
            if (dir->d_name[0] != '.' && strstr(dir->d_name, ".c")) {
                strncpy(test_files[test_file_count], dir->d_name, 255);
                test_file_count++;
            }
        }
        closedir(d);
    }
    /* Sort alphabetically for a professional list */
    if (test_file_count > 1) {
        qsort(test_files, test_file_count, sizeof(test_files[0]), compare_names);
    }
}

/* ──────────────────────────────────────────────
 * Helper: Print professional menu header
 * ────────────────────────────────────────────── */
void print_menu_header() {
    system(CLEAR_SCREEN);
    printf(BOLD CYAN "==========================================================\n");
    printf("                MINI-C COMPILER RUNNER                    \n");
    printf("==========================================================\n" RESET);
}

/* ──────────────────────────────────────────────
 * Helper: Execute compiler with specific target
 * ────────────────────────────────────────────── */
void run_compiler(const char *filename, int phase) {
    char command[512];
    sprintf(command, "%s %s/%s %d", COMPILER_EXE, TEST_DIR, filename, phase);
    
    printf(BOLD YELLOW "\n[EXECUTING] %s\n" RESET, command);
    printf("----------------------------------------------------------\n");
    
    int result = system(command);
    
    if (result == 0) {
        printf(BOLD GREEN "\n[SUCCESS] Compilation & Execution Finished.\n" RESET);
    } else {
        printf(BOLD RED "\n[ERROR] Compiler found errors (Exit Code: %d).\n" RESET, result);
    }
    
    printf("\nPress Enter to return to file list...");
    while(getchar() != '\n'); // Clear buffer
    getchar(); // Wait for user
}

/* ──────────────────────────────────────────────
 * Main Interactive Loop
 * ────────────────────────────────────────────── */
int main() {
    int choice;

    while (1) {
        load_test_files();
        print_menu_header();
        
        printf(BOLD "AVAILABLE TEST FILES:\n" RESET);
        printf("----------------------------------------------------------\n");
        
        if (test_file_count == 0) {
            printf(RED " No .c files found in '%s/' directory.\n" RESET, TEST_DIR);
        } else {
            for (int i = 0; i < test_file_count; i++) {
                printf(" %2d. %-25s", i + 1, test_files[i]);
                if ((i + 1) % 2 == 0) printf("\n");
            }
            if (test_file_count % 2 != 0) printf("\n");
        }
        
        printf("\n " RED " 0. Exit Program\n" RESET);
        printf("----------------------------------------------------------\n");
        printf("Enter the test file number to run (1-%d): ", test_file_count);

        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // Clear buffer
            continue;
        }

        if (choice == 0) break;

        if (choice >= 1 && choice <= test_file_count) {
            /* For demonstration, we run the FULL COMPILER (Phase 6) by default */
            run_compiler(test_files[choice - 1], 6);
        } else {
            printf(RED "Invalid selection. Please choose a number from the list.\n" RESET);
            SLEEP(1000);
        }
    }

    printf(BOLD CYAN "\nThank you for using Mini-C Compiler Runner. Goodbye!\n" RESET);
    return 0;
}
