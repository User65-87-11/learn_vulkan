#include <vulkan/vulkan.h>
#include "shader_common.h" 
#include "descriptor.h"
#include "device.h"
#include "util/common.h"


// static VkDescriptorPool descriptor_pool = NULL; 

// static VkDescriptorSetLayout descriptor_layout_global; 
// static VkDescriptorSetLayout descriptor_layout_instance; 
// static VkDescriptorSetLayout descriptor_layout_material; 
// static VkDescriptorSetLayout descriptor_layout_sampler; 


static struct DescriptorContext context;


// static void createDescriptors(VkDevice device);


// lifecycle
void Descriptor_Init(){
	PRINT_FNAME;
	
  uint32_t cnt_glob = MAX_FRAMES_IN_FLIGHT;
  uint32_t cnt_inst = MAX_FRAMES_IN_FLIGHT;
  uint32_t cnt_material = MAX_MATERIALS;
  uint32_t cnt_textures = 1;
  uint32_t max_sets = cnt_inst + cnt_glob + cnt_material + cnt_textures;



  VkDevice device = Device_Get()->device;
  
  VkDescriptorPoolSize poolSizes[] = {
      (VkDescriptorPoolSize){
      	.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      	.descriptorCount = MAX_TEXTURES
      },
      (VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                             .descriptorCount = MAX_FRAMES_IN_FLIGHT},
      (VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                             .descriptorCount =
                                 MAX_FRAMES_IN_FLIGHT * MAX_INSTANCES +
                                 MAX_MATERIALS},

  };

  VkDescriptorPoolCreateInfo poolInfo = {

      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
      .pNext = NULL,
      // .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
      .maxSets = max_sets,
      .poolSizeCount = ARR_LEN(poolSizes),
      .pPoolSizes = poolSizes};

  VkResult result =
      vkCreateDescriptorPool(device, &poolInfo, NULL, &context.pool);

  {
    VkDescriptorSetLayoutBinding globalBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

    VkDescriptorSetLayoutCreateInfo globalLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &globalBinding};

    VkResult res = vkCreateDescriptorSetLayout(device, &globalLayoutInfo, NULL,
                                               &context.globalLayout);

    if (res != VK_SUCCESS) {
      EXIT_CLEAN("vkCreateDescriptorSetLayout4 failed");
    }
  }

  {
    VkDescriptorSetLayoutBinding materialBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorCount = 1,
        .stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

    VkDescriptorSetLayoutCreateInfo materialLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &materialBinding};

    VkResult res = vkCreateDescriptorSetLayout(
        device, &materialLayoutInfo, NULL, &context.materialLayout);

    if (res != VK_SUCCESS) {
      EXIT_CLEAN("vkCreateDescriptorSetLayout  materialDescriptorSetLayout");
    }
  }

  {
    VkDescriptorSetLayoutBinding instanceBindings = {

        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorCount = 1,
        .stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

    VkDescriptorSetLayoutCreateInfo instanceLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &instanceBindings};

    VkResult res = vkCreateDescriptorSetLayout(
        device, &instanceLayoutInfo, NULL, &context.instanceLayout);

    if (res != VK_SUCCESS) {
      EXIT_CLEAN("vkCreateDescriptorSetLayout instanceDescriptorSetLayout");
    }
  }

  {
    VkDescriptorSetLayoutBinding samplerBindings = {

        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = MAX_TEXTURES,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT};

    VkDescriptorSetLayoutCreateInfo samplerLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &samplerBindings};

    VkResult res = vkCreateDescriptorSetLayout(device, &samplerLayoutInfo, NULL,
                                               &context.samplerLayout);

    if (res != VK_SUCCESS) {
      EXIT_CLEAN("vkCreateDescriptorSetLayout samplerBindings");
    }
  }

  
	
}

void Descriptor_Shutdown(){
	VkDevice device = Device_Get()->device;
	
	 vkDestroyDescriptorPool(device, context.pool, NULL);
	
	vkDestroyDescriptorSetLayout(device, context.globalLayout, NULL);
	vkDestroyDescriptorSetLayout(device, context.instanceLayout, NULL);
	vkDestroyDescriptorSetLayout(device, context.materialLayout, NULL);
	vkDestroyDescriptorSetLayout(device, context.samplerLayout, NULL);
}

// pool control
void Descriptor_ResetPool(){
	VkDevice device = Device_Get()->device;
	 vkResetDescriptorPool(device, context.pool, 0);
}

// layouts access
struct DescriptorContext* Descriptor_GetContext(){
	return &context;
}


VkDescriptorSet Descriptor_Allocate( VkDescriptorSetLayout layout)
{
	VkDevice device = Device_Get()->device;
    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = context.pool,
        .descriptorSetCount = 1,
        .pSetLayouts = &layout
    };

    VkDescriptorSet set;
    vkAllocateDescriptorSets(device, &allocInfo, &set);
    return set;
}


// updates
void Descriptor_UpdateBuffer(

    VkDescriptorSet set,
    uint32_t binding,
    VkDescriptorType type,
    VkBuffer buffer,
    VkDeviceSize size
);

void Descriptor_UpdateImage(

    VkDescriptorSet set,
    uint32_t binding,
    VkDescriptorImageInfo* imageInfo
);







// static void createDescriptors(VkDevice device) {
//   PRINT_FNAME;
//   uint32_t cnt_inst = MAX_FRAMES_IN_FLIGHT;
//   uint32_t cnt_glob = MAX_FRAMES_IN_FLIGHT;
//   uint32_t cnt_material = 1;
//   uint32_t cnt_textures = 1;
//   uint32_t max_sets = cnt_inst + cnt_glob + cnt_material + cnt_textures;

//   // VkDevice device = Device_Get();
  
//   VkDescriptorPoolSize poolSizes[] = {
//       (VkDescriptorPoolSize){
//       	.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//       	.descriptorCount = MAX_TEXTURES
//       },
//       (VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//                              .descriptorCount = MAX_FRAMES_IN_FLIGHT},
//       (VkDescriptorPoolSize){.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
//                              .descriptorCount =
//                                  MAX_FRAMES_IN_FLIGHT * MAX_INSTANCES +
//                                  MAX_MATERIALS},

//   };

//   VkDescriptorPoolCreateInfo poolInfo = {

//       .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
//       .pNext = NULL,
//       .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
//       .maxSets = max_sets,
//       .poolSizeCount = ARR_LEN(poolSizes),
//       .pPoolSizes = poolSizes};

//   VkResult result =
//       vkCreateDescriptorPool(device, &poolInfo, NULL, &descriptor_pool);

//   {
//     VkDescriptorSetLayoutBinding globalBinding = {
//         .binding = 0,
//         .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//         .descriptorCount = 1,
//         .stageFlags =
//             VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

//     VkDescriptorSetLayoutCreateInfo globalLayoutInfo = {
//         .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
//         .bindingCount = 1,
//         .pBindings = &globalBinding};

//     VkResult res = vkCreateDescriptorSetLayout(device, &globalLayoutInfo, NULL,
//                                                &descriptor_layout_global);

//     if (res != VK_SUCCESS) {
//       EXIT_CLEAN("vkCreateDescriptorSetLayout4 failed");
//     }
//   }

//   {
//     VkDescriptorSetLayoutBinding materialBinding = {
//         .binding = 0,
//         .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
//         .descriptorCount = 1,
//         .stageFlags =
//             VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

//     VkDescriptorSetLayoutCreateInfo materialLayoutInfo = {
//         .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
//         .bindingCount = 1,
//         .pBindings = &materialBinding};

//     VkResult res = vkCreateDescriptorSetLayout(
//         device, &materialLayoutInfo, NULL, &descriptor_layout_material);

//     if (res != VK_SUCCESS) {
//       EXIT_CLEAN("vkCreateDescriptorSetLayout  materialDescriptorSetLayout");
//     }
//   }

//   {
//     VkDescriptorSetLayoutBinding instanceBindings = {

//         .binding = 0,
//         .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
//         .descriptorCount = 1,
//         .stageFlags =
//             VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

//     VkDescriptorSetLayoutCreateInfo instanceLayoutInfo = {
//         .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
//         .bindingCount = 1,
//         .pBindings = &instanceBindings};

//     VkResult res = vkCreateDescriptorSetLayout(
//         device, &instanceLayoutInfo, NULL, &descriptor_layout_instance);

//     if (res != VK_SUCCESS) {
//       EXIT_CLEAN("vkCreateDescriptorSetLayout instanceDescriptorSetLayout");
//     }
//   }

//   {
//     VkDescriptorSetLayoutBinding samplerBindings = {

//         .binding = 0,
//         .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//         .descriptorCount = MAX_TEXTURES,
//         .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT};

//     VkDescriptorSetLayoutCreateInfo samplerLayoutInfo = {
//         .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
//         .bindingCount = 1,
//         .pBindings = &samplerBindings};

//     VkResult res = vkCreateDescriptorSetLayout(device, &samplerLayoutInfo, NULL,
//                                                &descriptor_layout_sampler);

//     if (res != VK_SUCCESS) {
//       EXIT_CLEAN("vkCreateDescriptorSetLayout instanceDescriptorSetLayout");
//     }
//   }

//   {
//     VkDescriptorSetLayout layouts[MAX_FRAMES_IN_FLIGHT];

//     for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//       layouts[i] = descriptor_layout_global;
//     }

//     VkDescriptorSetAllocateInfo allocInfo = {
//         .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
//         .descriptorPool = descriptor_pool,
//         .descriptorSetCount = MAX_FRAMES_IN_FLIGHT,
//         .pSetLayouts = layouts};

//     vkAllocateDescriptorSets(device, &allocInfo, descriptor_set_globals);
//   }

//   {
//     VkDescriptorSetLayout layouts[MAX_FRAMES_IN_FLIGHT];

//     for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//       layouts[i] = descriptor_layout_instance;
//     }

//     VkDescriptorSetAllocateInfo allocInfo = {
//         .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
//         .descriptorPool = descriptor_pool,
//         .descriptorSetCount = MAX_FRAMES_IN_FLIGHT,
//         .pSetLayouts = layouts};

//     vkAllocateDescriptorSets(device, &allocInfo, descriptor_set_instances);
//   }

//   {

//     VkDescriptorSetAllocateInfo allocInfo = {
//         .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
//         .descriptorPool = descriptor_pool,
//         .descriptorSetCount = 1,
//         .pSetLayouts = &descriptor_layout_material};

//     vkAllocateDescriptorSets(device, &allocInfo, &descriptor_set_materials);
//   }

//   {

//     VkDescriptorSetAllocateInfo allocInfo = {
//         .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
//         .descriptorPool = descriptor_pool,
//         .descriptorSetCount = 1,
//         .pSetLayouts = &descriptor_layout_sampler};

//     vkAllocateDescriptorSets(device, &allocInfo, &descriptor_set_samplers);
//   }
// }

