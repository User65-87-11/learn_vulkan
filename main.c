



// 

#include <vulkan/vulkan_core.h>




#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "cglm/cglm.h"
#include "cglm/cam.h"
#include "cglm/types.h"
#include "cglm/mat4.h"
#include "cglm/util.h"
#include "cglm/affine-pre.h"

// #define TINYOBJ_LOADER_C_IMPLEMENTATION
// #include "obj_loader/tinyobj_loader_c.h"

#define CGLTF_IMPLEMENTATION 
#include "cgltf/cgltf.h"



#include <math.h>


/**

includes

*/




const uint32_t WIDTH = 800;

const uint32_t HEIGHT = 600;

#define PRINT_FNAME printf("Call to: %s\n",__FUNCTION__)

#define ARR_LEN(A) sizeof(A)/sizeof(*A)

#define EXIT_CLEAN(msg){\
		printf("ERROR: %s\n",msg);\
		cleanup();\
		exit(1);\
	}while(0)\

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#define MAX_IMAGE_VIEWS 16

#define MAX_FRAMES_IN_FLIGHT 2


const uint32_t validationLayerCount = 1;

const char* validationLayers[] = {
        "VK_LAYER_KHRONOS_validation"
};

const uint32_t requiredDeviceExtensionCount = 1;

const char* requiredDeviceExtensions[] = {

        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

bool framebufferResized = false;

GLFWwindow* window = NULL;

VkInstance instance = NULL;

VkDebugUtilsMessengerEXT debugUtilsMessengerEXT = NULL;

VkPhysicalDevice physicalDevice = NULL;

VkDevice device = NULL;

VkQueue graphicsQueue = NULL;

VkQueue transferQueue = NULL;

uint32_t queueFamilyIndexCount = 2;

uint32_t graphicsQueueFamilyArrayIndex = 0;

uint32_t transferQueueFamilyArrayIndex = 1;

uint32_t queueFamilyIndeces[2] = {
	-1,-1,
};

uint32_t presentationSupportQueueFamilyIndex = - 1;

VkSurfaceKHR surface = NULL; 

VkSurfaceCapabilitiesKHR surfaceCapabilities ;

VkExtent2D swapChainExtent ;

uint32_t swapchainSurfaceFormat = -1;

uint32_t swapchainSurfaceColorSpace = -1;

VkSwapchainKHR swapchain = NULL;

uint32_t swapchainImageCount = 0 ;

VkImage swapchainImages[MAX_IMAGE_VIEWS];

uint32_t swapchainImageViewCount = 0;

VkImageView swapchainImageViews[MAX_IMAGE_VIEWS];

VkShaderModule shaderModuleFrag = NULL;

VkShaderModule shaderModuleVert = NULL;

VkDescriptorSetLayout descriptorSetLayout = NULL;

VkPipelineLayout pipelineLayout = NULL;

VkPipeline graphicsPipeline = NULL;

uint32_t commandBufferCount = MAX_FRAMES_IN_FLIGHT;

uint32_t frameIndex = 0;

VkCommandBuffer graphicsCommandBuffers[MAX_FRAMES_IN_FLIGHT];

uint32_t transferCommandBuffersCount = 1;

VkCommandBuffer transferCommandBuffers ;

VkCommandPool graphicsCommnadPool = NULL;

VkCommandPool transferCommnadPool = NULL;

VkSemaphore presentCompleteSemaphore[MAX_FRAMES_IN_FLIGHT];

VkSemaphore renderFinishedSemaphore[MAX_FRAMES_IN_FLIGHT];

VkFence inFlightFences [MAX_FRAMES_IN_FLIGHT];

VkRenderPass renderPass = NULL;

VkBuffer vertextBuffer = NULL;

VkDeviceMemory vertexBufferMemory = NULL;

VkBuffer indexBuffer = NULL;

VkDeviceMemory indexBufferMemory = NULL;

VkBuffer uniformBuffers[MAX_FRAMES_IN_FLIGHT] = {NULL,NULL};

VkDeviceMemory uniformBuffersMemory[MAX_FRAMES_IN_FLIGHT]= {NULL,NULL};

void * uniformBuffersMapped[MAX_FRAMES_IN_FLIGHT]= {NULL,NULL};

VkDescriptorPool descriptorPool = NULL;

VkDescriptorSet descriptorSets [MAX_FRAMES_IN_FLIGHT];

VkImage textureImage = NULL;

VkDeviceMemory textureImageMemory = NULL;

VkImageView textureImageView = NULL;

VkSampler textureSampler = NULL;

VkImage depthImage = NULL;

VkDeviceMemory depthImageMemory = NULL;

VkImageView depthImageView = NULL;


uint32_t mipLevels;
VkImage* textureImageMip;

// VkImageLayout currentSwapchainLayouts[MAX_IMAGE_VIEWS];

//--------------------------


struct Vertex{

	vec3 pos;
	vec3 col;
	vec2 texCoords;
};
//alignas(16)?

void vertexGetBindingDescription(VkVertexInputBindingDescription *bindingDescription){

	/**
	    uint32_t             binding;
   	 	uint32_t             stride;
    	VkVertexInputRate    inputRate;
	*/
	*bindingDescription = (VkVertexInputBindingDescription){
		.binding = 0,
		.stride = sizeof(struct Vertex),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};
 
}
void vertexGetAttributeDescriptions(VkVertexInputAttributeDescription bindingDescription[3]){

 

	bindingDescription[0] = (VkVertexInputAttributeDescription){0,0,VK_FORMAT_R32G32B32_SFLOAT,  offsetof(struct Vertex, pos)};
	bindingDescription[1] = (VkVertexInputAttributeDescription){1,0,VK_FORMAT_R32G32B32_SFLOAT,  offsetof(struct Vertex, col)};
	bindingDescription[2] = (VkVertexInputAttributeDescription){2,0,VK_FORMAT_R32G32_SFLOAT,  offsetof(struct Vertex, texCoords)};
 
 
}





// struct Vertex vertices[] = {
// 	{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
//     {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
//     {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}
// };


// UP TO DAETH BUFFERING
	// uint16_t indices[] = {
	// 	0, 1, 2, 2, 3, 0,
	// 	4, 5, 6, 6, 7, 4
	// };

	// struct Vertex vertices[] = {
	// 	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	// 	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	// 	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	// 	{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

	// 	{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	// 	{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	// 	{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	// 	{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	// };
//END UP TO DAETH BUFFERING


char * model_path_gltf	= 	"models_gltf/viking_room3.gltf";
char * model_path_bin	= 	"models_gltf/viking_room3.bin";
char * model_path_text	= 	"models_gltf/viking_room.png";


uint32_t indicesNum = 0;
uint32_t* indices = NULL;

uint32_t verticesNum = 0;
struct Vertex* vertices = NULL;





/**

Scalars have to be aligned by N (= 4 bytes given 32-bit floats).

A float2 must be aligned by 2N (= 8 bytes)

A float3 or float4 must be aligned by 4N (= 16 bytes)

A nested structure must be aligned by the base alignment of its members rounded up to a multiple of 16.

A float4x4 matrix must have the same alignment as a float4.
*/

struct UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
};

double  start_time = 0.0f;


/**

                                                                                                                                                                         
                                           bbbbbbbb                                                                                                              dddddddd
                   lllllll                 b::::::b                              lllllll                                                                         d::::::d
                   l:::::l                 b::::::b                              l:::::l                                                                         d::::::d
                   l:::::l                 b::::::b                              l:::::l                                                                         d::::::d
                   l:::::l                  b:::::b                              l:::::l                                                                         d:::::d 
   ggggggggg   gggggl::::l    ooooooooooo   b:::::bbbbbbbbb      aaaaaaaaaaaaa    l::::l     ssssssssss            eeeeeeeeeeee    nnnn  nnnnnnnn        ddddddddd:::::d 
  g:::::::::ggg::::gl::::l  oo:::::::::::oo b::::::::::::::bb    a::::::::::::a   l::::l   ss::::::::::s         ee::::::::::::ee  n:::nn::::::::nn    dd::::::::::::::d 
 g:::::::::::::::::gl::::l o:::::::::::::::ob::::::::::::::::b   aaaaaaaaa:::::a  l::::l ss:::::::::::::s       e::::::eeeee:::::een::::::::::::::nn  d::::::::::::::::d 
g::::::ggggg::::::ggl::::l o:::::ooooo:::::ob:::::bbbbb:::::::b           a::::a  l::::l s::::::ssss:::::s     e::::::e     e:::::enn:::::::::::::::nd:::::::ddddd:::::d 
g:::::g     g:::::g l::::l o::::o     o::::ob:::::b    b::::::b    aaaaaaa:::::a  l::::l  s:::::s  ssssss      e:::::::eeeee::::::e  n:::::nnnn:::::nd::::::d    d:::::d 
g:::::g     g:::::g l::::l o::::o     o::::ob:::::b     b:::::b  aa::::::::::::a  l::::l    s::::::s           e:::::::::::::::::e   n::::n    n::::nd:::::d     d:::::d 
g:::::g     g:::::g l::::l o::::o     o::::ob:::::b     b:::::b a::::aaaa::::::a  l::::l       s::::::s        e::::::eeeeeeeeeee    n::::n    n::::nd:::::d     d:::::d 
g::::::g    g:::::g l::::l o::::o     o::::ob:::::b     b:::::ba::::a    a:::::a  l::::l ssssss   s:::::s      e:::::::e             n::::n    n::::nd:::::d     d:::::d 
g:::::::ggggg:::::gl::::::lo:::::ooooo:::::ob:::::bbbbbb::::::ba::::a    a:::::a l::::::ls:::::ssss::::::s     e::::::::e            n::::n    n::::nd::::::ddddd::::::dd
 g::::::::::::::::gl::::::lo:::::::::::::::ob::::::::::::::::b a:::::aaaa::::::a l::::::ls::::::::::::::s       e::::::::eeeeeeee    n::::n    n::::n d:::::::::::::::::d
  gg::::::::::::::gl::::::l oo:::::::::::oo b:::::::::::::::b   a::::::::::aa:::al::::::l s:::::::::::ss         ee:::::::::::::e    n::::n    n::::n  d:::::::::ddd::::d
    gggggggg::::::gllllllll   ooooooooooo   bbbbbbbbbbbbbbbb     aaaaaaaaaa  aaaallllllll  sssssssssss             eeeeeeeeeeeeee    nnnnnn    nnnnnn   ddddddddd   ddddd
            g:::::g                                                                                                                                                      
gggggg      g:::::g                                                                                                                                                      
g:::::gg   gg:::::g                                                                                                                                                      
 g::::::ggg:::::::g                                                                                                                                                      
  gg:::::::::::::g                                                                                                                                                       
    ggg::::::ggg                                                                                                                                                         
       gggggg                                                                                                                                                                                                                                  
*/



uint32_t createShaderFromFile(const char * path, uint8_t** buffer);

void loadModel(char *fname) ;

void createUniformBuffers();

void createBuffer(
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkBuffer *buffer,
	VkDeviceMemory *bufferMemory);

uint32_t findMemoryType(
	uint32_t typeFilter, 
	VkMemoryPropertyFlags properties);

void recreateSwapChain();

void cleanup();

void createVertexBuffer();

void createIndexBuffer();

void clearUniformBuffers();

void createDescriptorSetLayout();

void createDescriptorSets();

void createTextureImage(char * path);

void createTextureImageView();


void generateMipmaps(VkImage* image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) ;

void createImage(
	uint32_t width, 
	uint32_t height,
	uint32_t mipLevels,
	VkFormat format, 
	VkImageTiling tiling, 
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties, 
	VkImage *image, 
	VkDeviceMemory * imageMemory
);

void beginSingleTimeCommands(VkCommandBuffer *commandBuffer);

void endSingleTimeCommands(VkCommandBuffer *commandBuffer);

// void transitionImageLayout(VkImage *image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlagBits aspectFlags);

void transitionImageLayout(
	VkImage *image, 

	VkImageLayout oldLayout, 
	VkImageLayout newLayout, 

	VkAccessFlags srcAccessMask,
	VkAccessFlags dstAccessMask,

	VkPipelineStageFlags srcStageMask,
	VkPipelineStageFlags dstStageMask,

	VkImageAspectFlagBits aspectFlags,

	uint32_t mipLevels
);

void copyBufferToImage(VkBuffer *buffer, VkImage *image, uint32_t width, uint32_t height);

void createImageView(VkImageView *imageView, VkImage* image, VkFormat format, VkImageAspectFlagBits aspectFlags,uint32_t mipLevels);

void createTextureSampler();

void createDepthResources();

VkFormat findSupportedFormat(VkFormat *formats, uint32_t len, VkImageTiling tiling, VkFormatFeatureFlags features);

VkFormat findDepthFormat();

bool hasStencilComponent(VkFormat format);

void appendstr(void * dst, char* src);
/*


                                                                                     
88                                            88                                     
88                                            88                                     
88                                            88                                     
88,dPPYba,    ,adPPYba,  ,adPPYYba,   ,adPPYb,88   ,adPPYba,  8b,dPPYba,  ,adPPYba,  
88P'    "8a  a8P_____88  ""     `Y8  a8"    `Y88  a8P_____88  88P'   "Y8  I8[    ""  
88       88  8PP"""""""  ,adPPPPP88  8b       88  8PP"""""""  88           `"Y8ba,   
88       88  "8b,   ,aa  88,    ,88  "8a,   ,d88  "8b,   ,aa  88          aa    ]8I  
88       88   `"Ybbd8"'  `"8bbdP"Y8   `"8bbdP"Y8   `"Ybbd8"'  88          `"YbbdP"'  
                                                                                     
                                                                                     

*/

void appendstr(void * dst, char* src){
	char * last = dst;
	char * beg = src;
	while(*last != 0)
	{
		last ++;
	}
	while(*beg != 0){
		
		*last = *beg;
		last ++;
		beg ++;
	}
	*last = 0;
}


void loadModel(char *fname){
	PRINT_FNAME;

	printf("load model: %s\n",fname);
	/*
	cgltf_load_buffers(const cgltf_options *options, cgltf_data *data, const char *gltf_path) -> cgltf_result
	
	*/
	const cgltf_options options={};
	cgltf_data *data;
	uint32_t data_size;
	const char * gltf_path = fname;
	
	cgltf_result res;

	/**
cgltf_result cgltf_parse_file(const cgltf_options* options, const
 * char* path, cgltf_data** out_data)
  */

  	res = cgltf_parse_file(&options,fname,&data);


	// res = cgltf_load_buffers(&options,data,gltf_path);	

	if(res != cgltf_result_success)
	{
		printf("res %d\n",res);
		EXIT_CLEAN("Cant load buffers :(");
	}

	for(int i=0;i<data->buffers_count;i++){
		printf("Buffer:%s\n",data->buffers[i].uri);
		printf("Buffer.size:%ld\n",data->buffers[i].size);
		printf("Buffer.name:%s\n",data->buffers[i].name);
		printf("Buffer.uri:%s\n",data->buffers[i].uri);
		printf("Buffer.data:%p\n",data->buffers[i].data);
	}

	/*
	typedef enum cgltf_buffer_view_type
{
	cgltf_buffer_view_type_invalid,
	cgltf_buffer_view_type_indices,
	cgltf_buffer_view_type_vertices,
	cgltf_buffer_view_type_max_enum
} cgltf_buffer_view_type;
	*/


	// for(int i=0;i<data->buffer_views_count;i++){
	// 	printf("Buffer views:");
	// 	printf(" .buffer ptr:%p\n",data->buffer_views[i].buffer);
	// 	printf(" .data:%p\n",data->buffer_views[i].data);
	// 	printf(" .type:%d\n",data->buffer_views[i].type);
	// 	printf(" .size:%ld\n",data->buffer_views[i].size);
	// 	printf(" .offset:%ld\n",data->buffer_views[i].offset);
	// 	printf(" .stride:%ld\n",data->buffer_views[i].stride);
	// 	printf(" .data:%p\n",data->buffer_views[i].data);
	// }
	
	for(int i=0;i<data->meshes_count;i++){
	
		printf("Meshes :");
		printf(" .name:%s\n",data->meshes[i].name);
	
		printf(" .primitives_count:%ld\n",data->meshes[i].primitives_count);
		

		for(int j=0;j<data->meshes[i].primitives_count;j++){

			printf(" Primitives :\n");
			printf("  .attributes_count:%ld\n",data->meshes[i].primitives[j].attributes_count);
			printf("  .material:%p\n",data->meshes[i].primitives[j].material);

			//prim->indices → pointer to a cgltf_accessor
			printf("  .indices:%p\n",data->meshes[i].primitives[j].indices);

			if(data->meshes[i].primitives[j].indices)
			{
				cgltf_accessor * accessor = data->meshes[i].primitives[j].indices;
				printf("    .cnt:%ld\n",accessor->count);
				printf("    .type:%d\n",accessor->type);
				printf("    .buffer offset:%ld\n",accessor->buffer_view->offset);
				printf("    .offset:%ld\n",accessor->offset);
				printf("    .stride:%ld\n",accessor->stride);
			}

			for(int k=0;k<data->meshes[i].primitives[j].attributes_count;k++)
			{
				printf("  Attributes :");
				printf("   .name:%s\n",data->meshes[i].primitives[j].attributes[k].name);
				printf("   .data:%p\n",data->meshes[i].primitives[j].attributes[k].data);
				printf("   .type:%d\n",data->meshes[i].primitives[j].attributes[k].type);
				printf("   .index:%d\n",data->meshes[i].primitives[j].attributes[k].index);

				cgltf_accessor * accessor = data->meshes[i].primitives[j].attributes[k].data;
				cgltf_attribute_type attribType =  data->meshes[i].primitives[j].attributes[k].type;

				if(attribType == cgltf_attribute_type_position)
				{
					printf("  .vertex position\n");
					printf("    .count:%ld\n",accessor->count);
					printf("    .buffer_view->buffer:%p\n",accessor->buffer_view->buffer);
					printf("    .buffer_view->offset:%ld\n",accessor->buffer_view->offset);
					printf("    .accessor->offset:%ld\n",accessor->offset);
					printf("    .stride:%ld\n",accessor->stride);
					printf("    .cgltf_type:%d\n",accessor->type);
				}

				if(attribType == cgltf_attribute_type_texcoord)
				{
					printf("  .tex coords\n");
					printf("    .count:%ld\n",accessor->count);
					printf("    .buffer_view->buffer:%p\n",accessor->buffer_view->buffer);
					printf("    .buffer_view->offset:%ld\n",accessor->buffer_view->offset);
					printf("    .accessor->offset:%ld\n",accessor->offset);
					printf("    .stride:%ld\n",accessor->stride);
					printf("    .cgltf_type:%d\n",accessor->type);
				}

				
			}
			printf(" .primitives_count:%ld\n",data->meshes[i].primitives_count);
		}
		break;
	}

	if(data->buffers_count > 0){

		const char url_path[256] = "models_gltf/";
		printf("Buffer.path:%s\n",url_path);
		appendstr((char *)url_path, data->buffers[0].uri);

		printf("Buffer.path:%s\n",url_path);
		 
		cgltf_load_buffers(&options,data,url_path);

		printf("Buffer.data:%p\n",data->buffers[0].data);
		printf("Buffer.size:%ld\n",data->buffers[0].size);
		
	}
	
	printf("data.data_extensions_count. %ld\n",data->data_extensions_count);
	printf("data.buffer_views_count %ld\n",data->buffer_views_count);
	printf("data.animations_count %ld\n",data->animations_count);
	printf("data.images_count %ld\n",data->images_count);
	printf("data.materials_count %ld\n",data->materials_count);
	printf("data.nodes_count %ld\n",data->nodes_count);
	printf("data.buffers_count %ld\n",data->buffers_count);
	printf("data.lights_count %ld\n",data->lights_count);
	printf("data.meshes_count %ld\n",data->meshes_count);
 


	//LOAD MODEL

	/**
	typedef enum cgltf_attribute_type
{
	cgltf_attribute_type_invalid,
	cgltf_attribute_type_position,
	cgltf_attribute_type_normal,
	cgltf_attribute_type_tangent,
	cgltf_attribute_type_texcoord,
	cgltf_attribute_type_color,
	cgltf_attribute_type_joints,
	cgltf_attribute_type_weights,
	cgltf_attribute_type_custom,
	cgltf_attribute_type_max_enum
} cgltf_attribute_type;

typedef enum cgltf_type
{
	cgltf_type_invalid,
	cgltf_type_scalar,
	cgltf_type_vec2,
	cgltf_type_vec3,
	cgltf_type_vec4,
	cgltf_type_mat2,
	cgltf_type_mat3,
	cgltf_type_mat4,
	cgltf_type_max_enum
} cgltf_type;

	*/
	cgltf_primitive *primitive = &data->meshes[0].primitives[0];
	 

	cgltf_accessor *acc_idx = primitive->indices;
	cgltf_accessor *acc_vert = NULL; 
 	
	for(int i=0;i<primitive->attributes_count;i++){

		cgltf_attribute attr =  primitive->attributes[i];
		if(attr.type == cgltf_attribute_type_position)
		{

			cgltf_accessor* accessor = attr.data;
		
			cgltf_buffer_view* view = accessor->buffer_view;

			uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;
			size_t stride = accessor->stride ? accessor->stride : sizeof(float) * 3;

			verticesNum = accessor->count;
			vertices = malloc(sizeof(struct Vertex) * verticesNum);

			printf("verticesNum: %d\n",verticesNum);
			printf("stride: %ld\n",stride);
		 
			for (size_t d = 0; d < verticesNum; d++) {
				float* data = (float*)(base + d * stride);

			
				vertices[d].pos[0] = data[0];
				vertices[d].pos[1] = data[1];
				vertices[d].pos[2] = data[2];

				
			 
				vertices[d].col[0] = 1.0f;
				vertices[d].col[1] = 1.0f;
				vertices[d].col[2] = 0.5f;
			}

		 
			break;
		}

		
	}

 
	for(int i=0;i<primitive->attributes_count;i++){

		cgltf_attribute attr =  primitive->attributes[i];
		
		if(attr.type == cgltf_attribute_type_texcoord)
		{
			assert(attr.data->type == cgltf_type_vec2);
			assert(attr.data->component_type == cgltf_component_type_r_32f);

			cgltf_accessor* accessor = attr.data;
			cgltf_buffer_view* view = accessor->buffer_view;

			uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;
			size_t stride = accessor->stride ? accessor->stride : sizeof(float) * 2;

			// verticesNum = accessor->count;

			for (size_t d = 0; d < verticesNum; d++) {
				float* data = (float*)(base + d * stride);

				vertices[d].texCoords[0] = data[0];
				vertices[d].texCoords[1] = data[1];
			}
			break;
		}
	}

	if(primitive->indices)
	{
		int offset = primitive->indices->offset;
		
		// printf("primitive->indices->component_type %d\n",primitive->indices->component_type);
		// assert(primitive->indices->component_type == cgltf_component_type_r_16u);


		cgltf_accessor* accessor = primitive->indices;
		cgltf_buffer_view* view = accessor->buffer_view;


		uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;

		size_t stride = accessor->stride;

		if (stride == 0) {
			stride = cgltf_component_size(accessor->component_type);
		}

		indicesNum = primitive->indices->count;

		indices = malloc(sizeof(uint32_t)* indicesNum);

		for(int d=0;d<indicesNum;d++){

			uint8_t* ptr = base + d * stride;

			 switch (accessor->component_type) {
				case cgltf_component_type_r_16u:
					indices[d] = *(uint16_t*)ptr;
					break;
				case cgltf_component_type_r_32u:
					indices[d] = *(uint32_t*)ptr;
					break;
				case cgltf_component_type_r_8u:
					indices[d] = *(uint8_t*)ptr;
					break;
				default:{
						printf("datatype: %d\n",accessor->component_type);
						EXIT_CLEAN("UNSUPPORTED INDEX DATA TYPE");
					}
					break;
			}
			
		}
		
	}

 

	cgltf_free(data);
}




















bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat findDepthFormat(){

	VkFormat formats[]={
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT 
	};

	return findSupportedFormat(
		formats, 
		ARR_LEN(formats),
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}
VkFormat findSupportedFormat(VkFormat *formats,uint32_t len, VkImageTiling tiling, VkFormatFeatureFlags features ){

 
	for(int i=0;i<len;i++)
	{

		VkFormatProperties props;
		
		vkGetPhysicalDeviceFormatProperties(physicalDevice, formats[i], &props);
		
		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
		 
			return   formats[i];
		}
		if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {

		 
			return   formats[i];
		}
	}
	EXIT_CLEAN("failed to find supported format!");
	
}

void createDepthResources() {

	VkFormat depthFormat = findDepthFormat();

	createImage(
		swapChainExtent.width, 
		swapChainExtent.height, 
		1,
		depthFormat, 
		VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&depthImage, 
		&depthImageMemory
	);
	// depthImageView = 
	
	createImageView(&depthImageView,&depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,1);

	 transitionImageLayout(
        &depthImage,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
        0,
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT,
		1
    );

}

void createTextureSampler(){


	// vk::SamplerCreateInfo samplerInfo{.magFilter = vk::Filter::eLinear, .minFilter = vk::Filter::eLinear};

	VkPhysicalDeviceProperties physicalDeviceProperties ;

	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	VkSamplerCreateInfo samplerInfo = {
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,

		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.mipLodBias = 0.0f,
		.minLod = 0.0f,
		.maxLod = VK_LOD_CLAMP_NONE,
		// .maxLod = 0.0f,


		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy,
		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,

		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
	};

	vkCreateSampler(device, &samplerInfo, NULL, &textureSampler);

}

void createImageView(VkImageView *imageView, VkImage* image, VkFormat format, VkImageAspectFlagBits aspectFlags,uint32_t mipLevels){

		
		/*
		
		*/
		VkImageViewCreateInfo viewInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = *image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = format,
			.subresourceRange  = {
					
					.aspectMask = aspectFlags,
					.baseMipLevel = 0,
					.levelCount = mipLevels,
					.baseArrayLayer= 0,
					.layerCount =1,
			},
			
		};
		vkCreateImageView(device, &viewInfo,  NULL, imageView);
}

void createTextureImageView(){

	createImageView(&textureImageView,&textureImage,VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,mipLevels);

}

void copyBufferToImage(VkBuffer *buffer, VkImage *image, uint32_t width, uint32_t height) {
	PRINT_FNAME;
    VkCommandBuffer commandBuffer ;
	beginSingleTimeCommands(&commandBuffer);


	
	VkBufferImageCopy bufferImageCopy = {
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			0,0,1,
		},
		.imageOffset = { 
			0,0,0
		},
		.imageExtent = {width,height,1},
	};

	vkCmdCopyBufferToImage(commandBuffer, *buffer, *image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);
	

    endSingleTimeCommands(&commandBuffer);
	
}
void transitionImageLayout(
	VkImage *image, 
	
	VkImageLayout oldLayout, 
	VkImageLayout newLayout, 

	VkAccessFlags srcAccessMask,
	VkAccessFlags dstAccessMask,

	VkPipelineStageFlags srcStageMask,
	VkPipelineStageFlags dstStageMask,

	VkImageAspectFlagBits aspectFlags,
	uint32_t mipLevels
) {

	// PRINT_FNAME;
    VkCommandBuffer commandBuffer;
	beginSingleTimeCommands(&commandBuffer);


	VkImageMemoryBarrier imageMemoryBarriers = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.oldLayout = oldLayout,
		.newLayout = newLayout,

		.image = *image,
		.subresourceRange = {
			.aspectMask = aspectFlags,
            .baseMipLevel = 0,
            .levelCount = mipLevels,
            .baseArrayLayer = 0,
            .layerCount = 1
		},
		.srcAccessMask = srcAccessMask,
		.dstAccessMask = dstAccessMask,
	}; 



	vkCmdPipelineBarrier(
		commandBuffer, 
		srcStageMask, 
		dstStageMask, 
		0, 
		0, (VkMemoryBarrier* )NULL,
		0, (VkBufferMemoryBarrier*) NULL, 
		1, &imageMemoryBarriers);

    endSingleTimeCommands(&commandBuffer);
}

void  beginSingleTimeCommands(VkCommandBuffer *commandBuffer){

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = graphicsCommnadPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffer);

	VkCommandBufferBeginInfo beginInfo  = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};

	vkBeginCommandBuffer(*commandBuffer, &beginInfo);
}
void endSingleTimeCommands(VkCommandBuffer *commandBuffer){

	vkEndCommandBuffer(*commandBuffer);
	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = commandBuffer,
	};
	vkQueueSubmit(graphicsQueue, 1, &submitInfo, NULL);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, graphicsCommnadPool, 1, commandBuffer);
}

void createImage(
	uint32_t width, 
	uint32_t height, 
	uint32_t mipLevels,
	VkFormat format, 
	VkImageTiling tiling, 
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties, 
	VkImage* image, 
	VkDeviceMemory* imageMemory
) 
{

	
	VkImageCreateInfo imageCreateInfo ={
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		// .format = VK_FORMAT_R8G8B8A8_SRGB,
		.format  = format,
		// .extent = {imageExtent.width,imageExtent.height,1},
		.extent = {width,height,1},
		.mipLevels = mipLevels,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		// .tiling = VK_IMAGE_TILING_OPTIMAL,
		.tiling = tiling,
		// .usage = VK_IMAGE_USAGE_SAMPLED_BIT,
		.usage  = usage ,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		
	};

	vkCreateImage(device, &imageCreateInfo, NULL, image);
 

    VkMemoryRequirements memRequirements ;

	vkGetImageMemoryRequirements(device, *image, &memRequirements);
	
    VkMemoryAllocateInfo allocInfo ={
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties),
		
	};

	vkAllocateMemory(device, &allocInfo,  NULL, imageMemory);
	
 	vkBindImageMemory(device, *image, *imageMemory,0);
}

uint32_t getMipmapLevels(uint32_t w,uint32_t h){

	//mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
	uint32_t max = w > h? w: h;
	uint32_t d = floor(log2(max));
	d++;
	return  d;

}
void createTextureImage(char * path){
	/**
	Staging is the process of getting image data into the GPU’s memory.
	
	*/
	PRINT_FNAME;

	int texWidth, texHeight, texChannels;

	// stbi_set_flip_vertically_on_load(true);
	stbi_uc* pixels  = stbi_load(path,  &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	mipLevels  = getMipmapLevels(texWidth,texHeight);

	
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	if( ! pixels){
		EXIT_CLEAN("failed to load texture image!");
	}


	VkBuffer stagingBuffer;

	VkDeviceMemory stagingBufferMemory;

	createBuffer(
		imageSize, 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer,
		&stagingBufferMemory
	);
	
	void* data = NULL;

	vkMapMemory(device, 
		stagingBufferMemory,
		0, 
		imageSize, 
		0, 
		&data
	);
	memcpy(data, pixels, imageSize);

	vkUnmapMemory(device, stagingBufferMemory);
	
	stbi_image_free(pixels);

	
	createImage(
		texWidth, 
		texHeight, 
		mipLevels,
		VK_FORMAT_R8G8B8A8_SRGB, 
		VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		&textureImage, 
		&textureImageMemory
	);


	transitionImageLayout(
		&textureImage, 
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		0,  // srcAccessMask: no prior access
		VK_ACCESS_2_TRANSFER_WRITE_BIT,  // dst: we will write via transfer
		VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_2_TRANSFER_BIT,  // dst stage: transfer op
		VK_IMAGE_ASPECT_COLOR_BIT,
		mipLevels
	);

	copyBufferToImage(&stagingBuffer, &textureImage, texWidth, texHeight);

	 
	// transitionImageLayout(
	// 	&textureImage, 
	// 	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	// 	VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
	// 	VK_ACCESS_2_TRANSFER_WRITE_BIT,   // src: transfer just wrote
	// 	VK_ACCESS_2_SHADER_READ_BIT,      // dst: shader will read
	// 	VK_PIPELINE_STAGE_2_TRANSFER_BIT, // src stage
	// 	VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,  // ✅ dst stage: shader (NOT transfer!)
	// 	VK_IMAGE_ASPECT_COLOR_BIT,
	// 	mipLevels
	// );

	// vkFreeMemory(device, textureImageMemoryTemp, NULL);

	vkDestroyBuffer(device,stagingBuffer,  NULL);
	
	vkFreeMemory(device, stagingBufferMemory,  NULL);


	// generateMipmaps(*textureImage, vk::Format::eR8G8B8A8Srgb, texWidth, texHeight, mipLevels);

	generateMipmaps(&textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
}

void generateMipmaps(VkImage* image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {

    VkCommandBuffer commandBuffer = {};
	
	beginSingleTimeCommands(&commandBuffer);
 

	VkFormatProperties2 formatProperties = {
		.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2,
	};
	
	vkGetPhysicalDeviceFormatProperties2(physicalDevice, imageFormat, &formatProperties);
// vk::FormatFeatureFlagBits::eSampledImageFilterLinear
	if (!(formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
    	// throw std::runtime_error("texture image format does not support linear blitting!");

		EXIT_CLEAN("texture image format does not support linear blitting!");
	}


    VkImageMemoryBarrier2 barrier ={
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = *image,
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
		},
		.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,

		.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		.newLayout =VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,

		.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
		.dstAccessMask =VK_ACCESS_2_TRANSFER_READ_BIT,
	};
	
 
	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	VkDependencyInfo dependencyInfo = {
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
			.dependencyFlags = 0,
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &barrier,
			
	};	
	 

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;

		// When transitioning mip level to SHADER_READ_ONLY_OPTIMAL:
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT;
	
		barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
		barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;  // or ALL_GRAPHICS_BIT
				
 

		vkCmdPipelineBarrier2(
			commandBuffer, 
			&dependencyInfo
		 
		);

		// VkOffset3D offsets[2];
		// VkOffset3D dstOffsets[2];
	 
		// offsets[0] = (VkOffset3D){0,0,0};
		// offsets[1] = (VkOffset3D){mipWidth,mipHeight,1};

		// dstOffsets[0] =  (VkOffset3D){0,0,0};
		// dstOffsets[1] =  (VkOffset3D){mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};

		VkImageBlit2 imageBlit2 = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2,
			.srcOffsets = {
				{0, 0, 0},
				{mipWidth, mipHeight, 1}
			},
			.dstOffsets = {
				{0, 0, 0},
				{mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1}
			},
			.srcSubresource = (VkImageSubresourceLayers){
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = i - 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
			.dstSubresource = (VkImageSubresourceLayers){
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = i,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		};
		/**
		    VkStructureType        sType;
    const void*            pNext;
    VkImage                srcImage;
    VkImageLayout          srcImageLayout;
    VkImage                dstImage;
    VkImageLayout          dstImageLayout;
    uint32_t               regionCount;
    const VkImageBlit2*    pRegions;
    VkFilter               filter;


	{blit}, vk::Filter::eLinear
		*/
		VkBlitImageInfo2 blitImageInfo2 = {
			.sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2,
			.srcImage = *image,
			.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			.dstImage = *image,
			.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			.filter = VK_FILTER_LINEAR,
			.pRegions = &imageBlit2,
			.regionCount = 1,
		};

		vkCmdBlitImage2(commandBuffer, &blitImageInfo2);


		/*
		barrier.oldLayout     = vk::ImageLayout::eTransferSrcOptimal;
			barrier.newLayout     = vk::ImageLayout::eShaderReadOnlyOptimal;
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
		*/


		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout =VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
		barrier.dstAccessMask =VK_ACCESS_2_SHADER_READ_BIT;



		/**
		commandBuffer->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, {}, {}, {}, barrier);
		*/

		
		// VkDependencyInfo dependencyInfo = {
		// 	.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		// 	.dependencyFlags = 0,
		// 	.imageMemoryBarrierCount = 1,
		// 	.pImageMemoryBarriers = &barrier,
		// };	
	 
		vkCmdPipelineBarrier2(
			commandBuffer, 
			&dependencyInfo
		 
		);


		if (mipWidth > 1)
			mipWidth /= 2;
		if (mipHeight > 1)
			mipHeight /= 2;
	}

	/**
	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout                     = vk::ImageLayout::eTransferDstOptimal;
		barrier.newLayout                     = vk::ImageLayout::eShaderReadOnlyOptimal;
		barrier.srcAccessMask                 = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask                 = vk::AccessFlagBits::eShaderRead;

	
	*/


	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT;
	barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
	barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;  

	vkCmdPipelineBarrier2(
		commandBuffer, 
		&dependencyInfo
		
	);

    endSingleTimeCommands(&commandBuffer);
}

void createDescriptorSets(){
	PRINT_FNAME;


	VkDescriptorSetLayout layouts [MAX_FRAMES_IN_FLIGHT]={
		//C99 designated initializer
		 [0 ... MAX_FRAMES_IN_FLIGHT-1] = descriptorSetLayout
	};

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = descriptorPool,
		.descriptorSetCount = MAX_FRAMES_IN_FLIGHT,
		.pSetLayouts = layouts,
		
	};
	vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, descriptorSets);


	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		VkDescriptorBufferInfo bufferInfo = { 
			.buffer = uniformBuffers[i], 
			.offset = 0, 
			//.range = VK_WHOLE_SIZE 
			.range = sizeof(struct UniformBufferObject) 
		};


		VkDescriptorImageInfo imageInfo = {
			.sampler = textureSampler,
			.imageView = textureImageView,
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		};



		VkWriteDescriptorSet   descriptorWrite[] = {
			(VkWriteDescriptorSet){
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = descriptorSets[i], 
				.dstBinding = 0, 
				.dstArrayElement = 0, 
				.descriptorCount = 1, 
				// .descriptorType = vk::DescriptorType::eUniformBuffer, 
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 
				.pBufferInfo = &bufferInfo
			},
			(VkWriteDescriptorSet){
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = descriptorSets[i], 
				.dstBinding = 1, 
				.dstArrayElement = 0, 
				.descriptorCount = 1, 
				// .descriptorType = vk::DescriptorType::eUniformBuffer, 
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 
				// .pBufferInfo = &bufferInfo
				.pImageInfo = &imageInfo,
			},
		};


		vkUpdateDescriptorSets(device, 2, descriptorWrite, 0, NULL);
		
	}

}




void clearUniformBuffers(){
	
	PRINT_FNAME;

	for(int i=0;i<MAX_FRAMES_IN_FLIGHT;i++){
		if(uniformBuffersMapped[i] != NULL)
		{
			vkUnmapMemory(device,uniformBuffersMemory[i]);
			uniformBuffersMapped[i] = NULL;
		}
		if(uniformBuffers[i] != NULL){
			vkDestroyBuffer(device, uniformBuffers[i], NULL);
			uniformBuffers[i] = NULL;
		}
		if(uniformBuffersMemory[i] != NULL){
			vkFreeMemory(device, uniformBuffersMemory[i], NULL);
			uniformBuffersMemory[i] = NULL;
		}
	
		
	}
}

void createUniformBuffers(){

	clearUniformBuffers();

	VkDeviceSize bufferSize = sizeof(struct UniformBufferObject);

	for(int i=0; i < MAX_FRAMES_IN_FLIGHT; i++){

		VkBuffer buffer;
		VkDeviceMemory bufferMemory;

		createBuffer(
			bufferSize, 
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
			&buffer, &bufferMemory
		);
		uniformBuffers[i] = buffer;
		uniformBuffersMemory[i] = bufferMemory;

		void * memptr = NULL;
		

		vkMapMemory(device, bufferMemory, 0, bufferSize, 0, &memptr);

		uniformBuffersMapped[i] = memptr;
	}
}

void createDescriptorPool() {

	VkDescriptorPoolSize poolSize[]={
		(VkDescriptorPoolSize){
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = MAX_FRAMES_IN_FLIGHT,
		},
		(VkDescriptorPoolSize){
			.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = MAX_FRAMES_IN_FLIGHT,
		},
	};

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
		.maxSets = MAX_FRAMES_IN_FLIGHT,
		.poolSizeCount = sizeof(poolSize) / sizeof(*poolSize),
		.pPoolSizes = poolSize,
	};

	vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, NULL, &descriptorPool);
};

void createDescriptorSetLayout(){

	// uniform buffer object
	/*
	// .descriptorCount > 1 is used for transformation for each of the bones in a skeleton for skeletal animation
	
	*/
	

	VkDescriptorSetLayoutBinding descriptorSetLayoutBindings[] = {
		(VkDescriptorSetLayoutBinding) {
			.binding = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.pImmutableSamplers = NULL,
		},
		(VkDescriptorSetLayoutBinding) {
			.binding = 1,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
			.pImmutableSamplers = NULL,
		},
	};




	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = sizeof(descriptorSetLayoutBindings) / sizeof(VkDescriptorSetLayoutBinding),
		.pBindings = descriptorSetLayoutBindings,
	};

	VkResult res= vkCreateDescriptorSetLayout(device,&descriptorSetLayoutCreateInfo,NULL,&descriptorSetLayout);

	if(res != VK_SUCCESS){
		EXIT_CLEAN("vkCreateDescriptorSetLayout failed");
	}
}

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice,&physicalDeviceMemoryProperties);
	// = physicalDevice.getMemoryProperties();

	for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	EXIT_CLEAN("failed to find suitable memory type!");
}


//vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory
void createBuffer(
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkBuffer *buffer,
	VkDeviceMemory *bufferMemory){



	VkBufferCreateInfo bufferCreateInfo = {

		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		
	};
	 
	vkCreateBuffer(device, &bufferCreateInfo, NULL, buffer);

	VkMemoryRequirements memoryRequirements={
		
	};
	
	vkGetBufferMemoryRequirements(device, *buffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {

		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memoryRequirements.size,
		.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits,properties),
		
	};

	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties ;

	vkGetPhysicalDeviceMemoryProperties(physicalDevice,&physicalDeviceMemoryProperties);

	vkAllocateMemory(device, &memoryAllocateInfo, NULL, bufferMemory);

	vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
}
void copyBuffer(VkBuffer  srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {

	PRINT_FNAME;


	VkCommandBuffer commandCopyBuffer ;


	beginSingleTimeCommands(&commandCopyBuffer);

	VkBufferCopy bufferCopy = {
		.size = size,
		.dstOffset = 0,
		.srcOffset = 0
	};
	vkCmdCopyBuffer(commandCopyBuffer,  srcBuffer, dstBuffer, 1, &bufferCopy);

	endSingleTimeCommands(&commandCopyBuffer);

}
void createIndexBuffer(){

	VkDeviceSize bufferSize = sizeof(uint32_t) * indicesNum;

	VkBuffer stagingBuffer;

    VkDeviceMemory bufferMemory;

	createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT ,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer,
		&bufferMemory
	);

	void * data = NULL;

	vkMapMemory( device, bufferMemory, 0, bufferSize, 0, &data);

	memcpy(data, indices, bufferSize);

	VkMappedMemoryRange mappedMemoryRange = {
		.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
		.memory = bufferMemory,
		.offset = 0,
		.size = bufferSize,
		
	};

	vkFlushMappedMemoryRanges(device, 1, &mappedMemoryRange);

	vkUnmapMemory(device, bufferMemory);
	

	
	createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&indexBuffer,
		&indexBufferMemory
	);

    copyBuffer(stagingBuffer, indexBuffer,  bufferSize);

	vkDestroyBuffer(device, stagingBuffer, NULL);

	vkFreeMemory(device, bufferMemory, NULL);

}
void createVertexBuffer() {

    VkDeviceSize bufferSize = sizeof(struct Vertex) * verticesNum;

	VkBuffer stagingBuffer;
	
    VkDeviceMemory bufferMemory;


	createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT ,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer,
		&bufferMemory
	);


	void * data = NULL;

	vkMapMemory( device, bufferMemory, 0, bufferSize, 0, &data);

	memcpy(data, vertices, sizeof(struct Vertex)*verticesNum);

	VkMappedMemoryRange mappedMemoryRange = {
		.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
		.memory = bufferMemory,
		.offset = 0,
		.size = bufferSize,
		
	};

	vkFlushMappedMemoryRanges(device, 1, &mappedMemoryRange);

	vkUnmapMemory(device, bufferMemory);


	
	createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&vertextBuffer,
		&vertexBufferMemory
	);

    copyBuffer(stagingBuffer, vertextBuffer,  bufferSize);

	vkDestroyBuffer(device, stagingBuffer, NULL);

	vkFreeMemory(device, bufferMemory, NULL);
}


static void framebufferResizeCallback(GLFWwindow *win,int w,int h)
{
	framebufferResized = true;
}


void createSurface(){
	PRINT_FNAME;;

	glfwCreateWindowSurface(instance, window,NULL, &surface);
	
	
}

uint32_t createShaderFromFile(const char * path, uint8_t** buffer){

	PRINT_FNAME;

	FILE *file = fopen(path, "rb");  // open file in read mode

    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }


	fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    *buffer = malloc(size);
    if (*buffer == NULL) {
        printf("Fill buffer mem alloc error\n");
        fclose(file);
        return 0;
    }

    // read file
    fread(*buffer, 1, size, file);


	// free(buffer);
    fclose(file);  // close file

	return size;
}
// bool isDeviceSuitable(VkPhysicalDevice device)
// {
// 	PRINT_FNAME;
 
// 	VkPhysicalDeviceFeatures2 physicalDeviceFeatures2;

// 	VkPhysicalDeviceProperties physicalDeviceProperties;


// 	vkGetPhysicalDeviceFeatures2(device,&physicalDeviceFeatures2);

// 	vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);

	
// 	VkPhysicalDeviceFeatures2 *features2 = &physicalDeviceFeatures2;
// 	uint32_t feature_cnt = 0 ;
// 	while(features2 != NULL){
// 		switch(features2->sType)
// 		{
// 			case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2:
// 			break;

// 			// case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2:
// 			// break;
			

// 			// case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2:
// 			// break;
			

// 			// case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2:
// 			// break;
			


// 			default:
// 				break;
// 		}
// 	}

//     // if (physicalDeviceProperties.deviceType > VK_PHYSICAL_DEVICE_TYPE_OTHER && 
// 	// 	physicalDeviceFeatures2. &&
// 	// 	physicalDeviceFeatures2.samplerAnisotropy
		
		
// 	// ) {
//     //     return true;
//     // }

//     return false;
// }
void createCommandPool(){
	PRINT_FNAME;

	VkCommandPoolCreateInfo commandPoolCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = queueFamilyIndeces[graphicsQueueFamilyArrayIndex],
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	
	vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, &graphicsCommnadPool);

	VkCommandBufferAllocateInfo allocInfo = { 
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool =  graphicsCommnadPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = MAX_FRAMES_IN_FLIGHT  ,
	};

	vkAllocateCommandBuffers(device, &allocInfo, graphicsCommandBuffers);


	// --
	commandPoolCreateInfo = (VkCommandPoolCreateInfo){
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = queueFamilyIndeces[transferQueueFamilyArrayIndex] ,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	
	vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, &transferCommnadPool);

	 allocInfo = (VkCommandBufferAllocateInfo){ 
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool =  transferCommnadPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = transferCommandBuffersCount  ,
		
	};

	vkAllocateCommandBuffers(device, &allocInfo, &transferCommandBuffers);

	

}

void recordCommandBuffer(uint32_t imageIndex,uint32_t frameIndex) {

	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pInheritanceInfo = NULL,	
	};

	vkBeginCommandBuffer(graphicsCommandBuffers[frameIndex], &beginInfo );

	
	transitionImageLayout(
		&swapchainImages[imageIndex], 
		VK_IMAGE_LAYOUT_UNDEFINED,
		// currentSwapchainLayouts[imageIndex], 
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
		0, 
		VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT, 
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT, 
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT, 
		VK_IMAGE_ASPECT_COLOR_BIT,
		1
	);

	//  currentSwapchainLayouts[imageIndex] = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	transitionImageLayout(
		&depthImage, 
		VK_IMAGE_LAYOUT_UNDEFINED, 
		VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL, 
		VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, 
		VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, 
		VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT, 
		VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT, 
		VK_IMAGE_ASPECT_DEPTH_BIT,
		1
	);

	VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

	 VkClearValue clearDepth = {1.0f, 0};




    VkRenderingAttachmentInfo colorAttachmentInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = swapchainImageViews[imageIndex],
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp =VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = clearColor
    };

	VkRenderingAttachmentInfo deapthAttachmentInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = depthImageView,
        .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        .loadOp =VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .clearValue = clearDepth
    };

    // Set up the rendering info
    VkRenderingInfo renderingInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = { 
			.offset = { 0, 0 }, 
			//.extent = surfaceCapabilities.currentExtent,
			.extent = swapChainExtent,
		},
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentInfo,
		.pDepthAttachment = &deapthAttachmentInfo,
		
    };

	vkCmdBeginRendering(graphicsCommandBuffers[frameIndex], &renderingInfo);
    // Begin rendering
    

	vkCmdBindPipeline(graphicsCommandBuffers[frameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);


	VkViewport viewPort = {
		.x = 0,
		.y = 0,
		.width = swapChainExtent.width,
		.height = swapChainExtent.height,
		.minDepth = 0.0f,  // Explicitly set
    	.maxDepth = 1.0f   // Explicitly set
		
	};
	vkCmdSetViewport(graphicsCommandBuffers[frameIndex], 0, 1, &viewPort);

	VkRect2D scissor = {
		.extent = swapChainExtent,
		.offset = {},
	};

	vkCmdSetScissor(graphicsCommandBuffers[frameIndex], 0, 1, &scissor);


	// vkCmdBindPipeline(graphicsCommandBuffers[frameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkDeviceSize offset = 0;

	vkCmdBindVertexBuffers(graphicsCommandBuffers[frameIndex], 0, 1, &vertextBuffer, &offset);

	vkCmdBindIndexBuffer(graphicsCommandBuffers[frameIndex], indexBuffer, 0, VK_INDEX_TYPE_UINT32);

	 
	vkCmdBindDescriptorSets(
		graphicsCommandBuffers[frameIndex],
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipelineLayout, 
		0,
		1, 
		&descriptorSets[frameIndex], 
		0, 
		NULL);

	// uint32_t indicesCount = sizeof(indices) / sizeof(uint32_t);

	vkCmdDrawIndexed(graphicsCommandBuffers[frameIndex], indicesNum, 1, 0, 0, 0);

	vkCmdEndRendering(graphicsCommandBuffers[frameIndex]);

 
	
	transitionImageLayout(
		&swapchainImages[imageIndex], 
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 
		VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT, 
		0, 
		VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT, 
		VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT, 
		VK_IMAGE_ASPECT_COLOR_BIT,
		1
	);

	// currentSwapchainLayouts[imageIndex] = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	vkEndCommandBuffer(graphicsCommandBuffers[frameIndex]);

}
void updateUniformBuffer(uint32_t currentImage){

	double current_time;

	current_time = glfwGetTime();

	double time = current_time - start_time;

	// printf("time %f\n",time);

	struct UniformBufferObject ubo = {};
	// mat4 model;
	// vec3 rotation = {0.f,0.f,1.f};

	glm_mat4_identity(ubo.model);
	


	glm_rotate_x(ubo.model, time*glm_rad(90.0f), ubo.model);


	 

	glm_lookat((vec3){2.0f,2.0f,2.0f}, (vec3){.0f,.0f,.0f}, (vec3){.0f,1.0f,0.0f}, ubo.view);

	
	glm_perspective(glm_rad(45.0f), (float)swapChainExtent.width / swapChainExtent.height, 0.1f, 10.0f , ubo.proj);

	ubo.proj[1][1] *= -1;

	memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));

	// ubo.model = glm_rotate(model, time*glm_rad(90.0f), rotation);


};

void drawFrame() {


	VkResult result = vkWaitForFences(
		device, 
		1, 
		&inFlightFences[frameIndex],
		VK_TRUE, UINT64_MAX
	);

	if(result != VK_SUCCESS)
	{
		EXIT_CLEAN("failed to wait for fence!");
	}

	uint32_t imageIndex =  -1;
	result = vkAcquireNextImageKHR(
		device, 
		swapchain, 
		UINT64_MAX, 
		presentCompleteSemaphore[frameIndex], 
		NULL, 
		&imageIndex
	);
 
	if(result == VK_ERROR_OUT_OF_DATE_KHR){

		recreateSwapChain();

		return;

	}else if(result != VK_SUCCESS && result!= VK_SUBOPTIMAL_KHR)
	{
		assert(result == VK_TIMEOUT || result == VK_NOT_READY);
		EXIT_CLEAN("failed to acquire swap chain image!");
	}

	
	updateUniformBuffer(frameIndex);

	vkResetFences(device, 1, &inFlightFences[frameIndex]);

	// vkResetCommandBuffer(graphicsCommandBuffers[frameIndex],VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);


	recordCommandBuffer(imageIndex,frameIndex);

	VkPipelineStageFlags waitDestinationStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT ;

	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &presentCompleteSemaphore[frameIndex],
		
		.pWaitDstStageMask = &waitDestinationStageMask,
		
		.commandBufferCount = 1,
		.pCommandBuffers = &graphicsCommandBuffers[frameIndex],

		.signalSemaphoreCount = 1,
		.pSignalSemaphores =  &renderFinishedSemaphore[frameIndex],

	};

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[frameIndex]);


	VkPresentInfoKHR presentInfo = {

		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &renderFinishedSemaphore[frameIndex],
		.swapchainCount = 1,
		.pSwapchains = &swapchain,
		.pImageIndices = &imageIndex,
		
	};


	result = vkQueuePresentKHR(graphicsQueue, &presentInfo);

	if(
		(result == VK_SUBOPTIMAL_KHR) || 
		(result == VK_ERROR_OUT_OF_DATE_KHR) || 
		framebufferResized
	){
		framebufferResized = false;
		recreateSwapChain();
	}else
	{
		assert(result == VK_SUCCESS);
	}
	frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;

}
void createSyncObjects(){
	
	PRINT_FNAME;


	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{

	
		VkSemaphoreCreateInfo semaphoreCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};
		vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &presentCompleteSemaphore[i]);

		vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &renderFinishedSemaphore[i]);

		VkFenceCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT,
		};

		vkCreateFence(device, &createInfo, NULL,  &inFlightFences[i]);
	}
	
}

void queueFamilyCheck(){

	PRINT_FNAME;
	

	uint32_t queueFamilyPropertieCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertieCount, NULL);

	VkQueueFamilyProperties queueFamilyProperties[queueFamilyPropertieCount];

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertieCount, queueFamilyProperties);
	
	/**
	VK_QUEUE_GRAPHICS_BIT = 0x00000001,
    VK_QUEUE_COMPUTE_BIT = 0x00000002,
    VK_QUEUE_TRANSFER_BIT = 0x00000004,
    VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
    VK_QUEUE_PROTECTED_BIT = 0x00000010,
    VK_QUEUE_VIDEO_DECODE_BIT_KHR = 0x00000020,
    VK_QUEUE_VIDEO_ENCODE_BIT_KHR = 0x00000040,
    VK_QUEUE_OPTICAL_FLOW_BIT_NV = 0x00000100,
	
	*/

	queueFamilyIndeces[graphicsQueueFamilyArrayIndex] = -1;
	  

	for(int i=0;i<queueFamilyPropertieCount;i++){
		printf("queue count: %d, flags: %x\n",queueFamilyProperties[i].queueCount,queueFamilyProperties[i].queueFlags);
		
		if(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
			queueFamilyIndeces[graphicsQueueFamilyArrayIndex]  = i;
			break;
		}
	}
	if(queueFamilyIndeces[graphicsQueueFamilyArrayIndex]  == - 1 ){
		EXIT_CLEAN("No supported VK_QUEUE_GRAPHICS_BIT");
	}

	queueFamilyIndeces[transferQueueFamilyArrayIndex] = -1;
	
	for(int i=0;i<queueFamilyPropertieCount;i++){
		printf("queue count: %d, flags: %x\n",queueFamilyProperties[i].queueCount,queueFamilyProperties[i].queueFlags);
		
		if(queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT){
			queueFamilyIndeces[transferQueueFamilyArrayIndex]  = i;
			break;
		}
	}

	if(queueFamilyIndeces[transferQueueFamilyArrayIndex]  == -1){
		EXIT_CLEAN("No supported VK_QUEUE_TRANSFER_BIT");
	}


}
void physicalDeviceExtensionCheck(){

	PRINT_FNAME;


	uint32_t deviceExtensionPropertieCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, NULL,&deviceExtensionPropertieCount,NULL);

	VkExtensionProperties exp_props[deviceExtensionPropertieCount];
	vkEnumerateDeviceExtensionProperties(physicalDevice, NULL,&deviceExtensionPropertieCount,exp_props);

	// bool found = false;
	int supportedCnt = 0;
	// bool requiredDeviceExtensionSupport = false;
	for(int i=0;i<deviceExtensionPropertieCount; i++){

		// printf("\tphys device extension: %s\n",exp_props[i].extensionName);

		for(int k = 0;k < requiredDeviceExtensionCount ; k++)
		{
			if(strcmp(exp_props[i].extensionName, requiredDeviceExtensions[k]) == 0){
				supportedCnt ++;
				break;
			}
		}
	}
	
	// if(supportedCnt == requiredDeviceExtensionCount)
	// {
		 
	// 	requiredDeviceExtensionSupport = true;
	// }

	if(!(supportedCnt == requiredDeviceExtensionCount)){
		printf("No suported extensions\n");
		printf("supportedCnt: %d of %d\n",supportedCnt, requiredDeviceExtensionCount);

		for(int i=0;i<deviceExtensionPropertieCount; i++){

			printf("\tfound: %s\n",exp_props[i].extensionName);
		}
		for(int i=0;i<requiredDeviceExtensionCount; i++){

			printf("\trequired: %s\n",requiredDeviceExtensions[i]);
		}
		// exit(1);
		EXIT_CLEAN("\n");
	}

}

void physicalDeviceFeatureCheck(){

	PRINT_FNAME;

	VkPhysicalDeviceFeatures2 physicalDeviceFeatures2={
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		// .features = {.samplerAnisotropy = true},
	};



	VkPhysicalDeviceVulkan13Features features13 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		// .dynamicRendering = VK_TRUE,
		// .synchronization2 = VK_TRUE,
		
	};

	VkPhysicalDeviceExtendedDynamicStateFeaturesEXT physicalDeviceExtendedDynamicStateFeaturesEXT = {

		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
	};

	features13.pNext = &physicalDeviceExtendedDynamicStateFeaturesEXT;
	physicalDeviceFeatures2.pNext = &features13;


	vkGetPhysicalDeviceFeatures2(physicalDevice, &physicalDeviceFeatures2 );

	VkPhysicalDeviceFeatures2 * next = &physicalDeviceFeatures2;

	int all_ok = 0;

	while(next!= NULL){
	
		switch(next->sType){
			case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2:
					printf("supported: VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2\n");

					{
						
						if(next->features.samplerAnisotropy ){
							all_ok ++;
						}
					}
					
				break;
			case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT:
					printf("supported: VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT\n");

					{
						VkPhysicalDeviceExtendedDynamicStateFeaturesEXT * temp = (VkPhysicalDeviceExtendedDynamicStateFeaturesEXT*)next;
						if(temp->extendedDynamicState){
							all_ok ++;
						}
					}
					
					// all_ok ++;
				break;
			case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES:
					printf("supported: VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES\n");

					{
						VkPhysicalDeviceVulkan13Features * temp = (VkPhysicalDeviceVulkan13Features*)next;
						if(temp->dynamicRendering && temp->synchronization2 ){
							all_ok +=2;
						}
					}
				break;
			default:
					printf("next %d\n",next->sType);
				break;
		}		
		next = next->pNext;
	}
	if(all_ok != 4){

		printf("supported :%d \n",all_ok);

		EXIT_CLEAN("Some Device features not supported");
	}
	// requiredDeviceFeaturesSupport = true;
}
void  createLogicalDevice(){

	PRINT_FNAME;

	uint32_t physicalDeviceQueueFamilyPropertieCount = 0 ;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueFamilyPropertieCount, NULL);

	VkQueueFamilyProperties queueFamilyProperties[physicalDeviceQueueFamilyPropertieCount];

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueFamilyPropertieCount, queueFamilyProperties);

	VkBool32 supported = VK_FALSE;
	
	presentationSupportQueueFamilyIndex = - 1;

	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndeces[graphicsQueueFamilyArrayIndex] , surface, &supported);

	if(supported == VK_TRUE)
	{
		presentationSupportQueueFamilyIndex = queueFamilyIndeces[graphicsQueueFamilyArrayIndex];
	}
	else
	{
		EXIT_CLEAN("vkGetPhysicalDeviceSurfaceSupportKHR == false");
	}


	float queuePriority = 0.5f;

	VkDeviceQueueCreateInfo deviceQueueCreateInfo = {

		.sType =  VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = queueFamilyIndeces[graphicsQueueFamilyArrayIndex],
		.queueCount = 1,
		.pQueuePriorities = &queuePriority,
	};

	VkPhysicalDeviceFeatures2 physicalDeviceFeatures2={

		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.features.samplerAnisotropy = VK_TRUE,
		
	};

	VkPhysicalDeviceVulkan13Features physicalDeviceFeatures13={

		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.dynamicRendering = VK_TRUE,
		.synchronization2 = VK_TRUE
	};

	VkPhysicalDeviceExtendedDynamicStateFeaturesEXT physicalDeviceExtendedDynamicStateFeaturesEXT = {

		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
		.extendedDynamicState = VK_TRUE,
	};

	physicalDeviceFeatures13.pNext = &physicalDeviceExtendedDynamicStateFeaturesEXT;

	physicalDeviceFeatures2.pNext = &physicalDeviceFeatures13;

	VkDeviceCreateInfo deviceCreateInfo = {

		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = &deviceQueueCreateInfo,
		.queueCreateInfoCount = 1,
		.pNext = &physicalDeviceFeatures2,
		.ppEnabledExtensionNames = requiredDeviceExtensions,
		.enabledExtensionCount = requiredDeviceExtensionCount,
	};

	VkResult res = vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device);

	if(res != VK_SUCCESS)
	{
	
		EXIT_CLEAN("failed to create logical device");
	}


}

void createPhysicalDevice()
{
	PRINT_FNAME;
	
	uint32_t physicalDeviceCount = 0;

	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL);

	VkPhysicalDevice physicalDevices[physicalDeviceCount];

	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);



	VkPhysicalDeviceFeatures features;

	VkPhysicalDeviceProperties properties;

	
	uint32_t ratings[physicalDeviceCount]={};

	for(int i=0;i<physicalDeviceCount;i++){

		 
		vkGetPhysicalDeviceFeatures(physicalDevices[i],&features);

		vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

		printf("\ndevice name: %s\n",properties.deviceName);

				/*
		    VK_PHYSICAL_DEVICE_TYPE_OTHER = 0,
			VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU = 1,
			VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU = 2,
			VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU = 3,
			VK_PHYSICAL_DEVICE_TYPE_CPU = 4,
		
		*/

		if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
		{

			ratings[i] +=1000;
		}
		else if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU )
		{
		 
			ratings[i] +=500;
			
		}

		ratings[i]  += properties.limits.maxImageDimension2D;

    	if (!features.geometryShader)
		{
			ratings[i] = 0;
			continue;	
        }
		
		
	}

	uint32_t max = 1;

	uint32_t max_id = 1000;

	for(int i=0;i<physicalDeviceCount;i++){

		if(max < ratings[i]){

			max_id = i;

			max = ratings[i];
		}
	}

	if(max_id == 1000)
	{

		EXIT_CLEAN("failed to find a suitable GPU!");
	}
	physicalDevice = physicalDevices[max_id];


	
	vkGetPhysicalDeviceFeatures(physicalDevice,&features);

	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	printf("\nSELECTED device name: %s\n",properties.deviceName);
	printf("device rating: %d\n",ratings[max_id]);
	printf("device type: %d\n",properties.deviceType);
	printf("geometry shader: %d\n",features.geometryShader);
	bool supportsVulkan1_3 = properties.apiVersion >= VK_API_VERSION_1_3;
	printf("API >= 1.3 support: %d\n",supportsVulkan1_3);
	printf("\n");



}

void createSwapchain(){
	PRINT_FNAME;
	
	
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		physicalDevice, 
		surface, 
		&surfaceCapabilities);


	printf(
		"Surface extent: w:%d h:%d\n",
		surfaceCapabilities.currentExtent.width, 
		surfaceCapabilities.currentExtent.height
	);

	uint32_t physicalDeviceSurfaceFormatCount = 0;

	vkGetPhysicalDeviceSurfaceFormatsKHR(
		physicalDevice, 
		surface, 
		&physicalDeviceSurfaceFormatCount , NULL);

	VkSurfaceFormatKHR surfaceFormats[physicalDeviceSurfaceFormatCount];

	vkGetPhysicalDeviceSurfaceFormatsKHR(
		physicalDevice, surface, 
		&physicalDeviceSurfaceFormatCount , surfaceFormats);

	printf("Surface formats:\n");

	
	swapchainSurfaceFormat = VK_FORMAT_B8G8R8A8_SRGB;

	swapchainSurfaceColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

	// uint32_t color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	
	
	for(int i=0;i<physicalDeviceSurfaceFormatCount;i++)
	{
		printf("\tSurfaceFormat.format: %d\n",surfaceFormats[i].format);

		swapchainSurfaceFormat = surfaceFormats[i].format;

		if(swapchainSurfaceFormat == VK_FORMAT_B8G8R8A8_SRGB)
		{
			break;
		}
	}

	for(int i=0;i<physicalDeviceSurfaceFormatCount;i++)
	{
		printf("\tSurfaceFormat.colorSpace:%d\n",surfaceFormats[i].colorSpace);

		swapchainSurfaceColorSpace = surfaceFormats[i].colorSpace;

		if(swapchainSurfaceColorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			break;
		}
		
	}

	uint32_t presentModeCount = 0 ;

	vkGetPhysicalDeviceSurfacePresentModesKHR(
		physicalDevice,
		surface, 
		&presentModeCount, NULL);

	VkPresentModeKHR presentModes[presentModeCount];
	
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		physicalDevice, 
		surface, 
		&presentModeCount, 
		presentModes);
	


	printf("Surface pPresentModes:\n");

	uint32_t presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

	for(int i=0;i<presentModeCount;i++)
	{
		printf("\tpresent mode: %d\n",presentModes[i]);

		if(presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			presentMode  = VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}




	swapChainExtent = surfaceCapabilities.currentExtent;

	if(swapChainExtent.width > surfaceCapabilities.maxImageExtent.width 
		&& swapChainExtent.height > surfaceCapabilities.maxImageExtent.height 
	){
		swapChainExtent = surfaceCapabilities.maxImageExtent;
		swapChainExtent = (VkExtent2D){
			.width = WIDTH,
			.height = HEIGHT,
		};
	}
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	if(
		width >= surfaceCapabilities.minImageExtent.width &&
		width <= surfaceCapabilities.maxImageExtent.width &&
		height >= surfaceCapabilities.minImageExtent.height &&
		height <= surfaceCapabilities.maxImageExtent.height
		)
		{
			swapChainExtent = (VkExtent2D){
			.width = width,
			.height = height,
			};
		}
	
	VkSwapchainCreateInfoKHR createInfo={
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.minImageCount = surfaceCapabilities.minImageCount  == 0? 1: surfaceCapabilities.minImageCount,
		.imageFormat = swapchainSurfaceFormat,
		.imageColorSpace = swapchainSurfaceColorSpace,
		.imageExtent = swapChainExtent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = surfaceCapabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = presentMode,
		.surface = surface,
		.clipped = true,
	};
	
	
	
	vkCreateSwapchainKHR(device, &createInfo, NULL, &swapchain);

}



void createImageViews(){

	PRINT_FNAME;

	vkGetSwapchainImagesKHR(device, swapchain,  &swapchainImageCount,NULL);

	vkGetSwapchainImagesKHR(device, swapchain,  &swapchainImageCount, swapchainImages);



	// printf("pSwapchainImageCount %d\n",swapchainImageCount);
	
	swapchainImageViewCount = swapchainImageCount;

	for(int i=0;i<swapchainImageCount;i++)
	{

		// printf("img: %p\n",swapchainImages[i]);

		createImageView(&swapchainImageViews[i], &swapchainImages[i], swapchainSurfaceFormat,VK_IMAGE_ASPECT_COLOR_BIT,1);

		// currentSwapchainLayouts[i] = VK_IMAGE_LAYOUT_UNDEFINED;  
	}


 
}
void cleanupSwapChain() {


	for(int i=0;i<swapchainImageViewCount;i++)
	{
		vkDestroyImageView(device, swapchainImageViews[i], NULL);
	}
	swapchainImageViewCount = 0;
	
    vkDestroySwapchainKHR(device, swapchain, NULL);

	swapchain = NULL;

}

void recreateSwapChain(){
	PRINT_FNAME;

	int width = 0,height = 0;

	glfwGetFramebufferSize(window, &width, &height);

	while(width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);

		glfwWaitEvents();
	}


	vkDeviceWaitIdle(device);

	cleanupSwapChain();

	createSwapchain();	

	createImageViews();
	
}

VkBool32 debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData)
{
	
	printf("validation layer:%d %d %s\n",messageSeverity,messageTypes ,pCallbackData->pMessage);
	return VK_FALSE;
}




void setupDebugMessenger()
{
	PRINT_FNAME;;
    if (!enableValidationLayers) return;

	VkDebugUtilsMessageSeverityFlagsEXT severityFlags = 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ;

	VkDebugUtilsMessageTypeFlagsEXT messageTypeFlags =     
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT ;

	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = severityFlags,
		.messageType = messageTypeFlags,
		.pfnUserCallback = debugCallback
	};
	assert(instance);


	 PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != NULL) {
        if(func(instance, &debugUtilsMessengerCreateInfoEXT, NULL, &debugUtilsMessengerEXT)!= VK_SUCCESS){
			printf("%s\n","cannot setup debug messenger");
		}
    } else {
     

		printf("%s\n","VK_ERROR_EXTENSION_NOT_PRESENT");
	
    }
 

}
void createQueue(){
	
	vkGetDeviceQueue(device, queueFamilyIndeces[graphicsQueueFamilyArrayIndex], 0, &graphicsQueue);
	vkGetDeviceQueue(device, queueFamilyIndeces[transferQueueFamilyArrayIndex], 0, &transferQueue);
	
}

void createInstance(){
	printf( "%s\n", __FUNCTION__ );

	VkApplicationInfo applicationInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 ),
		.pEngineName        = "No Engine",
		.engineVersion      = VK_MAKE_VERSION( 1, 0, 0 ),
		.apiVersion         = VK_API_VERSION_1_4
	};

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions  =  glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	

	
	uint32_t glfwExtensionCountExtra = glfwExtensionCount + 1;
	const char* glfwExtensionsExtra[glfwExtensionCountExtra];

	for(int i=0;i<glfwExtensionCount;i++){
			glfwExtensionsExtra[i] = glfwExtensions[i];
	}
	
	

	if(enableValidationLayers){
		
		glfwExtensionsExtra[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
	}else
	{
		glfwExtensionCountExtra = glfwExtensionCount;
	}

	
	// printf("glfwExtensionsExtra [%d]:\n",glfwExtensionCountExtra);
	for(int i=0;i<glfwExtensionCountExtra;i++){

		printf("extensions extra: %s\n",glfwExtensionsExtra[i]);
	}

	{

		uint32_t layerCount = 0;
		VkResult result = vkEnumerateInstanceLayerProperties(&layerCount, NULL);
		if (result != VK_SUCCESS) {
			printf("Failed to get layer count\n");
			return ;
		}

		VkLayerProperties layers[layerCount];
		result = vkEnumerateInstanceLayerProperties(&layerCount, layers);
		if (result != VK_SUCCESS) {
			printf("Failed to enumerate layers\n");
		 
			return  ;
		}

		printf("Print layers:\n");
		bool validationLayerSupported = false;
		for (uint32_t i = 0; i < layerCount; i++) {
			
			printf("layer: %s\n",layers[i].layerName);

			uint32_t cnt = 0;

			vkEnumerateInstanceExtensionProperties(layers[i].layerName, &cnt, NULL);

			VkExtensionProperties expr[cnt];
			vkEnumerateInstanceExtensionProperties(layers[i].layerName, &cnt, expr);

			for(int i=0;i<cnt;i++){

				printf("\tlayer extensions: %s\n",expr[i].extensionName);
			}

			if(strcmp(layers[i].layerName, validationLayers[0]) ==0){
				validationLayerSupported = true;
			}
		}
		if(validationLayerSupported == false){
			printf("Required layer is not supported \n\t%s\n",validationLayers[0]);
			return ; 
		}

		 
	}
	 
	

	VkInstanceCreateInfo instanceCreateInfo={
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &applicationInfo,
		.enabledExtensionCount = glfwExtensionCountExtra,
		.ppEnabledExtensionNames = glfwExtensionsExtra,
		
	};
	

	if(enableValidationLayers)
	{
		instanceCreateInfo.enabledLayerCount = validationLayerCount;
		instanceCreateInfo.ppEnabledLayerNames = validationLayers;

	}


	if(vkCreateInstance(&instanceCreateInfo,NULL,&instance) != VK_SUCCESS ){
		// printf("vkCreateInstance failed\n");
		EXIT_CLEAN("vkCreateInstance failed");
	
	}


}
void createGraphicsPipeline() {

	PRINT_FNAME;

	uint8_t* data = NULL;
	size_t dataSize = createShaderFromFile("shaders/frag.spv", &data);
	
	//--- FRAGMENT



	if(dataSize == 0)
	{
		printf("failed to read %s\n","shaders/frag.spv");
		return;
	}
	VkShaderModuleCreateInfo createInfo={
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,
		
	};
 
	vkCreateShaderModule(device, &createInfo, NULL, &shaderModuleFrag);

	free(data);



	//--- VERTEX
	
	dataSize = createShaderFromFile("shaders/vert.spv", &data);
	
	if(dataSize == 0)
	{
		printf("failed to read %s\n","shaders/vert.spv");
		return;
	}

	createInfo = (VkShaderModuleCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,
		
	};
	
	vkCreateShaderModule(device, &createInfo, NULL, &shaderModuleVert);
	free(data);
	//---------

	VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = shaderModuleVert,
		.pName = "main",
		

	};

	VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = shaderModuleFrag,
		.pName = "main",

	};

	size_t dynamicStateCount = 2;

	VkDynamicState dynamicState[]={VK_DYNAMIC_STATE_VIEWPORT,VK_DYNAMIC_STATE_SCISSOR};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = dynamicStateCount,
		.pDynamicStates = dynamicState,
		
	};

	
	VkPipelineShaderStageCreateInfo shaderStageCreateInf[] = {
		shaderStageCreateInfoVert,
		shaderStageCreateInfoFrag
	};

	size_t shaderStageCreateInfCnt = sizeof(shaderStageCreateInf) /  sizeof(VkPipelineShaderStageCreateInfo);


	VkVertexInputBindingDescription vertexInputBindingDescription;

	vertexGetBindingDescription(&vertexInputBindingDescription);


	VkVertexInputAttributeDescription vertexInputAttributeDescriptions[3];

	vertexGetAttributeDescriptions(vertexInputAttributeDescriptions);

	uint32_t vertexInputAttributeDescriptionsCount = sizeof(vertexInputAttributeDescriptions) / (sizeof(VkVertexInputAttributeDescription));



	VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &vertexInputBindingDescription,
		.vertexAttributeDescriptionCount = vertexInputAttributeDescriptionsCount,
		.pVertexAttributeDescriptions = vertexInputAttributeDescriptions,
	
	};

	VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	};

	VkViewport viewport = {
		.x = 0,
		.y = 0,
		.height = swapChainExtent.height,
		.width = swapChainExtent.width,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	//The actual viewport(s) and scissor rectangle(s) will then later be set up at drawing time.

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pViewports = {},
		.viewportCount = 1,
		.pScissors = {},
		.scissorCount  =1,
	};

 

	VkPipelineRasterizationStateCreateInfo rasterizer = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		// .cullMode = VK_CULL_MODE_NONE,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
		.depthBiasSlopeFactor = 1.0f,
		.lineWidth = 1.0f,
		
	};

	/*
	
	.rasterizationSamples = vk::SampleCountFlagBits::e1, .sampleShadingEnable = vk::False

	* */
	VkPipelineMultisampleStateCreateInfo multisampling = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.sampleShadingEnable = VK_FALSE,
	};

	//for now?
	VkPipelineDepthStencilStateCreateInfo depthStencil={
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable =  VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS,
		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE,
	};


	//Color blending

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState ={
		.blendEnable = VK_FALSE,
		.colorWriteMask  = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		.srcColorBlendFactor  = VK_BLEND_FACTOR_SRC_ALPHA,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.alphaBlendOp = VK_BLEND_OP_ADD,
	};


	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo={
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachmentState,
	};


	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &descriptorSetLayout,
		.pushConstantRangeCount  = 0,
	};

	vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, &pipelineLayout );


	VkFormat depthFormat = findDepthFormat();

	printf("depthFormat %d\n",depthFormat);

	VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = &swapchainSurfaceFormat,
		.depthAttachmentFormat = depthFormat,
		
		
	};
	


	VkSubpassDependency subpassDependency = {
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask = 0 ,
		.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		// .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT ,
	};
	VkAttachmentDescription colorAttachment = {
		.format = swapchainSurfaceFormat,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};



	VkAttachmentReference colorRef = {
    	.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	VkSubpassDescription subpass = {
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorRef
	};

	VkRenderPassCreateInfo renderPassCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.dependencyCount = 1,
		.pDependencies = &subpassDependency, 
		.subpassCount = 1,
		.attachmentCount = 1,
		.pAttachments = &colorAttachment,
		.pSubpasses = &subpass,
		
	};

	vkCreateRenderPass(device, &renderPassCreateInfo, NULL, &renderPass);

	// renderpass


	VkGraphicsPipelineCreateInfo  graphicsPipelineCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO , 
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
		.layout = pipelineLayout,
		// .renderPass = renderPass,
		.renderPass = VK_NULL_HANDLE,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = - 1,
		.pDepthStencilState = &depthStencil,
		
	};

	vkCreateGraphicsPipelines(
		device,
		NULL, 
		1, 
		&graphicsPipelineCreateInfo,
		NULL, 
		&graphicsPipeline
	);

}





/**
                                                                                                                                                                    
                                 
                                                                                                   
88               88                                        88  88                                  
""               ""    ,d                                  88  88                                  
                       88                                  88  88                                  
88  8b,dPPYba,   88  MM88MMM     8b       d8  88       88  88  88   ,d8   ,adPPYYba,  8b,dPPYba,   
88  88P'   `"8a  88    88        `8b     d8'  88       88  88  88 ,a8"    ""     `Y8  88P'   `"8a  
88  88       88  88    88         `8b   d8'   88       88  88  8888[      ,adPPPPP88  88       88  
88  88       88  88    88,         `8b,d8'    "8a,   ,a88  88  88`"Yba,   88,    ,88  88       88  
88  88       88  88    "Y888         "8"       `"YbbdP'Y8  88  88   `Y8a  `"8bbdP"Y8  88       88  
                                                                                                   
                                                                                                                                                                                                                                                               
                                                                                                                                                                         
*/


























void initVulkan(){
	PRINT_FNAME;

	createInstance();

	setupDebugMessenger();

	createSurface();

	createPhysicalDevice();

	queueFamilyCheck();

	physicalDeviceExtensionCheck();

	physicalDeviceFeatureCheck();

	createLogicalDevice();
	
	createSwapchain();

	createImageViews();

	createQueue();

	createDescriptorSetLayout();

	createGraphicsPipeline();

	createCommandPool();

	createDepthResources();

	createTextureImage(model_path_text);

	createTextureImageView();

	createTextureSampler();

	loadModel(model_path_gltf);

	createVertexBuffer();

	createIndexBuffer();

	createUniformBuffers();

	createDescriptorPool();

	createDescriptorSets();

	createSyncObjects();

}
void mainLoop(){
	PRINT_FNAME;

	start_time = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
		drawFrame();
    }

	vkDeviceWaitIdle(device);
	
}
void cleanup(){
	PRINT_FNAME;

	if(enableValidationLayers)
	{
		PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != NULL) {
			func(instance, debugUtilsMessengerEXT, NULL);
		}
	}
 
	//DEVICE
 
	vkDestroySwapchainKHR(device, swapchain,NULL);

	for(int i=0;i<swapchainImageViewCount;i++){
		
		vkDestroyImageView(device, swapchainImageViews[i],NULL);
	}
  
	vkDestroyRenderPass(device, renderPass, NULL);
	
	for(int i=0;i < MAX_FRAMES_IN_FLIGHT;i++)
	{
		vkDestroySemaphore(device,presentCompleteSemaphore[i],NULL);
	
		vkDestroySemaphore(device,renderFinishedSemaphore[i],NULL);

		vkDestroyFence(device,inFlightFences[i],NULL);
	}
	
	vkDestroyImageView(device, textureImageView,  NULL);
	
	vkDestroyImage(device, depthImage, NULL);

	vkDestroyImageView(device,depthImageView, NULL);

	vkFreeMemory(device, depthImageMemory, NULL);

	vkDestroySampler(device, textureSampler, NULL);

	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, NULL);

	vkDestroyDescriptorPool(device, descriptorPool, NULL);

	vkDestroyCommandPool(device, graphicsCommnadPool,NULL);

	vkDestroyCommandPool(device, transferCommnadPool,NULL);
	
	vkDestroyPipeline(device, graphicsPipeline,NULL);

	vkDestroyPipelineLayout(device,pipelineLayout, NULL);

	vkDestroyShaderModule(device,shaderModuleFrag,NULL);
	
	vkDestroyShaderModule(device,shaderModuleVert,NULL);

	vkDestroyBuffer(device,vertextBuffer,NULL);

	vkFreeMemory(device, vertexBufferMemory, NULL);

	vkDestroyBuffer(device,indexBuffer,NULL);

	vkFreeMemory(device, indexBufferMemory, NULL);

	vkDestroyImage(device, textureImage, NULL);

	vkFreeMemory(device, textureImageMemory, NULL);

	clearUniformBuffers();

	vkDestroyDevice(device, NULL);

	// INSTANCE

	vkDestroySurfaceKHR(instance, surface,NULL);

	vkDestroyInstance(instance,NULL);

	
	glfwDestroyWindow(window);

    glfwTerminate();


	free(vertices);
	free(indices);

}
void initWindow(){
	printf("%s %d %d\n",__FUNCTION__, WIDTH, HEIGHT );

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
 	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	
	
	
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);

 
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

int main(){
	printf("%s\n", __FUNCTION__ );
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();


	return 0;
};