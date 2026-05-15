// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    // Use the fuzzer input to influence behavior
    // We'll use the first few bytes to determine which operations to perform
    
    if (Size == 0) {
        return 0;
    }
    
    // Use the first byte to determine which operations to perform
    uint8_t operation = Data[0];
    
    // Operation 0: Basic initialization and shutdown sequence
    if ((operation & 0x01) != 0) {
        // Call initialize and shutdown in various orders
        if ((operation & 0x02) != 0) {
            sqlite3_initialize();
        }
        
        sqlite3_shutdown();
        
        if ((operation & 0x04) != 0) {
            sqlite3_initialize();
        }
        
        // Shutdown is idempotent, call multiple times
        sqlite3_shutdown();
        sqlite3_shutdown();
    }
    
    // Operation 1: Thread-related functions
    if ((operation & 0x08) != 0) {
        // Call thread-related functions
        sqlite3_thread_cleanup();  // No-op, but safe to call
        
        int threadsafe = sqlite3_threadsafe();
        (void)threadsafe;  // Use the value to avoid compiler warnings
        
        // Call thread_cleanup multiple times (it's a no-op)
        sqlite3_thread_cleanup();
        sqlite3_thread_cleanup();
    }
    
    // Operation 2: Shared cache configuration
    if ((operation & 0x10) != 0 && Size > 1) {
        // Use second byte as parameter for enable_shared_cache
        int enable = Data[1] & 0x01;
        sqlite3_enable_shared_cache(enable);
        
        // Try the opposite value
        sqlite3_enable_shared_cache(!enable);
        
        // Try with different values
        if (Size > 2) {
            sqlite3_enable_shared_cache(Data[2] & 0x01);
        }
    }
    
    // Operation 3: sqlite3_config with various options
    if ((operation & 0x20) != 0 && Size > 1) {
        // Try different configuration options based on input data
        // We'll try a subset of safe configuration options
        
        // First ensure we're in a clean state
        sqlite3_shutdown();
        
        // Try single-threaded mode (SQLITE_CONFIG_SINGLETHREAD = 1)
        if ((Data[1] & 0x01) != 0) {
            sqlite3_config(1);  // SQLITE_CONFIG_SINGLETHREAD
        }
        
        // Try multi-threaded mode (SQLITE_CONFIG_MULTITHREAD = 2)
        if (Size > 2 && (Data[2] & 0x01) != 0) {
            sqlite3_config(2);  // SQLITE_CONFIG_MULTITHREAD
        }
        
        // Try serialized mode (SQLITE_CONFIG_SERIALIZED = 3)
        if (Size > 3 && (Data[3] & 0x01) != 0) {
            sqlite3_config(3);  // SQLITE_CONFIG_SERIALIZED
        }
        
        // Re-initialize after configuration
        sqlite3_initialize();
    }
    
    // Operation 4: Mixed operations
    if ((operation & 0x40) != 0) {
        // Mix various API calls in different orders
        
        // Start with shutdown to ensure clean state
        sqlite3_shutdown();
        
        // Check thread safety
        (void)sqlite3_threadsafe();
        
        // Initialize
        sqlite3_initialize();
        
        // Configure shared cache based on input
        if (Size > 1) {
            sqlite3_enable_shared_cache(Data[1] & 0x03);
        }
        
        // Call thread cleanup (no-op)
        sqlite3_thread_cleanup();
        
        // Shutdown again
        sqlite3_shutdown();
        
        // Final thread safety check
        (void)sqlite3_threadsafe();
    }
    
    // Operation 5: Stress test with repeated calls
    if ((operation & 0x80) != 0) {
        // Make many repeated calls to stress the library
        
        for (int i = 0; i < 10 && i < (int)Size; i++) {
            sqlite3_shutdown();
            sqlite3_initialize();
            sqlite3_thread_cleanup();
            (void)sqlite3_threadsafe();
            
            if (i % 2 == 0 && Size > 1) {
                sqlite3_enable_shared_cache((Data[1] + i) & 0x01);
            }
        }
        
        // Final cleanup
        sqlite3_shutdown();
    }
    
    // Always end with a shutdown to clean up resources
    sqlite3_shutdown();
    
    return 0;
}