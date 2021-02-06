#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

#include <ctime>

#include "Astra.h"
#include "Synchronous.h"
#include "Player.h"

int main()
{
    using namespace Astra;
    using namespace Astra::Graphics;
    using namespace Astra::Math;
    using namespace Astra::Audio;

    srand((unsigned)time(0));

    std::vector<Synchronous*> worldItems;
    std::vector<const Entity*> entities;
    Window::SetWindowTitle("Astra");
    Window::SetWindowSize(960, 540);
    Window::SetWindowResizeCallback([&](int width, int height) { RendererController::UpdateScreen(width, height); });

    Scene* const mainScene = new Scene(NULL, Vec3(0.1f));

    mainScene->Start();

    TerrainMaterial* grassTerrainMat = new TerrainMaterial("res/textures/grass.jpg");
    //TerrainMaterial* flowerTerrainMat = new TerrainMaterial("res/textures/grassFlowers.png");
    TerrainMaterial* mudTerrainMat = new TerrainMaterial("res/textures/mud.png");
    //TerrainMaterial* pathTerrainMat = new TerrainMaterial("res/textures/path.png");

    //TerrainMaterialPack pack(grassTerrainMat, flowerTerrainMat, mudTerrainMat, pathTerrainMat);
    TerrainMaterialPack* pack = new TerrainMaterialPack(grassTerrainMat, grassTerrainMat, mudTerrainMat, grassTerrainMat);
    TerrainMaterial* blendMap = new TerrainMaterial("res/textures/blendMap.png");

    //Terrain terrain = Terrain(0, 0, "res/textures/meteorcrater_heightmap.png", &pack, blendMap);
    Terrain terrain = Terrain(0, 0, 40, 4, 0.01f, 4862, pack, blendMap);
    terrain(TRANSLATION, SUB_EQ, X_POS, 128);
    terrain(TRANSLATION, SUB_EQ, Z_POS, 128);
    
    mainScene->AddTerrain(&terrain);

    Player player(Vec3(-25, 50, -100), &terrain);
    
    mainScene->SetMainCamera(player.GetCamera());
    mainScene->AddEntity(player.GetRendering());
    worldItems.emplace_back(&player);

    WaterTile tile1 = WaterTile(0, 0, -2.5f, 128);

    mainScene->AddWaterTile(&tile1);

    // TODO: Store Fonts in a directory and handle deletion before game closure or when no references

    const Texture* fontTexture = Loader::LoadAtlasTexture("res/fonts/candara.png");
    FontType* font = new FontType(fontTexture, "res/fonts/candara.fnt");
    /*GuiText* text = new GuiText("This is a test text!", font, 3, Vec2(0, 0), 1, true);
    GuiText* outlineText = new GuiText("Outlines", font, 3, Vec2(0), Vec3(0), 0, 1, Vec3(0, 0, 1));*/

    /*mainScene->AddText(text);
    mainScene->AddText(outlineText);*/

    //const Texture* texture = Loader::LoadTexture("res/textures/grassTexture.png", false);
    auto* guiMat = ResourceManager::LoadGuiMaterial("res/textures/fernAtlas.png", 2);
    Image gui = Image(guiMat, Vec2(300), Vec2(0.3f), 0);
    gui.SetModulate(Color::White);
    mainScene->AddGui(&gui, 0);
    
    auto* fontAtlas = ResourceManager::LoadFontAtlas("res/fonts/OpenSans-Regular.ttf", 48);
    TextBox textbox = TextBox("OpenGL", fontAtlas, Vec2(10), 0, Vec2(1));
    mainScene->AddGui(&textbox, 0);

    /*auto* guiMat2 = ResourceManager::LoadGuiMaterial("res/fonts/candara.png", 1);
    MetaFile* temp = new MetaFile("res/fonts/candara.fnt");
    Panel panel = Panel(guiMat2, Vec2(100), Vec2(1), 0, temp);
    mainScene->AddGui(&panel, 1);*/

   /* Label label = Label(guiMat, Vec2(0), 0, Vec2(0.25), 1);
    label.SetFont(font);
    label.SetText("This is Text the testing the line length thingy");

    mainScene->AddGui(&label, 1);*/

    //auto* guiMat2 = ResourceManager::LoadGuiMaterial("res/textures/fernTexture.png", 1);
    //Image gui2 = Image(guiMat2, Vec2(0.8f, 0.8f), Vec2(0.1f, 0.1f));
    //mainScene->AddGui(&gui2, -1);
    
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

    SkyboxMaterial* skybox = new SkyboxMaterial(m_textureFiles, m_nightTextureFiles);
    mainScene->SetSkyBox(skybox);
    
    Vec3 light_pos = Math::Vec3(-55, terrain.GetHeightOfTerrain(-55, 55) + 7, 55);
    DirectionalLight* dir_light = new DirectionalLight(Vec3(0), Vec3(-0.2f, -1.0f, -0.3f), Vec3(0.2f), Vec3(0.3f), Vec3(0));
    PointLight* light4 = new PointLight(Vec3(-28.75f, 0, -65.5f), Vec3(3, 1.5f, 0), Vec3(1), Vec3(5), 1, 0.22f, 0.20f);
    PointLight* light3 = new PointLight(light_pos, Vec3(3, 1.5f, 0), Vec3(1), Vec3(25));

    mainScene->AddPointLight(light3);
    mainScene->AddPointLight(light4);
    mainScene->SetDirectionalLight(dir_light);

    auto* barrelMat = ResourceManager::LoadMaterial("res/textures/barrel.png", "res/textures/barrelSpecular.jpg", "res/textures/barrelNormal.png", NULL, 0, NULL, 1, 32);
    auto* barrelModel = ResourceManager::LoadNormalEntity("res/barrel.obj", 0, Vec3(-40, terrain.GetHeightOfTerrain(-40, 55) + 5, 55), Vec3(0), Vec3(1));
    barrelModel->SetMaterial(barrelMat);
    barrelModel->SetSelected(true);
    entities.emplace_back(barrelModel);
    mainScene->AddEntity(barrelModel);

    auto* brickMat = ResourceManager::LoadMaterial("res/textures/bricks.jpg", "res/textures/bricks_specular.jpg", "res/textures/bricks_normal.jpg", "res/textures/bricks_heightmap.jpg", 0.1f, NULL, 1, 16);
    auto* brick = ResourceManager::LoadNormalEntity("res/plane.obj", 0, Vec3(-50, terrain.GetHeightOfTerrain(-50, 50) + 5, 50), Vec3(90, 0, 0), Vec3(5, 1, 5));
    brick->SetMaterial(brickMat);
    entities.emplace_back(brick);
    mainScene->AddEntity(brick);

    auto* runestoneMat = ResourceManager::LoadMaterial("res/textures/rock1_basecolor.png", "res/textures/rock1_roughness.png", "res/textures/rock1_normal.png", NULL, 0, "res/textures/rock1_emissive.png", 1, 32);
    auto* runestone = ResourceManager::LoadNormalEntity("res/runestone_1.obj", 0, Vec3(-60, terrain.GetHeightOfTerrain(-60, 60) + 2, 60), Vec3::Zero, Vec3(2));
    runestone->SetMaterial(runestoneMat);
    entities.emplace_back(runestone);
    mainScene->AddEntity(runestone);

    auto* lampMat = ResourceManager::LoadMaterial("res/textures/Lamp_UV_Layout.png", "res/textures/Lamp_Specular.png", "res/textures/Lamp_Emission.png", 1, 32);
    auto* lamp = ResourceManager::LoadEntity("res/Lamp.obj", 0, Vec3(-28.75f, -1.25f, -65.5f), Vec3::Zero, Vec3(1.5f));
    lamp->SetMaterial(lampMat);
    entities.emplace_back(lamp);
    mainScene->AddEntity(lamp);

    auto* mushroomMat = ResourceManager::LoadMaterial("res/textures/Boxing_Shroom_UV_Layout.png", "res/textures/Boxing_Shroom_Specular.png", NULL, 1, 8);
    auto* mushroom = ResourceManager::LoadEntity("res/Boxing_Shroom.obj", 0, Vec3(-25, terrain.GetHeightOfTerrain(-25, -65), -65), Vec3(0, 180, 0), Vec3(2));
    mushroom->SetMaterial(mushroomMat);
    entities.emplace_back(mushroom);
    mainScene->AddEntity(mushroom);

    // Example of duplicate usage of vertex array object (duplicate of player)
    auto* containerMat = ResourceManager::LoadMaterial("res/textures/container.png", "res/textures/container_specular.png", NULL, 1, 32);
    auto* container = ResourceManager::LoadEntity("res/cube.obj", 0, Vec3(-30, terrain.GetHeightOfTerrain(-30, -65) + 2, -65), Vec3(0), Vec3(2));
    container->SetMaterial(containerMat);
    container->SetSelected(true);
    entities.emplace_back(container);
    mainScene->AddEntity(container);

    auto* fernMat = ResourceManager::LoadMaterial("res/textures/fernAtlas.png", Texture::DefaultSpecular, NULL, 2, 0.25f, true);
    fernMat->FakeLight = true;
    for (int i = 0; i < 12; i++)
    {
        int x = (rand() % 256) - 128;
        int z = (rand() % 256) - 128;
        float y = terrain.GetHeightOfTerrain(x, z);
        auto* fern = ResourceManager::LoadEntity("res/fern.obj", rand() % 4, Vec3(static_cast<float>(x), y, static_cast<float>(z)), Vec3::Zero, Vec3(Math::Random() * 1.5f));
        fern->SetMaterial(fernMat);
        entities.emplace_back(fern);
        mainScene->AddEntity(fern);
        fern->SetSelected(Math::RandomRange(0, 10) > 5);
    }
    
    ParticleMaterial* partMaterial = new ParticleMaterial("res/textures/particleAtlas.png", 4);
    Vec3 particleCenter(-80, terrain.GetHeightOfTerrain(-80, 80) + 5, 80);
    
    ConeParticleSystem partSystem(partMaterial, &particleCenter, 15, 5, -0.1f, 1.5f, 2, true);
    partSystem.SetDirection(Vec3(0, 1, 0), 0.5f);
    partSystem.SetLifeError(0.1f);
    partSystem.SetSpeedError(0.4f);
    partSystem.SetScaleError(0.8f);
    partSystem.SetRandomRotation(true);
    mainScene->AddParticleSystem(&partSystem);
    
    /*ParticleMaterial* partMaterial2 = new ParticleMaterial("res/textures/particleStar.png", 1);
    Vec3 particleCenter2(-100, terrain.GetHeightOfTerrain(-100, 80) + 5, 80);

    ConeParticleSystem partSystem2(partMaterial2, &particleCenter2, 15, 5, -0.1f, 1.5f, 2, false);
    partSystem2.SetDirection(Vec3(0, 1, 0), 0.5f);
    partSystem2.SetLifeError(0.1f);
    partSystem2.SetSpeedError(0.4f);
    partSystem2.SetScaleError(0.8f);
    partSystem2.SetRandomRotation(true);
    mainScene->AddParticleSystem(&partSystem2);*/

    // Example of Gui Texture Instancing
    #define EXAMPLE_GUI_INSTANCING  0
    
    #if EXAMPLE_GUI_INSTANCING
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            Vec2 temp((float)x / 10.0f + offset, (float)y / 10.0f + offset);
            std::string t = std::to_string(x) + std::string(" ") + std::to_string(y) + std::string("temp");
            GuiTexture* gui = new GuiTexture(t.c_str(), texture.id, temp, Vec2(0.05f));
            mainScene->AddGui(gui);
        }
    }
    #endif

    mainScene->End();
    RendererController::SetCurrentScene(mainScene);

    //RendererController::SetSelectionColor(Vec3(0.04f, 0.28f, 0.26f));
    RendererController::SetSelectionColor(Vec3(0.5f, 0.25f, 0));

    unsigned int tempSound = AudioController::LoadSound("res/bounce.wav");
    AudioSource source(1, 12, 15);
    source.SetPosition(Vec3(-25, terrain.GetHeightOfTerrain(-25, -65), -65));
    source.SetLooping(true);
    source.Play(tempSound);

    const float InGameTimeSpeed = 0.00005f;
    short timeDir = 1;

    Timer timer;
    float elapsedTime = 0;
    unsigned int frames = 0;
    float delta;


    /*unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    std::string testString = "Open";

    auto vertices = std::vector<float>();
    vertices.reserve(testString.size() * 4 * 6);
    std::string::const_iterator c;
    float cursorX = 0;
    
    for (c = testString.begin(); c != testString.end(); c++)
    {
        const auto& character = fontAtlas->GetCharacter(*c);// m_characters[*c];
        const auto flipped = character.GetFlipped();
        //std::cout << character.GetChar() << " flipped " << flipped << std::endl;

        float w = character.GetWidth();
        float h = character.GetHeight();
        
        float xpos = cursorX + character.GetBearingX();
        float ypos = fontAtlas->GetFontSize() - (character.GetBearingY() - h);// (character.GetSize().y - character.GetBearing().y);

        float l_x = character.GetTexCoord(0);
        float r_x = character.GetTexCoord(1);
        float top = character.GetTexCoord(2);
        float bottom = character.GetTexCoord(3);
        
        vertices.push_back(xpos);
        vertices.push_back(ypos);
        vertices.push_back(flipped ? r_x : l_x);
        vertices.push_back(bottom);

        vertices.push_back(xpos + w);
        vertices.push_back(ypos - h);
        vertices.push_back(flipped ? l_x : r_x);
        vertices.push_back(top);

        vertices.push_back(xpos);
        vertices.push_back(ypos - h);
        vertices.push_back(l_x);
        vertices.push_back(flipped ? bottom : top);

        vertices.push_back(xpos);
        vertices.push_back(ypos);
        vertices.push_back(flipped ? r_x : l_x);
        vertices.push_back(bottom);

        vertices.push_back(xpos + w);
        vertices.push_back(ypos);
        vertices.push_back(r_x);
        vertices.push_back(flipped ? top : bottom);

        vertices.push_back(xpos + w);
        vertices.push_back(ypos - h);
        vertices.push_back(flipped ? l_x : r_x);
        vertices.push_back(top);

        cursorX += character.GetAdvance();
    }

    // update content of VBO memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6 * testString.size(), &vertices[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int tex_VAO, tex_VBO;
    glGenVertexArrays(1, &tex_VAO);
    glGenBuffers(1, &tex_VBO);
    glBindVertexArray(tex_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, tex_VBO);

    float tex_vertices[6][4] = {
        { 0.0f, 1.0f,   0.0f, 1.0f },
        { 1.0f, 0.0f,   1.0f, 0.0f },
        { 0.0f, 0.0f,   0.0f, 0.0f },

        { 0.0f, 1.0f,   0.0f, 1.0f },
        { 1.0f, 1.0f,   1.0f, 1.0f },
        { 1.0f, 0.0f,   1.0f, 0.0f }
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_vertices), tex_vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);*/

    /*Shader shader("res/temp.shader");
    shader.Start();
    auto ortho = Math::Mat4::Orthographic(0, 960, 540, 0, -1, 1);

    shader.SetUniformMat4("projectionMatrix", ortho);
    shader.Stop();
    */

    while (!Window::Closed())
    {
        delta = Window::GetDelta();
        Window::Clear();
        
        // Update All Updatable Items and Collision Detections
        for (Synchronous* item : worldItems)
        {
            item->Update(delta);
        }

        //gui.Rotation += 0.5f;

        (*barrelModel)(ROTATION, SUM_EQ, Y_POS, 0.5f);

        skybox->BlendFactor() += InGameTimeSpeed * timeDir;
        if (skybox->BlendFactor() >= 1)
        {
            skybox->BlendFactor() = 1;
            timeDir = -1;
        }
        else if (skybox->BlendFactor() <= 0)
        {
            skybox->BlendFactor() = 0;
            timeDir = 1;
        }

    #if _DEBUG
        if (Input::IsKeyJustPressed(Key::F3))
        {
            RendererController::ToggleWireframeMode();
        }
    #endif

        RendererController::Render(delta);
        
        /*shader.Start();
                                                                        // Size * Scale
        auto transform = Math::Mat4Utils::Transformation(Vec2(10,0), 0, Vec2(1));
        shader.SetUniformMat4("transformMatrix", transform);

        shader.SetUniform3f("textColor", Vec3(1, 0, 0));
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindTexture(GL_TEXTURE_2D, fontAtlas->GetId());

        // render quad
        glDrawArrays(GL_TRIANGLES, 0, testString.size() * 6);
        glDisable(GL_BLEND);

        shader.Stop();*/
        
        Window::Update();

        frames++;
        if (timer.Elapsed() - elapsedTime > 1.0f)
        {
            elapsedTime += 1.0f;
            printf("%dfps\n", frames);
            //printf("%f delta\n", delta);
            frames = 0;
        }
    }

    // Clean up fern entities
    for (const auto* entity : entities)
    {
        delete entity;
    }

    //delete text;
    //delete outlineText;
    
    // TODO: Store Fonts in a directory and handle deletion before game closure or when no references
    delete font;

    delete dir_light;
    delete light3;
    delete light4;

    return 0;
}