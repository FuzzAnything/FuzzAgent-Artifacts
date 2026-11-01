// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

static void write_dummy_file_w(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file_w", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 9) return 0;

    pugi::xml_document doc;
    pugi::xml_document doc2;

    // Prepare random parameters safely
    unsigned int options = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    unsigned int flags = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
    int enc_idx = Data[8] % 10;
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(enc_idx);

    // Write dummy files for file-based operations
    write_dummy_file(Data, Size);
    write_dummy_file_w(Data, Size);

    // 1. Test load_file (char*)
    pugi::xml_parse_result res1 = doc.load_file("./dummy_file", options, encoding);
    (void)res1;

    // 2. Test load_file (wchar_t*)
    pugi::xml_parse_result res2 = doc2.load_file(L"./dummy_file_w", options, encoding);
    (void)res2;

    // 3. Test save_file (char*)
    doc.save_file("./dummy_file_out", PUGIXML_TEXT("\t"), flags, encoding);
    doc2.save_file("./dummy_file_out2", PUGIXML_TEXT("\t"), flags, encoding);

    // 4. Test save_file (wchar_t*)
    doc.save_file(L"./dummy_file_out_w", PUGIXML_TEXT("\t"), flags, encoding);
    doc2.save_file(L"./dummy_file_out2_w", PUGIXML_TEXT("\t"), flags, encoding);

    // 5. Test save with xml_writer_file
    FILE *f = fopen("./dummy_file_save", "wb");
    if (f) {
        pugi::xml_writer_file writer(f);
        doc.save(writer, PUGIXML_TEXT("\t"), flags, encoding);
        fclose(f);
    }

    // 6. Test save with xml_writer_stream
    std::ofstream ofs("./dummy_file_save_stream", std::ios::binary);
    if (ofs.is_open()) {
        pugi::xml_writer_stream writer(ofs);
        doc.save(writer, PUGIXML_TEXT("\t"), flags, encoding);
        ofs.close();
    }

    // 7. Test print on root node
    pugi::xml_node root = doc.root();
    if (root) {
        FILE *f2 = fopen("./dummy_file_print", "wb");
        if (f2) {
            pugi::xml_writer_file writer(f2);
            root.print(writer, PUGIXML_TEXT("\t"), flags, encoding, 0);
            fclose(f2);
        }

        std::ostringstream oss;
        pugi::xml_writer_stream writer2(oss);
        root.print(writer2, PUGIXML_TEXT("\t"), flags, encoding, 0);
    }

    // 8. Test print on a child node if exists
    pugi::xml_node child = doc.child("test");
    if (child) {
        FILE *f3 = fopen("./dummy_file_print_child", "wb");
        if (f3) {
            pugi::xml_writer_file writer(f3);
            child.print(writer, PUGIXML_TEXT("\t"), flags, encoding, 1);
            fclose(f3);
        }
    }

    return 0;
}