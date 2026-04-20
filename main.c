

#include "cglm/vec3.h"
#include "gm_array2.h"
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

// #include <time.h>


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
#include <unistd.h>   // for getcwd
#endif
#include <limits.h>   // for PATH_MAX


 


const uint32_t WIDTH = 800;

const uint32_t HEIGHT = 600;

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




// uint32_t validationLayerCount = 0;

// char** validationLayers ;

// uint32_t requiredDeviceExtensionCount = 0;

// char** requiredDeviceExtensions ;

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


VkDescriptorSetLayout descriptorSetLayout;


struct GameObject * gameObjectsA;
struct GameObject * gameObjectsB;
struct GameObject * gameObjectsC;

struct Pipeline{
	
	char * frag_path;

	char * vert_path;

	VkShaderModule shaderModuleFrag ;
	
	VkShaderModule shaderModuleVert ;
	
	VkPipelineLayout pipelineLayout ;
	
	VkPipeline graphicsPipeline ;

	uint32_t colorAttachmentCount;

};

// #define PIPELINE_CNT 3 

// struct Pipeline pipelines[PIPELINE_CNT];

struct GmArray arrayPipelines;

VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;

uint32_t frameIndex = 0;

VkCommandBuffer graphicsCommandBuffers[MAX_FRAMES_IN_FLIGHT];

VkCommandPool graphicsCommnadPool = NULL;

VkCommandBuffer transferCommandBuffers;

VkCommandPool transferCommnadPool = NULL;


VkFence transferFence;

struct BufferRes{
	// uint32_t binding;

	// uint32_t descriptorCount ;

	// VkShaderStageFlags stageFlag;

	// VkDescriptorType descriptorType;


	VkBuffer handle ;

	VkDeviceMemory memory ;

	void * mapped;

	VkDeviceSize size;
    
	VkBufferUsageFlags usage;

	// struct GmArray  * arrayData;

	// uint32_t range;

	 uint32_t width;
	
	// uint32_t length;
};
// struct GmArray arrayBuffers;

enum  ResoruceType{
    GM_RESOURCE_BUFFER = 1,
    GM_RESOURCE_TEXTURE
};


struct DescriptorResourceBinding {

	enum ResoruceType res_type;

    uint32_t binding;
    VkDescriptorType descriptorType;
    VkShaderStageFlags shaderStages;

	uint32_t count;
	
    union {
		struct BufferRes* buffer;
		struct TextureRes* texture;
    };
};
 
struct GmArray arrayDescriptorResourceBindings;
struct Frame{


 

	VkCommandBuffer * graphicsCommandBuffers;

	VkSemaphore presentCompleteSemaphore;

	VkSemaphore renderFinishedSemaphore;

	VkFence inFlightFence ;


	struct GmArray arrayBuffers;

	struct GmArray *arrayDescriptorResourceBindings;

 


	struct BufferRes* model_1;
	struct BufferRes* objectIds_1;
	struct BufferRes* viewProjection_1;
	struct BufferRes* directionLight_1;

	struct BufferRes* viewProjection_2;
	struct BufferRes* model_2;
	struct BufferRes* colors_2;

	// VkBuffer buffer_Model_1 ;

	// VkDeviceMemory bufferMemory_Model_1  ;

	// void * bufferMapped_Model_1  ;


	// VkBuffer buffer_Model_2  ;

	// VkDeviceMemory bufferMemory_Model_2 ;

	// void * bufferMapped_Model_2 ;



	// VkBuffer buffer_Colors_2  ;

	// VkDeviceMemory bufferMemory_Colors_2 ;

	// void * bufferMapped_Colors_2 ;


	// VkBuffer buffer_ObjectIds_1;

	// VkDeviceMemory bufferMemory_ObjectIds_1 ;

	// void * bufferMapped_ObjectIds_1 ;


	// VkBuffer buffers_ViewProjection_1 ;

	// VkDeviceMemory buffersMemory_ViewProjection_1 ;

	// void * buffersMapped_ViewProjection_1 ;


	// VkBuffer buffers_ViewProjection_2 ;

	// VkDeviceMemory buffersMemory_ViewProjection_2 ;

	// void * buffersMapped_ViewProjection_2 ;



	// VkBuffer buffers_DirectionalLight ;

	// VkDeviceMemory buffersMemory_DirectionalLight ;

	// void * buffersMapped_DirectionalLight ;


	VkImage depthImage ;

	VkDeviceMemory depthImageMemory ;

	VkImageView depthImageView ;





	VkDescriptorSet* descriptorSets;

	//-- TODO

	VkImage pickImage;

    VkImageView pickView;
    
	VkDeviceMemory pickMemory;

    VkBuffer pickStagingBuffer;
    
	VkDeviceMemory pickStagingMemory;

	void * pickMappedMem;



    uint32_t pickedID;

	//---
};

struct Frame frames[MAX_FRAMES_IN_FLIGHT] ={};

struct ModelVertexData
{
	char * gltfPath;

	VkBuffer vertextBuffer ;

	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;

	VkDeviceMemory indexBufferMemory;

	uint32_t indices_num;

};

#define MODEL_NUM 2


struct ModelVertexData modelVertexData[MODEL_NUM]={
	{.gltfPath = "models_gltf/viking_room.gltf"},
	{.gltfPath =  "models_gltf/box.gltf"},
};







/* END UNIFORM BUFFERS */

VkDescriptorPool descriptorPool = NULL;

VkDescriptorSet descriptorSets [MAX_FRAMES_IN_FLIGHT];




struct TextureRes{

	char * path;

	VkImage textureImage ;

	VkDeviceMemory textureImageMemory ;

	VkImageView textureImageView ;

	VkSampler textureSampler;

	uint32_t mipLevels;

	uint32_t textureIdx;
};

#define TEXTURE_COUNT_PIPE_1  3

struct TextureRes textures[]= {
	{
		.path = "models_gltf/viking_room.png"
	},
	{
		.path = "models_gltf/viking_room2.png"
	},
	{
		.path = "models_gltf/wooden_small.jpg"
	},
};

#define TEXTURE_COUNT_PIPE_2  1

struct TextureRes textures_2[]= {
	{
		.path = "models_gltf/wooden_small.jpg"
	},
};








// VkImageLayout currentSwapchainLayouts[MAX_IMAGE_VIEWS];

//--------------------------

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
//alignas(16)?



//END UP TO DAETH BUFFERING


char * model_path_gltf	= 	"models_gltf/viking_room.gltf";
char * model_path_bin	= 	"models_gltf/viking_room.bin";
char * model_path_text	= 	"models_gltf/viking_room.png";




#define INSTANCE_NUM 6

uint32_t instanceNum = INSTANCE_NUM;


//__attribute__((packed)) 
//__attribute__((aligned(16)))
// 
// struct __attribute__((aligned(16))) SBO_Model  {

// 	float model[16];
// 	uint32_t objectId;
// };


struct SSB_ObjectId{
	uint32_t objectId;
};
struct SBO_Model{
	mat4 model;
};
struct UBOCommon {

    mat4 view;
    mat4 proj;
};

uint32_t uboDirectionalLightCnt = 1;

struct UBODirectionalLight {
  vec4 lightPos; 
  vec4 viewPos; 
  vec4 lightColor;
} uniformBufferObjectDirectionalLight;

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


struct GameObjectInstance{
	vec3 position;
	vec3 scale;
	vec3 rotation;
	uint32_t uboModelIndex;

};

struct GameObject {


	// struct ModelVertexData * mesh;

	// struct TextureRes * tex;

	uint32_t vertexIdx;

	uint32_t textureIdx;

	struct GmArrayView arrayViewUboModel;
	
	struct GmArrayView arrayViewUboObjectIds;

	struct UBOPointLight * uboLight;

	
	struct GmArrayView arrayViewGameObjectInstances;


};


struct GmArray arrayGameObjectInstances;

struct GmArray arrayDescriptorPoolSizes;

struct GmArray arrayGameObjects;


struct GmArray arraySBO_Models_1;

struct GmArray arraySBO_Models_2;

struct GmArray arraySBO_ObjectIds_1;


struct GmArray arrayColors_2;


void initVariables();

void freeVariables();

struct BufferRes * iterateBuffer(
	uint32_t binding,
	uint32_t contextLen,
	uint32_t *context
);

//---
















//---


void loadModels();
void createDepthResources3(VkImage *depthImage,VkImageView *depthImageView,VkDeviceMemory *depthImageMemory);

void createDepthImages();

void createTextures();
void createPipelines();

// void initGameObjects3();

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

void recreateSwapChain();

void cleanup();

void createVertexBuffer(
	uint32_t verticesNum,
	struct Vertex * vertices,
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

// void createDescriptorSets();

void createTextureImage(
	char * path, 
	VkImage *textureImage,
	VkDeviceMemory *textureImageMemory,
	uint32_t *mipmap
);

void createPickImage(
	// VkImage *pickImage,
	// VkDeviceMemory *pickImageMemory,
	// VkImageView * pickIamgeView,
	// VkBuffer * stagingBuffer,
	// VkDeviceMemory *stagingBufferMemory,
	// void ** mappedMemory
);

void createTextureImageView();


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

void beginSingleTimeCommands(VkCommandBuffer commandBuffer);

void endSingleTimeCommands(VkCommandBuffer commandBuffer);

// void transitionImageLayout(VkImage *image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlagBits aspectFlags);

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

void createTextureSampler(VkSampler * sampler);

void createDepthResources();

VkFormat findSupportedFormat(VkFormat *formats, uint32_t len, VkImageTiling tiling, VkFormatFeatureFlags features);

VkFormat findDepthFormat();

bool hasStencilComponent(VkFormat format);

void processInput(GLFWwindow *window);
 
void createGraphicsPipeline(struct  Pipeline * pipeline );



float rand_float()
{
    return ((float)rand() / (float)RAND_MAX) * 20.0f - 10.0f;
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



	float sensitivity = 0.1f; // change this value to your liking
    dx *= sensitivity;
    dy *= sensitivity;

    yaw += dx;
    pitch += dy;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
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

	// printf("%f %f\n",xposIn,yposIn);


	float xpos =  xposIn;
    float ypos =  yposIn;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

	// printf("mouse: %f %f\n",lastX,lastY);

	// if(leftPressed == false) return;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
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

void createDepthImages(){

	
	for(int i=0; i< MAX_FRAMES_IN_FLIGHT; i++)
	{
		struct  Frame * frame = &frames[i];

		createDepthResources3(
			&(frame->depthImage),
			&(frame->depthImageView),
			&(frame->depthImageMemory)
		);
	}
	
}
void createTextures(){

	
	for( int i=0;i < TEXTURE_COUNT_PIPE_1 ;i++)
	{
		struct TextureRes * t = &textures[i];
		createTextureImage(
			t->path,
			&t->textureImage,
			&t->textureImageMemory,
			&t->mipLevels
		);
		t->textureIdx = i;

		 createImageView(
			&t->textureImageView,
			&t->textureImage,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
			t->mipLevels
		);

		createTextureSampler(&t->textureSampler);
	}

	for( int i=0;i < TEXTURE_COUNT_PIPE_2 ;i++)
	{
		struct TextureRes * t = &textures_2[i];
		createTextureImage(
			t->path,
			&t->textureImage,
			&t->textureImageMemory,
			&t->mipLevels
		);
		t->textureIdx = i;

		 createImageView(
			&t->textureImageView,
			&t->textureImage,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
			t->mipLevels
		);

		createTextureSampler(&t->textureSampler);
	}

}
void initVariables(){
	PRINT_FNAME;



	uint32_t total_objects = 30;


	gmArrayInit(&arrayPipelines, sizeof(struct Pipeline), 3,_Alignof(struct Pipeline));

	gmArrayInit(&arrayGameObjects, sizeof(struct GameObject), 3, _Alignof(struct GameObject));



	
	gmArrayInit(&arraySBO_Models_1, sizeof(struct SBO_Model), total_objects, _Alignof(struct SBO_Model));

	gmArrayInit(&arraySBO_Models_2, sizeof(struct SBO_Model), 1, _Alignof(struct SBO_Model));

	gmArrayInit(&arraySBO_ObjectIds_1, sizeof(struct SSB_ObjectId), total_objects, _Alignof(struct SSB_ObjectId));

	gmArrayInit(&arrayGameObjectInstances, sizeof(struct GameObjectInstance), total_objects, _Alignof(struct GameObjectInstance));

	
	gmArrayInit(&arrayDescriptorPoolSizes, sizeof(VkDescriptorPoolSize), 12, _Alignof(VkDescriptorPoolSize));

	gmArrayInit(&arrayColors_2, sizeof(vec4), 1, _Alignof(vec4));

	for(int i=0; i< MAX_FRAMES_IN_FLIGHT;i++){
		struct Frame * frame = &frames[i];
		gmArrayInit(&frame->arrayBuffers, sizeof(struct BufferRes), 10, _Alignof(struct GameObjectInstance));
	}
	gmArrayInit(&arrayDescriptorResourceBindings,sizeof(struct DescriptorResourceBinding), 12, _Alignof(struct DescriptorResourceBinding));
	


	// initGameObjects3();

	
	yaw = glm_deg(atan2(cameraFront[2], cameraFront[0]));
	pitch = glm_deg(asin(cameraFront[1]));

	
	vec4 v = {1.0,1.0,1.0,1.0};
	GLM_VEC4_COPY(uniformBufferObjectDirectionalLight.lightColor, v);
	GLM_VEC4_SET(uniformBufferObjectDirectionalLight.lightPos, 1.2f, 1.0f, 2.0f,0.0);
	GLM_VEC4_SET(uniformBufferObjectDirectionalLight.viewPos, 0.0f, 0.0f, 0.0f, 0.0);


	for(int i=0;i<1;i++)
	{
		vec4 color ={1.0,0.0,1.0,1.0};
		gmArrayPushValue(&arrayColors_2,color);
		struct SBO_Model m ;
		glm_mat4_identity(m.model) ;

		vec4 pos = {(float) swapChainExtent.width / 2, (float)swapChainExtent.height / 2, 0.0, 1.0};
		glm_translate(m.model, pos);

		gmArrayPushValue(&arraySBO_Models_2,&m);
	}
	


	
	for(int i=0; i < total_objects; i++)
	{

		struct SBO_Model m ;
		struct GameObjectInstance * g = gmArrayPush(&arrayGameObjectInstances);
		struct SSB_ObjectId o ;

		o.objectId = i +1;

		
		GLM_VEC3_SET(g->position, rand_float(), 0.0, rand_float());
		g->uboModelIndex = i;

		// GLM_VEC4_SET(m.objectId, i, 0.0, 0.0, 1.0);
		


		glm_mat4_identity(m.model) ;
		glm_translate(m.model, g->position);

	
		gmArrayPushValue(&arraySBO_ObjectIds_1,&o);

		gmArrayPushValue(&arraySBO_Models_1,&m);


		
	}
	


	uint32_t texture_index = 2;
	uint32_t vertexData_index = 1;

	//for(int i=0;i<gmArrayLength(gameObjectsCubes);i++)
	
	{
		gameObjectsA = gmArrayPush(&arrayGameObjects);
	
		gmArrayViewCreateSlice(&arraySBO_Models_1, &gameObjectsA->arrayViewUboModel ,0, 10);	
		gmArrayViewCreateSlice(&arrayGameObjectInstances, &gameObjectsA->arrayViewGameObjectInstances ,0, 10);	
		gmArrayViewCreateSlice(&arraySBO_ObjectIds_1, &gameObjectsA->arrayViewUboObjectIds ,0, 10);	


		gameObjectsA->textureIdx = texture_index;
		gameObjectsA->vertexIdx = vertexData_index;

	}

	texture_index = 1;
	vertexData_index = 0;
	{
		gameObjectsB = gmArrayPush(&arrayGameObjects);
	
		gmArrayViewCreateSlice(&arraySBO_Models_1, &gameObjectsB->arrayViewUboModel ,10, 10);	
		gmArrayViewCreateSlice(&arrayGameObjectInstances, &gameObjectsB->arrayViewGameObjectInstances ,10, 10);	
		gmArrayViewCreateSlice(&arraySBO_ObjectIds_1, &gameObjectsB->arrayViewUboObjectIds ,10, 10);	
		

		gameObjectsB->textureIdx = texture_index;
		gameObjectsB->vertexIdx = vertexData_index;

		// assert(gameObjectsTemp->mesh->vertextBuffer != NULL);
		
	}
	


	texture_index = 0;
	vertexData_index = 0;
	{
		gameObjectsC = gmArrayPush(&arrayGameObjects);
	
		gmArrayViewCreateSlice(&arraySBO_Models_1, &gameObjectsC->arrayViewUboModel ,20, 10);	
		gmArrayViewCreateSlice(&arrayGameObjectInstances, &gameObjectsC->arrayViewGameObjectInstances ,20, 10);	
		gmArrayViewCreateSlice(&arraySBO_ObjectIds_1, &gameObjectsC->arrayViewUboObjectIds ,20, 10);	
	
		gameObjectsC->textureIdx = texture_index;
		gameObjectsC->vertexIdx = vertexData_index;
		

	 
	}



	for(int i=0;i<MAX_FRAMES_IN_FLIGHT;i++){

		struct Frame * frame  =&frames[i];


		struct GmArray * arrayBuffers = &frame->arrayBuffers;

		struct GmArray * arrayDRB = &arrayDescriptorResourceBindings;

		frame->arrayDescriptorResourceBindings = arrayDRB;

		struct BufferRes * buffer = gmArrayPush(arrayBuffers);

		buffer->handle = buffer->mapped = buffer->memory =NULL;
		buffer->size = sizeof(struct SBO_Model) * arraySBO_Models_1.len;
		buffer->width = sizeof(struct SBO_Model);
		buffer->usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT ;
		frame->model_1 = buffer;
	
		if(i == 0){

			struct DescriptorResourceBinding * res = gmArrayPush(arrayDRB);
			res->binding = BINDING_VERT_1_SSBO_Models;
			res->res_type = GM_RESOURCE_BUFFER;
			res->descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			res->buffer = buffer;
			res->shaderStages = VK_SHADER_STAGE_VERTEX_BIT;
			res->count = 1;
		}
		
			//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
		
		

		buffer = gmArrayPush(arrayBuffers);
		buffer->handle = buffer->mapped = buffer->memory =NULL;
		buffer->size = sizeof(struct SBO_Model) * arraySBO_Models_2.len;
		buffer->width = sizeof(struct SBO_Model);
		buffer->usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT ;
		frame->model_2 = buffer;

		if(i == 0)
		{
			struct DescriptorResourceBinding * res = gmArrayPush(arrayDRB);
			res->binding = BINDING_VERT_2_SSBO_Models;
			res->res_type = GM_RESOURCE_BUFFER;
			res->descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			res->buffer = buffer;
			res->shaderStages = VK_SHADER_STAGE_VERTEX_BIT;
			res->count = 1;
		}



		//---
		buffer = gmArrayPush(arrayBuffers);
		buffer->handle = buffer->mapped = buffer->memory =NULL;
		buffer->size = sizeof(struct SSB_ObjectId) * arraySBO_ObjectIds_1.len;
		buffer->width = sizeof(struct SSB_ObjectId);
		buffer->usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT ;
		frame->objectIds_1= buffer;

		if(i == 0)
		{

			struct DescriptorResourceBinding * res = gmArrayPush(arrayDRB);
			res->binding = BINDING_VERT_1_SSBO_ObjectIDS;
			res->res_type = GM_RESOURCE_BUFFER;
			res->descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			res->buffer = buffer;
			res->shaderStages = VK_SHADER_STAGE_VERTEX_BIT;
			res->count = 1;
		}

		//-
		buffer = gmArrayPush(arrayBuffers);
		buffer->handle = buffer->mapped = buffer->memory =NULL;
		buffer->size = sizeof(vec4) * arrayColors_2.len;
		buffer->width = sizeof(vec4);
		buffer->usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT ;
		frame->colors_2 = buffer;

		if(i == 0)
		{

			struct DescriptorResourceBinding * res = gmArrayPush(arrayDRB);
			res->binding = BINDING_VERT_2_SSBO_Colors;
			res->res_type = GM_RESOURCE_BUFFER;
			res->descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			res->buffer = buffer;
			res->shaderStages = VK_SHADER_STAGE_VERTEX_BIT;
			res->count = 1;
		}


		//-
		
		buffer = gmArrayPush(arrayBuffers);
		buffer->handle = buffer->mapped = buffer->memory =NULL;
		buffer->size = sizeof(struct UBODirectionalLight) * 1;
		buffer->width = sizeof(struct UBODirectionalLight);
		buffer->usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ;
		frame->directionLight_1 = buffer;

		if(i == 0)
		{

			struct DescriptorResourceBinding * res = gmArrayPush(arrayDRB);
			res->binding = BINDING_FRAG_1_UBO_Lights;
			res->res_type = GM_RESOURCE_BUFFER;
			res->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			res->buffer = buffer;
			res->shaderStages = VK_SHADER_STAGE_FRAGMENT_BIT;
			res->count = 1;
		}


		//-
		buffer = gmArrayPush(arrayBuffers);
		buffer->handle = buffer->mapped = buffer->memory =NULL;
		buffer->size = sizeof(struct UBOCommon) * 1;
		buffer->width = sizeof(struct UBOCommon);
		buffer->usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ;
		frame->viewProjection_1 = buffer;


		if(i == 0)
		{

			struct DescriptorResourceBinding * res = gmArrayPush(arrayDRB);
			res->binding = BINDING_VERT_1_UBO_ViewProjection;
			res->res_type = GM_RESOURCE_BUFFER;
			res->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			res->buffer = buffer;
			res->shaderStages = VK_SHADER_STAGE_VERTEX_BIT;
			res->count = 1;
		}
		//-
		buffer = gmArrayPush(arrayBuffers);
		buffer->handle = buffer->mapped = buffer->memory =NULL;
		buffer->size = sizeof(struct UBOCommon) * 1;
		buffer->width = sizeof(struct UBOCommon);
		buffer->usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ;
		frame->viewProjection_2 = buffer;

		
		if(i == 0)
		{

			struct DescriptorResourceBinding * res = gmArrayPush(arrayDRB);
			res->binding = BINDING_VERT_2_UBO_ViewProjection;
			res->res_type = GM_RESOURCE_BUFFER;
			res->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			res->buffer = buffer;
			res->shaderStages = VK_SHADER_STAGE_FRAGMENT_BIT;
			res->count = 1;
		}

		//textures


		if(i == 0)
		{

			struct DescriptorResourceBinding * res = gmArrayPush(arrayDRB);
			res->binding = BINDING_FRAG_1_SAMPLER;
			res->res_type = GM_RESOURCE_TEXTURE;
			res->descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			res->texture = textures;
			res->shaderStages = VK_SHADER_STAGE_FRAGMENT_BIT;
			res->count = TEXTURE_COUNT_PIPE_1;
	
			res = gmArrayPush(arrayDRB);
			res->binding = BINDING_FRAG_2_SAMPLER;
			res->res_type = GM_RESOURCE_TEXTURE;
			res->descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			res->texture = textures_2;
			res->shaderStages = VK_SHADER_STAGE_FRAGMENT_BIT;
			res->count = TEXTURE_COUNT_PIPE_2;
		}


	}
	
	//pool sizes
	
	
	printf("arrayDescriptorResourceBindings\n" );
	for(int i=0;i<arrayDescriptorResourceBindings.len;i++)
	{
		struct DescriptorResourceBinding * bind = gmArrayGet(&arrayDescriptorResourceBindings, i);
		VkDescriptorPoolSize * poolSize  = gmArrayPush(&arrayDescriptorPoolSizes);
		poolSize->descriptorCount = bind->count * MAX_FRAMES_IN_FLIGHT;
		poolSize->type = bind->descriptorType;

		printf("\t %d %d %d\n",i, poolSize->descriptorCount,poolSize->type);
	}

	printf("arrayDescriptorPoolSizes.len = %d\n",arrayDescriptorPoolSizes.len);
	descriptorPoolCreateInfo.poolSizeCount = arrayDescriptorPoolSizes.len;
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolCreateInfo.maxSets = MAX_FRAMES_IN_FLIGHT;
	descriptorPoolCreateInfo.pPoolSizes = arrayDescriptorPoolSizes.data;
	

	
}
void loadModels(){
	for(int i=0;i<MODEL_NUM;i++)
	{

		struct ModelVertexData * ref = &modelVertexData[i];

		if(ref->gltfPath == NULL) continue;

		uint32_t verticesCnt;
		struct Vertex * vertices;
		

		uint32_t indicesCnt;
		uint32_t * indices;
	
		
		loadModel(
			ref->gltfPath,
			&indicesCnt,
			&indices,
			&verticesCnt,
			&vertices
		);

		ref->indices_num = indicesCnt;

		createVertexBuffer(
			verticesCnt,
			vertices,
			&ref->vertextBuffer,
			&ref->vertexBufferMemory
		);

		createIndexBuffer(
			indicesCnt,
			indices,
			&ref->indexBuffer,
			&ref->indexBufferMemory
		);

		free(vertices);
		free(indices);
	}
	
}
void freeVariables(){


	for(int i=0; i< MAX_FRAMES_IN_FLIGHT;i++){
		struct Frame * frame = &frames[i];
		gmArrayFree(&frame->arrayBuffers);
		
	}
	gmArrayFree(&arrayDescriptorResourceBindings);
	
	
	gmArrayFree(&arrayDescriptorPoolSizes);

	gmArrayFree(&arrayColors_2);

	gmArrayFree(&arrayPipelines);

	gmArrayFree(&arrayGameObjects);
	
	gmArrayFree(&arraySBO_Models_1);

	gmArrayFree(&arraySBO_Models_2);

	gmArrayFree(&arraySBO_ObjectIds_1);

	gmArrayFree(&arrayGameObjectInstances);

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

				
			 
				// vertices[d].col[0] = 1.0f;
				// vertices[d].col[1] = 1.0f;
				// vertices[d].col[2] = 0.5f;
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

			// verticesNum = accessor->count;

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

			// verticesNum = accessor->count;

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
	// depthImageView = 
	
	createImageView(depthImageView,depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,1);


	beginSingleTimeCommands(transferCommandBuffers);


	transitionImageLayout(
		transferCommandBuffers,
        depthImage,
        VK_IMAGE_LAYOUT_UNDEFINED,
        // VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL, FIX
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
void createTextureSampler(VkSampler * sampler){


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
 


		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy,
		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,

		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
	};

	vkCreateSampler(device, &samplerInfo, NULL, sampler);

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

	for(int i=0;i<TEXTURE_COUNT_PIPE_1;i++)
	{
	
		 createImageView(
			&textures[i].textureImageView,
			&textures[i].textureImage,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
			textures[i].mipLevels
		);
	}

	for(int i=0;i<TEXTURE_COUNT_PIPE_2;i++)
	{
	
		 createImageView(
			&textures_2[i].textureImageView,
			&textures_2[i].textureImage,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
			textures_2[i].mipLevels
		);
	}
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

	// PRINT_FNAME;
    // VkCommandBuffer commandBuffer;
	
	

	// TODO
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
			.commandBuffer = commandBuffer,//fix transferCommandBuffers
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

	//mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
	uint32_t max = w > h? w: h;
	uint32_t d = floor(log2(max));
	d++;
	return  d;

}
void createPickImage(
	// VkImage *pickImage,
	// VkDeviceMemory *pickImageMemory,
	// VkImageView * pickIamgeView,
	// VkBuffer * stagingBuffer,
	// VkDeviceMemory *stagingBufferMemory,
	// void ** mappedMemory
){
	PRINT_FNAME;

	
	for(int i=0;i<MAX_FRAMES_IN_FLIGHT;i++)
	{
		struct Frame *frame = &frames[i];
		

		createBuffer(
			sizeof(uint32_t), 
			VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&frame->pickStagingBuffer,
			&frame->pickStagingMemory
		);


		
		vkMapMemory(device, frame->pickStagingMemory, 0, VK_WHOLE_SIZE, 0, &frame->pickMappedMem);
		
		memset(frame->pickMappedMem, 0, sizeof(uint32_t));
		frame->pickedID = 0;

		createImage(
			swapChainExtent.width, 
			swapChainExtent.height, 
			1,
			VK_FORMAT_R32_UINT, 
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			&frame->pickImage, 
			&frame->pickMemory
		);

		createImageView(
			&frame->pickView, 
			&frame->pickImage,
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

	// stbi_set_flip_vertically_on_load(true);
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
		0,  // srcAccessMask: no prior access
		VK_ACCESS_2_TRANSFER_WRITE_BIT,  // dst: we will write via transfer
		VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_2_TRANSFER_BIT,  // dst stage: transfer op
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

void generateMipmaps(VkImage* image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {

    PRINT_FNAME;
    // VkCommandBuffer commandBuffer;
    beginSingleTimeCommands(transferCommandBuffers);

    // Check linear blitting support
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
	// performs the transition
    vkCmdPipelineBarrier2(transferCommandBuffers, &dependencyInfo);

    // Generate mip levels
    for (uint32_t i = 1; i < mipLevels; i++) {
        // Blit from mip i-1 to mip i
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

        // Transition mip i-1 to SHADER_READ_ONLY_OPTIMAL (no longer needed as source)
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT;
        barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
        barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;

        vkCmdPipelineBarrier2(transferCommandBuffers, &dependencyInfo);

        // Transition mip i to TRANSFER_SRC_OPTIMAL for next iteration (if not last)
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

    // Transition final mip level to SHADER_READ_ONLY_OPTIMAL
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


void createDescriptorSets3(){
	PRINT_FNAME;


	VkDescriptorSetLayout layouts [MAX_FRAMES_IN_FLIGHT]={
		//C99 designated initializer
		//  [0 ... MAX_FRAMES_IN_FLIGHT-1] = descriptorSetLayout
	};

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
   		layouts[i] = descriptorSetLayout;
	}



	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = descriptorPool,
		.descriptorSetCount = MAX_FRAMES_IN_FLIGHT,
		.pSetLayouts = layouts,
	};
	vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, descriptorSets);

	// assert(UNIFORM_BUFFER_COUNT == 2);

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		struct Frame * frame = &frames[i];

		frame->descriptorSets = &descriptorSets[i];

		
		for(int j=0;j<arrayDescriptorResourceBindings.len;j++){

			struct DescriptorResourceBinding * drb =  gmArrayGet(&arrayDescriptorResourceBindings,j);

			if(drb->res_type == GM_RESOURCE_BUFFER){
				struct BufferRes * res = drb->buffer;
				VkDescriptorBufferInfo info = {
					.buffer = res->handle,
					.offset = 0,
					.range = res->size
				};

				VkWriteDescriptorSet write = {
					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet = *frame->descriptorSets,
					.dstBinding = drb->binding,
					.dstArrayElement = 0,   // update one slot at a time
					.descriptorType = drb->descriptorType,
					.descriptorCount = 1,
					.pBufferInfo = &info
				};

				vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
			}
			else if(drb->res_type == GM_RESOURCE_TEXTURE){
				struct TextureRes * res = drb->texture;

				for (uint32_t k = 0; k < drb->count; k++) {
					VkDescriptorImageInfo info = {
						.imageView = res[k].textureImageView,
						.sampler = res[k].textureSampler,
						.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
					};

					VkWriteDescriptorSet write = {
						.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						.dstSet = *frame->descriptorSets,
						.dstBinding = drb->binding,
						.dstArrayElement = k,   // update one slot at a time
						.descriptorType = drb->descriptorType,
						.descriptorCount = 1,
						.pImageInfo = &info
					};

					vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
				}
			}
		}

		
	}

}

void clearUniformBuffers3(){
	
	PRINT_FNAME;

	for(int i=0; i < MAX_FRAMES_IN_FLIGHT; i++){

		struct GmArray * arrayBuffer = &frames[i].arrayBuffers;


		for(int j=0;j<arrayBuffer->len;j++)
		{
			struct BufferRes *b = gmArrayGet(arrayBuffer,j);
			
			 
			
			if(b->mapped != NULL)
			{ 
				vkUnmapMemory(device,b->memory);
				b->mapped = NULL;
			}
			if(b->handle != NULL){

				vkDestroyBuffer(device,b->handle, NULL);
				b->handle = NULL;
			}
			if(b->memory != NULL){

				vkFreeMemory(device,b->memory, NULL);
				b->memory= NULL;
			}
		}
	
	}
	

}
void initGameObjects3(){
	

	printf("sizeof(struct SBO_Model) = %d\n",sizeof(struct SBO_Model));
	// assert(sizeof(struct SBO_Model) ==  96);

 

	yaw = glm_deg(atan2(cameraFront[2], cameraFront[0]));
	pitch = glm_deg(asin(cameraFront[1]));

	
	vec4 v = {1.0,1.0,1.0,1.0};
	GLM_VEC4_COPY(uniformBufferObjectDirectionalLight.lightColor, v);
	GLM_VEC4_SET(uniformBufferObjectDirectionalLight.lightPos, 1.2f, 1.0f, 2.0f,0.0);
	GLM_VEC4_SET(uniformBufferObjectDirectionalLight.viewPos, 0.0f, 0.0f, 0.0f, 0.0);


	for(int i=0;i<1;i++)
	{
		vec4 color ={1.0,0.0,1.0,1.0};
		gmArrayPushValue(&arrayColors_2,color);
		struct SBO_Model m ;
		glm_mat4_identity(m.model) ;

		vec4 pos = {(float) swapChainExtent.width / 2, (float)swapChainExtent.height / 2, 0.0, 1.0};
		glm_translate(m.model, pos);

		gmArrayPushValue(&arraySBO_Models_2,&m);
	}
	


	
	for(int i=0; i < 30; i++)
	{

		struct SBO_Model m ;
		struct GameObjectInstance * g = gmArrayPush(&arrayGameObjectInstances);
		struct SSB_ObjectId o ;

		o.objectId = i +1;

		
		GLM_VEC3_SET(g->position, rand_float(), 0.0, rand_float());
		g->uboModelIndex = i;

		// GLM_VEC4_SET(m.objectId, i, 0.0, 0.0, 1.0);
		


		glm_mat4_identity(m.model) ;
		glm_translate(m.model, g->position);

	
		gmArrayPushValue(&arraySBO_ObjectIds_1,&o);

		gmArrayPushValue(&arraySBO_Models_1,&m);


		
	}
	


	uint32_t texture_index = 2;
	uint32_t vertexData_index = 1;


	
	{
		struct GameObject * gameObjectsTemp = gmArrayPush(&arrayGameObjects);
	
		gmArrayViewCreateSlice(&arraySBO_Models_1, &gameObjectsTemp->arrayViewUboModel ,0, 10);	
		gmArrayViewCreateSlice(&arrayGameObjectInstances, &gameObjectsTemp->arrayViewGameObjectInstances ,0, 10);	
		gmArrayViewCreateSlice(&arraySBO_ObjectIds_1, &gameObjectsTemp->arrayViewUboObjectIds ,0, 10);	

	
	}

	texture_index = 1;
	vertexData_index = 0;
	{
		struct GameObject * gameObjectsTemp = gmArrayPush(&arrayGameObjects);
	
		gmArrayViewCreateSlice(&arraySBO_Models_1, &gameObjectsTemp->arrayViewUboModel ,10, 10);	
		gmArrayViewCreateSlice(&arrayGameObjectInstances, &gameObjectsTemp->arrayViewGameObjectInstances ,10, 10);	
		gmArrayViewCreateSlice(&arraySBO_ObjectIds_1, &gameObjectsTemp->arrayViewUboObjectIds ,10, 10);	
	
		
	}
	


	texture_index = 0;
	vertexData_index = 0;
	{
		struct GameObject * gameObjectsTemp = gmArrayPush(&arrayGameObjects);
	
		gmArrayViewCreateSlice(&arraySBO_Models_1, &gameObjectsTemp->arrayViewUboModel ,20, 10);	
		gmArrayViewCreateSlice(&arrayGameObjectInstances, &gameObjectsTemp->arrayViewGameObjectInstances ,20, 10);	
		gmArrayViewCreateSlice(&arraySBO_ObjectIds_1, &gameObjectsTemp->arrayViewUboObjectIds ,20, 10);	
	
	}


}


void createUniformBuffers3(){



	clearUniformBuffers3();

	

	for(int i=0; i < MAX_FRAMES_IN_FLIGHT; i++){

		struct Frame * frame = &frames[i];


		

		for(int j=0;j<frame->arrayBuffers.len;j++){
			struct BufferRes *b = gmArrayGet(&frame->arrayBuffers, j);

			VkDeviceSize bufferSize = b->size;
			VkBuffer buffer;
			VkDeviceMemory bufferMemory;

			createBuffer(
				bufferSize, 
				b->usage, 
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
				&buffer, 
				&bufferMemory
			);
			b->handle= buffer;
			b->memory = bufferMemory;

			void * mapped_mem = NULL;
			

			vkMapMemory(device, bufferMemory, 0, bufferSize, 0, &mapped_mem);

			b->mapped = mapped_mem;

		}
	
	}
}


void createDescriptorPool() {

	// assert(UNIFORM_BUFFER_COUNT == 2);

	

	vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, NULL, &descriptorPool);
};


void createShaderDescriptorSetLayout(){


	VkDescriptorSetLayoutBinding descriptorSetLayoutBindings[arrayDescriptorResourceBindings.len];

	for(int i=0;i<arrayDescriptorResourceBindings.len;i++){
		struct DescriptorResourceBinding * drb = gmArrayGet(&arrayDescriptorResourceBindings, i);
		VkDescriptorSetLayoutBinding * lb  = &descriptorSetLayoutBindings[i];
		
		lb->binding = drb->binding;
		lb->descriptorType = drb->descriptorType;
		lb->descriptorCount = drb->count;
		lb->stageFlags = drb->shaderStages;
		lb->pImmutableSamplers = NULL;
		
		
	}


	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = sizeof(descriptorSetLayoutBindings) / sizeof(VkDescriptorSetLayoutBinding),
		.pBindings = descriptorSetLayoutBindings,
	};

	VkResult res= vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, NULL, &descriptorSetLayout);

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


	// VkCommandBuffer commandCopyBuffer ;


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
	// vkCmdCopyBuffer(transferCommandBuffers,  srcBuffer, dstBuffer, 1, &bufferCopy);

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
void createVertexBuffer(

	uint32_t verticesNum,
	struct Vertex * vertices,
	VkBuffer *vertexBuffer, 
	VkDeviceMemory *vertexBufferMemory

) {

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


	FILE *file = fopen(path, "rb");  // open file in read mode

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

    // read file
    fread(*buffer, 1, size, file);


	// free(buffer);
    fclose(file);  // close file

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
 
	VkCommandBufferAllocateInfo allocInfo = { 
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool =  graphicsCommnadPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = MAX_FRAMES_IN_FLIGHT  ,
	};

	vkAllocateCommandBuffers(device, &allocInfo, graphicsCommandBuffers);

	for(int i=0;i< MAX_FRAMES_IN_FLIGHT;i++){
		frames[i].graphicsCommandBuffers = &graphicsCommandBuffers[i];
	}

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
		.commandBufferCount = 1  ,
		
	};

	vkAllocateCommandBuffers(device, &allocInfo, &transferCommandBuffers);

	
}


void recordCommandBuffer3(uint32_t imageIndex,uint32_t frameIndex){

	struct Frame * frame = &frames[frameIndex];

	VkCommandBuffer commandBuffer = *frame->graphicsCommandBuffers;
	VkDescriptorSet * descriptorSet = frame->descriptorSets;

	VkImageView * pickImageView = &frame->pickView;

	VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0, 
        .pInheritanceInfo = NULL,
    };
    vkBeginCommandBuffer(commandBuffer, &beginInfo);


	VkImageMemoryBarrier2 beginBarrier[]={

		 (VkImageMemoryBarrier2){

			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
			.srcStageMask = VK_PIPELINE_STAGE_2_NONE,
			.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED ,  //UNDEFINES
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
			.image = frame->pickImage,
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
			.imageView = *pickImageView,
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
        .imageView = frame->depthImageView,
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

    vkCmdBeginRendering(commandBuffer, &renderingInfo);

	// for(int i=0;i<arrayPipelines.len;i++)
	
	struct Pipeline *  pipeline = gmArrayGet(&arrayPipelines, 0);

	if (!pipeline || pipeline->graphicsPipeline == VK_NULL_HANDLE) {
		printf("ERROR: pipeline or graphicsPipeline is invalid!\n");
		printf("  pipeline ptr: %p\n", pipeline);
		if (pipeline) {
			printf("  frag_path: %s\n", pipeline->frag_path);
			printf("  vert_path: %s\n", pipeline->vert_path);
			printf("  graphicsPipeline: %llu\n", (unsigned long long)pipeline->graphicsPipeline);
		}
		fflush(stdout);
		return; // or EXIT_CLEAN
	}

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicsPipeline);
    
    VkViewport viewPort = {
        .x = 0, 
		.y = 0, 
		.width = swapChainExtent.width, 
		.height = swapChainExtent.height, 
        .minDepth = 0.0f, 
		.maxDepth = 1.0f
    };

    vkCmdSetViewport(commandBuffer, 0, 1, &viewPort);
    
    VkRect2D scissor = { .extent = swapChainExtent, .offset = {0, 0} };
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);



	
	for(int i=0; i< arrayGameObjects.len;i++)
	{
		VkDeviceSize offset = 0;
		struct GameObject * gameObject = gmArrayGet(&arrayGameObjects, i);
		//draw Room0
		{
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &modelVertexData[gameObject->vertexIdx].vertextBuffer, &offset);
			vkCmdBindIndexBuffer(commandBuffer, modelVertexData[gameObject->vertexIdx].indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	
			vkCmdBindDescriptorSets(
				commandBuffer, 
				VK_PIPELINE_BIND_POINT_GRAPHICS, 
				pipeline->pipelineLayout, 0, 1, 
				descriptorSet, 
				0, 
				NULL
				//  dynamicOffset = cameraIndex * sizeof(Camera)

			);
	
			struct PushConst constants0 = {
				.hasColor = false,
				.texIdx = textures[gameObject->textureIdx].textureIdx,
				.objectId = frame->pickedID
			};
			// int textureIndex = 0; // choose texture
			vkCmdPushConstants(
				commandBuffer,
				 pipeline->pipelineLayout,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(struct PushConst),
				&constants0
			);
	
			vkCmdDrawIndexed(
				commandBuffer, 
				// gameObject->mesh->indices_num, 
				modelVertexData[gameObject->vertexIdx].indices_num,
				gameObject->arrayViewUboModel.len , 
				0, 
				0,
				gameObject->arrayViewUboModel.offset
			);
		}
	}
	



    vkCmdEndRendering(commandBuffer);


	{
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
			.image = frame->pickImage,
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
		vkCmdPipelineBarrier2(commandBuffer, &toTransferDep);
		
	
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
			commandBuffer,
			frame->pickImage,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			frame->pickStagingBuffer,
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
			.image = frame->pickImage,
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
		vkCmdPipelineBarrier2(commandBuffer, &toGeneralDep);
	}
	


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

void updateUniformBuffer3(uint32_t currentFrame){

	struct Frame * frame = &frames[currentFrame];


	memcpy(frame->model_1->mapped, arraySBO_Models_1.data, sizeof(struct SBO_Model)* arraySBO_Models_1.len);

	memcpy(frame->model_2->mapped, arraySBO_Models_2.data, sizeof(struct SBO_Model)* arraySBO_Models_2.len);

	memcpy(frame->objectIds_1->mapped, arraySBO_ObjectIds_1.data, sizeof(struct SSB_ObjectId)* arraySBO_ObjectIds_1.len);

	memcpy(frame->colors_2->mapped, arrayColors_2.data, sizeof(vec4)* arrayColors_2.len);

	// gmArrayCopyBySize(frame->bufferModelMapped, &arrayUboModels);

	
	struct UBOCommon ubo = {};

	vec3 cameraCenter;
	
	glm_vec3_add(cameraPos, cameraFront, cameraCenter);

	glm_lookat(cameraPos, cameraCenter, cameraUp, ubo.view);


	

	
	glm_perspective(glm_rad(45.0f), (float)swapChainExtent.width / swapChainExtent.height, 0.1f, 40.0f , ubo.proj);

	ubo.proj[1][1] *= -1;

	memcpy(frame->viewProjection_1->mapped, &ubo, sizeof(struct UBOCommon));


	memcpy(frame->viewProjection_2->mapped, &ubo, sizeof(struct UBOCommon));

	// gmArrayCopyBySize(frame->buffersViewProjectionMapped, ubo);

	GLM_VEC3_COPY(uniformBufferObjectDirectionalLight.viewPos,cameraPos);


	// vec4 lightPos ;
	uniformBufferObjectDirectionalLight.lightPos[0] = 5.0f*sin(lastTime);
	

	memcpy(frame->directionLight_1->mapped, &uniformBufferObjectDirectionalLight, sizeof(struct UBODirectionalLight));
	// ubo.model = glm_rotate(model, time*glm_rad(90.0f), rotation);


};
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


	uint32_t pickedObjectId = *(uint32_t*)frame->pickMappedMem;
	frame->pickedID = pickedObjectId;
	if(frame->pickedID != 0)
	{
		// printf("pickedObjectId %d\n",frame->pickedID);
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

	}else if(result != VK_SUCCESS && result!= VK_SUBOPTIMAL_KHR)
	{
		assert(result == VK_TIMEOUT || result == VK_NOT_READY);
		EXIT_CLEAN("failed to acquire swapchain image!");
	}
	
	
	updateUniformBuffer3(frameIndex);
	
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
			.deviceIndex = 0,// FIX 1
		},

		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &(VkCommandBufferSubmitInfo){
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.commandBuffer = *frame->graphicsCommandBuffers,
			
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
		// Note: Do NOT use VK_FENCE_CREATE_SIGNALED_BIT here. 
		// We want it to start unsignaled.
	};
	vkCreateFence(device, &fenceInfo, NULL, &transferFence);
		
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
	 
	for(int i=0;i < deviceExtensionPropertieCount; i++){

		// printf("\tphys device extension: %s\n",exp_props[i].extensionName);

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


	VkPhysicalDeviceVulkan12Features features12 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
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

	// uint32_t color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	
	
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
		// swapChainExtent = surfaceCapabilities.maxImageExtent;
	
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

	// clamp to minimum
	if (imageCount < surfaceCapabilities.minImageCount){

		imageCount = surfaceCapabilities.minImageCount;
	}

	// clamp to maximum (0 = no limit)
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

		// printf("img: %p\n",swapchainImages[i]);

		createImageView(&swapchainImageViews[i], &swapchainImages[i], swapchainSurfaceFormat, VK_IMAGE_ASPECT_COLOR_BIT,1);

		// currentSwapchainLayouts[i] = VK_IMAGE_LAYOUT_UNDEFINED;  
	}

	

 
}
void cleanupPickImages(){

	for(int i=0;i< MAX_FRAMES_IN_FLIGHT;i++)
	{
		struct Frame * frame = &frames[i];

		vkDestroyImage(device,frame->pickImage, NULL);
		vkDestroyImageView(device, frame->pickView, NULL);
		if(frame->pickMappedMem!= NULL){

			vkUnmapMemory(device, frame->pickStagingMemory);
			frame->pickMappedMem = NULL;
		}
		vkFreeMemory(device, frame->pickMemory, NULL);
		vkDestroyBuffer(device,frame->pickStagingBuffer, NULL);
		vkFreeMemory(device, frame->pickStagingMemory, NULL);
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

	
	// printf("glfwExtensionsExtra [%d]:\n",glfwExtensionCountExtra);
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
void createHUDPipeline(struct  Pipeline * pipeline){

	PRINT_FNAME;

	/*
	vkCmdDrawIndexed(
		VkCommandBuffer commandBuffer,
		uint32_t indexCount, 
		uint32_t instanceCount, 
		uint32_t firstIndex, 
		int32_t vertexOffset, 
		uint32_t firstInstance
	);

	{
		shaders
		layout(location = 0) in vec2 in_position;
		layout(location = 1) in vec2 in_texCoord;
	
	}

	"shaders/frag_hud.spv"
	"shaders/vert_hud.spv"


	mat4 ortho = ortho_matrix(
    	0.0f, (float)width,
		(float)height, 0.0f,   // flip Y if needed
		-1.0f, 1.0f
	);


	1. everything is a square
	2. instance count 
	3. what is instance data?
		3.1 texture idx
		3.2 model transform
		3.3 selected/highlighted
		3.4 grayed out

	*/



	

	// char * path ="shaders/frag.spv";
	char * path = pipeline->frag_path;
	uint8_t* data = NULL;
	uint32_t dataSize = createShaderFromFile(path, &data);
	
	//--- FRAGMENT

 
		

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




	//--- VERTEX
	
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
	//---------

	 
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

	// uint32_t dynamicStateCount = 2;

	/*
	INFO
		set during command buffer recording part
		vkCmdSetViewport
	*/
	VkDynamicState dynamicState[]={VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

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

 
	//viewport is dynamic

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


	//Color blending
	//needed for picking
	// const uint32_t colorBlendAttachmentStateCnt = 2;
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState[]=
	{

		(VkPipelineColorBlendAttachmentState){
			.blendEnable = VK_FALSE, // true of false?
			.colorWriteMask  = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			.srcColorBlendFactor  = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
		},

		// (VkPipelineColorBlendAttachmentState){
		// 	.blendEnable = VK_FALSE,
		// 	.colorWriteMask  = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		// 	.srcColorBlendFactor  = VK_BLEND_FACTOR_SRC_ALPHA,
		// 	.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		// 	.colorBlendOp = VK_BLEND_OP_ADD,
		// 	.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		// 	.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		// 	.alphaBlendOp = VK_BLEND_OP_ADD,
		// },
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
		.pSetLayouts = &descriptorSetLayout,
		.pushConstantRangeCount  = 1,
		.pPushConstantRanges = &pushRange
	};

	vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, &pipeline->pipelineLayout );


	VkFormat depthFormat = findDepthFormat();



	// const uint32_t colorAttachmentFormatsCount = 2;
	VkFormat formats[] = {
		swapchainSurfaceFormat,   // color
		// VK_FORMAT_R32_UINT          // picking (or UNORM if encoded)
	};

	VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.colorAttachmentCount = 1,
		
		.pColorAttachmentFormats = formats,
		// .depthAttachmentFormat = depthFormat,
		.depthAttachmentFormat = VK_FORMAT_UNDEFINED
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

	

	struct Pipeline * worldPipeline  = 	gmArrayPush(&arrayPipelines);

	worldPipeline->frag_path = "shaders/frag.spv";
	worldPipeline->vert_path = "shaders/vert.spv";
	worldPipeline->colorAttachmentCount = 2;
	


	createGraphicsPipeline(worldPipeline);
	/*
		create shaders
		create descriptsors
		create pipeline
	
	*/

	 
	struct Pipeline * hudPipeline = gmArrayPush(&arrayPipelines);

	hudPipeline->frag_path = "shaders/frag_hud.spv";
	hudPipeline->vert_path = "shaders/vert_hud.spv";


	createHUDPipeline(hudPipeline);

	/*
	
		createDescriptorPool();

		
		// for( int i=0;i<TEXTURE_COUNT ;i++)
		{
			// struct TextureRes * t = &textures[i];
			struct Pipeline * pipeline = gmArrayGet(&arrayPipelines, 0);
			createDescriptorSets3(pipeline);
		}
	
		check here
	*/
	
}

void createGraphicsPipeline(struct  Pipeline * pipeline ) {

	PRINT_FNAME;

	// char * path ="shaders/frag.spv";
	char * path = pipeline->frag_path;
	uint8_t* data = NULL;
	uint32_t dataSize = createShaderFromFile(path, &data);
	
	//--- FRAGMENT

 
		

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




	//--- VERTEX
	
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
	//---------

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

 
	//viewport is dynamic

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


	//Color blending
	//needed for picking
	// const uint32_t colorAttachmentCount = 2;
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState[]=
	{

		(VkPipelineColorBlendAttachmentState){
			.blendEnable = VK_FALSE, // true of false?
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
		.pSetLayouts = &descriptorSetLayout,
		.pushConstantRangeCount  = 1,
		.pPushConstantRanges = &pushRange
	};

	vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, &pipeline->pipelineLayout );


	VkFormat depthFormat = findDepthFormat();



	// const uint32_t colorAttachmentFormatsCount = 2;
	VkFormat formats[] = {
		swapchainSurfaceFormat,   // color
		VK_FORMAT_R32_UINT          // picking (or UNORM if encoded)
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
	
	//----

	createSwapchain();

	createImageViews();

	createPickImage();

	createQueue();

	createShaderDescriptorSetLayout();

	createPipelines();

	// createGraphicsPipeline();

	createCommandPool();

	createSyncObjects3();


	createDepthImages();

	createTextures();

	loadModels();
	

	// init MODELS

	// initGameObjects3();
	

	

	createUniformBuffers3();

	createDescriptorPool();


	createDescriptorSets3();
	

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

	for(int i=0;i < swapchainImageViewCount;i++){
		
		vkDestroyImageView(device, swapchainImageViews[i],NULL);
	}
  
	
	
	for(int i=0;i < MAX_FRAMES_IN_FLIGHT;i++)
	{
		struct Frame * frame = &frames[i];

		vkDestroySemaphore(device,frame->presentCompleteSemaphore,NULL);
	
		vkDestroySemaphore(device,frame->renderFinishedSemaphore,NULL);

		vkDestroyFence(device,frame->inFlightFence,NULL);


		

		// vkDestroySemaphore(device,presentCompleteSemaphore[i],NULL);
	
		// vkDestroySemaphore(device,renderFinishedSemaphore[i],NULL);

		// vkDestroyFence(device,inFlightFences[i],NULL);
	}

	cleanupPickImages();
		
	vkDestroyFence(device, transferFence, NULL);

	// for( int i=0;i<TEXTURE_COUNT ;i++)
	// {
	// 	struct TextureRes * t = &textures[i];
	// 	vkDestroyImageView(device, t->textureImageView,  NULL);
	// }

	for(int i=0;i < MAX_FRAMES_IN_FLIGHT;i++)
	{
		struct Frame *frame = &frames[i];

		vkDestroyImage(device, frame->depthImage, NULL);
		vkDestroyImageView(device,frame->depthImageView, NULL);
		vkFreeMemory(device, frame->depthImageMemory, NULL);


	}

	for( int i=0;i<TEXTURE_COUNT_PIPE_1 ;i++)
	{
		struct TextureRes * t = &textures[i];

		vkDestroyImageView(device, t->textureImageView,  NULL);
		vkDestroySampler(device, t->textureSampler, NULL);
		vkDestroyImage(device, t->textureImage, NULL);
		vkFreeMemory(device, t->textureImageMemory, NULL);
	}

	for( int i=0;i<TEXTURE_COUNT_PIPE_2 ;i++)
	{
		struct TextureRes * t = &textures_2[i];

		vkDestroyImageView(device, t->textureImageView,  NULL);
		vkDestroySampler(device, t->textureSampler, NULL);
		vkDestroyImage(device, t->textureImage, NULL);
		vkFreeMemory(device, t->textureImageMemory, NULL);
	}

	for(int i=0;i<arrayPipelines.len;i++)
	{
		struct Pipeline * pipeline = gmArrayGet(&arrayPipelines, i);

		

		vkDestroyPipeline(device, pipeline->graphicsPipeline, NULL);

		vkDestroyPipelineLayout(device,pipeline->pipelineLayout, NULL);

		vkDestroyShaderModule(device, pipeline->shaderModuleFrag, NULL);
		
		vkDestroyShaderModule(device, pipeline->shaderModuleVert, NULL);
	}

	

	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, NULL);

	vkFreeDescriptorSets(device,descriptorPool, MAX_FRAMES_IN_FLIGHT,descriptorSets);

	vkDestroyDescriptorPool(device, descriptorPool, NULL);


	vkFreeCommandBuffers(device, graphicsCommnadPool, MAX_FRAMES_IN_FLIGHT, graphicsCommandBuffers);
	
	vkFreeCommandBuffers(device, transferCommnadPool, 1, &transferCommandBuffers);



	vkDestroyCommandPool(device, graphicsCommnadPool, NULL);

	vkDestroyCommandPool(device, transferCommnadPool, NULL);
	


	for(int i=0; i < MODEL_NUM; i++)
	{
		struct ModelVertexData * ref = &modelVertexData[i];

		if(ref->vertextBuffer != NULL){

			vkDestroyBuffer(device,ref->vertextBuffer,NULL);
			ref->vertextBuffer = NULL;
		}
 
		if(ref->vertexBufferMemory != NULL){

			vkFreeMemory(device, ref->vertexBufferMemory, NULL);
			ref->vertexBufferMemory = NULL;
		}

		if(ref->indexBuffer != NULL){

			vkDestroyBuffer(device,ref->indexBuffer,NULL);
			ref->indexBuffer = NULL;
		}

		if(ref->indexBufferMemory != NULL){
			
			vkFreeMemory(device, ref->indexBufferMemory, NULL);
			ref->indexBufferMemory = NULL;
		}
	}



	// for( int i=0;i<TEXTURE_COUNT ;i++)
	// {
	// 	struct TextureRes * t = &textures[i];

	// 	vkDestroyImage(device, t->textureImage, NULL);

	// 	vkFreeMemory(device, t->textureImageMemory, NULL);
	// }
	clearUniformBuffers3();


	vkDestroyDevice(device, NULL);

	// INSTANCE

	vkDestroySurfaceKHR(instance, surface,NULL);

	vkDestroyInstance(instance,NULL);

	
	glfwDestroyWindow(window);

    glfwTerminate();


	// free(vertices);
	// free(indices);

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
	mainLoop();
	cleanup();

	freeVariables();

	return 0;
};