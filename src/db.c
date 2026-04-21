#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "db.h"

static sqlite3 *db = NULL;

int db_open(const char *path) {
    if (db) return 0;
    int rc = sqlite3_open(path, &db);
    return rc;
}

int db_init(void) {
    const char *sql = "CREATE TABLE IF NOT EXISTS todos (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, status INTEGER);";
    char *err = NULL;
    int rc = sqlite3_exec(db, sql, 0, 0, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "DB init error: %s\n", err);
        sqlite3_free(err);
        return rc;
    }
    return SQLITE_OK;
}

int db_add_task(const char *name) {
    const char *sql = "INSERT INTO todos (name, status) VALUES (?, 0);";
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Insert failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }
    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int db_view_tasks(void) {
    const char *sql = "SELECT id, name, status FROM todos;";
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        int status = sqlite3_column_int(stmt, 2);
        printf("%d: %s [%s]\n", id, name, status ? "done" : "todo");
    }
    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int db_complete_task(int id) {
    const char *sql = "UPDATE todos SET status = 1 WHERE id = ?;";
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Update failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }
    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

void db_close(void) {
    if (db) sqlite3_close(db);
    db = NULL;
}
