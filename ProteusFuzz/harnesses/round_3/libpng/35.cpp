// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
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

static void write_dummy_png(const uint8_t *data, size_t size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input to a dummy file to simulate a PNG file
    write_dummy_png(Data, Size, "./dummy_file");

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Setup error handling
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

    // Attempt to read the PNG header and info
    png_read_info(png_ptr, info_ptr);

    fclose(fp);

    // Fuzz png_get_valid with valid flags
    // Replaced invalid PNG_INFO_iTXt with valid PNG_INFO_iCCP and PNG_INFO_eXIf
    png_uint_32 flags[] = {
        PNG_INFO_tRNS, PNG_INFO_sBIT, PNG_INFO_cICP, PNG_INFO_iCCP, 
        PNG_INFO_eXIf, PNG_INFO_gAMA, PNG_INFO_cHRM, PNG_INFO_sRGB,
        PNG_INFO_bKGD, PNG_INFO_hIST, PNG_INFO_pHYs, PNG_INFO_oFFs,
        PNG_INFO_pCAL, PNG_INFO_sCAL, PNG_INFO_sPLT, PNG_INFO_iCCP,
        PNG_INFO_eXIf, PNG_INFO_eXIf, PNG_INFO_eXIf, PNG_INFO_eXIf
    };
    const int num_flags = 20;
    for (int i = 0; i < num_flags; ++i) {
        png_get_valid(png_ptr, info_ptr, flags[i]);
    }

    // Fuzz png_get_sBIT
    png_color_8 sig_bit;
    png_color_8p sig_bit_ptr = &sig_bit;
    png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);

    // Fuzz png_get_cICP
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function, 
                 &matrix_coefficients, &video_full_range_flag);

    // Fuzz png_get_iCCP
    char *name = NULL;
    int compression_type = 0;
    png_byte *profile = NULL;
    png_uint_32 proflen = 0;
    png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);

    // Fuzz png_get_eXIf_1
    png_uint_32 num_exif = 0;
    png_byte *exif = NULL;
    png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);

    // Fuzz png_get_tRNS
    png_byte *trans_alpha = NULL;
    int num_trans = 0;
    png_color_16 trans_color;
    png_color_16p trans_color_ptr = &trans_color;
    png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}