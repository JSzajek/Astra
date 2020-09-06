#include <iostream>
#include <vector>

#include "Astra.h"
#include "Synchronous.h"
#include "Player.h"

int main()
{
    using namespace Astra;
    using namespace Astra::Graphics;
    using namespace Astra::Math;

    std::vector<Synchronous*> worldItems;

    Window window("Astra", 960, 540);

    RendererController renderer;
    window.SetWindowResizeCallback([&](float width, float height) { renderer.UpdateScreen(width, height); });

    Player player(&window);
    renderer.SetMainCamera(player.GetCamera());
    renderer.AddEntity(player.GetRendering());
    worldItems.emplace_back(&player);

    Texture texture = Loader::LoadTexture("res/textures/grassTexture.png");
    GuiTexture gui = GuiTexture(texture.id, Vec2(0.75, 0.75), Vec2(0.1, 0.1));
    renderer.AddGui(&gui);

    ImageMaterial* mat1 = new ImageMaterial();//new ImageMaterial("res/textures/red.jpg", 8, 1);
    
    const VertexArray* vertArray2 = ObjLoader::LoadObjectModel("res/pylon.obj");
    Entity entity = Entity(vertArray2, Vec3(0, 1, 0), Vec3(0), Vec3(0.5f));
    renderer.AddEntity(&entity);

    ImageMaterial* grassMat = new ImageMaterial("res/textures/grassTexture.png", 1, 0, true, true);
    ImageMaterial* fernMat = new ImageMaterial("res/textures/fernTexture.png", 1, 0, true, true);

    const VertexArray* grassVertArray = ObjLoader::LoadObjectModel("res/grass.obj");
    const VertexArray* fernVertArray = ObjLoader::LoadObjectModel("res/fern.obj");
    Entity grassModel = Entity(grassVertArray, grassMat, Vec3(0), Vec3(0), Vec3(1));
    Entity fernModel = Entity(fernVertArray, fernMat, Vec3(-5, 0, 1), Vec3(0), Vec3(0.3f));

    renderer.AddEntity(&grassModel);
    renderer.AddEntity(&fernModel);

    TerrainMaterial* grassTerrainMat = new TerrainMaterial("res/textures/grass.jpg");
    TerrainMaterial* flowerTerrainMat = new TerrainMaterial("res/textures/grassFlowers.png");
    TerrainMaterial* mudTerrainMat = new TerrainMaterial("res/textures/mud.png");
    TerrainMaterial* pathTerrainMat = new TerrainMaterial("res/textures/path.png");

    TerrainMaterialPack pack(grassTerrainMat, flowerTerrainMat, mudTerrainMat, pathTerrainMat);
    TerrainMaterial* blendMap = new TerrainMaterial("res/textures/blendMap.png");

    Terrain terrain = Terrain(0, 0, &pack, blendMap);
    Terrain terrain1 = Terrain(-1, 0, &pack, blendMap);
    Terrain terrain2 = Terrain(0, -1, &pack, blendMap);
    Terrain terrain3 = Terrain(-1, -1, &pack, blendMap);
    renderer.AddTerrain(&terrain);
    renderer.AddTerrain(&terrain1);
    renderer.AddTerrain(&terrain2);
    renderer.AddTerrain(&terrain3);

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
    return 0;
}