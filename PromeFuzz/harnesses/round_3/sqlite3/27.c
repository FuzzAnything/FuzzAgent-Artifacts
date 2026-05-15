// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_free(void* ptr) {
    // Do nothing - used for SQLITE_STATIC binding
}

static int get_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) <= size) {
        int value;
        memcpy(&value, data + *offset, sizeof(int));
        *offset += sizeof(int);
        return value;
    }
    *offset = size;
    return 0;
}

static sqlite3_int64 get_int64(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(sqlite3_int64) <= size) {
        sqlite3_int64 value;
        memcpy(&value, data + *offset, sizeof(sqlite3_int64));
        *offset += sizeof(sqlite3_int64);
        return value;
    }
    *offset = size;
    return 0;
}

static const char* get_text(const uint8_t *data, size_t size, size_t *offset, int *length) {
    if (*offset >= size) {
        *length = 0;
        return "";
    }
    
    // Use remaining data as text
    *length = size - *offset;
    const char *text = (const char*)(data + *offset);
    *offset = size;
    return text;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a prepared statement with many parameters
    const char *sql = "SELECT ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14, ?15, ?16, ?17, ?18, ?19, ?20, ?21, ?22, ?23, ?24, ?25, ?26, ?27, ?28, ?29, ?30, ?31, ?32, ?33, ?34, ?35";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Extract values from fuzzer input
    size_t data_offset = 0;
    int text_len;
    const char *text;
    
    // Execute the sequence of function calls
    // 1. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 1, get_int64(Data, Size, &data_offset));
    
    // 2. sqlite3_bind_int
    sqlite3_bind_int(stmt, 2, get_int(Data, Size, &data_offset));
    
    // 3. sqlite3_bind_text
    text = get_text(Data, Size, &data_offset, &text_len);
    sqlite3_bind_text(stmt, 3, text, text_len, SQLITE_STATIC);
    
    // 4. sqlite3_bind_int
    sqlite3_bind_int(stmt, 4, get_int(Data, Size, &data_offset));
    
    // 5. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 5, get_int64(Data, Size, &data_offset));
    
    // 6. sqlite3_bind_text
    text = get_text(Data, Size, &data_offset, &text_len);
    sqlite3_bind_text(stmt, 6, text, text_len, SQLITE_STATIC);
    
    // 7. sqlite3_bind_int
    sqlite3_bind_int(stmt, 7, get_int(Data, Size, &data_offset));
    
    // 8. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 8, get_int64(Data, Size, &data_offset));
    
    // 9. sqlite3_bind_text
    text = get_text(Data, Size, &data_offset, &text_len);
    sqlite3_bind_text(stmt, 9, text, text_len, SQLITE_STATIC);
    
    // 10. sqlite3_bind_int
    sqlite3_bind_int(stmt, 10, get_int(Data, Size, &data_offset));
    
    // 11. sqlite3_bind_int
    sqlite3_bind_int(stmt, 11, get_int(Data, Size, &data_offset));
    
    // 12. sqlite3_bind_text
    text = get_text(Data, Size, &data_offset, &text_len);
    sqlite3_bind_text(stmt, 12, text, text_len, SQLITE_STATIC);
    
    // 13. sqlite3_bind_text
    text = get_text(Data, Size, &data_offset, &text_len);
    sqlite3_bind_text(stmt, 13, text, text_len, SQLITE_STATIC);
    
    // 14. sqlite3_bind_text
    text = get_text(Data, Size, &data_offset, &text_len);
    sqlite3_bind_text(stmt, 14, text, text_len, SQLITE_STATIC);
    
    // 15. sqlite3_bind_int
    sqlite3_bind_int(stmt, 15, get_int(Data, Size, &data_offset));
    
    // 16. sqlite3_bind_int
    sqlite3_bind_int(stmt, 16, get_int(Data, Size, &data_offset));
    
    // 17. sqlite3_bind_int
    sqlite3_bind_int(stmt, 17, get_int(Data, Size, &data_offset));
    
    // 18. sqlite3_bind_int
    sqlite3_bind_int(stmt, 18, get_int(Data, Size, &data_offset));
    
    // 19. sqlite3_bind_text
    text = get_text(Data, Size, &data_offset, &text_len);
    sqlite3_bind_text(stmt, 19, text, text_len, SQLITE_STATIC);
    
    // 20. sqlite3_bind_int
    sqlite3_bind_int(stmt, 20, get_int(Data, Size, &data_offset));
    
    // 21. sqlite3_bind_int
    sqlite3_bind_int(stmt, 21, get_int(Data, Size, &data_offset));
    
    // 22. sqlite3_bind_int
    sqlite3_bind_int(stmt, 22, get_int(Data, Size, &data_offset));
    
    // 23. sqlite3_bind_int
    sqlite3_bind_int(stmt, 23, get_int(Data, Size, &data_offset));
    
    // 24. sqlite3_bind_int
    sqlite3_bind_int(stmt, 24, get_int(Data, Size, &data_offset));
    
    // 25. sqlite3_bind_int
    sqlite3_bind_int(stmt, 25, get_int(Data, Size, &data_offset));
    
    // 26. sqlite3_bind_int
    sqlite3_bind_int(stmt, 26, get_int(Data, Size, &data_offset));
    
    // 27. sqlite3_bind_int
    sqlite3_bind_int(stmt, 27, get_int(Data, Size, &data_offset));
    
    // 28. sqlite3_bind_int
    sqlite3_bind_int(stmt, 28, get_int(Data, Size, &data_offset));
    
    // 29. sqlite3_bind_int
    sqlite3_bind_int(stmt, 29, get_int(Data, Size, &data_offset));
    
    // 30. sqlite3_bind_text
    text = get_text(Data, Size, &data_offset, &text_len);
    sqlite3_bind_text(stmt, 30, text, text_len, SQLITE_STATIC);
    
    // 31. sqlite3_bind_int
    sqlite3_bind_int(stmt, 31, get_int(Data, Size, &data_offset));
    
    // 32. sqlite3_libversion_number
    sqlite3_libversion_number();
    
    // 33. sqlite3_bind_int
    sqlite3_bind_int(stmt, 32, get_int(Data, Size, &data_offset));
    
    // 34. sqlite3_bind_text
    text = get_text(Data, Size, &data_offset, &text_len);
    sqlite3_bind_text(stmt, 33, text, text_len, SQLITE_STATIC);
    
    // 35. sqlite3_bind_text
    text = get_text(Data, Size, &data_offset, &text_len);
    sqlite3_bind_text(stmt, 34, text, text_len, SQLITE_STATIC);
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}