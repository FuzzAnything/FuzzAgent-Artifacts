// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
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
#include <random>

static std::mt19937 g_rng(0);

static unsigned int random_options() {
    return static_cast<unsigned int>(g_rng());
}

static pugi::xml_encoding random_encoding() {
    // enum values are 0 to 9 (encoding_latin1)
    return static_cast<pugi::xml_encoding>(g_rng() % 10);
}

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare random parameters
    unsigned int options = random_options();
    pugi::xml_encoding encoding = random_encoding();

    // 1. Test load_buffer_inplace_own
    // Allocate memory using malloc (pugixml uses malloc by default unless configured otherwise)
    void* buffer = std::malloc(Size);
    if (buffer) {
        std::memcpy(buffer, Data, Size);
        pugi::xml_document doc1;
        doc1.load_buffer_inplace_own(buffer, Size, options, encoding);
        // Document owns buffer, do not free manually
    }

    // 2. Test load_buffer (copies data)
    pugi::xml_document doc2;
    doc2.load_buffer(Data, Size, options, encoding);

    // 3. Test load_string (ensure null termination)
    pugi::xml_document doc3;
    // Create a temporary buffer with null terminator using pugi::char_t
    std::vector<pugi::char_t> str_buf(Size + 1);
    if (Size > 0) {
        // Copy data. Note: If PUGIXML_WCHAR_MODE is defined, char_t is wchar_t.
        // Casting raw bytes to wchar_t might produce invalid data, but the function should handle it or return an error.
        std::memcpy(str_buf.data(), Data, Size);
        str_buf[Size] = 0;
        doc3.load_string(str_buf.data(), options);
    }

    // 4. Test load_file
    write_dummy_file(Data, Size);
    pugi::xml_document doc4;
    doc4.load_file("./dummy_file", options, encoding);

    // 5. Test load (stream)
    // FIX: Use std::string constructor that takes (const char*, size_t) to avoid strlen on non-null-terminated data
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    std::istringstream stream(input_str, std::ios::binary);
    pugi::xml_document doc5;
    doc5.load(stream, options, encoding);

    // 6. Test save
    // We need a valid document to save. Use doc2 which loaded from buffer.
    // Create a dummy output file for xml_writer_file
    FILE* out_f = fopen("./dummy_out", "wb");
    if (out_f) {
        pugi::xml_writer_file writer(out_f);
        // Save with various flags
        doc2.save(writer, "\t", random_options(), random_encoding());
        fclose(out_f);
    }

    // Test save to stream
    std::ostringstream out_stream;
    pugi::xml_writer_stream writer_stream(out_stream);
    doc2.save(writer_stream, "\t", random_options(), random_encoding());

    // Cleanup: dummy files
    std::remove("./dummy_file");
    std::remove("./dummy_out");

    return 0;
}