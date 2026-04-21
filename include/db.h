#ifndef DB_H
#define DB_H

int db_open(const char *path);
int db_init(void);
int db_add_task(const char *name);
int db_view_tasks(void);
int db_complete_task(int id);
void db_close(void);

#endif /* DB_H */
