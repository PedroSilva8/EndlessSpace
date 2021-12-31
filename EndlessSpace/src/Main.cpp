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
