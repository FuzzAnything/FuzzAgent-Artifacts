// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
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
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *dummy_sql = "SELECT ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14, ?15, ?16, ?17, ?18, ?19, ?20, ?21, ?22, ?23, ?24, ?25, ?26, ?27, ?28, ?29, ?30, ?31, ?32, ?33, ?34, ?35";
    const char *text_data = NULL;
    int text_len = 0;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Prepare a statement with enough placeholders
    if (sqlite3_prepare_v2(db, dummy_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer data for text binding
    if (Size > 0) {
        text_data = (const char *)Data;
        text_len = Size > 100 ? 100 : (int)Size;
    } else {
        text_data = "";
        text_len = 0;
    }
    
    // Execute the sequence of bind calls
    // 1. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 1, (sqlite3_int64)Data[0]);
    
    // 2. sqlite3_bind_int
    sqlite3_bind_int(stmt, 2, (int)Data[0]);
    
    // 3. sqlite3_bind_text
    sqlite3_bind_text(stmt, 3, text_data, text_len, SQLITE_TRANSIENT);
    
    // 4. sqlite3_bind_int
    sqlite3_bind_int(stmt, 4, (int)Data[0]);
    
    // 5. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 5, (sqlite3_int64)Data[0]);
    
    // 6. sqlite3_bind_text
    sqlite3_bind_text(stmt, 6, text_data, text_len, SQLITE_TRANSIENT);
    
    // 7. sqlite3_bind_int
    sqlite3_bind_int(stmt, 7, (int)Data[0]);
    
    // 8. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 8, (sqlite3_int64)Data[0]);
    
    // 9. sqlite3_bind_text
    sqlite3_bind_text(stmt, 9, text_data, text_len, SQLITE_TRANSIENT);
    
    // 10. sqlite3_bind_int
    sqlite3_bind_int(stmt, 10, (int)Data[0]);
    
    // 11. sqlite3_bind_int
    sqlite3_bind_int(stmt, 11, (int)Data[0]);
    
    // 12. sqlite3_bind_text
    sqlite3_bind_text(stmt, 12, text_data, text_len, SQLITE_TRANSIENT);
    
    // 13. sqlite3_bind_text
    sqlite3_bind_text(stmt, 13, text_data, text_len, SQLITE_TRANSIENT);
    
    // 14. sqlite3_bind_text
    sqlite3_bind_text(stmt, 14, text_data, text_len, SQLITE_TRANSIENT);
    
    // 15. sqlite3_bind_int
    sqlite3_bind_int(stmt, 15, (int)Data[0]);
    
    // 16. sqlite3_bind_int
    sqlite3_bind_int(stmt, 16, (int)Data[0]);
    
    // 17. sqlite3_bind_int
    sqlite3_bind_int(stmt, 17, (int)Data[0]);
    
    // 18. sqlite3_bind_int
    sqlite3_bind_int(stmt, 18, (int)Data[0]);
    
    // 19. sqlite3_bind_text
    sqlite3_bind_text(stmt, 19, text_data, text_len, SQLITE_TRANSIENT);
    
    // 20. sqlite3_bind_int
    sqlite3_bind_int(stmt, 20, (int)Data[0]);
    
    // 21. sqlite3_bind_int
    sqlite3_bind_int(stmt, 21, (int)Data[0]);
    
    // 22. sqlite3_bind_int
    sqlite3_bind_int(stmt, 22, (int)Data[0]);
    
    // 23. sqlite3_bind_int
    sqlite3_bind_int(stmt, 23, (int)Data[0]);
    
    // 24. sqlite3_bind_int
    sqlite3_bind_int(stmt, 24, (int)Data[0]);
    
    // 25. sqlite3_bind_int
    sqlite3_bind_int(stmt, 25, (int)Data[0]);
    
    // 26. sqlite3_bind_int
    sqlite3_bind_int(stmt, 26, (int)Data[0]);
    
    // 27. sqlite3_bind_int
    sqlite3_bind_int(stmt, 27, (int)Data[0]);
    
    // 28. sqlite3_bind_int
    sqlite3_bind_int(stmt, 28, (int)Data[0]);
    
    // 29. sqlite3_bind_int
    sqlite3_bind_int(stmt, 29, (int)Data[0]);
    
    // 30. sqlite3_bind_text
    sqlite3_bind_text(stmt, 30, text_data, text_len, SQLITE_TRANSIENT);
    
    // 31. sqlite3_bind_int
    sqlite3_bind_int(stmt, 31, (int)Data[0]);
    
    // 32. sqlite3_libversion_number
    sqlite3_libversion_number();
    
    // 33. sqlite3_bind_int
    sqlite3_bind_int(stmt, 33, (int)Data[0]);
    
    // 34. sqlite3_bind_text
    sqlite3_bind_text(stmt, 34, text_data, text_len, SQLITE_TRANSIENT);
    
    // 35. sqlite3_bind_text
    sqlite3_bind_text(stmt, 35, text_data, text_len, SQLITE_TRANSIENT);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}