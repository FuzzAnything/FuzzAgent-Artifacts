// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *db_name;
    void *file_control_arg = NULL;
    
    // Write fuzzer data to dummy file
    write_dummy_file(Data, Size);
    
    // First sqlite3_open call
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        // Clean up and return if open fails
        remove_dummy_file();
        return 0;
    }
    
    // sqlite3_db_name call - try different N values
    for (int i = 0; i < 3; i++) {
        db_name = sqlite3_db_name(db, i);
        // db_name may be NULL if N is out of range
    }
    
    // First sqlite3_file_control call
    // Use different opcodes to explore more states
    // SQLITE_FCNTL_LOCKSTATE = 1, SQLITE_FCNTL_GET_LOCKPROXYFILE = 2, etc.
    int op1 = Size > 0 ? Data[0] % 10 : 1;
    sqlite3_file_control(db, "main", op1, file_control_arg);
    
    // Second sqlite3_file_control call
    int op2 = Size > 1 ? Data[1] % 10 : 2;
    sqlite3_file_control(db, "temp", op2, file_control_arg);
    
    // sqlite3_close call
    sqlite3_close(db);
    
    // Second sqlite3_open call - try with different flags
    sqlite3 *db2 = NULL;
    rc = sqlite3_open("./dummy_file", &db2);
    if (rc == SQLITE_OK) {
        // Clean up the second database
        sqlite3_close(db2);
    }
    
    // Clean up dummy file
    remove_dummy_file();
    
    return 0;
}