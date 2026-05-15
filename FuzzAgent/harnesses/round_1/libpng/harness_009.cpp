#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_009 {

// Simple ICC profile header (minimal valid ICC header)
// ICC profile header is 128 bytes minimum
static const uint8_t minimal_icc_header[128] = {
    0x00, 0x00, 0x00, 0x00, // Size (to be filled)
    0x61, 0x63, 0x73, 0x70, // Signature 'acsp'
    0x41, 0x50, 0x50, 0x4C, // Device manufacturer 'APPL'
    0x00, 0x00, 0x00, 0x00, // Device model
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Device attributes
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Rendering intent
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // XYZ values
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, // Profile creator
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Profile ID
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, // Reserved
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

// Error callback
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

// Warning callback
static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful metadata testing
    // Need enough for: ICC profile name, text entries, sCAL parameters
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create write structure
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // 1. Prepare and set ICC profile data
    {
        // Consume ICC profile name (max 79 chars as per PNG spec)
        std::string icc_name = fdp.ConsumeRandomLengthString(79);
        if (icc_name.empty()) {
            icc_name = "icc_profile";
        }
        
        // Consume compression type (should be 0 for PNG_COMPRESSION_TYPE_BASE)
        int compression_type = fdp.ConsumeIntegralInRange<int>(0, 0);
        
        // Create ICC profile data: header + optional extra data
        size_t extra_profile_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
        std::vector<uint8_t> icc_profile_data(sizeof(minimal_icc_header) + extra_profile_size);
        
        // Copy minimal ICC header
        memcpy(icc_profile_data.data(), minimal_icc_header, sizeof(minimal_icc_header));
        
        // Fill in size field (big endian)
        uint32_t total_size = icc_profile_data.size();
        icc_profile_data[0] = (total_size >> 24) & 0xFF;
        icc_profile_data[1] = (total_size >> 16) & 0xFF;
        icc_profile_data[2] = (total_size >> 8) & 0xFF;
        icc_profile_data[3] = total_size & 0xFF;
        
        // Fill extra profile data with fuzzed bytes if available
        if (extra_profile_size > 0) {
            std::vector<uint8_t> extra_data = fdp.ConsumeBytes<uint8_t>(extra_profile_size);
            if (extra_data.size() == extra_profile_size) {
                memcpy(icc_profile_data.data() + sizeof(minimal_icc_header), 
                       extra_data.data(), extra_profile_size);
            }
        }
        
        // Call png_set_iCCP - the primary target with 56 undiscovered branches
        png_set_iCCP(png_ptr, info_ptr, 
                     icc_name.c_str(), 
                     compression_type,
                     icc_profile_data.data(), 
                     static_cast<png_uint_32>(icc_profile_data.size()));
    }
    
    // 2. Prepare and set text entries
    {
        // Determine number of text entries (1-5 for reasonable testing)
        int num_text_entries = fdp.ConsumeIntegralInRange<int>(1, 5);
        std::vector<png_text> text_entries(num_text_entries);
        std::vector<std::string> persistent_strings; // Store all strings to ensure they persist
        persistent_strings.reserve(num_text_entries * 4); // keyword, text, lang, lang_key
        
        for (int i = 0; i < num_text_entries; i++) {
            // Consume text compression type
            int compression = fdp.ConsumeIntegralInRange<int>(-1, 2); // PNG_TEXT_COMPRESSION values
            
            // Consume and store keyword (max 79 chars as per PNG spec)
            persistent_strings.push_back(fdp.ConsumeRandomLengthString(79));
            if (persistent_strings.back().empty()) {
                persistent_strings.back() = "Keyword" + std::to_string(i);
            }
            
            // Consume and store text content
            persistent_strings.push_back(fdp.ConsumeRandomLengthString(1024));
            if (persistent_strings.back().empty()) {
                persistent_strings.back() = "Text content " + std::to_string(i);
            }
            
            // Set up png_text structure
            text_entries[i].compression = compression;
            text_entries[i].key = const_cast<char*>(persistent_strings[persistent_strings.size() - 2].c_str());
            text_entries[i].text = const_cast<char*>(persistent_strings.back().c_str());
            text_entries[i].text_length = persistent_strings.back().length();
            
            // For iTXt support (compression 1 or 2)
            if (compression == 1 || compression == 2) {
                // Consume and store language for iTXt
                persistent_strings.push_back(fdp.ConsumeRandomLengthString(79));
                
                // Consume and store language key for iTXt
                persistent_strings.push_back(fdp.ConsumeRandomLengthString(1024));
                
                text_entries[i].lang = const_cast<char*>(persistent_strings[persistent_strings.size() - 2].c_str());
                text_entries[i].lang_key = const_cast<char*>(persistent_strings.back().c_str());
                text_entries[i].itxt_length = persistent_strings.back().length();
            } else {
                text_entries[i].lang = NULL;
                text_entries[i].lang_key = NULL;
                text_entries[i].itxt_length = 0;
            }
        }
        
        // Call png_set_text
        png_set_text(png_ptr, info_ptr, text_entries.data(), num_text_entries);
    }
    
    // 3. Prepare and set sCAL data
    {
        // Consume unit type (0=meters, 1=radians)
        int unit = fdp.ConsumeIntegralInRange<int>(0, 1);
        
        // Use png_set_sCAL_s for string-based width/height (more flexible)
        std::string width_str = fdp.ConsumeRandomLengthString(50);
        std::string height_str = fdp.ConsumeRandomLengthString(50);
        
        if (width_str.empty()) width_str = "1.0";
        if (height_str.empty()) height_str = "1.0";
        
        // Try png_set_sCAL_s first (string-based)
        png_set_sCAL_s(png_ptr, info_ptr, unit, width_str.c_str(), height_str.c_str());
        
        // Also test floating point version if we have enough data
        if (fdp.remaining_bytes() >= sizeof(double) * 2) {
            double width = fdp.ConsumeFloatingPoint<double>();
            double height = fdp.ConsumeFloatingPoint<double>();
            
            // Call png_set_sCAL (floating point version)
            png_set_sCAL(png_ptr, info_ptr, unit, width, height);
        }
    }
    
    // 4. Test that the metadata was set by attempting to write header
    // (This would normally fail without image dimensions, but we can still
    // test that the structures are properly set up)
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_009
