#include "SpaceObject.hpp"
#include "Graphics/Renderer.hpp"
#include "Core/Camera.hpp"
#include "Core/ShaderManager.hpp"
#include "Core/MeshManager.hpp"
#include "Collisions/CollisionManager.hpp"
#include "Core/TextureManager.hpp"
#include "Graphics/Vulkan/PipelineManager.hpp"
#include "Graphics/Vulkan/RenderPassManager.hpp"

SpaceObject::SpaceObject() {
    isOrbitable = true;
}

void SpaceObject::LoadObject(const char* Shader, const char* Model) {

    shader = ShaderManager::GetShader(Shader);
    mesh = MeshManager::GetMesh(Model);
    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);
}

void SpaceObject::LoadCollider(Mesh* colliderMesh) {

    collider = new Collider();
    object->AddComponent(collider);
    collider->LoadCollider(colliderMesh);
    CollisionManager::AddCollider(collider);
}

void SpaceObject::Load() {
    if (shader == nullptr)
        LoadObject("Data/Shaders/Tutorial.shader", "Data/Meshes/Default.obj");
    
    uniformData.PrepareUniforms(shader);
    uniformData.GetUniform("MVP")->Generate(sizeof(Matrix4));
    uniformData.GetUniform("texSampler")->SetTexture((texture == nullptr ? TextureManager::GetTexture("Data/Textures/texture.jpg") : texture));
    uniformData.Generate(shader);

    pipeLine = new VulkanPipeLine();
    pipeLine->PreparePipelineLayout();
    pipeLine->SetVertexInputInfo(Vertex::GetBindingDescription(), Vertex::GetAttributeDescriptions());
    pipeLine->CreatePipelineLayout(&uniformData);
    pipeLine->CreatePipeline(shader, RenderPassManager::GetRenderPass("Default"));
    pipeLine->Name = "BasicPipeline";

    PipelineManager::AddPipeline(pipeLine);

    if (collider == nullptr)
        LoadCollider(mesh);
}

void SpaceObject::PreRender() {
    commandBuffer = CommandBufferManager::GetOrCreate(object->Tag);
    Vulkan::commandBuffer = commandBuffer;
    for (size_t Index = 0; Index < commandBuffer->commandBuffers.size(); Index++) {
        Vulkan::CurrentCommandBuffer = Index;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(Vulkan::GetCurrentCommandBuffer(), &beginInfo) != VK_SUCCESS)
            Debug::Error("failed to begin recording command buffer!");

        Renderer::Prepare();

        pipeLine->Bind();
        mesh->Bind();
        uniformData.Bind(pipeLine);
        mesh->Draw();
        
        vkCmdEndRenderPass(Vulkan::GetCurrentCommandBuffer());

        if (vkEndCommandBuffer(Vulkan::GetCurrentCommandBuffer()) != VK_SUCCESS)
            Debug::Error("failed to record command buffer!");
    }
}

void SpaceObject::Update() {

    model = Camera::ProjectionView * object->transform.ModelMatrix();
    uniformData.GetUniform("MVP")->UpdateUniform(&model);
}

void SpaceObject::Render() {
    CommandBufferManager::AddToQueuePool(commandBuffer);
}

void SpaceObject::Clean() {
     uniformData.Clean();
}