// flags
/*
	selected 0000_0001
	colored 0000_0010
	passes light 0000_0100
	textured  0000_1000

*/



struct InstanceHUD{
    vec2 position;
    vec2 size;
    vec4 color;
    uint material_id;
    uint object_id;

	uint flags;
};

struct InstanceMesh{
	mat4 model;
	vec4 color;
	uint object_id;
	uint material_id;
	uint flags;
	//add padding
};
struct Material{
	vec4 baseColorFactor;
    float metallicFactor;
    float roughnessFactor;
	//add padding
};


struct GlobalData {
	vec2 screenSize;
	float time;
	float deltaTime;
	//add paddng
};

struct CameraData {
    mat4 view;
    mat4 proj;
    vec4 position;
	//add padding
};


struct LightData {
	mat4 view;
	mat4 proj;
    vec4 direction;
    vec4 color;
    float intensity;
	//add padding
};
