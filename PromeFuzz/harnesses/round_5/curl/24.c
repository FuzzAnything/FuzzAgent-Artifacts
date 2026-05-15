// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mprintf.h"
#include <stdarg.h>

// Wrapper functions for va_list APIs
static void call_curl_mvsprintf(const char* format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void call_curl_mvfprintf(const char* format, ...) {
    FILE *dummy_file = fopen("./dummy_file", "a");
    if (!dummy_file) return;
    
    va_list args;
    va_start(args, format);
    curl_mvfprintf(dummy_file, format, args);
    va_end(args);
    
    fclose(dummy_file);
}

static void call_curl_mvsnprintf(const char* format, size_t maxlen, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, maxlen);
    curl_mvsnprintf(buffer, maxlen, format, args);
    va_end(args);
}

static void call_curl_mvprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use first byte to determine format string type
    uint8_t format_type = Data[0] % 6;
    const char *format_strings[] = {
        "%s",
        "%d",
        "%x",
        "%c",
        "%.*s",
        "%d %s"
    };
    
    const char *format = format_strings[format_type];
    
    // Prepare arguments based on remaining data
    size_t data_offset = 1;
    size_t remaining = Size > 1 ? Size - 1 : 0;
    
    // Prepare string argument
    const char *str_arg = remaining > 0 ? (const char *)(Data + data_offset) : "";
    size_t str_len = remaining < 100 ? remaining : 100;
    char temp_str[101];
    if (remaining > 0) {
        memcpy(temp_str, str_arg, str_len);
        temp_str[str_len] = '\0';
    } else {
        temp_str[0] = '\0';
    }
    
    int int_arg = remaining > 0 ? (int)Data[data_offset % (remaining > 0 ? remaining : 1)] : 0;
    
    // 1. Test curl_mvsprintf using wrapper
    if (remaining > 0) {
        if (strstr(format, "%d") != NULL && strstr(format, "%s") != NULL) {
            call_curl_mvsprintf(format, int_arg, temp_str);
        } else if (strstr(format, "%d") != NULL) {
            call_curl_mvsprintf(format, int_arg);
        } else if (strstr(format, "%s") != NULL) {
            call_curl_mvsprintf(format, temp_str);
        } else if (strstr(format, "%c") != NULL) {
            call_curl_mvsprintf(format, (char)int_arg);
        } else if (strstr(format, "%x") != NULL) {
            call_curl_mvsprintf(format, int_arg);
        } else if (strstr(format, "%.*s") != NULL) {
            call_curl_mvsprintf(format, (int)str_len, temp_str);
        }
    }
    
    // 2. Test curl_mvfprintf using wrapper
    if (remaining > 0) {
        if (strstr(format, "%d") != NULL && strstr(format, "%s") != NULL) {
            call_curl_mvfprintf(format, int_arg, temp_str);
        } else if (strstr(format, "%d") != NULL) {
            call_curl_mvfprintf(format, int_arg);
        } else if (strstr(format, "%s") != NULL) {
            call_curl_mvfprintf(format, temp_str);
        } else if (strstr(format, "%c") != NULL) {
            call_curl_mvfprintf(format, (char)int_arg);
        } else if (strstr(format, "%x") != NULL) {
            call_curl_mvfprintf(format, int_arg);
        } else if (strstr(format, "%.*s") != NULL) {
            call_curl_mvfprintf(format, (int)str_len, temp_str);
        }
    }
    
    // 3. Test curl_mvsnprintf using wrapper
    if (remaining > 0) {
        size_t maxlen = (remaining > 0 && Data[data_offset % remaining] % 500) + 1;
        
        if (strstr(format, "%d") != NULL && strstr(format, "%s") != NULL) {
            call_curl_mvsnprintf(format, maxlen, int_arg, temp_str);
        } else if (strstr(format, "%d") != NULL) {
            call_curl_mvsnprintf(format, maxlen, int_arg);
        } else if (strstr(format, "%s") != NULL) {
            call_curl_mvsnprintf(format, maxlen, temp_str);
        } else if (strstr(format, "%c") != NULL) {
            call_curl_mvsnprintf(format, maxlen, (char)int_arg);
        } else if (strstr(format, "%x") != NULL) {
            call_curl_mvsnprintf(format, maxlen, int_arg);
        } else if (strstr(format, "%.*s") != NULL) {
            call_curl_mvsnprintf(format, maxlen, (int)str_len, temp_str);
        }
    }
    
    // 4. Test curl_mprintf directly
    if (remaining > 0) {
        switch(format_type) {
            case 0: curl_mprintf(format, temp_str); break;
            case 1: curl_mprintf(format, int_arg); break;
            case 2: curl_mprintf(format, int_arg); break;
            case 3: curl_mprintf(format, (char)int_arg); break;
            case 4: curl_mprintf(format, (int)str_len, temp_str); break;
            case 5: curl_mprintf(format, int_arg, temp_str); break;
        }
    }
    
    // 5. Test curl_mvprintf using wrapper
    if (remaining > 0) {
        if (strstr(format, "%d") != NULL && strstr(format, "%s") != NULL) {
            call_curl_mvprintf(format, int_arg, temp_str);
        } else if (strstr(format, "%d") != NULL) {
            call_curl_mvprintf(format, int_arg);
        } else if (strstr(format, "%s") != NULL) {
            call_curl_mvprintf(format, temp_str);
        } else if (strstr(format, "%c") != NULL) {
            call_curl_mvprintf(format, (char)int_arg);
        } else if (strstr(format, "%x") != NULL) {
            call_curl_mvprintf(format, int_arg);
        } else if (strstr(format, "%.*s") != NULL) {
            call_curl_mvprintf(format, (int)str_len, temp_str);
        }
    }
    
    // 6. Test curl_msprintf directly
    if (remaining > 0) {
        char buffer2[1024];
        switch(format_type) {
            case 0: curl_msprintf(buffer2, format, temp_str); break;
            case 1: curl_msprintf(buffer2, format, int_arg); break;
            case 2: curl_msprintf(buffer2, format, int_arg); break;
            case 3: curl_msprintf(buffer2, format, (char)int_arg); break;
            case 4: curl_msprintf(buffer2, format, (int)str_len, temp_str); break;
            case 5: curl_msprintf(buffer2, format, int_arg, temp_str); break;
        }
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}