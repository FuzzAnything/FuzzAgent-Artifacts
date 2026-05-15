// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
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
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - used for sqlite3_bind_text
}

static sqlite3_int64 get_int64(const uint8_t *Data, size_t Size, size_t *data_index) {
    sqlite3_int64 val = 0;
    if (*data_index + sizeof(val) <= Size) {
        memcpy(&val, Data + *data_index, sizeof(val));
        *data_index += sizeof(val);
    } else if (*data_index < Size) {
        memcpy(&val, Data + *data_index, Size - *data_index);
        *data_index = Size;
    }
    return val;
}

static int get_int(const uint8_t *Data, size_t Size, size_t *data_index) {
    int val = 0;
    if (*data_index + sizeof(val) <= Size) {
        memcpy(&val, Data + *data_index, sizeof(val));
        *data_index += sizeof(val);
    } else if (*data_index < Size) {
        memcpy(&val, Data + *data_index, Size - *data_index);
        *data_index = Size;
    }
    return val;
}

static const char *get_text(const uint8_t *Data, size_t Size, size_t *data_index, int *text_len) {
    static char buffer[256];
    size_t len = 0;
    if (*data_index < Size) {
        len = (Size - *data_index > 255) ? 255 : (Size - *data_index);
        memcpy(buffer, Data + *data_index, len);
        *data_index += len;
    }
    buffer[len] = '\0';
    *text_len = (int)len;
    return buffer;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a dummy table and prepare a statement with enough parameters
    rc = sqlite3_exec(db, "CREATE TABLE t1(a INTEGER, b INTEGER, c TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement with many parameters (at least 33)
    const char *sql = "INSERT INTO t1 VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // Use Data to generate values for binding
    size_t data_index = 0;
    int param_idx = 1;
    int text_len;
    
    // Execute the bind calls in the specified order
    // We need to bind to 33 parameters total
    
    // 1. sqlite3_bind_int64
    if (param_idx <= 33) sqlite3_bind_int64(stmt, param_idx++, get_int64(Data, Size, &data_index));
    
    // 2. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 3. sqlite3_bind_text
    if (param_idx <= 33) sqlite3_bind_text(stmt, param_idx++, get_text(Data, Size, &data_index, &text_len), text_len, dummy_destructor);
    
    // 4. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 5. sqlite3_bind_int64
    if (param_idx <= 33) sqlite3_bind_int64(stmt, param_idx++, get_int64(Data, Size, &data_index));
    
    // 6. sqlite3_bind_text
    if (param_idx <= 33) sqlite3_bind_text(stmt, param_idx++, get_text(Data, Size, &data_index, &text_len), text_len, dummy_destructor);
    
    // 7. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 8. sqlite3_bind_int64
    if (param_idx <= 33) sqlite3_bind_int64(stmt, param_idx++, get_int64(Data, Size, &data_index));
    
    // 9. sqlite3_bind_text
    if (param_idx <= 33) sqlite3_bind_text(stmt, param_idx++, get_text(Data, Size, &data_index, &text_len), text_len, dummy_destructor);
    
    // 10. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 11. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 12. sqlite3_bind_text
    if (param_idx <= 33) sqlite3_bind_text(stmt, param_idx++, get_text(Data, Size, &data_index, &text_len), text_len, dummy_destructor);
    
    // 13. sqlite3_bind_text
    if (param_idx <= 33) sqlite3_bind_text(stmt, param_idx++, get_text(Data, Size, &data_index, &text_len), text_len, dummy_destructor);
    
    // 14. sqlite3_bind_text
    if (param_idx <= 33) sqlite3_bind_text(stmt, param_idx++, get_text(Data, Size, &data_index, &text_len), text_len, dummy_destructor);
    
    // 15. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 16. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 17. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 18. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 19. sqlite3_bind_text
    if (param_idx <= 33) sqlite3_bind_text(stmt, param_idx++, get_text(Data, Size, &data_index, &text_len), text_len, dummy_destructor);
    
    // 20. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 21. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 22. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 23. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 24. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 25. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 26. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 27. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 28. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 29. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 30. sqlite3_bind_text
    if (param_idx <= 33) sqlite3_bind_text(stmt, param_idx++, get_text(Data, Size, &data_index, &text_len), text_len, dummy_destructor);
    
    // 31. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 32. sqlite3_libversion_number
    (void)sqlite3_libversion_number();
    
    // 33. sqlite3_bind_int
    if (param_idx <= 33) sqlite3_bind_int(stmt, param_idx++, get_int(Data, Size, &data_index));
    
    // 34. sqlite3_bind_text
    if (param_idx <= 33) sqlite3_bind_text(stmt, param_idx++, get_text(Data, Size, &data_index, &text_len), text_len, dummy_destructor);
    
    // 35. sqlite3_bind_text
    if (param_idx <= 33) sqlite3_bind_text(stmt, param_idx++, get_text(Data, Size, &data_index, &text_len), text_len, dummy_destructor);
    
    // Execute the statement to explore more states
    sqlite3_step(stmt);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}