// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
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
#include <fstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = std::fopen("./dummy_file", "wb");
    if (f) {
        std::fwrite(data, 1, size, f);
        std::fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Convert input to string for parsing, ensuring null termination if needed for string APIs
    // We treat the input as raw bytes to be parsed as XML
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    pugi::xml_document doc;
    
    // 1. Test load_file
    write_dummy_file(Data, Size);
    // Random options and encoding
    uint32_t rand_opts = 0;
    if (Size > 0) rand_opts = Data[0] % 16; // Limit to reasonable range
    pugi::xml_encoding rand_enc = static_cast<pugi::xml_encoding>(Size > 1 ? Data[1] % 10 : 0);
    
    pugi::xml_parse_result load_result = doc.load_file("./dummy_file", rand_opts, rand_enc);
    
    // 2. Test save_file
    // Try to save to a different dummy file
    bool save_file_result = doc.save_file("./dummy_output", PUGIXML_TEXT("\t"), 
                                          Size > 2 ? Data[2] % 256 : 0, 
                                          static_cast<pugi::xml_encoding>(Size > 3 ? Data[3] % 10 : 0));
    
    // 3. Test save to stream
    std::ostringstream oss;
    doc.save(oss, PUGIXML_TEXT("\t"), Size > 4 ? Data[4] % 256 : 0, static_cast<pugi::xml_encoding>(Size > 5 ? Data[5] % 10 : 0));
    
    // 4. Test xml_writer_file::write
    FILE* f = std::fopen("./dummy_writer_file", "wb");
    if (f) {
        pugi::xml_writer_file writer_file(f);
        const char* test_data = "<?xml version=\"1.0\"?><root/>";
        size_t test_size = std::strlen(test_data);
        writer_file.write(test_data, test_size);
        std::fclose(f);
    }
    
    // 5. Test xml_writer_stream::write
    std::wstringstream woss;
    pugi::xml_writer_stream writer_stream(woss);
    const wchar_t* wtest_data = L"<?xml version=\"1.0\"?><root/>";
    size_t wtest_size = std::wcslen(wtest_data) * sizeof(wchar_t);
    writer_stream.write(wtest_data, wtest_size);
    
    // 6. Test xml_node::print
    pugi::xml_node node = doc.child("root");
    if (!node) {
        // If no root, try to load from string to ensure we have a node to print
        // Note: load(const char_t*, options) does not take encoding as 3rd arg
        doc.load(input_str.c_str(), rand_opts);
        node = doc.child("root");
    }
    
    if (node) {
        std::ostringstream node_oss;
        node.print(node_oss, PUGIXML_TEXT("\t"), Size > 6 ? Data[6] % 256 : 0, static_cast<pugi::xml_encoding>(Size > 7 ? Data[7] % 10 : 0), 0);
        
        // Also test with xml_writer_file
        FILE* f2 = std::fopen("./dummy_node_file", "wb");
        if (f2) {
            pugi::xml_writer_file writer_file2(f2);
            node.print(writer_file2, PUGIXML_TEXT("\t"), Size > 8 ? Data[8] % 256 : 0, static_cast<pugi::xml_encoding>(Size > 9 ? Data[9] % 10 : 0), 0);
            std::fclose(f2);
        }
        
        // Test with xml_writer_stream
        std::wstringstream node_woss;
        pugi::xml_writer_stream writer_stream2(node_woss);
        node.print(writer_stream2, PUGIXML_TEXT("\t"), Size > 10 ? Data[10] % 256 : 0, static_cast<pugi::xml_encoding>(Size > 11 ? Data[11] % 10 : 0), 0);
    }
    
    return 0;
}