// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:63856:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Allocate buffer with extra padding at the beginning to prevent negative offset reads
    // We add 4 bytes of padding to safely handle zName[-1]...zName[-4] checks in sqlite3
    size_t padding = 4;
    char *input_buf = (char *)malloc(Size + 1 + padding);
    if (!input_buf) {
        return 0;
    }

    // Fill padding with null bytes to ensure safe reads before the actual data
    memset(input_buf, 0, padding);
    
    // Copy data after the padding
    char *data_start = input_buf + padding;
    memcpy(data_start, Data, Size);
    data_start[Size] = '\0';

    // Use the padded pointer (data_start) as the filename
    // This ensures that if sqlite3 checks negative offsets, it reads the safe padding
    sqlite3_filename db_name = (sqlite3_filename)data_start;

    // 1. Test sqlite3_filename_database
    const char *db_result = sqlite3_filename_database(db_name);
    (void)db_result;

    // 2. Test sqlite3_filename_journal
    const char *journal_result = sqlite3_filename_journal(db_name);
    (void)journal_result;

    // 3. Test sqlite3_filename_wal
    const char *wal_result = sqlite3_filename_wal(db_name);
    (void)wal_result;

    // 4. Test sqlite3_uri_parameter
    const char *param_name = "mode";
    if (Size > 5) {
        char param_buf[10] = {0};
        size_t copy_len = (Size - 5) < 9 ? (Size - 5) : 9;
        memcpy(param_buf, &data_start[5], copy_len);
        param_name = param_buf;
    }
    
    const char *uri_result = sqlite3_uri_parameter(db_name, param_name);
    (void)uri_result;

    // 5. Test sqlite3_database_file_object
    sqlite3_file *file_obj = sqlite3_database_file_object(data_start);
    (void)file_obj;

    // 6. Test sqlite3_free_filename with NULL
    sqlite3_free_filename(NULL);

    // Cleanup
    free(input_buf);

    return 0;
}