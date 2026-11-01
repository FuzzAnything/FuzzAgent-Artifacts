// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_complete at sqlite3.c:185350:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. sqlite3_complete
    // Ensure the input is null-terminated for sqlite3_complete
    char *sql_input = (char *)malloc(Size + 1);
    if (sql_input == NULL) {
        return 0;
    }
    memcpy(sql_input, Data, Size);
    sql_input[Size] = '\0';

    int complete_result = sqlite3_complete(sql_input);
    free(sql_input);
    (void)complete_result;

    // 2. sqlite3_free
    // Allocate some memory to test sqlite3_free
    void *test_mem = sqlite3_malloc(64);
    if (test_mem != NULL) {
        memset(test_mem, 0xAA, 64);
        sqlite3_free(test_mem);
    }
    sqlite3_free(NULL);

    // 3. sqlite3_close
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db != NULL) {
        sqlite3_exec(db, "CREATE TABLE test (id INT);", NULL, NULL, NULL);
        sqlite3_close(db);
    }
    sqlite3_close(NULL);

    // 4. sqlite3_memory_highwater
    sqlite3_int64 highwater = sqlite3_memory_highwater(0);
    sqlite3_int64 prev_highwater = sqlite3_memory_highwater(1);
    (void)highwater;
    (void)prev_highwater;

    // 5. sqlite3_memory_used
    sqlite3_int64 mem_used = sqlite3_memory_used();
    (void)mem_used;

    // 6. sqlite3_status
    int current_val = 0;
    int highwater_val = 0;
    int status_rc = sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current_val, &highwater_val, 0);
    (void)status_rc;
    (void)current_val;
    (void)highwater_val;

    // 7. sqlite3_memory_used (second call as per sequence)
    mem_used = sqlite3_memory_used();
    (void)mem_used;

    // 8. sqlite3_hard_heap_limit64
    sqlite3_int64 hard_limit = sqlite3_hard_heap_limit64(-1);
    sqlite3_hard_heap_limit64(1048576);
    hard_limit = sqlite3_hard_heap_limit64(-1);
    (void)hard_limit;

    // 9. sqlite3_soft_heap_limit64
    sqlite3_int64 soft_limit = sqlite3_soft_heap_limit64(-1);
    sqlite3_soft_heap_limit64(524288);
    soft_limit = sqlite3_soft_heap_limit64(-1);
    (void)soft_limit;

    return 0;
}