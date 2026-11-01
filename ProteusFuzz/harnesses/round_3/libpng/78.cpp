// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_bKGD at pngset.c:24:1 in png.h
// png_get_bKGD at pngget.c:502:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    } else if (info_ptr) {
        png_destroy_info_struct(NULL, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    FuzzedDataProvider fdp(Data, Size);

    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Initialize dummy file for reading
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Write fuzzed data to dummy file
    size_t write_size = fdp.ConsumeIntegralInRange<size_t>(8, Size);
    fwrite(Data, 1, write_size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read PNG signature and IHDR
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    // Fuzz png_set_background
    png_color_16 background_color = {0};
    background_color.red = fdp.ConsumeIntegral<uint16_t>();
    background_color.green = fdp.ConsumeIntegral<uint16_t>();
    background_color.blue = fdp.ConsumeIntegral<uint16_t>();
    background_color.gray = fdp.ConsumeIntegral<uint16_t>();

    int background_gamma_code = fdp.ConsumeIntegralInRange<int>(0, 3);
    int need_expand = fdp.ConsumeBool() ? 1 : 0;
    double background_gamma = fdp.ConsumeFloatingPoint<double>();

    png_set_background(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma);

    // Fuzz png_set_bKGD
    png_color_16 bkgd_color = {0};
    bkgd_color.red = fdp.ConsumeIntegral<uint16_t>();
    bkgd_color.green = fdp.ConsumeIntegral<uint16_t>();
    bkgd_color.blue = fdp.ConsumeIntegral<uint16_t>();
    bkgd_color.gray = fdp.ConsumeIntegral<uint16_t>();

    png_set_bKGD(png_ptr, info_ptr, &bkgd_color);

    // Fuzz png_get_bKGD
    png_color_16 *retrieved_bkgd = NULL;
    png_get_bKGD(png_ptr, info_ptr, &retrieved_bkgd);

    // Fuzz png_set_tRNS
    png_byte *trans_alpha = NULL;
    int num_trans = fdp.ConsumeIntegralInRange<int>(0, 256);
    if (num_trans > 0) {
        trans_alpha = (png_byte *)malloc(num_trans);
        if (trans_alpha) {
            std::vector<uint8_t> bytes = fdp.ConsumeBytes<uint8_t>(num_trans);
            memcpy(trans_alpha, bytes.data(), num_trans);
        }
    }

    png_color_16 trans_color = {0};
    trans_color.red = fdp.ConsumeIntegral<uint16_t>();
    trans_color.green = fdp.ConsumeIntegral<uint16_t>();
    trans_color.blue = fdp.ConsumeIntegral<uint16_t>();
    trans_color.gray = fdp.ConsumeIntegral<uint16_t>();

    png_set_tRNS(png_ptr, info_ptr, trans_alpha, num_trans, &trans_color);
    free(trans_alpha);

    // Fuzz png_get_tRNS
    png_byte *retrieved_trans_alpha = NULL;
    int retrieved_num_trans = 0;
    png_color_16 *retrieved_trans_color = NULL;
    png_get_tRNS(png_ptr, info_ptr, &retrieved_trans_alpha, &retrieved_num_trans, &retrieved_trans_color);

    // Fuzz png_get_valid with various flags
    png_uint_32 flags[] = {
        PNG_INFO_tRNS,
        PNG_INFO_bKGD,
        PNG_INFO_gAMA,
        PNG_INFO_cHRM,
        PNG_INFO_sRGB,
        PNG_INFO_iCCP,
        PNG_INFO_sPLT,
        PNG_INFO_sCAL,
        PNG_INFO_sBIT,
        PNG_INFO_hIST,
        PNG_INFO_pHYs,
        PNG_INFO_oFFs,
        PNG_INFO_pCAL,
        PNG_INFO_sCAL,
        PNG_INFO_sPLT,
        PNG_INFO_sRGB,
        PNG_INFO_iCCP,
        PNG_INFO_gAMA,
        PNG_INFO_cHRM,
        PNG_INFO_bKGD,
        PNG_INFO_tRNS,
        PNG_INFO_hIST,
        PNG_INFO_pHYs,
        PNG_INFO_oFFs,
        PNG_INFO_pCAL,
        PNG_INFO_sCAL,
        PNG_INFO_sBIT,
        PNG_INFO_sPLT,
        PNG_INFO_sRGB,
        PNG_INFO_iCCP
    };

    for (int i = 0; i < 30; i++) {
        png_get_valid(png_ptr, info_ptr, flags[i]);
    }

    // Cleanup
    cleanup_png(png_ptr, info_ptr);
    return 0;
}