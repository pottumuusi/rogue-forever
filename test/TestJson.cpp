#include "gtest/gtest.h"

#include "Json.hpp"

class TestJson : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TestJson, ParseJsonFromFile)
{
    const std::string file_path = "test_data/map2_16x16_redone.tmj";
    cJSON* json = NULL;

    json = Json::readFromFile(file_path);

    EXPECT_NE(json, nullptr);
}

TEST_F(TestJson, ParseJsonFromFileNlohmann)
{
    nlohmann::json json;

    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    json = nullptr;
    json = Json::readFromFileNlohmann(file_path);

    EXPECT_NE(json, nullptr);
}

TEST_F(TestJson, ReadMapTileHeightIsNumber)
{
    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    cJSON* json;
    cJSON* json_tile_height;

    json = NULL;
    json_tile_height = NULL;

    json = Json::readFromFile(file_path);
    json_tile_height = cJSON_GetObjectItemCaseSensitive(json, "tileheight");

    EXPECT_EQ(1, cJSON_IsNumber(json_tile_height));
}

TEST_F(TestJson, ReadMapTileHeight)
{
    const std::string file_path = "test_data/map2_16x16_redone.tmj";
    const uint64_t tile_height_expected = 32;

    cJSON* json;
    cJSON* json_tile_height;
    uint64_t tile_height;

    json = NULL;
    json_tile_height = NULL;
    tile_height = -1;

    json = Json::readFromFile(file_path);
    json_tile_height = cJSON_GetObjectItemCaseSensitive(json, "tileheight");
    tile_height = json_tile_height->valueint;

    EXPECT_EQ(tile_height_expected, tile_height);
}

TEST_F(TestJson, ReadMapTileHeightNlohmann)
{
    nlohmann::json json;
    uint64_t tileheight;

    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    tileheight = 0;
    json = nullptr;

    json = Json::readFromFileNlohmann(file_path);
    tileheight = json["tileheight"];

    EXPECT_EQ(32, tileheight);
}
