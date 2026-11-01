// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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

static void write_data_to_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Convert input to char_t* for loading
    std::vector<pugi::char_t> buffer(Size);
    std::memcpy(buffer.data(), Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // 1. Test load_file by writing data to a file first
    write_data_to_file(Data, Size);
    result = doc.load_file("./dummy_file");
    (void)result; // Ignore result for fuzzing stability, just ensure no crash

    // 2. Test load_buffer directly with various encodings
    // Randomly select an encoding for load_buffer
    pugi::xml_encoding encodings[] = {
        pugi::encoding_auto, pugi::encoding_utf8, pugi::encoding_utf16_le,
        pugi::encoding_utf16_be, pugi::encoding_utf16, pugi::encoding_utf32_le,
        pugi::encoding_utf32_be, pugi::encoding_utf32, pugi::encoding_wchar,
        pugi::encoding_latin1
    };
    
    // Safe access to Data with size checks
    int enc_idx = (Size > 0) ? (Data[0] % 10) : 0;
    pugi::xml_encoding enc = encodings[enc_idx];

    // Try loading as buffer to populate doc if file load failed or for variety
    doc.load_buffer(buffer.data(), buffer.size(), pugi::parse_default, enc);

    // 3. Test xml_node::print
    pugi::xml_node root = doc.first_child();
    if (!root) root = doc; // Fallback to document if no children

    std::ostringstream oss;
    // Random flags and indent
    unsigned int flags = (Size > 1) ? (Data[1] & 0xFF) : 0;
    
    // Create a string for indent to satisfy const char_t* requirement
    pugi::char_t indent_char = (Size > 1) ? (pugi::char_t)Data[1] : '\t';
    if (indent_char < 32 && indent_char != '\t') indent_char = '\t';
    pugi::char_t indent_str[] = { indent_char, 0 };
    const pugi::char_t* indent_ptr = indent_str;

    root.print(oss, indent_ptr, flags, enc, 0);

    // 4. Test xml_document::save
    std::ostringstream oss2;
    doc.save(oss2, indent_ptr, flags, enc);

    // 5. Test xml_writer_file::write
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        pugi::xml_writer_file writer(f);
        // Write some dummy data to trigger the write logic
        const char* dummy = "test";
        writer.write(dummy, 4);
        fclose(f);
    }

    // 6. Test xml_document::save_file
    doc.save_file("./dummy_file", indent_ptr, flags, enc);

    // 7. Test xml_writer_stream::write
    std::ostringstream oss3;
    pugi::xml_writer_stream stream_writer(oss3);
    const char* stream_data = "stream_test";
    stream_writer.write(stream_data, strlen(stream_data));

    // 8. Test load_file with random options and encoding
    write_data_to_file(Data, Size);
    unsigned int options = (Size > 2) ? (Data[2] & 0xFF) : 0;
    int enc_idx2 = (Size > 3) ? (Data[3] % 10) : 0;
    pugi::xml_encoding enc2 = encodings[enc_idx2];
    result = doc.load_file("./dummy_file", options, enc2);
    (void)result;

    return 0;
}