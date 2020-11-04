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

    window.SetWindowResizeCallback([&](float width, float height) { RendererController::UpdateScreen(width, height); });

    Scene* const mainScene = new Scene(NULL);

    mainScene->Start();

    TerrainMaterial* grassTerrainMat = new TerrainMaterial("res/textures/grass.jpg");
    TerrainMaterial* flowerTerrainMat = new TerrainMaterial("res/textures/grassFlowers.png");
    TerrainMaterial* mudTerrainMat = new TerrainMaterial("res/textures/mud.png");
    TerrainMaterial* pathTerrainMat = new TerrainMaterial("res/textures/path.png");

    TerrainMaterialPack pack(grassTerrainMat, flowerTerrainMat, mudTerrainMat, pathTerrainMat);
    TerrainMaterial* blendMap = new TerrainMaterial("res/textures/blendMap.png");

    //Terrain terrain = Terrain(0, 0, "res/textures/meteorcrater_heightmap.png", &pack, blendMap);
    Terrain terrain = Terrain(0, 0, 40, 4, 0.01f, 4862, &pack, blendMap);
    terrain(TRANSLATION, SUB_EQ, X, 128);
    terrain(TRANSLATION, SUB_EQ, Z, 128);
    
    mainScene->AddTerrain(&terrain);

    Player player(Vec3(-100,50,100), &window, &terrain);
    
    mainScene->SetMainCamera(player.GetCamera());
    mainScene->AddEntity(player.GetRendering());
    worldItems.emplace_back(&player);

    WaterTile tile1 = WaterTile(0, 0, -2.5f, 128);

    mainScene->AddWaterTile(&tile1);

    // TODO: Store Fonts in a directory and handle deletion before game closure or when no references

    Texture fontTexture = Loader::LoadAtlasTexture("res/fonts/candara.png");
    FontType* font = new FontType(fontTexture.id, "res/fonts/candara.fnt");
    GuiText* text = new GuiText("This is a test text!", font, 3, Vec2(0, 0), 1, true);
    GuiText* outlineText = new GuiText("Outlines", font, 3, Vec2(0), Vec3(0), 0, 1, Vec3(0, 0, 1));

    mainScene->AddText(text);
    mainScene->AddText(outlineText);

    Texture texture = Loader::LoadTexture("res/textures/grassTexture.png");
    GuiTexture gui = GuiTexture(texture.id, Vec2(0.75, 0.75), Vec2(0.1, 0.1));
    mainScene->AddGui(&gui);

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
    mainScene->SetSkyBox(&skybox);
    
    //Light* light1 = new Light(Math::Vec3(20000, 20000, 20000), Math::Vec3(0.75f));  // Sun 
    //Light* light2 = new Light(Math::Vec3(-20, 50, 20), Math::Vec3(0, 1, 1), Math::Vec3(1, 0.01f, 0.002f));
    //Light* light3 = new Light(Math::Vec3(20, 50, -20), Math::Vec3(1, 0, 0), Math::Vec3(1, 0.01f, 0.002f));

    Vec3 light_pos = Math::Vec3(-55, terrain.GetHeightOfTerrain(-55, 55) + 7, 55);
    const VertexArray* cubeVertArray = ObjLoader::LoadObjectModel("res/cube.obj");
    Entity light_indicator = Entity(cubeVertArray, light_pos, Vec3(0), Vec3(0.5f));
    DirectionalLight* dir_light = new DirectionalLight(Vec3(0), Vec3(-0.2f, -1.0f, -0.3f), Vec3(0.2f), Vec3(0.3f), Vec3(0.6f));
    //Light* light4 = new DirectionalLight(light_pos, Vec3(-0.2f, -1.0f, -0.3f), Vec3(0.4f), Vec3(0.5f), Vec3(1.0f));
    PointLight* light4 = new PointLight(light_pos, Vec3(1), Vec3(1), Vec3(1.0f));

    //light_pos.y += 18;
    //Light* light4 = new SpotLight(light_pos, Vec3(-0.2f, -1.0f, -0.3f), Vec3(0.4f), Vec3(0.5f), Vec3(0.7f), cosf(Math::ToRadians(12.5)), cosf(Math::ToRadians(17.5)));

    //renderer.AddDirectionalLight(light1);
    //renderer.AddLight(light2);
    //renderer.AddLight(light3);
    //renderer.AddEntity(&light_indicator);
    //renderer.AddLight(light4);
    //mainScene->AddEntity(&light_indicator);
    mainScene->AddPointLight(light4);
    mainScene->SetDirectionalLight(dir_light);

    ImageMaterial* fernMat = new ImageMaterial("res/textures/fernAtlas.png", 2, 0.25f, true, true);
    const VertexArray* fernVertArray = ObjLoader::LoadObjectModel("res/fern.obj");

    ImageMaterial* barrelMat2 = new ImageMaterial("res/textures/barrel.png", "res/textures/barrelSpecular.png", 1, 32);
    Entity barrelModel2 = Entity("res/barrel.obj", "res/textures/barrelNormal.png", barrelMat2, Vec3(-35, terrain.GetHeightOfTerrain(-35, 45) + 5, 45), Vec3(0), Vec3(1));
    mainScene->AddEntity(&barrelModel2);

    ImageMaterial* brickMat = new ImageMaterial("res/textures/bricks.jpg", "res/textures/bricks_specular.jpg", 1, 16);
    Entity brick = Entity("res/plane.obj", "res/textures/bricks_normal.jpg", "res/textures/bricks_heightmap.jpg", 0.1f, brickMat, Vec3(-50, terrain.GetHeightOfTerrain(-50, 50) + 5, 50), Vec3(90, 0, 0), Vec3(5, 1, 5));
    mainScene->AddEntity(&brick);

    std::vector<const Entity*> entities;
    for (int i = 0; i < 20; i++)
    {
        float x = (rand() % 256) - 128;
        float z = (rand() % 256) - 128;
        float y = terrain.GetHeightOfTerrain(x, z);
        Entity *entity = new Entity(fernVertArray, fernMat, rand() % 4, Vec3(x, y, z), Vec3(0), Vec3(1));
        entities.emplace_back(entity);
        mainScene->AddEntity(entity);
    }
    
    ParticleMaterial* partMaterial = new ParticleMaterial("res/textures/particleAtlas.png", 4);

    Vec3 particleCenter(-80, terrain.GetHeightOfTerrain(-80, 80) + 5, 80);
    ParticleSystem partSystem(partMaterial, &particleCenter, 15, 5, -0.1f, 3);
    mainScene->AddParticleSystem(&partSystem);
    
    //ConeParticleSystem partSystem(partMaterial, 15, 25, 0.5f, 1.5f, 3);
    //partSystem.SetDirection(Vec3(0, 1, 0), 0.1f);
    //partSystem.SetLifeError(0.1f);
    //partSystem.SetSpeedError(0.4f);
    //partSystem.SetScaleError(0.8f);
    //partSystem.SetRandomRotation(true);

    mainScene->End();

    RendererController::SetCurrentScene(mainScene);

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

        barrelModel2(ROTATION, SUM_EQ, Y, 0.5f);

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

        //renderer.Render();
        RendererController::Render();
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