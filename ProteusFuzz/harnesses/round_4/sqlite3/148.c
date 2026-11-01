// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:189478:18 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Static helper function to safely close a database connection.
 * Handles SQLITE_BUSY by retrying once if necessary, though typically
 * sqlite3_close_v2 handles pending statements gracefully.
 */
static void safe_close_db(sqlite3 *db) {
    if (db) {
        int rc = sqlite3_close_v2(db);
        /* If SQLITE_BUSY, it means there are still open statements.
         * In a fuzzing context, we generally just want to ensure no crash.
         * We do not retry indefinitely to avoid hangs. */
        (void)rc;
    }
}

/*
 * Static callback for autovacuum pages.
 * Returns a random amount of pages to vacuum based on input.
 * This function is called by SQLite, not the fuzzer directly.
 */
static unsigned int dummy_autovac_callback(
    void *pClientData,
    const char *zSchema,
    unsigned int nDbPage,
    unsigned int nFreePage,
    unsigned int nBytePerPage
) {
    /* Avoid complex logic to prevent re-entrancy issues or crashes.
     * Just return a safe value. */
    (void)pClientData;
    (void)zSchema;
    (void)nDbPage;
    (void)nBytePerPage;
    
    /* Return 0 to do nothing, or nFreePage to vacuum all.
     * We use a simple modulo to vary behavior slightly. */
    return nFreePage % 2 == 0 ? 0 : nFreePage;
}

/*
 * Static destructor for client data.
 */
static void dummy_client_data_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/*
 * Static destructor for autovacuum pages data.
 */
static void dummy_autovac_destructor(void *p) {
    if (p) {
        free(p);
    }
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int prev_val = 0;
    void *client_data_ptr = NULL;
    void *autovac_data_ptr = NULL;

    /* 1. Prepare the environment */
    if (Size < 1) {
        return 0;
    }

    /* Use the input data to determine flags and filenames if needed.
     * We use a dummy file for simplicity as per instructions. */
    const char *filename = "./dummy_file";
    
    /* Derive flags from input data to explore different states */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 1) {
        int flag_choice = Data[0] % 4;
        switch (flag_choice) {
            case 0: flags = SQLITE_OPEN_READONLY; break;
            case 1: flags = SQLITE_OPEN_READWRITE; break;
            case 2: flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE; break;
            case 3: flags = SQLITE_OPEN_URI; break;
        }
    }

    /* Open the database */
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        /* If open fails, we can't test the other functions safely.
         * Return early to avoid null pointer dereferences. */
        return 0;
    }

    /* 2. Fuzz sqlite3_db_config */
    /* We need to ensure we pass the 4th argument (pointer to int) if required.
     * The prompt specifies: "Do not omit this pointer argument."
     * We will test a few known operation codes. */
    if (Size > 2) {
        int op = Data[1] % 10; /* Limit to a small range of known ops */
        int val = Data[2];
        
        /* SQLITE_DBCONFIG_LOOKASIDE is 1, SQLITE_DBCONFIG_TRIGGER_EQP is 2, etc.
         * We'll try a few specific ones that are safe to call. */
        int safe_ops[] = {
            SQLITE_DBCONFIG_LOOKASIDE,
            SQLITE_DBCONFIG_TRIGGER_EQP,
            SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE
        };
        
        int op_idx = Data[1] % 3;
        int selected_op = safe_ops[op_idx];
        
        /* Call with the 4th argument as required */
        rc = sqlite3_db_config(db, selected_op, val, &prev_val);
        (void)rc;
        
        /* Try calling with NULL for the 4th argument if the op allows it,
         * but the prompt emphasizes the 4th arg for specific ops.
         * Let's stick to the safe pattern with the pointer. */
    }

    /* 3. Fuzz sqlite3_set_clientdata and sqlite3_get_clientdata */
    /* Generate a key from input data */
    char key[64];
    size_t key_len = Size > 64 ? 63 : Size;
    if (key_len > 0) {
        memcpy(key, Data, key_len);
        key[key_len] = '\0';
    } else {
        strcpy(key, "fuzz_key");
    }

    /* Allocate some dummy data to attach */
    size_t data_size = Size > 1024 ? 1024 : Size;
    if (data_size > 0) {
        client_data_ptr = malloc(data_size);
        if (client_data_ptr) {
            memcpy(client_data_ptr, Data, data_size);
            /* Set client data with a destructor */
            rc = sqlite3_set_clientdata(db, key, client_data_ptr, dummy_client_data_destructor);
            (void)rc;
        }
    }

    /* Try to retrieve it */
    void *retrieved = sqlite3_get_clientdata(db, key);
    (void)retrieved;

    /* Try setting NULL to remove it */
    rc = sqlite3_set_clientdata(db, key, NULL, NULL);
    (void)rc;

    /* 4. Fuzz sqlite3_autovacuum_pages */
    if (Size > 3) {
        int choice = Data[3] % 3;
        if (choice == 0) {
            /* Register callback with data */
            autovac_data_ptr = malloc(16);
            if (autovac_data_ptr) {
                memset(autovac_data_ptr, 0, 16);
                rc = sqlite3_autovacuum_pages(db, dummy_autovac_callback, autovac_data_ptr, dummy_autovac_destructor);
                (void)rc;
            }
        } else if (choice == 1) {
            /* Register callback without data */
            rc = sqlite3_autovacuum_pages(db, dummy_autovac_callback, NULL, NULL);
            (void)rc;
        } else {
            /* Cancel callback */
            rc = sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
            (void)rc;
        }
    }

    /* 5. Cleanup */
    /* Close the database. This should trigger destructors for client data and autovac data. */
    safe_close_db(db);

    return 0;
}