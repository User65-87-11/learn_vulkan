#include "pipeline.h"
#include "device.h"
#include "vertex.h"
#include "util/common.h"
#include "shader_common.h"


// static VkFormat findDepthFormat();


void Pipeline_CreateGraphics(
   struct  GraphicsPipelineCreateInfo* info,
   struct GraphicsPipeline* pipeline
   
)
{
		
	PRINT_FNAME;
	VkDevice device = Device_Get()->logical_device;
	
	VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert = {
	    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
	    .stage = VK_SHADER_STAGE_VERTEX_BIT,
	    .module = info->vertexShader,
	    .pName = "main",
	
	};

  VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = info->fragmentShader,
      .pName = "main",

  };

  // uint32_t dynamicStateCount = 2;

  /*
  INFO
          set during command buffer recording part
          vkCmdSetViewport
  */
  VkDynamicState dynamicState[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                   VK_DYNAMIC_STATE_SCISSOR};

  VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
      .dynamicStateCount = ARR_LEN(dynamicState),
      .pDynamicStates = dynamicState,

  };

  VkPipelineShaderStageCreateInfo shaderStageCreateInf[] = {
      shaderStageCreateInfoVert, shaderStageCreateInfoFrag
  };

  uint32_t shaderStageCreateInfCnt = ARR_LEN(shaderStageCreateInf);

  VkVertexInputBindingDescription vertexInputBindingDescription = {
      .binding = 0,
      .stride = sizeof(struct Vertex),
      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
  };

  VkVertexInputAttributeDescription vertexInputAttributeDescriptions[3];

  vertexInputAttributeDescriptions[0] = (VkVertexInputAttributeDescription)
  {
      0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(struct Vertex, pos)
  };
  vertexInputAttributeDescriptions[1] = (VkVertexInputAttributeDescription)
  {
      1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(struct Vertex, norm)
  };
  vertexInputAttributeDescriptions[2] = (VkVertexInputAttributeDescription)
  {
      2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(struct Vertex, texCoords)
  };

  uint32_t vertexInputAttributeDescriptionsCount = ARR_LEN(vertexInputAttributeDescriptions);
      // sizeof(vertexInputAttributeDescriptions) /
      // (sizeof(VkVertexInputAttributeDescription));

  VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .vertexBindingDescriptionCount = 1,
      .pVertexBindingDescriptions = &vertexInputBindingDescription,
      .vertexAttributeDescriptionCount = vertexInputAttributeDescriptionsCount,
      .pVertexAttributeDescriptions = vertexInputAttributeDescriptions,

  };

  VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo =
      {
          .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
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
      .cullMode = VK_CULL_MODE_BACK_BIT,
      .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
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
      .depthWriteEnable = VK_TRUE,
      .depthCompareOp = VK_COMPARE_OP_LESS,
      .depthBoundsTestEnable = VK_FALSE,
      .stencilTestEnable = VK_FALSE,
  };

  VkPipelineColorBlendAttachmentState colorBlendAttachmentState[] = {

      (VkPipelineColorBlendAttachmentState){
          .blendEnable = VK_FALSE,

          .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                            VK_COLOR_COMPONENT_G_BIT |
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
      .setLayoutCount = info->descriptorSetLayoutCount,
      .pSetLayouts = info->descriptorSetLayouts,
      .pushConstantRangeCount = 0,
      .pPushConstantRanges = NULL
  };

	vkCreatePipelineLayout(
		device, 
		&pipelineLayoutCreateInfo, 
		NULL,
		&pipeline->layout
	);

	VkFormat formats[]={
		info->colorFormat,
	};
  
  
  VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
      .colorAttachmentCount = ARR_LEN(formats),
      .pColorAttachmentFormats = formats,
      .depthAttachmentFormat =info->depthFormat,
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
      .layout = pipeline->layout,
      .renderPass = VK_NULL_HANDLE,
      .basePipelineHandle = VK_NULL_HANDLE,
      .basePipelineIndex = -1,
      .pDepthStencilState = &depthStencil,

  };

  
  vkCreateGraphicsPipelines(device, NULL, 1, &graphicsPipelineCreateInfo, NULL, &pipeline->handle);
}




