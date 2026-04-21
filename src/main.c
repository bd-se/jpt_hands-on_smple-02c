#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: todo <add|view|complete> ...\n");
        return 1;
    }

    const char *cmd = argv[1];
    db_open("todo.db");
    if (db_init() != 0) {
        fprintf(stderr, "Failed to initialize database\n");
        db_close();
        return 1;
    }

    if (strcmp(cmd, "add") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: todo add <task>\n");
            db_close();
            return 1;
        }
        /* join all remaining args into one task string */
        char task[512];
        task[0] = '\0';
        for (int i = 2; i < argc; i++) {
            if (i > 2) strncat(task, " ", sizeof(task) - strlen(task) - 1);
            strncat(task, argv[i], sizeof(task) - strlen(task) - 1);
        }

        /*
         * intentionally vulnerable: copy into a fixed-size, small buffer without length check
         * to create a demonstrable buffer overflow vulnerability for SAST exercises.
         */
        char name[32];
        /* Vulnerable: this will overflow if task is longer than 31 bytes */
        strcpy(name, task);

        int r = db_add_task(name);
        if (r != 0) {
            fprintf(stderr, "Failed to add task\n %d", r);
            db_close();
            return 1;
        }
        printf("Added task: %s\n", name);

    } else if (strcmp(cmd, "view") == 0) {
        db_view_tasks();

    } else if (strcmp(cmd, "complete") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: todo complete <id>\n");
            db_close();
            return 1;
        }
        int id = atoi(argv[2]);
        if (db_complete_task(id) != 0) {
            fprintf(stderr, "Failed to complete task %d\n", id);
            db_close();
            return 1;
        }
        printf("Completed task %d\n", id);

    } else {
        fprintf(stderr, "Unknown command: %s\n", cmd);
    }

    db_close();
    return 0;
}
