// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_set_iCCP at pngset.c:859:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

static void safe_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (info_ptr) {
        png_destroy_info_struct(png_ptr, &info_ptr);
    }
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        safe_cleanup(png_ptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_cleanup(png_ptr, info_ptr);
        return 0;
    }

    fp = fopen(dummy_file, "rb");
    if (!fp) {
        safe_cleanup(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);

    char *name = nullptr;
    int compression_type = 0;
    png_byte *profile = nullptr;
    png_uint_32 proflen = 0;
    png_uint_32 ret = png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);
    (void)ret;

    png_byte comp_type = png_get_compression_type(png_ptr, info_ptr);
    (void)comp_type;

    if (Size > 20) {
        const char *fuzz_name = (const char *)Data;
        int fuzz_compression = Data[10] % 2;
        const png_byte *fuzz_profile = Data + 20;
        png_uint_32 fuzz_proflen = Size - 20;
        png_set_iCCP(png_ptr, info_ptr, fuzz_name, fuzz_compression, fuzz_profile, fuzz_proflen);
    }

    png_free_data(png_ptr, info_ptr, PNG_FREE_ICCP, 0);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);

    fclose(fp);
    safe_cleanup(png_ptr, info_ptr);

    remove(dummy_file);

    return 0;
}