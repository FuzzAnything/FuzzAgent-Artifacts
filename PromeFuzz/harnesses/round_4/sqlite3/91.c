// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:170771:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    // Limit input size to avoid excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }

    // Create a temporary database in memory
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Test sqlite3_complete and sqlite3_complete16
    if (Size > 0) {
        // For sqlite3_complete - needs null-terminated UTF-8 string
        char *sql_utf8 = malloc(Size + 1);
        if (sql_utf8) {
            memcpy(sql_utf8, Data, Size);
            sql_utf8[Size] = '\0';
            sqlite3_complete(sql_utf8);
            free(sql_utf8);
        }

        // For sqlite3_complete16 - needs null-terminated UTF-16 string
        // Allocate enough space for UTF-16 with null terminator
        size_t utf16_size = (Size + 1) * 2;
        void *sql_utf16 = malloc(utf16_size);
        if (sql_utf16) {
            // Simple conversion: treat each byte as low byte of UTF-16 char
            uint16_t *utf16_ptr = (uint16_t *)sql_utf16;
            for (size_t i = 0; i < Size; i++) {
                utf16_ptr[i] = (uint16_t)Data[i];
            }
            utf16_ptr[Size] = 0; // Null terminator
            sqlite3_complete16(sql_utf16);
            free(sql_utf16);
        }
    }

    // Test prepare functions with various parameters
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    const void *tail16 = NULL;

    // Prepare statements with different nByte values
    int nByte_options[] = {-1, 0, (int)Size, (int)Size/2};
    
    for (int i = 0; i < 4; i++) {
        int nByte = nByte_options[i];
        
        // Test sqlite3_prepare (legacy)
        if (Size > 0) {
            char *sql = malloc(Size + 1);
            if (sql) {
                memcpy(sql, Data, Size);
                sql[Size] = '\0';
                sqlite3_prepare(db, sql, nByte, &stmt, &tail);
                if (stmt) {
                    sqlite3_finalize(stmt);
                    stmt = NULL;
                }
                free(sql);
            }
        }

        // Test sqlite3_prepare_v3 with different flags
        unsigned int prepFlags_options[] = {0, 1, 2, 4};
        for (int j = 0; j < 4; j++) {
            if (Size > 0) {
                char *sql = malloc(Size + 1);
                if (sql) {
                    memcpy(sql, Data, Size);
                    sql[Size] = '\0';
                    sqlite3_prepare_v3(db, sql, nByte, prepFlags_options[j], &stmt, &tail);
                    if (stmt) {
                        sqlite3_finalize(stmt);
                        stmt = NULL;
                    }
                    free(sql);
                }
            }
        }

        // Test UTF-16 prepare functions
        if (Size > 0) {
            // Prepare UTF-16 string
            size_t utf16_size = (Size + 1) * 2;
            void *sql_utf16 = malloc(utf16_size);
            if (sql_utf16) {
                uint16_t *utf16_ptr = (uint16_t *)sql_utf16;
                for (size_t k = 0; k < Size; k++) {
                    utf16_ptr[k] = (uint16_t)Data[k];
                }
                utf16_ptr[Size] = 0;

                // Test sqlite3_prepare16
                sqlite3_prepare16(db, sql_utf16, nByte * 2, &stmt, &tail16);
                if (stmt) {
                    sqlite3_finalize(stmt);
                    stmt = NULL;
                }

                // Test sqlite3_prepare16_v2
                sqlite3_prepare16_v2(db, sql_utf16, nByte * 2, &stmt, &tail16);
                if (stmt) {
                    sqlite3_finalize(stmt);
                    stmt = NULL;
                }

                free(sql_utf16);
            }
        }
    }

    // Clean up
    sqlite3_close(db);
    
    return 0;
}