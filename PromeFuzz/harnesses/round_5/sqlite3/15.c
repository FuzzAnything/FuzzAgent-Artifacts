// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int val;
    memcpy(&val, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return val;
}

static sqlite3_int64 read_int64(const uint8_t **data, size_t *size) {
    if (*size < sizeof(sqlite3_int64)) return 0;
    sqlite3_int64 val;
    memcpy(&val, *data, sizeof(sqlite3_int64));
    *data += sizeof(sqlite3_int64);
    *size -= sizeof(sqlite3_int64);
    return val;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 50) return 0;
    
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    for (int i = 0; i < 4; i++) {
        int id = read_int(&ptr, &remaining);
        int newVal = read_int(&ptr, &remaining);
        sqlite3_limit(db, id, newVal);
    }
    
    sqlite3_int64 heap_limit = read_int64(&ptr, &remaining);
    sqlite3_hard_heap_limit64(heap_limit);
    
    int test_op = read_int(&ptr, &remaining);
    sqlite3_test_control(test_op);
    
    unsigned char *pData = (unsigned char*)ptr;
    size_t data_size = remaining;
    if (data_size > 100) data_size = 100;
    
    sqlite3_int64 szDb = data_size;
    sqlite3_int64 szBuf = data_size;
    unsigned flags = 0;
    
    sqlite3_deserialize(db, "main", pData, szDb, szBuf, flags);
    
    int file_op = read_int(&ptr, &remaining);
    sqlite3_file_control(db, "main", file_op, NULL);
    
    sqlite3_close(db);
    return 0;
}