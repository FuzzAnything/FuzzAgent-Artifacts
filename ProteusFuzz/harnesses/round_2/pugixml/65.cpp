// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
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
#include <random>

static const char* dummy_file_path = "./dummy_file";

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen(dummy_file_path, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void write_dummy_file_wide(const uint8_t* data, size_t size) {
    // For wide path tests, we just ensure the file exists with some content
    // The path itself is hardcoded to dummy_file_path, but we use a different internal name for wide test if needed
    // Here we just reuse the same file for simplicity as the API tests the path handling
    FILE* f = fopen(dummy_file_path, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize random generator for flags and encodings
    std::mt19937 gen(Size);
    std::uniform_int_distribution<unsigned int> flag_dist(0, 0xFFFFFFFF);
    std::uniform_int_distribution<int> enc_dist(0, 9); // 0 to 9 for enum values

    // Prepare data buffer
    std::vector<char> buffer(Size);
    std::memcpy(buffer.data(), Data, Size);

    // Write to dummy file for file-based operations
    write_dummy_file(Data, Size);

    // Test 1: load_file (char*)
    pugi::xml_document doc1;
    pugi::xml_parse_result result1 = doc1.load_file(dummy_file_path, flag_dist(gen), static_cast<pugi::xml_encoding>(enc_dist(gen)));
    // Check result status if needed, but fuzzer just needs to run
    
    // Test 2: load_file (wchar_t*)
    pugi::xml_document doc2;
    // Convert dummy_file_path to wchar_t if necessary, but for simplicity we assume the path is valid ASCII
    // In a real scenario, we might need to handle wchar_t conversion properly, but for fuzzing we assume the path string is valid
    // Since dummy_file_path is a const char*, we can't directly cast to wchar_t* without conversion.
    // However, the API expects wchar_t*. We will use a simple approach: if the system supports it, we try to load.
    // For the purpose of this fuzzer, we will skip the wchar_t path if conversion is complex, but the requirement says to invoke it.
    // We will assume the environment can handle the path as wide string if it's ASCII.
    // Note: This might not be portable, but for fuzzing we assume the path is valid.
    // To be safe, we will only call it if the path is valid ASCII (which it is).
    // We cast the char* to wchar_t* assuming ASCII compatibility. This is risky but common in fuzzing for simplicity.
    // Alternatively, we can use a wide string literal if the path is known.
    // Let's use a wide string literal for the path to be safe.
    const wchar_t* wide_path = L"./dummy_file";
    pugi::xml_parse_result result2 = doc2.load_file(wide_path, flag_dist(gen), static_cast<pugi::xml_encoding>(enc_dist(gen)));

    // If load succeeded, we can test save and print
    if (result1.status == pugi::status_ok || result2.status == pugi::status_ok) {
        // Use doc1 if it loaded successfully, otherwise doc2
        pugi::xml_document* doc = (result1.status == pugi::status_ok) ? &doc1 : &doc2;

        // Test 3: save to xml_writer_file
        FILE* f_out = fopen(dummy_file_path, "wb");
        if (f_out) {
            pugi::xml_writer_file writer_file(f_out);
            doc->save(writer_file, PUGIXML_TEXT("\t"), flag_dist(gen), static_cast<pugi::xml_encoding>(enc_dist(gen)));
            fclose(f_out);
        }

        // Test 4: save to std::ostream
        {
            std::ofstream ofs(dummy_file_path, std::ios::binary);
            if (ofs) {
                pugi::xml_writer_stream writer_stream(ofs);
                doc->save(writer_stream, PUGIXML_TEXT("\t"), flag_dist(gen), static_cast<pugi::xml_encoding>(enc_dist(gen)));
            }
        }

        // Test 5: save_file (char*)
        doc->save_file(dummy_file_path, PUGIXML_TEXT("\t"), flag_dist(gen), static_cast<pugi::xml_encoding>(enc_dist(gen)));

        // Test 6: save_file (wchar_t*)
        doc->save_file(wide_path, PUGIXML_TEXT("\t"), flag_dist(gen), static_cast<pugi::xml_encoding>(enc_dist(gen)));

        // Test 7: print on a node (root node)
        pugi::xml_node root = doc->root();
        if (root) {
            FILE* f_out2 = fopen(dummy_file_path, "wb");
            if (f_out2) {
                pugi::xml_writer_file writer_file2(f_out2);
                root.print(writer_file2, PUGIXML_TEXT("\t"), flag_dist(gen), static_cast<pugi::xml_encoding>(enc_dist(gen)), 0);
                fclose(f_out2);
            }

            {
                std::ofstream ofs2(dummy_file_path, std::ios::binary);
                if (ofs2) {
                    pugi::xml_writer_stream writer_stream2(ofs2);
                    root.print(writer_stream2, PUGIXML_TEXT("\t"), flag_dist(gen), static_cast<pugi::xml_encoding>(enc_dist(gen)), 0);
                }
            }
        }
    }

    // Cleanup: The documents are local and will be destroyed automatically.
    // No explicit cleanup needed for the documents as they are stack-allocated.
    // The dummy file is overwritten, which is acceptable for fuzzing.

    return 0;
}