/* ========================================================================== */
/* File: snippet.c - Snippet note tracker
 *
 * Author: Michael Smith
 * Date: 2/1/2020
 *
 * Description: Light-weight note tracking app in the command line
 * for when you're journaling on-the-go.
 * 
 * Usage:
 *
 * 	`snippet -p "My project" -a "My note"`
 * 	  Append note specific to a specific project; if no project is
 * 	  specified, "Miscellaneous" is used by default
 *
 * 	`snippet -e <entry_id>`
 * 	  Edit entry, either changing the project or note
 *
 * 	`snippet --list-projects`
 * 	  List projects that have been documented
 *
 * 	`snippet -p "My project"`
 * 	  List historic notes for a specific project
 * 
 * TODO:
 *   [ ] Configuration YAML file for default project name and file
 *
 *   [ ] List recent notes
 *      `snippet --recent`
 *      List recent notes
 *
 *   [ ] Actually edit the file entry
 *      --> Parse out function for "affect file" and "read file", where the
 *      former will rewrite the file and the latter will just analyze the file
 *
 *   [ ] Delete entry
 * 	`snippet -d <entry_id>`
 * 	  Delete entry
 *
 *   [ ] Configure writing to 'snippet.txt' anywhere in the terminal
 *
 * 	 [x] Append to file
 *
 * 	 [x] Require ~at least one~ argument when calling the program
 *
 * 	 [x] List projects:
 *
 * 	 [x] List historic notes:
 *
 * 	 [x] Clean up hash code (abstract hash function out of hash table)
 *
 * 	 [x] Create long options with getopt and create clean help message
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>               // printf
#include <stdlib.h>			     // EXIT_FAILURE, realpath
#include <string.h>			     // memset, strdup
#include <getopt.h>			     // getopt
#include <time.h>			     // time
#include <ctype.h>			     // tolower

// ---------------- Local includes  e.g., "file.h"
#include "utils.h"               // HELP_OUTPUT, inputString
#include "hashtable.h"			 // JenkinsHash

// ---------------- Constant definitions
#define OUTPUT_FILE "~/Documents/Code/snippet/snippet.txt"
#define DEFAULT_PROJECT "Miscellaneous"

// ---------------- Macro definitions
#define SAFEFREE(a) free(a); a=NULL;

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
void makeEntry(char *accomplish, char *project);
void readFile(char* match, int(*lineAction)(char *));
int editLine(char* line);
int deleteLine(char* line);
int projectNotes(char* line);
void projectList();

/* ========================================================================== */

int main(int argc, char **argv) {

	// Print helpful output if the program is called with no input
	if (argc == 1) {
		printf("%s", HELP_OUTPUT);
		return EXIT_FAILURE;
	}

	// getopt arguments
	int c;
	opterr = 0;

	// Snippet variable initialization
	char *project;
	char *accomplish;
	char *hash_id;

	// Flags for functionality
	int entry = 0, group = 0;

	// https://azrael.digipen.edu/~mmead/www/Courses/CS180/getopt.html
	while (1) 
	{
		int option_index = 0;
		static struct option long_options[] = 
		{
			{"project", required_argument, NULL, 'p'},
			{"add",     required_argument, NULL, 'a'},
			{"edit",    required_argument, NULL, 'e'},
			{"delete",  required_argument, NULL, 'd'},
			{"project-list", no_argument,  NULL, 'x'},
			{"help",    no_argument,       NULL, '?'},
			{NULL,      0,                 NULL,  0 }
		};

		c = getopt_long(argc, argv, ":p:a:e:d:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) 
		{
			case 'p':
				project = strdup(optarg);
				group = 1;
				break;

			case 'a':
				accomplish = strdup(optarg);
				entry = 1;
				break;

			case 'e':
				hash_id = strdup(optarg);
				readFile(hash_id, editLine);
				return EXIT_SUCCESS;

			case 'd':
				hash_id = strdup(optarg);
				readFile(hash_id, deleteLine);
				return EXIT_SUCCESS;

			case 'x':
				// Print list of projects
				projectList();
				return EXIT_SUCCESS;

			case '?':
				printf("%s", HELP_OUTPUT);
				return EXIT_FAILURE;

			case ':':
				printf("Missing argument for %c\n", optopt);
				return EXIT_FAILURE;

			default:
				printf("?? getopt returned character code 0%o ??\n", c);
		}
	}

	/* Create note */
	if ((entry == 0) && (group == 1)) {
	  	//fprintf(stderr, "Error receiving project with no note\n"
		//        "snippet -p 'My project' -a 'My note'\n");
		readFile(project, projectNotes);
		SAFEFREE(project);
		//return EXIT_FAILURE;
	}
	else if (entry == 1) {
		if (group == 0) project = DEFAULT_PROJECT;
		makeEntry(accomplish, project);
	}

	if (entry == 1) SAFEFREE(accomplish);
	if (group == 1) SAFEFREE(project);

	return EXIT_SUCCESS;
}

/* write the file to target directory using specified file_name and webpage */
void makeEntry(char *accomplish, char *project) {
 	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	long hash_id = JenkinsHash(accomplish, MAX_HASH_SLOT);
	printf("%lu (%d-%02d-%02d) %s: %s\n", hash_id, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, project, accomplish);

	// Print to config file
	FILE *fp = fopen(OUTPUT_FILE, "a+");

	if (!fp) {
		fprintf(stderr, "Error opening file '%s'\n", OUTPUT_FILE);
		return;
	}

	fprintf(fp, "%lu (%d-%02d-%02d) %s: %s\n", hash_id, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, project, accomplish);
	fclose(fp);
}

void readFile(char* match, int(*lineAction)(char*)) {
	// https://riptutorial.com/c/example/8274/get-lines-from-a-file-using-getline--
	/* Open the file for reading */
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	ssize_t line_size;

	FILE *fp = fopen(OUTPUT_FILE, "r");

	if (!fp) {
		fprintf(stderr, "Error opening file '%s'\n", OUTPUT_FILE);
		return;
	}

	/* Get the first line of the file. */
	line_size = getline(&line_buf, &line_buf_size, fp);

	int continue_loop = 1;

	/* Loop through until we are done with the file. */
	while ((line_size >= 0) && (continue_loop)) {

		// https://stackoverflow.com/a/6389959
		if (strstr(line_buf, match)) {
			continue_loop = (*lineAction)(line_buf);
		}

		/* Get the next line */
		line_size = getline(&line_buf, &line_buf_size, fp);
	}

	/* Free the allocated line buffer */
	SAFEFREE(line_buf);

	/* Close the file now that we are done with it */
	fclose(fp);
}

int editLine(char* line) {
	// Print entry that matches hash key
	printf("\nFound entry: %s", line);

	// Request user confirmation (hash collisions may exist)
	printf("Is this the entry you would like to edit? (y/n) ");
	char* response = inputString(stdin,10);

	if (tolower(response[0])=='y') {
		// Initialize edits
		char *edit_project;
		char *edit_accomplish;

		// Grab hash ID and date from previous line (this will remain)
		// ASSUMES THAT THE LINE IS FORMATTED : "<Hash ID> (<Date>) ..."
		char *start = strtok(line,")");

		// Grab edits for project title
		printf("Re-enter the project name (default will set this to '%s'): ", DEFAULT_PROJECT);
		edit_project = inputString(stdin, 10);

		// Grab edits for note
		printf("Re-enter the note: ");
		edit_accomplish = inputString(stdin, 10);

		// Print output to user
		printf("%s) %s: %s\n", start, edit_project, edit_accomplish);

		SAFEFREE(edit_project);
		SAFEFREE(edit_accomplish);

		return 1;
	}
	else {
		return 0;
	}
}

int deleteLine(char* line) {
	// Print entry that matches hash key
	printf("\nFound entry: %s", line);

	// Request user confirmation (hash collisions may exist)
	printf("Is this the entry you would like to delete? (y/n) ");
	char* response = inputString(stdin,10);

	if (tolower(response[0])=='y') {
		// Delete line

		return 1;
	}
	else {
		return 0;
	}
}

int projectNotes(char* line) {
	printf("%s", line);

	return 1;
}

void projectList() {
	/* Create hash table to parse out distinct projects */
	HashTable *projects = InitHash();

	/* Parse through the file */
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	ssize_t line_size;

	FILE *fp = fopen(OUTPUT_FILE, "r");

	if (!fp) {
		fprintf(stderr, "Error opening file '%s'\n", OUTPUT_FILE);
		return;
	}

	/* Get the first line of the file. */
	line_size = getline(&line_buf, &line_buf_size, fp);

	/* Loop through until we are done with the file. */
	while ((line_size >= 0)) {

		/* Grab hash ID and date from previous line (this will remain) */
		// ASSUMES THAT THE LINE IS FORMATTED : "<Hash ID> (<Date>) ..."
		char *project = strtok(line_buf," ");
		project = strtok(NULL," ");
		project = strtok(NULL,":");

		/* If duplicate, 0 is returned; otherwise 1 */
		if ( AddToHashTable(project, projects) ) printf("%s\n", project);

		/* Get the next line */
		line_size = getline(&line_buf, &line_buf_size, fp);
	}

	/* Clean up memory allocation */
	SAFEFREE(line_buf);
	FreeHashTable(projects);

	/* Close the file now that we are done with it */
	fclose(fp);
}
