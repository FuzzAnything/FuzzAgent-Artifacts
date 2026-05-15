// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_rows at pngset.c:1845:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_set_sPLT at pngset.c:1215:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

#include "code/png.h"

static void dummy_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void dummy_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static png_byte* create_dummy_rowbytes(png_structp png_ptr, png_infop info_ptr) {
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    if (rowbytes == 0) {
        int channels = 1;
        if (color_type & PNG_COLOR_MASK_COLOR) channels = 3;
        if (color_type & PNG_COLOR_MASK_ALPHA) channels++;
        rowbytes = (width * channels * bit_depth + 7) / 8;
    }
    
    return (png_byte*)png_malloc(png_ptr, rowbytes);
}

static void cleanup_png_structs(png_structp png_ptr, png_infop info_ptr, FILE* fp) {
    if (png_ptr) {
        if (info_ptr) {
            png_destroy_info_struct(png_ptr, &info_ptr);
        }
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
    if (fp) {
        fclose(fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_398(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_png_error, dummy_png_warning);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png_structs(png_ptr, nullptr, fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png_structs(png_ptr, nullptr, fp);
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 0);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    
    if (width > 0 && height > 0) {
        png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; ++y) {
            row_pointers[y] = create_dummy_rowbytes(png_ptr, info_ptr);
        }
        png_set_rows(png_ptr, info_ptr, row_pointers);
        
        png_read_image(png_ptr, row_pointers);
        
        png_read_end(png_ptr, info_ptr);
        
        for (png_uint_32 y = 0; y < height; ++y) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }

    png_uint_32 valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_IDAT);
    (void)valid_flags;

    if (Size > 20) {
        png_sPLT_tp splt_entry = (png_sPLT_tp)png_malloc(png_ptr, sizeof(png_sPLT_t));
        splt_entry->name = (char*)png_malloc(png_ptr, 6);
        strcpy(splt_entry->name, "dummy");
        splt_entry->depth = 8;
        splt_entry->nentries = 1;
        splt_entry->entries = (png_sPLT_entryp)png_malloc(png_ptr, sizeof(png_sPLT_entry));
        splt_entry->entries[0].red = 255;
        splt_entry->entries[0].green = 0;
        splt_entry->entries[0].blue = 0;
        splt_entry->entries[0].alpha = 255;
        splt_entry->entries[0].frequency = 1;
        
        png_set_sPLT(png_ptr, info_ptr, splt_entry, 1);
        
        png_free(png_ptr, splt_entry->entries);
        png_free(png_ptr, splt_entry->name);
        png_free(png_ptr, splt_entry);
    }

    if (Size > 30) {
        png_unknown_chunkp unknown_chunk = (png_unknown_chunkp)png_malloc(png_ptr, sizeof(png_unknown_chunk));
        strcpy((char*)unknown_chunk->name, "fUzZ");
        unknown_chunk->size = Size > 100 ? 100 : Size;
        unknown_chunk->data = (png_byte*)png_malloc(png_ptr, unknown_chunk->size);
        memcpy(unknown_chunk->data, Data, unknown_chunk->size);
        unknown_chunk->location = 0;
        
        png_set_unknown_chunks(png_ptr, info_ptr, unknown_chunk, 1);
        
        png_free(png_ptr, unknown_chunk->data);
        png_free(png_ptr, unknown_chunk);
    }

    cleanup_png_structs(png_ptr, info_ptr, fp);
    return 0;
}