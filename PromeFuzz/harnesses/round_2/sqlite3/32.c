// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *db_name = NULL;
    int rc;
    
    // Create a dummy file for the database
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fclose(fp);
    }
    
    // First sqlite3_open
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        // If open fails, we can't proceed with the sequence
        if (db) {
            sqlite3_close(db);
        }
        // Clean up the dummy file
        remove("./dummy_file");
        return 0;
    }
    
    // sqlite3_db_name - try different N values (0=main, 1=temp, 2=attached)
    if (Size > 0) {
        int n = Data[0] % 3; // 0, 1, or 2
        db_name = sqlite3_db_name(db, n);
        // db_name may be NULL if n out of range, that's OK
    } else {
        db_name = sqlite3_db_name(db, 0); // main database
    }
    
    // First sqlite3_file_control
    // Create a valid integer argument instead of NULL
    int file_control_arg = 0;
    if (Size > 1) {
        // Use fuzzer data to influence opcode selection
        int op1 = Data[1 % Size] % 100; // Limit to reasonable range
        sqlite3_file_control(db, "main", op1, &file_control_arg);
    } else {
        sqlite3_file_control(db, "main", 0, &file_control_arg);
    }
    
    // Second sqlite3_file_control
    // Use a different argument for the second call
    int file_control_arg2 = 0;
    if (Size > 2) {
        int op2 = Data[2 % Size] % 100;
        sqlite3_file_control(db, NULL, op2, &file_control_arg2);
    } else {
        sqlite3_file_control(db, NULL, 0, &file_control_arg2);
    }
    
    // sqlite3_close
    rc = sqlite3_close(db);
    // Note: rc may be SQLITE_BUSY if there are unfinalized statements
    // We ignore the return value as per instructions
    
    // Second sqlite3_open (reopen)
    sqlite3 *db2 = NULL;
    rc = sqlite3_open("./dummy_file", &db2);
    if (rc == SQLITE_OK && db2) {
        sqlite3_close(db2);
    }
    
    // Clean up the dummy file
    remove("./dummy_file");
    
    return 0;
}