#include <iostream>
#include <vector>

#include "Astra.h"

int main()
{
    using namespace Astra;
    using namespace Astra::Graphics;
    using namespace Astra::Math;

    Window window("Astra", 960, 540);
    window.SetClearColor(Vec3(0.6));

    RendererController renderer;
    window.SetWindowResizeCallback([&](float width, float height) { renderer.UpdateScreen(width, height); });

    Texture* texture = Loader::LoadTexture("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/textures/test1.jpg");
    GuiTexture gui = GuiTexture(texture->id, Vec2(0.75, 0.75), Vec2(0.1, 0.1));
    renderer.AddGui(&gui);

    Texture* texture2 = Loader::LoadTexture("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/textures/red.jpg");
    Texture* texture3 = Loader::LoadTexture("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/textures/purple.jpg");
    texture2->reflectivity = 1;
    texture2->shineDampener = 8;

    texture3->reflectivity = 1;
    texture3->shineDampener = 8;
    //const VertexArray* vertArray = ObjLoader::LoadObjectModel("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/cube.obj");
    const VertexArray* vertArray2 = ObjLoader::LoadObjectModel("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/cube.obj");
    //Entity entity = Entity(vertArray, texture2, Vec3(0, 0, 0), Vec3(0), Vec3(0.5f));
    Entity entity = Entity(vertArray2, texture2, Vec3(0, 0, 0), Vec3(0), Vec3(0.5f));
    Entity entity2 = Entity(vertArray2, texture3, Vec3(0, 2, 0), Vec3(0), Vec3(0.5f));
    renderer.AddEntity(&entity);
    renderer.AddEntity(&entity2);

    Texture* grassModelTexture = Loader::LoadTexture("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/textures/grassTexture.png");
    grassModelTexture->transparent = true;
    Texture* fernModelTexture = Loader::LoadTexture("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/textures/fernTexture.png");
    fernModelTexture->transparent = true;

    const VertexArray* grassVertArray = ObjLoader::LoadObjectModel("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/grass.obj");
    const VertexArray* fernVertArray = ObjLoader::LoadObjectModel("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/fern.obj");
    Entity grassModel = Entity(grassVertArray, grassModelTexture, Vec3(0), Vec3(0), Vec3(1));
    Entity fernModel = Entity(fernVertArray, fernModelTexture, Vec3(-5, 0, 1), Vec3(0), Vec3(0.3f));

    renderer.AddEntity(&grassModel);
    renderer.AddEntity(&fernModel);

    Texture* grassTexture = Loader::LoadTexture("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/textures/grass.png", GL_REPEAT);
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
        //entity.GetRotation().y += 0.0001f;
        entity2.GetRotation().y -= 0.0001f;
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
            renderer.UpdateDistance(-0.001f);
        }
        if (window.isKeyPressed(88))
        {
            renderer.UpdateDistance(0.001f);
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