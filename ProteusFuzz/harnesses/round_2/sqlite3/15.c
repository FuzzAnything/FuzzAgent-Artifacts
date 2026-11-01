// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

static char *safe_vmprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char *result = sqlite3_vmprintf(fmt, args);
  va_end(args);
  return result;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  const char *zTail = NULL;
  char *formatted = NULL;
  int rc;

  /* Initialize database in memory */
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Ensure we have enough data for at least a format string and SQL */
  if (Size < 2) {
    sqlite3_close(db);
    return 0;
  }

  /* Use first part of input as format string for sqlite3_vmprintf */
  size_t fmtLen = 0;
  while (fmtLen < Size - 1 && Data[fmtLen] != '\0') {
    fmtLen++;
  }
  if (fmtLen == 0) fmtLen = 1;

  char *fmt = (char *)sqlite3_malloc(fmtLen + 1);
  if (!fmt) {
    sqlite3_close(db);
    return 0;
  }
  memcpy(fmt, Data, fmtLen);
  fmt[fmtLen] = '\0';

  /* Invoke sqlite3_vmprintf via helper function to handle va_list correctly */
  formatted = safe_vmprintf(fmt, 42, 3.14, "test");

  /* Use remaining data as SQL statement */
  size_t sqlStart = fmtLen + 1;
  size_t sqlLen = 0;
  char *sql = NULL;

  if (sqlStart < Size) {
    sqlLen = Size - sqlStart;
    if (sqlLen == 0) sqlLen = 1;
    
    /* Ensure SQL is null-terminated */
    sql = (char *)sqlite3_malloc(sqlLen + 1);
    if (!sql) {
      sqlite3_free(fmt);
      sqlite3_free(formatted);
      sqlite3_close(db);
      return 0;
    }
    memcpy(sql, Data + sqlStart, sqlLen);
    sql[sqlLen] = '\0';
  } else {
    /* Fallback to default SQL if no data remains */
    const char *default_sql = "SELECT 1";
    sqlLen = strlen(default_sql);
    sql = (char *)sqlite3_malloc(sqlLen + 1);
    if (!sql) {
      sqlite3_free(fmt);
      sqlite3_free(formatted);
      sqlite3_close(db);
      return 0;
    }
    memcpy(sql, default_sql, sqlLen);
    sql[sqlLen] = '\0';
  }

  /* Invoke sqlite3_prepare_v2 */
  rc = sqlite3_prepare_v2(db, sql, (int)sqlLen, &stmt, &zTail);

  /* Invoke sqlite3_extended_errcode and sqlite3_errmsg after prepare */
  if (rc != SQLITE_OK) {
    (void)sqlite3_extended_errcode(db);
    (void)sqlite3_errmsg(db);
  }

  /* Execute statement if prepared successfully */
  if (stmt != NULL) {
    rc = sqlite3_step(stmt);
    
    /* Invoke sqlite3_extended_errcode and sqlite3_errmsg after step */
    (void)sqlite3_extended_errcode(db);
    (void)sqlite3_errmsg(db);

    /* Reset statement */
    sqlite3_reset(stmt);
  }

  /* Cleanup: free formatted string, finalize statement, close db */
  sqlite3_free(formatted);
  sqlite3_free(fmt);
  sqlite3_free(sql);
  
  if (stmt != NULL) {
    sqlite3_finalize(stmt);
  }
  
  sqlite3_close(db);

  return 0;
}