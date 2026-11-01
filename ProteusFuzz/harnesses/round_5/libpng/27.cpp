// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_cLLI at pngget.c:808:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <setjmp.h>

static void
fuzzer_error_fn(png_structp png_ptr, png_const_charp error_message)
{
    (void)png_ptr;
    (void)error_message;
}

static void
fuzzer_warning_fn(png_structp png_ptr, png_const_charp warning_message)
{
    (void)png_ptr;
    (void)warning_message;
}

// Context structure for the custom read function
struct fuzzer_io_context {
    const uint8_t *data;
    size_t size;
    size_t offset;
};

// Custom read callback to feed data from the fuzzer input
static void
fuzzer_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
    fuzzer_io_context *ctx = (fuzzer_io_context *)png_get_io_ptr(png_ptr);

    if (!ctx) return;

    if (ctx->offset + length > ctx->size) {
        // Not enough data, fill with zeros or stop
        size_t remaining = ctx->size - ctx->offset;
        if (remaining > 0) {
            memcpy(data, ctx->data + ctx->offset, remaining);
            ctx->offset += remaining;
            // Fill the rest with zeros to avoid uninitialized memory
            memset(data + remaining, 0, length - remaining);
        } else {
            memset(data, 0, length);
        }
    } else {
        memcpy(data, ctx->data + ctx->offset, length);
        ctx->offset += length;
    }
}

extern "C" int
LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Context for the custom read function
    fuzzer_io_context ctx = { Data, Size, 0 };

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, fuzzer_error_fn, fuzzer_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set the custom read function and pass the context
    png_set_read_fn(png_ptr, &ctx, fuzzer_read_data);

    // Use setjmp to catch libpng errors
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Read the signature and IHDR
    png_read_info(png_ptr, info_ptr);

    // Test png_get_IHDR
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0, interlace_method = 0, compression_method = 0, filter_method = 0;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Test png_get_valid with various flags
    png_get_valid(png_ptr, info_ptr, PNG_INFO_cLLI);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_cHRM);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_sRGB);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_eXIf);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);

    // Test png_get_cLLI
    double maximum_content_light_level = 0.0;
    double maximum_frame_average_light_level = 0.0;
    png_get_cLLI(png_ptr, info_ptr, &maximum_content_light_level, &maximum_frame_average_light_level);

    // Test png_get_cHRM_XYZ_fixed
    png_fixed_point int_red_X = 0, int_red_Y = 0, int_red_Z = 0;
    png_fixed_point int_green_X = 0, int_green_Y = 0, int_green_Z = 0;
    png_fixed_point int_blue_X = 0, int_blue_Y = 0, int_blue_Z = 0;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z,
                           &int_green_X, &int_green_Y, &int_green_Z,
                           &int_blue_X, &int_blue_Y, &int_blue_Z);

    // Test png_get_sRGB
    int file_srgb_intent = 0;
    png_get_sRGB(png_ptr, info_ptr, &file_srgb_intent);

    // Test png_get_eXIf_1
    png_uint_32 num_exif = 0;
    png_byte *exif = NULL;
    png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}