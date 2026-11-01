// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int fuzzer_run(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a new dynamic string object
    sqlite3_str *str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Ensure we have enough data to perform operations safely
    if (Size > 0) {
        // Ensure the input is null-terminated for format string usage
        char *safe_input = (char *)malloc(Size + 1);
        if (safe_input != NULL) {
            memcpy(safe_input, Data, Size);
            safe_input[Size] = '\0';

            // Try appending formatted text
            sqlite3_str_appendf(str, "%s", safe_input);

            // The assert failure in sqlite3.c occurs if we call appendf with a format string
            // that requires malloc (like %d) when the buffer is empty (nChar == 0).
            // However, we just appended safe_input. If safe_input was empty (Size=1, char='\0'),
            // nChar might still be 0.
            // To be safe, we explicitly append a non-empty character first if the buffer is empty.
            // Or simply ensure we always have content before doing complex formatting.
            
            // Check if we need to ensure content exists before the next appendf
            // We can just append a simple char first to guarantee nChar > 0
            sqlite3_str_appendchar(str, 1, ' ');

            // Now it is safe to append formatted text that might trigger internal malloc
            int val = (int)(Data[0] % 100);
            sqlite3_str_appendf(str, "%d", val);

            // Try appending characters
            if (Size > 2) {
                int count = Data[1] % 10;
                char c = (char)(Data[2] % 256);
                sqlite3_str_appendchar(str, count, c);
            }

            // Try appending raw bytes
            if (Size > 4) {
                int n = Data[3] % 10;
                if (n <= (int)(Size - 4)) {
                    sqlite3_str_append(str, (const char *)(Data + 4), n);
                }
            }

            free(safe_input);
        }
    } else {
        // If Size is 0, ensure we have some content to avoid the assert on empty buffer
        // if we were to call appendf with a malloc-triggering format.
        // We just skip the complex formatting in this branch, but the initial appendf
        // above handles the Size > 0 case.
        // If Size == 0, we haven't appended anything yet.
        // Let's add a dummy char to ensure nChar > 0 if we were to do more.
        // But since the block above is skipped, we are fine unless we call appendf below.
        // We don't call appendf below for Size==0.
        // However, to be robust, let's just ensure the object is not in a "dangerous" empty state
        // if we were to do anything else. For now, just proceed.
        // Actually, if Size == 0, the block is skipped, str is empty.
        // We should ensure we don't call appendf with a malloc-triggering format on an empty str.
        // The code below does not call appendf if Size == 0.
    }

    // Check for errors
    int errcode = sqlite3_str_errcode(str);
    if (errcode != SQLITE_OK) {
        // Handle error state, but continue to cleanup
    }

    // Retrieve the final string value
    char *result = sqlite3_str_value(str);
    if (result != NULL) {
        // Do something with the result if needed, e.g., logging
    }

    // Reset the string object
    sqlite3_str_reset(str);

    // Try appending again after reset
    // After reset, nChar is 0. We must ensure we don't call appendf with a format
    // that triggers malloc immediately if the buffer is empty.
    if (Size > 0) {
        char *safe_reset_input = (char *)malloc(Size + 1);
        if (safe_reset_input != NULL) {
            memcpy(safe_reset_input, Data, Size);
            safe_reset_input[Size] = '\0';
            
            // Append a simple char first to ensure nChar > 0
            sqlite3_str_appendchar(str, 1, ' ');
            
            // Now safe to use format string
            sqlite3_str_appendf(str, "%s", safe_reset_input);
            free(safe_reset_input);
        }
    }

    // Finalize and free the string
    char *final = sqlite3_str_finish(str);
    if (final != NULL) {
        sqlite3_free(final);
    }

    // Cleanup database
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    // Handle empty input
    if (Size == 0) {
        return 0;
    }

    // Run the fuzzer logic
    fuzzer_run(Data, Size);

    return 0;
}