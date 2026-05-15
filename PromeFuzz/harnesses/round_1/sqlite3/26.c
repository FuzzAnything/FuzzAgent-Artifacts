// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
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
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Create dummy database and statement for binding
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *dummy_sql = "SELECT ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14, ?15, ?16, ?17, ?18, ?19, ?20, ?21, ?22, ?23, ?24, ?25, ?26, ?27, ?28, ?29, ?30, ?31, ?32, ?33, ?34";
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Prepare a statement with enough parameters
    if (sqlite3_prepare_v2(db, dummy_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Generate values from fuzzer input
    sqlite3_int64 int64_val = 0;
    int int_val = 0;
    const char *text_val = NULL;
    int text_len = 0;
    
    // Use Data to generate parameter values
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&int64_val, Data, sizeof(sqlite3_int64));
    } else if (Size > 0) {
        int64_val = (sqlite3_int64)Data[0];
    }
    
    if (Size >= sizeof(int)) {
        memcpy(&int_val, Data, sizeof(int));
    } else if (Size > 0) {
        int_val = (int)Data[0];
    }
    
    // Prepare text value - use up to 100 bytes from input
    text_len = Size > 100 ? 100 : (int)Size;
    char *text_buf = (char*)malloc(text_len + 1);
    if (text_buf) {
        if (Size > 0) {
            memcpy(text_buf, Data, text_len);
        }
        text_buf[text_len] = '\0';
        text_val = text_buf;
    } else {
        text_val = "";
        text_len = 0;
    }
    
    // Execute the sequence of bind calls
    // 1. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 1, int64_val);
    
    // 2. sqlite3_bind_int
    sqlite3_bind_int(stmt, 2, int_val);
    
    // 3. sqlite3_bind_text
    sqlite3_bind_text(stmt, 3, text_val, text_len, SQLITE_TRANSIENT);
    
    // 4. sqlite3_bind_int
    sqlite3_bind_int(stmt, 4, int_val);
    
    // 5. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 5, int64_val);
    
    // 6. sqlite3_bind_text
    sqlite3_bind_text(stmt, 6, text_val, text_len, SQLITE_TRANSIENT);
    
    // 7. sqlite3_bind_int
    sqlite3_bind_int(stmt, 7, int_val);
    
    // 8. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 8, int64_val);
    
    // 9. sqlite3_bind_text
    sqlite3_bind_text(stmt, 9, text_val, text_len, SQLITE_TRANSIENT);
    
    // 10. sqlite3_bind_int
    sqlite3_bind_int(stmt, 10, int_val);
    
    // 11. sqlite3_bind_int
    sqlite3_bind_int(stmt, 11, int_val);
    
    // 12. sqlite3_bind_text
    sqlite3_bind_text(stmt, 12, text_val, text_len, SQLITE_TRANSIENT);
    
    // 13. sqlite3_bind_text
    sqlite3_bind_text(stmt, 13, text_val, text_len, SQLITE_TRANSIENT);
    
    // 14. sqlite3_bind_text
    sqlite3_bind_text(stmt, 14, text_val, text_len, SQLITE_TRANSIENT);
    
    // 15. sqlite3_bind_int
    sqlite3_bind_int(stmt, 15, int_val);
    
    // 16. sqlite3_bind_int
    sqlite3_bind_int(stmt, 16, int_val);
    
    // 17. sqlite3_bind_int
    sqlite3_bind_int(stmt, 17, int_val);
    
    // 18. sqlite3_bind_int
    sqlite3_bind_int(stmt, 18, int_val);
    
    // 19. sqlite3_bind_text
    sqlite3_bind_text(stmt, 19, text_val, text_len, SQLITE_TRANSIENT);
    
    // 20. sqlite3_bind_int
    sqlite3_bind_int(stmt, 20, int_val);
    
    // 21. sqlite3_bind_int
    sqlite3_bind_int(stmt, 21, int_val);
    
    // 22. sqlite3_bind_int
    sqlite3_bind_int(stmt, 22, int_val);
    
    // 23. sqlite3_bind_int
    sqlite3_bind_int(stmt, 23, int_val);
    
    // 24. sqlite3_bind_int
    sqlite3_bind_int(stmt, 24, int_val);
    
    // 25. sqlite3_bind_int
    sqlite3_bind_int(stmt, 25, int_val);
    
    // 26. sqlite3_bind_int
    sqlite3_bind_int(stmt, 26, int_val);
    
    // 27. sqlite3_bind_int
    sqlite3_bind_int(stmt, 27, int_val);
    
    // 28. sqlite3_bind_int
    sqlite3_bind_int(stmt, 28, int_val);
    
    // 29. sqlite3_bind_int
    sqlite3_bind_int(stmt, 29, int_val);
    
    // 30. sqlite3_bind_text
    sqlite3_bind_text(stmt, 30, text_val, text_len, SQLITE_TRANSIENT);
    
    // 31. sqlite3_bind_int
    sqlite3_bind_int(stmt, 31, int_val);
    
    // 32. sqlite3_libversion_number
    sqlite3_libversion_number();
    
    // 33. sqlite3_bind_int
    sqlite3_bind_int(stmt, 33, int_val);
    
    // 34. sqlite3_bind_text
    sqlite3_bind_text(stmt, 34, text_val, text_len, SQLITE_TRANSIENT);
    
    // Cleanup
    if (text_buf) {
        free(text_buf);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}