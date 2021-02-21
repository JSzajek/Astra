#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

#include <ctime>

#include <Astra.h>
#include <Astra/EntryPoint.h>

#include "Player.h"
#include "Synchronous.h"

using namespace Astra::Graphics;
using namespace Astra::Math;
using namespace Astra::Audio;

class TestGame : public Astra::Application
{
private:
    Astra::Scene* scene;
    Player* m_player;
    TextBox* m_textbox;
    DirectionalLight* dir_light;
    PointLight* light3;
    PointLight* light4;
    Model* cubeModel;
    Model* cubeModel2;
    Model* cubeModel3;
    Model* barrelModel;
    Model* brickModel;
    SkyboxMaterial* skybox;
    AudioSource* audioSource;
    Image* image;
    Vec3* particleCenter;
    ToggleButton* vsycnToggle;
    Button* multisamplingButton;
    ToggleButton* bloomToggle;
    ToggleButton* hdrToggle;
    ToggleButton* reflectionToggle;
    std::vector<const Model*> models;
    
    const float InGameTimeSpeed = 0.005f;
    short timeDir = 1;

    Timer timer;
    float elapsedTime = 0;
    unsigned int frames = 0;
public:
    void OnButtonPress()
    {
        image->operator()(ROTATION, SUM_EQ, 5);
    }

    void ToggleVsync(bool enabled)
    {
        vsycnToggle->SetText(enabled ? "Vsync: 1" : "Vsync: 0");
        GetWindow().SetVSync(enabled);
    }

    void ToggleMultiSampling()
    {
        static int multisampling = 0;
        multisampling += 1;
        multisampling %= 5; // Current Max multi sampling 4 - check:   glGetInteger64v(GL_MAX_SAMPLES) for maximum sampling?
        std::string text = "Multi: " + std::to_string(multisampling);
        multisamplingButton->SetText(text);
        GetWindow().SetMultisampling(multisampling);
    }

    void ToggleBloomSampling(bool enabled)
    {
        bloomToggle->SetText(enabled ? "Bloom: 1" : "Bloom: 0");
        GetWindow().SetBloom(enabled);
    }

    void ToggleHDR(bool enabled)
    {
        hdrToggle->SetText(enabled ? "HDR: 1" : "HDR: 0");
        GetWindow().SetHDR(enabled);
    }

    void ToggleReflection(bool enabled)
    {
        reflectionToggle->SetText(enabled ? "Refl: 1" : "Refl: 0");
        GetWindow().SetReflections(enabled);
    }

    TestGame()
        : Application()
    {
        srand((unsigned)time(0));

        scene = new Astra::Scene();
        SetCurrentScene(scene);

        auto* blendMap = ResourceManager::LoadTerrainMaterial("res/textures/blendMap.png");
        auto* pack = ResourceManager::LoadTerrainMaterialPack("res/textures/grass.jpg", "res/textures/grass.jpg", "res/textures/mud.png", "res/textures/grass.jpg");

        //Terrain terrain = Terrain(0, 0, "res/textures/meteorcrater_heightmap.png", &pack, blendMap);
        Terrain* terrain = new Terrain(0, 0, 40, 4, 0.01f, 4862, pack, blendMap);
        terrain->operator()(TRANSLATION, SUB_EQ, X_POS, 128);
        terrain->operator()(TRANSLATION, SUB_EQ, Z_POS, 128);
        scene->AddTerrain(terrain);
        
        m_player = new Player(Vec3(-25, 50, -100), terrain);
        scene->SetMainCamera(m_player->GetCamera());
        scene->AddModel(m_player->GetBody());

        WaterTile* tile1 = new WaterTile(0, 0, -2.5f, 128);
        scene->AddWaterTile(tile1);

        //const Texture* texture = Loader::LoadTexture("res/textures/grassTexture.png", false);
        auto* guiMat = ResourceManager::LoadGuiMaterial("res/textures/grassTexture.png");
        image = new Image(guiMat, Vec2(10, 200), Vec2(1), 1);
        image->SetModulate(Color::White);
        scene->AddGui(image, 0);

        //auto* fontAtlas = ResourceManager::LoadFontAtlas("res/fonts/OpenSans-Regular.ttf", 24);
        m_textbox = new TextBox("", Vec2(10), 0, Vec2(1));
        m_textbox->SetModulate(Color::Red);
        scene->AddGui(m_textbox, 0);

        auto* buttonMat = ResourceManager::LoadGuiMaterial("res/textures/Panel.png");
        Button* button = new Button(buttonMat, Vec2(200, 10), Vec2(1));
        button->SetHoverColor(Color::Red);
        button->SetPressedColor(Color::Blue);
        button->SetText("button");
        scene->AddGui(button, 2);

        button->SetOnPressed(std::bind(&TestGame::OnButtonPress, this));

        vsycnToggle = new ToggleButton(buttonMat, Vec2(850, 10), Vec2(1));
        vsycnToggle->SetHoverColor(Color::Red);
        vsycnToggle->SetToggledColor(Color::Green);
        vsycnToggle->SetText("Vsync: 0");
        scene->AddGui(vsycnToggle, 1);
        vsycnToggle->SetOnToggled(std::bind(&TestGame::ToggleVsync, this, std::placeholders::_1));

        multisamplingButton = new Button(buttonMat, Vec2(850, 50), Vec2(1));
        multisamplingButton->SetHoverColor(Color::Red);
        multisamplingButton->SetPressedColor(Color::Green);
        multisamplingButton->SetText("Multi: 0");
        scene->AddGui(multisamplingButton, 1);
        multisamplingButton->SetOnPressed(std::bind(&TestGame::ToggleMultiSampling, this));

        bloomToggle = new ToggleButton(buttonMat, Vec2(850, 90), Vec2(1));
        bloomToggle->SetHoverColor(Color::Red);
        bloomToggle->SetToggledColor(Color::Green);
        bloomToggle->SetText("Bloom: 0");
        scene->AddGui(bloomToggle, 1);
        bloomToggle->SetOnToggled(std::bind(&TestGame::ToggleBloomSampling, this, std::placeholders::_1));

        hdrToggle = new ToggleButton(buttonMat, Vec2(850, 130), Vec2(1));
        hdrToggle->SetHoverColor(Color::Red);
        hdrToggle->SetToggledColor(Color::Green);
        hdrToggle->SetText("HDR: 0");
        scene->AddGui(hdrToggle, 1);
        hdrToggle->SetOnToggled(std::bind(&TestGame::ToggleHDR, this, std::placeholders::_1));

        reflectionToggle = new ToggleButton(buttonMat, Vec2(850, 170), Vec2(1));
        reflectionToggle->SetHoverColor(Color::Red);
        reflectionToggle->SetToggledColor(Color::Green);
        reflectionToggle->SetText("Refl: 0");
        scene->AddGui(reflectionToggle, 1);
        reflectionToggle->SetOnToggled(std::bind(&TestGame::ToggleReflection, this, std::placeholders::_1));

        auto* panelMat = ResourceManager::LoadGuiMaterial("res/textures/Panel.png");
        Panel* panel = new Panel(panelMat, Vec2(100, 200), Vec2(1));
        panel->SetText("panel");
        scene->AddGui(panel, 1);

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

        skybox = ResourceManager::LoadSkyboxMaterial(m_textureFiles, m_nightTextureFiles);
        scene->SetSkyBox(skybox);

        Vec3 light_pos = Astra::Math::Vec3(20, terrain->GetHeightOfTerrain(-55, 55) + 7, 5);
        dir_light = new DirectionalLight(Vec3(25), Vec3(-0.2f, -1.0f, -0.3f), Vec3(0.2f), Vec3(0.3f), Vec3(0));
        light4 = new PointLight(Vec3(-28.75f, 0, -65.5f), Vec3(3, 1.5f, 0), Vec3(1), Vec3(5), 1, 0.22f, 0.20f);
        light3 = new PointLight(light_pos, Vec3(3, 1.5f, 0), Vec3(1), Vec3(25));

        scene->AddPointLight(light3);
        scene->AddPointLight(light4);
        scene->SetDirectionalLight(dir_light);

        cubeModel = ResourceManager::LoadModel("res/cube.fbx");
        cubeModel->SetScale(Math::Vec3(1));
        cubeModel->SetTranslation(Math::Vec3(0, 0, 20));
        models.emplace_back(cubeModel);
        scene->AddModel(cubeModel);

        // Example of loaded already loaded model and deleting pointer but not source
        cubeModel2 = ResourceManager::LoadModel("res/cube.fbx");
        RESOURCE_UNLOAD(cubeModel2);

        cubeModel3 = ResourceManager::LoadModel("res/cube.fbx");
        cubeModel3->SetScale(Math::Vec3(1));
        cubeModel3->SetTranslation(Math::Vec3(20, 0, 20));
        models.emplace_back(cubeModel3);
        scene->AddModel(cubeModel3);

        barrelModel = ResourceManager::LoadModel("res/barrel_2.fbx", true);
        barrelModel->SetScale(Math::Vec3(3));
        barrelModel->SetSelected(true);
        models.emplace_back(barrelModel);
        scene->AddModel(barrelModel);

        // FBX doesn't export displacement map- Work Around
        auto* heightMap = Loader::LoadTexture("res/textures/bricks_heightmap.jpg", false, GL_REPEAT, false);
        heightMap->type = TextureType::HeightMap;
        brickModel = ResourceManager::LoadModel("res/bricks.fbx", true);
        brickModel->GetMesh(0).GetMaterial()->AddTexture(heightMap);
        brickModel->GetMesh(0).GetMaterial()->SetHeightOffset(0.1f);
        brickModel->SetScale(Math::Vec3(10));
        brickModel->SetRotation(Math::Vec3(0, 0, -90));
        brickModel->SetTranslation(Math::Vec3(-10, 0, -10));
        models.emplace_back(brickModel);
        scene->AddModel(brickModel);

        //auto* barrelMat = ResourceManager::LoadMaterial("res/textures/barrel.png", "res/textures/barrelSpecular.jpg", "res/textures/barrelNormal.png", NULL, 0, NULL, 1, 32);
        //barrelModel = ResourceManager::LoadNormalEntity("res/barrel.obj", 0, Vec3(-40, terrain->GetHeightOfTerrain(-40, 55) + 5, 55), Vec3(0), Vec3(1));
        //barrelModel->SetMaterial(barrelMat);
        //barrelModel->SetSelected(true);
        //entities.emplace_back(barrelModel);
        //scene->AddEntity(barrelModel);

        //auto* brickMat = ResourceManager::LoadMaterial("res/textures/bricks.jpg", "res/textures/bricks_specular.jpg", "res/textures/bricks_normal.jpg", "res/textures/bricks_heightmap.jpg", 0.1f, NULL, 1, 16);
        //auto* brick = ResourceManager::LoadNormalEntity("res/plane.obj", 0, Vec3(-50, terrain->GetHeightOfTerrain(-50, 50) + 5, 50), Vec3(90, 0, 0), Vec3(5, 1, 5));
        //brick->SetMaterial(brickMat);
        //entities.emplace_back(brick);
        //scene->AddEntity(brick);

        //auto* runestoneMat = ResourceManager::LoadMaterial("res/textures/rock1_basecolor.png", "res/textures/rock1_roughness.png", "res/textures/rock1_normal.png", NULL, 0, "res/textures/rock1_emissive.png", 1, 32);
        //auto* runestone = ResourceManager::LoadNormalEntity("res/runestone_1.obj", 0, Vec3(-60, terrain->GetHeightOfTerrain(-60, 60) + 2, 60), Vec3::Zero, Vec3(2));
        //runestone->SetMaterial(runestoneMat);
        //entities.emplace_back(runestone);
        //scene->AddEntity(runestone);

        //auto* lampMat = ResourceManager::LoadMaterial("res/textures/Lamp_UV_Layout.png", "res/textures/Lamp_Specular.png", "res/textures/Lamp_Emission.png", 1, 32);
        //auto* lamp = ResourceManager::LoadEntity("res/Lamp.obj", 0, Vec3(-28.75f, -1.25f, -65.5f), Vec3::Zero, Vec3(1.5f));
        //lamp->SetMaterial(lampMat);
        //entities.emplace_back(lamp);
        //scene->AddEntity(lamp);

        //auto* mushroomMat = ResourceManager::LoadMaterial("res/textures/Boxing_Shroom_UV_Layout.png", "res/textures/Boxing_Shroom_Specular.png", NULL, 1, 8);
        //auto* mushroom = ResourceManager::LoadEntity("res/Boxing_Shroom.obj", 0, Vec3(-25, terrain->GetHeightOfTerrain(-25, -65), -65), Vec3(0, 180, 0), Vec3(2));
        //mushroom->SetMaterial(mushroomMat);
        //entities.emplace_back(mushroom);
        //scene->AddEntity(mushroom);

        //// Example of duplicate usage of vertex array object (duplicate of player)
        //auto* containerMat = ResourceManager::LoadMaterial("res/textures/container.png", "res/textures/container_specular.png", NULL, 1, 32);
        //auto* container = ResourceManager::LoadEntity("res/cube.obj", 0, Vec3(-25, 3000, 1000), Vec3(0), Vec3(2));
        //container->SetMaterial(containerMat);
        //container->SetSelected(true);
        //entities.emplace_back(container);
        //scene->AddEntity(container);

        ParticleMaterial* partMaterial = ResourceManager::LoadParticleMaterial("res/textures/particleAtlas.png", 4);
        particleCenter = new Vec3(-80, terrain->GetHeightOfTerrain(-80, 80) + 5, 80);

        ConeParticleSystem* partSystem =  new ConeParticleSystem(partMaterial, particleCenter, 15, 5, -0.1f, 1.5f, 2, true);
        partSystem->SetDirection(Vec3(0, 1, 0), 0.5f);
        partSystem->SetLifeError(0.1f);
        partSystem->SetSpeedError(0.4f);
        partSystem->SetScaleError(0.8f);
        partSystem->SetRandomRotation(true);
        scene->AddParticleSystem(partSystem);

        unsigned int tempSound = AudioController::LoadSound("res/audio/bounce.wav");
        audioSource = new AudioSource (1, 12, 15);
        audioSource->SetPosition(Vec3(-25, terrain->GetHeightOfTerrain(-25, -65), -65));
        audioSource->SetLooping(true);
        audioSource->Play(tempSound);

        /*auto* fernMat = ResourceManager::LoadMaterial("res/textures/fernAtlas.png", Texture::DefaultSpecular, NULL, 2, 0.25f, true);
        fernMat->FakeLight = true;
        for (int i = 0; i < 12; i++)
        {
            int x = (rand() % 256) - 128;
            int z = (rand() % 256) - 128;
            float y = terrain->GetHeightOfTerrain(x, z);
            auto* fern = ResourceManager::LoadEntity("res/fern.obj", rand() % 4, Vec3(static_cast<float>(x), y, static_cast<float>(z)), Vec3::Zero, Vec3(Math::Random() * 1.5f));
            fern->SetMaterial(fernMat);
            entities.emplace_back(fern);
            fern->SetSelected(Math::RandomRange(0, 10) > 5);
            scene->AddEntity(fern);
        }*/
        scene->Enable();
    }

    virtual void OnUpdate(float delta) override
    {
        m_player->Update(delta);

        barrelModel->operator()(ROTATION, SUM_EQ, Y_POS, 0.5f);

        skybox->BlendFactor() += InGameTimeSpeed * timeDir * delta;
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

        frames++;
        if (timer.Elapsed() - elapsedTime > 1.0f)
        {
            elapsedTime += 1.0f;
            m_textbox->SetText(std::to_string(frames) + std::string("fps"));
            frames = 0;
        }

    #if ASTRA_DEBUG
        if (Input::IsKeyJustPressed(Key::F3))
        {
            scene->ToggleWireframeMode();
        }
    #endif
    }

    ~TestGame()
    {
        scene->Disable();

        delete audioSource;

        // Clean up models
        for (const auto* model : models)
        {
            delete model;
        }

        delete particleCenter;

        delete dir_light;
        delete light3;
        delete light4;

        delete scene;
    }
};

Astra::Application* Astra::CreateApplication()
{
    return new TestGame();
}