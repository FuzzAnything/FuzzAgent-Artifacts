// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment: Create dummy file for file-based APIs
    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // 2. Setup randomization helpers
    bool use_random_options = (Size > 0) && (Data[0] & 1);
    bool use_random_encoding = (Size > 1) && (Data[1] & 1);
    
    unsigned int options = use_random_options ? (Data[0] & 0xFF) : pugi::parse_default;
    pugi::xml_encoding encoding = use_random_encoding 
        ? static_cast<pugi::xml_encoding>(Data[1] % 10) 
        : pugi::encoding_auto;

    // 3. Fuzz load_file and save_file
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(dummy_file, options, encoding);
        
        // If loaded successfully (or partially), try to save back to file
        if (result.status != pugi::status_file_not_found && result.status != pugi::status_io_error) {
            // Try saving with different flags
            unsigned int save_flags = (Size > 2) ? (Data[2] & 0xFF) : pugi::format_default;
            doc.save_file(dummy_file, "\t", save_flags, encoding);
        }
    }

    // 4. Fuzz save to stream (std::ostream)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(dummy_file, options, encoding);
        
        if (result.status != pugi::status_file_not_found && result.status != pugi::status_io_error) {
            std::ostringstream stream;
            unsigned int save_flags = (Size > 3) ? (Data[3] & 0xFF) : pugi::format_default;
            pugi::xml_encoding save_enc = (Size > 4) ? static_cast<pugi::xml_encoding>(Data[4] % 10) : pugi::encoding_auto;
            
            doc.save(stream, "\t", save_flags, save_enc);
        }
    }

    // 5. Fuzz xml_node::print to xml_writer_stream
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(dummy_file, options, encoding);
        
        if (result.status != pugi::status_file_not_found && result.status != pugi::status_io_error) {
            std::wstringstream wstream;
            pugi::xml_writer_stream writer(wstream);
            
            // child() requires a name argument. Use child() with empty string or first child logic.
            // Using child() with empty string returns the first child if any, or null.
            pugi::xml_node root = doc.child(""); 
            if (!root) {
                root = doc; // Fallback to document root
            }
            
            unsigned int print_flags = (Size > 5) ? (Data[5] & 0xFF) : pugi::format_default;
            pugi::xml_encoding print_enc = (Size > 6) ? static_cast<pugi::xml_encoding>(Data[6] % 10) : pugi::encoding_auto;
            
            root.print(writer, "\t", print_flags, print_enc, 0);
        }
    }

    // 6. Fuzz xml_writer_file::write directly
    {
        FILE* out_fp = fopen(dummy_file, "wb");
        if (out_fp) {
            pugi::xml_writer_file writer(out_fp);
            
            // Write raw data from fuzz input to test the writer's fwrite logic
            // We write a chunk to ensure the write method is exercised
            size_t write_size = (Size > 7) ? (Data[7] % 1024) : 64;
            if (write_size > Size) write_size = Size;
            
            writer.write(Data, write_size);
            fclose(out_fp);
        }
    }

    // 7. Cleanup
    std::remove(dummy_file);

    return 0;
}