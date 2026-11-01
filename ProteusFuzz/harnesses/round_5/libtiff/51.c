// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldIsAnonymous at tif_dirinfo.c:879:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldIsAnonymous at tif_dirinfo.c:879:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

/* 
 * We rely on tiffio.h for TIFFDataType definition.
 * We must NOT redefine it or forward declare it if tiffio.h already does.
 * However, TIFFSetGetFieldType might be incomplete or defined differently.
 * We will define the struct manually but use the types from the library where possible.
 * If TIFFSetGetFieldType is not fully defined in tiffio.h, we might need to 
 * cast from int or assume it's an enum. 
 * To avoid redefinition errors, we only define what is strictly necessary and not present in headers.
 */

/* 
 * Re-defining the structure to match libtiff internal layout for fuzzing purposes.
 * We use the types provided by the library.
 */
struct _TIFFField_Fuzz {
    uint32_t field_tag;
    short field_readcount;
    short field_writecount;
    TIFFDataType field_type;
    uint32_t field_anonymous;
    /* TIFFSetGetFieldType is often an enum. If not fully defined, we treat it as int for fuzzing */
    int set_get_field_type; 
    unsigned short field_bit;
    unsigned char field_oktochange;
    unsigned char field_passcount;
    char *field_name;
    TIFFFieldArray *field_subfields;
};

typedef struct _TIFFField_Fuzz TIFFField_Fuzz;

static void
cleanup_fields(TIFFField_Fuzz *fields, uint32_t count)
{
    if (fields == NULL) {
        return;
    }
    for (uint32_t i = 0; i < count; i++) {
        if (fields[i].field_name != NULL) {
            free(fields[i].field_name);
            fields[i].field_name = NULL;
        }
        if (fields[i].field_subfields != NULL) {
            free(fields[i].field_subfields);
            fields[i].field_subfields = NULL;
        }
    }
    free(fields);
}

int
LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size)
{
    if (Size < 4) {
        return 0;
    }

    uint32_t count = 0;
    if (Size >= 4) {
        count = *(const uint32_t *)Data;
        count &= 0xFFFF; /* Limit to reasonable number to avoid OOM */
    }

    if (count == 0) {
        count = 1;
    }

    TIFFField_Fuzz *fields = (TIFFField_Fuzz *)malloc(count * sizeof(TIFFField_Fuzz));
    if (fields == NULL) {
        return 0;
    }

    memset(fields, 0, count * sizeof(TIFFField_Fuzz));

    /* Initialize fields with fuzzed data */
    size_t offset = 4;
    for (uint32_t i = 0; i < count; i++) {
        if (offset + 4 > Size) break;
        fields[i].field_tag = *(const uint32_t *)(Data + offset);
        offset += 4;
        if (offset + 2 > Size) break;
        fields[i].field_readcount = *(const short *)(Data + offset);
        offset += 2;
        if (offset + 2 > Size) break;
        fields[i].field_writecount = *(const short *)(Data + offset);
        offset += 2;
        if (offset + 4 > Size) break;
        fields[i].field_type = *(const TIFFDataType *)(Data + offset);
        offset += 4;
        if (offset + 4 > Size) break;
        fields[i].field_anonymous = *(const uint32_t *)(Data + offset);
        offset += 4;
        if (offset + 4 > Size) break;
        fields[i].set_get_field_type = *(const int *)(Data + offset);
        offset += 4;
        if (offset + 2 > Size) break;
        fields[i].field_bit = *(const unsigned short *)(Data + offset);
        offset += 2;
        if (offset + 1 > Size) break;
        fields[i].field_oktochange = *(const unsigned char *)(Data + offset);
        offset += 1;
        if (offset + 1 > Size) break;
        fields[i].field_passcount = *(const unsigned char *)(Data + offset);
        offset += 1;

        /* Allocate a dummy name */
        fields[i].field_name = (char *)malloc(10);
        if (fields[i].field_name) {
            memset(fields[i].field_name, 0, 10);
        }
    }

    /* 
     * Note: The target functions in libtiff (e.g., tif_dirinfo.c) do not check for NULL
     * before dereferencing the pointer in their implementation (e.g., return fip->field_passcount).
     * Passing NULL causes a Segmentation Fault. Therefore, we skip NULL pointer testing
     * to prevent the fuzzer from crashing immediately.
     */

    /* Test with valid fields */
    for (uint32_t i = 0; i < count; i++) {
        /* Cast our internal struct to the library's TIFFField type */
        TIFFField *current = (TIFFField *)&fields[i];

        int c1 = TIFFFieldSetGetCountSize(current);
        (void)c1;

        int c2 = TIFFFieldPassCount(current);
        (void)c2;

        int c3 = TIFFFieldWriteCount(current);
        (void)c3;

        int c4 = TIFFFieldIsAnonymous(current);
        (void)c4;

        int c5 = TIFFFieldReadCount(current);
        (void)c5;

        int c6 = TIFFFieldSetGetSize(current);
        (void)c6;

        /* Mutate field to explore more states */
        fields[i].field_passcount = (unsigned char)(i % 256);
        fields[i].field_writecount = (short)(i % 1000);
        fields[i].field_readcount = (short)(i % 1000);
        fields[i].field_anonymous = (i % 2) ? 1 : 0;
        fields[i].set_get_field_type = (int)(i % 10);

        /* Re-test after mutation */
        (void)TIFFFieldSetGetCountSize(current);
        (void)TIFFFieldPassCount(current);
        (void)TIFFFieldWriteCount(current);
        (void)TIFFFieldIsAnonymous(current);
        (void)TIFFFieldReadCount(current);
        (void)TIFFFieldSetGetSize(current);
    }

    cleanup_fields(fields, count);
    return 0;
}