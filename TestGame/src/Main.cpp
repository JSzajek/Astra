#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

#include <ctime>

#include <Astra.h>
#include <Astra/EntryPoint.h>

#include "Player.h"
#include "Synchronous.h"

#include <unordered_set>

using namespace Astra::Graphics;
using namespace Astra::Math;
using namespace Astra::Audio;

class TestGame : public Astra::Application
{
private:
    Astra::Scene* scene;
    Player* m_player;
    AudioSource* audioSource;

    TextBox* m_textbox;
    Image* image;
    ToggleButton* vsycnToggle;
    Button* multisamplingButton;
    ToggleButton* bloomToggle;
    ToggleButton* hdrToggle;
    ToggleButton* reflectionToggle;
    Model* bunny;
    
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

    void OnAwake() override
    {
        m_textbox = scene->Get<TextBox>("lol");

        image = scene->Get<Image>("image");

        scene->Get<Button>("button")->SetOnPressed(std::bind(&TestGame::OnButtonPress, this));
        
        vsycnToggle = scene->Get<ToggleButton>("vsync toggle");
        multisamplingButton = scene->Get<Button>("multisampling button");
        bloomToggle = scene->Get<ToggleButton>("bloom toggle");
        hdrToggle = scene->Get<ToggleButton>("hdr toggle");
        reflectionToggle = scene->Get<ToggleButton>("reflection toggle");

        bunny = scene->Get<Model>("bunny");

        vsycnToggle->SetOnToggled(std::bind(&TestGame::ToggleVsync, this, std::placeholders::_1));
        multisamplingButton->SetOnPressed(std::bind(&TestGame::ToggleMultiSampling, this));
        bloomToggle->SetOnToggled(std::bind(&TestGame::ToggleBloomSampling, this, std::placeholders::_1));
        hdrToggle->SetOnToggled(std::bind(&TestGame::ToggleHDR, this, std::placeholders::_1));
        reflectionToggle->SetOnToggled(std::bind(&TestGame::ToggleReflection, this, std::placeholders::_1));
    }

    TestGame()
        : Application()
    {
        srand((unsigned)time(0));

        // Should open scene from file data.
        scene = new Astra::Scene();
        SetCurrentScene(scene);

        auto terrainMat = TerrainMaterial("res/textures/blendMap.png", "res/textures/grass.jpg", "res/textures/grass.jpg", "res/textures/mud.png", "res/textures/grass.jpg");
        
        //Terrain terrain = Terrain(0, 0, "res/textures/meteorcrater_heightmap.png", &pack, blendMap);
        Terrain terrain(0, 0, 40, 4, 0.01f, 4862, terrainMat);
        //terrain->operator()(TRANSLATION, SUB_EQ, X_POS, 128);
        //terrain->operator()(TRANSLATION, SUB_EQ, Z_POS, 128);
        terrain.SetTranslation(terrain.GetTranslation() - Vec3(128, 0, 128));
        auto* terr = scene->AddTerrain(terrain);
        
        m_player = new Player(Vec3(-25, 50, -100), terr);
        scene->SetMainCamera(m_player->GetCamera());
        //scene->AddModel(m_player->GetBody());

        WaterTile tile1(0, 0, -2.5f, 128);
        scene->AddWaterTile(tile1);

        TextBox textbox("lol", "", Vec2(10), 0, Vec2(1));
        textbox.SetModulate(Color::Red);
        scene->Add<TextBox>(textbox, 0);

        auto guiMat = GuiMaterial("res/textures/grassTexture.png");
        auto image = Image("image", guiMat, Vec2(10, 200), Vec2(1), 1);
        image.SetModulate(Color::White);
        scene->Add<Image>(image, 0);

        auto buttonMat = GuiMaterial("res/textures/Panel.png");
        auto button = Button("button", buttonMat, Vec2(200, 10), Vec2(1));
        button.SetHoverColor(Color::Red);
        button.SetPressedColor(Color::Blue);
        button.SetText("button");
        scene->Add<Button>(button, 2);

        auto vsycnToggle = ToggleButton("vsync toggle", buttonMat, Vec2(850, 10), Vec2(1));
        vsycnToggle.SetHoverColor(Color::Red);
        vsycnToggle.SetToggledColor(Color::Green);
        vsycnToggle.SetText("Vsync: 0");
        scene->Add<ToggleButton>(vsycnToggle, 1);

        auto multisamplingButton = Button("multisampling button", buttonMat, Vec2(850, 50), Vec2(1));
        multisamplingButton.SetHoverColor(Color::Red);
        multisamplingButton.SetPressedColor(Color::Green);
        multisamplingButton.SetText("Multi: 0");
        scene->Add<Button>(multisamplingButton, 1);

        auto bloomToggle = ToggleButton("bloom toggle", buttonMat, Vec2(850, 90), Vec2(1));
        bloomToggle.SetHoverColor(Color::Red);
        bloomToggle.SetToggledColor(Color::Green);
        bloomToggle.SetText("Bloom: 0");
        scene->Add<ToggleButton>(bloomToggle, 1);

        auto hdrToggle = ToggleButton("hdr toggle", buttonMat, Vec2(850, 130), Vec2(1));
        hdrToggle.SetHoverColor(Color::Red);
        hdrToggle.SetToggledColor(Color::Green);
        hdrToggle.SetText("HDR: 0");
        scene->Add<ToggleButton>(hdrToggle, 1);

        auto reflectionToggle = ToggleButton("reflection toggle", buttonMat, Vec2(850, 170), Vec2(1));
        reflectionToggle.SetHoverColor(Color::Red);
        reflectionToggle.SetToggledColor(Color::Green);
        reflectionToggle.SetText("Refl: 0");
        scene->Add<ToggleButton>(reflectionToggle, 1);

        auto panelMat = GuiMaterial("res/textures/Panel.png");
        auto panel = Panel(panelMat, Vec2(100, 200), Vec2(1));
        panel.SetText("panel");
        scene->Add<Panel>(panel, 1);

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

        auto skybox1 = SkyboxMaterial(m_textureFiles, m_nightTextureFiles);
        scene->SetSkyBox(skybox1);

        Vec3 light_pos = Astra::Math::Vec3(20, terrain.GetHeightOfTerrain(-55, 55) + 7, 5);
        auto dir_light = DirectionalLight("Sun", Vec3(25), Vec3(-0.2f, -1.0f, -0.3f), Vec3(0.2f), Vec3(0.3f), Vec3(0));
        auto light4 = PointLight(Vec3(-28.75f, 5, -65.5f), Vec3(3, 1.5f, 0), Vec3(1), Vec3(5), 1, 0.22f, 0.20f);
        auto light3 = PointLight("Lamp Light", light_pos, Vec3(3, 1.5f, 0), Vec3(1), Vec3(25));

        scene->SetDirectionalLight(dir_light);
        scene->AddPointLight(light3);
        scene->AddPointLight(light4);

        auto cubeModel = Model("res/cube.fbx", false);
        cubeModel.SetScale(Math::Vec3(1));
        cubeModel.SetTranslation(Math::Vec3(0, 0, 20));
        scene->AddModel(cubeModel);

        /*auto vampire = Model("res/vampire/dancing_vampire.dae", true);
        vampire.SetScale(Math::Vec3(10));
        vampire.SetTranslation(Math::Vec3(50, terrain.GetHeightOfTerrain(50, 70), 70));
        vampire.AddAnimator();
        vampire.PlayAnimation("");
        scene->AddModel(vampire);*/

        // FBX doesn't export displacement map- Work Around
        auto heightMap = Resource::LoadTexture(TextureCreationSpec("res/textures/bricks_heightmap.jpg", false, false));
        heightMap->type = TextureType::HeightMap;
        auto brickModel = Model("res/bricks.fbx", true);
        brickModel.GetMaterial().AddTexture(heightMap);
        brickModel.GetMaterial().SetHeightOffset(0.1f);
        brickModel.SetScale(Math::Vec3(10));
        brickModel.SetRotation(Math::Vec3(0, 0, -90));
        brickModel.SetTranslation(Math::Vec3(-10, terrain.GetHeightOfTerrain(-10, -10) + 20, -10));
        scene->AddModel(brickModel);

       /* auto bunnyModel = Model("bunny", "res/bunny.fbx", false);
        bunnyModel.SetTranslation(Math::Vec3(-28.75f, terrain.GetHeightOfTerrain(-28.75f, -75), -75));
        bunnyModel.SetScale(Math::Vec3(2));
        scene->AddModel(bunnyModel);*/

        auto partMaterial = ParticleMaterial("res/textures/particleAtlas.png", 4);
        auto particleCenter = Vec3(-80, terrain.GetHeightOfTerrain(-80, 80) + 5, 80);

        ParticleSystem partSystem("Fire Spout", partMaterial, particleCenter, 15, 5, -0.1f, 1.5f, true);
        partSystem.SetScale(2);
        partSystem.SetDirection(Vec3(0, 1, 0), 0.5f);
        partSystem.SetLifeError(0.1f);
        partSystem.SetSpeedError(0.4f);
        partSystem.SetScaleError(0.8f);
        partSystem.SetRandomRotation(true);
        scene->AddParticleSystem(partSystem);

        unsigned int tempSound = AudioController::LoadSound("res/audio/bounce.wav");
        audioSource = new AudioSource (1, 12, 15);
        audioSource->SetPosition(Vec3(-25, terrain.GetHeightOfTerrain(-25, -65), -65));
        audioSource->SetLooping(true);
        audioSource->Play(tempSound);

        scene->Enable();
    }

    virtual void OnUpdate(float delta) override
    {
        auto* system = scene->Get<ParticleSystem>("Fire Spout");
        auto str3 = system->ToString();

        m_player->Update(delta);

        //barrelModel->operator()(ROTATION, SUM_EQ, Y_POS, 0.5f);

        /*skybox->BlendFactor() += InGameTimeSpeed * timeDir * delta;
        if (skybox->BlendFactor() >= 1)
        {
            skybox->BlendFactor() = 1;
            timeDir = -1;
        }
        else if (skybox->BlendFactor() <= 0)
        {
            skybox->BlendFactor() = 0;
            timeDir = 1;
        }*/

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

        if (Input::IsKeyJustPressed(Key::L))
        {
            bunny->SetLODMesh(0.5f);
        }
    }

    ~TestGame()
    {
        scene->Disable();

        delete audioSource;
        delete m_player;
        delete scene;
    }
};

Astra::Application* Astra::CreateApplication()
{
    return new TestGame();
}