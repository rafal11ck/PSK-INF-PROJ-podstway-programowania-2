#include <sqlite3.h>
#include <stdio.h>

int main() {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  sqlite3_open("test.db", &db);
  char *zerrMsg = 0;

  char *err;
  int rc = sqlite3_exec(
      db, "CREATE TABLE IF NOT EXISTS tt1(x1 INT, x2 INT, name varchar(100));",
      NULL, NULL, &err);
  if (rc != SQLITE_OK) {
    printf("error: %s\n", err);
  }
  for (int i = 0; i < 10; ++i) {
    char query[] = "INSERT INTO tt1 VALUES(1, 13, 'ngffgfgd');\n";
    printf(query, i);
    rc = sqlite3_exec(db, query, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
      printf("error: %s\n", err);
    }
  }
  sqlite3_prepare_v2(db, "SELECT x1,x2,name FROM tt1", -1, &stmt, 0);
  while (sqlite3_step(stmt) != SQLITE_DONE) {
    int a1, a2;
    const unsigned char *myname;
    a1 = sqlite3_column_int(stmt, 0);
    a2 = sqlite3_column_int(stmt, 1);
    myname = sqlite3_column_text(stmt, 2);
    printf("%d\t%d\t%s\n", a1, a2, myname);
  }
}
