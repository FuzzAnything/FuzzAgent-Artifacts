// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_pCAL at pngget.c:994:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
#include <cstddef>
#include <cstring>
#include <vector>

// Dummy file path as requested
static const char* DUMMY_FILE = "./dummy_file";

// Helper to create a valid PNG file from fuzz input
static bool create_png_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen(DUMMY_FILE, "wb");
    if (!fp) return false;

    // Write a minimal valid PNG header
    // PNG signature
    const uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, fp);

    // IHDR chunk (minimal valid)
    // Length: 13 bytes
    uint32_t length = 13;
    fwrite(&length, 4, 1, fp);
    // Type: IHDR
    const char type[4] = {'I', 'H', 'D', 'R'};
    fwrite(type, 1, 4, fp);
    // Data: Width(4), Height(4), BitDepth(1), ColorType(1), Compression(1), Filter(1), Interlace(1)
    // Use minimal valid values
    uint32_t width = 1, height = 1;
    uint8_t bit_depth = 8, color_type = 2, compression = 0, filter = 0, interlace = 0;
    fwrite(&width, 4, 1, fp);
    fwrite(&height, 4, 1, fp);
    fwrite(&bit_depth, 1, 1, fp);
    fwrite(&color_type, 1, 1, fp);
    fwrite(&compression, 1, 1, fp);
    fwrite(&filter, 1, 1, fp);
    fwrite(&interlace, 1, 1, fp);
    
    // CRC (dummy, not validated by libpng in read mode for fuzzing usually, but we write 0)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, fp);

    // Append the fuzz data as raw bytes after IHDR (libpng will likely fail parsing, which is fine for fuzzing)
    // To make it more interesting, we can try to construct valid chunks if possible, but raw data is safer for fuzzing
    if (size > 0) {
        fwrite(data, 1, size, fp);
    }

    // Add IEND chunk
    length = 0;
    fwrite(&length, 4, 1, fp);
    const char iend_type[4] = {'I', 'E', 'N', 'D'};
    fwrite(iend_type, 1, 4, fp);
    crc = 0;
    fwrite(&crc, 4, 1, fp);

    fclose(fp);
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    // 1. Prepare environment
    // Create a dummy PNG file from input data
    if (!create_png_file(Data, Size)) {
        return 0;
    }

    FILE *fp = fopen(DUMMY_FILE, "rb");
    if (!fp) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Set up error handling to avoid crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the IHDR chunk
    // We use a try-catch block logic via setjmp, but for the read_info call, we just call it.
    // If the file is malformed, it will jump to the setjmp block above.
    png_read_info(png_ptr, info_ptr);

    // 2. Invoke target API functions
    // We will call the functions with various parameters to explore states.

    // --- png_get_IHDR ---
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // --- png_get_valid ---
    // Check for various chunk flags
    png_get_valid(png_ptr, info_ptr, PNG_INFO_sCAL);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_pCAL);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_sRGB);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_cHRM);

    // --- png_get_sCAL_s ---
    int unit;
    char *swidth = nullptr, *sheight = nullptr;
    png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
    // Note: If successful, libpng may allocate memory for swidth/sheight depending on version/flags,
    // but png_data_freer handles cleanup.

    // --- png_get_pCAL ---
    char *purpose = nullptr;
    png_int_32 X0, X1;
    int type, nparams;
    char *units = nullptr;
    char **params = nullptr;
    png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);

    // --- png_get_sRGB ---
    int file_srgb_intent;
    png_get_sRGB(png_ptr, info_ptr, &file_srgb_intent);

    // --- png_data_freer ---
    // Test setting freer for various masks. 
    // Note: PNG_FREE_sCAL, PNG_FREE_pCAL, PNG_FREE_sRGB are not standard macros in older libpng versions.
    // We use PNG_INFO_* flags which are often used as masks in png_data_freer in newer versions, 
    // or generic masks like PNG_FREE_ALL or PNG_FREE_NONE.
    // To ensure compilation across versions, we use PNG_INFO flags which are defined for these chunks.
    // If the specific PNG_FREE_* macros are missing, we rely on PNG_INFO_* flags which act as the mask.
    
    // Try to free data allocated by libpng (if any) using PNG_INFO flags as masks
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_sCAL);
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_pCAL);
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_sRGB);
    
    // Try user will free (should not crash even if data is null)
    png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, PNG_INFO_sCAL);
    png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, PNG_INFO_pCAL);

    // 3. Cleanup
    // Destroy structures
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}