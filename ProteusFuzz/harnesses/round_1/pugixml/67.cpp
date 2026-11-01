// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>

static bool is_valid_xml_encoding(unsigned int val) {
    return val <= pugi::encoding_latin1;
}

static bool is_valid_parse_options(unsigned int val) {
    return val < 0x10000000;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    unsigned int options = pugi::parse_default;
    pugi::xml_encoding encoding = pugi::encoding_auto;

    if (Size >= 2) {
        unsigned int raw_options = Data[0] | (Data[1] << 8);
        if (is_valid_parse_options(raw_options)) {
            options = raw_options;
        }
    }
    if (Size >= 3) {
        unsigned int raw_enc = Data[2];
        if (is_valid_xml_encoding(raw_enc)) {
            encoding = static_cast<pugi::xml_encoding>(raw_enc);
        }
    }

    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    pugi::xml_document doc;
    
    pugi::xml_parse_result result = doc.load_file(dummy_file, options, encoding);
    (void)result;

    doc.save_file(dummy_file, PUGIXML_TEXT("\t"), pugi::format_default, encoding);

    std::wstringstream wss;
    doc.save(wss, PUGIXML_TEXT("\t"), pugi::format_default | pugi::format_write_bom);

    std::stringstream ss;
    pugi::xml_writer_stream stream_writer(ss);
    if (doc.root()) {
        doc.root().print(stream_writer, PUGIXML_TEXT("\t"), pugi::format_default, encoding);
    }

    if (f) {
        FILE* fw = fopen(dummy_file, "wb");
        if (fw) {
            pugi::xml_writer_file file_writer(fw);
            const char* test_data = "<test/>";
            file_writer.write(test_data, sizeof(test_data) - 1);
            fclose(fw);
        }
    }

    remove(dummy_file);

    return 0;
}