#pragma once
#include "../common.h"
#include "../mess.h"


void Asset0_init(struct Mess * ref);






struct Mesh* Assets_NewMesh(
	struct Mess* ref
);

struct Mesh* Assets_GetMesh(
	struct Mess* ref, 
	uint32_t position
);

struct MaterialData* Assets_NewMaterial(
	struct Mess* ref);

struct MaterialData* Assets_GetMaterial(
	struct Mess* ref, 
	uint32_t position
);

struct InstanceData* Assets_NewInstanceData(
	struct Mess* ref
);

struct InstanceData* Assets_GeInstanceData(
	struct Mess* ref, 
	uint32_t position
);

struct Image* Assets_NewTexture(
	struct Mess* ref, 
	void* data, 
	uint32_t size
);

struct Image* Assets_GetTexture(
	struct Mess* ref, 
	uint32_t position
);
