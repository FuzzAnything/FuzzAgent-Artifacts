// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc;
    sqlite3_filename filename = NULL;
    const char *wal_name = NULL;
    const char *journal_name = NULL;
    const char *db_name = NULL;
    const char *uri_param = NULL;

    // Create a dummy file to use as a database
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "w");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }

    // Open the database
    rc = sqlite3_open(dummy_file, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Get the database filename
    filename = sqlite3_db_filename(db, "main");
    if (filename != NULL) {
        // Test sqlite3_filename_wal
        wal_name = sqlite3_filename_wal(filename);
        if (wal_name != NULL) {
            // No need to free wal_name as it is derived from filename
        }

        // Test sqlite3_filename_journal
        journal_name = sqlite3_filename_journal(filename);
        if (journal_name != NULL) {
            // No need to free journal_name as it is derived from filename
        }

        // Test sqlite3_filename_database
        db_name = sqlite3_filename_database(filename);
        if (db_name != NULL) {
            // No need to free db_name as it is derived from filename
        }

        // Test sqlite3_uri_parameter with various parameters derived from input
        if (Size >= 2) {
            char param[256];
            size_t param_len = (Size < 255) ? Size : 255;
            memcpy(param, Data, param_len);
            param[param_len] = '\0';

            // Ensure param is a valid C string (no internal nulls)
            for (size_t i = 0; i < param_len; i++) {
                if (param[i] == '\0') {
                    param[i] = 'x';
                }
            }

            uri_param = sqlite3_uri_parameter(filename, param);
            if (uri_param != NULL) {
                // uri_param points to internal memory, do not free
            }
        }
    }

    // Test with a constructed filename that might contain URI parameters
    // We construct a simple URI-like string from the input
    if (Size > 5) {
        char uri_path[512];
        size_t uri_len = (Size < 510) ? Size : 510;
        memcpy(uri_path, Data, uri_len);
        uri_path[uri_len] = '\0';

        // Ensure it looks like a URI for testing purposes
        if (uri_path[0] != 'f' || uri_path[1] != 'i' || uri_path[2] != 'l' || uri_path[3] != 'e') {
            // Prepend "file:" to make it a valid URI for testing
            if (uri_len + 6 < 512) {
                memmove(uri_path + 5, uri_path, uri_len + 1);
                memcpy(uri_path, "file:", 5);
            }
        }

        // Use sqlite3_create_filename to create a valid filename object
        // Signature: sqlite3_create_filename(const char *zDatabase, const char *zJournal, const char *zWal, int nParam, const char **azParam)
        // We pass NULL for optional components and 0 for params to test basic construction
        sqlite3_filename created_filename = sqlite3_create_filename(uri_path, NULL, NULL, 0, NULL);
        if (created_filename != NULL) {
            // Test sqlite3_filename_wal
            wal_name = sqlite3_filename_wal(created_filename);
            // Test sqlite3_filename_journal
            journal_name = sqlite3_filename_journal(created_filename);
            // Test sqlite3_filename_database
            db_name = sqlite3_filename_database(created_filename);

            // Test sqlite3_uri_parameter
            if (Size >= 2) {
                char param[256];
                size_t param_len = (Size < 255) ? Size : 255;
                memcpy(param, Data, param_len);
                param[param_len] = '\0';
                for (size_t i = 0; i < param_len; i++) {
                    if (param[i] == '\0') {
                        param[i] = 'x';
                    }
                }
                uri_param = sqlite3_uri_parameter(created_filename, param);
            }

            // Free the created filename
            sqlite3_free_filename(created_filename);
        }
    }

    // Close the database
    sqlite3_close(db);

    // Remove the dummy file
    remove(dummy_file);

    return 0;
}