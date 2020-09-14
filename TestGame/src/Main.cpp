#include <iostream>
#include <vector>

#include <ctime>

#include "Astra.h"
#include "Synchronous.h"
#include "Player.h"

int main()
{
    using namespace Astra;
    using namespace Astra::Graphics;
    using namespace Astra::Math;

    srand((unsigned)time(0));

    std::vector<Synchronous*> worldItems;

    Window window("Astra", 960, 540);

    RendererController renderer;
    window.SetWindowResizeCallback([&](float width, float height) { renderer.UpdateScreen(width, height); });

    TerrainMaterial* grassTerrainMat = new TerrainMaterial("res/textures/grass.jpg");
    TerrainMaterial* flowerTerrainMat = new TerrainMaterial("res/textures/grassFlowers.png");
    TerrainMaterial* mudTerrainMat = new TerrainMaterial("res/textures/mud.png");
    TerrainMaterial* pathTerrainMat = new TerrainMaterial("res/textures/path.png");

    TerrainMaterialPack pack(grassTerrainMat, flowerTerrainMat, mudTerrainMat, pathTerrainMat);
    TerrainMaterial* blendMap = new TerrainMaterial("res/textures/blendMap.png");

    Terrain terrain = Terrain(0, 0, &pack, blendMap);
    renderer.AddTerrain(&terrain);
    terrain.Translation().x -= 128;
    terrain.Translation().z -= 128;

    WaterTile tile1 = WaterTile(0, 0, 45, 100);
    renderer.AddWaterTile(tile1);

    Player player(Vec3(-100,50,100),&window, &terrain);
    renderer.SetMainCamera(player.GetCamera());
    renderer.AddEntity(player.GetRendering());
    worldItems.emplace_back(&player);

    //Texture texture = Loader::LoadTexture("res/textures/grassTexture.png");
    //GuiTexture gui = GuiTexture(texture.id, Vec2(0.75, 0.75), Vec2(0.1, 0.1));
    //renderer.AddGui(&gui);

    std::vector<const char*> m_textureFiles =
    {
        "res/textures/Default_Skybox/right.png",
        "res/textures/Default_Skybox/left.png",
        "res/textures/Default_Skybox/top.png",
        "res/textures/Default_Skybox/bottom.png",
        "res/textures/Default_Skybox/back.png",
        "res/textures/Default_Skybox/front.png",
    };

    std::vector<const char*> m_nightTextureFiles =
    {
        "res/textures/Default_Night_Skybox/right.png",
        "res/textures/Default_Night_Skybox/left.png",
        "res/textures/Default_Night_Skybox/top.png",
        "res/textures/Default_Night_Skybox/bottom.png",
        "res/textures/Default_Night_Skybox/back.png",
        "res/textures/Default_Night_Skybox/front.png",
    };

    SkyboxMaterial skybox(m_textureFiles, m_nightTextureFiles);
    renderer.SetSkyBox(&skybox);

    Light* light1 = new Light(Math::Vec3(20000, 20000, 2000), Math::Vec3(0.4f));  // Sun 
    Light* light2 = new Light(Math::Vec3(-20, 50, 20), Math::Vec3(0, 1, 1), Math::Vec3(1, 0.01f, 0.002f));
    Light* light3 = new Light(Math::Vec3(20, 50, -20), Math::Vec3(1, 0, 0), Math::Vec3(1, 0.01f, 0.002f));
    Light* light4 =  new Light(Math::Vec3(-20, 50, -20), Math::Vec3(1, 0, 1), Math::Vec3(1, 0.01f, 0.002f));

    renderer.AddLight(light1);
    renderer.AddLight(light2);
    renderer.AddLight(light3);
    renderer.AddLight(light4);

    ImageMaterial* grassMat = new ImageMaterial("res/textures/grassTexture.png", 1, 1, 0, true, true);
    ImageMaterial* fernMat = new ImageMaterial("res/textures/fernAtlas.png", 2, 1, 0, true, true);

    const VertexArray* grassVertArray = ObjLoader::LoadObjectModel("res/grass.obj");
    const VertexArray* fernVertArray = ObjLoader::LoadObjectModel("res/fern.obj");
    Entity grassModel = Entity(grassVertArray, grassMat, Vec3(0, terrain.GetHeightOfTerrain(0,0), 0), Vec3(0), Vec3(1));

    renderer.AddEntity(&grassModel);

    std::vector<const Entity*> entities;
    for (int i = 0; i < 20; i++)
    {
        float x = (rand() % 256) - 128;
        float z = (rand() % 256) - 128;
        float y = terrain.GetHeightOfTerrain(x, z);
        Entity *entity = new Entity(fernVertArray, fernMat, rand() % 4, Vec3(x, y, z), Vec3(0), Vec3(1));
        entities.emplace_back(entity);
        renderer.AddEntity(entity);
    }

    const float InGameTimeSpeed = 0.00005f;
    short timeDir = 1;

    Timer time;
    float timer = 0;
    unsigned int frames = 0;

    while (!window.Closed())
    {
        window.Clear();
        
        for (Synchronous* item : worldItems)
        {
            item->Update();
        }

        skybox.BlendFactor() += InGameTimeSpeed * timeDir;
        if (skybox.BlendFactor() >= 1)
        {
            skybox.BlendFactor() = 1;
            timeDir = -1;
        }
        else if (skybox.BlendFactor() <= 0)
        {
            skybox.BlendFactor() = 0;
            timeDir = 1;
        }

        renderer.Render();
        window.Update();

        frames++;
        if (time.Elapsed() - timer > 1.0f)
        {
            timer += 1.0f;
            printf("%dfps\n", frames);
            frames = 0;
        }
    }

    /*for (const Entity* entity : entities)
    {
        delete entity;
    }*/

    return 0;
}