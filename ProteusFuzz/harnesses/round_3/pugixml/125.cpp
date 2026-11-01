// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
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
#include <vector>
#include <limits>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void write_dummy_wchar_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file_wchar", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void write_dummy_save_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_save_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void write_dummy_save_file_wchar(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_save_file_wchar", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

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

extern "C" int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Seed random with data to make fuzzing deterministic if needed, 
    // but typically fuzzers handle seeding. Here we just use data for variability.
    rand(); 

    pugi::xml_document doc;
    pugi::xml_node node;

    // 1. Test load_file
    write_dummy_file(Data, Size);
    unsigned int options = rand();
    pugi::xml_encoding encoding = get_random_encoding();
    pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
    (void)result; // Suppress unused variable warning

    // 2. Test save_file (char* overload)
    write_dummy_save_file(Data, Size);
    pugi::char_t indent_char[] = PUGIXML_TEXT("\t");
    unsigned int flags = rand();
    pugi::xml_encoding enc_save = get_random_encoding();
    bool save_result_char = doc.save_file("./dummy_save_file", indent_char, flags, enc_save);
    (void)save_result_char;

    // 2. Test save_file (wchar_t* overload)
    write_dummy_save_file_wchar(Data, Size);
    // Note: On systems where wchar_t is 2 or 4 bytes, path conversion might be needed for real paths,
    // but for fuzzing with a dummy file name, we cast assuming the string fits or is valid for the platform.
    // For safety in fuzzing, we use a static wide string literal if possible, or cast carefully.
    // Since the path is constant "./dummy_save_file_wchar", we can construct it.
    const wchar_t* wpath = L"./dummy_save_file_wchar";
    bool save_result_wchar = doc.save_file(wpath, indent_char, flags, enc_save);
    (void)save_result_wchar;

    // 3. Test save (std::basic_ostream<wchar_t>& overload)
    {
        std::wstringstream wstream;
        doc.save(wstream, indent_char, flags);
    }

    // 3. Test save (xml_writer& overload)
    {
        FILE* f = fopen("./dummy_writer_file", "wb");
        if (f) {
            pugi::xml_writer_file writer(f);
            doc.save(writer, indent_char, flags, enc_save);
            fclose(f);
        }
    }

    // 4. Test print (xml_writer& overload)
    // We need a valid node. If load succeeded, use doc.first_child(), else use doc (root).
    // If doc is empty, node is null, print should handle it gracefully.
    pugi::xml_node test_node = doc.first_child();
    if (!test_node) {
        test_node = doc;
    }
    
    {
        FILE* f = fopen("./dummy_print_file", "wb");
        if (f) {
            pugi::xml_writer_file writer(f);
            unsigned int depth = rand() % 100;
            test_node.print(writer, indent_char, flags, enc_save, depth);
            fclose(f);
        }
    }

    return 0;
}