// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void extract_strings(const uint8_t *data, size_t size, char *dest, size_t dest_size) {
    if (dest_size == 0) return;
    size_t copy_len = (size < dest_size - 1) ? size : dest_size - 1;
    if (copy_len > 0) {
        memcpy(dest, data, copy_len);
    }
    dest[copy_len] = '\0';
}

static void extract_hex_string(const uint8_t *data, size_t size, char *dest, size_t dest_size) {
    if (dest_size == 0) return;
    size_t copy_len = (size < dest_size - 1) ? size : dest_size - 1;
    if (copy_len > 0) {
        memcpy(dest, data, copy_len);
    }
    dest[copy_len] = '\0';
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;

    size_t offset = 0;
    char prop_name[256];
    char prop_value[256];
    char patch[256];
    char sample[256];
    char comment[256];
    char key[256];
    char buffer[256];

    size_t chunk_size = Size / 7;
    if (chunk_size == 0) chunk_size = 1;

    offset = 0;
    extract_strings(Data + offset, chunk_size, prop_name, sizeof(prop_name));
    offset += chunk_size;
    extract_strings(Data + offset, chunk_size, prop_value, sizeof(prop_value));
    offset += chunk_size;
    cmsIT8SetPropertyStr(hIT8, prop_name, prop_value);

    offset = 0;
    extract_strings(Data + offset, chunk_size, prop_name, sizeof(prop_name));
    offset += chunk_size;
    cmsUInt32Number hex_val = 0;
    if (Size - offset >= sizeof(hex_val)) {
        memcpy(&hex_val, Data + offset, sizeof(hex_val));
        offset += sizeof(hex_val);
    } else {
        hex_val = 0xDEADBEEF;
    }
    cmsIT8SetPropertyHex(hIT8, prop_name, hex_val);

    offset = 0;
    extract_strings(Data + offset, chunk_size, patch, sizeof(patch));
    offset += chunk_size;
    extract_strings(Data + offset, chunk_size, sample, sizeof(sample));
    offset += chunk_size;
    extract_strings(Data + offset, chunk_size, prop_value, sizeof(prop_value));
    offset += chunk_size;
    cmsIT8SetData(hIT8, patch, sample, prop_value);

    offset = 0;
    extract_strings(Data + offset, chunk_size, prop_name, sizeof(prop_name));
    offset += chunk_size;
    extract_strings(Data + offset, chunk_size, prop_value, sizeof(prop_value));
    offset += chunk_size;
    cmsIT8SetPropertyStr(hIT8, prop_name, prop_value);

    offset = 0;
    extract_strings(Data + offset, chunk_size, key, sizeof(key));
    offset += chunk_size;
    extract_strings(Data + offset, chunk_size, buffer, sizeof(buffer));
    offset += chunk_size;
    cmsIT8SetPropertyUncooked(hIT8, key, buffer);

    offset = 0;
    extract_strings(Data + offset, chunk_size, comment, sizeof(comment));
    offset += chunk_size;
    cmsIT8SetComment(hIT8, comment);

    cmsIT8SaveToFile(hIT8, "./dummy_file");

    cmsIT8Free(hIT8);
    return 0;
}