// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_length at sqlite3.c:33215:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

/*
 * The fuzzing entry point.
 * Note: The function must be visible to the linker. Do not use 'static' for this function.
 */
int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    int rc;
    char *result;
    int i;

    // Initialize database connection (optional for sqlite3_str, but good practice)
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a new dynamic string object
    str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Fuzzing logic based on input data
    for (i = 0; i < Size; i++) {
        uint8_t op = Data[i] % 7;

        switch (op) {
            case 0:
                // sqlite3_str_appendchar
                if (i + 1 < Size) {
                    int nChars = Data[i + 1];
                    char cChar = (i + 2 < Size) ? (char)Data[i + 2] : 'A';
                    sqlite3_str_appendchar(str, nChars, cChar);
                    if (i + 2 < Size) i += 2;
                }
                break;

            case 1:
                // sqlite3_str_append
                if (i + 1 < Size) {
                    int nAppend = Data[i + 1];
                    if (i + 1 + nAppend <= Size && nAppend > 0) {
                        sqlite3_str_append(str, (const char *)(Data + i + 1), nAppend);
                        i += nAppend;
                    }
                }
                break;

            case 2:
                // sqlite3_str_truncate
                if (i + 1 < Size) {
                    int nTruncate = Data[i + 1];
                    sqlite3_str_truncate(str, nTruncate);
                    i++;
                }
                break;

            case 3:
                // Check error code
                rc = sqlite3_str_errcode(str);
                // No action needed, just checking state
                break;

            case 4:
                // Get length
                (void)sqlite3_str_length(str);
                // No action needed, just checking state
                break;

            case 5:
                // Get value
                result = sqlite3_str_value(str);
                if (result != NULL) {
                    // Do nothing with result to avoid use-after-free issues
                }
                break;

            case 6:
                // Append a single character from input
                sqlite3_str_appendchar(str, 1, (char)Data[i]);
                break;
        }
    }

    // Finalize the string to prevent memory leaks
    result = sqlite3_str_finish(str);
    if (result != NULL) {
        sqlite3_free(result);
    }

    // Cleanup database connection
    sqlite3_close(db);

    return 0;
}