
#include "cglm/vec3.h"
#include "gm_array2.h"
#include "gm_list.h"
#include <vulkan/vulkan_core.h>
#include <time.h>

#include "shaders/shader_inc.glsl"



#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>





#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "cglm/cglm.h"
#include "cglm/cam.h"
#include "cglm/types.h"
#include "cglm/mat4.h"
#include "cglm/util.h"
#include "cglm/affine-pre.h"


#define CGLTF_IMPLEMENTATION 
#include "cgltf/cgltf.h"



#include <math.h>

#include <stdio.h>
#ifdef _WIN32
#include <direct.h>
#include <stdlib.h>
#else
#include <unistd.h>   

#endif
#include <limits.h>   



 


const uint32_t WIDTH = 800;

const uint32_t HEIGHT = 600;

#define GM_INDEX_UNUSED UINT32_MAX

#define PRINT_FNAME printf("Call to: %s\n",__FUNCTION__)


#define ARR_LEN(A) sizeof(A)/sizeof(*A)

#define EXIT_CLEAN(msg)\
	do{\
		printf("ERROR: %s\n",msg);\
		cleanup();\
		exit(1);\
	}while(0)\

#define GLM_VEC3_COPY(dst,src)\
	do{\
		dst[0]=src[0];\
		dst[1]=src[1];\
		dst[2]=src[2];\
	}while(0)\

#define GLM_MAT4_COPY(dst,src) memcpy(dst,src,sizeof(mat4))

#define GLM_VEC2_COPY(dst,src)\
	do{\
		dst[0]=src[0];\
		dst[1]=src[1];\
	}while(0)\

#define GLM_VEC4_COPY(dst,src)\
	do{\
		dst[0]=src[0];\
		dst[1]=src[1];\
		dst[2]=src[2];\
		dst[3]=src[3];\
	}while(0)\

#define GLM_VEC4_SET(dst,a,b,c,d)\
	do{\
		dst[0]=a;\
		dst[1]=b;\
		dst[2]=c;\
		dst[3]=d;\
	}while(0)\

#define GLM_VEC3_SET(dst,a,b,c)\
	do{\
		dst[0]=a;\
		dst[1]=b;\
		dst[2]=c;\
	}while(0)\


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#define MAX_IMAGE_VIEWS 16

#define MAX_FRAMES_IN_FLIGHT 2

#define TOTAL_3D_OBJECT 30

#define TOTAL_2D_OBJECT 1

#define VIEW_PERSPECTIVES 3






uint8_t  texture0[] = {
	0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x20, 0x20, 0x20, 0x0d, 0x49, 0x48, 0x44, 0x52, 0x20, 0x20, 0x20, 0x02, 0x20, 0x20, 0x20, 0x02, 0x08, 0x06, 0x20, 0x20, 0x20, 0x72, 0xb6, 0x0d, 0x24, 0x20, 0x20, 0x20, 0x01, 0x73, 0x52, 0x47, 0x42, 0x01, 0xd9, 0xc9, 0x2c, 0x7f, 0x20, 0x20, 0x20, 0x04, 0x67, 0x41, 0x4d, 0x41, 0x20, 0x20, 0xb1, 0x8f, 0x0b, 0xfc, 0x61, 0x05, 0x20, 0x20, 0x20, 0x20, 0x63, 0x48, 0x52, 0x4d, 0x20, 0x20, 0x7a, 0x26, 0x20, 0x20, 0x80, 0x84, 0x20, 0x20, 0xfa, 0x20, 0x20, 0x20, 0x80, 0xe8, 0x20, 0x20, 0x75, 0x30, 0x20, 0x20, 0xea, 0x60, 0x20, 0x20, 0x3a, 0x98, 0x20, 0x20, 0x17, 0x70, 0x9c, 0xba, 0x51, 0x3c, 0x20, 0x20, 0x20, 0x09, 0x70, 0x48, 0x59, 0x73, 0x20, 0x20, 0x2e, 0x23, 0x20, 0x20, 0x2e, 0x23, 0x01, 0x78, 0xa5, 0x3f, 0x76, 0x20, 0x20, 0x20, 0x18, 0x49, 0x44, 0x41, 0x54, 0x08, 0xd7, 0x05, 0xc1, 0x01, 0x01, 0x20, 0x20, 0x20, 0x82, 0xa0, 0x4e, 0xfa, 0x7f, 0x0a, 0xc1, 0x2a, 0x30, 0xa8, 0x1c, 0x69, 0xa8, 0x0b, 0xf5, 0xa9, 0xc4, 0xf1, 0x86, 0x20, 0x20, 0x20, 0x20, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};


struct TextureRes tex_res0;





uint32_t validationLayerCnt = 1;
const char * validationLayers[]={
	"VK_LAYER_KHRONOS_validation",
};

uint32_t requiredDeviceExtensionCnt = 1;
const char * requiredDeviceExtensions[]={
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
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

VkFormat swapchainSurfaceFormat = -1;

uint32_t swapchainSurfaceColorSpace = -1;

VkSwapchainKHR swapchain = NULL;

uint32_t swapchainImageCount = 0 ;

VkImage swapchainImages[MAX_IMAGE_VIEWS];

uint32_t swapchainImageViewCount = 0;

VkImageView swapchainImageViews[MAX_IMAGE_VIEWS];





VkDescriptorSetLayout descriptorSetLayout4;





struct GameObject * gameObjectsA;
struct GameObject * gameObjectsB;
struct GameObject * gameObjectsC;



/**

Object = primitive[]
primitive = mesh



*/
struct DataInstanceHUD{
    vec2 position;
    vec2 size;
    vec4 color;
    uint32_t material_id;
    uint32_t object_id;
	uint32_t flags;
};

struct DataInstanceMesh{
	mat4 model;
	vec4 color;
	uint32_t object_id;
	uint32_t material_id;
	uint32_t flags;
	//add padding
};
struct DataMaterial{
	vec4 baseColorFactor;
    float metallicFactor;
    float roughnessFactor;

		// in shader
	uint32_t baseColorTexture_idx;
    uint32_t metallicRoughnessTexture_idx;
    uint32_t normalTexture_idx;
	uint32_t emissiveTexture_idx;

	//uint32_t pad;
};


struct DataGlobal {
	vec2 screenSize;
	float time;
	float deltaTime;
	//add paddng
};

struct DataCamera {
    mat4 view;
    mat4 proj;
    vec4 position;
	//add padding
};


struct DataLight {
	mat4 view;
	mat4 proj;
	vec4 position;
    vec4 color;
    float intensity;
	//add padding
};

struct Range{
	uint32_t offset;
	uint32_t count;
	uint32_t cap;
};
struct Primitive{
	struct Range vertices;
	struct Range indices;

	uint32_t material_idx;
	

};
struct Mesh {
	struct Range primitives;
};

struct Model{
	char * name;
	struct Range meshs;
};
struct GameObject3D{
	uint32_t id;

	struct Model model;
	
	uint32_t inst_idx;
	uint32_t light_idx;
	
	
};
// struct stbi_image_data{

// 	int w; 
// 	int h; 
// 	int channels;
// 	stbi_uc* pixels ;

// };

struct GameObjectHUD{
	uint32_t id;
	uint32_t material_idx;
	uint32_t mesh_idx;
	uint32_t inst_idx;//hud

};

struct Pipeline{
	
	char * frag_path;

	char * vert_path;

	VkShaderModule shaderModuleFrag ;
	
	VkShaderModule shaderModuleVert ;
	
	VkPipelineLayout pipelineLayout ;
	
	VkPipeline graphicsPipeline ;

	uint32_t colorAttachmentCount;


	


	VkRenderingInfo renderingInfo;

	

};


struct GmArray arrayPipelines;




uint32_t frameIndex = 0;


VkCommandPool graphicsCommnadPool = NULL;

VkCommandBuffer transferCommandBuffers;

VkCommandPool transferCommnadPool = NULL;

struct Allocation{
    VkDeviceMemory memory;
    VkDeviceSize size;
    VkDeviceSize offset;
    void* mapped;
} ;
struct ImageRes{
    VkImage handle;
    VkImageView view;
	struct  Allocation alloc;
    VkFormat format;
    uint32_t width;
    uint32_t height;
	uint32_t mipLevels;
} ;


VkFence transferFence;

struct BufferRes{
	VkBuffer handle ;

	struct Allocation alloc;

	VkBufferUsageFlags usage;

	

};

uint32_t data_buffer_idx= 0xff000000;
struct DataBuffer{
	uint32_t idx;
	uint32_t size;
	uint8_t  data[];
};




char * texture_paths []= {
	"models_gltf/viking_room.png",
	"models_gltf/viking_room2.png",
	"models_gltf/wooden_small.jpg",
	"models_gltf/wooden_small.jpg",
	"textures/cross32x32.png",
	"textures/cross32x32a.png",
};


struct TextureRes{

	struct ImageRes image;

	VkSampler textureSampler;

	uint32_t textureIdx;


};

struct Texture{
	uint32_t idx;
};




struct BufferRes ssbo_material;

VkDescriptorSet descriptorSets2 [MAX_FRAMES_IN_FLIGHT];

struct Frame{




	VkCommandBuffer  graphicsCommandBuffers;

	VkSemaphore presentCompleteSemaphore;

	VkSemaphore renderFinishedSemaphore;

	VkFence inFlightFence ;


	
	VkDescriptorSet globalSets;

	VkDescriptorSet instanceSets;

	VkDescriptorSet shadowSets;

	
	// -- V1
	// struct BufferRes ubo_ViewProjection;
	// struct BufferRes ubo_Lights;
	// struct BufferRes ssbo_objectIds;
	// struct BufferRes ssbo_models;
	// struct BufferRes ssbo_colors;
	
	
	// -- V2
	struct BufferRes ubo_global;
	struct BufferRes ubo_camera;
	struct BufferRes ubo_light;
	struct BufferRes ssbo_instance_mesh;
	struct BufferRes ssbo_instance_hud;






	struct ImageRes deapth;


	struct TextureRes shadow;

	VkDescriptorImageInfo shadow_descriptor_info;

	VkDescriptorSet* descriptorSets;

	



	struct{
		struct ImageRes image;
		
		struct BufferRes buffer;

		uint32_t pickedID;
	} pick;

	


};

struct Frame frames[MAX_FRAMES_IN_FLIGHT] ={};

struct GmList list_BufferRes;

struct BufferRes vertexBuffer;
struct BufferRes indexBuffer;



struct ModelVertexData2
{
	


	struct Vertex* vertices;
	uint32_t vertexCount;

	uint32_t* indices;
	uint32_t indexCount;


	// uint32_t indices_num;

	/**
	1. read each mesh here
	2. create vertex, index buffers
	3. copy data with offsets
		3.1 while creating struct mesh instances
	
	*/

};


/*
initialized with default texture
*/
VkDescriptorImageInfo  texture_infos_3d[MAX_TEXTURES_3D] = {};
VkDescriptorImageInfo  texture_infos_2d[MAX_TEXTURES_2D] = {};


/*
fix shader to array textures[]
*/

#define MODEL_NUM 2

char * model_names [] ={
	"viking_room",
	"box"
};
char  * model_paths [] = {
	"models_gltf/viking_room.gltf",
	"models_gltf/box.gltf",
};


// struct GmArray array_vertex_data_3D;
// struct GmArray array_vertex_data_2D;


// struct ModelVertexData2 modelVertexData_3D[MODEL_NUM]={};


// struct ModelVertexData2 modelVertexData_HUD2 = {};




/* END UNIFORM BUFFERS */

VkDescriptorPool descriptorPool = NULL;






VkDescriptorPool descriptorPool2;

VkDescriptorSetLayout globalLayout;
VkDescriptorSetLayout materialLayout;
VkDescriptorSetLayout instanceLayout;
VkDescriptorSetLayout samplerLayout;
VkDescriptorSetLayout shadowLayout;



VkDescriptorSet materialSet;
VkDescriptorSet samplerSet;


struct DataGlobal *global_data;
struct DataCamera *camera_data;
struct DataLight *light_data;




struct GameObject3D * global_light;









bool leftPressed = false;
bool firstMouse = true;
float yaw   = -90.0f;	
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

float prevX, prevY;


vec3 cameraPos   = {1.5f, 0.5f, -1.6f};
vec3 cameraFront = {-0.7f, 0.0f, 0.7f};
vec3 cameraUp    = {0.0f, 1.0f, 0.0f};


struct PushConst{
	int texIdx;
	int hasColor;
	int objectId;
};


struct PushConst2D{
	uint32_t texIdx;
	uint32_t hasColor;
};

struct Vertex{

	vec3 pos;
	vec3 norm;
	vec2 texCoords;
};

struct Vertex2D{

	vec2 pos;
	vec2 texCoords;
};









char * model_path_gltf	= 	"models_gltf/viking_room.gltf";
char * model_path_bin	= 	"models_gltf/viking_room.bin";
char * model_path_text	= 	"models_gltf/viking_room.png";




#define INSTANCE_NUM 6

uint32_t instanceNum = INSTANCE_NUM;





struct SSBO_ObjectID{
	uint32_t objectId;
};
struct SSBO_Model{
	mat4 model;
};
struct SSBO_Colors{
	vec4 color;
};

struct UBO_ViewProjection {

    mat4 view;
    mat4 proj;
};

uint32_t UBO_DirectionLightCnt = 1;

struct UBO_DirectionLight {
  vec4 lightPos; 
  vec4 viewPos; 
  vec4 lightColor;
} UBO_DirLight;

float startTime = 0.0f;
float deltaTime = 0.0f;
float lastTime = 0.0f;


uint32_t uboPointLightCnt =0 ;

struct UBOPointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 


// struct Object3DTransforms{
// 	vec3 position;
// 	vec3 scale;
// 	vec3 rotation;

// };


// struct GameObject {



// 	uint32_t vertexIdx;

// 	uint32_t textureIdx;

// 	struct GmArrayView arrayViewUboModel;
	
// 	struct GmArrayView arrayViewUboObjectIds;

// 	// struct UBOPointLight * uboLight;

	
// 	struct GmArrayView arrayViewGameObjectInstances;


// };
// struct Range{
// 	uint32_t start;
// 	uint32_t cap;
// 	uint32_t len;
// };
// struct Object3D_Inst{
// 	bool visible;
	
// 	uint32_t id;
// };
// struct Hud_Inst{
// 	bool visible;

// 	uint32_t id;
// };
// struct Transforms{
// 	vec4 position;
// 	vec4 scale;
// 	vec4 rotation;
// };
// struct ObjColor{
// 	vec4 color;
// };
// struct ObjectID{
// 	uint32_t objectId;
// };
// struct ObjectType {


// 	uint32_t geometry_idx;

// 	uint32_t material_idx;

// 	struct Range object_ids;
// 	struct Range instances;
// 	struct Range transforms;
// 	struct Range color;

// };
// struct ObjectHUDType {


// 	uint32_t geometry_idx;

// 	uint32_t material_idx;

// 	struct Range object_ids;
// 	struct Range instances;
// 	struct Range transforms;
// 	struct Range color;

// };



// struct GmArray arrayGameObjectInstances;



// struct GmArray arrayGameObjects;


// struct GmArray array_ssbo_models3d;

// struct GmArray array_ssbo_models2d;


// struct GmArray arraySBO_ObjectIds_1;


// struct GmArray arrayColors_2;



// -- v3 Data
struct GmArray array_inst_mesh_data;
struct GmArray array_inst_hud_data;
struct GmArray array_global_data;
struct GmArray array_camera_data;
struct GmArray array_light_data;
struct GmArray array_material_data;
struct GmArray array_obj_3d;
struct GmArray array_obj_hud;
struct GmArray array_mesh;

struct GmArray array_model;
struct GmArray array_textures;

struct GmArray array_vertex_data_3D;
struct GmArray array_vertex_indices_3D;

struct GmArray array_vertex_data_2D;
struct GmArray array_vertex_indices_2D;

struct GmArray array_primitives;

// struct GmArray array_texture_info;


struct GmArray arrayTextures_2D;

struct GmArray arrayTextures_3D;

struct GmList list_load_model;
struct GmList list_image_buffers;

struct GmList list_obj3d;



//-- V2 game object


uint32_t total_objects3d = 0;
uint32_t total_objects2d = 0;


// struct GmArray array_transforms;
// struct GmArray array_object_ids;
// struct GmArray array_object3d_inst;
// struct GmArray array_hud_inst;
// struct GmArray array_obj_color;



// struct ObjectType obj_rooms_a;
// struct ObjectType obj_rooms_b;
// struct ObjectType obj_cubes;
// struct ObjectType obj_light;




// struct ObjectType obj2d_cross;


// -- V2 game object





void cleanAllocation(struct Allocation * all);
void cleanImageRes(struct ImageRes * img);
void cleanBuffer(struct BufferRes * buff);
void cleanTextureRes(struct TextureRes * tex);

void initVariables();

void freeVariables();


// -- V2



void create_Object3D_with_model_ref( struct GameObject3D * out, struct Model * model);
void create_Object3D_with_model_name( struct GameObject3D * out, char * model_name);



struct BufferRes * iterateBuffer(
	uint32_t binding,
	uint32_t contextLen,
	uint32_t *context
);






struct BufferRes* buffer_set(
	struct BufferRes * buffer_res,
	VkBuffer handle,
	uint32_t size,
	uint32_t offset,
	
	VkBufferUsageFlags usage,
	VkDeviceMemory memory,
	void * mapped
);

struct DataBuffer * alloc_data_buffer(uint32_t size);
void copy_to_data_buffer(struct DataBuffer * dst,void * src, uint32_t size);

void createBufferRes(
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	struct BufferRes * buffer
);
void createDepthResources4(struct ImageRes * image, bool sampled_bit) ;
void createImageView4(struct ImageRes * tex,  VkFormat format, VkImageAspectFlagBits aspectFlags);

void createTextureImage4(
	struct TextureRes * tex
	
);
void loadModels();

void loadModels2();

struct Model * loadModel2( char *fname );

void createDepthResources3(VkImage *depthImage,VkImageView *depthImageView,VkDeviceMemory *depthImageMemory);

void createDepthImages();

void createTextures();

void createPipelines();

void createVertexBuffer2(

	uint32_t verticesNum,
	void * vertices,
	uint32_t data_size,
	struct BufferRes *out

);

void createIndexBuffer2(
	uint32_t indicesNum,
	uint32_t * indices,
	struct BufferRes *out

);

void procMouseInput(GLFWwindow* window);

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);

uint32_t createShaderFromFile(const char * path, uint8_t** buffer);


void loadModel(
	
	char *fname,
	uint32_t *indicesNum,
	uint32_t ** indices,
	uint32_t *verticesNum,
	struct Vertex ** vertices
) ;

void createUniformBuffers();

void createBuffer(
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkBuffer *buffer,
	VkDeviceMemory *bufferMemory
);

uint32_t findMemoryType(
	uint32_t typeFilter, 
	VkMemoryPropertyFlags properties
);

struct Model * find_ModelByName(char * name,uint32_t *start_pos);

void recreateSwapChain();

void cleanup();

void createVertexBuffer(
	uint32_t verticesNum,
	void * vertices,
	uint32_t data_size,
	VkBuffer *vertexBuffer, 
	VkDeviceMemory *vertexBufferMemory

);

void createIndexBuffer(
	uint32_t indicesNum,
	uint32_t * indices,
	VkBuffer *indexBuffer, 
	VkDeviceMemory *indexBufferMemory

);

void clearUniformBuffers();



void createShaderDescriptorSetLayout();

void createTextureImage_from_data(struct TextureRes * tex, uint8_t* data,uint32_t size);

void createPickImage(

);


struct Model * createModel2D(
	struct Vertex2D * vertex,
	uint32_t vertex_num,
	uint32_t * index,
	uint32_t index_num,
	uint32_t material_idx
);

void generateMipmaps(
	VkImage* image, 
	VkFormat imageFormat, 
	int32_t texWidth, 
	int32_t texHeight, 
	uint32_t mipLevels
) ;

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

void createShadowResource(struct TextureRes * image);

void beginSingleTimeCommands(VkCommandBuffer commandBuffer);

void endSingleTimeCommands(VkCommandBuffer commandBuffer);


void transitionImageLayout(
	VkCommandBuffer cmdBuffer,
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

void createTextureSamplerShadow(VkSampler * sampler);

void createTextureSampler(VkSampler * sampler);

void createDepthResources();

VkFormat findSupportedFormat(VkFormat *formats, uint32_t len, VkImageTiling tiling, VkFormatFeatureFlags features);

VkFormat findDepthFormat();

bool hasStencilComponent(VkFormat format);

void processInput(GLFWwindow *window);
 
void createGraphicsPipeline(struct  Pipeline * pipeline );


float rand_float(float from,float to)
{
	
	// printf("random : %f %f\n",from,to);
    return from + (to - from) *((float)rand() / (float)RAND_MAX);
	
}
int32_t rand_int32(int32_t from, int32_t to) {
	return rand_float(from,to);
}

void create_Object3D_with_model_name( struct GameObject3D * out, char * model_name){

	struct Model * m = find_ModelByName(model_name, 0);
	out = gmArrayNew(&array_obj_3d);
	out->model = *m;

	struct DataInstanceMesh * inst = gmArrayNew(&array_inst_mesh_data);
	glm_mat4_identity(inst->model);
	inst->material_id = m.?

}


// void create_Object(uint32_t material_id, uint32_t geometry_id, uint32_t inst_cnt, struct ObjectType * out){

	


// 	out = malloc(sizeof(struct ObjectType));
// 	memset(out, 0, sizeof(struct ObjectType));

// 	out->geometry_idx = geometry_id;
// 	out->material_idx = material_id;
	
// 	{

// 		float rand_from = -10.0;
// 		float rand_to = 10.0;

// 		out->transforms.start = array_transforms.len;
// 		out->transforms.cap = inst_cnt;
// 		gmArrayNewN(&array_transforms,inst_cnt);
// 		int start=out->transforms.start;
// 		int cap = start + inst_cnt;
		
// 		for(int i = start;i<cap;i++ ){
// 			struct Transforms * inst = gmArrayGet(&array_transforms,i);;
// 			GLM_VEC3_SET(inst->position, rand_float(rand_from,rand_to), 0.0, rand_float(rand_from,rand_to));
// 			GLM_VEC3_SET(inst->scale, 1.0, 1.0, 1.0);
// 			// GLM_VEC3_SET(inst->rotation, 0.0, 0.0, 0.0);
// 		}
// 	}





// 	{
// 		out->object_ids.start = array_object_ids.len;
// 		out->object_ids.cap = inst_cnt;
	
// 		gmArrayNewN(&array_object_ids,inst_cnt);

// 		int start=out->object_ids.start;
// 		int cap = start + inst_cnt;
		
// 		for(int i = start;i<cap;i++ ){
// 			struct ObjectID * inst = gmArrayGet(&array_object_ids,i);
			
// 			inst->objectId = i;
// 		}

// 	}
// }

// void create_Object3D(uint32_t material_id, uint32_t geometry_id, uint32_t inst_cnt, struct ObjectType * out){

	
// 	create_Object(material_id, geometry_id, inst_cnt, out);

// 	total_objects3d += inst_cnt;
	
// 	gmListPushBack(&list_obj3d,out);

// 	{
// 		out->instances.start = array_object3d_inst.len;
// 		out->instances.cap = inst_cnt;
// 		gmArrayNewN(&array_object3d_inst,inst_cnt);
// 		int start=out->instances.start;
// 		int cap = start + inst_cnt;
		
// 		for(int i = start;i<cap;i++ ){
// 			struct Object3D_Inst * inst = 	gmArrayGet(&array_object3d_inst,i);
// 			inst->visible = true;
		
// 			inst->id = i;
// 		}
// 	}


// }

// void create_ObjectHUD(uint32_t material_id, uint32_t geometry_id, uint32_t inst_cnt, struct ObjectType * out){


	
// 	create_Object(material_id, geometry_id, inst_cnt, out);

// 	total_objects2d += inst_cnt;


// 	{
// 		out->instances.start = array_hud_inst.len;
// 		out->instances.cap = inst_cnt;
// 		gmArrayNewN(&array_hud_inst,inst_cnt);
// 		int start=out->instances.start;
// 		int cap = start + inst_cnt;
		
// 		for(int i = start;i<cap;i++ ){
// 			struct Hud_Inst * inst = 	gmArrayGet(&array_hud_inst,i);
// 			inst->visible = true;
// 			inst->id = i;
// 		}
// 	}



// }




// void disable_Models(struct SSBO_Model * array, struct Range slice, uint32_t pos){
// 	assert(slice.cap > pos);
	
// 	struct SSBO_Model*  target =  (array + slice.start + pos);
// 	struct SSBO_Model*  last = (array + slice.start + slice.len - 1);

// 	memcpy(target, last, sizeof(struct SSBO_Model));


// }
// void disable_HudInst(struct Hud_Inst * instances, struct Range slice, uint32_t pos){
// 	assert(slice.cap > pos);
	
// 	struct Hud_Inst*  target =  (instances + slice.start + pos);
// 	struct Hud_Inst*  last = (instances + slice.start + slice.len - 1);

// 	memcpy(target, last, sizeof(struct Object3D_Inst));
// }
// void disable_Object3DInst(struct Object3D_Inst * instances, struct Range slice, uint32_t pos){
// 	assert(slice.cap > pos);
	
// 	struct Object3D_Inst*  target =  (instances + slice.start + pos);
// 	struct Object3D_Inst*  last = (instances + slice.start + slice.len - 1);

// 	memcpy(target, last, sizeof(struct Object3D_Inst));
// }


struct BufferRes* buffer_set(
	struct BufferRes * buffer,
	VkBuffer handle,
	uint32_t size,
	uint32_t offset,
	
	VkBufferUsageFlags usage,
	VkDeviceMemory memory,
	void * mapped

){
	buffer->handle = handle;
	buffer->alloc.mapped = mapped;
	buffer->alloc.memory = memory;
	buffer->alloc.offset = offset;
	buffer->alloc.size = size;
	buffer->usage = usage;
	
	return buffer;
}
 



void procMouseInput(GLFWwindow* window){

	double xpos,ypos;
	double dx,dy;
	glfwGetCursorPos(window, &xpos, &ypos);

	

	dx = xpos - prevX;
	dy = ypos - prevY;


	printf("dx:%f, dy:%f\n",dx,dy);

	prevX = xpos;
	prevY = ypos;
	
	glfwSetCursorPos(window, WIDTH >> 1, HEIGHT >> 1);



	float sensitivity = 0.1f; 

    dx *= sensitivity;
    dy *= sensitivity;

    yaw += dx;
    pitch += dy;

    

    if (pitch > 89.0f)
        pitch = 89.0f;
	
    if (pitch < -89.0f)
        pitch = -89.0f;

	vec3 front;
	
	front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

	
	GLM_VEC3_COPY(cameraFront,front);

}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn){

	



	float xpos =  xposIn;
    float ypos =  yposIn;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; 
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

 
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

	vec3 front;
	
	front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

	
	GLM_VEC3_COPY(cameraFront,front);
}

void processInput(GLFWwindow *window){

	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	leftPressed = false;
	
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ){
		leftPressed = true;
		
	}


    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		 
		glm_vec3_muladds(cameraFront, cameraSpeed, cameraPos);
		
	}

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm_vec3_mulsubs(cameraFront, cameraSpeed, cameraPos);
		 
	}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
		vec3 temp;
		glm_vec3_crossn(cameraFront, cameraUp, temp);
		glm_vec3_mulsubs(temp,cameraSpeed,cameraPos);
		
	}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
		vec3 temp;
		glm_vec3_crossn(cameraFront, cameraUp, temp);
		glm_vec3_muladds(temp, cameraSpeed, cameraPos);
		
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {

		glm_vec3_muladds(cameraUp, cameraSpeed, cameraPos);
		
	}
	
}

void createShadowImages(){

	
	for(int i=0; i< MAX_FRAMES_IN_FLIGHT; i++)
	{
		struct  Frame * frame = &frames[i];

		// createDepthResources4(&frame->shadow.image,true);
		createShadowResource(&frame->shadow);
		

		frame->shadow_descriptor_info = (VkDescriptorImageInfo){
			.sampler =frame->shadow.textureSampler,
			.imageView =frame->shadow.image.view,
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		};
	}
	
}

void createDepthImages(){

	
	for(int i=0; i< MAX_FRAMES_IN_FLIGHT; i++)
	{
		struct  Frame * frame = &frames[i];

		createDepthResources4(&frame->deapth,false);
		
	}
	
}
void createTextures2(){



	
	createTextureImage_from_data(&tex_res0, texture0,sizeof(texture0));
	createImageView4(&tex_res0.image,VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	createTextureSampler(&tex_res0.textureSampler);


	VkDescriptorImageInfo fallback = {
		.imageView = tex_res0.image.view,
		.sampler = tex_res0.textureSampler,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	};



	for (int i = 0; i < MAX_TEXTURES_2D; i++)
	{
		texture_infos_2d[i] = fallback;
	}

	for (int i = 0; i < MAX_TEXTURES_3D; i++)
	{
		texture_infos_3d[i] = fallback;
	}



	struct GmNode * n = list_image_buffers.head;
	while( n != NULL)
	{
		struct DataBuffer * db = n->data;

		struct TextureRes * texture = gmArrayNew(&arrayTextures_3D);
		texture->textureIdx = arrayTextures_3D.len - 1;
		createTextureImage_from_data(texture, db->data, db->size);
		
		// createTextureImage4(texture);
		createImageView4(&texture->image,VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		createTextureSampler(&texture->textureSampler);


		VkDescriptorImageInfo * di =  &texture_infos_3d [db->idx];
		di->imageView = texture->image.view;
		di->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		di->sampler = texture->textureSampler;

		n = n->next;
	}

	gmListFree(&list_image_buffers);
	
	// for( int i=0;i < arrayTextures_3D.len ;i++)
	// {
	// 	struct TextureRes * t = gmArrayGet(&arrayTextures_3D, i);
	// 	createTextureImage4(t);
		
		
	// 	t->textureIdx = i;

	// 	createImageView4(&t->image,VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	// 	createTextureSampler(&t->textureSampler);
	// }
	// printf("arrayTextures_2D.len %d\n",arrayTextures_2D.len);
	// for( int i=0;i < arrayTextures_2D.len ;i++)
	// {
	// 	struct TextureRes * t = gmArrayGet(&arrayTextures_2D, i);
	// 	createTextureImage4(t);
	// 	t->textureIdx = i;
	// 	createImageView4(&t->image,VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		
	// 	createTextureSampler(&t->textureSampler);
	// }

}
// void createTextures(){

	
// 	for( int i=0;i < arrayTextures_3D.len ;i++)
// 	{
// 		struct TextureRes * t = gmArrayGet(&arrayTextures_3D, i);
// 		createTextureImage4(t);
		
		
// 		t->textureIdx = i;

// 		createImageView4(&t->image,VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

// 		createTextureSampler(&t->textureSampler);
// 	}
// 	printf("arrayTextures_2D.len %d\n",arrayTextures_2D.len);
// 	for( int i=0;i < arrayTextures_2D.len ;i++)
// 	{
// 		struct TextureRes * t = gmArrayGet(&arrayTextures_2D, i);
// 		createTextureImage4(t);
// 		t->textureIdx = i;
// 		createImageView4(&t->image,VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		
// 		createTextureSampler(&t->textureSampler);
// 	}

// }

void updateLight(struct DataLight *light, vec3 pos,vec3 view_pos){

		GLM_VEC3_COPY(light_data->position, pos);

		glm_mat4_identity(light_data->view);
	
		

		vec3 LightCenter;
		
		glm_vec3_add(light_data->position, view_pos, LightCenter);
	
		glm_lookat(light_data->position, LightCenter, cameraUp, light_data->view);
	
		glm_perspective(glm_rad(45.0f), (float)swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 40.0f , light_data->proj);
	
		light_data->proj[1][1] *= -1;
}


struct DataBuffer * alloc_data_buffer(uint32_t size){
	struct DataBuffer *db = malloc(sizeof(*db) + size);
	// db->idx = data_buffer_idx ++;
	return db;
}
void copy_to_data_buffer(struct DataBuffer * dst,void * src, uint32_t size){

	memcpy((uint8_t *)(dst + 1), src, size);
}

void updateGlobal(struct DataGlobal *global){
	global->screenSize[0] =  swapChainExtent.width ;
	global->screenSize[1] =  swapChainExtent.height ;
	global->deltaTime = deltaTime;
	global->time = lastTime;
}
void updateCamera(struct DataCamera *camera){
	vec3 cameraCenter;
		
	glm_vec3_add(cameraPos, cameraFront, cameraCenter);

	glm_lookat(cameraPos, cameraCenter, cameraUp, camera->view);


	glm_perspective(glm_rad(45.0f), (float)swapChainExtent.width / swapChainExtent.height, 0.1f, 40.0f , camera->proj);

	camera->proj[1][1] *= -1;
}

void initVariables(){
	PRINT_FNAME;


	gmListInit(&list_BufferRes);

	
// 	struct GmArray array_inst_mesh_data;
// struct GmArray array_inst_hud_data;
// struct GmArray array_global_data;
// struct GmArray array_material_data;



	gmArrayInit(&array_inst_mesh_data,sizeof(struct DataInstanceMesh),16,_Alignof(struct DataInstanceMesh));
	gmArrayInit(&array_inst_hud_data,sizeof(struct DataInstanceHUD),16,_Alignof(struct DataInstanceHUD));
	gmArrayInit(&array_global_data,sizeof(struct DataGlobal),16,_Alignof(struct DataGlobal));
	gmArrayInit(&array_primitives,sizeof(struct Primitive),16,_Alignof(struct Primitive));
	gmArrayInit(&array_camera_data,sizeof(struct DataCamera),16,_Alignof(struct DataCamera));
	gmArrayInit(&array_light_data,sizeof(struct DataLight),16,_Alignof(struct DataLight));
	gmArrayInit(&array_material_data,sizeof(struct DataMaterial),16,_Alignof(struct DataMaterial));
	gmArrayInit(&array_obj_3d,sizeof(struct GameObject3D),16,_Alignof(struct GameObject3D));
	gmArrayInit(&array_obj_hud,sizeof(struct GameObjectHUD),16,_Alignof(struct GameObjectHUD));
	// gmArrayInit(&array_texture_info,sizeof(struct VkDescriptorImageInfo),1024,_Alignof(struct VkDescriptorImageInfo));
	gmArrayInit(&array_model,sizeof(struct Model),16,_Alignof(struct Model));


	gmArrayInit(&array_vertex_data_3D,sizeof(struct Vertex),1024*16,_Alignof(struct Vertex));
	gmArrayInit(&array_vertex_indices_3D,sizeof(uint32_t),1024*16,_Alignof(uint32_t));
	gmArrayInit(&array_vertex_indices_2D,sizeof(uint32_t),1024*16,_Alignof(uint32_t));
	gmArrayInit(&array_vertex_data_2D,sizeof(struct Vertex2D),64,_Alignof(struct Vertex2D));
	gmArrayInit(&array_textures,sizeof(struct Texture),64,_Alignof(struct Texture));

	// gmArrayInit(&array_vertex_data_2D,sizeof(struct ModelVertexData2),10,_Alignof(struct ModelVertexData2));


	
	gmListInit(&list_obj3d);
	gmListInit(&list_image_buffers);

	// gmArrayInit(&array_hud_inst,sizeof(struct Hud_Inst),10,_Alignof(struct Hud_Inst));
	// gmArrayInit(&array_object3d_inst,sizeof(struct Object3D_Inst),10,_Alignof(struct Object3D_Inst));
	// gmArrayInit(&array_obj_color,sizeof(struct ObjColor),10,_Alignof(struct ObjColor));
	// gmArrayInit(&array_object_ids,sizeof(struct ObjectID),10,_Alignof(struct ObjectID));
	// gmArrayInit(&array_transforms,sizeof(struct Transforms),10,_Alignof(struct Transforms));



	// uint32_t total_objects = TOTAL_3D_OBJECT + TOTAL_2D_OBJECT;


	gmArrayInit(&arrayPipelines, sizeof(struct Pipeline), 3,_Alignof(struct Pipeline));


	gmArrayInit(&arrayTextures_2D, sizeof(struct TextureRes), 3,_Alignof(struct TextureRes));


	gmArrayInit(&arrayTextures_3D, sizeof(struct TextureRes), 3,_Alignof(struct TextureRes));


	//gmArrayInit(&arrayGameObjects, sizeof(struct GameObject), 3, _Alignof(struct GameObject));



	
	// gmArrayInit(&array_ssbo_models3d, sizeof(struct SSBO_Model), TOTAL_3D_OBJECT , _Alignof(struct SSBO_Model));

	// gmArrayInit(&array_ssbo_models2d, sizeof(struct SSBO_Model), TOTAL_2D_OBJECT , _Alignof(struct SSBO_Model));

	
	// gmArrayInit(&arraySBO_ObjectIds_1, sizeof(struct SSBO_ObjectID), TOTAL_3D_OBJECT, _Alignof(struct SSBO_ObjectID));

	// gmArrayInit(&arrayGameObjectInstances, sizeof(struct Object3DTransforms), TOTAL_3D_OBJECT, _Alignof(struct Object3DTransforms));

	
	// gmArrayInit(&arrayColors_2, sizeof(vec4), 1, _Alignof(vec4));


	swapChainExtent.width = WIDTH;
	swapChainExtent.height = HEIGHT;

	// {

	// 	struct TextureRes * texRes = gmArrayNew(&arrayTextures_3D);
	

	// 	texRes = gmArrayNew(&arrayTextures_3D);
	


	// 	texRes = gmArrayNew(&arrayTextures_3D);
	
	// }

	
	// {

	// 	struct TextureRes * texRes = gmArrayNew(&arrayTextures_2D);
	

	// 	texRes = gmArrayNew(&arrayTextures_2D);
	


	// 	texRes = gmArrayNew(&arrayTextures_2D);
	
	// }


	// struct Texture * tex0 = gmArrayNew(&array_textures);
	



	global_data = gmArrayNew(&array_global_data);
	camera_data = gmArrayNew(&array_camera_data);
	light_data = gmArrayNew(&array_light_data);



	

	
	yaw = glm_deg(atan2(cameraFront[2], cameraFront[0]));
	pitch = glm_deg(asin(cameraFront[1]));

	//light data
	{

		vec4 v = {1.0,1.0,1.0,1.0};
		GLM_VEC4_COPY(light_data->color, v);
		vec3 pos;
		GLM_VEC3_SET(pos, 1.2f, 1.0f, 2.0f);
		vec3 view_pos;
		GLM_VEC3_SET(view_pos,0.0,0.0,0.0);

		updateLight(light_data, pos, view_pos);
	
	}
	// global data
	{
		updateGlobal(global_data);
	}
	//update camera
	{
		updateCamera(camera_data);
			
	}


	float rand_from = -10.0;
	float rand_to = 10.0;

	for(int i=0; i < TOTAL_3D_OBJECT; i++)
	{

		// struct SSBO_Model m ;
		// struct Object3DTransforms * g = gmArrayNew(&arrayGameObjectInstances);
		// struct SSBO_ObjectID o ;

		// o.objectId = i +1;

		
		// GLM_VEC3_SET(g->position, rand_float(rand_from,rand_to), 0.0, rand_float(rand_from,rand_to));
		

		// glm_mat4_identity(m.model) ;
		// glm_translate(m.model, g->position);

	
		// gmArrayPushValue(&arraySBO_ObjectIds_1,&o);

		// gmArrayPushValue(&array_ssbo_models3d,&m);


		
	}

	
	//LIGHT MODEL?
	{
		// vec4 color ={1.0,0.0,1.0,1.0};
		// gmArrayPushValue(&arrayColors_2,color);
		// struct SSBO_Model m ;
		// glm_mat4_identity(m.model) ;

		// vec4 pos = {(float) WIDTH / 2, (float) HEIGHT / 2, 0.0, 1.0};
		// glm_translate(m.model, pos);

		// gmArrayPushValue(&array_ssbo_models2d,&m);
	}
	


	
	


	uint32_t texture_index = 2;
	uint32_t vertexData_index = 1;


	{
		// gameObjectsA = gmArrayNew(&arrayGameObjects);
	
		// gmArrayViewCreateSlice(&array_ssbo_models3d, &gameObjectsA->arrayViewUboModel ,0, 10);	
		// gmArrayViewCreateSlice(&arrayGameObjectInstances, &gameObjectsA->arrayViewGameObjectInstances ,0, 10);	
		// gmArrayViewCreateSlice(&arraySBO_ObjectIds_1, &gameObjectsA->arrayViewUboObjectIds ,0, 10);	


		// gameObjectsA->textureIdx = texture_index;
		// gameObjectsA->vertexIdx = vertexData_index;

	//	create_Object3D(texture_index, vertexData_index, 10, &obj_cubes);
	}

	texture_index = 1;
	vertexData_index = 0;
	{
		// gameObjectsB = gmArrayNew(&arrayGameObjects);
	
		// gmArrayViewCreateSlice(&array_ssbo_models3d, &gameObjectsB->arrayViewUboModel ,10, 10);	
		// gmArrayViewCreateSlice(&arrayGameObjectInstances, &gameObjectsB->arrayViewGameObjectInstances ,10, 10);	
		// gmArrayViewCreateSlice(&arraySBO_ObjectIds_1, &gameObjectsB->arrayViewUboObjectIds ,10, 10);	
		

		// gameObjectsB->textureIdx = texture_index;
		// gameObjectsB->vertexIdx = vertexData_index;

		//create_Object3D(texture_index, vertexData_index, 10, &obj_rooms_a);
			
	}
	


	texture_index = 0;
	vertexData_index = 0;
	{
		// gameObjectsC = gmArrayNew(&arrayGameObjects);
	
		// gmArrayViewCreateSlice(&array_ssbo_models3d, &gameObjectsC->arrayViewUboModel ,20, 10);	
		// gmArrayViewCreateSlice(&arrayGameObjectInstances, &gameObjectsC->arrayViewGameObjectInstances ,20, 10);	
		// gmArrayViewCreateSlice(&arraySBO_ObjectIds_1, &gameObjectsC->arrayViewUboObjectIds ,20, 10);	
	
		// gameObjectsC->textureIdx = texture_index;
		// gameObjectsC->vertexIdx = vertexData_index;
		

		//create_Object3D(texture_index, vertexData_index, 10, &obj_rooms_b);
	}


	//create_Object3D(2, 1, 10, &obj_light);

	//create_ObjectHUD(2,0,1, &obj2d_cross);
	
}
void freeVariables(){

	gmListFree(&list_obj3d);
	gmListFree(&list_image_buffers);

	gmArrayFree(&array_inst_mesh_data);
	gmArrayFree(&array_inst_hud_data);
	gmArrayFree(&array_global_data);
	gmArrayFree(&array_primitives);
	gmArrayFree(&array_material_data);
	gmArrayFree(&array_camera_data);
	gmArrayFree(&array_light_data);
	gmArrayFree(&array_obj_3d);
	gmArrayFree(&array_obj_hud);
	gmArrayFree(&array_model);
	// gmArrayFree(&array_texture_info);
	gmArrayFree(&array_textures);
	
	gmArrayFree(&array_vertex_data_3D);
	gmArrayFree(&array_vertex_data_2D);
	gmArrayFree(&array_vertex_indices_3D);
	gmArrayFree(&array_vertex_indices_2D);
	


	// gmArrayFree(&array_hud_inst);
	// gmArrayFree(&array_object3d_inst);
	// gmArrayFree(&array_obj_color);
	// gmArrayFree(&array_object_ids);
	// gmArrayFree(&array_transforms);

	gmListFree(&list_BufferRes);

	gmArrayFree(&arrayTextures_2D);

	gmArrayFree(&arrayTextures_3D);


	// gmArrayFree(&arrayColors_2);

	gmArrayFree(&arrayPipelines);

	// gmArrayFree(&arrayGameObjects);
	
	// gmArrayFree(&array_ssbo_models3d);

	// gmArrayFree(&array_ssbo_models2d);

	
	// gmArrayFree(&arraySBO_ObjectIds_1);

	// gmArrayFree(&arrayGameObjectInstances);

}

void loadModels2(){

	uint32_t size = sizeof(model_paths)/sizeof(model_paths[0]) ;

	for(int i=0;i<size;i++)
	{
		struct Model * model = loadModel2(model_paths[i]);
		model->name = model_names[i];
	}



	// for(int i=0;i<MODEL_NUM;i++)
	// {

	// 	struct ModelVertexData2 * ref = &modelVertexData_3D[i];

	// 	if(ref->gltfPath == NULL) continue;

	// 	uint32_t verticesCnt;
	// 	struct Vertex * vertices;
		

	// 	uint32_t indicesCnt;
	// 	uint32_t * indices;
	
		
	// 	loadModel(
	// 		ref->gltfPath,
	// 		&indicesCnt,
	// 		&indices,
	// 		&verticesCnt,
	// 		&vertices
	// 	);

	// 	ref->indices_num = indicesCnt;

	// 	createVertexBuffer2(
	// 		verticesCnt, 
	// 		vertices, 
	// 		sizeof(struct Vertex)*verticesCnt, 
	// 		&ref->vertexBuffer
	// 	);
	// 	createIndexBuffer2(
	// 		indicesCnt, 
	// 		indices, 
	// 		&ref->indexBuffer
	// 	);
	// 	// createVertexBuffer(
	// 	// 	verticesCnt,
	// 	// 	vertices,
	// 	// 	sizeof(struct Vertex)*verticesCnt,
	// 	// 	&ref->vertextBuffer,
	// 	// 	&ref->vertexBufferMemory
	// 	// );

	// 	// createIndexBuffer(
	// 	// 	indicesCnt,
	// 	// 	indices,
	// 	// 	&ref->indexBuffer,
	// 	// 	&ref->indexBufferMemory
	// 	// );

	// 	free(vertices);
	// 	free(indices);
	// }


	struct Vertex2D vertices[4] = {
		

		{{-10.0f, -10.0f},   {0.0f, 0.0f}}, 

		{{ 10.0f, -10.0f},   {1.0f, 0.0f}}, 

		{{ 10.0f,  10.0f},   {1.0f, 1.0f}}, 

		{{-10.0f,  10.0f},   {0.0f, 1.0f}}  

	};
	uint32_t indices[6] = {
		0, 1, 2,
		2, 3, 0
	};

	struct Model * model =  createModel2D(
		vertices, 
		sizeof(vertices)/sizeof(vertices[0]), 
		indices, 
		sizeof(indices)/sizeof(indices[0]), 
		GM_INDEX_UNUSED
	);
	model->name = "square";

	// modelVertexData_HUD2.indices_num = sizeof(indices)/sizeof(uint32_t);

	// createVertexBuffer2(
	// 	sizeof(vertices)/sizeof(struct Vertex2D),
	// 	vertices,
	// 	sizeof(vertices),
	// 	&modelVertexData_HUD2.vertexBuffer
	// );
	// createIndexBuffer2(
	// 	modelVertexData_HUD2.indices_num,
	// 	indices,
	// 	&modelVertexData_HUD2.indexBuffer
	// );

	
	// createVertexBuffer(
	// 	sizeof(vertices)/sizeof(struct Vertex2D),
	// 	vertices,
	// 	sizeof(vertices),
	// 	&modelVertexData_HUD.vertextBuffer,
	// 	&modelVertexData_HUD.vertexBufferMemory
	// );

	// createIndexBuffer(
	// 	modelVertexData_HUD.indices_num,
	// 	indices,
	// 	&modelVertexData_HUD.indexBuffer,
	// 	&modelVertexData_HUD.indexBufferMemory
	// );


}

// void loadModels(){
// 	for(int i=0;i<MODEL_NUM;i++)
// 	{

// 		struct ModelVertexData2 * ref = &modelVertexData_3D[i];

// 		if(ref->gltfPath == NULL) continue;

// 		uint32_t verticesCnt;
// 		struct Vertex * vertices;
		

// 		uint32_t indicesCnt;
// 		uint32_t * indices;
	
		
// 		loadModel(
// 			ref->gltfPath,
// 			&indicesCnt,
// 			&indices,
// 			&verticesCnt,
// 			&vertices
// 		);

// 		ref->indices_num = indicesCnt;

// 		createVertexBuffer2(
// 			verticesCnt, 
// 			vertices, 
// 			sizeof(struct Vertex)*verticesCnt, 
// 			&ref->vertexBuffer
// 		);
// 		createIndexBuffer2(
// 			indicesCnt, 
// 			indices, 
// 			&ref->indexBuffer
// 		);
// 		// createVertexBuffer(
// 		// 	verticesCnt,
// 		// 	vertices,
// 		// 	sizeof(struct Vertex)*verticesCnt,
// 		// 	&ref->vertextBuffer,
// 		// 	&ref->vertexBufferMemory
// 		// );

// 		// createIndexBuffer(
// 		// 	indicesCnt,
// 		// 	indices,
// 		// 	&ref->indexBuffer,
// 		// 	&ref->indexBufferMemory
// 		// );

// 		free(vertices);
// 		free(indices);
// 	}


// 	struct Vertex2D vertices[4] = {
		

// 		{{-10.0f, -10.0f},   {0.0f, 0.0f}}, 

// 		{{ 10.0f, -10.0f},   {1.0f, 0.0f}}, 

// 		{{ 10.0f,  10.0f},   {1.0f, 1.0f}}, 

// 		{{-10.0f,  10.0f},   {0.0f, 1.0f}}  

// 	};
// 	uint32_t indices[6] = {
// 		0, 1, 2,
// 		2, 3, 0
// 	};
// 	modelVertexData_HUD2.indices_num = sizeof(indices)/sizeof(uint32_t);

// 	createVertexBuffer2(
// 		sizeof(vertices)/sizeof(struct Vertex2D),
// 		vertices,
// 		sizeof(vertices),
// 		&modelVertexData_HUD2.vertexBuffer
// 	);
// 	createIndexBuffer2(
// 		modelVertexData_HUD2.indices_num,
// 		indices,
// 		&modelVertexData_HUD2.indexBuffer
// 	);

	
// 	// createVertexBuffer(
// 	// 	sizeof(vertices)/sizeof(struct Vertex2D),
// 	// 	vertices,
// 	// 	sizeof(vertices),
// 	// 	&modelVertexData_HUD.vertextBuffer,
// 	// 	&modelVertexData_HUD.vertexBufferMemory
// 	// );

// 	// createIndexBuffer(
// 	// 	modelVertexData_HUD.indices_num,
// 	// 	indices,
// 	// 	&modelVertexData_HUD.indexBuffer,
// 	// 	&modelVertexData_HUD.indexBufferMemory
// 	// );


// }
struct Model * find_ModelByName(char * name, uint32_t *start_pos){
	
	if(*start_pos >= array_model.len) return NULL;

	struct Model * ret  = NULL;
	for(int i=*start_pos;i<array_model.len;i++)
	{
		struct Model * mod = gmArrayGet(&array_model, i);
		if(strcmp(mod->name , name) == 0){
			ret = mod;
			*start_pos  = i;
			break;
		}
	}
	if(ret == NULL){
		*start_pos = array_model.len;
	}
	return ret;
}

void loadModel(
	char *fname,
	uint32_t *indicesNum,
	uint32_t ** indices,
	uint32_t *verticesNum,
	struct Vertex ** vertices
){
	PRINT_FNAME;

	printf("load model: %s\n",fname);

	const cgltf_options options={};
	cgltf_data *data;
	uint32_t data_size;
	const char * gltf_path = fname;
	
	cgltf_result res;


  	res = cgltf_parse_file(&options,fname,&data);


	if(res != cgltf_result_success)
	{
		printf("res %d\n",res);
		EXIT_CLEAN("Cant load buffers :(");
	}

	for(int i=0;i<data->buffers_count;i++){
		printf("Buffer:%s\n",data->buffers[i].uri);
		printf("Buffer.size:%lld\n",data->buffers[i].size);
		printf("Buffer.name:%s\n",data->buffers[i].name);
		printf("Buffer.uri:%s\n",data->buffers[i].uri);
		printf("Buffer.data:%p\n",data->buffers[i].data);
	}


	
	printf("Scene count: %d\n",data->scenes_count);
	for(int i=0;i<data->scenes_count;i++)
	{

		printf("Node count: %d\n",data->scenes[i].nodes_count);
		for(int j=0;j<data->scenes[i].nodes_count;j++)
		{
			printf("Node name: %s\n",data->scenes[i].nodes[j]->name);
		}
	}



	for(int i=0;i<data->meshes_count;i++){
	
		printf("Meshes :");
		printf(" .name:%s\n",data->meshes[i].name);
		
	
		printf(" .primitives_count:%ld\n",data->meshes[i].primitives_count);
		

		for(int j=0;j<data->meshes[i].primitives_count;j++){

			printf(" Primitives :\n");
			printf("  .attributes_count:%ld\n",data->meshes[i].primitives[j].attributes_count);
			printf("  .material:%p\n",data->meshes[i].primitives[j].material);

			

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


		char url_path[256] ;
	 
		uint32_t n = snprintf(url_path, sizeof(url_path), "models_gltf/%s", data->buffers[0].uri);

		if (n >= (uint32_t)sizeof(url_path)) {
			
		 
			EXIT_CLEAN("url_path Overflow!");
		}


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
 

	cgltf_primitive *primitive = &data->meshes[0].primitives[0];
	 

	cgltf_accessor *acc_idx = primitive->indices;
	cgltf_accessor *acc_vert = NULL; 
 	
	for(int i=0;i < primitive->attributes_count;i++){

		cgltf_attribute attr =  primitive->attributes[i];
		if(attr.type == cgltf_attribute_type_position)
		{

			cgltf_accessor* accessor = attr.data;
		
			cgltf_buffer_view* view = accessor->buffer_view;

			uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;
			uint32_t stride = accessor->stride ? accessor->stride : sizeof(float) * 3;

			*verticesNum = accessor->count;
			*vertices = malloc(sizeof(struct Vertex) * (*verticesNum));

			printf("verticesNum: %d\n",*verticesNum);
			printf("stride: %ld\n",stride);
		 
			for (uint32_t d = 0; d < *verticesNum; d++) {
				float* data = (float*)(base + d * stride);

				struct Vertex * v = *vertices;
			
				v[d].pos[0] = data[0];
				v[d].pos[1] = data[1];
				v[d].pos[2] = data[2];

				
			 
			}

		 
			break;
		}

		
	}

	for(int i=0;i<primitive->attributes_count;i++){

		cgltf_attribute attr =  primitive->attributes[i];
		
		if(attr.type == cgltf_attribute_type_normal)
		{
			assert(attr.data->type == cgltf_type_vec3);
			assert(attr.data->component_type == cgltf_component_type_r_32f);

			cgltf_accessor* accessor = attr.data;
			cgltf_buffer_view* view = accessor->buffer_view;

			uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;
			uint32_t stride = accessor->stride ? accessor->stride : sizeof(float) * 2;

			


			for (uint32_t d = 0; d < *verticesNum; d++) {
				float* data = (float*)(base + d * stride);

				struct Vertex * v = *vertices;
			
				v[d].norm[0] = data[0];
				v[d].norm[1] = data[1];
				v[d].norm[2] = data[2];

 
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
			uint32_t stride = accessor->stride ? accessor->stride : sizeof(float) * 2;

			


			for (uint32_t d = 0; d < *verticesNum; d++) {
				float* data = (float*)(base + d * stride);


				struct Vertex * v = *vertices;
			
				v[d].texCoords[0] = data[0];
				v[d].texCoords[1] = data[1];
		 
 
			}
			break;
		}
	}

	if(primitive->indices)
	{
	
	

		cgltf_accessor* accessor = primitive->indices;
		cgltf_buffer_view* view = accessor->buffer_view;


		uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;

		uint32_t stride = accessor->stride;

		if (stride == 0) {
			stride = cgltf_component_size(accessor->component_type);
		}

		*indicesNum = primitive->indices->count;

		
		printf("indices num: %d\n",*indicesNum);
		*indices = malloc(sizeof(uint32_t) * (*indicesNum));

		for(int d=0;d < *indicesNum;d++){

			uint8_t* ptr = base + d * stride;

			uint32_t * ind = *indices;

			 switch (accessor->component_type) {
				case cgltf_component_type_r_16u:
					ind[d] = *(uint16_t*)ptr;
					break;
				case cgltf_component_type_r_32u:
					ind[d] = *(uint32_t*)ptr;
					break;
				case cgltf_component_type_r_8u:
					ind[d] = *(uint8_t*)ptr;
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
struct Model * createModel2D(
	struct Vertex2D * vertex,
	uint32_t vertex_num,
	uint32_t * index,
	uint32_t index_num,
	uint32_t material_idx
){

	struct Model * ret = gmArrayNew(&array_model);
	ret->meshs.offset = array_mesh.len;
	struct Mesh * mesh = gmArrayNew(&array_mesh);
	ret->meshs.cap = 1;
	ret->meshs.count = 1;

	mesh->primitives.offset = array_mesh.len;
	struct Primitive * prim = gmArrayNew(&array_primitives);
	mesh->primitives.cap = 1;
	mesh->primitives.count = 1;


	prim->material_idx = material_idx;
	prim->vertices.offset = array_vertex_data_2D.len;
	prim->vertices.cap = vertex_num;
	prim->vertices.count = vertex_num;
	prim->indices.count = index_num;
	prim->indices.cap = index_num;
	prim->indices.offset = array_vertex_indices_2D.len;

	struct Vertex2D * verts = gmArrayNewN(&array_vertex_data_2D, vertex_num);
	memcpy(verts, vertex, vertex_num * sizeof(struct Vertex2D));

	// for(int i=0;i < vertex_num;i++)
	// {
	// 	struct Vertex2D * ver = gmArrayNew(&array_vertex_data_2D);
	// 	*ver = vertex[i];
	// }


	uint32_t * ind = gmArrayNewN(&array_vertex_indices_2D,index_num);
	memcpy(ind, index, index_num * sizeof(uint32_t));



	return ret;
};
struct Model *  loadModel2(
	char *fname

){
	PRINT_FNAME;

	printf("load model: %s\n",fname);

	const cgltf_options options={};
	cgltf_data *data;
	uint32_t data_size;
	const char * gltf_path = fname;
	
	cgltf_result res;


  	res = cgltf_parse_file(&options,fname,&data);


	if(res != cgltf_result_success)
	{
		printf("res %d\n",res);
		EXIT_CLEAN("Cant load buffers :(");
	}

	for(int i=0;i<data->buffers_count;i++){
		printf("Buffer:%s\n",data->buffers[i].uri);
		printf("Buffer.size:%lld\n",data->buffers[i].size);
		printf("Buffer.name:%s\n",data->buffers[i].name);
		printf("Buffer.uri:%s\n",data->buffers[i].uri);
		printf("Buffer.data:%p\n",data->buffers[i].data);
	}


	
	printf("Scene count: %d\n",data->scenes_count);
	for(int i=0;i<data->scenes_count;i++)
	{

		printf("Node count: %d\n",data->scenes[i].nodes_count);
		for(int j=0;j<data->scenes[i].nodes_count;j++)
		{
			printf("Node name: %s\n",data->scenes[i].nodes[j]->name);
		}
	}

	/*
		unlinked are many meshes
		linked with different materials are many primitives
	*/


	for(int i=0;i<data->meshes_count;i++){
	
		printf("Meshes :");
		printf(" .name:%s\n",data->meshes[i].name);
		
	
		printf(" .primitives_count:%ld\n",data->meshes[i].primitives_count);
		

		for(int j=0;j<data->meshes[i].primitives_count;j++){

			printf(" Primitives :\n");
			printf("  .attributes_count:%ld\n",data->meshes[i].primitives[j].attributes_count);
			printf("  .material:%p\n",data->meshes[i].primitives[j].material);

			

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


		char url_path[256] ;
	 
		uint32_t n = snprintf(url_path, sizeof(url_path), "models_gltf/%s", data->buffers[0].uri);

		if (n >= (uint32_t)sizeof(url_path)) {
			
		 
			EXIT_CLEAN("url_path Overflow!");
		}


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
 



	/*
	
	
	array_vertex_data_3D
	array_inst_mesh_data
	array_obj_3d
	array_material_data
	array_mesh
	arrayTextures_3D
	
	vertexBuffer
	indexBuffer

	createTextureImage4
	*/

	// struct ModelVertexData2 a1;
	// struct DataInstanceMesh a2;
	// struct GameObject3D a3;
	// struct Mesh a4;
	// struct DataMaterial a5;
	// struct TextureRes a6;


	struct Model * game_object = gmArrayNew(&array_model);
	game_object->meshs.cap = data->meshes_count;
	game_object->meshs.count = data->meshes_count;
	game_object->meshs.offset = array_mesh.len;
	
	
	
	for(int m=0; m < data->meshes_count; m++){
		struct Mesh * mesh = gmArrayNew(&array_mesh);
		
		cgltf_mesh* cgl_mesh = &data->meshes[m];
		
		mesh->primitives.offset = array_primitives.len;
		mesh->primitives.cap = cgl_mesh->primitives_count;
		mesh->primitives.count = cgl_mesh->primitives_count;

		for(int p =0; p < cgl_mesh->primitives_count;p++)
		{

			struct Primitive * prim = gmArrayNew(&array_primitives);
			

			cgltf_primitive *cgl_primitive = &cgl_mesh->primitives[p];
	 
			assert(cgl_primitive->attributes_count > 0);


			uint32_t vertex_cnt  =  cgl_primitive->attributes->data->count;
			
			prim->vertices.offset = array_vertex_data_3D.len;
			prim->vertices.count = vertex_cnt;
			prim->vertices.cap = vertex_cnt;

			for(int v = 0; v < vertex_cnt ; v++){

				struct Vertex * vert = gmArrayNew(&array_vertex_data_3D);

				for(int a=0; a < cgl_primitive->attributes_count; a++){
	
					cgltf_attribute * cgl_attribute = &cgl_primitive->attributes[a];
					
					cgltf_accessor* accessor = cgl_attribute->data;

					cgltf_buffer_view* view = accessor->buffer_view;

					assert(view != NULL);

					uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;

					// uint32_t stride = accessor->stride ;
					uint32_t stride = view->stride;

					int num_components = cgltf_num_components(accessor->type);
					int component_size = cgltf_component_size(accessor->component_type);

					if(stride == 0)
					{
						stride  = num_components * component_size;
					}

					if(cgl_attribute->type == cgltf_attribute_type_position){
						
		
						assert(accessor->type == cgltf_type_vec3);
						assert(accessor->component_type == cgltf_component_type_r_32f);
						assert(vertex_cnt == accessor->count);
			
						
						float* data = (float*)(base + v * stride);
					
						vert->pos[0] = data[0];
						vert->pos[1] = data[1];
						vert->pos[2] = data[2];
	
	
					}else if(cgl_attribute->type == cgltf_attribute_type_texcoord){

						assert(cgl_attribute->data->type == cgltf_type_vec2);
						assert(cgl_attribute->data->component_type == cgltf_component_type_r_32f);


						assert(vertex_cnt == accessor->count);


						float* data = (float*)(base + v * stride);

						vert->texCoords[0] = data[0];
						vert->texCoords[1] = data[1];
					
							
	
					}else if(cgl_attribute->type == cgltf_attribute_type_normal){

						assert(cgl_attribute->data->type == cgltf_type_vec3);
						assert(cgl_attribute->data->component_type == cgltf_component_type_r_32f);

						assert(vertex_cnt == accessor->count);


						float* data = (float*)(base + v * stride);

						vert->norm[0] = data[0];
						vert->norm[1] = data[1];
						vert->norm[2] = data[2];
					}
						
					
	
				}
			}

			cgltf_accessor* cgl_indices = cgl_primitive->indices;
			if(cgl_indices){

				cgltf_accessor* accessor = cgl_indices;
				cgltf_buffer_view* view = accessor->buffer_view;

				assert(view != NULL);

				uint8_t* base = (uint8_t*)view->buffer->data + view->offset + accessor->offset;


				// uint32_t stride = view->stride;

				
				int component_size = cgltf_component_size(accessor->component_type);

				// if(stride == 0)
				// {
				// 	stride  =  component_size;
				// }

				uint32_t indices_cnt = accessor->count;

				prim->indices.offset = array_vertex_indices_3D.len;
				prim->indices.cap = indices_cnt;
				prim->indices.count = indices_cnt;
		

				uint32_t * ind = gmArrayNewN(&array_vertex_indices_3D,indices_cnt);

				for(int y=0;y < indices_cnt ; y++){

					uint8_t* ptr = base + y * component_size;

					

					// uint32_t * ind =  gmArrayNew(&array_vertex_indices);

					switch (accessor->component_type) {
						case cgltf_component_type_r_16u:
							ind[y] = *(uint16_t*)ptr;
							break;
						case cgltf_component_type_r_32u:
							ind[y] = *(uint32_t*)ptr;
							break;
						case cgltf_component_type_r_8u:
							ind[y] = *(uint8_t*)ptr;
							break;
						default:{
								printf("datatype: %d\n",accessor->component_type);
								EXIT_CLEAN("UNSUPPORTED INDEX DATA TYPE");
							}
							break;
					}
					
				}
		
			}

			cgltf_material* cgl_material = cgl_primitive->material;

			if(cgl_material){
				prim->material_idx = array_material_data.len;
				
				struct DataMaterial * material = gmArrayNew(&array_material_data);

				

				material->metallicRoughnessTexture_idx = GM_INDEX_UNUSED;
				material->baseColorTexture_idx = GM_INDEX_UNUSED;
				GLM_VEC4_SET(material->baseColorFactor, 0.3, 0.8, 0.6, 1.0);
				material->roughnessFactor = 1;
				material->normalTexture_idx = GM_INDEX_UNUSED;

				if (cgl_material->normal_texture.texture &&
					cgl_material->normal_texture.texture->image &&
					cgl_material->normal_texture.texture->image->buffer_view)
				{

					cgltf_buffer_view* bv = cgl_material->normal_texture.texture->image->buffer_view;
						 

					uint32_t data_size = bv->size;
					uint8_t* data = (uint8_t*)bv->buffer->data + bv->offset;

					struct DataBuffer * db = alloc_data_buffer(data_size);
					
					copy_to_data_buffer(db, data, data_size);

					gmListPushBack(&list_image_buffers, db);
					
					struct Texture * te = gmArrayNew(&array_textures);
					te->idx = array_textures.len - 1;
					db->idx = te->idx;

					material->normalTexture_idx = te->idx;

				}

				
				if (cgl_material->emissive_texture.texture &&
					cgl_material->emissive_texture.texture->image &&
					cgl_material->emissive_texture.texture->image->buffer_view)
				{

					cgltf_buffer_view* bv = cgl_material->emissive_texture.texture->image->buffer_view;
						 

					uint32_t data_size = bv->size;
					uint8_t* data = (uint8_t*)bv->buffer->data + bv->offset;

					struct DataBuffer * db = alloc_data_buffer(data_size);
					
					copy_to_data_buffer(db, data, data_size);

					gmListPushBack(&list_image_buffers, db);

					struct Texture * te = gmArrayNew(&array_textures);
					te->idx = array_textures.len - 1;
					db->idx = te->idx;

					material->emissiveTexture_idx = te->idx;

				}


				if(cgl_material->has_pbr_metallic_roughness){

					cgltf_pbr_metallic_roughness mr = cgl_material->pbr_metallic_roughness;

					GLM_VEC4_COPY(material->baseColorFactor , mr.base_color_factor);
					material->metallicFactor = mr.metallic_factor;
					material->roughnessFactor = mr.roughness_factor;

				

					
					/*
					mr.base_color_texture.texture->image->buffer_view->data

					texture{
						idx 0|N = texture_res idx
						draw_data ptr
						loaded y|n
					}
					

					*/
					
					 if (mr.base_color_texture.texture &&
						mr.base_color_texture.texture->image &&
						mr.base_color_texture.texture->image->buffer_view)
					{

						cgltf_buffer_view* bv = mr.base_color_texture.texture->image->buffer_view;
						 

						uint32_t data_size = bv->size;
						uint8_t* data = (uint8_t*)bv->buffer->data + bv->offset;
	
						
						struct DataBuffer * db = alloc_data_buffer(data_size);
						copy_to_data_buffer(db, data, data_size);

						gmListPushBack(&list_image_buffers, db);

						struct Texture * te = gmArrayNew(&array_textures);
						te->idx = array_textures.len - 1;
						db->idx = te->idx;
						material->baseColorTexture_idx = te->idx;
					}



					
					 if (mr.metallic_roughness_texture.texture &&
						mr.metallic_roughness_texture.texture->image &&
						mr.metallic_roughness_texture.texture->image->buffer_view)
					{

						cgltf_buffer_view* bv = mr.metallic_roughness_texture.texture->image->buffer_view;
					 
						uint32_t data_size = bv->size;
						uint8_t* data = (uint8_t*)bv->buffer->data + bv->offset;
	
						struct DataBuffer * db = alloc_data_buffer(data_size);
						copy_to_data_buffer(db, data, data_size);

						gmListPushBack(&list_image_buffers, db);

						struct Texture * te = gmArrayNew(&array_textures);
						te->idx = array_textures.len - 1;
						db->idx = te->idx;
						
						material->metallicRoughnessTexture_idx = te->idx;
					}
				


				}
				
			}else
			{
				prim->material_idx = GM_INDEX_UNUSED;
			}
		}
	}

	

	cgltf_free(data);


	return game_object;

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
void createShadowResource(struct TextureRes * tex){
	PRINT_FNAME;
	
	VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

	VkFormat depthFormat = findDepthFormat();
	tex->image.alloc.mapped = NULL;  
    tex->image.alloc.memory = VK_NULL_HANDLE; 

	createImage(
		swapChainExtent.width, 
		swapChainExtent.height, 
		1,
		depthFormat, 
		VK_IMAGE_TILING_OPTIMAL, 
		usage,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&tex->image.handle, 
		&tex->image.alloc.memory
	);
	

	
	createImageView(&tex->image.view,&tex->image.handle, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,1);



	
	createTextureSamplerShadow(&tex->textureSampler);
		
		tex->textureIdx= -1;
	
		beginSingleTimeCommands(transferCommandBuffers);
		transitionImageLayout(
			transferCommandBuffers,
			&tex->image.handle,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,  
			0,
			0,
			VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT,
			1
		);
		endSingleTimeCommands(transferCommandBuffers);


	// beginSingleTimeCommands(transferCommandBuffers);


	// transitionImageLayout(
	// 	transferCommandBuffers,
    // 	&image->handle,
    //     VK_IMAGE_LAYOUT_UNDEFINED,
	// 	VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    //     0,
    //     VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    //     VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    //     VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    //     VK_IMAGE_ASPECT_DEPTH_BIT,
	// 	1
    // );

	// endSingleTimeCommands(transferCommandBuffers);
}
void createDepthResources4(struct ImageRes * image,bool sampled_bit) {
	PRINT_FNAME;
	
	VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

	if(sampled_bit)
	{
		usage |=VK_IMAGE_USAGE_SAMPLED_BIT;
	}
	

	VkFormat depthFormat = findDepthFormat();
	image->alloc.mapped = NULL;  
    image->alloc.memory = VK_NULL_HANDLE; 

	createImage(
		swapChainExtent.width, 
		swapChainExtent.height, 
		1,
		depthFormat, 
		VK_IMAGE_TILING_OPTIMAL, 
		usage,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&image->handle, 
		&image->alloc.memory
	);
	

	
	createImageView(&image->view,&image->handle, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,1);


	beginSingleTimeCommands(transferCommandBuffers);


	transitionImageLayout(
		transferCommandBuffers,
    	&image->handle,
        VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        0,
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT,
		1
    );

	endSingleTimeCommands(transferCommandBuffers);
	

}
void createDepthResources3(VkImage *depthImage,VkImageView *depthImageView,VkDeviceMemory *depthImageMemory) {
	PRINT_FNAME;
	VkFormat depthFormat = findDepthFormat();

	createImage(
		swapChainExtent.width, 
		swapChainExtent.height, 
		1,
		depthFormat, 
		VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		depthImage, 
		depthImageMemory
	);
	

	
	createImageView(depthImageView,depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,1);


	beginSingleTimeCommands(transferCommandBuffers);


	transitionImageLayout(
		transferCommandBuffers,
        depthImage,
        VK_IMAGE_LAYOUT_UNDEFINED,
    
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        0,
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT,
		1
    );

	endSingleTimeCommands(transferCommandBuffers);
	

}
void createTextureSamplerShadow(VkSampler * sampler){


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
 
 		.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, // ✅ Changed from REPEAT for shadow maps
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, // ✅ Important for depth clamping

		// .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		// .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		
		.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy,
		.compareOp = VK_COMPARE_OP_LESS,
		// .compareOp = VK_COMPARE_OP_ALWAYS,

		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
	};

	vkCreateSampler(device, &samplerInfo, NULL, sampler);

}
void createTextureSampler(VkSampler * sampler){


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
 
 		// .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, // ✅ Changed from REPEAT for shadow maps
        // .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        // .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, // ✅ Important for depth clamping

		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		
		.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy,
		// .compareOp = compare ? VK_COMPARE_OP_LESS : VK_COMPARE_OP_ALWAYS,
		.compareOp = VK_COMPARE_OP_ALWAYS,

		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
	};

	vkCreateSampler(device, &samplerInfo, NULL, sampler);

}

void createImageView4(struct ImageRes * image, VkFormat format, VkImageAspectFlagBits aspectFlags){

		
		/*
		
		*/
		image->format = format;
		
		VkImageViewCreateInfo viewInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = image->handle,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = image->format,
			.subresourceRange  = {
					
					.aspectMask = aspectFlags,
					.baseMipLevel = 0,
					.levelCount = image->mipLevels,
					.baseArrayLayer= 0,
					.layerCount =1,
			},
			
		};
		
		vkCreateImageView(device, &viewInfo,  NULL, &image->view);
}
void createImageView(VkImageView *imageView, VkImage* image, VkFormat format, VkImageAspectFlagBits aspectFlags,uint32_t mipLevels){


		
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


void copyBufferToImage(VkBuffer *buffer, VkImage *image, uint32_t width, uint32_t height) {
	PRINT_FNAME;
  

	beginSingleTimeCommands(transferCommandBuffers);


	
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

	vkCmdCopyBufferToImage(transferCommandBuffers, *buffer, *image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);
	

	
    endSingleTimeCommands(transferCommandBuffers);
	
	

}
void transitionImageLayout(
	VkCommandBuffer cmdBuffer,
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


	VkImageMemoryBarrier2 imageMemoryBarrier2 = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		
	};
	
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
		cmdBuffer, 
		srcStageMask, 
		dstStageMask, 
		0, 
		0, (VkMemoryBarrier* )NULL,
		0, (VkBufferMemoryBarrier*) NULL, 
		1, 
		&imageMemoryBarriers
	);

 
}

void  beginSingleTimeCommands(VkCommandBuffer commandBuffer){


	vkResetCommandBuffer(commandBuffer, 0);


	
	VkCommandBufferBeginInfo beginInfo  = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};

	vkBeginCommandBuffer(commandBuffer, &beginInfo);
}
void endSingleTimeCommands(VkCommandBuffer commandBuffer){

	vkEndCommandBuffer(commandBuffer);

	VkPipelineStageFlags2 stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;

	VkSubmitInfo2 submitInfo2 ={
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &(VkCommandBufferSubmitInfo){
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.commandBuffer = commandBuffer,

		},

	};

    vkResetFences(device, 1, &transferFence);

	vkQueueSubmit2(transferQueue, 1, &submitInfo2, transferFence);
   
    vkWaitForFences(device, 1, &transferFence, VK_TRUE, UINT64_MAX);
 
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
		.format  = format,
		.extent = (VkExtent3D)
		{
			.width= width,
			.height = height,
			.depth = 1
		},
		.mipLevels = mipLevels,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = tiling,
		.usage  = usage ,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	};

	vkCreateImage(device, &imageCreateInfo, NULL, image);
 

	VkImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo2 ={
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2,
		.image = *image,
	};

	VkMemoryRequirements2  memoryRequirements2={
		.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2,
	};
 
	vkGetImageMemoryRequirements2(
		device,
		&imageMemoryRequirementsInfo2,
		&memoryRequirements2
	);
	

    VkMemoryAllocateInfo allocInfo ={
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memoryRequirements2.memoryRequirements.size,
		.memoryTypeIndex = findMemoryType(memoryRequirements2.memoryRequirements.memoryTypeBits, properties),
		
	};
	
	vkAllocateMemory(device, &allocInfo,  NULL, imageMemory);
	
 	vkBindImageMemory(device, *image, *imageMemory,0);
}

uint32_t getMipmapLevels(uint32_t w,uint32_t h){
	uint32_t max = w > h? w: h;
	uint32_t d = floor(log2(max));
	d++;
	return  d;

}
void createPickImage(

){
	PRINT_FNAME;

	
	for(int i=0;i<MAX_FRAMES_IN_FLIGHT;i++)
	{
		struct Frame *frame = &frames[i];
		

		createBuffer(
			sizeof(uint32_t), 
			VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&frame->pick.buffer.handle,
			&frame->pick.buffer.alloc.memory
		);


		
		vkMapMemory(device, frame->pick.buffer.alloc.memory, 0, VK_WHOLE_SIZE, 0, &frame->pick.buffer.alloc.mapped);
		
		memset(frame->pick.buffer.alloc.mapped, 0, sizeof(uint32_t));
		frame->pick.pickedID = 0;

		createImage(
			swapChainExtent.width, 
			swapChainExtent.height, 
			1,
			VK_FORMAT_R32_UINT,
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			&frame->pick.image.handle, 
			&frame->pick.image.alloc.memory
		);

		createImageView(
			&frame->pick.image.view, 
			&frame->pick.image.handle,
			VK_FORMAT_R32_UINT, 
			VK_IMAGE_ASPECT_COLOR_BIT, 
			1
		);

	}
}

void createTextureImage(
	char * path, 
	VkImage *textureImage,
	VkDeviceMemory *textureImageMemory,
	uint32_t *mipmap
){

	PRINT_FNAME;

	int texWidth, texHeight, texChannels;

	

	stbi_uc* pixels  = stbi_load(path,  &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	*mipmap  = getMipmapLevels(texWidth,texHeight);

	
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
		*mipmap,
		VK_FORMAT_R8G8B8A8_SRGB, 
		VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		textureImage, 
		textureImageMemory
	);



	beginSingleTimeCommands(transferCommandBuffers);

	transitionImageLayout(
		transferCommandBuffers,
		textureImage, 
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		0,  
		VK_ACCESS_2_TRANSFER_WRITE_BIT,  
		VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_2_TRANSFER_BIT,  
		VK_IMAGE_ASPECT_COLOR_BIT,
		*mipmap
	);

	endSingleTimeCommands(transferCommandBuffers);

	copyBufferToImage(
		&stagingBuffer, 
		textureImage, 
		texWidth, 
		texHeight
	);


	vkDestroyBuffer(device, stagingBuffer,  NULL);
	
	vkFreeMemory(device, stagingBufferMemory,  NULL);


	generateMipmaps(
		textureImage, 
		VK_FORMAT_R8G8B8A8_SRGB, 
		texWidth, 
		texHeight, 
		*mipmap
	);
}


void createTextureImage_from_data(struct TextureRes * tex, uint8_t* in_data,uint32_t in_size)
{
	PRINT_FNAME;

	int texWidth, texHeight, texChannels;

	
	stbi_uc* pixels  = stbi_load_from_memory(in_data, in_size,  &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	
	tex->image.mipLevels = getMipmapLevels(texWidth,texHeight);

	tex->image.alloc.mapped = NULL;
	tex->image.alloc.memory = VK_NULL_HANDLE;
	
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
		tex->image.mipLevels,
		VK_FORMAT_R8G8B8A8_SRGB, 
		VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		&tex->image.handle, 
		&tex->image.alloc.memory
	);



	beginSingleTimeCommands(transferCommandBuffers);

	transitionImageLayout(
		transferCommandBuffers,
		&tex->image.handle, 
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		0,  
		VK_ACCESS_2_TRANSFER_WRITE_BIT,  
		VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_2_TRANSFER_BIT,  
		VK_IMAGE_ASPECT_COLOR_BIT,
		tex->image.mipLevels
	);

	endSingleTimeCommands(transferCommandBuffers);

	copyBufferToImage(
		&stagingBuffer, 
		&tex->image.handle, 
		texWidth, 
		texHeight
	);


	vkDestroyBuffer(device, stagingBuffer,  NULL);
	
	vkFreeMemory(device, stagingBufferMemory,  NULL);


	generateMipmaps(
		&tex->image.handle, 
		VK_FORMAT_R8G8B8A8_SRGB, 
		texWidth, 
		texHeight, 
		tex->image.mipLevels
	);
}
// void createTextureImage4(
// 	struct TextureRes * tex
	
// ){

// 	PRINT_FNAME;

// 	int texWidth, texHeight, texChannels;

	
// 	stbi_uc* pixels  = stbi_load(tex->path,  &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	
// 	tex->image.mipLevels = getMipmapLevels(texWidth,texHeight);

// 	tex->image.alloc.mapped = NULL;
// 	tex->image.alloc.memory = VK_NULL_HANDLE;
	
// 	VkDeviceSize imageSize = texWidth * texHeight * 4;

// 	if( ! pixels){
// 		EXIT_CLEAN("failed to load texture image!");
// 	}


// 	VkBuffer stagingBuffer;

// 	VkDeviceMemory stagingBufferMemory;

// 	createBuffer(
// 		imageSize, 
// 		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
// 		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
// 		&stagingBuffer,
// 		&stagingBufferMemory
// 	);
	
// 	void* data = NULL;

// 	vkMapMemory(device, 
// 		stagingBufferMemory,
// 		0, 
// 		imageSize, 
// 		0, 
// 		&data
// 	);
// 	memcpy(data, pixels, imageSize);

// 	vkUnmapMemory(device, stagingBufferMemory);
	
// 	stbi_image_free(pixels);

	
	
// 	createImage(
// 		texWidth, 
// 		texHeight, 
// 		tex->image.mipLevels,
// 		VK_FORMAT_R8G8B8A8_SRGB, 
// 		VK_IMAGE_TILING_OPTIMAL, 
// 		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, 
// 		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
// 		&tex->image.handle, 
// 		&tex->image.alloc.memory
// 	);



// 	beginSingleTimeCommands(transferCommandBuffers);

// 	transitionImageLayout(
// 		transferCommandBuffers,
// 		&tex->image.handle, 
// 		VK_IMAGE_LAYOUT_UNDEFINED,
// 		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
// 		0,  
// 		VK_ACCESS_2_TRANSFER_WRITE_BIT,  
// 		VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
// 		VK_PIPELINE_STAGE_2_TRANSFER_BIT,  
// 		VK_IMAGE_ASPECT_COLOR_BIT,
// 		tex->image.mipLevels
// 	);

// 	endSingleTimeCommands(transferCommandBuffers);

// 	copyBufferToImage(
// 		&stagingBuffer, 
// 		&tex->image.handle, 
// 		texWidth, 
// 		texHeight
// 	);


// 	vkDestroyBuffer(device, stagingBuffer,  NULL);
	
// 	vkFreeMemory(device, stagingBufferMemory,  NULL);


// 	generateMipmaps(
// 		&tex->image.handle, 
// 		VK_FORMAT_R8G8B8A8_SRGB, 
// 		texWidth, 
// 		texHeight, 
// 		tex->image.mipLevels
// 	);
// }

void generateMipmaps(VkImage* image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {

    PRINT_FNAME;
 
    beginSingleTimeCommands(transferCommandBuffers);

  
    VkFormatProperties2 formatProperties = {
        .sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2,
    };
    vkGetPhysicalDeviceFormatProperties2(physicalDevice, imageFormat, &formatProperties);
    
    if (!(formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_2_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        EXIT_CLEAN("texture image format does not support linear blitting!");
    }

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;


    
    VkImageMemoryBarrier2 barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = *image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        .srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT,
        .srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
    };
	
    VkDependencyInfo dependencyInfo = {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &barrier,
    };

    vkCmdPipelineBarrier2(transferCommandBuffers, &dependencyInfo);


    for (uint32_t i = 1; i < mipLevels; i++) {
  
        VkImageBlit2 blit = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2,
            .srcOffsets = {{0, 0, 0}, {mipWidth, mipHeight, 1}},
            .dstOffsets = {{0, 0, 0}, {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1}},
            .srcSubresource = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = i - 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
            .dstSubresource = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = i,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
        };

        VkBlitImageInfo2 blitInfo = {
            .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2,
            .srcImage = *image,
            .srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            .dstImage = *image,
            .dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .filter = VK_FILTER_LINEAR,
            .regionCount = 1,
            .pRegions = &blit,
        };

        vkCmdBlitImage2(transferCommandBuffers, &blitInfo);

     
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT;
        barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
        barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;

        vkCmdPipelineBarrier2(transferCommandBuffers, &dependencyInfo);

      
        if (i < mipLevels - 1) {
            barrier.subresourceRange.baseMipLevel = i;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
            barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
            barrier.dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;

            vkCmdPipelineBarrier2(transferCommandBuffers, &dependencyInfo);
        }

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }


    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT;
    barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;

    vkCmdPipelineBarrier2(transferCommandBuffers, &dependencyInfo);

    endSingleTimeCommands(transferCommandBuffers);
}
void allocateDescriptorSets(){


	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		
		struct Frame * f = &frames[i];

		//global
		{
			VkDescriptorSetAllocateInfo allocInfo = {
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
				.descriptorPool = descriptorPool,
				.descriptorSetCount = 1,
				.pSetLayouts = &globalLayout
			};
		
			vkAllocateDescriptorSets(device, &allocInfo, &f->globalSets);


			VkDescriptorBufferInfo bufferInfo = {
				.buffer = f->ubo_global.handle,
				.offset = 0,
				.range = VK_WHOLE_SIZE
			};

			VkWriteDescriptorSet write = {
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = f->globalSets,
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.pBufferInfo = &bufferInfo
			};

			vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
			// instance


		}
		//storage
		{
				//

			VkDescriptorSetAllocateInfo allocInfo = {
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
				.descriptorPool = descriptorPool,
				.descriptorSetCount = 1,
				.pSetLayouts = &instanceLayout
			};
		
			vkAllocateDescriptorSets(device, &allocInfo, &f->instanceSets);
			
			VkDescriptorBufferInfo meshInfo = {
				.buffer = f->ssbo_instance_mesh.handle,
				.offset = 0,
				.range = VK_WHOLE_SIZE
			};

			VkDescriptorBufferInfo hudInfo = {
				.buffer =  f->ssbo_instance_hud.handle,
				.offset = 0,
				.range = VK_WHOLE_SIZE
			};

			VkWriteDescriptorSet writes[2] = {
				{
					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet = f->instanceSets,
					.dstBinding = 0,
					.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
					.descriptorCount = 1,
					.pBufferInfo = &meshInfo
				},
				{
					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet = f->instanceSets,
					.dstBinding = 1,
					.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
					.descriptorCount = 1,
					.pBufferInfo = &hudInfo
				}
			};

			vkUpdateDescriptorSets(device, 2, writes, 0, NULL);
		}
		//shadow
		{

			VkDescriptorSetAllocateInfo allocInfo = {
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
				.descriptorPool = descriptorPool,
				.descriptorSetCount = 1,
				.pSetLayouts = &shadowLayout   // your sampler descriptor set layout
			};

			VkResult res = vkAllocateDescriptorSets(device, &allocInfo, &f->shadowSets);
			


			VkWriteDescriptorSet writes= 
			{
					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet = f->shadowSets,
					.dstBinding = 0, // binding 0 = sampler2D array
					.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.descriptorCount = 1,
					.pImageInfo = &f->shadow_descriptor_info,
			};
			
			

			vkUpdateDescriptorSets(device, 1, &writes, 0, NULL);
		}

	}

	//material
	{
		VkDescriptorSetAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool = descriptorPool,
			.descriptorSetCount = 1,
			.pSetLayouts = &materialLayout
		};

		vkAllocateDescriptorSets(device, &allocInfo, &materialSet);

		VkDescriptorBufferInfo materialInfo = {
			.buffer = ssbo_material.handle,
			.offset = 0,
			.range = VK_WHOLE_SIZE // or sizeof(MaterialData) * materialCount
		};
		
		VkWriteDescriptorSet materialWrite = {
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = materialSet,
			.dstBinding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			.descriptorCount = 1,
			.pBufferInfo = &materialInfo
		};
		
		vkUpdateDescriptorSets(device, 1, &materialWrite, 0, NULL);
	}
	{
		VkDescriptorSetAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool = descriptorPool,
			.descriptorSetCount = 1,
			.pSetLayouts = &samplerLayout   // your sampler descriptor set layout
		};

		VkResult res = vkAllocateDescriptorSets(device, &allocInfo, &samplerSet);

		



		VkWriteDescriptorSet writes[2] = {
			{
					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet = samplerSet,
					.dstBinding = 0, // binding 0 = sampler2D array
					.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.descriptorCount = MAX_TEXTURES_3D,
					.pImageInfo = texture_infos_3d
			},
			{
					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet = samplerSet,
					.dstBinding = 1, // binding 0 = sampler2D array
					.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.descriptorCount = MAX_TEXTURES_2D,
					.pImageInfo = texture_infos_2d
			},
		};
		

		vkUpdateDescriptorSets(device, sizeof(writes) / sizeof(writes[0]), writes, 0, NULL);

	}


}

// void createDescriptorSets33(){
// 	PRINT_FNAME;

	

// 	VkDescriptorSetLayout layouts [MAX_FRAMES_IN_FLIGHT]={
		

		

// 	};

// 	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//    		layouts[i] = descriptorSetLayout4;
// 	}



// 	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
// 		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
// 		.descriptorPool = descriptorPool,
// 		.descriptorSetCount = MAX_FRAMES_IN_FLIGHT,
// 		.pSetLayouts = layouts,
// 	};
// 	vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, descriptorSets2);

	


// 	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

// 		struct Frame * frame = &frames[i];

// 		frame->descriptorSets = &descriptorSets2[i];


// 		{
// 			VkDescriptorBufferInfo info = {
// 				.buffer = frame->ssbo_models.handle,
// 				.offset = 0,
// 				.range = sizeof(struct SSBO_Model)*TOTAL_3D_OBJECT
// 			};

// 			VkWriteDescriptorSet write = {
// 				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 				.dstSet = *frame->descriptorSets,
// 				.dstBinding = BINDING_MODELS_3D,
// 				.dstArrayElement = 0,   

// 				.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
// 				.descriptorCount = 1,
// 				.pBufferInfo = &info
// 			};
// 			vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 		}
// 		{
// 			VkDescriptorBufferInfo info = {
// 				.buffer = frame->ssbo_models.handle,
// 				.offset = sizeof(struct SSBO_Model)*TOTAL_3D_OBJECT,
// 				.range = sizeof(struct SSBO_Model)*TOTAL_2D_OBJECT
// 			};

// 			VkWriteDescriptorSet write = {
// 				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 				.dstSet = *frame->descriptorSets,
// 				.dstBinding = BINDING_MODELS_2D,
// 				.dstArrayElement = 0,   

// 				.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
// 				.descriptorCount = 1,
// 				.pBufferInfo = &info
// 			};
// 			vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 		}
// 		{
// 			VkDescriptorBufferInfo info = {
// 				.buffer = frame->ssbo_objectIds.handle,
// 				.offset = 0,
// 				.range = frame->ssbo_objectIds.alloc.size
// 			};

// 			VkWriteDescriptorSet write = {
// 				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 				.dstSet = *frame->descriptorSets,
// 				.dstBinding = BINDING_OBJECT_IDS,
// 				.dstArrayElement = 0,   

// 				.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
// 				.descriptorCount = 1,
// 				.pBufferInfo = &info
// 			};
// 			vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 		}
// 		{
// 			VkDescriptorBufferInfo info = {
// 				.buffer = frame->ssbo_colors.handle,
// 				.offset = 0,
// 				.range = frame->ssbo_colors.alloc.size
// 			};

// 			VkWriteDescriptorSet write = {
// 				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 				.dstSet = *frame->descriptorSets,
// 				.dstBinding = BINDING_COLORS,
// 				.dstArrayElement = 0,   

// 				.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
// 				.descriptorCount = 1,
// 				.pBufferInfo = &info
// 			};
// 			vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 		}
// 		{
// 			VkDescriptorBufferInfo info = {
// 				.buffer = frame->ubo_Lights.handle,
// 				.offset = 0,
// 				.range = frame->ubo_Lights.alloc.size
// 			};

// 			VkWriteDescriptorSet write = {
// 				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 				.dstSet = *frame->descriptorSets,
// 				.dstBinding = BINDING_DIRECTIONAL_LIGHTS,
// 				.dstArrayElement = 0,   

// 				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 				.descriptorCount = 1,
// 				.pBufferInfo = &info
// 			};
// 			vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 		}
		

// 		{
			

// 			VkDescriptorBufferInfo info = {
// 				.buffer = frame->ubo_ViewProjection.handle,
// 				.offset = 0,
// 				.range = sizeof(struct UBO_ViewProjection)
// 			};

// 			VkWriteDescriptorSet write = {
// 				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 				.dstSet = *frame->descriptorSets,
// 				.dstBinding = BINDING_VIEW_PROJECTIONS_3D,
// 				.dstArrayElement = 0,   

// 				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 				.descriptorCount = 1,
// 				.pBufferInfo = &info
// 			};
// 			vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 		}
// 		{
			

// 			VkDescriptorBufferInfo info = {
// 				.buffer = frame->ubo_ViewProjection.handle,
// 				.offset = sizeof(struct UBO_ViewProjection) * 1,
// 				.range = sizeof(struct UBO_ViewProjection)
// 			};

// 			VkWriteDescriptorSet write = {
// 				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 				.dstSet = *frame->descriptorSets,
// 				.dstBinding = BINDING_VIEW_PROJECTIONS_2D,
// 				.dstArrayElement = 0,   

// 				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 				.descriptorCount = 1,
// 				.pBufferInfo = &info
// 			};
// 			vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 		}
// 		{
			

// 			VkDescriptorBufferInfo info = {
// 				.buffer = frame->ubo_ViewProjection.handle,
// 				.offset = sizeof(struct UBO_ViewProjection) * 2,
// 				.range = sizeof(struct UBO_ViewProjection)
// 			};

// 			VkWriteDescriptorSet write = {
// 				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 				.dstSet = *frame->descriptorSets,
// 				.dstBinding = BINDING_VIEW_PROJECTIONS_LIGHT,
// 				.dstArrayElement = 0,   

// 				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 				.descriptorCount = 1,
// 				.pBufferInfo = &info
// 			};
// 			vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 		}
// 		{
// 			{
// 				VkDescriptorImageInfo info = {
// 					.imageView = frame->shadow.image.view,
// 					.sampler = frame->shadow.textureSampler,
// 					.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
// 				};

// 				VkWriteDescriptorSet write = {
// 					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 					.dstSet = *frame->descriptorSets,
// 					.dstBinding = BINDING_3D_SAMPLERS_SHADOW,
// 					.dstArrayElement = 0,   

// 					.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
// 					.descriptorCount = 1,
// 					.pImageInfo = &info
// 				};

// 				vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 			}
// 			for(int k=0;k<arrayTextures_3D.len;k++)
// 			{
// 				struct TextureRes * text =  gmArrayGet(&arrayTextures_3D, k);
// 				VkDescriptorImageInfo info = {
// 					.imageView = text->image.view,
// 					.sampler = text->textureSampler,
// 					.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
// 				};

// 				VkWriteDescriptorSet write = {
// 					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 					.dstSet = *frame->descriptorSets,
// 					.dstBinding = BINDING_3D_SAMPLERS,
// 					.dstArrayElement = k,   

// 					.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
// 					.descriptorCount = 1,
// 					.pImageInfo = &info
// 				};

// 				vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 			}
// 			for(int k=0;k<arrayTextures_2D.len;k++)
// 			{
// 				struct TextureRes * text =  gmArrayGet(&arrayTextures_2D, k);
// 				VkDescriptorImageInfo info = {
// 					.imageView = text->image.view,
// 					.sampler = text->textureSampler,
// 					.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
// 				};

// 				VkWriteDescriptorSet write = {
// 					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
// 					.dstSet = *frame->descriptorSets,
// 					.dstBinding = BINDING_2D_SAMPLERS,
// 					.dstArrayElement = k,   

// 					.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
// 					.descriptorCount = 1,
// 					.pImageInfo = &info
// 				};

// 				vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
// 			}

// 		}

		
// 	}

// }


void cleanShaderBuffers(){
	struct GmNode * n = list_BufferRes.head;
	while(n != NULL)
	{
		struct BufferRes* ref  =n->data;
		cleanBuffer(ref);
		n = n->next;
	}
	gmListFree(&list_BufferRes);
	
}

void mapBufferMemory(struct BufferRes * buffer){
	vkMapMemory(device, buffer->alloc.memory, 0, buffer->alloc.size, 0, &buffer->alloc.mapped);
	
}
void unmapBufferMemory(struct BufferRes * buffer){
	vkUnmapMemory(device, buffer->alloc.memory);
	buffer->alloc.mapped = NULL;
	// buffer->alloc.memory = NULL;
}

void createBuffers(){





	for(int i=0; i < MAX_FRAMES_IN_FLIGHT; i++){

		struct Frame * frame = &frames[i];

		createBufferRes(
			sizeof(struct DataInstanceMesh) *array_inst_mesh_data.len,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&frame->ssbo_instance_mesh
		);
		mapBufferMemory(&frame->ssbo_instance_mesh);
		gmListPushBack(&list_BufferRes,&frame->ssbo_instance_mesh);


		createBufferRes(
			sizeof(struct DataInstanceHUD) *array_inst_hud_data.len,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&frame->ssbo_instance_hud
		);
		mapBufferMemory(&frame->ssbo_instance_hud);
		gmListPushBack(&list_BufferRes,&frame->ssbo_instance_hud);

		
		createBufferRes(
			sizeof(struct DataGlobal) *array_global_data.len,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&frame->ubo_global
		);
		mapBufferMemory(&frame->ubo_global);
		gmListPushBack(&list_BufferRes,&frame->ubo_global);

		//
		createBufferRes(
			sizeof(struct DataLight) *array_light_data.len,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&frame->ubo_light
		);
		mapBufferMemory(&frame->ubo_light);
		gmListPushBack(&list_BufferRes,&frame->ubo_light);


		createBufferRes(
			sizeof(struct DataCamera) *array_camera_data.len,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&frame->ubo_camera
		);
		mapBufferMemory(&frame->ubo_camera);
		gmListPushBack(&list_BufferRes,&frame->ubo_camera);

		
	}

	
	createBufferRes(
		sizeof(struct DataGlobal)*array_material_data.len ,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&ssbo_material
	);
	mapBufferMemory(&ssbo_material);
	gmListPushBack(&list_BufferRes,&ssbo_material);
}

// void createUniformBuffers33(){


// 	for(int i=0; i < MAX_FRAMES_IN_FLIGHT; i++){

// 		struct Frame * frame = &frames[i];

		
// 		createBufferRes(
// 			sizeof(struct SSBO_Model)*(TOTAL_3D_OBJECT+TOTAL_2D_OBJECT), 
// 			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
// 			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
// 			&frame->ssbo_models
// 		);
// 		mapBufferMemory(&frame->ssbo_models);
// 		gmListPushBack(&list_BufferRes,&frame->ssbo_models);


// 		createBufferRes(
// 			sizeof(struct SSBO_Colors)*TOTAL_3D_OBJECT, 
// 			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
// 			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
// 			&frame->ssbo_colors
// 		);
// 		mapBufferMemory(&frame->ssbo_colors);
// 		gmListPushBack(&list_BufferRes,&frame->ssbo_colors);

// 		createBufferRes(
// 			sizeof(struct SSBO_ObjectID)*TOTAL_3D_OBJECT, 
// 			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
// 			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
// 			&frame->ssbo_objectIds
// 		);
// 		mapBufferMemory(&frame->ssbo_objectIds);
// 		gmListPushBack(&list_BufferRes,&frame->ssbo_objectIds);

// 		createBufferRes(
// 			sizeof(struct UBO_ViewProjection)*VIEW_PERSPECTIVES, 
// 			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
// 			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
// 			&frame->ubo_ViewProjection
// 		);
// 		mapBufferMemory(&frame->ubo_ViewProjection);
// 		gmListPushBack(&list_BufferRes,&frame->ubo_ViewProjection);

// 		createBufferRes(
// 			sizeof(struct UBO_DirectionLight)*1, 
// 			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
// 			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
// 			&frame->ubo_Lights
// 		);
// 		mapBufferMemory(&frame->ubo_Lights);
// 		gmListPushBack(&list_BufferRes,&frame->ubo_Lights);

	
// 	}
// }

void createDescriptorPool2() {

	uint32_t uniform_cnt = 3;
	uint32_t ssbo_cnt = 2;
	uint32_t shadow_cnt = 1;
	uint32_t ssbo_materials = 1;
	uint32_t samplers = 1;
	uint32_t descripto_sets = MAX_FRAMES_IN_FLIGHT * 3 + ssbo_materials + samplers;

	VkDescriptorPoolSize poolSizes[3] = {
		{
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = MAX_FRAMES_IN_FLIGHT * uniform_cnt 
		},
		{
			.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			.descriptorCount = (MAX_FRAMES_IN_FLIGHT * ssbo_cnt )+ ssbo_materials
		
		},
		{
			.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount =   (MAX_TEXTURES_2D + MAX_TEXTURES_3D) + shadow_cnt * MAX_FRAMES_IN_FLIGHT,
		},
	};

	VkDescriptorPoolCreateInfo poolInfo = {
				 
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.pNext = NULL,
		.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,  
		.maxSets =  descripto_sets,  
		.poolSizeCount = sizeof(poolSizes) / sizeof(VkDescriptorPoolSize),
		.pPoolSizes = poolSizes
	};

	
	VkResult result = vkCreateDescriptorPool(device, &poolInfo, NULL, &descriptorPool2);

	assert(result == VK_SUCCESS);
}

// void createDescriptorPool() {


// 	uint32_t alltextures = arrayTextures_3D.len + arrayTextures_2D.len + 1;

// 	VkDescriptorPoolSize poolSizes[] = {
// 		(VkDescriptorPoolSize){
// 			.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
// 			.descriptorCount =  MAX_FRAMES_IN_FLIGHT * (alltextures)
// 		},
// 		(VkDescriptorPoolSize){
// 			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 			.descriptorCount = 4 *  MAX_FRAMES_IN_FLIGHT
// 		},
// 		(VkDescriptorPoolSize){
// 			.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
// 			.descriptorCount = 4 *  MAX_FRAMES_IN_FLIGHT
// 		},
// 	};
	

 

// 	VkDescriptorPoolCreateInfo poolInfo = {
				 
// 		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
// 		.pNext = NULL,
// 		.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,  
// 		.maxSets = MAX_FRAMES_IN_FLIGHT,  
// 		.poolSizeCount = sizeof(poolSizes) / sizeof(VkDescriptorPoolSize),
// 		.pPoolSizes = poolSizes
// 	};

	

// 	VkResult result = vkCreateDescriptorPool(device, &poolInfo, NULL, &descriptorPool);

// 	if (result != VK_SUCCESS) {
		

// 	}
	


// };

void createDescriptorSetLayout22(){
	PRINT_FNAME;


	{
		VkDescriptorSetLayoutBinding globalBinding[3] ={
			{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
			},
			{
				.binding = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
			},
			{
				.binding = 2,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
			},
		};
		VkDescriptorSetLayoutCreateInfo globalLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = sizeof(globalBinding) / sizeof(VkDescriptorSetLayoutBinding),
			.pBindings = globalBinding
		};

		VkResult res = vkCreateDescriptorSetLayout(device, &globalLayoutInfo, NULL, &globalLayout);

		if(res != VK_SUCCESS){
			EXIT_CLEAN("vkCreateDescriptorSetLayout4 failed");
		}

	}
	{
		VkDescriptorSetLayoutBinding materialBinding = {
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			.descriptorCount = 1,
			.stageFlags =  VK_SHADER_STAGE_FRAGMENT_BIT
		};
			
		VkDescriptorSetLayoutCreateInfo materialLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &materialBinding
		};

		VkResult res = vkCreateDescriptorSetLayout(device, &materialLayoutInfo, NULL, &materialLayout);

		if(res != VK_SUCCESS){
			EXIT_CLEAN("vkCreateDescriptorSetLayout  materialDescriptorSetLayout");
		}

	}
	{
		VkDescriptorSetLayoutBinding instanceBindings[2] = {
			{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT| VK_SHADER_STAGE_FRAGMENT_BIT
			},
			{
				.binding = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT| VK_SHADER_STAGE_FRAGMENT_BIT
			}
		};

		VkDescriptorSetLayoutCreateInfo instanceLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 2,
			.pBindings = instanceBindings
		};

		VkResult res = vkCreateDescriptorSetLayout(device, &instanceLayoutInfo, NULL, &instanceLayout);

		if(res != VK_SUCCESS){
			EXIT_CLEAN("vkCreateDescriptorSetLayout instanceDescriptorSetLayout");
		}

	}
	{
		VkDescriptorSetLayoutBinding samplers[2] = {
			{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = MAX_TEXTURES_3D,
				.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
			},
			{
				.binding = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = MAX_TEXTURES_2D,
				.stageFlags =  VK_SHADER_STAGE_FRAGMENT_BIT
			}
		};

		VkDescriptorSetLayoutCreateInfo samplersLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 2,
			.pBindings = samplers
		};

		VkResult res = vkCreateDescriptorSetLayout(device, &samplersLayoutInfo, NULL, &samplerLayout);

		if(res != VK_SUCCESS){
			EXIT_CLEAN("vkCreateDescriptorSetLayout instanceDescriptorSetLayout");
		}

	}

}


// void createDescriptorSetLayout2(){
// 	PRINT_FNAME;


 

// 	VkDescriptorSetLayoutBinding lb[BINDING_MAX]={
// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_3D_SAMPLERS,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
// 			.descriptorCount = arrayTextures_3D.len,
// 			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
// 		},
// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_3D_SAMPLERS_SHADOW,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
// 			.descriptorCount = 1,
// 			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
// 		},
// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_2D_SAMPLERS,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
// 			.descriptorCount = arrayTextures_2D.len,
// 			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
// 		},
		

// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_VIEW_PROJECTIONS_3D,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 			.descriptorCount = 1,
// 			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
// 		},
// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_VIEW_PROJECTIONS_2D,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 			.descriptorCount = 1,
// 			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
// 		},
// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_VIEW_PROJECTIONS_LIGHT,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 			.descriptorCount = 1,
// 			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
// 		},
// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_DIRECTIONAL_LIGHTS,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 			.descriptorCount = 1,
// 			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
// 		},
		

// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_MODELS_3D,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
// 			.descriptorCount = 1,
// 			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
// 		},
// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_MODELS_2D,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
// 			.descriptorCount = 1,
// 			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
// 		},
// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_OBJECT_IDS,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
// 			.descriptorCount = 1,
// 			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
// 		},
// 		(VkDescriptorSetLayoutBinding){
// 			.binding = BINDING_COLORS,
// 			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
// 			.descriptorCount = 1,
// 			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
// 		},
// 	};
	
	


// 	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
// 		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
// 		.bindingCount = sizeof(lb) / sizeof(VkDescriptorSetLayoutBinding),
// 		.pBindings = lb,
// 	};

// 	VkResult res= vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, NULL, &descriptorSetLayout4);

// 	if(res != VK_SUCCESS){
// 		EXIT_CLEAN("vkCreateDescriptorSetLayout4 failed");
// 	}
// }


uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice,&physicalDeviceMemoryProperties);
	


	for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	EXIT_CLEAN("failed to find suitable memory type!");
}




void createBufferRes(
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	struct BufferRes * buffer
){
	memset(buffer, 0, sizeof(struct BufferRes));
	buffer->alloc.size = size;
	createBuffer(size,usage,properties,&buffer->handle, &buffer->alloc.memory);
	
}
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
	 
	VkResult res = vkCreateBuffer(device, &bufferCreateInfo, NULL, buffer);


	if (res != VK_SUCCESS) { 

		printf("VkDeviceSize :%ld, VkBufferUsageFlags %d, properties %d\n",size, usage,properties);

		EXIT_CLEAN("Cannot create buffer! vkCreateBuffer");
	}


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


	



	beginSingleTimeCommands(transferCommandBuffers);

	VkBufferCopy bufferCopy = {
		
		.size = size,
		.dstOffset = 0,
		.srcOffset = 0
	};
	VkCopyBufferInfo2 copyBufferInfo2 = {
		.sType = VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2,
		.dstBuffer = dstBuffer,
		.srcBuffer = srcBuffer,
		.pRegions = &(VkBufferCopy2){
			.sType = VK_STRUCTURE_TYPE_BUFFER_COPY_2,
			.size = size,
		},
		.regionCount = 1,
	};
	vkCmdCopyBuffer2(transferCommandBuffers, &copyBufferInfo2);
	


	endSingleTimeCommands(transferCommandBuffers);

}
void createIndexBuffer(
	uint32_t indicesNum,
	uint32_t * indices,
	VkBuffer *indexBuffer, 
	VkDeviceMemory *indexBufferMemory
	){

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

	vkUnmapMemory(device, bufferMemory);
	
	
	
	createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		indexBuffer,
		indexBufferMemory
	);

    copyBuffer(stagingBuffer, *indexBuffer,  bufferSize);

	vkDestroyBuffer(device, stagingBuffer, NULL);

	vkFreeMemory(device, bufferMemory, NULL);

}
void createIndexBuffer2(
	uint32_t indicesNum,
	uint32_t * indices,
	struct BufferRes *out
	){

	VkDeviceSize bufferSize = sizeof(uint32_t) * indicesNum;

	struct BufferRes staging = {};
	

	createBufferRes(
		bufferSize, 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT ,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		&staging
	);

	mapBufferMemory(&staging);
	// void * data = NULL;

	// vkMapMemory( device, bufferMemory, 0, bufferSize, 0, &data);

	memcpy(staging.alloc.mapped, indices, staging.alloc.size);

	// VkMappedMemoryRange mappedMemoryRange = {
	// 	.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
	// 	.memory = staging.alloc.memory,
	// 	.offset = 0,
	// 	.size = staging.alloc.size,
		
	// };

	// vkFlushMappedMemoryRanges(device, 1, &mappedMemoryRange);

	unmapBufferMemory(&staging);
	// vkUnmapMemory(device,staging.alloc.memory);


	createBufferRes(
		bufferSize, 
		VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_INDEX_BUFFER_BIT ,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		out
	);

	
	// createBuffer(
	// 	staging.alloc.size,
	// 	VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
	// 	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	// 	&out->handle,
	// 	out-
	// );

    copyBuffer(staging.handle, out->handle,  bufferSize);

	cleanBuffer(&staging);
	// vkDestroyBuffer(device, stagingBuffer, NULL);

	// vkFreeMemory(device, bufferMemory, NULL);
}
void createVertexBuffer2(

	uint32_t verticesNum,
	void * vertices,
	uint32_t data_size,
	struct BufferRes *out

) {

	
    // VkDeviceSize bufferSize = data_size;

	// VkBuffer stagingBuffer;
	
    // VkDeviceMemory bufferMemory;


	// createBuffer(
	// 	bufferSize,
	// 	VK_BUFFER_USAGE_TRANSFER_SRC_BIT ,
	// 	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	// 	&stagingBuffer,
	// 	&bufferMemory
	// );

	struct BufferRes staging = {};

	createBufferRes(
		data_size, 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT ,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		&staging
	);

	mapBufferMemory(&staging);
	// void * data = NULL;

	// vkMapMemory( device, bufferMemory, 0, bufferSize, 0, &data);

	memcpy(staging.alloc.mapped, vertices,staging.alloc.size);

	// VkMappedMemoryRange mappedMemoryRange = {
	// 	.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
	// 	.memory = staging.alloc.memory,
	// 	.offset = 0,
	// 	.size = staging.alloc.size,
		
	// };

	// vkFlushMappedMemoryRanges(device, 1, &mappedMemoryRange);

	unmapBufferMemory(&staging);
	// vkUnmapMemory(device,staging.alloc.memory);


	createBufferRes(
		data_size, 
		VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT ,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		out
	);

	
	// createBuffer(
	// 	staging.alloc.size,
	// 	VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
	// 	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	// 	&out->handle,
	// 	out-
	// );

    copyBuffer(staging.handle, out->handle,  data_size);

	cleanBuffer(&staging);
	// vkDestroyBuffer(device, stagingBuffer, NULL);

	// vkFreeMemory(device, bufferMemory, NULL);
}
void createVertexBuffer(

	uint32_t verticesNum,
	void * vertices,
	uint32_t data_size,
	VkBuffer *vertexBuffer, 
	VkDeviceMemory *vertexBufferMemory

) {

    VkDeviceSize bufferSize = data_size;

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

	memcpy(data, vertices, bufferSize);

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
		VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer,
		vertexBufferMemory
	);

    copyBuffer(stagingBuffer, *vertexBuffer,  bufferSize);

	vkDestroyBuffer(device, stagingBuffer, NULL);

	vkFreeMemory(device, bufferMemory, NULL);
}

 
static void framebufferResizeCallback(GLFWwindow *win,int w,int h)
{
	framebufferResized = true;
}


void createSurface(){
	PRINT_FNAME;

	glfwCreateWindowSurface(instance, window,NULL, &surface);
	
	
}

uint32_t createShaderFromFile(const char * path, uint8_t** buffer){

	PRINT_FNAME;


	char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current working directory:\n");
        printf("%s\n", cwd);
		printf("%s\n", path);
    } else {
        perror("getcwd() error");
    }


	FILE *file = fopen(path, "rb");  


    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }


	fseek(file, 0, SEEK_END);
    uint32_t size = ftell(file);
    rewind(file);

    *buffer = malloc(size);
    if (*buffer == NULL) {
        printf("Fill buffer mem alloc error\n");
        fclose(file);
        return 0;
    }

    

    fread(*buffer, 1, size, file);


	

    fclose(file);  


	return size;
}
void createCommandPool(){
	 
	PRINT_FNAME;


	VkCommandPoolCreateInfo commandPoolCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = queueFamilyIndeces[graphicsQueueFamilyArrayIndex],
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	
	vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, &graphicsCommnadPool);


	for(int i=0;i< MAX_FRAMES_IN_FLIGHT;i++){

		
		// frames[i].graphicsCommandBuffers;

			VkCommandBufferAllocateInfo allocInfo = { 
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				.commandPool =  graphicsCommnadPool,
				.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				.commandBufferCount = 1  ,
			};

			vkAllocateCommandBuffers(device, &allocInfo, &frames[i].graphicsCommandBuffers);
	}

	

	VkCommandPoolCreateInfo commandPoolCreateInfo2 = (VkCommandPoolCreateInfo){
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = queueFamilyIndeces[transferQueueFamilyArrayIndex] ,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	
	vkCreateCommandPool(device, &commandPoolCreateInfo2, NULL, &transferCommnadPool);
 
	VkCommandBufferAllocateInfo allocInfo = (VkCommandBufferAllocateInfo){ 
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool =  transferCommnadPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1  ,
		
	};

	vkAllocateCommandBuffers(device, &allocInfo, &transferCommandBuffers);

	
}

void renderHUD(
	VkCommandBuffer cmd, 
	struct Frame * frame,
	uint32_t imageIndex

){
	struct Pipeline *pipe_HUD = gmArrayGet(&arrayPipelines, 1);

	{
		VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
		VkRenderingAttachmentInfo colorAttachmentsInfos[1];
		{

			colorAttachmentsInfos[0] = (VkRenderingAttachmentInfo){
				
				.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
				.imageView = swapchainImageViews[imageIndex],
				.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.clearValue = clearColor
				
			};

		

		}
	
		VkRenderingInfo renderingInfoHUD = {
			.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
			.renderArea = { {0, 0}, swapChainExtent },
			.layerCount = 1,
			.colorAttachmentCount = 1,
			.pColorAttachments = colorAttachmentsInfos,
		
		};

		vkCmdBeginRendering(cmd, &renderingInfoHUD);
		{
			vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe_HUD->graphicsPipeline);
			VkDeviceSize offset = 0;

		

			vkCmdBindVertexBuffers(cmd, 0, 1, &modelVertexData_HUD2.vertexBuffer.handle, &offset);
			vkCmdBindIndexBuffer(cmd, modelVertexData_HUD2.indexBuffer.handle, 0, VK_INDEX_TYPE_UINT32);
			vkCmdBindDescriptorSets(
				cmd, 
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipe_HUD->pipelineLayout
				,0,1,
				frame->descriptorSets,
				0, NULL
			);
	
			struct PushConst2D hudConstants = {
				.texIdx = 2, 
				.hasColor = 0
			};
			vkCmdPushConstants(
            	cmd,
				pipe_HUD->pipelineLayout,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(struct PushConst2D),
				&hudConstants
			);
			

			
	
			vkCmdDrawIndexed(cmd, modelVertexData_HUD2.indices_num, 1, 0, 0, 0  );
		}
		vkCmdEndRendering(cmd);

	}
}
void renderMainPass(
	VkCommandBuffer cmd, 
	struct Frame * frame,
	uint32_t imageIndex
){


	{
			VkImageMemoryBarrier2 beginBarrier[]={

		
				(VkImageMemoryBarrier2){

					.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
					.srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
					.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
					.srcAccessMask = 0,
					.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
					.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,  
					.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
					.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
					.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
					.image = frame->pick.image.handle,
					.subresourceRange = {
						.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
						.baseMipLevel = 0,
						.levelCount = 1,
						.baseArrayLayer = 0,
						.layerCount = 1,
					},
				},
				

		};

	
		VkDependencyInfo beginDepInfo = {

			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
			.imageMemoryBarrierCount = sizeof(beginBarrier) / sizeof(VkImageMemoryBarrier2),
			.pImageMemoryBarriers = beginBarrier,
			
		};

		vkCmdPipelineBarrier2(cmd, &beginDepInfo);
	}

	



    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    VkClearValue clearDepth = {{{1.0f, 0}}};
    
	
	const uint32_t colorAttachmentCnt = 2;

	VkRenderingAttachmentInfo colorAttachmentsInfos[colorAttachmentCnt];
	{

		colorAttachmentsInfos[0] = (VkRenderingAttachmentInfo){
			
			.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
			.imageView = swapchainImageViews[imageIndex],
			.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.clearValue = clearColor
			
		};

		colorAttachmentsInfos[1] = (VkRenderingAttachmentInfo){
			
			.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
			.imageView = frame->pick.image.view,
			.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.clearValue  = { 
				.color = {{0, 0, 0, 0}} 
			}
		};

	}

    VkRenderingAttachmentInfo depthAttachmentInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = frame->deapth.view,
        .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .clearValue = clearDepth
    };

    VkRenderingInfo renderingInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = { 
			.offset = {0, 0}, 
			.extent = swapChainExtent 
		},
		
        .layerCount = 1,
        .colorAttachmentCount = colorAttachmentCnt,
        .pColorAttachments = colorAttachmentsInfos,

        .pDepthAttachment = &depthAttachmentInfo,
    };

    vkCmdBeginRendering(cmd, &renderingInfo);

	

	
	struct Pipeline *  pipeline = gmArrayGet(&arrayPipelines, 0);

	if (!pipeline || pipeline->graphicsPipeline == VK_NULL_HANDLE) {
		EXIT_CLEAN("ERROR in PIPELINE");
	}

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicsPipeline);
    
    VkViewport viewPort = {
        .x = 0, 
		.y = 0, 
		.width = swapChainExtent.width, 
		.height = swapChainExtent.height, 
        .minDepth = 0.0f, 
		.maxDepth = 1.0f
    };

    vkCmdSetViewport(cmd, 0, 1, &viewPort);
    
    VkRect2D scissor = { .extent = swapChainExtent, .offset = {0, 0} };
    vkCmdSetScissor(cmd, 0, 1, &scissor);



	
	
	// for(int i=0; i< arrayGameObjects.len;i++)
	// {
	// 	VkDeviceSize offset = 0;
	// 	struct GameObject * gameObject = gmArrayGet(&arrayGameObjects, i);
		

	
	// 	vkCmdBindVertexBuffers(cmd, 0, 1, &modelVertexData_3D[gameObject->vertexIdx].vertexBuffer.handle, &offset);
	// 	vkCmdBindIndexBuffer(cmd, modelVertexData_3D[gameObject->vertexIdx].indexBuffer.handle, 0, VK_INDEX_TYPE_UINT32);

	// 	vkCmdBindDescriptorSets(
	// 		cmd, 
	// 		VK_PIPELINE_BIND_POINT_GRAPHICS, 
	// 		pipeline->pipelineLayout, 0, 1, 
	// 		frame->descriptorSets, 
	// 		0, 
	// 		NULL
			


	// 	);
		
	// 	struct TextureRes * texture = gmArrayGet(&arrayTextures_3D, gameObject->textureIdx);
	// 	struct PushConst constants0 = {
	// 		.hasColor = false,
	// 		.texIdx = texture->textureIdx,
	// 		.objectId = frame->pick.pickedID
	// 	};
		

	// 	vkCmdPushConstants(
	// 		cmd,
	// 		pipeline->pipelineLayout,
	// 		VK_SHADER_STAGE_FRAGMENT_BIT,
	// 		0,
	// 		sizeof(struct PushConst),
	// 		&constants0
	// 	);

	// 	vkCmdDrawIndexed(
	// 		cmd, 
	// 		modelVertexData_3D[gameObject->vertexIdx].indices_num,
	// 		gameObject->arrayViewUboModel.len , 
	// 		0, 
	// 		0,
	// 		gameObject->arrayViewUboModel.offset
	// 	);
		

	// }
	// draw light
	{
		
	}
	



    vkCmdEndRendering(cmd);


	
	VkImageMemoryBarrier2 toTransferBarrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
		.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		.dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = frame->pick.image.handle,
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0, .levelCount = 1,
			.baseArrayLayer = 0, .layerCount = 1
		},
	};

	VkDependencyInfo toTransferDep = {
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &toTransferBarrier
	};
	vkCmdPipelineBarrier2(cmd, &toTransferDep);
	

	VkBufferImageCopy region = {
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel = 0, .baseArrayLayer = 0, .layerCount = 1
		},
		.imageOffset = {swapChainExtent.width/2,swapChainExtent.height/2, 0},
		.imageExtent = {1, 1, 1}
	};
	vkCmdCopyImageToBuffer(
		cmd,
		frame->pick.image.handle,
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		frame->pick.buffer.handle,
		1, 
		&region
	);

	VkImageMemoryBarrier2 toGeneralBarrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,           
		.dstStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
		.srcAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT,      
		.dstAccessMask = 0,
		.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_GENERAL,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = frame->pick.image.handle,
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		}
	};
	VkDependencyInfo toGeneralDep = {
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &toGeneralBarrier
	};
	vkCmdPipelineBarrier2(cmd, &toGeneralDep);
}
void renderShadowMap(
	VkCommandBuffer cmd, 
	struct Frame * frame,
	uint32_t imageIndex
){



	VkImageMemoryBarrier2 barrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		
		.srcStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT,
        .srcAccessMask = VK_ACCESS_2_SHADER_SAMPLED_READ_BIT,
        .dstAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,  // ✅ FROM
        .newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,  // ✅ TO
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,

		.image = frame->shadow.image.handle,

		// .image = frame->shadow.image.handle,
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
			.levelCount = 1,
			.layerCount = 1
		}
	};
		

	
	VkDependencyInfo beginDepInfo = {

		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &barrier,
		
	};

	vkCmdPipelineBarrier2(cmd, &beginDepInfo);
	


    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    VkClearValue clearDepth = {{{1.0f, 0}}};
    
	
	


    VkRenderingAttachmentInfo depthAttachmentInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = frame->shadow.image.view,
        .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .clearValue = clearDepth
    };

    VkRenderingInfo renderingInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = { 
			.offset = {0, 0}, 
			.extent = swapChainExtent 
		},
		
        .layerCount = 1,
        .colorAttachmentCount = 0,
        .pColorAttachments = NULL,

        .pDepthAttachment = &depthAttachmentInfo,
    };

    vkCmdBeginRendering(cmd, &renderingInfo);

	

	
	struct Pipeline *  pipeline = gmArrayGet(&arrayPipelines, 2);

	if (!pipeline || pipeline->graphicsPipeline == VK_NULL_HANDLE) {
	
		EXIT_CLEAN("ERROR in PIPELINE Shadow");
	}

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicsPipeline);
    
    VkViewport viewPort = {
        .x = 0, 
		.y = 0, 
		.width = swapChainExtent.width, 
		.height = swapChainExtent.height, 
        .minDepth = 0.0f, 
		.maxDepth = 1.0f
    };

    vkCmdSetViewport(cmd, 0, 1, &viewPort);
    
    VkRect2D scissor = { .extent = swapChainExtent, .offset = {0, 0} };
    vkCmdSetScissor(cmd, 0, 1, &scissor);



	//TODO!
	// for(int i=0; i< arrayGameObjects.len;i++)
	// {
	// 	VkDeviceSize offset = 0;
	// 	struct GameObject * gameObject = gmArrayGet(&arrayGameObjects, i);
		

	// 	{
	// 		vkCmdBindVertexBuffers(cmd, 0, 1, &modelVertexData_3D[gameObject->vertexIdx].vertexBuffer.handle, &offset);
	// 		vkCmdBindIndexBuffer(cmd, modelVertexData_3D[gameObject->vertexIdx].indexBuffer.handle, 0, VK_INDEX_TYPE_UINT32);
	
	// 		vkCmdBindDescriptorSets(
	// 			cmd, 
	// 			VK_PIPELINE_BIND_POINT_GRAPHICS, 
	// 			pipeline->pipelineLayout, 0, 1, 
	// 			frame->descriptorSets, 
	// 			0, 
	// 			NULL
				


	// 		);

	// 		vkCmdDrawIndexed(
	// 			cmd, 
				

	// 			modelVertexData_3D[gameObject->vertexIdx].indices_num,
	// 			gameObject->arrayViewUboModel.len , 
	// 			0, 
	// 			0,
	// 			gameObject->arrayViewUboModel.offset
	// 		);
	// 	}
	// }

    vkCmdEndRendering(cmd);

	VkImageMemoryBarrier2 endBarrier = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
			.srcStageMask = VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
			.srcAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
			.dstAccessMask = VK_ACCESS_2_SHADER_SAMPLED_READ_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
			.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = frame->shadow.image.handle,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			},
		};
		VkDependencyInfo endDepInfo = {
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &endBarrier,
		};
		vkCmdPipelineBarrier2(cmd, &endDepInfo);  // ← MUST be after vkCmdEndRendering



}
void recordCommandBuffer3(uint32_t imageIndex,uint32_t frameIndex){

	struct Frame * frame = &frames[frameIndex];

	VkCommandBuffer commandBuffer = frame->graphicsCommandBuffers;
	VkDescriptorSet * descriptorSet = frame->descriptorSets;

	VkImageView * pickImageView = &frame->pick.image.view;

	VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0, 
        .pInheritanceInfo = NULL,
    };
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

	{
		
		VkImageMemoryBarrier2 beginBarrier[]={

			(VkImageMemoryBarrier2){

				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
				.srcStageMask = VK_PIPELINE_STAGE_2_NONE,
				.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
				.srcAccessMask = 0,
				.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED ,  

				.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = swapchainImages[imageIndex],
				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

		};

	
		VkDependencyInfo beginDepInfo = {

			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
			.imageMemoryBarrierCount = sizeof(beginBarrier) / sizeof(VkImageMemoryBarrier2),
			.pImageMemoryBarriers = beginBarrier,
		};

		vkCmdPipelineBarrier2(commandBuffer, &beginDepInfo);
	}

	renderShadowMap(commandBuffer,frame, imageIndex);

	renderMainPass(commandBuffer,frame, imageIndex);

	

	


	renderHUD(commandBuffer,frame, imageIndex);


	



	VkImageMemoryBarrier2 endBarrier[]={

		 (VkImageMemoryBarrier2){
			 .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
			 .srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
			 .dstStageMask = VK_PIPELINE_STAGE_2_NONE,
			 .srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
			 .dstAccessMask = 0,
			 .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			 .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			 .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			 .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			 .image = swapchainImages[imageIndex],
			 .subresourceRange = { 
				 .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, 
				 .baseMipLevel = 0, 
				 .levelCount = 1, 
				 .baseArrayLayer = 0, 
				 .layerCount = 1 
			 },
		 },
		
	 };
   

    VkDependencyInfo dependencyInfo = {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = sizeof(endBarrier)/ sizeof(VkImageMemoryBarrier2), 
        .pImageMemoryBarriers = endBarrier
    };
    vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);

    vkEndCommandBuffer(commandBuffer);
}
// void updateUniformBuffer33(uint32_t currentFrame){

// 	struct Frame * frame = &frames[currentFrame];

	
// 	memcpy(frame->ssbo_colors.alloc.mapped,arrayColors_2.data,sizeof(struct SSBO_Colors)* arrayColors_2.len);

// 	memcpy(
// 		frame->ssbo_models.alloc.mapped, 
// 		array_ssbo_models3d.data, 
// 		sizeof(struct SSBO_Model)* array_ssbo_models3d.len
// 	);

// 	memcpy(
// 		frame->ssbo_models.alloc.mapped + sizeof(struct SSBO_Model)* array_ssbo_models3d.len, 
// 		array_ssbo_models2d.data, 
// 		sizeof(struct SSBO_Model)* array_ssbo_models2d.len
// 	);

// 	memcpy(frame->ssbo_objectIds.alloc.mapped, arraySBO_ObjectIds_1.data, sizeof(struct SSBO_ObjectID)* arraySBO_ObjectIds_1.len);


	



// 	struct UBO_ViewProjection ubo_vp[3];
// 	{
// 		struct UBO_ViewProjection* ubo = &ubo_vp[0];
// 		uint32_t stride = sizeof(struct UBO_ViewProjection);
// 		uint32_t offset = 0;
	
// 		vec3 cameraCenter;
	
// 		glm_vec3_add(cameraPos, cameraFront, cameraCenter);
	
// 		glm_lookat(cameraPos, cameraCenter, cameraUp, ubo->view);
	
	
// 		glm_perspective(glm_rad(45.0f), (float)swapChainExtent.width / swapChainExtent.height, 0.1f, 40.0f , ubo->proj);
	
// 		ubo->proj[1][1] *= -1;

// 	}
	




// 	{

// 		struct UBO_ViewProjection* ubo = &ubo_vp[1];
// 		glm_mat4_identity(ubo->view);
	
		
// 		glm_ortho(
// 			0.0f, 
// 			(float)swapChainExtent.width, 
// 			(float)swapChainExtent.height, 
// 			0.0f, -1.0f, 1.0f,
// 			ubo->proj);
	
	
		

// 	}


	


// 	GLM_VEC3_COPY(UBO_DirLight.viewPos,cameraPos);


	

// 	UBO_DirLight.lightPos[0] = 5.0f*sin(lastTime);
	

// 	memcpy(frame->ubo_Lights.alloc.mapped, &UBO_DirLight, sizeof(struct UBO_DirectionLight));
	



// 	{

// 		struct UBO_ViewProjection* ubo = &ubo_vp[2];
// 		glm_mat4_identity(ubo->view);
	
	
// 		vec3 LightCenter;
		
// 		glm_vec3_add(UBO_DirLight.lightPos, UBO_DirLight.viewPos, LightCenter);
	
// 		glm_lookat(UBO_DirLight.lightPos, LightCenter, cameraUp, ubo->view);
	
// 		glm_perspective(glm_rad(45.0f), (float)swapChainExtent.width / swapChainExtent.height, 0.1f, 40.0f , ubo->proj);
	
// 		ubo->proj[1][1] *= -1;
	
		

// 	}

// 	memcpy(frame->ubo_ViewProjection.alloc.mapped, ubo_vp, sizeof(ubo_vp));

// }
void updateObjects(uint32_t currentFrame){

	struct Frame * frame = &frames[currentFrame];


	UBO_DirLight.lightPos[0] = 5.0f*sin(lastTime);


	updateGlobal(global_data);
	updateLight(light_data);
	updateCamera(camera_data);


}
void updateBuffers(uint32_t currentFrame){

	struct Frame * frame = &frames[currentFrame];

	

	memcpy(
		frame->ssbo_instance_mesh.alloc.mapped,
		array_inst_mesh_data.data,
		sizeof(struct DataInstanceMesh)* array_inst_mesh_data.len
	);
	memcpy(
		frame->ssbo_instance_hud.alloc.mapped,
		array_inst_hud_data.data,
		sizeof(struct DataInstanceHUD)* array_inst_hud_data.len
	);

	memcpy(
		frame->ubo_global.alloc.mapped,
		array_global_data.data,
		sizeof(struct DataGlobal)* array_global_data.len
	);
	memcpy(
		frame->ubo_camera.alloc.mapped,
		array_camera_data.data,
		sizeof(struct DataCamera)* array_camera_data.len
	);
	memcpy(
		frame->ubo_light.alloc.mapped,
		array_light_data.data,
		sizeof(struct DataLight)* array_light_data.len
	);

	memcpy(
		ssbo_material.alloc.mapped,
		array_material_data.data,
		sizeof(struct DataMaterial)* array_material_data.len
	);
	


	// memcpy(frame->ssbo_colors.alloc.mapped,arrayColors_2.data,sizeof(struct SSBO_Colors)* arrayColors_2.len);

	// memcpy(
	// 	frame->ssbo_models.alloc.mapped, 
	// 	array_ssbo_models3d.data, 
	// 	sizeof(struct SSBO_Model)* array_ssbo_models3d.len
	// );

	// memcpy(
	// 	frame->ssbo_models.alloc.mapped + sizeof(struct SSBO_Model)* array_ssbo_models3d.len, 
	// 	array_ssbo_models2d.data, 
	// 	sizeof(struct SSBO_Model)* array_ssbo_models2d.len
	// );

	// memcpy(frame->ssbo_objectIds.alloc.mapped, arraySBO_ObjectIds_1.data, sizeof(struct SSBO_ObjectID)* arraySBO_ObjectIds_1.len);


	



	// struct UBO_ViewProjection ubo_vp[3];
	// {
	// 	struct UBO_ViewProjection* ubo = &ubo_vp[0];
	// 	uint32_t stride = sizeof(struct UBO_ViewProjection);
	// 	uint32_t offset = 0;
	
	// 	vec3 cameraCenter;
	
	// 	glm_vec3_add(cameraPos, cameraFront, cameraCenter);
	
	// 	glm_lookat(cameraPos, cameraCenter, cameraUp, ubo->view);
	
	
	// 	glm_perspective(glm_rad(45.0f), (float)swapChainExtent.width / swapChainExtent.height, 0.1f, 40.0f , ubo->proj);
	
	// 	ubo->proj[1][1] *= -1;

	// }
	




	// {

	// 	struct UBO_ViewProjection* ubo = &ubo_vp[1];
	// 	glm_mat4_identity(ubo->view);
	
		
	// 	glm_ortho(
	// 		0.0f, 
	// 		(float)swapChainExtent.width, 
	// 		(float)swapChainExtent.height, 
	// 		0.0f, -1.0f, 1.0f,
	// 		ubo->proj);
	
	
		

	// }


	


	// GLM_VEC3_COPY(UBO_DirLight.viewPos,cameraPos);


	

	// UBO_DirLight.lightPos[0] = 5.0f*sin(lastTime);
	

	// memcpy(frame->ubo_Lights.alloc.mapped, &UBO_DirLight, sizeof(struct UBO_DirectionLight));
	



	// {

	// 	struct UBO_ViewProjection* ubo = &ubo_vp[2];
	// 	glm_mat4_identity(ubo->view);
	
	
	// 	vec3 LightCenter;
		
	// 	glm_vec3_add(UBO_DirLight.lightPos, UBO_DirLight.viewPos, LightCenter);
	
	// 	glm_lookat(UBO_DirLight.lightPos, LightCenter, cameraUp, ubo->view);
	
	// 	glm_perspective(glm_rad(45.0f), (float)swapChainExtent.width / swapChainExtent.height, 0.1f, 40.0f , ubo->proj);
	
	// 	ubo->proj[1][1] *= -1;
	
		

	// }

	// memcpy(frame->ubo_ViewProjection.alloc.mapped, ubo_vp, sizeof(ubo_vp));

}
// void updateUniformBuffer333(uint32_t currentFrame){

// 	struct Frame * frame = &frames[currentFrame];

	
// 	memcpy(frame->ssbo_colors.alloc.mapped,array_obj_color.data,sizeof(struct SSBO_Colors)* array_obj_color.len);

// 	memcpy(
// 		frame->ssbo_models.alloc.mapped, 
// 		array_ssbo_models3d.data, 
// 		sizeof(struct SSBO_Model)* array_ssbo_models3d.len
// 	);

// 	memcpy(
// 		frame->ssbo_models.alloc.mapped + sizeof(struct SSBO_Model)* array_ssbo_models3d.len, 
// 		array_ssbo_models2d.data, 
// 		sizeof(struct SSBO_Model)* array_ssbo_models2d.len
// 	);

// 	memcpy(frame->ssbo_objectIds.alloc.mapped, array_object_ids.data, sizeof(struct SSBO_ObjectID)* array_object_ids.len);


	



// 	struct UBO_ViewProjection ubo_vp[3];
// 	{
// 		struct UBO_ViewProjection* ubo = &ubo_vp[0];
// 		uint32_t stride = sizeof(struct UBO_ViewProjection);
// 		uint32_t offset = 0;
	
// 		vec3 cameraCenter;
	
// 		glm_vec3_add(cameraPos, cameraFront, cameraCenter);
	
// 		glm_lookat(cameraPos, cameraCenter, cameraUp, ubo->view);
	
	
// 		glm_perspective(glm_rad(45.0f), (float)swapChainExtent.width / swapChainExtent.height, 0.1f, 40.0f , ubo->proj);
	
// 		ubo->proj[1][1] *= -1;

// 	}
	




// 	{

// 		struct UBO_ViewProjection* ubo = &ubo_vp[1];
// 		glm_mat4_identity(ubo->view);
	
		
// 		glm_ortho(
// 			0.0f, 
// 			(float)swapChainExtent.width, 
// 			(float)swapChainExtent.height, 
// 			0.0f, -1.0f, 1.0f,
// 			ubo->proj);
	
	
		

// 	}


	


// 	GLM_VEC3_COPY(UBO_DirLight.viewPos,cameraPos);


	

// 	UBO_DirLight.lightPos[0] = 5.0f*sin(lastTime);
	

// 	memcpy(frame->ubo_Lights.alloc.mapped, &UBO_DirLight, sizeof(struct UBO_DirectionLight));
	

	
// 	struct SSBO_Model * mod = gmArrayGet(&array_ssbo_models3d, obj_light.object_ids.start);
// 	glm_mat4_identity(mod->model);
	
// 	glm_translate(mod->model, UBO_DirLight.lightPos);

// 	memcpy(
// 		frame->ssbo_models.alloc.mapped + obj_light.object_ids.start, 
// 		mod->model, 
// 		sizeof(struct SSBO_Model)
// 	);



	

// 	{

// 		struct UBO_ViewProjection* ubo = &ubo_vp[2];
// 		glm_mat4_identity(ubo->view);
	
	
// 		vec3 LightCenter;
		
// 		glm_vec3_add(UBO_DirLight.lightPos, UBO_DirLight.viewPos, LightCenter);
	
// 		glm_lookat(UBO_DirLight.lightPos, LightCenter, cameraUp, ubo->view);
	
// 		glm_perspective(glm_rad(45.0f), (float)swapChainExtent.width / swapChainExtent.height, 0.1f, 40.0f , ubo->proj);
	
// 		ubo->proj[1][1] *= -1;
	
		

// 	}

// 	memcpy(frame->ubo_ViewProjection.alloc.mapped, ubo_vp, sizeof(ubo_vp));

// }


VkResult acquireNextImage(
	struct Frame * frame,
	uint32_t *out_image_index){


	VkResult	result = vkAcquireNextImageKHR(
		device, 
		swapchain, 
		UINT64_MAX, 
		frame->presentCompleteSemaphore, 
		NULL, 
		out_image_index
	);

	if(result != VK_SUCCESS && result!= VK_SUBOPTIMAL_KHR)
	{
		assert(result == VK_TIMEOUT || result == VK_NOT_READY);
		EXIT_CLEAN("failed to acquire swapchain image!");
	}

	return result;
}
void drawFrame3() {

	struct Frame * frame = &frames[frameIndex];
 
	VkResult result = vkWaitForFences(
		device, 
		1, 
		&frame->inFlightFence,
		VK_TRUE, UINT64_MAX
	);


	
	if(result != VK_SUCCESS)
	{
		EXIT_CLEAN("failed to wait for fence!");
	}


	uint32_t pickedObjectId = *(uint32_t*)frame->pick.buffer.alloc.mapped;
	frame->pick.pickedID = pickedObjectId;
	if(frame->pick.pickedID != 0)
	{
		

	}

	uint32_t imageIndex =  -1;

	 
	result = vkAcquireNextImageKHR(
		device, 
		swapchain, 
		UINT64_MAX, 
		frame->presentCompleteSemaphore, 
		NULL, 
		&imageIndex
	);
 	
	if(result == VK_ERROR_OUT_OF_DATE_KHR){

		recreateSwapChain();

		return;

	}
	
	updateObjects(frameIndex);
	
	updateBuffers(frameIndex);
	
	vkResetFences(device, 1, &frame->inFlightFence);

	
	
	recordCommandBuffer3(imageIndex,frameIndex);



	VkPipelineStageFlags2 stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
	
	VkSubmitInfo2 submitInfo2 ={
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,

		.waitSemaphoreInfoCount = 1,
		.pWaitSemaphoreInfos = &(VkSemaphoreSubmitInfo){
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
			.semaphore = frame->presentCompleteSemaphore,
			.stageMask = stageMask,
			.value = 0,
			.deviceIndex = 0,

		},

		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &(VkCommandBufferSubmitInfo){
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.commandBuffer = frame->graphicsCommandBuffers,
			
		},
		.signalSemaphoreInfoCount = 1,
		.pSignalSemaphoreInfos = &(VkSemaphoreSubmitInfo) {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
			.semaphore = frame->renderFinishedSemaphore,
			.stageMask = stageMask,
			
		},
	};
	
	 
	vkQueueSubmit2(graphicsQueue, 1, &submitInfo2, frame->inFlightFence);
	
	

	VkPresentInfoKHR presentInfo = {

		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &frame->renderFinishedSemaphore,
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
void createSyncObjects3(){
	
	PRINT_FNAME;


	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		struct Frame * frame = &frames[i];
	
		VkSemaphoreCreateInfo semaphoreCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};
		vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &frame->presentCompleteSemaphore);

		vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &frame->renderFinishedSemaphore);


		VkFenceCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT,
		};

		vkCreateFence(device, &createInfo, NULL,  &frame->inFlightFence);
	}

	VkFenceCreateInfo fenceInfo = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
		

		

	};
	vkCreateFence(device, &fenceInfo, NULL, &transferFence);
		
}

void queueFamilyCheck(){

	PRINT_FNAME;
	

	uint32_t queueFamilyPropertieCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertieCount, NULL);

	VkQueueFamilyProperties queueFamilyProperties[queueFamilyPropertieCount];

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertieCount, queueFamilyProperties);
	


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

	

	int supportedCnt = 0;
	

	 
	for(int i=0;i < deviceExtensionPropertieCount; i++){

		


		for(int k = 0;k < requiredDeviceExtensionCnt ; k++)
		{
			if(strcmp(exp_props[i].extensionName, requiredDeviceExtensions[k]) == 0){
				printf("\tExtension found %s\n",requiredDeviceExtensions[k]);
				supportedCnt ++;
				break;
			}
		}
	}
	
	
	if(!(supportedCnt == requiredDeviceExtensionCnt)){
		printf("No suported extensions\n");
		printf("supportedCnt: %d of %d\n",supportedCnt, requiredDeviceExtensionCnt);

		for(int i=0;i < deviceExtensionPropertieCount; i++){

			printf("\tfound: %s\n",exp_props[i].extensionName);
		}
		for(int i=0;i < requiredDeviceExtensionCnt; i++){

			printf("\trequired: %s\n",requiredDeviceExtensions[i]);
		}
		

		EXIT_CLEAN("\n");
	}

}

void physicalDeviceFeatureCheck(){

	PRINT_FNAME;

	VkPhysicalDeviceFeatures2 physicalDeviceFeatures2={
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
	
		

	};


	VkPhysicalDeviceVulkan12Features features12 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
	};


	VkPhysicalDeviceVulkan13Features features13 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		

		

		
	};

	VkPhysicalDeviceExtendedDynamicStateFeaturesEXT physicalDeviceExtendedDynamicStateFeaturesEXT = {

		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
	};

	features13.pNext = &physicalDeviceExtendedDynamicStateFeaturesEXT;
	features12.pNext = &features13;
	physicalDeviceFeatures2.pNext = &features12;


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
						if(next->features.vertexPipelineStoresAndAtomics)
						{
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
			case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES:
					printf("supported: VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES\n");

					{
						VkPhysicalDeviceVulkan12Features * temp = (VkPhysicalDeviceVulkan12Features*)next;
						if(temp->runtimeDescriptorArray ){
							all_ok +=1;
						}
					}
				break;
			default:
					printf("next %d\n",next->sType);
				break;
		}		
		next = next->pNext;
	}
	if(all_ok != 6){

		printf("supported :%d \n",all_ok);

		EXIT_CLEAN("Some Device features not supported");
	}
	

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

	VkPhysicalDeviceVulkan12Features physicalDeviceFeatures12 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.runtimeDescriptorArray = VK_TRUE,
	};


	VkPhysicalDeviceVulkan13Features physicalDeviceFeatures13={

		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.dynamicRendering = VK_TRUE,
		.synchronization2 = VK_TRUE,
		
		
	};

	VkPhysicalDeviceExtendedDynamicStateFeaturesEXT physicalDeviceExtendedDynamicStateFeaturesEXT = {

		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
		.extendedDynamicState = VK_TRUE,
		
		
	};

	physicalDeviceExtendedDynamicStateFeaturesEXT.pNext = NULL;
	physicalDeviceFeatures13.pNext = &physicalDeviceExtendedDynamicStateFeaturesEXT;
	physicalDeviceFeatures12.pNext = &physicalDeviceFeatures13;
	physicalDeviceFeatures2.pNext = &physicalDeviceFeatures12;

	VkDeviceCreateInfo deviceCreateInfo = {

		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = &deviceQueueCreateInfo,
		.queueCreateInfoCount = 1,
		.pNext = &physicalDeviceFeatures2,
		.ppEnabledExtensionNames = requiredDeviceExtensions,
		.enabledExtensionCount = requiredDeviceExtensionCnt,
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
	bool supportsVulkan1_3 = properties.apiVersion >= VK_VERSION_1_3;
	printf("API >= 1.3 support: %d\n",supportsVulkan1_3);
	printf("\n");



}

void createSwapchain(){
	PRINT_FNAME;
	
	
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		physicalDevice, 
		surface, 
		&surfaceCapabilities
	);

	
	printf(
		"Surface extent: w:%d h:%d\n",
		surfaceCapabilities.currentExtent.width, 
		surfaceCapabilities.currentExtent.height
	);

	uint32_t physicalDeviceSurfaceFormatCount = 0;

	vkGetPhysicalDeviceSurfaceFormatsKHR(
		physicalDevice, 
		surface, 
		&physicalDeviceSurfaceFormatCount , 
		NULL
	);

	VkSurfaceFormatKHR surfaceFormats[physicalDeviceSurfaceFormatCount];

	vkGetPhysicalDeviceSurfaceFormatsKHR(
		physicalDevice, surface, 
		&physicalDeviceSurfaceFormatCount , 
		surfaceFormats
	);

	printf("Surface formats:\n");

	
	swapchainSurfaceFormat = VK_FORMAT_B8G8R8A8_SRGB;

	swapchainSurfaceColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

	

	
	
	for(int i=0;i<physicalDeviceSurfaceFormatCount;i++)
	{
		printf("\tSurfaceFormat.format: %d\n",surfaceFormats[i].format);

		

		if(surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB)
		{

			swapchainSurfaceFormat = surfaceFormats[i].format;
			
		}
	}

	for(int i=0;i<physicalDeviceSurfaceFormatCount;i++)
	{
		printf("\tSurfaceFormat.colorSpace:%d\n",surfaceFormats[i].colorSpace);

	

		if(surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			swapchainSurfaceColorSpace = surfaceFormats[i].colorSpace;
			
			
		}
		
	}

	uint32_t presentModeCount = 0 ;

	vkGetPhysicalDeviceSurfacePresentModesKHR(
		physicalDevice,
		surface, 
		&presentModeCount, 
		NULL
	);

	VkPresentModeKHR presentModes[presentModeCount];
	
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		physicalDevice, 
		surface, 
		&presentModeCount, 
		presentModes
	);
	


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

	if(
		swapChainExtent.width > surfaceCapabilities.maxImageExtent.width &&
		swapChainExtent.height > surfaceCapabilities.maxImageExtent.height 
	){
		

	
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
	){
		swapChainExtent = (VkExtent2D){
			.width = width,
			.height = height,
		};
	}
	
	/*
		INFO
		


		Color attachments: at least 4–8 (depends on GPU)
		Depth/stencil: 1

		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		 - one per queue family
	*/

 	uint32_t imageCount = 2;

	

	if (imageCount < surfaceCapabilities.minImageCount){

		imageCount = surfaceCapabilities.minImageCount;
	}

	

	if (surfaceCapabilities.maxImageCount > 0 &&
		imageCount > surfaceCapabilities.maxImageCount){

		imageCount = surfaceCapabilities.maxImageCount;
	}
	
	VkSwapchainCreateInfoKHR createInfo={
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.minImageCount = imageCount,
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

	/*
	INFO

	how many images you want (minImageCount)
	how they should be formatted (imageFormat)
	how they will be used (imageUsage)
	their size (imageExtent)

	*/

	vkGetSwapchainImagesKHR(device, swapchain,  &swapchainImageCount,NULL);

	vkGetSwapchainImagesKHR(device, swapchain,  &swapchainImageCount, swapchainImages);



	printf("pSwapchainImageCount %d\n",swapchainImageCount);
	
	swapchainImageViewCount = swapchainImageCount;

	for(int i=0;i<swapchainImageCount;i++)
	{

		


		createImageView(&swapchainImageViews[i], &swapchainImages[i], swapchainSurfaceFormat, VK_IMAGE_ASPECT_COLOR_BIT,1);

		

	}

	

 
}
void cleanAllocation(struct Allocation * alloc){
	//PRINT_FNAME;
	if(alloc->mapped != NULL){

		vkUnmapMemory(device,alloc->memory);
		alloc->mapped = NULL;
	}	
	


	if( alloc->memory != NULL)
	{
		vkFreeMemory(device, alloc->memory, NULL);
		alloc->memory = NULL;
	}
			
}
void cleanImageRes(struct ImageRes * img){
	//PRINT_FNAME;
	vkDestroyImage(device,img->handle, NULL);
	vkDestroyImageView(device, img->view, NULL);
	cleanAllocation(&img->alloc);
	
}
void cleanBuffer(struct BufferRes * buff){
	//PRINT_FNAME;
	if(buff->handle!= NULL){
		vkDestroyBuffer(device,buff->handle, NULL);
		buff->handle = NULL;
	}
	
	cleanAllocation(&buff->alloc);
	
}
void cleanTextureRes(struct TextureRes * tex){
		// PRINT_FNAME;
		cleanImageRes(&tex->image);
		vkDestroySampler(device, tex->textureSampler, NULL);
		
		

		

		

}
void cleanupPickImages(){
	//PRINT_FNAME;
	for(int i=0;i< MAX_FRAMES_IN_FLIGHT;i++)
	{
		struct Frame * frame = &frames[i];

		cleanImageRes(&frame->pick.image);
		cleanBuffer(&frame->pick.buffer);
	
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

	cleanupPickImages();

	createSwapchain();	

	createImageViews();

	createPickImage();
	
}

VkBool32 debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData)
{
	
	printf("[validation layer]: %d %d %s\n",messageSeverity,messageTypes ,pCallbackData->pMessage);
	return VK_FALSE;
}




void setupDebugMessenger()
{
	PRINT_FNAME;

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
     

		printf("[NULl] %s\n","PFN_vkCreateDebugUtilsMessengerEXT");
	
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

	
	

	for(int i=0;i<glfwExtensionCountExtra;i++){

		printf("\tglfw required extensions: %s\n",glfwExtensionsExtra[i]);
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

		printf("Print layers: %d\n",layerCount);
		bool validationLayerSupported = false;
		for (uint32_t i = 0; i < layerCount; i++) {
			
			printf("layer: %s\n",layers[i].layerName);

			uint32_t cnt = 0;
			vkEnumerateInstanceExtensionProperties(layers[i].layerName, &cnt, NULL);

			if(cnt > 0)
			{
				VkExtensionProperties expr[cnt];
				vkEnumerateInstanceExtensionProperties(layers[i].layerName, &cnt, expr);

				for(int i=0;i<cnt;i++){

					printf("\tlayer extensions: %s\n",expr[i].extensionName);
				}
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
		instanceCreateInfo.enabledLayerCount = validationLayerCnt;
		instanceCreateInfo.ppEnabledLayerNames = validationLayers;

	}


	if(vkCreateInstance(&instanceCreateInfo,NULL,&instance) != VK_SUCCESS ){
		
		EXIT_CLEAN("vkCreateInstance failed");
	
	}


}
void createShadowPipeline(struct Pipeline * pipeline){
	
	PRINT_FNAME;

	

	char * path = pipeline->frag_path;
	uint8_t* data = NULL;
	uint32_t dataSize = createShaderFromFile(path, &data);
	
	


 
		

	if(dataSize == 0)
	{

		printf("Shader path: %s\n",path);
		EXIT_CLEAN("failed to read fragment shader file\n");
	
	}
	VkShaderModuleCreateInfo createInfo={
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,
	 
		
	};
 
	vkCreateShaderModule(device, &createInfo, NULL, &pipeline->shaderModuleFrag);

	free(data);




	

	
	path = pipeline->vert_path;

	dataSize = createShaderFromFile(path, &data);
	
	 
	if(dataSize == 0)
	{
		printf("Shader path: %s\n",path);
		EXIT_CLEAN("failed to read vertext shader file\n");
		
	}

	createInfo = (VkShaderModuleCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,
		
	};
	
	vkCreateShaderModule(device, &createInfo, NULL, &pipeline->shaderModuleVert);
	free(data);
	


	VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = pipeline->shaderModuleVert,
		.pName = "main",
		

	};

	VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = pipeline->shaderModuleFrag,
		.pName = "main",

	};

	uint32_t dynamicStateCount = 2;

	/*
	INFO
		set during command buffer recording part
		vkCmdSetViewport
	*/
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

	uint32_t shaderStageCreateInfCnt = sizeof(shaderStageCreateInf) /  sizeof(VkPipelineShaderStageCreateInfo);


	VkVertexInputBindingDescription vertexInputBindingDescription={
		.binding = 0,
		.stride = sizeof(struct Vertex),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};

  

	VkVertexInputAttributeDescription vertexInputAttributeDescriptions[1];

 

	vertexInputAttributeDescriptions[0] = (VkVertexInputAttributeDescription){0, 0, VK_FORMAT_R32G32B32_SFLOAT,  offsetof(struct Vertex, pos)};

 

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

 
	


	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pViewports = 0,
		.viewportCount = 1,
		.pScissors = 0,
		.scissorCount  =1,
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

 
	VkPipelineDepthStencilStateCreateInfo depthStencil={
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable =  VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS,
		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE,
	};



	

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo={
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 0,
		.pAttachments = NULL,
	};



	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &descriptorSetLayout4,
		.pushConstantRangeCount  = 0,
		

	};

	vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, &pipeline->pipelineLayout );


	VkFormat depthFormat = findDepthFormat();




	VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.colorAttachmentCount = 0,
		.pColorAttachmentFormats = NULL,
		.depthAttachmentFormat = depthFormat,
	};
	


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
		.layout = pipeline->pipelineLayout,
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
		&pipeline->graphicsPipeline
	);
}
void createHUDPipeline(struct  Pipeline * pipeline){

	PRINT_FNAME;



	

	

	char * path = pipeline->frag_path;
	uint8_t* data = NULL;
	uint32_t dataSize = createShaderFromFile(path, &data);
	
	


	
 
		

	if(dataSize == 0)
	{

		printf("Shader path: %s\n",path);
		EXIT_CLEAN("failed to read fragment shader file\n");
	
	}
	VkShaderModuleCreateInfo createInfo={
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,
	 
		
	};
 
	vkCreateShaderModule(device, &createInfo, NULL, &pipeline->shaderModuleFrag);

	free(data);




	

	
	path = pipeline->vert_path;

	dataSize = createShaderFromFile(path, &data);
	
	 
	if(dataSize == 0)
	{
		printf("Shader path: %s\n",path);
		EXIT_CLEAN("failed to read vertext shader file\n");
		
	}

	createInfo = (VkShaderModuleCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,
		
	};
	
	vkCreateShaderModule(device, &createInfo, NULL, &pipeline->shaderModuleVert);
	free(data);
	


	 
	VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = pipeline->shaderModuleVert,
		.pName = "main",
		

	};

	VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = pipeline->shaderModuleFrag,
		.pName = "main",

	};

	VkDynamicState dynamicState[]={
		VK_DYNAMIC_STATE_VIEWPORT, 
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = sizeof(dynamicState)/sizeof(VkDynamicState),
		.pDynamicStates = dynamicState,
		
	};

	
	VkPipelineShaderStageCreateInfo shaderStageCreateInf[] = {
		shaderStageCreateInfoVert,
		shaderStageCreateInfoFrag
	};

	uint32_t shaderStageCreateInfCnt = sizeof(shaderStageCreateInf) /  sizeof(VkPipelineShaderStageCreateInfo);


	VkVertexInputBindingDescription vertexInputBindingDescription={
		.binding = 0,
		.stride = sizeof(struct Vertex2D),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};

  

	VkVertexInputAttributeDescription vertexInputAttributeDescriptions[2];

	vertexInputAttributeDescriptions[0] = (VkVertexInputAttributeDescription){0, 0, VK_FORMAT_R32G32_SFLOAT,  offsetof(struct Vertex2D, pos)};
	vertexInputAttributeDescriptions[1] = (VkVertexInputAttributeDescription){1, 0, VK_FORMAT_R32G32_SFLOAT,  offsetof(struct Vertex2D, texCoords)};
 

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

 
	


	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pViewports = 0,
		.viewportCount = 1,
		.pScissors = 0,
		.scissorCount  =1,
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

 
	VkPipelineDepthStencilStateCreateInfo depthStencil={
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = VK_FALSE,
		.depthWriteEnable = VK_FALSE,

		.depthCompareOp = VK_COMPARE_OP_ALWAYS,

		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE,
	};


	

	

	

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState[]=
	{

		(VkPipelineColorBlendAttachmentState){
			.blendEnable = VK_TRUE, 

			.colorWriteMask  = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			.srcColorBlendFactor  = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
		},

	
	};


	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo={
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = sizeof(colorBlendAttachmentState) / sizeof(VkPipelineColorBlendAttachmentState),
		.pAttachments = colorBlendAttachmentState,
	};


	VkPushConstantRange pushRange = {};
	pushRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pushRange.offset = 0;
	pushRange.size = sizeof(struct PushConst2D);

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &descriptorSetLayout4,
		.pushConstantRangeCount  = 1,
		.pPushConstantRanges = &pushRange
	};

	vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, &pipeline->pipelineLayout );


	VkFormat depthFormat = findDepthFormat();



	

	VkFormat formats[] = {
		swapchainSurfaceFormat,   

		

	};

	VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.colorAttachmentCount = 1,
		
		.pColorAttachmentFormats = formats,
		.depthAttachmentFormat = depthFormat,
		

	};
	


 



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
		.layout = pipeline->pipelineLayout,
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
		&pipeline->graphicsPipeline
	);

}
void createPipelines(){

	

	struct Pipeline * worldPipeline  = 	gmArrayNew(&arrayPipelines);

	worldPipeline->frag_path = "shaders/out/main.frag.spv";
	worldPipeline->vert_path = "shaders/out/main.vert.spv";
	
	


	createGraphicsPipeline(worldPipeline);
	/*
		create shaders
		create descriptsors
		create pipeline
	
	*/

	 
	struct Pipeline * hudPipeline = gmArrayNew(&arrayPipelines);

	hudPipeline->frag_path = "shaders/out/hud.frag.spv";
	hudPipeline->vert_path = "shaders/out/hud.vert.spv";


	createHUDPipeline(hudPipeline);


	struct Pipeline * shadowPipeline = gmArrayNew(&arrayPipelines);

	shadowPipeline->frag_path = "shaders/out/shadow.frag.spv";
	shadowPipeline->vert_path = "shaders/out/shadow.vert.spv";

	createShadowPipeline(shadowPipeline);


	
}

void createGraphicsPipeline(struct  Pipeline * pipeline ) {

	PRINT_FNAME;

	

	char * path = pipeline->frag_path;
	uint8_t* data = NULL;
	uint32_t dataSize = createShaderFromFile(path, &data);
	
	


 
		

	if(dataSize == 0)
	{

		printf("Shader path: %s\n",path);
		EXIT_CLEAN("failed to read fragment shader file\n");
	
	}
	VkShaderModuleCreateInfo createInfo={
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,
	 
		
	};
 
	vkCreateShaderModule(device, &createInfo, NULL, &pipeline->shaderModuleFrag);

	free(data);




	

	
	path = pipeline->vert_path;

	dataSize = createShaderFromFile(path, &data);
	
	 
	if(dataSize == 0)
	{
		printf("Shader path: %s\n",path);
		EXIT_CLEAN("failed to read vertext shader file\n");
		
	}

	createInfo = (VkShaderModuleCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pCode = (uint32_t*)data,
		.codeSize = dataSize,
		
	};
	
	vkCreateShaderModule(device, &createInfo, NULL, &pipeline->shaderModuleVert);
	free(data);
	


	VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = pipeline->shaderModuleVert,
		.pName = "main",
		

	};

	VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = pipeline->shaderModuleFrag,
		.pName = "main",

	};

	uint32_t dynamicStateCount = 2;

	/*
	INFO
		set during command buffer recording part
		vkCmdSetViewport
	*/
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

	uint32_t shaderStageCreateInfCnt = sizeof(shaderStageCreateInf) /  sizeof(VkPipelineShaderStageCreateInfo);


	VkVertexInputBindingDescription vertexInputBindingDescription={
		.binding = 0,
		.stride = sizeof(struct Vertex),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};

  

	VkVertexInputAttributeDescription vertexInputAttributeDescriptions[3];

 

	vertexInputAttributeDescriptions[0] = (VkVertexInputAttributeDescription){0, 0, VK_FORMAT_R32G32B32_SFLOAT,  offsetof(struct Vertex, pos)};
	vertexInputAttributeDescriptions[1] = (VkVertexInputAttributeDescription){1, 0, VK_FORMAT_R32G32B32_SFLOAT,  offsetof(struct Vertex, norm)};
	vertexInputAttributeDescriptions[2] = (VkVertexInputAttributeDescription){2, 0, VK_FORMAT_R32G32_SFLOAT,  offsetof(struct Vertex, texCoords)};
 

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

 
	


	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pViewports = 0,
		.viewportCount = 1,
		.pScissors = 0,
		.scissorCount  =1,
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

 
	VkPipelineDepthStencilStateCreateInfo depthStencil={
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable =  VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS,
		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE,
	};


	

	

	

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState[]=
	{

		(VkPipelineColorBlendAttachmentState){
			.blendEnable = VK_FALSE, 

			.colorWriteMask  = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			.srcColorBlendFactor  = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
		},

		(VkPipelineColorBlendAttachmentState){
			.blendEnable = VK_FALSE,
			.colorWriteMask  = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			.srcColorBlendFactor  = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
		},
	};


	

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo={
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = sizeof(colorBlendAttachmentState)/ sizeof(VkPipelineColorBlendAttachmentState),
		.pAttachments = colorBlendAttachmentState,
	};


	VkPushConstantRange pushRange = {};
	pushRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pushRange.offset = 0;
	pushRange.size = sizeof(struct PushConst);

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &descriptorSetLayout4,
		.pushConstantRangeCount  = 1,
		.pPushConstantRanges = &pushRange
	};

	vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, &pipeline->pipelineLayout );


	VkFormat depthFormat = findDepthFormat();



	

	VkFormat formats[] = {
		swapchainSurfaceFormat,   

		VK_FORMAT_R32_UINT          

	};

	VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.colorAttachmentCount = sizeof(formats)/sizeof(VkFormat),
		.pColorAttachmentFormats = formats,
		.depthAttachmentFormat = depthFormat,
	};
	


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
		.layout = pipeline->pipelineLayout,
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
		&pipeline->graphicsPipeline
	);

}

void initObjects(){

	create_Object3D(global_light, struct DataMaterial *material, struct Mesh *mesh);

}

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

	createPickImage();

	createQueue();

	createDescriptorSetLayout22();

	createPipelines();

	


	createCommandPool();

	createSyncObjects3();


	createDepthImages();

	createShadowImages();

	
	loadModels2();
	
	
	
	createTextures2();
	

	createBuffers();

	createDescriptorPool2();


	allocateDescriptorSets();
	// createDescriptorSets33();
	

}
void mainLoop(){
	PRINT_FNAME;



	startTime = glfwGetTime();



	while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

		
		float currentTime =   glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		

		processInput(window);

		drawFrame3();
    }

	vkDeviceWaitIdle(device);
	
}

void cleanup(){
	//PRINT_FNAME;

	if(enableValidationLayers)
	{
		PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != NULL) {
			func(instance, debugUtilsMessengerEXT, NULL);
		}
	}
 
	

 
	vkDestroySwapchainKHR(device, swapchain,NULL);

	for(int i=0;i < swapchainImageViewCount;i++){
		
		vkDestroyImageView(device, swapchainImageViews[i],NULL);
	}
  
	
	
	for(int i=0;i < MAX_FRAMES_IN_FLIGHT;i++)
	{
		struct Frame * frame = &frames[i];

		vkDestroySemaphore(device,frame->presentCompleteSemaphore,NULL);
	
		vkDestroySemaphore(device,frame->renderFinishedSemaphore,NULL);

		vkDestroyFence(device,frame->inFlightFence,NULL);

	}

	cleanupPickImages();
		
	vkDestroyFence(device, transferFence, NULL);


	for(int i=0;i < MAX_FRAMES_IN_FLIGHT;i++)
	{
		struct Frame *frame = &frames[i];

		cleanImageRes(&frame->deapth);
		cleanImageRes(&frame->shadow.image);
		vkDestroySampler(device, frame->shadow.textureSampler,0);


	}

	cleanTextureRes(&tex_res0);

	for( int i=0;i<arrayTextures_3D.len ;i++)
	{
		struct TextureRes * t = gmArrayGet(&arrayTextures_3D, i);

		cleanTextureRes(t);
		
	}

	for( int i=0;i<arrayTextures_2D.len ;i++)
	{
		struct TextureRes * t = gmArrayGet(&arrayTextures_2D, i);

		cleanTextureRes(t);

	}

	for(int i=0;i<arrayPipelines.len;i++)
	{
		struct Pipeline * pipeline = gmArrayGet(&arrayPipelines, i);

		

		vkDestroyPipeline(device, pipeline->graphicsPipeline, NULL);

		vkDestroyPipelineLayout(device,pipeline->pipelineLayout, NULL);

		vkDestroyShaderModule(device, pipeline->shaderModuleFrag, NULL);
		
		vkDestroyShaderModule(device, pipeline->shaderModuleVert, NULL);
	}

	

	vkDestroyDescriptorSetLayout(device, descriptorSetLayout4, NULL);

	vkFreeDescriptorSets(device,descriptorPool, MAX_FRAMES_IN_FLIGHT,descriptorSets2);

	vkDestroyDescriptorPool(device, descriptorPool, NULL);

	for(int i=0;i<MAX_FRAMES_IN_FLIGHT;i++)
	{

		vkFreeCommandBuffers(device, graphicsCommnadPool, 1, &frames[i].graphicsCommandBuffers);
	}
	
	vkFreeCommandBuffers(device, transferCommnadPool, 1, &transferCommandBuffers);



	vkDestroyCommandPool(device, graphicsCommnadPool, NULL);

	vkDestroyCommandPool(device, transferCommnadPool, NULL);
	


	for(int i=0; i < MODEL_NUM; i++)
	{
		struct ModelVertexData2 * ref = &modelVertexData_3D[i];
		// freeModelVertexData(ref);
		cleanBuffer(&ref->vertexBuffer);
		cleanBuffer(&ref->indexBuffer);
		
	}

	cleanBuffer(&modelVertexData_HUD2.vertexBuffer);
	cleanBuffer(&modelVertexData_HUD2.indexBuffer);
	// freeModelVertexData(&modelVertexData_HUD);

	cleanShaderBuffers();



	vkDestroyDevice(device, NULL);

	


	vkDestroySurfaceKHR(instance, surface,NULL);

	vkDestroyInstance(instance,NULL);

	
	glfwDestroyWindow(window);

    glfwTerminate();



}
void initWindow(){

	printf("%s %d %d\n",__FUNCTION__, WIDTH, HEIGHT );





	glfwInit();
 
 

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

 	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);

	swapChainExtent.width = WIDTH;
	swapChainExtent.height = HEIGHT;

	prevX = WIDTH >> 1;
	prevY = HEIGHT >> 1;

	


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouseCallback);
	
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

int main(){

	PRINT_FNAME;

	initVariables();

	initWindow();

	initVulkan();

	initObjects();

	mainLoop();

	cleanup();

	freeVariables();

	return 0;
};