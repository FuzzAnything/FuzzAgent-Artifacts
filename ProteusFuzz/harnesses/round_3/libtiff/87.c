// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldIsAnonymous at tif_dirinfo.c:879:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* 
 * The TIFFField struct is incomplete in tiffio.h. 
 * We define a compatible local struct to allow direct manipulation for fuzzing.
 */
typedef struct {
    uint32_t field_tag;
    short field_readcount;
    short field_writecount;
    TIFFDataType field_type;
    uint32_t field_anonymous;
    int set_get_field_type; /* Using int as the underlying type for the enum */
    unsigned short field_bit;
    unsigned char field_oktochange;
    unsigned char field_passcount;
    char *field_name;
    TIFFFieldArray *field_subfields;
} FuzzTIFFField;

static void fuzz_TIFFDataWidth(const uint8_t *data, size_t size) {
    if (size < 1) return;
    /* Map fuzz data to valid TIFFDataType range (0-15 typically) */
    TIFFDataType type = (TIFFDataType)(data[0] & 0x0F);
    (void)TIFFDataWidth(type);
}

static void fuzz_TIFFFieldFunctions(const uint8_t *data, size_t size) {
    if (size < 32) return;

    FuzzTIFFField field;
    memset(&field, 0, sizeof(field));

    /* Parse fuzz input into the struct fields */
    field.field_tag = *(uint32_t *)(data + 0);
    field.field_readcount = *(int16_t *)(data + 4);
    field.field_writecount = *(int16_t *)(data + 6);
    field.field_type = (TIFFDataType)(data[8] & 0x0F);
    field.field_anonymous = *(uint32_t *)(data + 12);
    /* Cast to int to simulate the enum value */
    field.set_get_field_type = (int)(data[16] & 0x0F);
    field.field_bit = *(uint16_t *)(data + 20);
    field.field_oktochange = data[22];
    field.field_passcount = data[23];
    field.field_name = NULL;
    field.field_subfields = NULL;

    /* 
     * The target functions expect a pointer to TIFFField. 
     * Since our FuzzTIFFField has the same layout (based on the provided definition),
     * we cast the pointer. This is safe for fuzzing the internal logic of these getters.
     */
    TIFFField *tif_field_ptr = (TIFFField *)&field;

    int count_size = TIFFFieldSetGetCountSize(tif_field_ptr);
    int is_anon = TIFFFieldIsAnonymous(tif_field_ptr);
    int read_count = TIFFFieldReadCount(tif_field_ptr);
    int write_count = TIFFFieldWriteCount(tif_field_ptr);
    int pass_count = TIFFFieldPassCount(tif_field_ptr);

    /* Suppress unused variable warnings */
    (void)count_size;
    (void)is_anon;
    (void)read_count;
    (void)write_count;
    (void)pass_count;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_TIFFDataWidth(Data, Size);
    fuzz_TIFFFieldFunctions(Data, Size);

    return 0;
}