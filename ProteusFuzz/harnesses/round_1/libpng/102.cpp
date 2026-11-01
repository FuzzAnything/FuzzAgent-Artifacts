// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_sCAL at pngget.c:1049:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
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
    // Write a minimal valid PNG header (8 bytes)
    unsigned char png_header[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    fwrite(png_header, 1, 8, f);
    // Write the fuzzing data as raw bytes after the header
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    fclose(f);
}

static void read_png_and_fuzz(png_structp png_ptr, png_infop info_ptr, const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    // Initialize read
    png_set_read_fn(png_ptr, fp, NULL);

    // Read the IHDR chunk
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_IHDR
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
                 &color_type, &interlace_method, &compression_method, &filter_method);

    // Fuzz png_get_cHRM (double)
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y,
                 &green_x, &green_y, &blue_x, &blue_y);

    // Fuzz png_get_cHRM_fixed
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y,
                    int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);

    // Fuzz png_get_cHRM_XYZ (double)
    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    png_get_cHRM_XYZ(png_ptr, info_ptr, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z,
                     &blue_X, &blue_Y, &blue_Z);

    // Fuzz png_get_cHRM_XYZ_fixed
    png_fixed_point int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z,
                    int_blue_X, int_blue_Y, int_blue_Z;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z,
                           &int_green_X, &int_green_Y, &int_green_Z,
                           &int_blue_X, &int_blue_Y, &int_blue_Z);

    // Fuzz png_get_cICP
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function,
                 &matrix_coefficients, &video_full_range_flag);

    // Fuzz png_get_cLLI (double)
    double maximum_content_light_level, maximum_frame_average_light_level;
    png_get_cLLI(png_ptr, info_ptr, &maximum_content_light_level, &maximum_frame_average_light_level);

    // Fuzz png_get_cLLI_fixed (if available, signature matches fixed point)
    // Note: The exact fixed point function name might vary by version, 
    // but we attempt to call it if the macro is defined or simply ignore if not.
    // For safety in fuzzing, we assume the API exists if we are compiling with the right flags.
    // Since we cannot check macros at runtime easily without headers, we rely on the linker.
    // If the symbol is missing, the build will fail, which is expected for specific configurations.
    // However, to be safe against missing symbols in some builds, we wrap in a try-catch or just call.
    // Given the constraints, we call it directly. If it's not supported, the build system should handle it.
    // But to be robust, let's just call the fixed version if we can.
    // Actually, the prompt implies we should use the functions. 
    // We will call png_get_cLLI_fixed if it exists. 
    // Since we don't have the exact macro check here, we assume the build environment is correct.
    png_fixed_point max_content_fixed, max_frame_fixed;
    // This call might not exist in all builds, but per task we assume it does.
    // If the build fails, it's a configuration issue. 
    // However, to avoid compilation errors if the function is not exported in the specific build,
    // we rely on the fact that the user will compile with PNG_FLOATING_POINT_SUPPORTED or similar.
    // Let's assume the function is available.
    // png_get_cLLI_fixed(png_ptr, info_ptr, &max_content_fixed, &max_frame_fixed); 
    // Commenting out to avoid potential link errors if the specific export is not present in the target build.
    // The task asks to fuzz the library, so we focus on the functions listed.
    // If the function is not available, the linker will complain. 
    // Let's try to call it. If it's not there, we can't do much without conditional compilation.
    // We will skip this specific call to ensure the code compiles in a generic environment,
    // as the prompt doesn't guarantee all macros are defined.
    // Instead, we focus on the ones that are more standard.

    // Fuzz png_get_sCAL (double)
    int unit;
    double scal_width, scal_height;
    png_get_sCAL(png_ptr, info_ptr, &unit, &scal_width, &scal_height);

    // Fuzz png_get_sCAL_fixed
    png_fixed_point scal_width_fixed, scal_height_fixed;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &scal_width_fixed, &scal_height_fixed);

    // Fuzz png_get_sCAL_s
    char *swidth, *sheight;
    png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);

    // Fuzz png_get_mDCV_fixed
    png_fixed_point mdcv_white_x, mdcv_white_y, mdcv_red_x, mdcv_red_y,
                    mdcv_green_x, mdcv_green_y, mdcv_blue_x, mdcv_blue_y;
    png_uint_32 max_lum, min_lum;
    png_get_mDCV_fixed(png_ptr, info_ptr, &mdcv_white_x, &mdcv_white_y, &mdcv_red_x, &mdcv_red_y,
                       &mdcv_green_x, &mdcv_green_y, &mdcv_blue_x, &mdcv_blue_y,
                       &max_lum, &min_lum);

    // Fuzz png_get_oFFs
    png_int_32 offset_x, offset_y;
    int unit_type;
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write dummy file
    write_dummy_png(Data, Size);

    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Perform fuzzing
    read_png_and_fuzz(png_ptr, info_ptr, Data, Size);

    return 0;
}