// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:32182:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

static void helper_vappendf(sqlite3_str *str, const char *f, ...) {
    va_list args;
    va_start(args, f);
    sqlite3_str_vappendf(str, f, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Initialize sqlite3_str
    sqlite3_str *s = sqlite3_str_new(NULL);
    if (!s) {
        return 0;
    }

    // Use input data to drive fuzzing
    // We interpret the input as a sequence of commands
    // Command 0: appendf
    // Command 1: vappendf
    // Command 2: appendall
    // Command 3: append
    // Command 4: appendchar
    // Command 5: reset
    // Command 6: truncate
    // Command 7: value (read)
    
    size_t idx = 0;
    while (idx < Size) {
        uint8_t cmd = Data[idx] % 8;
        idx++;

        if (cmd == 0 && idx < Size) {
            // sqlite3_str_appendf
            // We need a format string. We'll use a safe subset of the input.
            size_t len = (Size - idx) > 64 ? 64 : (Size - idx);
            if (len > 0) {
                char *fmt = (char *)malloc(len + 1);
                if (fmt) {
                    memcpy(fmt, Data + idx, len);
                    fmt[len] = '\0';
                    // To avoid va_start issues, we use a simple format that doesn't require args
                    // or we construct a safe call. Since we can't easily pass args from fuzz data
                    // without complex parsing, we'll just use a format that expects no args
                    // or we replace % with %% to escape them.
                    for (size_t i = 0; i < len; i++) {
                        if (fmt[i] == '%') {
                            fmt[i] = ' '; // Replace % with space to avoid format string errors
                        }
                    }
                    sqlite3_str_appendf(s, fmt);
                    free(fmt);
                    idx += len;
                }
            }
        } else if (cmd == 1 && idx < Size) {
            // sqlite3_str_vappendf
            // We call the helper which handles va_start correctly
            size_t len = (Size - idx) > 64 ? 64 : (Size - idx);
            if (len > 0) {
                char *fmt = (char *)malloc(len + 1);
                if (fmt) {
                    memcpy(fmt, Data + idx, len);
                    fmt[len] = '\0';
                    // Sanitize format string
                    for (size_t i = 0; i < len; i++) {
                        if (fmt[i] == '%') {
                            fmt[i] = ' ';
                        }
                    }
                    helper_vappendf(s, fmt);
                    free(fmt);
                    idx += len;
                }
            }
        } else if (cmd == 2 && idx < Size) {
            // sqlite3_str_appendall
            size_t len = 0;
            while (idx + len < Size && Data[idx + len] != 0) {
                len++;
            }
            if (len > 0) {
                char *str = (char *)malloc(len + 1);
                if (str) {
                    memcpy(str, Data + idx, len);
                    str[len] = '\0';
                    sqlite3_str_appendall(s, str);
                    free(str);
                    idx += len + 1;
                } else {
                    idx += len;
                }
            } else {
                idx++;
            }
        } else if (cmd == 3 && idx + 1 < Size) {
            // sqlite3_str_append
            uint8_t n = Data[idx];
            idx++;
            if (n > 0 && idx + n <= Size) {
                sqlite3_str_append(s, (const char *)(Data + idx), n);
                idx += n;
            } else {
                idx = Size;
            }
        } else if (cmd == 4 && idx + 1 < Size) {
            // sqlite3_str_appendchar
            uint8_t n = Data[idx];
            idx++;
            if (idx < Size) {
                uint8_t c = Data[idx];
                idx++;
                sqlite3_str_appendchar(s, n, c);
            }
        } else if (cmd == 5) {
            // sqlite3_str_reset
            sqlite3_str_reset(s);
        } else if (cmd == 6 && idx < Size) {
            // sqlite3_str_truncate
            uint32_t n = Data[idx];
            idx++;
            sqlite3_str_truncate(s, n);
        } else if (cmd == 7) {
            // sqlite3_str_value
            char *val = sqlite3_str_value(s);
            (void)val;
        }

        // Check for errors
        if (sqlite3_str_errcode(s) != 0) {
            sqlite3_str_reset(s);
        }
    }

    // Finalize the string
    char *result = sqlite3_str_finish(s);
    if (result) {
        sqlite3_free(result);
    }

    return 0;
}