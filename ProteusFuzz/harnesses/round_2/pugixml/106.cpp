// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare data for file writing
    write_dummy_file(Data, Size);
    write_dummy_file_wide(Data, Size);

    pugi::xml_document doc;
    pugi::xml_node node;

    // Random parameters
    unsigned int options = 0;
    if (Size > 0) options = Data[0] & 0xFF;
    if (Size > 1) options |= (Data[1] & 0xFF) << 8;
    if (Size > 2) options |= (Data[2] & 0xFF) << 16;
    if (Size > 3) options |= (Data[3] & 0xFF) << 24;

    int enc_idx = 0;
    if (Size > 4) enc_idx = Data[4] % 10; // 0 to 9 for xml_encoding
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(enc_idx);

    unsigned int flags = 0;
    if (Size > 5) flags = Data[5] & 0xFF;
    if (Size > 6) flags |= (Data[6] & 0xFF) << 8;

    // 1. Test pugi::xml_document::load_file
    pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
    (void)result; // Suppress unused warning

    // 2. Test pugi::xml_node::print
    // Prepare a simple output buffer
    std::ostringstream oss;
    pugi::xml_writer_stream writer(oss);
    
    // Try printing the document root (if loaded) or an empty node
    if (doc.root()) {
        doc.root().print(writer, PUGIXML_TEXT("\t"), flags, encoding, 0);
    } else {
        node.print(writer, PUGIXML_TEXT("\t"), flags, encoding, 0);
    }
    oss.str(""); // Clear buffer

    // 3. Test pugi::xml_document::save_file (char* overload)
    doc.save_file("./dummy_file", PUGIXML_TEXT("\t"), flags, encoding);

    // 3. Test pugi::xml_document::save_file (wchar_t* overload)
    // Note: On some platforms wchar_t might be 2 or 4 bytes, but the path string is simple
    doc.save_file(L"./dummy_file_wide", PUGIXML_TEXT("\t"), flags, encoding);

    // 4. Test pugi::xml_document::save (std::basic_ostream<char>& overload)
    std::ofstream ofs("./dummy_file");
    if (ofs.is_open()) {
        doc.save(ofs, PUGIXML_TEXT("\t"), flags, encoding);
        ofs.close();
    }

    // 4. Test pugi::xml_document::save (xml_writer& overload)
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        pugi::xml_writer_file writer_file(f);
        doc.save(writer_file, PUGIXML_TEXT("\t"), flags, encoding);
        fclose(f);
    }

    // Cleanup is handled by destructors of pugi objects
    return 0;
}