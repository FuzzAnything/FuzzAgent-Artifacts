// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:185350:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Helper function to safely extract a null-terminated string from fuzz data */
static const char *get_string_from_data(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return "SELECT 1;";
    }
    
    /* Check if the input data is already null-terminated */
    if (Data[Size - 1] == 0) {
        return (const char *)Data;
    }

    /* If not null-terminated, allocate a buffer, copy data, and append null */
    char *safe_str = (char *)sqlite3_malloc(Size + 1);
    if (!safe_str) {
        return "SELECT 1;";
    }
    memcpy(safe_str, Data, Size);
    safe_str[Size] = '\0';
    
    /* Store the pointer in a static variable so it can be freed later in the main function 
       if we need to track it, but since we can't easily pass it back, 
       we will handle the cleanup in the main function by using a different strategy:
       We will not return the allocated pointer directly to avoid leak if we can't free it.
       Instead, we will use a static buffer for small sizes or just return the data if it's safe.
       However, the requirement is to use sqlite3_malloc. 
       To fix the leak, we must free this memory. 
       Since get_string_from_data is called once per input, and we can't easily free from here 
       without a global or passing a pointer to free, let's change the strategy:
       We will perform the allocation and usage inside LLVMFuzzerTestOneInput directly 
       or use a static buffer for the string to avoid dynamic allocation in the helper 
       if it causes leaks that are hard to track.
       
       Revised strategy: Use a static buffer for the string construction to avoid leaks 
       in the helper function, as the fuzzer runs many times and static buffers are reused.
       But static buffers have size limits. Let's use a large enough static buffer.
    */
    static char static_buf[8192];
    size_t copy_len = (Size < sizeof(static_buf) - 1) ? Size : sizeof(static_buf) - 1;
    memcpy(static_buf, Data, copy_len);
    static_buf[copy_len] = '\0';
    
    /* Free the allocated memory if we allocated it above (we did, but we are returning static_buf now) */
    sqlite3_free(safe_str);
    
    return static_buf;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    /* 1. Prepare the environment */
    sqlite3 *db = NULL;
    
    /* Open a database in memory to avoid file system issues and ensure cleanup */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    /* 2. Invoke the target functions in the specified order */

    /* Step 1: sqlite3_complete */
    const char *sql = get_string_from_data(Data, Size);
    int complete = sqlite3_complete(sql);
    (void)complete; /* Suppress unused variable warning */

    /* Step 2: sqlite3_free */
    /* We need a pointer to free. Let's allocate one using sqlite3_malloc 
       to test the free path. */
    void *ptr = NULL;
    if (Size > 0) {
        ptr = sqlite3_malloc(Size > 1024 ? 1024 : Size);
    } else {
        ptr = sqlite3_malloc(1);
    }
    if (ptr) {
        sqlite3_free(ptr);
    }

    /* Step 3: sqlite3_close */
    /* Close the database connection. This should be safe. */
    sqlite3_close(db);
    db = NULL; /* Ensure we don't use it again */

    /* Step 4: sqlite3_memory_highwater */
    sqlite3_int64 highwater = sqlite3_memory_highwater(0);
    (void)highwater;

    /* Step 5: sqlite3_memory_used */
    sqlite3_int64 used = sqlite3_memory_used();
    (void)used;

    /* Step 6: sqlite3_status */
    int current = 0;
    int highwater_status = 0;
    int rc_status = sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current, &highwater_status, 0);
    (void)rc_status;

    /* Step 7: sqlite3_memory_used (again) */
    used = sqlite3_memory_used();
    (void)used;

    /* Step 8: sqlite3_hard_heap_limit64 */
    /* Set a limit, then query it. Use data to vary the limit if possible. */
    sqlite3_int64 limit_val = 1024 * 1024; /* Default 1MB */
    if (Size > 0) {
        /* Use first 8 bytes as limit if available, otherwise default */
        if (Size >= 8) {
            limit_val = *(sqlite3_int64 *)Data;
            if (limit_val < 0) limit_val = 1024 * 1024; /* Keep positive for setting */
        }
    }
    sqlite3_int64 old_hard_limit = sqlite3_hard_heap_limit64(limit_val);
    (void)old_hard_limit;

    /* Step 9: sqlite3_soft_heap_limit64 */
    sqlite3_int64 soft_limit_val = limit_val;
    if (Size > 8) {
        soft_limit_val = *(sqlite3_int64 *)(Data + 8);
        if (soft_limit_val < 0) soft_limit_val = 1024 * 1024;
    }
    sqlite3_int64 old_soft_limit = sqlite3_soft_heap_limit64(soft_limit_val);
    (void)old_soft_limit;

    /* 3. Cleanup */
    /* sqlite3_close was already called. 
       We don't need to free anything else as sqlite3_free was called on our malloc'd ptr.
       The database is closed. */
    
    return 0;
}