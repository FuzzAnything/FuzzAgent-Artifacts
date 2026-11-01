// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_blob64 at sqlite3.c:92722:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:92954:17 in sqlite3.h
// sqlite3_result_pointer at sqlite3.c:92788:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:92844:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static void dummy_destructor(void *p) {
    if (p) {
        free(p);
    }
}

static void fuzz_callback(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Access the fuzzing data stored in user_data
    // We will store the data pointer in the user_data of the function
    void *userData = sqlite3_user_data(ctx);
    if (!userData) {
        sqlite3_result_null(ctx);
        return;
    }

    // userData is actually a pointer to a struct containing data and size
    struct {
        uint8_t *data;
        size_t size;
    } *fuzzInfo = (struct { uint8_t *data; size_t size; } *)userData;

    uint8_t *data = fuzzInfo->data;
    size_t size = fuzzInfo->size;

    if (size < 1) {
        sqlite3_result_null(ctx);
        return;
    }

    uint8_t choice = data[0] % 6;

    if (choice == 0) {
        // sqlite3_result_blob64
        if (size > 0) {
            sqlite3_uint64 len = (size > 0) ? size : 0;
            // Pass a copy or the original data? 
            // If we pass 'data', the destructor must not free it if it's static/fuzz input.
            // The API says: "If length exceeds 2GB, it frees the buffer and returns an error"
            // "otherwise, it stores the blob."
            // If we provide a destructor, it might be called later.
            // To be safe, we pass NULL destructor if we don't want it freed, 
            // or a dummy destructor if we allocated a copy.
            // Here we pass the fuzz input directly but with NULL destructor to avoid double free issues
            // unless the API takes ownership. The doc says "optional destructor".
            // If we pass a destructor, SQLite will call it when done.
            // Since 'data' comes from fuzzer, we should not let SQLite free it.
            // So we pass NULL destructor.
            sqlite3_result_blob64(ctx, data, len, NULL);
        } else {
            sqlite3_result_null(ctx);
        }
    } else if (choice == 1) {
        // sqlite3_result_null
        sqlite3_result_null(ctx);
    } else if (choice == 2) {
        // sqlite3_result_error_toobig
        sqlite3_result_error_toobig(ctx);
    } else if (choice == 3) {
        // sqlite3_result_pointer
        // Allocate a dummy object to return as a pointer
        void *ptr = malloc(1);
        if (ptr) {
            // We provide a destructor to clean up this specific allocation
            sqlite3_result_pointer(ctx, ptr, "fuzz_type", dummy_destructor);
        } else {
            sqlite3_result_null(ctx);
        }
    } else if (choice == 4) {
        // sqlite3_result_text16be
        // Needs even length
        int len = size;
        if (len % 2 != 0) len--;
        if (len > 0) {
            // Pass NULL destructor as we don't own the buffer
            sqlite3_result_text16be(ctx, data, len, NULL);
        } else {
            sqlite3_result_null(ctx);
        }
    } else if (choice == 5) {
        // sqlite3_result_text64
        // Encoding: 0 for UTF8, 1 for UTF16BE, 2 for UTF16LE
        unsigned char encoding = (size > 1) ? (data[1] % 3) : 0;
        if (size > 0) {
            sqlite3_uint64 len = size;
            sqlite3_result_text64(ctx, (char*)data, len, NULL, encoding);
        } else {
            sqlite3_result_null(ctx);
        }
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    if (Size < 1) {
        return 0;
    }

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a dummy table
    const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare user data structure
    struct {
        uint8_t *data;
        size_t size;
    } fuzzInfo;
    
    // We need to ensure the data persists for the duration of the function call.
    // Since the callback is called synchronously during sqlite3_step, 
    // we can just point to the input buffer 'Data'.
    fuzzInfo.data = (uint8_t*)Data;
    fuzzInfo.size = Size;

    // Register the function with the user_data
    rc = sqlite3_create_function(db, "fuzz_func", -1, SQLITE_UTF8, &fuzzInfo, fuzz_callback, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Execute a query that calls the function
    const char *sql = "SELECT fuzz_func(1);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Just iterate to trigger the callback
        }
        sqlite3_finalize(stmt);
    }

    // Cleanup
    sqlite3_close(db);

    return 0;
}