#include <iostream>
#include <vector>

#include "Astra.h"

int main()
{
    using namespace Astra;
    using namespace Astra::Graphics;
    using namespace Astra::Math;

    Window window("Astra", 960, 540);

    RendererController renderer;
    window.SetWindowResizeCallback([&](float width, float height) { renderer.UpdateScreen(width, height); });

    Texture* texture = Loader::LoadTexture("res/textures/grassTexture.png");
    GuiTexture gui = GuiTexture(texture->id, Vec2(0.75, 0.75), Vec2(0.1, 0.1));
    renderer.AddGui(&gui);

    Texture* texture2 = Loader::LoadTexture("res/textures/red.jpg");
    Texture* texture3 = Loader::LoadTexture("res/textures/purple.jpg");
    texture2->reflectivity = 1;
    texture2->shineDampener = 8;

    texture3->reflectivity = 1;
    texture3->shineDampener = 8;
    const VertexArray* vertArray2 = ObjLoader::LoadObjectModel("res/dragon.obj");
    Entity entity = Entity(vertArray2, texture2, Vec3(0, 1, 0), Vec3(0), Vec3(0.5f));
    renderer.AddEntity(&entity);

    Texture* grassModelTexture = Loader::LoadTexture("res/textures/grassTexture.png");
    grassModelTexture->fakeLight = true;
    grassModelTexture->transparent = true;
    Texture* fernModelTexture = Loader::LoadTexture("res/textures/fernTexture.png");
    fernModelTexture->transparent = true;
    grassModelTexture->fakeLight = true;

    const VertexArray* grassVertArray = ObjLoader::LoadObjectModel("res/grass.obj");
    const VertexArray* fernVertArray = ObjLoader::LoadObjectModel("res/fern.obj");
    Entity grassModel = Entity(grassVertArray, grassModelTexture, Vec3(0), Vec3(0), Vec3(1));
    Entity fernModel = Entity(fernVertArray, fernModelTexture, Vec3(-5, 0, 1), Vec3(0), Vec3(0.3f));

    renderer.AddEntity(&grassModel);
    renderer.AddEntity(&fernModel);

    Texture* grassTexture = Loader::LoadTexture("res/textures/grass.jpg");

    Terrain terrain = Terrain(0, 0, grassTexture);
    Terrain terrain1 = Terrain(-1, 0, grassTexture);
    Terrain terrain2 = Terrain(0, -1, grassTexture);
    Terrain terrain3 = Terrain(-1, -1, grassTexture);
    renderer.AddTerrain(&terrain);
    renderer.AddTerrain(&terrain1);
    renderer.AddTerrain(&terrain2);
    renderer.AddTerrain(&terrain3);

    Timer time;
    float timer = 0;
    unsigned int frames = 0;

    while (!window.Closed())
    {
        entity.GetRotation().y += 0.0001f;
        window.Clear();
        renderer.Render();
        window.Update();

        if (window.isKeyPressed(87))
        {
            renderer.UpdatePitch(0.2f);
        }
        if (window.isKeyPressed(83))
        {
            renderer.UpdatePitch(-0.2f);
        }

        if (window.isKeyPressed(65))
        {
            renderer.UpdateYaw(-0.2f);
        }
        if (window.isKeyPressed(68))
        {
            renderer.UpdateYaw(0.2f);
        }

        if (window.isKeyPressed(90))
        {
            renderer.UpdateDistance(-0.01f);
        }
        if (window.isKeyPressed(88))
        {
            renderer.UpdateDistance(0.01f);
        }

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