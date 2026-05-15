// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    // Create a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    // Test sqlite3_str_new with NULL database
    sqlite3_str *str1 = sqlite3_str_new(NULL);
    if (str1) {
        // Test sqlite3_str_appendf with various format strings using fuzzer data
        if (Size > 0) {
            // Use first byte to determine format string
            uint8_t format_selector = Data[0];
            switch (format_selector % 4) {
                case 0:
                    sqlite3_str_appendf(str1, "Test: %d", (int)(Size % 100));
                    break;
                case 1:
                    sqlite3_str_appendf(str1, "Size: %lu", (unsigned long)Size);
                    break;
                case 2:
                    if (Size > 1) {
                        // Create a null-terminated string from fuzzer data
                        size_t str_len = Size > 100 ? 100 : Size - 1;
                        char *temp_str = malloc(str_len + 1);
                        if (temp_str) {
                            memcpy(temp_str, Data + 1, str_len);
                            temp_str[str_len] = '\0';
                            sqlite3_str_appendf(str1, "Data: %s", temp_str);
                            free(temp_str);
                        }
                    } else {
                        sqlite3_str_appendf(str1, "Empty data");
                    }
                    break;
                default:
                    sqlite3_str_appendf(str1, "Hello %s!", "World");
                    break;
            }
        } else {
            sqlite3_str_appendf(str1, "Empty input");
        }

        // Test sqlite3_str_value
        char *value = sqlite3_str_value(str1);
        // value may be NULL if string is empty, that's okay

        // Test sqlite3_str_reset
        sqlite3_str_reset(str1);
        
        // Append something else after reset - use safe format specifiers
        sqlite3_str_appendf(str1, "After reset: %lu", (unsigned long)Size);

        // Test sqlite3_str_finish
        char *finished_str = sqlite3_str_finish(str1);
        if (finished_str) {
            sqlite3_free(finished_str);
        }
        // str1 is now invalid, don't use it
    }

    // Test another sqlite3_str with multiple operations
    sqlite3_str *str2 = sqlite3_str_new(NULL);
    if (str2) {
        // Append multiple times with safe format specifiers
        for (size_t i = 0; i < (Size % 10) + 1 && i < 10; i++) {
            sqlite3_str_appendf(str2, "Iteration %lu: ", (unsigned long)i);
            if (Size > i) {
                sqlite3_str_appendf(str2, "0x%02x ", Data[i % Size]);
            }
        }

        // Test sqlite3_str_value again
        char *value2 = sqlite3_str_value(str2);
        
        // Test sqlite3_str_free (alternative to finish)
        sqlite3_str_free(str2);
    }

    // Test with empty string operations
    sqlite3_str *str3 = sqlite3_str_new(NULL);
    if (str3) {
        // Don't append anything
        char *value3 = sqlite3_str_value(str3);  // Should be NULL or empty string
        
        // Finish empty string
        char *finished_empty = sqlite3_str_finish(str3);
        if (finished_empty) {
            sqlite3_free(finished_empty);
        }
    }

    // Test reset on newly created string
    sqlite3_str *str4 = sqlite3_str_new(NULL);
    if (str4) {
        sqlite3_str_reset(str4);  // Reset even without content
        sqlite3_str_free(str4);
    }

    // Test sequence of operations
    sqlite3_str *str5 = sqlite3_str_new(NULL);
    if (str5) {
        sqlite3_str_appendf(str5, "Start: ");
        sqlite3_str_reset(str5);
        sqlite3_str_appendf(str5, "Restart with size %lu", (unsigned long)Size);
        sqlite3_str_reset(str5);
        sqlite3_str_appendf(str5, "Final");
        
        char *final_value = sqlite3_str_value(str5);
        char *final_finished = sqlite3_str_finish(str5);
        if (final_finished) {
            sqlite3_free(final_finished);
        }
    }

    return 0;
}