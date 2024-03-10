#include "VulkanPipeline.h"
#include <Runtime/Vulkan/Pipeline/VulkanPipelineUtils.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Shader/VulkanShader.h>
#include <Runtime/Vulkan/VulkanUtils.h>
#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Shader/VulkanShaderUtils.h>
#include <Runtime/Graphics/Texture/TextureUtils.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPass.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorSetLayout.h>

namespace Dream
{

	VulkanPipeline::VulkanPipeline(const GraphicsPipelineDesc& desc, VulkanDevice* pDevice) : Pipeline(desc,pDevice), mLayout(VK_NULL_HANDLE), mPipeline(VK_NULL_HANDLE), mLogicalDevice(pDevice->GetVkLogicalDevice())
	{
		/**
		* Create input layout
		*/
		std::vector<VkVertexInputBindingDescription> inputBindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions;
		for (unsigned int bindingIndex = 0; bindingIndex < desc.InputLayout.Bindings.size(); bindingIndex++)
		{
			const InputBinding& binding = desc.InputLayout.Bindings[bindingIndex];

			VkVertexInputBindingDescription vkBindingDescription = {};
			vkBindingDescription.inputRate = VulkanPipelineUtils::GetInputRate(binding.StepRate);
			vkBindingDescription.binding = bindingIndex;

			unsigned int inputOffset = 0;
			for (unsigned int attributeIndex = 0; attributeIndex < binding.Elements.size(); attributeIndex++)
			{
				const InputElement& inputElement = binding.Elements[attributeIndex];

				VkVertexInputAttributeDescription vkAttributeDescription = {};
				vkAttributeDescription.binding = bindingIndex;
				vkAttributeDescription.format = VulkanTextureUtils::GetTextureFormat(inputElement.Format);
				vkAttributeDescription.location = attributeIndex;
				vkAttributeDescription.offset = inputOffset;

				inputAttributeDescriptions.push_back(vkAttributeDescription);

				inputOffset += TextureUtils::format_size(inputElement.Format);
			}

			/**
			* Set stride
			*/
			vkBindingDescription.stride = inputOffset;

			/**
			* Add binding desc
			*/
			inputBindingDescriptions.push_back(vkBindingDescription);
		}
		VkPipelineVertexInputStateCreateInfo inputStateCreateInfo = {};
		inputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		inputStateCreateInfo.pNext = nullptr;
		inputStateCreateInfo.pVertexBindingDescriptions = inputBindingDescriptions.data();
		inputStateCreateInfo.vertexBindingDescriptionCount = inputBindingDescriptions.size();
		inputStateCreateInfo.pVertexAttributeDescriptions = inputAttributeDescriptions.data();
		inputStateCreateInfo.vertexAttributeDescriptionCount = inputAttributeDescriptions.size();

		/**
		* Create input assembly
		*/
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {};
		inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyCreateInfo.pNext = nullptr;
		inputAssemblyCreateInfo.primitiveRestartEnable = false;
		inputAssemblyCreateInfo.topology = VulkanPipelineUtils::GetTopology(desc.InputLayout.Topology);

		/**
		* Create viewport and scissors
		*/
		VkViewport viewport = {};
		viewport.x = desc.Viewport.X;
		viewport.y = desc.Viewport.Y;
		viewport.width = desc.Viewport.Width;
		viewport.height = desc.Viewport.Height;
		viewport.minDepth = desc.Viewport.DepthMin;
		viewport.maxDepth = desc.Viewport.DepthMax;

		VkRect2D scissor = {};
		scissor.offset = { 0,0 };
		scissor.extent = { 0,0 };

		VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
		viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateCreateInfo.pNext = nullptr;
		viewportStateCreateInfo.viewportCount = 1;
		viewportStateCreateInfo.scissorCount = 1;
		viewportStateCreateInfo.pViewports = &viewport;
		viewportStateCreateInfo.pScissors = &scissor;

		/**
		* Create rasterizer
		*/
		VkPipelineRasterizationStateCreateInfo rasterizerStateCreateInfo = {};
		rasterizerStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerStateCreateInfo.pNext = nullptr;
		rasterizerStateCreateInfo.rasterizerDiscardEnable = false;
		rasterizerStateCreateInfo.polygonMode = VulkanPipelineUtils::GetPolygonMode(desc.RasterizerState.FillMode);
		rasterizerStateCreateInfo.lineWidth = 1.0f;
		rasterizerStateCreateInfo.cullMode = VulkanPipelineUtils::GetCullModes(desc.RasterizerState.CullFlags);
		rasterizerStateCreateInfo.frontFace = desc.RasterizerState.bFrontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
		rasterizerStateCreateInfo.depthBiasEnable = desc.RasterizerState.bDepthBiasEnabled;
		rasterizerStateCreateInfo.depthBiasConstantFactor = desc.RasterizerState.DepthBiasFactor;
		rasterizerStateCreateInfo.depthBiasClamp = desc.RasterizerState.DepthBiasClamp;
		rasterizerStateCreateInfo.depthBiasSlopeFactor = desc.RasterizerState.DepthBiasSlope;

		/**
		* Create multisample state
		*/
		VkPipelineMultisampleStateCreateInfo multiSampleStateCreateInfo = {};
		multiSampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multiSampleStateCreateInfo.pNext = nullptr;
		multiSampleStateCreateInfo.sampleShadingEnable = desc.Multisample.bSampleShadingEnabled;
		multiSampleStateCreateInfo.rasterizationSamples = (VkSampleCountFlagBits)VulkanPipelineUtils::GetSampleFlags(desc.Multisample.Samples);
		multiSampleStateCreateInfo.minSampleShading = 1.0f;
		multiSampleStateCreateInfo.pSampleMask = nullptr;
		multiSampleStateCreateInfo.alphaToCoverageEnable = 0;
		multiSampleStateCreateInfo.alphaToOneEnable = 0;

		/**
		* Create depth stencil state
		*/
		VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};
		depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilStateCreateInfo.pNext = nullptr;
		depthStencilStateCreateInfo.depthTestEnable = desc.DepthStencilState.bDepthTestEnabled;
		depthStencilStateCreateInfo.depthWriteEnable = desc.DepthStencilState.bDepthWriteEnabled;
		depthStencilStateCreateInfo.depthCompareOp = VulkanUtils::GetCompareOperation(desc.DepthStencilState.DepthTestOperation);
		depthStencilStateCreateInfo.depthBoundsTestEnable = false;
		depthStencilStateCreateInfo.minDepthBounds = 0;
		depthStencilStateCreateInfo.maxDepthBounds = 1.0f;
		depthStencilStateCreateInfo.stencilTestEnable = desc.DepthStencilState.bStencilTestEnabled;
		depthStencilStateCreateInfo.front = VulkanPipelineUtils::GetStencilFaceState(desc.DepthStencilState.StencilFrontFace);
		depthStencilStateCreateInfo.back = VulkanPipelineUtils::GetStencilFaceState(desc.DepthStencilState.StencilBackFace);

		/**
		* Create blending state
		*/
		VkPipelineColorBlendStateCreateInfo blendingStateCreateInfo = {};
		blendingStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		blendingStateCreateInfo.logicOpEnable = desc.BlendState.bLogicOperationEnabled;
		blendingStateCreateInfo.logicOp = VulkanUtils::GetLogicOperation(desc.BlendState.LogicOperation);
		blendingStateCreateInfo.flags = VkPipelineColorBlendStateCreateFlags();
		blendingStateCreateInfo.blendConstants[0] = 1;
		blendingStateCreateInfo.blendConstants[1] = 1;
		blendingStateCreateInfo.blendConstants[2] = 1;
		blendingStateCreateInfo.blendConstants[3] = 1;

		std::vector<VkPipelineColorBlendAttachmentState> blendingAttachments;
		for (unsigned char attachmentIndex = 0; attachmentIndex < desc.BlendState.Attachments.size(); attachmentIndex++)
		{
			const BlendStateAttachment& attachment = desc.BlendState.Attachments[attachmentIndex];

			blendingAttachments.push_back(VulkanPipelineUtils::GetBlendAttachmentState(attachment));
		}
		blendingStateCreateInfo.pAttachments = blendingAttachments.data();
		blendingStateCreateInfo.attachmentCount = blendingAttachments.size();


		blendingStateCreateInfo.pNext = nullptr;

		/**
		* Create pipeline resource layout
		*/
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
		for (unsigned int layoutIndex = 0; layoutIndex < desc.ResourceLayout.ResourceLayouts.size(); layoutIndex++)
		{
			const VulkanDescriptorSetLayout* pLayout = (const VulkanDescriptorSetLayout*)desc.ResourceLayout.ResourceLayouts[layoutIndex];
			descriptorSetLayouts.push_back(pLayout->vk_layout());
		}

		VkPipelineLayoutCreateInfo layoutCreateInfo = {};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutCreateInfo.setLayoutCount = descriptorSetLayouts.size();
		layoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
		layoutCreateInfo.pPushConstantRanges = nullptr;
		layoutCreateInfo.pushConstantRangeCount = 0;
		layoutCreateInfo.pNext = nullptr;

		DEV_ASSERT(vkCreatePipelineLayout(pDevice->GetVkLogicalDevice(), &layoutCreateInfo, nullptr, &mLayout) == VK_SUCCESS, "VulkanPipeline", "Failed to create pipeline layout!");


		/**
		* Get shader stage info
		*/
		VkPipelineShaderStageCreateInfo vkShaderStageInfos[8];
		std::string vkShaderStageNameCache[8];
		for (unsigned char shaderIndex = 0; shaderIndex < desc.GraphicsShaders.size(); shaderIndex++)
		{
			const VulkanShader* pShader = (const VulkanShader*)desc.GraphicsShaders[shaderIndex];

			//Cache entry point
			vkShaderStageNameCache[shaderIndex] = pShader->entry_method();

			VkPipelineShaderStageCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			info.module = pShader->GetVkModule();
			info.pName = vkShaderStageNameCache[shaderIndex].c_str();
			info.stage = (VkShaderStageFlagBits)VulkanShaderUtils::GetShaderFlags(pShader->stage());
			info.pSpecializationInfo = nullptr;
			info.pNext = nullptr;
			vkShaderStageInfos[shaderIndex] = info;
		}

		constexpr VkDynamicState dynamicStates[] =
		{
			//VK_DYNAMIC_STATE_VIEWPORT,
			 VK_DYNAMIC_STATE_SCISSOR
		};
		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateCreateInfo.pNext = nullptr;
		dynamicStateCreateInfo.flags = VkPipelineDynamicStateCreateFlags();
		dynamicStateCreateInfo.dynamicStateCount = sizeof(dynamicStates) / sizeof(VkDynamicState);
		dynamicStateCreateInfo.pDynamicStates = dynamicStates;

		//Get render pass
		const VulkanRenderPass* pPass = (const VulkanRenderPass*)desc.pRenderPass;

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.pNext = nullptr;
		pipelineCreateInfo.stageCount = desc.GraphicsShaders.size();
		pipelineCreateInfo.pStages = vkShaderStageInfos;
		pipelineCreateInfo.pVertexInputState = &inputStateCreateInfo;
		pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
		pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
		pipelineCreateInfo.pRasterizationState = &rasterizerStateCreateInfo;
		pipelineCreateInfo.pMultisampleState = &multiSampleStateCreateInfo;
		pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
		pipelineCreateInfo.pColorBlendState = &blendingStateCreateInfo;
		pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
		pipelineCreateInfo.layout = mLayout;
		pipelineCreateInfo.renderPass = pPass->GetVkRenderPass();
		pipelineCreateInfo.subpass = desc.SubpassIndex;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;
		pipelineCreateInfo.flags = VkPipelineCreateFlags();

		DEV_ASSERT(vkCreateGraphicsPipelines(pDevice->GetVkLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &mPipeline) == VK_SUCCESS, "VulkanPipeline", "Failed to create graphics pipeline");

		//Clean up the trash
		mBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	}
	VulkanPipeline::VulkanPipeline(const ComputePipelineDesc& desc, VulkanDevice* pDevice) : Pipeline(desc,pDevice), mBindPoint(VK_PIPELINE_BIND_POINT_COMPUTE),mLayout(VK_NULL_HANDLE),mPipeline(VK_NULL_HANDLE), mLogicalDevice(pDevice->GetVkLogicalDevice())
	{
		//Create pipeline shader stage informations
		VkPipelineShaderStageCreateInfo vkShaderStageInfo;
		std::string vkShaderStageNameCache;
		const VulkanShader* pShader = (const VulkanShader*)desc.pComputeShader;

		//Cache entry point
		vkShaderStageNameCache = pShader->entry_method();

		VkPipelineShaderStageCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.module = pShader->GetVkModule();
		info.pName = vkShaderStageNameCache.c_str();
		info.stage = (VkShaderStageFlagBits)VulkanShaderUtils::GetShaderFlags(pShader->stage());
		info.pSpecializationInfo = nullptr;
		info.pNext = nullptr;
		vkShaderStageInfo = info;

		//Create resource layout
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
		for (unsigned int layoutIndex = 0; layoutIndex < desc.DescriptorSetLayouts.size(); layoutIndex++)
		{
			const VulkanDescriptorSetLayout* pLayout = (const VulkanDescriptorSetLayout*)desc.DescriptorSetLayouts[layoutIndex];
			descriptorSetLayouts.push_back(pLayout->vk_layout());
		}

		VkPipelineLayoutCreateInfo layoutCreateInfo = {};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutCreateInfo.setLayoutCount = descriptorSetLayouts.size();
		layoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
		layoutCreateInfo.pPushConstantRanges = nullptr;
		layoutCreateInfo.pushConstantRangeCount = 0;
		layoutCreateInfo.pNext = nullptr;

		DEV_ASSERT(vkCreatePipelineLayout(pDevice->GetVkLogicalDevice(), &layoutCreateInfo, nullptr, &mLayout) == VK_SUCCESS, "VulkanPipeline", "Failed to create pipeline layout!");

		//Create pipeine
		VkComputePipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineInfo.flags = VkPipelineCreateFlags();
		pipelineInfo.layout = mLayout;
		pipelineInfo.stage = vkShaderStageInfo;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateComputePipelines(mLogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline) == VK_SUCCESS, "VulkanPipeline", "Failed to create compute pipeline!");
	}

	Dream::VulkanPipeline::~VulkanPipeline()
	{
		vkDestroyPipelineLayout(mLogicalDevice, mLayout, nullptr);
		vkDestroyPipeline(mLogicalDevice, mPipeline, nullptr);
	}
}