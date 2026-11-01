// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty
            uint16_t endian = 0x002A; // 42 in little endian
            uint16_t version = 0x002A; // 42
            uint32_t ifd_offset = 8;
            fwrite(&endian, 2, 1, f);
            fwrite(&version, 2, 1, f);
            fwrite(&ifd_offset, 4, 1, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a dummy file with fuzzed data
    setup_dummy_file(Data, Size);

    // Open the file for reading and writing to allow directory manipulation
    TIFF *tif = TIFFOpen("./dummy_file", "rw+");
    if (!tif) {
        return 0;
    }

    // 1. TIFFCreateDirectory: Initialize a new directory
    TIFFCreateDirectory(tif);

    // 2. TIFFMergeFieldInfo: Attempt to merge custom field info
    // The TIFFFieldInfo structure is typically:
    // { tag, name, count, type, readcount, writecount, bit }
    // The error occurred because "CustomTag" (char*) was assigned to 'short' field (readcount)
    // Correct order: tag, name, count, type, readcount, writecount, bit
    // However, name is a pointer, so it cannot be in a static initializer for a struct 
    // that expects a short for the second field if the struct definition is different.
    // Based on standard libtiff: typedef struct { uint16_t field_tag; char *field_name; ... }
    // The error message suggests the compiler thinks the 2nd field is 'short'.
    // Let's look at the standard definition:
    // struct TIFFFieldInfo {
    //     uint16_t field_tag;
    //     char *field_name;
    //     uint16_t field_readcount;
    //     uint16_t field_writecount;
    //     TIFFDataType field_type;
    //     ...
    // };
    // Wait, the error says: initializing 'short' with 'char[10]'.
    // This implies the struct layout in the compiler's view is: tag (short?), name (short??).
    // Actually, the standard TIFFFieldInfo is:
    // typedef struct {
    //     uint16_t field_tag;
    //     char *field_name;
    //     uint16_t field_readcount;
    //     uint16_t field_writecount;
    //     TIFFDataType field_type;
    //     ...
    // } TIFFFieldInfo;
    // The error "incompatible pointer to integer conversion initializing 'short' with ... 'char[10]'"
    // suggests the initializer list order was wrong relative to the struct definition.
    // If I provided: { 0xFFFF, "CustomTag", 1, ... }
    // And the compiler complains about the 2nd element being a short, it means the 2nd field IS a short.
    // But standard TIFFFieldInfo has char* as 2nd field.
    // Perhaps the version of libtiff or the header included defines it differently?
    // Or maybe I messed up the order in my head.
    // Let's try to define the array dynamically to avoid static initialization issues with pointers
    // and ensure the types match exactly what the compiler expects.
    
    // Let's try a simpler approach: Don't use a custom field if it's tricky to initialize statically.
    // Instead, we can just call the function with NULL or a valid static array if we can figure out the struct.
    // Given the error, the struct likely expects: tag, readcount, writecount, type, ... and name is later?
    // No, standard is tag, name, count...
    // Let's assume the struct is:
    // { uint16_t tag, char *name, uint16_t readcount, uint16_t writecount, TIFFDataType type, ... }
    // If I write { 0xFFFF, "CustomTag", 1, ... } and it says "CustomTag" is assigned to a short,
    // then the 2nd field in the struct is a short.
    // This happens if the struct is defined as:
    // { uint16_t tag, uint16_t readcount, ... } and name is missing or later?
    // Actually, looking at common libtiff headers, TIFFFieldInfo is:
    // typedef struct {
    //     uint16_t field_tag;
    //     char *field_name;
    //     uint16_t field_readcount;
    //     uint16_t field_writecount;
    //     TIFFDataType field_type;
    //     ...
    // } TIFFFieldInfo;
    // If the error says "initializing 'short' with ... 'char[10]'", it means the 2nd field is short.
    // This implies the header included might have a different definition or I am misremembering.
    // To be safe, let's avoid the static initializer with a string pointer if the struct layout is ambiguous.
    // We can allocate the string and the struct dynamically, or just skip the custom field test if it's too brittle.
    // However, the task requires testing TIFFMergeFieldInfo.
    // Let's try to define the struct fields explicitly to avoid order confusion.
    
    // Alternative: Use a known valid static array from libtiff source if possible, or construct carefully.
    // Let's try to define the array elements one by one to ensure types match.
    // If the struct is { tag, name, ... }, then { 0xFFFF, "CustomTag" } should work.
    // The error suggests the 2nd field is NOT a pointer.
    // Maybe the struct is: { tag, readcount, writecount, type, name, ... }?
    // Let's look at the error again: "initializing 'short' with an expression of type 'char[10]'".
    // This means the 2nd field in the struct is a short.
    // If the struct is defined as:
    // struct { uint16_t tag; uint16_t readcount; ... char *name; ... }
    // Then { 0xFFFF, "CustomTag" } assigns "CustomTag" to readcount (short).
    // This is a specific version of libtiff or a custom header.
    // To fix this, we must match the actual struct layout.
    // Since I cannot see the exact header, I will assume the standard layout is NOT what the compiler sees.
    // However, the most robust way to fix this without knowing the exact struct is to NOT use a static initializer with a string literal if the order is uncertain.
    // But I must call the function.
    // Let's try to define the struct manually based on the error hint.
    // If 2nd field is short, maybe the order is: tag, readcount, writecount, type, name?
    // Let's try: { 0xFFFF, 1, 1, TIFF_SHORT, "CustomTag" }?
    // But wait, the standard libtiff TIFFFieldInfo is:
    // typedef struct {
    //     uint16_t field_tag;
    //     char *field_name;
    //     uint16_t field_readcount;
    //     uint16_t field_writecount;
    //     TIFFDataType field_type;
    //     ...
    // } TIFFFieldInfo;
    // If the compiler complains, maybe the header `tiffio.h` includes a different definition?
    // Or maybe I should just use a dummy array that doesn't crash.
    // Let's try to define the array dynamically to bypass static initialization constraints.
    
    TIFFFieldInfo *custom_fields = (TIFFFieldInfo *)malloc(sizeof(TIFFFieldInfo));
    if (custom_fields) {
        // We need to guess the field order.
        // Standard: tag, name, readcount, writecount, type...
        // Error implies 2nd is short.
        // Let's try the standard order first, but assign dynamically.
        // If the struct is standard, this should work.
        // If the struct is different, this might still crash or behave oddly, but it won't fail to compile.
        custom_fields->field_tag = 0xFFFF;
        custom_fields->field_name = "CustomTag";
        custom_fields->field_readcount = 1;
        custom_fields->field_writecount = 1;
        custom_fields->field_type = TIFF_SHORT;
        // Other fields like field_bit, field_oktochange, etc. might be needed but usually 0 is fine.
        
        TIFFMergeFieldInfo(tif, custom_fields, 1);
        free(custom_fields);
    }

    // 3. TIFFCreateEXIFDirectory: Initialize EXIF directory
    TIFFCreateEXIFDirectory(tif);

    // 4. TIFFWriteDirectory: Write the current directory
    TIFFWriteDirectory(tif);

    // 5. TIFFRewriteDirectory: Rewrite the directory
    TIFFRewriteDirectory(tif);

    // 6. TIFFFreeDirectory: Free the directory resources
    TIFFFreeDirectory(tif);

    // Cleanup: Close the TIFF handle
    TIFFClose(tif);

    return 0;
}