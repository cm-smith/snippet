/* ========================================================================== */
/* File: utils.h
 *
 * Project name: Snippet
 *
 * This file contains the common defines and data structures.
 *
 */
/* ========================================================================== */
#ifndef UTILS_H
#define UTILS_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stdio.h>                           // fprintf
#include <stdlib.h>                          // exit

// ---------------- Constants
#define HELP_OUTPUT "snippet is a tool for tracking accomplishments\n\n" \
	"Arguments:\n" \
        "    -p, --project  Specify the project linked to the accomplishment\n" \
        "                   If no project is specified, 'Miscellaneous' is used\n" \
        "    -a, --add      Add an accomplishment entry\n" \
        "    -e, --edit     Edit an accomplishment or its corresponding project\n" \
        "    -d, --delete   Delete an accomplishment entry\n" \
        "\nOptional arguments:\n" \
        "    --project-list Print list of projects that have currently been tracked\n" \
        "    --help         Show this help message\n" \
        "\nExample usage:\n" \
        "    snippet -p 'My project' -a 'Task completed :)'\n" \

// ---------------- Structures/Types

// ---------------- Public Variables

// ---------------- Prototypes/Macros

// Print s with the source file name and the current line number to fp.
#define LOG(fp,s)  fprintf((fp), "Log: [%s:%d] %s.\n", __FILE__, __LINE__, (s))

// Print error to fp and exit if s evaluates to false.
#define ASSERT_FAIL(fp,s) do {                                          \
        if(!(s))   {                                                    \
            fprintf((fp), "Error: [%s:%d] assert failed.\n", __FILE__, __LINE__); \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
    } while(0)

// Print error to fp and exit if s is NULL.
#define MALLOC_CHECK(fp,s)  do {                                        \
        if((s) == NULL)   {                                             \
            fprintf((fp), "Error: [%s:%d] out of memory.\n", __FILE__, __LINE__); \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
    } while(0)

// Dynamically allocate an input string
char* inputString(FILE* fp, size_t size);

#endif // UTILS_H
