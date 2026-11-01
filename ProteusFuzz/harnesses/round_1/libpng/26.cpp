// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cstdint>
#include <cstddef>

static void write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE *fp = static_cast<FILE*>(png_get_io_ptr(png_ptr));
    if (fp && length > 0) {
        fwrite(data, 1, length, fp);
    }
}

static void write_data_flush(png_structp png_ptr) {
    FILE *fp = static_cast<FILE*>(png_get_io_ptr(png_ptr));
    if (fp) {
        fflush(fp);
    }
}

static void write_data_close(png_structp png_ptr) {
    FILE *fp = static_cast<FILE*>(png_get_io_ptr(png_ptr));
    if (fp) {
        fclose(fp);
    }
}

static png_structp create_png_struct_for_write(png_infop *info_ptr_out, FILE *fp) {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return nullptr;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return nullptr;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return nullptr;
    }

    png_set_write_fn(png_ptr, fp, write_data, write_data_flush);
    png_set_IHDR(png_ptr, info_ptr, 10, 10, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    *info_ptr_out = info_ptr;
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }

    png_infop info_ptr = nullptr;
    png_structp png_ptr = create_png_struct_for_write(&info_ptr, fp);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    // Initialize row buffer
    const size_t row_size = 10 * 3; // 10 width, 3 channels (RGB)
    const size_t total_rows = 10;
    const size_t total_data_size = row_size * total_rows;
    
    // Use static arrays to avoid heap allocation and potential leaks in early returns
    png_byte row_data[total_data_size];
    png_byte* rows[total_rows];
    
    for (size_t i = 0; i < total_rows; ++i) {
        rows[i] = row_data + (i * row_size);
    }

    // Fill row data with fuzz input safely
    for (size_t i = 0; i < total_data_size; ++i) {
        row_data[i] = Data[i % Size];
    }

    // Call target functions
    // 1. png_write_sig
    png_write_sig(png_ptr);

    // 2. png_write_info (required before writing rows)
    png_write_info(png_ptr, info_ptr);

    // 3. png_write_row
    if (Size > 0) {
        png_write_row(png_ptr, row_data);
    }

    // 4. png_write_rows
    if (Size > 1) {
        png_uint_32 num_rows = (Data[0] % 10) + 1;
        png_write_rows(png_ptr, rows, num_rows);
    }

    // 5. png_write_flush
    png_write_flush(png_ptr);

    // 6. png_write_chunk
    if (Size > 3) {
        png_byte chunk_name[5] = {'t', 'E', 'X', 't', '\0'}; // tEXt chunk
        size_t data_len = Data[1] % (Size - 2);
        const png_byte *chunk_data = Data + 2;
        png_write_chunk(png_ptr, chunk_name, chunk_data, data_len);
    }

    // End of image
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}