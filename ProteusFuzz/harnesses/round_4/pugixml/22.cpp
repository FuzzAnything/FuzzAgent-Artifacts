// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <string>
#include <vector>

static std::string generate_xml(const uint8_t* data, size_t size) {
    std::string result = "<root>";
    // Convert raw input to safe XML content to avoid parsing errors
    for (size_t i = 0; i < size; ++i) {
        unsigned char c = data[i];
        if (c == '<' || c == '>' || c == '&' || c == '"' || c == '\'') {
            result += "\\x";
            char hex[4];
            snprintf(hex, sizeof(hex), "%02X", c);
            result += hex;
        } else if (c >= 32 && c < 127) {
            result += static_cast<char>(c);
        } else {
            result += "\\x";
            char hex[4];
            snprintf(hex, sizeof(hex), "%02X", c);
            result += hex;
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    std::string xml_content = generate_xml(Data, Size);
    
    // Parse the generated XML
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
    if (!result) {
        // If parsing fails, try a minimal fallback
        doc.load_string("<root><item>test</item></root>");
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // Ensure we have some children to work with for the fuzzing sequence
    if (root.first_child() == pugi::xml_node()) {
        root.append_child("item").text().set("fuzz");
    }

    // Execute the target API sequence
    // 1. pugi::xml_node::child
    pugi::xml_node n1 = root.child("item");
    if (!n1) n1 = root.first_child(); // Fallback if "item" not found

    // 2. pugi::xml_node::child
    pugi::xml_node n2 = n1.child("sub");
    if (!n2) n2 = n1.first_child();

    // 3. pugi::xml_node::insert_copy_before
    // We need a proto and a target. Use n1 as proto, n2 (or n1) as target
    pugi::xml_node target3 = n2 ? n2 : n1;
    pugi::xml_node n3 = n1.insert_copy_before(n1, target3);

    // 4. pugi::xml_node::child
    pugi::xml_node n4 = n3.child("data");
    if (!n4) n4 = n3.first_child();

    // 5. pugi::xml_node::insert_copy_before
    pugi::xml_node target5 = n4 ? n4 : n3;
    pugi::xml_node n5 = n3.insert_copy_before(n3, target5);

    // 6. pugi::xml_node::child
    pugi::xml_node n6 = n5.child("info");
    if (!n6) n6 = n5.first_child();

    // 7. pugi::xml_node::child
    pugi::xml_node n7 = n6.child("detail");
    if (!n7) n7 = n6.first_child();

    // 8. pugi::xml_node::child
    pugi::xml_node n8 = n7.child("extra");
    if (!n8) n8 = n7.first_child();

    // 9. pugi::xml_node::child
    pugi::xml_node n9 = n8.child("more");
    if (!n9) n9 = n8.first_child();

    // 10. pugi::xml_node::insert_copy_before
    pugi::xml_node target10 = n9 ? n9 : n8;
    pugi::xml_node n10 = n8.insert_copy_before(n8, target10);

    // 11. pugi::xml_node::child
    pugi::xml_node n11 = n10.child("test");
    if (!n11) n11 = n10.first_child();

    // 12. pugi::xml_node::first_child
    pugi::xml_node n12 = n11.first_child();

    // 13. pugi::xml_node::name
    const pugi::char_t* name13 = n12.name();
    (void)name13; // Suppress unused warning

    // 14. pugi::xml_node::child
    pugi::xml_node n14 = n12.child("node");
    if (!n14) n14 = n12.first_child();

    // 15. pugi::xml_node::insert_copy_before
    pugi::xml_node target15 = n14 ? n14 : n12;
    pugi::xml_node n15 = n12.insert_copy_before(n12, target15);

    // 16. pugi::xml_node::child
    pugi::xml_node n16 = n15.child("child");
    if (!n16) n16 = n15.first_child();

    // 17. pugi::xml_node::first_child
    pugi::xml_node n17 = n16.first_child();

    // 18. pugi::xml_node::child
    pugi::xml_node n18 = n17.child("val");
    if (!n18) n18 = n17.first_child();

    // 19. pugi::xml_node::last_child
    pugi::xml_node n19 = n18.last_child();

    // 20. pugi::xml_node::name
    const pugi::char_t* name20 = n19.name();
    (void)name20;

    // 21. pugi::xml_node::child
    pugi::xml_node n21 = n19.child("last");
    if (!n21) n21 = n19.first_child();

    // 22. pugi::xml_node::insert_copy_before
    pugi::xml_node target22 = n21 ? n21 : n19;
    pugi::xml_node n22 = n19.insert_copy_before(n19, target22);

    // 23. pugi::xml_node::child
    pugi::xml_node n23 = n22.child("copy");
    if (!n23) n23 = n22.first_child();

    // 24. pugi::xml_node::first_child
    pugi::xml_node n24 = n23.first_child();

    // 25. pugi::xml_node::next_sibling
    pugi::xml_node n25 = n24.next_sibling();

    // 26. pugi::xml_node::child
    pugi::xml_node n26 = n25.child("sib");
    if (!n26) n26 = n25.first_child();

    // 27. pugi::xml_node::first_child
    pugi::xml_node n27 = n26.first_child();

    // 28. pugi::xml_node::value
    const pugi::char_t* value28 = n27.value();
    (void)value28;

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}