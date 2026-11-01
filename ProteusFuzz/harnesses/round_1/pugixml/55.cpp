// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>

static pugi::xml_encoding get_random_encoding() {
    static const pugi::xml_encoding encodings[] = {
        pugi::encoding_auto,
        pugi::encoding_utf8,
        pugi::encoding_utf16_le,
        pugi::encoding_utf16_be,
        pugi::encoding_utf16,
        pugi::encoding_utf32_le,
        pugi::encoding_utf32_be,
        pugi::encoding_utf32,
        pugi::encoding_wchar,
        pugi::encoding_latin1
    };
    static const size_t count = sizeof(encodings) / sizeof(encodings[0]);
    return encodings[rand() % count];
}

static unsigned int get_random_flags() {
    // Combine various format flags randomly
    unsigned int flags = 0;
    if (rand() & 1) flags |= pugi::format_raw;
    if (rand() & 1) flags |= pugi::format_indent;
    if (rand() & 1) flags |= pugi::format_no_escapes;
    if (rand() & 1) flags |= pugi::format_write_bom;
    if (rand() & 1) flags |= pugi::format_no_declaration;
    if (rand() & 1) flags |= pugi::format_skip_control_chars;
    return flags;
}

static pugi::xml_parse_result load_document_from_buffer(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return doc.load_buffer_inplace(const_cast<void*>(static_cast<const void*>(data)), size);
    }
    
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = '\0';
    
    pugi::xml_parse_result result;
    result = doc.load_buffer_inplace(buffer.data(), size, pugi::parse_default, pugi::encoding_auto);
    if (result.status == pugi::status_ok) return result;
    
    result = doc.load_buffer_inplace(buffer.data(), size, pugi::parse_default, pugi::encoding_utf8);
    if (result.status == pugi::status_ok) return result;
    
    return doc.load_buffer_inplace(buffer.data(), size, pugi::parse_default, pugi::encoding_latin1);
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    srand(static_cast<unsigned int>(Data[0]) ^ (Size << 8));

    pugi::xml_document doc;
    
    pugi::xml_parse_result load_result = load_document_from_buffer(doc, Data, Size);
    
    if (load_result.status != pugi::status_ok) {
        std::string str(reinterpret_cast<const char*>(Data), Size);
        load_result = doc.load_string(str.c_str());
    }

    pugi::xml_node node = doc.root();
    if (node.empty()) {
        node = doc;
    }

    pugi::xml_encoding encoding = get_random_encoding();
    unsigned int flags = get_random_flags();
    const pugi::char_t* pugi_indent = (rand() & 1) ? PUGIXML_TEXT("\t") : PUGIXML_TEXT("  ");

    // Test save to stream
    {
        std::ostringstream oss;
        doc.save(oss, pugi_indent, flags, encoding);
    }

    // Test print to stream writer
    {
        std::ostringstream oss;
        pugi::xml_writer_stream writer(oss);
        node.print(writer, pugi_indent, flags, encoding, 0);
    }

    // Test save_file
    {
        bool success = doc.save_file("./dummy_file", pugi_indent, flags, encoding);
        (void)success; 
    }

    // Test load_file
    {
        FILE* f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
        }
        
        pugi::xml_document doc2;
        unsigned int options = rand();
        pugi::xml_encoding load_encoding = get_random_encoding();
        pugi::xml_parse_result load_res = doc2.load_file("./dummy_file", options, load_encoding);
        (void)load_res;
    }

    // Test xml_writer_file::write indirectly
    {
        FILE* f = fopen("./dummy_file", "wb");
        if (f) {
            pugi::xml_writer_file writer(f);
            node.print(writer, pugi_indent, flags, encoding, 0);
            fclose(f);
        }
    }

    // Test xml_writer_stream::write indirectly
    {
        std::ostringstream woss2;
        pugi::xml_writer_stream writer(woss2);
        node.print(writer, pugi_indent, flags, encoding, 0);
    }

    // Cleanup
    std::remove("./dummy_file");

    return 0;
}