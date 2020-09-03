#include <iostream>
#include <vector>

#include "src/math/Maths.h"
#include "src/graphics/Window.h"
#include "src/graphics/buffers/VertexArray.h"
#include "src/utils/Timer.h"

#include "src/graphics/loaders/Loader.h"
#include "src/graphics/loaders/ObjLoader.h"
#include "src/graphics/buffers/Texture.h"

#include "src/graphics/renderers/RendererController.h"

#include "src/graphics/entities/Entity.h"

int main()
{
    using namespace Astra;
    using namespace Astra::Graphics;
    using namespace Astra::Math;

    Window window("Astra", 960, 540);
    glClearColor(0.6f, 0.3f, 0.8f, 1.0f);

    RendererController renderer;
    window.SetWindowResizeCallback([&](float width, float height) { renderer.UpdateScreen(width, height); });

    Texture* texture = Loader::LoadTexture("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/textures/test1.jpg");
    renderer.AddGui(GuiTexture(texture->id, Vec2(0.75,0.75), Vec2(0.1,0.1)));

    Texture* texture2 = Loader::LoadTexture("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/textures/red.jpg");
    texture2->reflectivity = 1;
    texture2->shineDampener = 8;
    const VertexArray* vertArray = ObjLoader::LoadObjectModel("C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/dragon.obj");
    Entity entity = Entity(vertArray, texture2, Vec3(0, 0, 0), Vec3(0), Vec3(0.5f));
    Entity entity2 = Entity(vertArray, texture2, Vec3(0, 0, 2), Vec3(0), Vec3(0.2f));
    renderer.AddEntity(entity);
    renderer.AddEntity(entity2);

    Timer time;
    float timer = 0;
    unsigned int frames = 0;

    while (!window.Closed())
    {
        window.Clear();
        renderer.Render();
        window.Update();

        if (window.isKeyPressed(GLFW_KEY_W))
        {
            renderer.UpdatePitch(0.2f);
        }
        if (window.isKeyPressed(GLFW_KEY_S))
        {
            renderer.UpdatePitch(-0.2f);
        }

        if (window.isKeyPressed(GLFW_KEY_A))
        {
            renderer.UpdateYaw(-0.2f);
        }
        if (window.isKeyPressed(GLFW_KEY_D))
        {
            renderer.UpdateYaw(0.2f);
        }

        if (window.isKeyPressed(GLFW_KEY_Z))
        {
            renderer.UpdateDistance(-0.001f);
        }
        if (window.isKeyPressed(GLFW_KEY_X))
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