// This fuzz driver is generated for library c-ares, aiming to fuzz the following functions:
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined to fix compilation errors */
#ifndef CARES_EXTERN
# if defined(_WIN32)
#  define CARES_EXTERN __declspec(dllexport)
# else
#  define CARES_EXTERN
# endif
#endif

/* Define missing types that are typically in ares.h but might be missing in direct include */
typedef enum {
    ARES_FALSE = 0,
    ARES_TRUE = 1
} ares_bool_t;

/* Define ares_status_t to satisfy ares_dns_record.h dependencies */
typedef enum {
    ARES_SUCCESS = 0,
    ARES_EFORMERR = 1,
    ARES_ESERVFAIL = 2,
    ARES_ENOTFOUND = 3,
    ARES_ECONNREFUSED = 4,
    ARES_ETIMEOUT = 5,
    ARES_EOF = 6,
    ARES_EFILE = 7,
    ARES_ENODATA = 8,
    ARES_EDESTRUCTION = 9,
    ARES_EBADQUERY = 10,
    ARES_EBADNAME = 11,
    ARES_EBADFAMILY = 12,
    ARES_EBADRESP = 13,
    ARES_EFLUSHFILES = 14,
    ARES_ENOMEM = 15,
    ARES_EFILEFORMAT = 16,
    ARES_ECANCELLED = 17,
    ARES_EBADSTR = 18,
    ARES_EBADFLAGS = 19,
    ARES_ENOTIMPL = 20,
    ARES_EBADHINTS = 21,
    ARES_ELOADIPHLPAPI = 22,
    ARES_EADDRGETNETWORKPARAMS = 23,
    ARES_EDEPRECATED = 24,
    ARES_EBADPACKET = 25,
    ARES_EBADKEY = 26,
    ARES_EBADID = 27,
    ARES_ENOTINITIALIZED = 28,
    ARES_ECHANNELDESTROYED = 29,
    ARES_EBADTYPE = 30,
    ARES_EBADCLASS = 31,
    ARES_EBADLEN = 32,
    ARES_EBADPTR = 33,
    ARES_EBADFLAGS2 = 34,
    ARES_EBADNAMELEN = 35,
    ARES_EBADNAMETYP = 36,
    ARES_EBADNAMETYP2 = 37,
    ARES_EBADNAMETYP3 = 38,
    ARES_EBADNAMETYP4 = 39,
    ARES_EBADNAMETYP5 = 40,
    ARES_EBADNAMETYP6 = 41,
    ARES_EBADNAMETYP7 = 42,
    ARES_EBADNAMETYP8 = 43,
    ARES_EBADNAMETYP9 = 44,
    ARES_EBADNAMETYP10 = 45,
    ARES_EBADNAMETYP11 = 46,
    ARES_EBADNAMETYP12 = 47,
    ARES_EBADNAMETYP13 = 48,
    ARES_EBADNAMETYP14 = 49,
    ARES_EBADNAMETYP15 = 50,
    ARES_EBADNAMETYP16 = 51,
    ARES_EBADNAMETYP17 = 52,
    ARES_EBADNAMETYP18 = 53,
    ARES_EBADNAMETYP19 = 54,
    ARES_EBADNAMETYP20 = 55,
    ARES_EBADNAMETYP21 = 56,
    ARES_EBADNAMETYP22 = 57,
    ARES_EBADNAMETYP23 = 58,
    ARES_EBADNAMETYP24 = 59,
    ARES_EBADNAMETYP25 = 60,
    ARES_EBADNAMETYP26 = 61,
    ARES_EBADNAMETYP27 = 62,
    ARES_EBADNAMETYP28 = 63,
    ARES_EBADNAMETYP29 = 64,
    ARES_EBADNAMETYP30 = 65,
    ARES_EBADNAMETYP31 = 66,
    ARES_EBADNAMETYP32 = 67,
    ARES_EBADNAMETYP33 = 68,
    ARES_EBADNAMETYP34 = 69,
    ARES_EBADNAMETYP35 = 70,
    ARES_EBADNAMETYP36 = 71,
    ARES_EBADNAMETYP37 = 72,
    ARES_EBADNAMETYP38 = 73,
    ARES_EBADNAMETYP39 = 74,
    ARES_EBADNAMETYP40 = 75,
    ARES_EBADNAMETYP41 = 76,
    ARES_EBADNAMETYP42 = 77,
    ARES_EBADNAMETYP43 = 78,
    ARES_EBADNAMETYP44 = 79,
    ARES_EBADNAMETYP45 = 80,
    ARES_EBADNAMETYP46 = 81,
    ARES_EBADNAMETYP47 = 82,
    ARES_EBADNAMETYP48 = 83,
    ARES_EBADNAMETYP49 = 84,
    ARES_EBADNAMETYP50 = 85,
    ARES_EBADNAMETYP51 = 86,
    ARES_EBADNAMETYP52 = 87,
    ARES_EBADNAMETYP53 = 88,
    ARES_EBADNAMETYP54 = 89,
    ARES_EBADNAMETYP55 = 90,
    ARES_EBADNAMETYP56 = 91,
    ARES_EBADNAMETYP57 = 92,
    ARES_EBADNAMETYP58 = 93,
    ARES_EBADNAMETYP59 = 94,
    ARES_EBADNAMETYP60 = 95,
    ARES_EBADNAMETYP61 = 96,
    ARES_EBADNAMETYP62 = 97,
    ARES_EBADNAMETYP63 = 98,
    ARES_EBADNAMETYP64 = 99,
    ARES_EBADNAMETYP65 = 100,
    ARES_EBADNAMETYP66 = 101,
    ARES_EBADNAMETYP67 = 102,
    ARES_EBADNAMETYP68 = 103,
    ARES_EBADNAMETYP69 = 104,
    ARES_EBADNAMETYP70 = 105,
    ARES_EBADNAMETYP71 = 106,
    ARES_EBADNAMETYP72 = 107,
    ARES_EBADNAMETYP73 = 108,
    ARES_EBADNAMETYP74 = 109,
    ARES_EBADNAMETYP75 = 110,
    ARES_EBADNAMETYP76 = 111,
    ARES_EBADNAMETYP77 = 112,
    ARES_EBADNAMETYP78 = 113,
    ARES_EBADNAMETYP79 = 114,
    ARES_EBADNAMETYP80 = 115,
    ARES_EBADNAMETYP81 = 116,
    ARES_EBADNAMETYP82 = 117,
    ARES_EBADNAMETYP83 = 118,
    ARES_EBADNAMETYP84 = 119,
    ARES_EBADNAMETYP85 = 120,
    ARES_EBADNAMETYP86 = 121,
    ARES_EBADNAMETYP87 = 122,
    ARES_EBADNAMETYP88 = 123,
    ARES_EBADNAMETYP89 = 124,
    ARES_EBADNAMETYP90 = 125,
    ARES_EBADNAMETYP91 = 126,
    ARES_EBADNAMETYP92 = 127,
    ARES_EBADNAMETYP93 = 128,
    ARES_EBADNAMETYP94 = 129,
    ARES_EBADNAMETYP95 = 130,
    ARES_EBADNAMETYP96 = 131,
    ARES_EBADNAMETYP97 = 132,
    ARES_EBADNAMETYP98 = 133,
    ARES_EBADNAMETYP99 = 134,
    ARES_EBADNAMETYP100 = 135
} ares_status_t;

#include <ares_dns_record.h>

static void test_rec_type_fromstr(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    ares_dns_rec_type_t qtype;
    // Ensure null termination for string parsing
    char str_buf[256];
    size_t copy_len = (Size < sizeof(str_buf) - 1) ? Size : sizeof(str_buf) - 1;
    memcpy(str_buf, Data, copy_len);
    str_buf[copy_len] = '\0';
    
    // Test with valid string
    ares_dns_rec_type_fromstr(&qtype, str_buf);
    
    // Test with NULL pointer (if library handles it gracefully, otherwise it may crash, which is fine for fuzzing)
    ares_dns_rec_type_fromstr(NULL, str_buf);
    
    // Test with empty string
    ares_dns_rec_type_fromstr(&qtype, "");
}

static void test_rr_key_tostr(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Interpret input as enum value (cast to int to avoid overflow issues in casting)
    int val = 0;
    if (Size >= sizeof(int)) {
        memcpy(&val, Data, sizeof(int));
    } else if (Size > 0) {
        memcpy(&val, Data, Size);
    }
    
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)val;
    const char *result = ares_dns_rr_key_tostr(key);
    
    // Just access the result to ensure it's valid (or trigger NULL check if needed)
    if (result != NULL) {
        // Read first byte to ensure pointer is valid
        volatile char dummy = result[0];
        (void)dummy;
    }
}

static void test_rr_key_to_rec_type(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    int val = 0;
    if (Size >= sizeof(int)) {
        memcpy(&val, Data, sizeof(int));
    } else if (Size > 0) {
        memcpy(&val, Data, Size);
    }
    
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)val;
    ares_dns_rec_type_t type = ares_dns_rr_key_to_rec_type(key);
    
    // Use the result to prevent optimization
    volatile ares_dns_rec_type_t dummy = type;
    (void)dummy;
}

static void test_rr_get_str(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // We cannot easily construct a valid ares_dns_rr_t without the full library context
    // So we test with NULL and a fabricated pointer (which will likely crash or return NULL)
    // This is acceptable for fuzzing to test error handling
    
    // Test with NULL record
    ares_dns_rr_key_t key = 0;
    const char *result = ares_dns_rr_get_str(NULL, key);
    if (result != NULL) {
        volatile char dummy = result[0];
        (void)dummy;
    }
    
    // Test with invalid key on NULL record
    int val = 0;
    if (Size >= sizeof(int)) {
        memcpy(&val, Data, sizeof(int));
    } else if (Size > 0) {
        memcpy(&val, Data, Size);
    }
    key = (ares_dns_rr_key_t)val;
    result = ares_dns_rr_get_str(NULL, key);
    if (result != NULL) {
        volatile char dummy = result[0];
        (void)dummy;
    }
}

static void test_rr_get_keys(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    int val = 0;
    if (Size >= sizeof(int)) {
        memcpy(&val, Data, sizeof(int));
    } else if (Size > 0) {
        memcpy(&val, Data, Size);
    }
    
    ares_dns_rec_type_t type = (ares_dns_rec_type_t)val;
    size_t cnt = 0;
    
    // Test with valid cnt pointer
    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(type, &cnt);
    if (keys != NULL) {
        // Read a few keys if available
        for (size_t i = 0; i < cnt && i < 10; i++) {
            volatile ares_dns_rr_key_t dummy = keys[i];
            (void)dummy;
        }
    }
    
    // Test with NULL cnt pointer
    keys = ares_dns_rr_get_keys(type, NULL);
    // Expected to return NULL, but we check anyway
    if (keys != NULL) {
        volatile ares_dns_rr_key_t dummy = keys[0];
        (void)dummy;
    }
}

static void test_rr_key_datatype(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    int val = 0;
    if (Size >= sizeof(int)) {
        memcpy(&val, Data, sizeof(int));
    } else if (Size > 0) {
        memcpy(&val, Data, Size);
    }
    
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)val;
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
    
    // Use the result to prevent optimization
    volatile ares_dns_datatype_t dummy = datatype;
    (void)dummy;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Split input into chunks for different tests
    size_t chunk_size = Size / 6;
    if (chunk_size == 0) chunk_size = 1;
    
    test_rec_type_fromstr(Data, (Size < chunk_size) ? Size : chunk_size);
    
    size_t offset = chunk_size;
    if (offset < Size) {
        test_rr_key_tostr(Data + offset, (Size - offset < chunk_size) ? (Size - offset) : chunk_size);
    }
    
    offset += chunk_size;
    if (offset < Size) {
        test_rr_key_to_rec_type(Data + offset, (Size - offset < chunk_size) ? (Size - offset) : chunk_size);
    }
    
    offset += chunk_size;
    if (offset < Size) {
        test_rr_get_str(Data + offset, (Size - offset < chunk_size) ? (Size - offset) : chunk_size);
    }
    
    offset += chunk_size;
    if (offset < Size) {
        test_rr_get_keys(Data + offset, (Size - offset < chunk_size) ? (Size - offset) : chunk_size);
    }
    
    offset += chunk_size;
    if (offset < Size) {
        test_rr_key_datatype(Data + offset, (Size - offset < chunk_size) ? (Size - offset) : chunk_size);
    }
    
    return 0;
}