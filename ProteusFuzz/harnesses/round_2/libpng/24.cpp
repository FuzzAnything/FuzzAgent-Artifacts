// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_tIME at pngset.c:1118:1 in png.h
// png_set_iCCP at pngset.c:859:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
// png_set_pCAL at pngset.c:482:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <vector>
#include <string>

static void dummy_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    abort();
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for all the fixed offsets we use
    if (Size < 210) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up dummy callbacks to prevent crashes during internal operations
    png_set_write_fn(png_ptr, NULL, dummy_write, NULL);
    png_set_error_fn(png_ptr, NULL, dummy_error, dummy_warning);

    // Helper lambda to safely extract C-strings from fuzz data
    auto get_string = [&](size_t offset, size_t max_len) -> std::string {
        if (offset >= Size) return "";
        size_t len = 0;
        while (offset + len < Size && len < max_len && Data[offset + len] != 0) {
            len++;
        }
        return std::string(reinterpret_cast<const char*>(Data + offset), len);
    };

    // Extract strings safely
    std::string purpose_str = get_string(0, 100);
    std::string units_str = get_string(10, 50);
    std::string name_str = get_string(60, 50);
    std::string swidth_str = get_string(110, 20);
    std::string sheight_str = get_string(130, 20);

    // Extract integers from data (Safe because Size >= 210)
    png_int_32 X0 = Data[150];
    png_int_32 X1 = Data[151];
    int type = Data[152] % 4; // Ensure type is 0-3
    int nparams = Data[153] % 256; // Ensure nparams is 0-255
    int unit = (Data[154] % 3) + 1; // Ensure unit is 1 or 2
    int compression_type = (Data[155] % 2) + 1; // Ensure compression_type is 1 or 2

    // Prepare params array for png_set_pCAL
    std::vector<std::string> params_strs;
    // Limit the number of params to avoid excessive memory usage or offset overflow
    int max_params = 20;
    if (nparams > max_params) nparams = max_params;
    
    for (int i = 0; i < nparams; ++i) {
        // Ensure we don't read past the buffer
        size_t param_offset = 160 + i * 10;
        if (param_offset + 10 > Size) break;
        params_strs.push_back(get_string(param_offset, 10));
    }
    
    std::vector<char*> params_ptrs;
    for (const auto& s : params_strs) {
        params_ptrs.push_back(const_cast<char*>(s.c_str()));
    }

    // Fuzz png_set_benign_errors
    int allowed = (Data[160] % 2);
    png_set_benign_errors(png_ptr, allowed);

    // Fuzz png_chunk_warning
    png_chunk_warning(png_ptr, "Test warning message");

    // Fuzz png_set_pCAL
    png_set_pCAL(png_ptr, info_ptr, purpose_str.c_str(), X0, X1, type, nparams, units_str.c_str(), params_ptrs.data());

    // Fuzz png_set_tIME
    png_time mod_time;
    mod_time.year = 2000 + (Data[161] % 100);
    mod_time.month = (Data[162] % 12) + 1;
    mod_time.day = (Data[163] % 31) + 1;
    mod_time.hour = Data[164] % 24;
    mod_time.minute = Data[165] % 60;
    mod_time.second = Data[166] % 60;
    png_set_tIME(png_ptr, info_ptr, &mod_time);

    // Fuzz png_set_iCCP
    const png_byte *profile = Data + 200;
    png_uint_32 proflen = 0;
    if (Size > 200) {
        proflen = Size - 200;
        if (proflen > 10000) {
            proflen = 10000;
        }
    }
    png_set_iCCP(png_ptr, info_ptr, name_str.c_str(), compression_type, profile, proflen);

    // Fuzz png_set_sCAL_s
    png_set_sCAL_s(png_ptr, info_ptr, unit, swidth_str.c_str(), sheight_str.c_str());

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}