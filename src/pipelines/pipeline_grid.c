#include "pipeline.h"
#include "../shader.h"
#include "../config.h"


void Pipeline_Create_Grid(struct Mess * ref){

	PRINT_FNAME;

	ref->pipeline_grid.vertexShader =
		Shader_CreateFromFile(ref->ref_device->logical_device, GRID_SHADER_VERT);
	ref->pipeline_grid.fragmentShader =
		Shader_CreateFromFile(ref->ref_device->logical_device, GRID_SHADER_FRAG);

	// struct DescriptorContext* ctx = info->ref_descriptor;

	VkDescriptorSetLayout layouts[] = {
		ref->Layout.globalLayout, 
	};

	// pipe_info.descriptorSetLayouts = layouts;
	// pipe_info.descriptorSetLayoutCount = ARR_LEN(layouts);


	VkVertexInputAttributeDescription attributes []={
		
	// 	(VkVertexInputAttributeDescription){
	// 		0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(struct Vertex, pos)},
		
	// (VkVertexInputAttributeDescription){
	// 		1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(struct Vertex, norm)},
		
	// 	(VkVertexInputAttributeDescription){
	// 		2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(struct Vertex, texCoords)},
	};
	

	

	VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = ref->pipeline_grid.vertexShader,
		.pName = "main",

	};

	VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = ref->pipeline_grid.fragmentShader,
		.pName = "main",

	};

	// uint32_t dynamicStateCount = 2;

	/*
	INFO
			set during command buffer recording part
			vkCmdSetViewport
	*/
	VkDynamicState dynamicState[] = {
		VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = ARR_LEN(dynamicState),
		.pDynamicStates = dynamicState,

	};

	VkPipelineShaderStageCreateInfo shaderStageCreateInf[] = {
		shaderStageCreateInfoVert, 
		shaderStageCreateInfoFrag
	};

	uint32_t shaderStageCreateInfCnt = ARR_LEN(shaderStageCreateInf);

	VkVertexInputBindingDescription vertexInputBindingDescription = {
		.binding = 0,
		.stride = sizeof(struct Vertex),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};



	VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 0,
		.pVertexBindingDescriptions = &vertexInputBindingDescription,
		.vertexAttributeDescriptionCount = ARR_LEN(attributes),
		.pVertexAttributeDescriptions = attributes,

	};

	VkPipelineInputAssemblyStateCreateInfo
		pipelineInputAssemblyStateCreateInfo = {
			.sType =
				VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		};

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pViewports = 0,
		.viewportCount = 1,
		.pScissors = 0,
		.scissorCount = 1,
	};

	VkPipelineRasterizationStateCreateInfo rasterizer = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		 // .cullMode = VK_CULL_MODE_BACK_BIT,
		.cullMode = VK_CULL_MODE_NONE,
	
	//	 .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		.frontFace= VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
		.depthBiasSlopeFactor = 1.0f,
		.lineWidth = 1.0f,

	};

	VkPipelineMultisampleStateCreateInfo multisampling = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.sampleShadingEnable = VK_FALSE,
	};

	VkPipelineDepthStencilStateCreateInfo depthStencil = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = VK_TRUE,
		// .depthWriteEnable = VK_TRUE,
		.depthWriteEnable = VK_FALSE,
		// .depthCompareOp = VK_COMPARE_OP_LESS,
			// .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
			//get's the previous depth?
		.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE,
	};

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState[] = {

		(VkPipelineColorBlendAttachmentState){
			.blendEnable = VK_TRUE,

			.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
				
			.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
		},

	};

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = ARR_LEN(colorBlendAttachmentState),
		.pAttachments = colorBlendAttachmentState,
	};

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount =ARR_LEN(layouts),
		.pSetLayouts = layouts,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = NULL};

	vkCreatePipelineLayout(
		ref->ref_device->logical_device, &pipelineLayoutCreateInfo, NULL, &ref->pipeline_grid.layout);

	VkFormat formats[] = {
		ref->swapchain.surfaceFormat
	};

	VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.colorAttachmentCount = ARR_LEN(formats),
		.pColorAttachmentFormats = formats,
		.depthAttachmentFormat = ref->swapchain.depthFormat,
	};

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = &pipelineRenderingCreateInfo,
		.stageCount = shaderStageCreateInfCnt,
		.pStages = shaderStageCreateInf,
		.pVertexInputState = &pipelineVertexInputStateCreateInfo,
		.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo,
		.pViewportState = &viewportStateCreateInfo,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampling,
		.pColorBlendState = &colorBlendStateCreateInfo,
		.pDynamicState = &dynamicStateCreateInfo,
		.layout = ref->pipeline_grid.layout,
		.renderPass = VK_NULL_HANDLE,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1,
		.pDepthStencilState = &depthStencil,

	};

	vkCreateGraphicsPipelines(
		ref->ref_device->logical_device, NULL, 1, &graphicsPipelineCreateInfo, NULL, &ref->pipeline_grid.handle);
	
}
