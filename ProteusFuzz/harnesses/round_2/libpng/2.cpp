// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_valid at pngget.c:19:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <png.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void static_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write fuzz data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        static_cleanup(png_ptr, info_ptr);
        return 0;
    }

    // Reopen file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        static_cleanup(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read PNG signature and header
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_tRNS
    png_byte *trans_alpha = nullptr;
    int num_trans = 0;
    png_color_16 *trans_color = nullptr;
    png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);
    if (trans_alpha) {
        // No need to free, managed by libpng
    }

    // Fuzz png_get_iCCP
    char *name = nullptr;
    int compression_type = 0;
    png_byte *profile = nullptr;
    png_uint_32 proflen = 0;
    png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);

    // Fuzz png_get_eXIf_1
    png_uint_32 num_exif = 0;
    png_byte *exif = nullptr;
    png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);

    // Fuzz png_get_tIME
    png_time mod_time;
    png_timep mod_time_ptr = &mod_time;
    png_get_tIME(png_ptr, info_ptr, &mod_time_ptr);

    // Fuzz png_get_cICP
    png_byte colour_primaries = 0;
    png_byte transfer_function = 0;
    png_byte matrix_coefficients = 0;
    png_byte video_full_range_flag = 0;
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function,
                 &matrix_coefficients, &video_full_range_flag);

    // Fuzz png_get_valid with various flags
    png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_iCCP);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_eXIf);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_tIME);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_cICP);
    png_get_valid(png_ptr, info_ptr, 0xFFFFFFFF); // Invalid flag test

    // Cleanup
    fclose(fp);
    static_cleanup(png_ptr, info_ptr);
    return 0;
}