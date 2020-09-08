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
    terrain.Translation().x -= 200;
    terrain.Translation().z -= 200;

    Player player(&window, &terrain);
    renderer.SetMainCamera(player.GetCamera());
    renderer.AddEntity(player.GetRendering());
    worldItems.emplace_back(&player);

    Texture texture = Loader::LoadTexture("res/textures/grassTexture.png");
    GuiTexture gui = GuiTexture(texture.id, Vec2(0.75, 0.75), Vec2(0.1, 0.1));
    renderer.AddGui(&gui);


    Light light1(Light(Math::Vec3(100, 100, 100), Math::Vec3(1, 0, 1)));
    Light light2(Light(Math::Vec3(-100, 100, 100), Math::Vec3(0, 1, 1)));
    Light light3(Light(Math::Vec3(100, 100, -100), Math::Vec3(1, 1, 0)));
    Light light4(Light(Math::Vec3(-100, 100, -100), Math::Vec3(0, 0, 1)));

    renderer.AddLight(&light1);
    renderer.AddLight(&light2);
    renderer.AddLight(&light3);
    renderer.AddLight(&light4);

    ImageMaterial* mat1 = new ImageMaterial();
    
    const VertexArray* vertArray2 = ObjLoader::LoadObjectModel("res/pylon.obj");
    Entity entity = Entity(vertArray2, Vec3(0, 1, 0), Vec3(0), Vec3(0.5f));
    renderer.AddEntity(&entity);

    ImageMaterial* grassMat = new ImageMaterial("res/textures/grassTexture.png", 1, 1, 0, true, true);
    ImageMaterial* fernMat = new ImageMaterial("res/textures/fernAtlas.png", 2, 1, 0, true, true);

    const VertexArray* grassVertArray = ObjLoader::LoadObjectModel("res/grass.obj");
    const VertexArray* fernVertArray = ObjLoader::LoadObjectModel("res/fern.obj");
    Entity grassModel = Entity(grassVertArray, grassMat, Vec3(0, terrain.GetHeightOfTerrain(0,0), 0), Vec3(0), Vec3(1));

    renderer.AddEntity(&grassModel);

    std::vector<const Entity*> entities;
    for (int i = 0; i < 20; i++)
    {
        float x = (rand() % 400) - 200;
        float z = (rand() % 400) - 200;
        float y = terrain.GetHeightOfTerrain(x, z);
        Entity *entity = new Entity(fernVertArray, fernMat, rand() % 4, Vec3(x, y, z), Vec3(0), Vec3(1));
        entities.emplace_back(entity);
        renderer.AddEntity(entity);
    }

    Timer time;
    float timer = 0;
    unsigned int frames = 0;

    while (!window.Closed())
    {
        entity.Rotation().y += 0.0001f;
        window.Clear();
        
        for (Synchronous* item : worldItems)
        {
            item->Update();
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