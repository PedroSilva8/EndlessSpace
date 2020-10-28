#include "ShipAddon.hpp"
#include "Core/Camera.hpp"
#include "Core/ShaderManager.hpp"
#include "Core/MeshManager.hpp"
#include "Core/TextureManager.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Vulkan/VulkanDescriptor.hpp"
#include "Graphics/Vulkan/CommandBufferManager.hpp"
#include "ShipAddonDataManager.hpp"
#include "Graphics/Vulkan/PipelineManager.hpp"
#include "Graphics/Vulkan/RenderPassManager.hpp"
#include "Math/Matrix4.hpp"

void ShipAddon::Load(ShipAddonData* data) {

    shader = ShaderManager::GetShader(data->ShaderLocation);
    mesh = MeshManager::GetMesh(data->MeshLocation);
    texture = TextureManager::GetTexture(data->TextureLocation);

    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);

    uniformData = new VulkanDescriptor();
    uniformData->PrepareUniforms(shader);
    
    uniformData->GetUniform("MVP")->Generate(sizeof(Matrix4));
    if (texture != nullptr)
        uniformData->GetUniform("texSampler")->SetTexture(texture);
    
    uniformData->Generate(shader);
    
    pipeLine = PipelineManager::GetPipeline("ShipAddon_" + data->Name + "_" + data->Type);

    if (pipeLine == nullptr) {
        pipeLine = new VulkanPipeLine();
        pipeLine->PreparePipelineLayout();
        pipeLine->SetVertexInputInfo(Vertex::GetBindingDescription(), Vertex::GetAttributeDescriptions());
        pipeLine->CreatePipelineLayout(uniformData);
        pipeLine->CreatePipeline(shader, RenderPassManager::GetRenderPass("Default"));
        pipeLine->Name = "ShipAddon_" + data->Name + "_" + data->Type;

        PipelineManager::AddPipeline(pipeLine);
    }
}

void ShipAddon::Update(Vector3 Position, Quaternion Rotation) {
    Matrix4 model = Camera::ProjectionView * (Matrix4::Translate(Position)  * Matrix4::FromQuaternion(Rotation) * Matrix4::Translate(RelativePosition) * Matrix4::FromQuaternion(LocalRotation));
    uniformData->GetUniform("MVP")->UpdateUniform(&model);
}

void ShipAddon::PreRender() {
    commandBuffer = CommandBufferManager::Create();
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
        uniformData->Bind(pipeLine);
        mesh->Draw();
        
        vkCmdEndRenderPass(Vulkan::GetCurrentCommandBuffer());

        if (vkEndCommandBuffer(Vulkan::GetCurrentCommandBuffer()) != VK_SUCCESS)
            Debug::Error("failed to record command buffer!");
    }
}

void ShipAddon::Render() {
    CommandBufferManager::AddToQueuePool(commandBuffer);
}

void ShipAddon::Clean() {
    uniformData->Clean();
}