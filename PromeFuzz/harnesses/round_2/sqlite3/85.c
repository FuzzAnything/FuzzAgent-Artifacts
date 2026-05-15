// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:127713:17 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:127685:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:127685:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
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

static void dummy_extension_entry(void) {
    // Dummy extension entry point for testing
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    // Initialize SQLite library
    int init_ret = sqlite3_initialize();
    if (init_ret != SQLITE_OK) {
        // If initialization fails, we can't proceed with most operations
        return 0;
    }

    // Create a database connection for testing
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_shutdown();
        return 0;
    }

    // Use fuzzer input to determine which operations to perform
    // We'll use the first byte to select operation type
    if (Size > 0) {
        uint8_t op_type = Data[0] % 7; // 7 different operation types
        
        switch (op_type) {
            case 0:
                // Test sqlite3_enable_load_extension with random onoff value
                {
                    int onoff = (Size > 1) ? (Data[1] % 2) : 0;
                    sqlite3_enable_load_extension(db, onoff);
                }
                break;
                
            case 1:
                // Test sqlite3_auto_extension with dummy function
                sqlite3_auto_extension(dummy_extension_entry);
                break;
                
            case 2:
                // Test sqlite3_reset_auto_extension
                sqlite3_reset_auto_extension();
                break;
                
            case 3:
                // Test sqlite3_cancel_auto_extension
                sqlite3_cancel_auto_extension(dummy_extension_entry);
                break;
                
            case 4:
                // Test combination: auto_extension then cancel
                sqlite3_auto_extension(dummy_extension_entry);
                sqlite3_cancel_auto_extension(dummy_extension_entry);
                break;
                
            case 5:
                // Test multiple auto_extensions
                sqlite3_auto_extension(dummy_extension_entry);
                // Try to add same extension again (should be no-op)
                sqlite3_auto_extension(dummy_extension_entry);
                break;
                
            case 6:
                // Test enable_load_extension with different values
                if (Size > 1) {
                    sqlite3_enable_load_extension(db, Data[1] % 2);
                    if (Size > 2) {
                        sqlite3_enable_load_extension(db, Data[2] % 2);
                    }
                }
                break;
        }
    }

    // Clean up
    sqlite3_close(db);
    
    // Test shutdown and re-initialize
    sqlite3_shutdown();
    
    // Try to initialize again (should be harmless no-op if already initialized)
    sqlite3_initialize();
    
    // Try shutdown again (should be harmless no-op)
    sqlite3_shutdown();

    return 0;
}