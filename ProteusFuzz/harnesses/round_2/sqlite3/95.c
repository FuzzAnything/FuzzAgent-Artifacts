// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:189330:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:185515:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:147332:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int dummy_collation_compare(void *pArg, int nLeft, const void *pLeft, int nRight, const void *pRight) {
    if (nLeft == 0 || nRight == 0) return 0;
    return memcmp(pLeft, pRight, nLeft < nRight ? nLeft : nRight);
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    int rc;

    // Create a dummy file for the database
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Convert input to UTF-16 for filename and SQL
    // We need to ensure the buffer is large enough for the input bytes + null terminator
    // UTF-16 uses 2 bytes per character (assuming wchar_t is 2 bytes on this platform, 
    // but we allocate based on Size * sizeof(wchar_t) to be safe)
    size_t utf16_len = Size + 1; // +1 for null terminator
    wchar_t *utf16_filename = (wchar_t *)malloc(utf16_len * sizeof(wchar_t));
    wchar_t *utf16_sql = (wchar_t *)malloc(utf16_len * sizeof(wchar_t));
    
    if (!utf16_filename || !utf16_sql) {
        free(utf16_filename);
        free(utf16_sql);
        remove("./dummy_file");
        return 0;
    }

    // Convert bytes to UTF-16 (simple byte-to-char mapping for fuzzing)
    for (size_t i = 0; i < Size; i++) {
        utf16_filename[i] = (wchar_t)Data[i];
        utf16_sql[i] = (wchar_t)Data[i];
    }
    utf16_filename[Size] = L'\0';
    utf16_sql[Size] = L'\0';

    // 1. Open database using sqlite3_open16
    rc = sqlite3_open16((const void *)utf16_filename, &db);
    if (rc != SQLITE_OK) {
        free(utf16_filename);
        free(utf16_sql);
        remove("./dummy_file");
        return 0;
    }

    // 2. Create a collation using sqlite3_create_collation16
    // Use a small part of the input as the collation name
    if (Size > 4) {
        wchar_t coll_name[8];
        memset(coll_name, 0, sizeof(coll_name));
        for (int i = 0; i < 4 && i < (int)Size; i++) {
            coll_name[i] = (wchar_t)Data[i];
        }
        sqlite3_create_collation16(db, (const void *)coll_name, SQLITE_UTF16LE, NULL, dummy_collation_compare);
    }

    // 3. Check completeness using sqlite3_complete16
    sqlite3_complete16((const void *)utf16_sql);

    // 4. Prepare statement using sqlite3_prepare16_v2
    // nByte is the length in bytes of the UTF-16 string
    int nByte = (int)(Size * sizeof(wchar_t)); 
    
    rc = sqlite3_prepare16_v2(db, (const void *)utf16_sql, nByte, &stmt, &pzTail);
    
    if (rc == SQLITE_OK && stmt) {
        // 5. Retrieve column names using sqlite3_column_name16
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++) {
            const void *name = sqlite3_column_name16(stmt, i);
            (void)name; // Use the pointer to avoid unused warning
        }
        
        // Try to step to potentially trigger more states if data allows
        sqlite3_step(stmt);
    }

    // 6. Also try sqlite3_prepare16 (older version)
    sqlite3_stmt *stmt2 = NULL;
    rc = sqlite3_prepare16(db, (const void *)utf16_sql, nByte, &stmt2, &pzTail);
    if (rc == SQLITE_OK && stmt2) {
        int col_count = sqlite3_column_count(stmt2);
        for (int i = 0; i < col_count; i++) {
            const void *name = sqlite3_column_name16(stmt2, i);
            (void)name;
        }
        sqlite3_finalize(stmt2);
    }

    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    free(utf16_filename);
    free(utf16_sql);
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}