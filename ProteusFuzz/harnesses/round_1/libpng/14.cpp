// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <cstring>
#include <cstdio>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    // Ensure minimal PNG signature or just write raw data to file
    write_dummy_png(Data, Size);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the file; ignore errors for fuzzing robustness
    png_read_info(png_ptr, info_ptr);

    // 1. png_get_iCCP
    char *name = nullptr;
    int compression_type = 0;
    png_byte *profile = nullptr;
    png_uint_32 proflen = 0;
    png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);
    if (profile && proflen > 0) {
        // No need to free, libpng manages this
    }

    // 2. png_get_eXIf_1
    png_uint_32 num_exif = 0;
    png_byte *exif = nullptr;
    png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);

    // 3. png_get_cICP
    png_byte colour_primaries = 0;
    png_byte transfer_function = 0;
    png_byte matrix_coefficients = 0;
    png_byte video_full_range_flag = 0;
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function,
                 &matrix_coefficients, &video_full_range_flag);

    // 4. png_get_sBIT
    // Requires pointer to png_color_8p (which is png_color_8*)
    png_color_8 sig_bit_local;
    png_color_8p sig_bit_ptr = &sig_bit_local;
    png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);

    // 5. png_get_tRNS
    // Requires pointer to png_color_16p (which is png_color_16*)
    png_byte *trans_alpha = nullptr;
    int num_trans = 0;
    png_color_16 trans_color_local;
    png_color_16p trans_color_ptr = &trans_color_local;
    png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color_ptr);

    // 6. png_get_valid checks
    png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_iCCP);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_eXIf);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_cICP);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_sBIT);

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}