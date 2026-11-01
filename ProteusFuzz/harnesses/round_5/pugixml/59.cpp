// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
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
#include <random>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::mt19937 gen(Data[0] ^ (Size << 8));
    std::uniform_int_distribution<unsigned int> dist_opt(0, 0xFFFFFFFF);
    std::uniform_int_distribution<int> dist_enc(0, 10);

    pugi::xml_document doc;
    bool load_success = false;
    pugi::xml_parse_result result;

    write_dummy_file(Data, Size);

    unsigned int options = dist_opt(gen);
    int enc_val = dist_enc(gen);
    pugi::xml_encoding encoding = pugi::encoding_auto;
    switch (enc_val % 10) {
        case 0: encoding = pugi::encoding_auto; break;
        case 1: encoding = pugi::encoding_utf8; break;
        case 2: encoding = pugi::encoding_utf16_le; break;
        case 3: encoding = pugi::encoding_utf16_be; break;
        case 4: encoding = pugi::encoding_utf16; break;
        case 5: encoding = pugi::encoding_utf32_le; break;
        case 6: encoding = pugi::encoding_utf32_be; break;
        case 7: encoding = pugi::encoding_utf32; break;
        case 8: encoding = pugi::encoding_wchar; break;
        case 9: encoding = pugi::encoding_latin1; break;
    }

    result = doc.load_file("./dummy_file", options, encoding);
    load_success = result.status == pugi::status_ok;

    // Fuzz xml_node::print (Target 3)
    // Use std::ostringstream (char stream) and match the signature:
    // void print(std::basic_ostream<char>& os, const char_t* indent, unsigned int flags, xml_encoding encoding, unsigned int depth)
    std::ostringstream ostream;
    if (doc.root()) {
        doc.root().print(ostream, "\t", dist_opt(gen), pugi::encoding_auto, 0);
        if (doc.root().first_child()) {
            doc.root().first_child().print(ostream, "  ", dist_opt(gen), pugi::encoding_auto, 1);
        }
    }
    pugi::xml_node null_node;
    null_node.print(ostream, "\t", dist_opt(gen), pugi::encoding_auto, 0);

    // Fuzz xml_document::save (Target 1)
    // Signature: void save(std::basic_ostream<char>& stream, const char_t* indent, unsigned int flags) const;
    if (load_success) {
        doc.save(ostream, "\t", dist_opt(gen));
    }

    // Fuzz xml_writer_stream::write (Target 6)
    {
        std::stringstream narrow_stream;
        pugi::xml_writer_stream writer_stream(narrow_stream);
        const char* test_data = "test_write_data";
        size_t test_size = strlen(test_data);
        writer_stream.write(test_data, test_size);
        if (Size > 0) {
            writer_stream.write(Data, Size > 100 ? 100 : Size);
        }
    }

    // Fuzz xml_writer_file::write (Target 2)
    {
        FILE* f = fopen("./dummy_file_out", "wb");
        if (f) {
            pugi::xml_writer_file writer_file(f);
            const char* test_data = "file_write_test";
            size_t test_size = strlen(test_data);
            writer_file.write(test_data, test_size);
            if (Size > 0) {
                writer_file.write(Data, Size > 100 ? 100 : Size);
            }
            fclose(f);
        }
    }

    // Fuzz xml_document::save_file (Target 5)
    if (load_success) {
        unsigned int save_flags = dist_opt(gen);
        int save_enc_val = dist_enc(gen);
        pugi::xml_encoding save_encoding = pugi::encoding_auto;
        switch (save_enc_val % 10) {
            case 0: save_encoding = pugi::encoding_auto; break;
            case 1: save_encoding = pugi::encoding_utf8; break;
            case 2: save_encoding = pugi::encoding_utf16_le; break;
            case 3: save_encoding = pugi::encoding_utf16_be; break;
            case 4: save_encoding = pugi::encoding_utf16; break;
            case 5: save_encoding = pugi::encoding_utf32_le; break;
            case 6: save_encoding = pugi::encoding_utf32_be; break;
            case 7: save_encoding = pugi::encoding_utf32; break;
            case 8: save_encoding = pugi::encoding_wchar; break;
            case 9: save_encoding = pugi::encoding_latin1; break;
        }
        doc.save_file("./dummy_file_out", "\t", save_flags, save_encoding);
    }

    std::remove("./dummy_file");
    std::remove("./dummy_file_out");

    return 0;
}