// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Call sqlite3_compileoption_used first
    if (Size > 0) {
        // Use first part of input as option name (null-terminated)
        size_t opt_len = Size > 100 ? 100 : Size;
        char *opt_name = malloc(opt_len + 1);
        if (opt_name) {
            memcpy(opt_name, Data, opt_len);
            opt_name[opt_len] = '\0';
            sqlite3_compileoption_used(opt_name);
            free(opt_name);
        }
    } else {
        sqlite3_compileoption_used(NULL);
    }

    // Prepare for sqlite3_open_v2
    write_dummy_file(Data, Size);
    
    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";
    
    // Use different flags combinations based on input
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        // Derive flags from input data for more diverse testing
        uint8_t flag_selector = Data[0];
        flags = 0;
        
        if (flag_selector & 0x01) flags |= SQLITE_OPEN_READONLY;
        if (flag_selector & 0x02) flags |= SQLITE_OPEN_READWRITE;
        if (flag_selector & 0x04) flags |= SQLITE_OPEN_CREATE;
        if (flag_selector & 0x08) flags |= SQLITE_OPEN_MEMORY;
        if (flag_selector & 0x10) flags |= SQLITE_OPEN_NOMUTEX;
        if (flag_selector & 0x20) flags |= SQLITE_OPEN_FULLMUTEX;
        if (flag_selector & 0x40) flags |= SQLITE_OPEN_SHAREDCACHE;
        if (flag_selector & 0x80) flags |= SQLITE_OPEN_PRIVATECACHE;
    }
    
    // Call sqlite3_open_v2
    int rc = sqlite3_open_v2(filename, &db, flags, NULL);
    
    // Call sqlite3_close regardless of open result
    if (db) {
        sqlite3_close(db);
    }
    
    // Clean up
    remove_dummy_file();
    
    return 0;
}