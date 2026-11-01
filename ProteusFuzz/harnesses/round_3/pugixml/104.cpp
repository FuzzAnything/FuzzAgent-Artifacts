// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <random>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void write_dummy_file_wide(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file_wide", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize random number generator with input data for deterministic fuzzing
    std::mt19937 gen(Data[0]);
    std::uniform_int_distribution<unsigned int> opt_dist(0, 0xFFFFFFFF);
    std::uniform_int_distribution<int> enc_dist(0, 8); // Valid range for xml_encoding

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // 1. Test load_file
    // Write input to dummy file
    write_dummy_file(Data, Size);

    unsigned int options = opt_dist(gen);
    int enc_idx = enc_dist(gen);
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(enc_idx);

    result = doc.load_file("./dummy_file", options, encoding);

    // 2. Test save_file (char* overload)
    if (doc.empty()) {
        // If load failed or doc is empty, try to save anyway to test error handling
        doc.append_child("root");
    }
    
    // Fix: Ensure indent is a valid null-terminated string.
    // The previous code used raw input data which might not be null-terminated.
    // We create a safe buffer and copy a portion of the input, ensuring null termination.
    char indent_buf[16] = "\t"; // Default indent
    if (Size > 10) {
        size_t copy_len = (Size - 10) < 15 ? (Size - 10) : 15;
        memcpy(indent_buf, Data + 10, copy_len);
        indent_buf[copy_len] = '\0'; // Ensure null termination
    }
    const char* indent = indent_buf;

    unsigned int flags = opt_dist(gen);
    pugi::xml_encoding save_enc = static_cast<pugi::xml_encoding>(enc_dist(gen));
    
    bool save_res1 = doc.save_file("./dummy_file_out", indent, flags, save_enc);

    // 3. Test save_file (wchar_t* overload)
    const wchar_t* wide_path = L"./dummy_file_out_wide";
    bool save_res2 = doc.save_file(wide_path, indent, flags, save_enc);

    // 4. Test save (std::basic_ostream<char>& overload)
    std::ostringstream oss;
    doc.save(oss, indent, flags, save_enc);

    // 5. Test save (xml_writer& overload)
    FILE* f_writer = fopen("./dummy_file_writer", "wb");
    if (f_writer) {
        pugi::xml_writer_file writer(f_writer);
        doc.save(writer, indent, flags, save_enc);
        fclose(f_writer);
    }

    // 6. Test xml_node::print
    pugi::xml_node node = doc.child("root");
    if (!node) {
        node = doc.append_child("test_node");
    }
    
    // Print to stream
    std::ostringstream oss_print;
    node.print(oss_print, indent, flags, save_enc, 0);

    // Print to writer
    FILE* f_print = fopen("./dummy_file_print", "wb");
    if (f_print) {
        pugi::xml_writer_file print_writer(f_print);
        node.print(print_writer, indent, flags, save_enc, 0);
        fclose(f_print);
    }

    // Cleanup files created during fuzzing
    remove("./dummy_file");
    remove("./dummy_file_out");
    remove("./dummy_file_out_wide");
    remove("./dummy_file_writer");
    remove("./dummy_file_print");

    return 0;
}