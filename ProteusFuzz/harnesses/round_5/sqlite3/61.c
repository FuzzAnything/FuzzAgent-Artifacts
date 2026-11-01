// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:185515:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:185350:16 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:184486:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * 1. sqlite3_prepare16_v3
 * 2. sqlite3_complete
 * 3. sqlite3_keyword_name
 * 4. sqlite3_keyword_count
 * 5. sqlite3_keyword_check
 * 6. sqlite3_complete16
 */

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    int rc;

    /* Initialize database connection in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /*
     * Fuzz sqlite3_prepare16_v3
     * Input: Data is treated as raw bytes. We interpret it as UTF-16LE for simplicity.
     * We need to ensure the input is at least 2 bytes to form a valid UTF-16 char.
     */
    if (Size >= 2) {
        /* 
         * Calculate buffer size needed for UTF-16.
         * Size is in bytes. We need (Size + 1) bytes to hold the data + null terminator byte.
         * Then round up to even number of bytes for UTF-16 alignment.
         * Actually, simpler: allocate Size + 2 bytes to be safe for null termination.
         * Then treat as uint16_t array.
         */
        size_t allocBytes = Size + 2; 
        uint16_t *utf16Buf = (uint16_t *)malloc(allocBytes);
        if (utf16Buf) {
            /* Copy input as raw bytes */
            memcpy(utf16Buf, Data, Size);
            
            /* Null terminate the UTF-16 string */
            /* We need to write a 16-bit zero. 
               Index in uint16_t array is (Size + 1) / 2? 
               No, we just want to ensure the byte after the data is 0, 
               and if Size is odd, the next byte is also 0.
               Safest: set the last two bytes of the allocated buffer to 0.
            */
            size_t lastIndex = (allocBytes / sizeof(uint16_t)) - 1;
            utf16Buf[lastIndex] = 0;
            if (allocBytes > sizeof(uint16_t)) {
                utf16Buf[lastIndex - 1] = 0;
            }

            /* Call sqlite3_prepare16_v3 with various flags */
            unsigned int flags = 0;
            if (Size > 0) {
                flags = Data[0] & 0x0F; /* Use first byte to determine flags */
            }

            /* Pass Size as nByte (bytes), not chars */
            rc = sqlite3_prepare16_v3(db, utf16Buf, (int)Size, flags, &stmt, &pzTail);
            if (rc == SQLITE_OK && stmt) {
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
            free(utf16Buf);
        }
    }

    /*
     * Fuzz sqlite3_complete
     * Input: Data is treated as a UTF-8 string. We ensure it's null-terminated.
     */
    if (Size > 0) {
        char *utf8Buf = (char *)malloc(Size + 1);
        if (utf8Buf) {
            memcpy(utf8Buf, Data, Size);
            utf8Buf[Size] = '\0';
            rc = sqlite3_complete(utf8Buf);
            (void)rc; /* Suppress unused variable warning */
            free(utf8Buf);
        }
    }

    /*
     * Fuzz sqlite3_keyword_count and sqlite3_keyword_name
     * These are stateless regarding input data, but we can iterate based on input.
     */
    int keywordCount = sqlite3_keyword_count();
    if (keywordCount > 0 && Size > 0) {
        /* Use input data to determine which keyword index to check */
        int index = Data[0] % keywordCount;
        const char *keywordName = NULL;
        int keywordLen = 0;
        rc = sqlite3_keyword_name(index, &keywordName, &keywordLen);
        if (rc == SQLITE_OK && keywordName) {
            /* Optionally verify the keyword */
            int isKeyword = sqlite3_keyword_check(keywordName, keywordLen);
            (void)isKeyword;
        }
    }

    /*
     * Fuzz sqlite3_keyword_check
     * Use a portion of the input as a potential keyword.
     */
    if (Size >= 2) {
        int checkLen = Data[0] % (Size - 1);
        if (checkLen > 0) {
            int isKeyword = sqlite3_keyword_check((const char *)(Data + 1), checkLen);
            (void)isKeyword;
        }
    }

    /*
     * Fuzz sqlite3_complete16
     * Similar to sqlite3_prepare16_v3, treat input as UTF-16LE.
     */
    if (Size >= 2) {
        size_t allocBytes = Size + 2; 
        uint16_t *utf16Buf = (uint16_t *)malloc(allocBytes);
        if (utf16Buf) {
            memcpy(utf16Buf, Data, Size);
            
            /* Null terminate */
            size_t lastIndex = (allocBytes / sizeof(uint16_t)) - 1;
            utf16Buf[lastIndex] = 0;
            if (allocBytes > sizeof(uint16_t)) {
                utf16Buf[lastIndex - 1] = 0;
            }

            rc = sqlite3_complete16(utf16Buf);
            (void)rc;
            free(utf16Buf);
        }
    }

    /* Cleanup */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    return 0;
}