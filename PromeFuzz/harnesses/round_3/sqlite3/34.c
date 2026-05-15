// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size, fp);
        }
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
    void *arg = NULL;
    
    // Write fuzzer input to dummy file
    write_dummy_file(Data, Size);
    
    // First sqlite3_open
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        remove_dummy_file();
        return 0;
    }
    
    // sqlite3_db_name - try different database indices
    for (int i = 0; i < 3; i++) {
        db_name = sqlite3_db_name(db, i);
        // db_name may be NULL if i is out of range, which is fine
    }
    
    // First sqlite3_file_control
    // Use a safe opcode that doesn't require special arguments
    rc = sqlite3_file_control(db, "main", SQLITE_FCNTL_DATA_VERSION, &arg);
    
    // Second sqlite3_file_control with different opcode
    rc = sqlite3_file_control(db, "main", SQLITE_FCNTL_VFS_POINTER, &arg);
    
    // sqlite3_close
    rc = sqlite3_close(db);
    
    // Second sqlite3_open
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK) {
        sqlite3_close(db);
    }
    
    // Cleanup
    remove_dummy_file();
    
    return 0;
}