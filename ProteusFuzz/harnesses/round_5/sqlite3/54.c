// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *dbName = NULL;
    int rc;

    if (Size < 1) {
        return 0;
    }

    // 1. Prepare environment: Open the database
    char filename[256];
    size_t len = (Size < sizeof(filename) - 1) ? Size : sizeof(filename) - 1;
    memcpy(filename, Data, len);
    filename[len] = '\0';

    if (len == 0 || filename[0] == '\0') {
        rc = sqlite3_open("./dummy_file", &db);
    } else {
        rc = sqlite3_open(filename, &db);
    }

    if (rc != SQLITE_OK || db == NULL) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }

    // 2. Invoke sqlite3_db_name
    int nIndex = (Data[0] % 5); 
    dbName = sqlite3_db_name(db, nIndex);

    // 3. Invoke sqlite3_file_control (First call)
    int op1 = (Size > 1) ? (Data[1] % 100) : 0;
    
    // Use a buffer large enough to satisfy any potential pointer dereference by opcodes
    // like SQLITE_FCNTL_SIZE_HINT which expects an i64 (8 bytes).
    // We initialize it to avoid undefined behavior if the opcode writes to it.
    int64_t dummy_val = 0;
    void *arg1 = &dummy_val;

    const char *zDbName1 = (dbName != NULL) ? dbName : "main";
    
    rc = sqlite3_file_control(db, zDbName1, op1, arg1);
    (void)rc;

    // 4. Invoke sqlite3_file_control (Second call)
    int op2 = (Size > 3) ? (Data[3] % 100) : 0;
    // Reuse the same safe buffer
    dummy_val = 0; 
    void *arg2 = &dummy_val;
    const char *zDbName2 = NULL; 

    rc = sqlite3_file_control(db, zDbName2, op2, arg2);
    (void)rc;

    // 5. Invoke sqlite3_close
    rc = sqlite3_close(db);
    db = NULL; 

    // 6. Invoke sqlite3_open again
    rc = sqlite3_open(":memory:", &db);
    
    if (rc == SQLITE_OK && db != NULL) {
        sqlite3_close(db);
    }

    return 0;
}