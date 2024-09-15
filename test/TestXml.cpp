#include "gtest/gtest.h"

#include <pugixml/pugixml.hpp>

class TestXml : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TestXml, ParseXmlFromFile)
{
    pugi::xml_document document;

    pugi::xml_parse_result result = document.load_file("test_data/dc-dngn-collection-spritesheet.tsx");

    EXPECT_TRUE(result);
}