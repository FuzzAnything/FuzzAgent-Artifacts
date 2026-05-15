// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_iCCP at pngset.c:859:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "png.h"

struct buffer_info {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function - does nothing
}

static void user_flush_data(png_structp png_ptr) {
    // Dummy flush function - does nothing
}

static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    buffer_info* buf = static_cast<buffer_info*>(io_ptr);
    if (buf->offset + length > buf->size) {
        length = buf->size - buf->offset;
    }
    if (length > 0) {
        memcpy(data, buf->data + buf->offset, length);
        buf->offset += length;
    }
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 50) return 0;
    
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_read) return 0;
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr_read)) || setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    buffer_info buf_info = {Data, Size, 0};
    png_set_read_fn(png_ptr_read, &buf_info, user_read_data);
    png_set_write_fn(png_ptr_write, nullptr, user_write_data, user_flush_data);
    
    png_read_info(png_ptr_read, info_ptr_read);
    
    png_uint_32 valid_flags[] = {
        PNG_INFO_gAMA, PNG_INFO_sBIT, PNG_INFO_cHRM, PNG_INFO_PLTE,
        PNG_INFO_tRNS, PNG_INFO_bKGD, PNG_INFO_hIST, PNG_INFO_pHYs,
        PNG_INFO_oFFs, PNG_INFO_tIME, PNG_INFO_pCAL, PNG_INFO_sRGB,
        PNG_INFO_iCCP, PNG_INFO_sPLT, PNG_INFO_sCAL, PNG_INFO_IDAT,
        PNG_INFO_eXIf, PNG_INFO_cICP
    };
    
    for (size_t i = 0; i < sizeof(valid_flags)/sizeof(valid_flags[0]); i++) {
        png_get_valid(png_ptr_read, info_ptr_read, valid_flags[i]);
    }
    
    char* name = nullptr;
    int compression_type = 0;
    png_byte* profile = nullptr;
    png_uint_32 proflen = 0;
    png_get_iCCP(png_ptr_read, info_ptr_read, &name, &compression_type, &profile, &proflen);
    
    png_byte colour_primaries = 0;
    png_byte transfer_function = 0;
    png_byte matrix_coefficients = 0;
    png_byte video_full_range_flag = 0;
    png_get_cICP(png_ptr_read, info_ptr_read, &colour_primaries, &transfer_function,
                 &matrix_coefficients, &video_full_range_flag);
    
    png_uint_32 width = 1;
    png_uint_32 height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    int interlace_method = PNG_INTERLACE_NONE;
    int compression_method = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_method = PNG_FILTER_TYPE_DEFAULT;
    int srgb_intent = PNG_sRGB_INTENT_PERCEPTUAL;
    
    if (Size >= 14) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
        if (width == 0) width = 1;
        if (height == 0) height = 1;
        bit_depth = Data[4] % 16 + 1;
        color_type = Data[5] % 6;
        interlace_method = Data[6] % 2;
        compression_method = Data[7] % 2;
        filter_method = Data[8] % 2;
        srgb_intent = Data[9] % 4;
    }
    
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                 color_type, interlace_method, compression_method, filter_method);
    
    png_set_sRGB_gAMA_and_cHRM(png_ptr_write, info_ptr_write, srgb_intent);
    
    if (Size > 60) {
        const char* icc_name = "test";
        int icc_compression = PNG_COMPRESSION_TYPE_BASE;
        const png_byte* icc_profile = Data + 20;
        png_uint_32 icc_proflen = Size - 20;
        if (icc_proflen > 10000) icc_proflen = 10000;
        
        png_set_iCCP(png_ptr_write, info_ptr_write, icc_name, icc_compression,
                     icc_profile, icc_proflen);
    }
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (fp) {
        png_init_io(png_ptr_write, fp);
        png_write_info(png_ptr_write, info_ptr_write);
        png_write_end(png_ptr_write, nullptr);
        fclose(fp);
    }
    
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    
    return 0;
}