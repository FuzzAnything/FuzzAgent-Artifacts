// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static void safe_string_from_fuzz_data(const uint8_t *data, size_t size, char *out, size_t max_len) {
    if (size == 0 || max_len == 0) {
        out[0] = '\0';
        return;
    }
    size_t copy_len = (size < max_len - 1) ? size : max_len - 1;
    memcpy(out, data, copy_len);
    out[copy_len] = '\0';
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    char str1[256];
    char str2[256];
    char str3[256];
    char str4[256];
    char str5[256];
    char str6[256];
    char str7[256];
    char str8[256];
    char str9[256];
    char str10[256];
    char str11[256];
    char str12[256];
    char str13[256];
    char str14[256];
    char str15[256];
    char str16[256];

    // Ensure we have enough data for 16 strings, at least 1 byte each
    if (Size < 16) {
        return 0;
    }

    size_t chunk_size = Size / 16;
    if (chunk_size == 0) chunk_size = 1;

    // Calculate remaining bytes for the last chunk
    size_t current_offset = 0;
    
    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str1, sizeof(str1));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str2, sizeof(str2));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str3, sizeof(str3));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str4, sizeof(str4));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str5, sizeof(str5));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str6, sizeof(str6));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str7, sizeof(str7));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str8, sizeof(str8));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str9, sizeof(str9));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str10, sizeof(str10));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str11, sizeof(str11));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str12, sizeof(str12));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str13, sizeof(str13));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str14, sizeof(str14));
    current_offset += chunk_size;

    safe_string_from_fuzz_data(Data + current_offset, chunk_size, str15, sizeof(str15));
    current_offset += chunk_size;

    // Last chunk takes remaining data
    size_t remaining = Size - current_offset;
    safe_string_from_fuzz_data(Data + current_offset, remaining, str16, sizeof(str16));

    // Invoke sqlite3_stricmp
    int r1 = sqlite3_stricmp(str1, str2);
    (void)r1;

    // Invoke sqlite3_stricmp
    int r2 = sqlite3_stricmp(str3, str4);
    (void)r2;

    // Invoke sqlite3_stricmp
    int r3 = sqlite3_stricmp(str5, str6);
    (void)r3;

    // Invoke sqlite3_strnicmp
    int n = (int)(strnlen(str7, 256));
    int r4 = sqlite3_strnicmp(str7, str8, n);
    (void)r4;

    // Invoke sqlite3_stricmp
    int r5 = sqlite3_stricmp(str9, str10);
    (void)r5;

    // Invoke sqlite3_stricmp
    int r6 = sqlite3_stricmp(str11, str12);
    (void)r6;

    // Invoke sqlite3_stricmp
    int r7 = sqlite3_stricmp(str13, str14);
    (void)r7;

    // Invoke sqlite3_strglob
    int r8 = sqlite3_strglob(str15, str16);
    (void)r8;

    // Invoke sqlite3_strglob
    int r9 = sqlite3_strglob(str14, str13);
    (void)r9;

    // Invoke sqlite3_stricmp
    int r10 = sqlite3_stricmp(str12, str11);
    (void)r10;

    return 0;
}