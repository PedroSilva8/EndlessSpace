#include "PieMenu.hpp"
#include "Core/Window.hpp"
#include "Graphics/Vulkan/VulkanDescriptor.hpp"
#include "Graphics/Vulkan/PipelineManager.hpp"
#include "Graphics/Vulkan/RenderPassManager.hpp"
#include "Graphics/Vulkan/CommandBufferManager.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Renderer.hpp"
#include "Mesh/Mesh.hpp"
#include "Graphics/TextRenderer.hpp"

PieMenu::PieMenu() {
    innerRadius = 50.0f;
    outterRadius = 100.0f;
    distanceBetweenOptions = 10.0f;
    options = vector<MenuOption*>();
    circleDetail = 16;
    shader = nullptr;
}

void PieMenu::Load() {
    if (shader == nullptr)
        LoadObject("Data/Shaders/PieMenu.shader");

    mesh = new Mesh();
    int indices[] = {0,1,2,
                     0,2,3};

    mesh->UpdateIndex(indices, sizeof(int) * 6, 6);
    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);

    mesh->AlocateData(sizeof(float) * 16);

    pipeLine = PipelineManager::GetPipeline("InterfacePie");
    
    if (pipeLine == nullptr) {
        ///Created Just For The PipeLine Layout
        uniformData = new VulkanDescriptor();
        uniformData->PrepareUniforms(shader);
        uniformData->GetUniform("PieData")->Generate(sizeof(PieMenuData));
        uniformData->Generate(shader);

        pipeLine = new VulkanPipeLine();
        pipeLine->PreparePipelineLayout();
        pipeLine->vkPipeLineData.DepthStencil.depthTestEnable = VK_FALSE;
        pipeLine->SetVertexInputInfo(TextVertex::GetBindingDescription(), TextVertex::GetAttributeDescriptions());
        pipeLine->CreatePipelineLayout(uniformData);
        pipeLine->CreatePipeline(shader, RenderPassManager::GetRenderPass("Default"));
        pipeLine->Name = "InterfacePie";

        PipelineManager::AddPipeline(pipeLine);

        ///Not Needed Anymore
        uniformData->Clean();
        uniformData = nullptr;
    }

    size_t Size = Children.size();
    for (size_t Index = 0; Index < Size; Index++)
        Children[Index]->Load();
}

void PieMenu::AddOption(MenuOption* option) {
    options.push_back(option);
    optionSize = 360 / options.size() - distanceBetweenOptions;
    
    option->uniformData = new VulkanDescriptor();
    option->uniformData->PrepareUniforms(shader);
    option->uniformData->GetUniform("PieData")->Generate(sizeof(PieMenuData));
    option->uniformData->Generate(shader);
}

void PieMenu::PreRender() {
    commandBuffer = CommandBufferManager::GetOrCreate("Interface - " + Tag);
    Vulkan::commandBuffer = commandBuffer;
    for (size_t Index = 0; Index < commandBuffer->commandBuffers.size(); Index++) {
        Vulkan::CurrentCommandBuffer = Index;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(Vulkan::GetCurrentCommandBuffer(), &beginInfo) != VK_SUCCESS)
            Debug::Error("failed to begin recording command buffer!");

        Renderer::Prepare();

        pipeLine->Bind();
        VkBuffer vertexBuffers[] = { mesh->vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        mesh->Bind();
        for (int Index = 0; Index < options.size(); Index++) {
            options[Index]->uniformData->Bind(pipeLine);
            mesh->Draw();
        }
        
        vkCmdEndRenderPass(Vulkan::GetCurrentCommandBuffer());

        if (vkEndCommandBuffer(Vulkan::GetCurrentCommandBuffer()) != VK_SUCCESS)
            Debug::Error("failed to record command buffer!");
    }
    
    ExecuteChildren(PRERENDER);
}

void PieMenu::Update() {
    if (!Active)
        return;
        
    size_t Size = options.size();
    float Offset = 0;

    for (size_t Index = 0; Index < Size; Index++) {
        Offset += optionSize + distanceBetweenOptions;

        options[Index]->data.projection = Window::OrthoProjection;
        options[Index]->data.Center = Position;
        options[Index]->data.innerRadius = innerRadius / outterRadius;
        options[Index]->data.Angle = (optionSize) / 180.0f - (0.04f * Size);
        options[Index]->data.Rotation = Offset;
        options[Index]->uniformData->GetUniform("PieData")->UpdateUniform(&options[Index]->data);
    }

    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
        Vector2 MousePosition = Abs(Input::MousePosition - Position);
        if (MousePosition < Vector2(outterRadius, outterRadius))
            OnClick.F(OnClick.Param);
    }

    ExecuteChildren(UPDATE);
}

void PieMenu::ChangePosition(Vector2 NewPosition) {

    Position = NewPosition;
    float right = outterRadius + Position.X;
    float bottom = -outterRadius + Position.Y;
    float left = -outterRadius + Position.X;
    float top = outterRadius + Position.Y;
    float quad[16] = {
        //x, y, lx, ly
        right, bottom, 1.0, -1.0,
        right, top, 1.0, 1.0,
        left, top, -1.0, 1.0,
        left, bottom, -1.0, -1.0,
    };
    
    mesh->UpdateData(quad, sizeof(float) * 16);

}

void PieMenu::Render() {
    if (!Active)
        return;

    if (Visibel) {
        CommandBufferManager::AddToQueuePool(commandBuffer);
    }

    size_t Size = Children.size();
    for (size_t Index = 0; Index < Size; Index++)
        Children[Index]->Render();
}

void PieMenu::Clean() {

    for (size_t Index = 0; Index < options.size(); Index++)
        options[Index]->uniformData->Clean();

    InterfaceObject::Clean();
}