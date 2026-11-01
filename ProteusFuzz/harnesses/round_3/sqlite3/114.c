// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:33215:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  sqlite3 *db = NULL;
  sqlite3_str *str = NULL;
  int rc;

  // Initialize database connection (optional but good for lookaside)
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

  // Parse input data to drive fuzzing logic
  size_t offset = 0;
  while (offset < Size) {
    uint8_t op = Data[offset] % 10;
    offset++;

    switch (op) {
      case 0: {
        // sqlite3_str_appendf
        // Need a format string and an argument
        if (offset + 2 < Size) {
          int argVal = (int)(Data[offset] * 256 + Data[offset + 1]);
          offset += 2;
          // Create a safe format string based on input byte
          char fmt[32];
          int fmtLen = (Data[offset % Size]) % 10;
          if (fmtLen > 10) fmtLen = 10;
          fmt[0] = '%';
          if (fmtLen > 1) {
            fmt[1] = (Data[(offset+1) % Size] % 3 == 0) ? 'd' :
                     (Data[(offset+1) % Size] % 3 == 1) ? 's' : 'c';
            fmt[2] = '\0';
          } else {
            fmt[1] = '\0';
          }
          sqlite3_str_appendf(str, fmt, argVal);
        }
        break;
      }
      case 1: {
        // sqlite3_str_append
        if (offset + 1 < Size) {
          int n = (int)Data[offset];
          offset++;
          // Use a static buffer for the content to append
          static char buffer[256];
          memset(buffer, 'A', sizeof(buffer));
          // Ensure n is within bounds of our static buffer
          if (n > (int)sizeof(buffer)) n = (int)sizeof(buffer);
          if (n < 0) n = 0;
          sqlite3_str_append(str, buffer, n);
        }
        break;
      }
      case 2: {
        // sqlite3_str_appendchar
        if (offset + 1 < Size) {
          int n = (int)Data[offset];
          char c = (char)Data[offset + 1];
          offset += 2;
          // Limit n to prevent excessive memory allocation
          if (n > 10000) n = 10000;
          if (n < 0) n = 0;
          sqlite3_str_appendchar(str, n, c);
        }
        break;
      }
      case 3: {
        // sqlite3_str_truncate
        if (offset < Size) {
          int n = (int)Data[offset];
          offset++;
          // Allow negative to test no-op behavior
          sqlite3_str_truncate(str, n);
        }
        break;
      }
      case 4: {
        // sqlite3_str_length (read-only)
        int len = sqlite3_str_length(str);
        // Use len to influence next operation if possible, or just ignore
        (void)len;
        break;
      }
      case 5: {
        // sqlite3_str_errcode (read-only)
        int err = sqlite3_str_errcode(str);
        (void)err;
        break;
      }
      case 6: {
        // Reset string
        sqlite3_str_reset(str);
        break;
      }
      case 7: {
        // Append a zero-length string
        sqlite3_str_append(str, "", 0);
        break;
      }
      case 8: {
        // Truncate to 0
        sqlite3_str_truncate(str, 0);
        break;
      }
      case 9: {
        // Append a single char
        if (offset < Size) {
          char c = (char)Data[offset];
          offset++;
          sqlite3_str_appendchar(str, 1, c);
        }
        break;
      }
    }

    // Check for errors periodically
    if (sqlite3_str_errcode(str) != SQLITE_OK) {
      // If error, we can stop appending to this string or reset
      // For fuzzing, let's just reset and continue to explore other states
      sqlite3_str_reset(str);
    }
  }

  // Cleanup
  sqlite3_str_finish(str);
  sqlite3_close(db);

  return 0;
}