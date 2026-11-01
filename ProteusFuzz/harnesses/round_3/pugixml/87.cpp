// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>

static pugi::xml_document* g_doc = nullptr;

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize document if not already done
    if (!g_doc) {
        g_doc = new pugi::xml_document();
    }

    pugi::xml_document& doc = *g_doc;
    doc.reset();

    // Fuzz input might contain null bytes, so we cannot directly pass it to load_string
    // which expects a null-terminated string. We need to handle this carefully.
    
    // Create a safe buffer with a null terminator
    std::vector<char> safe_buffer(Size + 1);
    std::memcpy(safe_buffer.data(), Data, Size);
    safe_buffer[Size] = '\0';

    // Parse the input as XML
    pugi::xml_parse_result result = doc.load_string(safe_buffer.data());
    
    // Even if parsing fails, we can still test with default constructed xml_text
    pugi::xml_text text; // Default constructed, empty
    
    // Test 1: empty() on empty text
    bool is_empty = text.empty();
    (void)is_empty;

    // Test 2: as_bool on empty text with default
    bool bool_val = text.as_bool();
    bool bool_val_def = text.as_bool(true);
    (void)bool_val;
    (void)bool_val_def;

    // Test 3: set(bool) on empty text (should return false)
    bool set_bool_res = text.set(true);
    bool set_bool_res2 = text.set(false);
    (void)set_bool_res;
    (void)set_bool_res2;

    // Test 4: set(double) on empty text (should return false)
    bool set_double_res = text.set(3.14);
    bool set_double_res2 = text.set(-0.0);
    (void)set_double_res;
    (void)set_double_res2;

    // Test 5: as_ullong on empty text
    unsigned long long ull_val = text.as_ullong();
    unsigned long long ull_val_def = text.as_ullong(12345);
    (void)ull_val;
    (void)ull_val_def;

    // Test 6: as_double on empty text
    double double_val = text.as_double();
    double double_val_def = text.as_double(99.99);
    (void)double_val;
    (void)double_val_def;

    // If parsing succeeded, try to get a text node and test on it
    if (result.status == pugi::status_ok) {
        pugi::xml_node root = doc.first_child();
        if (root) {
            // Try to get text from the root or any child
            pugi::xml_text node_text = root.text();
            
            // Test empty()
            bool node_empty = node_text.empty();
            (void)node_empty;

            // Test as_bool
            bool node_bool = node_text.as_bool();
            bool node_bool_def = node_text.as_bool(true);
            (void)node_bool;
            (void)node_bool_def;

            // Test set(bool)
            bool node_set_bool = node_text.set(true);
            bool node_set_bool2 = node_text.set(false);
            (void)node_set_bool;
            (void)node_set_bool2;

            // Test set(double)
            bool node_set_double = node_text.set(1.5);
            bool node_set_double2 = node_text.set(0.0);
            (void)node_set_double;
            (void)node_set_double2;

            // Test as_ullong
            unsigned long long node_ull = node_text.as_ullong();
            unsigned long long node_ull_def = node_text.as_ullong(42);
            (void)node_ull;
            (void)node_ull_def;

            // Test as_double
            double node_double = node_text.as_double();
            double node_double_def = node_text.as_double(1.0);
            (void)node_double;
            (void)node_double_def;
        }
    }

    return 0;
}