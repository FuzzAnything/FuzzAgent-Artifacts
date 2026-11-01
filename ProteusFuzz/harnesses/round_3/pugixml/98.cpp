// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
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
#include <functional>
#include <new>

static pugi::xml_document* g_doc = nullptr;

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    if (g_doc == nullptr) {
        g_doc = new pugi::xml_document();
    }

    // Reset document for each input to ensure clean state
    g_doc->reset();

    // The input data from fuzzer is raw bytes and may contain null bytes in the middle.
    // load_string expects a null-terminated string. We must find the first null byte
    // or use the full size if no null byte exists to create a safe string.
    size_t len = Size;
    const char* str_start = reinterpret_cast<const char*>(Data);
    
    // Find the first null terminator to ensure we pass a valid C-string to load_string
    // If no null terminator is found, we treat the whole buffer as the string content,
    // but load_string requires null termination. 
    // Strategy: Copy data to a buffer with explicit null termination.
    // However, to avoid large allocations, we can just search for the first null.
    // If no null is found, we can't safely pass it to load_string directly without copying.
    // Given fuzzing constraints, let's find the first null or use Size if we copy.
    
    // Safe approach: Find first null. If found, use that length. If not, we must copy to a buffer
    // or just skip parsing if the data is too large to copy safely, but for correctness we copy.
    // Actually, a simpler fix for heap-buffer-overflow in load_string is to ensure the string passed
    // is null-terminated. The fuzzer data is not guaranteed to be null-terminated.
    
    // Let's find the first null byte.
    size_t null_pos = 0;
    for (size_t i = 0; i < Size; ++i) {
        if (Data[i] == 0) {
            null_pos = i;
            break;
        }
    }
    
    // If we found a null, we can use the substring up to that point.
    // If not, we have to be careful. load_string will read past the buffer if not null terminated.
    // We will create a temporary string with a null terminator if none exists.
    std::string xml_content;
    
    if (null_pos < Size) {
        xml_content.assign(reinterpret_cast<const char*>(Data), null_pos);
    } else {
        // No null terminator found. We must copy and add one to avoid buffer overflow.
        // To prevent huge allocations, we might limit the size, but for correctness we copy.
        // Fuzzers often generate large inputs. Let's just copy the whole thing and append null.
        xml_content.assign(reinterpret_cast<const char*>(Data), Size);
        xml_content += '\0';
    }

    pugi::xml_parse_result result = g_doc->load_string(xml_content.c_str());
    if (!result) {
        // Invalid XML is a valid test case, but we need a valid node to test attribute functions
        // If parsing fails completely, create a minimal valid document to test API on valid nodes
        std::string fallback = "<root/>";
        g_doc->load_string(fallback.c_str());
    }

    pugi::xml_node root = g_doc->document_element();
    if (!root) {
        // If no root, try to create one
        root = g_doc->append_child("root");
    }

    // 1. Test first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    (void)first_attr; // Suppress unused warning

    // 2. Test find_attribute with a simple predicate
    // Predicate: find attribute with name starting with 'a'
    auto predicate = [](const pugi::xml_attribute& attr) {
        if (!attr) return false;
        const char* name = attr.name();
        if (name == nullptr) return false;
        return name[0] == 'a';
    };
    pugi::xml_attribute found_attr = root.find_attribute(predicate);
    (void)found_attr;

    // 3. Test attribute
    // Try to get an attribute by name from the input data if possible, otherwise use a dummy
    if (xml_content.length() > 0) {
        // Use first byte as a char for attribute name, ensuring it's not null
        char name_char = xml_content[0];
        if (name_char == 0) name_char = 'x';
        // Create a small buffer for the name
        char name_buf[2];
        name_buf[0] = name_char;
        name_buf[1] = '\0';
        pugi::xml_attribute attr_by_name = root.attribute(name_buf);
        (void)attr_by_name;
    } else {
        pugi::xml_attribute attr_by_name = root.attribute("id");
        (void)attr_by_name;
    }

    // 4. Test insert_attribute_after
    // First, ensure there is at least one attribute to use as a reference
    pugi::xml_attribute ref_attr = root.first_attribute();
    if (!ref_attr) {
        // Create a reference attribute if none exists
        ref_attr = root.append_attribute("ref");
        ref_attr.set_value("ref_val");
    }
    
    // Prepare name for new attribute
    std::string new_name = "new_after";
    pugi::xml_attribute new_after = root.insert_attribute_after(new_name.c_str(), ref_attr);
    if (new_after) {
        new_after.set_value("after_val");
    }

    // 5. Test insert_attribute_before
    // Use the same or a different reference
    std::string new_name_before = "new_before";
    pugi::xml_attribute new_before = root.insert_attribute_before(new_name_before.c_str(), ref_attr);
    if (new_before) {
        new_before.set_value("before_val");
    }

    // 6. Test last_attribute
    pugi::xml_attribute last_attr = root.last_attribute();
    (void)last_attr;

    // Cleanup is handled by the document's destructor when g_doc is deleted (at program exit)
    // or reset() called. No explicit cleanup needed here for the document itself.
    
    return 0;
}