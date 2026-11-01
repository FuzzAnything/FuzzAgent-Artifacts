// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = std::fopen("./dummy_file", "wb");
    if (f) {
        std::fwrite(data, 1, size, f);
        std::fclose(f);
    }
}

static void write_dummy_file_wide(const uint8_t* data, size_t size) {
    FILE* f = std::fopen("./dummy_file_wide", "wb");
    if (f) {
        std::fwrite(data, 1, size, f);
        std::fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    
    // Write input data to dummy files
    write_dummy_file(Data, Size);
    write_dummy_file_wide(Data, Size);

    // Generate random parameters
    unsigned int options = (Size > 0) ? (unsigned int)Data[0] : 0;
    unsigned int flags = (Size > 1) ? (unsigned int)Data[1] : 0;
    int enc_idx = (Size > 2) ? (int)Data[2] % 10 : 0;
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(enc_idx);
    
    // Ensure encoding is within valid range (0-9 based on enum definition)
    if (enc_idx >= 10) {
        encoding = pugi::encoding_auto;
    }

    // 1. Fuzz load_file (char* overload)
    pugi::xml_parse_result res1 = doc.load_file("./dummy_file", options, encoding);
    (void)res1; // Suppress unused warning, result checked implicitly by library state

    // 2. Fuzz load_file (wchar_t* overload)
    // Note: We reuse the same dummy file content, assuming it might be valid UTF-16 or binary
    // In a real scenario, we might convert Data to wchar_t, but for fuzzing raw bytes is often sufficient
    // to trigger encoding detection logic.
    pugi::xml_parse_result res2 = doc.load_file(L"./dummy_file_wide", options, encoding);
    (void)res2;

    // 3. Fuzz save_file (char* overload)
    doc.save_file("./dummy_file_out", PUGIXML_TEXT("\t"), flags, encoding);

    // 4. Fuzz save_file (wchar_t* overload)
    doc.save_file(L"./dummy_file_out_wide", PUGIXML_TEXT("\t"), flags, encoding);

    // 5. Fuzz save (xml_writer&)
    // Create a string stream to act as output
    std::ostringstream oss;
    pugi::xml_writer_stream writer(oss);
    doc.save(writer, PUGIXML_TEXT("\t"), flags, encoding);

    // 6. Fuzz print (xml_writer&)
    // Get root node using child_node() which returns the first child element
    // or use doc.child(PUGIXML_TEXT("")) if available, but child_node() is safer for generic access
    // Actually, doc.first_child() or doc.child_node() is the standard way to get the first child.
    // The error was using doc.child() without arguments.
    // Let's use doc.first_child() to get the first child node.
    pugi::xml_node root = doc.first_child();
    
    if (root) {
        std::ostringstream oss2;
        pugi::xml_writer_stream writer2(oss2);
        root.print(writer2, PUGIXML_TEXT("\t"), flags, encoding, 0);
    }

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}