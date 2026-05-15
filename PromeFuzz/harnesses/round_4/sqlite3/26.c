// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_libversion_number at sqlite3.c:171000:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_int(const uint8_t *data, size_t size, size_t *pos) {
    if (*pos + sizeof(int) > size) {
        *pos = size;
        return 0;
    }
    int val = *(int*)(data + *pos);
    *pos += sizeof(int);
    return val;
}

static sqlite3_int64 get_int64(const uint8_t *data, size_t size, size_t *pos) {
    if (*pos + sizeof(sqlite3_int64) > size) {
        *pos = size;
        return 0;
    }
    sqlite3_int64 val = *(sqlite3_int64*)(data + *pos);
    *pos += sizeof(sqlite3_int64);
    return val;
}

static const char* get_text(const uint8_t *data, size_t size, size_t *pos, int *len) {
    if (*pos >= size) {
        *len = 0;
        return "";
    }
    *len = (size - *pos > 10) ? 10 : (size - *pos);
    const char* text = (const char*)(data + *pos);
    *pos += *len;
    return text;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    size_t pos = 0;
    int param_index = 1;
    
    if (Size < 1) return 0;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    const char *sql = "SELECT ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    sqlite3_bind_int64(stmt, param_index, get_int64(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    int len;
    sqlite3_bind_text(stmt, param_index, get_text(Data, Size, &pos, &len), len, SQLITE_TRANSIENT);
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int64(stmt, param_index, get_int64(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_text(stmt, param_index, get_text(Data, Size, &pos, &len), len, SQLITE_TRANSIENT);
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int64(stmt, param_index, get_int64(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_text(stmt, param_index, get_text(Data, Size, &pos, &len), len, SQLITE_TRANSIENT);
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_text(stmt, param_index, get_text(Data, Size, &pos, &len), len, SQLITE_TRANSIENT);
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_text(stmt, param_index, get_text(Data, Size, &pos, &len), len, SQLITE_TRANSIENT);
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_text(stmt, param_index, get_text(Data, Size, &pos, &len), len, SQLITE_TRANSIENT);
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_text(stmt, param_index, get_text(Data, Size, &pos, &len), len, SQLITE_TRANSIENT);
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_text(stmt, param_index, get_text(Data, Size, &pos, &len), len, SQLITE_TRANSIENT);
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_libversion_number();
    
    sqlite3_bind_int(stmt, param_index, get_int(Data, Size, &pos));
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_text(stmt, param_index, get_text(Data, Size, &pos, &len), len, SQLITE_TRANSIENT);
    param_index = (param_index % 10) + 1;
    
    sqlite3_bind_text(stmt, param_index, get_text(Data, Size, &pos, &len), len, SQLITE_TRANSIENT);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}