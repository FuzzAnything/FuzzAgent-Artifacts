// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to dummy file for load_file tests
    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_node node;
    
    // Random options and encoding for fuzzing
    unsigned int options = 0;
    if (Size > 0) options = Data[0];
    if (Size > 1) options |= (static_cast<unsigned int>(Data[1]) << 8);
    
    pugi::xml_encoding encoding = pugi::encoding_auto;
    if (Size > 2) {
        int enc_val = Data[2] % 10; // 0-9 covers all enum values
        encoding = static_cast<pugi::xml_encoding>(enc_val);
    }

    // Test 1: load_file
    pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
    (void)result; // Suppress unused variable warning

    // Test 2: save_file
    // Use a different file path to avoid overwriting the input file immediately
    // Provide a valid indent string instead of nullptr to avoid assertion failure
    doc.save_file("./dummy_output", PUGIXML_TEXT("\t"), options, encoding);

    // Test 3: save with xml_writer_file
    FILE* f_save = fopen("./dummy_output_file", "wb");
    if (f_save) {
        pugi::xml_writer_file writer_file(f_save);
        doc.save(writer_file, PUGIXML_TEXT("\t"), options, encoding);
        fclose(f_save);
    }

    // Test 4: print with xml_node
    // If doc has a root, use it; otherwise use empty node
    pugi::xml_node root = doc.first_child();
    if (root.empty()) {
        node = pugi::xml_node();
    } else {
        node = root;
    }

    FILE* f_print = fopen("./dummy_print", "wb");
    if (f_print) {
        pugi::xml_writer_file writer_print(f_print);
        node.print(writer_print, PUGIXML_TEXT("\t"), options, encoding);
        fclose(f_print);
    }

    // Test 5: save with xml_writer_stream
    std::ostringstream oss;
    pugi::xml_writer_stream writer_stream(oss);
    doc.save(writer_stream, PUGIXML_TEXT("\t"), options, encoding);

    // Test 6: write with xml_writer_stream directly
    std::ostringstream oss2;
    pugi::xml_writer_stream writer_stream2(oss2);
    const char* test_data = "test";
    writer_stream2.write(test_data, 4);

    // Test 7: write with xml_writer_file directly
    FILE* f_write = fopen("./dummy_write", "wb");
    if (f_write) {
        pugi::xml_writer_file writer_file2(f_write);
        writer_file2.write(test_data, 4);
        fclose(f_write);
    }

    // Cleanup dummy files
    remove("./dummy_file");
    remove("./dummy_output");
    remove("./dummy_output_file");
    remove("./dummy_print");
    remove("./dummy_write");

    return 0;
}