// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
#include "code/png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler
    (void)png_ptr;
    (void)warning_msg;
}

static png_structp create_read_struct() {
    return png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
}

static png_structp create_write_struct() {
    return png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
}

extern "C" int LLVMFuzzerTestOneInput_299(const uint8_t *Data, size_t Size) {
    // Initialize dummy file
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    dummy_file = fopen("./dummy_file", "rb");
    if (!dummy_file) {
        return 0;
    }
    
    // Test png_save_uint_16
    if (Size >= 2) {
        png_byte buf16[2];
        unsigned int value16 = (Data[0] << 8) | Data[1];
        png_save_uint_16(buf16, value16);
    }
    
    // Test png_save_int_32
    if (Size >= 4) {
        png_byte buf32[4];
        png_int_32 value32 = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        png_save_int_32(buf32, value32);
    }
    
    // Test png_save_uint_32
    if (Size >= 4) {
        png_byte buf32[4];
        png_uint_32 value32 = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        png_save_uint_32(buf32, value32);
    }
    
    // Test png_get_uint_31
    if (Size >= 4) {
        png_structp png_ptr = create_read_struct();
        if (png_ptr) {
            png_byte buf[4];
            memcpy(buf, Data, 4);
            png_uint_32 result = png_get_uint_31(png_ptr, buf);
            (void)result;
            png_destroy_read_struct(&png_ptr, NULL, NULL);
        }
    }
    
    // Test png_write_chunk_start and png_write_chunk_data
    if (Size >= 8) {
        png_structp png_ptr = create_write_struct();
        if (png_ptr) {
            if (setjmp(png_jmpbuf(png_ptr))) {
                png_destroy_write_struct(&png_ptr, NULL);
                fclose(dummy_file);
                remove("./dummy_file");
                return 0;
            }
            
            png_init_io(png_ptr, dummy_file);
            
            png_byte chunk_name[4];
            memcpy(chunk_name, Data, 4);
            
            png_uint_32 length = 0;
            if (Size >= 8) {
                length = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
                if (length > 1024) {
                    length = 1024;
                }
            }
            
            png_write_chunk_start(png_ptr, chunk_name, length);
            
            if (length > 0 && Size >= 8 + length) {
                const png_byte *chunk_data = Data + 8;
                png_write_chunk_data(png_ptr, chunk_data, length);
            }
            
            png_destroy_write_struct(&png_ptr, NULL);
        }
    }
    
    fclose(dummy_file);
    remove("./dummy_file");
    
    return 0;
}