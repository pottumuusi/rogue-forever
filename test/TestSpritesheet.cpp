#include "gtest/gtest.h"

#include "GraphicsUtil.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"

class TestSpritesheet : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TestSpritesheet, SpritesheetGetsFirstgid)
{
    spritesheet_pool spritesheet_pool;
    Map testMap = Map("test_data/map2_16x16_redone.tmj");

    GraphicsUtil::load_spritesheets_map(spritesheet_pool, testMap);

    Spritesheet& testSpritesheet = spritesheet_pool[0];

    for (Spritesheet& s : spritesheet_pool) {
        if (0 == s.get_name().compare("dc-misc-collection-spritesheet")) {
            testSpritesheet = s;
            break;
        }
    }

    EXPECT_EQ(testSpritesheet.get_tiled_firstgid(), 560);
}

TEST_F(TestSpritesheet, MonSpritesheetContainsOneLayer)
{
    cJSON* json_tmj;
    cJSON* json_layers_array;

    spritesheet_pool spritesheet_pool;

    Map test_map = Map("test_data/map2_16x16_redone.tmj");

    json_tmj = NULL;
    json_layers_array = NULL;

    GraphicsUtil::load_spritesheets_map(spritesheet_pool, test_map);

    Spritesheet& test_spritesheet = spritesheet_pool[0];

    for (Spritesheet& s : spritesheet_pool) {
        if (0 == s.get_name().compare("dc-mon-collection-spritesheet")) {
            test_spritesheet = s;
            break;
        }
    }

    json_tmj = test_spritesheet.get_json();

    json_layers_array = cJSON_GetObjectItemCaseSensitive(json_tmj, "layers");

    EXPECT_EQ(cJSON_GetArraySize(json_layers_array), 1);
}

TEST_F(TestSpritesheet, MonSpritesheetDataIndex82Gives83)
{
    cJSON* json_tmj;
    cJSON* json_layer;
    cJSON* json_layers_array;
    cJSON* json_data_array;
    cJSON* json_data_item;

    spritesheet_pool spritesheet_pool;

    Map test_map = Map("test_data/map2_16x16_redone.tmj");

    json_tmj = NULL;
    json_layer = NULL;
    json_layers_array = NULL;
    json_data_array = NULL;
    json_data_item = NULL;

    GraphicsUtil::load_spritesheets_map(spritesheet_pool, test_map);

    Spritesheet& test_spritesheet = spritesheet_pool[0];

    for (Spritesheet& s : spritesheet_pool) {
        if (0 == s.get_name().compare("dc-mon-collection-spritesheet")) {
            test_spritesheet = s;
            break;
        }
    }

    json_tmj = test_spritesheet.get_json();
    json_layers_array = cJSON_GetObjectItemCaseSensitive(json_tmj, "layers");
    json_layer = cJSON_GetArrayItem(json_layers_array, 0);
    json_data_array = cJSON_GetObjectItemCaseSensitive(json_layer, "data");
    json_data_item = cJSON_GetArrayItem(json_data_array, 82);

    EXPECT_EQ(json_data_item->valueint, 83);
}
