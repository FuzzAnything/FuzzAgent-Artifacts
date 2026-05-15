// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:127555:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_file(void) {
    remove("./dummy_file");
}

static const char* get_db_name(int idx) {
    switch (idx % 3) {
        case 0: return "main";
        case 1: return "temp";
        case 2: return NULL;
        default: return NULL;
    }
}

static int get_opcode(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) <= size) {
        int op;
        memcpy(&op, data + *offset, sizeof(int));
        *offset += sizeof(int);
        return op;
    }
    return 0;
}

static const char* get_string(const uint8_t *data, size_t size, size_t *offset, char *buf, size_t buf_size) {
    if (*offset >= size) return NULL;
    
    size_t remaining = size - *offset;
    size_t copy_len = remaining < buf_size - 1 ? remaining : buf_size - 1;
    
    if (copy_len > 0) {
        memcpy(buf, data + *offset, copy_len);
        buf[copy_len] = '\0';
        *offset += copy_len;
        return buf;
    }
    return NULL;
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    int rc;
    size_t offset = 0;
    char str_buf[256];
    
    // Test sqlite3_open with dummy file
    dummy_file_write(Data, Size);
    rc = sqlite3_open("./dummy_file", &db);
    
    if (rc == SQLITE_OK && db != NULL) {
        // Test sqlite3_db_name
        int n = (int)(Data[offset % Size]) % 10;
        offset++;
        const char *db_name = sqlite3_db_name(db, n);
        (void)db_name;
        
        // Test sqlite3_db_filename
        const char *db_name_for_filename = get_db_name(Data[offset % Size]);
        offset++;
        sqlite3_filename filename = sqlite3_db_filename(db, db_name_for_filename);
        (void)filename;
        
        // Test sqlite3_file_control
        int op = get_opcode(Data, Size, &offset);
        void *arg = NULL;
        rc = sqlite3_file_control(db, get_db_name(Data[offset % Size]), op, &arg);
        offset++;
        
        // Enable extension loading for sqlite3_load_extension test
        sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, 1, NULL);
        
        // Test sqlite3_load_extension with dummy file
        char *err_msg = NULL;
        rc = sqlite3_load_extension(db, "./dummy_file", NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
        }
        
        // Test sqlite3_open_v2 with various flags
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        if (Size > offset + 4) {
            int flag_bits;
            memcpy(&flag_bits, Data + offset, sizeof(int));
            offset += sizeof(int);
            flags = flag_bits;
        }
        
        const char *vfs_name = get_string(Data, Size, &offset, str_buf, sizeof(str_buf));
        rc = sqlite3_open_v2("./dummy_file", &db2, flags, vfs_name);
        
        if (db2) {
            // Test functions on second database
            const char *name2 = sqlite3_db_name(db2, 0);
            (void)name2;
            
            sqlite3_close(db2);
        }
        
        sqlite3_close(db);
    } else {
        if (db) sqlite3_close(db);
    }
    
    // Test sqlite3_open with in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db) {
        // Test with different database names
        for (int i = 0; i < 3; i++) {
            const char *test_name = sqlite3_db_name(db, i);
            (void)test_name;
        }
        
        // Test file control with various operations
        int test_ops[] = {
            SQLITE_FCNTL_LOCKSTATE,
            SQLITE_FCNTL_GET_LOCKPROXYFILE,
            SQLITE_FCNTL_SET_LOCKPROXYFILE,
            SQLITE_FCNTL_LAST_ERRNO,
            SQLITE_FCNTL_SIZE_HINT,
            SQLITE_FCNTL_CHUNK_SIZE,
            SQLITE_FCNTL_FILE_POINTER,
            SQLITE_FCNTL_SYNC_OMITTED,
            SQLITE_FCNTL_WIN32_AV_RETRY,
            SQLITE_FCNTL_PERSIST_WAL,
            SQLITE_FCNTL_OVERWRITE,
            SQLITE_FCNTL_VFSNAME,
            SQLITE_FCNTL_POWERSAFE_OVERWRITE,
            SQLITE_FCNTL_PRAGMA,
            SQLITE_FCNTL_BUSYHANDLER,
            SQLITE_FCNTL_TEMPFILENAME,
            SQLITE_FCNTL_MMAP_SIZE,
            SQLITE_FCNTL_HAS_MOVED,
            SQLITE_FCNTL_SYNC,
            SQLITE_FCNTL_COMMIT_PHASETWO,
            SQLITE_FCNTL_WIN32_SET_HANDLE,
            SQLITE_FCNTL_WAL_BLOCK,
            SQLITE_FCNTL_ZIPVFS,
            SQLITE_FCNTL_RBU,
            SQLITE_FCNTL_VFS_POINTER,
            SQLITE_FCNTL_JOURNAL_POINTER,
            SQLITE_FCNTL_WIN32_GET_HANDLE,
            SQLITE_FCNTL_PDB,
            SQLITE_FCNTL_BEGIN_ATOMIC_WRITE,
            SQLITE_FCNTL_COMMIT_ATOMIC_WRITE,
            SQLITE_FCNTL_ROLLBACK_ATOMIC_WRITE,
            SQLITE_FCNTL_LOCK_TIMEOUT,
            SQLITE_FCNTL_DATA_VERSION,
            SQLITE_FCNTL_SIZE_LIMIT,
            SQLITE_FCNTL_CKPT_DONE
        };
        
        size_t num_test_ops = sizeof(test_ops) / sizeof(test_ops[0]);
        for (size_t i = 0; i < num_test_ops && i < Size; i++) {
            void *arg = NULL;
            sqlite3_file_control(db, get_db_name(Data[i % Size]), test_ops[i], &arg);
        }
        
        sqlite3_close(db);
    }
    
    cleanup_file();
    return 0;
}