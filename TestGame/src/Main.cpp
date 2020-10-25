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

    // TODO: Store Fonts in a directory and handle deletion before game closure or when no references

    Texture fontTexture = Loader::LoadAtlasTexture("res/fonts/candara.png");
    FontType* font = new FontType(fontTexture.id, "res/fonts/candara.fnt");
    GuiText* text = new GuiText("This is a test text!", font, 3, Vec2(0, 0), 1, true);
    GuiText* outlineText = new GuiText("Outlines", font, 3, Vec2(0), Vec3(0), 0, 1, Vec3(0, 0, 1));

    Texture texture = Loader::LoadTexture("res/textures/grassTexture.png");
    GuiTexture gui = GuiTexture(texture.id, Vec2(0.75, 0.75), Vec2(0.1, 0.1));
    renderer.AddGui(&gui);

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

    ImageMaterial* fernMat = new ImageMaterial("res/textures/fernAtlas.png", 2, 1, 0, true, true);
    const VertexArray* fernVertArray = ObjLoader::LoadObjectModel("res/fern.obj");

    ImageMaterial* barrelMat2 = new ImageMaterial("res/textures/barrel.png", 1, 10, 0.5f);
    Entity barrelModel2 = Entity("res/barrel.obj", "res/textures/barrelNormal.png", barrelMat2, Vec3(-70, 70, 80), Vec3(0), Vec3(1));

    renderer.AddEntity(&barrelModel2);

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
    
    ParticleMaterial* partMaterial = new ParticleMaterial("res/textures/particleAtlas.png", 4);

    //ParticleSystem partSystem(partMaterial, 15, 5, -0.1f, 3);
    ConeParticleSystem partSystem(partMaterial, 15, 25, 0.5f, 3, 3);
    partSystem.SetDirection(Vec3(0, 1, 0), 0.1f);
    partSystem.SetLifeError(0.1f);
    partSystem.SetSpeedError(0.4f);
    partSystem.SetScaleError(0.8f);
    partSystem.SetRandomRotation(true);

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
        partSystem.GenerateParticles(player.GetRendering()->Translation());
        window.Update();

        frames++;
        if (time.Elapsed() - timer > 1.0f)
        {
            timer += 1.0f;
            printf("%dfps\n", frames);
            frames = 0;
        }
    }
    return 0;
}