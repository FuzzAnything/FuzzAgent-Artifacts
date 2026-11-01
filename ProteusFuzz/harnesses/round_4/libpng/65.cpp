// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_mDCV at pngget.c:860:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <fuzzer/FuzzedDataProvider.h>

// Define the structure before usage
struct MemoryReader {
    const uint8_t *data;
    size_t size;
    size_t offset;
};

// Custom read callback for memory buffer
static void read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == nullptr) return;
    
    MemoryReader *reader = (MemoryReader *)io_ptr;

    if (reader->offset + length > reader->size) {
        png_error(png_ptr, "Read error");
    }
    memcpy(data, reader->data + reader->offset, length);
    reader->offset += length;
}

static void fuzz_read_and_get_metadata(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    MemoryReader reader = {Data, Size, 0};

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    png_set_read_fn(png_ptr, &reader, read_from_memory);

    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_cLLI
    double max_content = 0.0, max_frame_avg = 0.0;
    png_get_cLLI(png_ptr, info_ptr, &max_content, &max_frame_avg);

    // Fuzz png_get_sRGB
    int srgb_intent = 0;
    png_get_sRGB(png_ptr, info_ptr, &srgb_intent);

    // Fuzz png_get_cHRM
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);

    // Fuzz png_get_mDCV
    double m_white_x, m_white_y, m_red_x, m_red_y, m_green_x, m_green_y, m_blue_x, m_blue_y;
    double max_lum, min_lum;
    png_get_mDCV(png_ptr, info_ptr, &m_white_x, &m_white_y, &m_red_x, &m_red_y, &m_green_x, &m_green_y, &m_blue_x, &m_blue_y, &max_lum, &min_lum);

    // Fuzz png_get_oFFs
    png_int_32 off_x, off_y;
    int unit_type;
    png_get_oFFs(png_ptr, info_ptr, &off_x, &off_y, &unit_type);

    // Fuzz png_get_cICP
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function, &matrix_coefficients, &video_full_range_flag);

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

static void fuzz_write_and_set_metadata(const uint8_t *Data, size_t Size) {
    if (Size < 19) return;

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Set basic image info (1x1, 8-bit grayscale)
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Fuzz png_set_cHRM with random doubles derived from input
    double w_x = (double)(Data[0] & 0xFF) / 255.0;
    double w_y = (double)(Data[1] & 0xFF) / 255.0;
    double r_x = (double)(Data[2] & 0xFF) / 255.0;
    double r_y = (double)(Data[3] & 0xFF) / 255.0;
    double g_x = (double)(Data[4] & 0xFF) / 255.0;
    double g_y = (double)(Data[5] & 0xFF) / 255.0;
    double b_x = (double)(Data[6] & 0xFF) / 255.0;
    double b_y = (double)(Data[7] & 0xFF) / 255.0;

    png_set_cHRM(png_ptr, info_ptr, w_x, w_y, r_x, r_y, g_x, g_y, b_x, b_y);

    // Fuzz png_set_mDCV
    double m_w_x = (double)(Data[8] & 0xFF) / 255.0;
    double m_w_y = (double)(Data[9] & 0xFF) / 255.0;
    double m_r_x = (double)(Data[10] & 0xFF) / 255.0;
    double m_r_y = (double)(Data[11] & 0xFF) / 255.0;
    double m_g_x = (double)(Data[12] & 0xFF) / 255.0;
    double m_g_y = (double)(Data[13] & 0xFF) / 255.0;
    double m_b_x = (double)(Data[14] & 0xFF) / 255.0;
    double m_b_y = (double)(Data[15] & 0xFF) / 255.0;
    double max_lum = (double)(Data[16] & 0xFF) * 1000.0;
    double min_lum = (double)(Data[17] & 0xFF);

    png_set_mDCV(png_ptr, info_ptr, m_w_x, m_w_y, m_r_x, m_r_y, m_g_x, m_g_y, m_b_x, m_b_y, max_lum, min_lum);

    // Write image data (1 byte for 1x1 grayscale)
    png_byte row[1];
    row[0] = static_cast<png_byte>(Data[18] & 0xFF);
    png_write_row(png_ptr, row);

    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Try both read and write paths
    fuzz_read_and_get_metadata(Data, Size);
    fuzz_write_and_set_metadata(Data, Size);

    return 0;
}