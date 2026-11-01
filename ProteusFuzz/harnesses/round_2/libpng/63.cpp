// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    (void)chunk;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
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
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    FILE *read_fp = fopen(dummy_file, "rb");
    if (!read_fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_init_io(png_ptr, read_fp);

    png_uint_32 cache_max = 0;
    if (Size >= 4) {
        cache_max = *(const png_uint_32 *)(Data);
    }
    png_set_chunk_cache_max(png_ptr, cache_max);

    int keep_mode = 0;
    if (Size >= 8) {
        keep_mode = *(const int *)(Data + 4);
    }
    const png_byte *chunk_list = nullptr;
    int num_chunks = 0;
    if (Size >= 12) {
        num_chunks = *(const int *)(Data + 8);
        if (num_chunks > 0 && Size >= 12 + num_chunks * 4) {
            chunk_list = Data + 12;
        } else {
            num_chunks = 0;
            chunk_list = nullptr;
        }
    }
    png_set_keep_unknown_chunks(png_ptr, keep_mode, chunk_list, num_chunks);

    void *user_chunk_ptr = (void *)(size_t)(Size > 16 ? *(const uint32_t *)(Data + 12) : 0);
    png_set_read_user_chunk_fn(png_ptr, user_chunk_ptr, user_chunk_callback);

    if (Size >= 20) {
        png_byte chunk_name[5];
        chunk_name[0] = static_cast<png_byte>(Data[16] & 0x7F);
        chunk_name[1] = static_cast<png_byte>(Data[17] & 0x7F);
        chunk_name[2] = static_cast<png_byte>(Data[18] & 0x7F);
        chunk_name[3] = static_cast<png_byte>(Data[19] & 0x7F);
        chunk_name[4] = 0;
        int handle_result = png_handle_as_unknown(png_ptr, chunk_name);
        (void)handle_result;
    }

    png_read_info(png_ptr, info_ptr);

    png_unknown_chunkp unknown_chunks = nullptr;
    int num_unknown = png_get_unknown_chunks(png_ptr, info_ptr, &unknown_chunks);
    if (num_unknown > 0 && unknown_chunks) {
        int chunk_index = 0;
        if (Size >= 24) {
            chunk_index = *(const int *)(Data + 20) % num_unknown;
            if (chunk_index < 0) chunk_index = 0;
            if (chunk_index >= num_unknown) chunk_index = num_unknown - 1;
        }
        int location = 0;
        if (Size >= 28) {
            location = *(const int *)(Data + 24);
        }
        png_set_unknown_chunk_location(png_ptr, info_ptr, chunk_index, location);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(read_fp);
    remove(dummy_file);

    return 0;
}