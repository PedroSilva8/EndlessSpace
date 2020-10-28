#include "Main.hpp"
#include "Core/Window.hpp"
#include "Core/FileManager.hpp"
#include "Core/Debug.hpp"
#include "Core/MeshManager.hpp"
#include "Graphics/Vulkan/Vulkan.hpp"
#include "Graphics/Vulkan/VulkanDeviceManager.hpp"
#include "Graphics/Vulkan/PipelineManager.hpp"
#include "Graphics/Vulkan/RenderPassManager.hpp"
#include "Graphics/Vulkan/VulkanFramebuffer.hpp"
#include "Graphics/Vulkan/VulkanCommandBuffer.hpp"
#include "Graphics/Vulkan/VulkanSemaphore.hpp"
#include "Graphics/Vulkan/VulkanOffScreenManager.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/TextRenderer.hpp"
#include "Core/TextureManager.hpp"
#include "Graphics/Vulkan/VulkanDescriptorPool.hpp"
#include "Core/Camera.hpp"
#include "Core/TimeHelper.hpp"
#include "Collisions/CollisionManager.hpp"
#include "Core/ObjectsHandler.hpp"
#include "Core/ShaderManager.hpp"
#include "Core/UpdateManager.hpp"
#include "Object/Ship/PlayerShip.hpp"
#include "Object/Asteroids/Asteroid.hpp"
#include "Object/Skybox.hpp"
#include "InterfaceObject/InterfaceManager.hpp"
#include "InterfaceObject/CustomObjects/Image.hpp"
#include "Interface/DebugData.hpp"
#include "Graphics/Vulkan/CommandPoolManager.hpp"
#include "Object/Ship/EnemyShip.hpp"
#include "Object/Ship/ShipAddonDataManager.hpp"

int main(void) {

    FileManager::UpdateGamePath();
    Window::Setup();
    CollisionManager::Init();
    ShaderManager::Init();
    ObjectsHandler::Load();
    InterfaceManager::Init();

    ///Setup Vulkan
    Vulkan::CreateInstance(InstanceData { "Vulkan", "Vulkan", VK_MAKE_VERSION(0, 1, 0), VK_MAKE_VERSION(0, 1, 0) });
    Vulkan::SetupDebugMessenger();
    Vulkan::CreateSurface();
    VulkanDeviceManager::UpdatePhysicalDevices();
    VulkanDeviceManager::CreateLogicalDevice();
    Vulkan::CreateSwapChain();
    Vulkan::CreateImageViews();

    CommandBufferManager::commandPool = new VulkanCommandPool();
    CommandBufferManager::commandPool->CreatePool();

    Vulkan::CreateDepthResources();
    VulkanSemaphore::CreateSemaphores();

    ///Load Default Vulkan Stuff
    VulkanRenderPass* renderPass = new VulkanRenderPass();
    renderPass->PrepareRenderPass();
    renderPass->renderPassData.attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    renderPass->renderPassData.attachments[0].initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    renderPass->renderPassData.attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    renderPass->renderPassData.attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    renderPass->CreateRenderPass();
    renderPass->Name = "Default";

    RenderPassManager::AddRenderPass(renderPass);

    renderPass = new VulkanRenderPass();
    renderPass->PrepareRenderPass();
    renderPass->CreateRenderPass();
    renderPass->Name = "Clear";

    RenderPassManager::AddRenderPass(renderPass);

    VulkanFramebuffer* frameBuffer = new VulkanFramebuffer();
    frameBuffer->CreateFramebuffers(renderPass);

    Vulkan::frameBuffer = frameBuffer;

    Debug::Log("Finished Loading Vulkan Starting Game");

    //Load Game
    //Renderer::Init();
    TextRenderer::Init();

    DebugData *dbData = new DebugData();
    dbData->Position = Vector2(0, 0);
    dbData->ForegroundColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
    InterfaceManager::AddObject(dbData);

    UpdateManager::CreateGroup("Asteroids1");
    UpdateManager::CreateGroup("Asteroids2");
    UpdateManager::CreateGroup("Others");

    Object *skybox = new Object();
    skybox->Tag = "Skybox";
    Skybox *skyComp = new Skybox();
    skybox->AddComponent(skyComp);
    skyComp->LoadSkybox("Data/Skybox/SmallRuralRoad.hdr", "Data/Meshes/Teste.obj", "Data/Shaders/Skybox.shader");
    UpdateManager::AddToGroup("Others", skybox);
    ObjectsHandler::AddObject(skybox);

    ShipAddonData* addonData = new ShipAddonData();
    addonData->Name = "Laser I";
    addonData->Type = "Laser";
    addonData->MeshLocation = "Data/Meshes/Missel Wepon.obj";
    addonData->ShaderLocation = "Data/Shaders/Tutorial.shader";
    addonData->TextureLocation = "Data/Textures/Test.png";

    ShipAddonDataManager::AddAddonData(addonData);

    Object* ship = new Object();
    ship->Tag = "PlayerShip";
    PlayerShip* playerShip = new PlayerShip();
    playerShip->AddAddon("Laser I", Vector3(11.0f, 1.14099f, -2.45f), Quaternion::EulerToQuaternion(Vector3(0.0f, 45.9f, 0.0f)));
    ship->AddComponent(playerShip);

    UpdateManager::AddToGroup("Others", ship);
    ObjectsHandler::AddObject(ship);

    Object* enemyShip = new Object();
    enemyShip->transform.localPosition = Vector3(10, 10, 10);
    enemyShip->Tag = "EnemyShip";
    enemyShip->AddComponent(new EnemyShip());

    UpdateManager::AddToGroup("Others", enemyShip);
    ObjectsHandler::AddObject(enemyShip);

    ///Asteroids
    for (int Index = 0; Index < 10; Index++) {
        Object* a = new Object();
        a->Tag = "Asteroid G1 N" + to_string(Index);
        a->AddComponent(new Asteroid());
        UpdateManager::AddToGroup("Asteroids1", a);
        ObjectsHandler::AddObject(a);
    }

    for (int Index = 0; Index < 10; Index++){
        Object* a = new Object();
        a->Tag = "Asteroid G2 N" + to_string(Index);
        a->AddComponent(new Asteroid());
        UpdateManager::AddToGroup("Asteroids2", a);
        ObjectsHandler::AddObject(a);
    }

    InterfaceManager::ExecuteCode(LOAD);
    ObjectsHandler::ExecuteCode(LOAD);
    TimeHelper::Start();
    
    ObjectsHandler::ExecuteCode(PRERENDER);
    InterfaceManager::ExecuteCode(PRERENDER);

    while (!glfwWindowShouldClose(Window::window)) {
        TimeHelper::Update();

        glfwPollEvents();

        Vulkan::PrepareFrame();

        if (Vulkan::SwapChainRecreated) {
            ObjectsHandler::ExecuteCode(PRERENDER);
            InterfaceManager::ExecuteCode(PRERENDER);
        }

        Renderer::Clear();
        Camera::Update();
        TextRenderer::BeginTextUpdate();
        InterfaceManager::ExecuteCode(UPDATE);
        ObjectsHandler::ExecuteCode(UPDATE);
        while (!CollisionManager::Work.empty()) { }
        ObjectsHandler::ExecuteCode(RENDER);
        ObjectsHandler::ExecuteCode(POSRENDER);
        InterfaceManager::ExecuteCode(RENDER);
        InterfaceManager::ExecuteCode(POSRENDER);
        TextRenderer::EndTextUpdate();
        Input::Clear();
        Vulkan::DrawFrame();
        CommandBufferManager::commandPool->drawQueue.clear();
    }

    vkDeviceWaitIdle(VulkanDeviceManager::GetSelectedDevice());
    
    ObjectsHandler::ExecuteCode(CLEAN);
    InterfaceManager::ExecuteCode(CLEAN);

    TextRenderer::Clean();
    TextureManager::Clear();
    ObjectsHandler::Clear();
    InterfaceManager::Clear();
    CollisionManager::Clear();
    MeshManager::Clear();
    ShaderManager::Clear();
    VulkanSemaphore::Clean();
    CommandBufferManager::Clear();
    frameBuffer->Clean();
    PipelineManager::CleanPipelines();
    RenderPassManager::ClearRenderPass();
    VulkanOffScreenManager::Clean();
    CommandPoolManager::Clean();
    Vulkan::Clean();
    Input::Clear();
    Window::Clear();

    _exit(0);
}
