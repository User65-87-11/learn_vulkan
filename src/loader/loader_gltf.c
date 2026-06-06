#include <stdlib.h>
#include <vulkan/vulkan_core.h>
#include <cgltf/cgltf.h>
#include "loader_gltf.h"
#include "../util/common.h"

static void parse_nodes(struct Mess  * ref,cgltf_data* data);
static void parse_meshes(struct Mess  * ref,cgltf_data* data);

void Loader_LoadGLTF(char * path, struct Mess  * ref){
	
	PRINT_FNAME;
	const cgltf_options options = {};
	cgltf_data* data;
	cgltf_result res = cgltf_parse_file(&options, path, &data);
	
	if (res != cgltf_result_success) {
		printf("cgltf_result: %d\n", res);
		EXIT_CLEAN("cgltf_parse_file failed!");
	}
	

	parse_nodes(ref,data);

	parse_meshes(ref,data);
	
	
	cgltf_free(data);

}

static void parse_meshes(struct Mess  * ref, cgltf_data* data){
	
}
static void parse_nodes(struct Mess  * ref, cgltf_data* data){

	
	ref->cpu_data.nodes = calloc(data->nodes_count, sizeof(struct Scene_Node));
	ref->cpu_data.node_count = data->nodes_count;

    
   
    for(int i=0;i< data->nodes_count; i++)
    {
        // struct GLTFNode * node_dst = &asset->nodes[i];
        struct Scene_Node * node_dst  = &ref->cpu_data.nodes[i];
        struct cgltf_node * node_src = &data->nodes[i];

        node_dst->has_matrix = node_src->has_matrix;
        GLM_MAT4_COPY(node_dst->matrix,node_src->matrix);

        GLM_VEC3_COPY(node_dst->translation,node_src->translation);

        GLM_VEC4_COPY(node_dst->rotation,node_src->rotation);

        GLM_VEC3_COPY(node_dst->scale,node_src->scale);

        node_dst->child_cnt = node_src->children_count;
        if(node_dst->child_cnt > MAX_STATIC_CHILD_NODES)
        {
        	node_dst->dynamic_nodes = true;
         	node_dst->child_nodes_ids = calloc(node_src->children_count , sizeof(uint32_t));
        }else {
        	node_dst->child_nodes_ids = node_dst->_child_nodes_ids;
         	node_dst->dynamic_nodes = false;
        }

		for (cgltf_size j = 0; j < node_src->children_count; ++j) {
		
			struct cgltf_node* child = node_src->children[j];

			uint32_t child_index = (uint32_t)(child - data->nodes);

			node_dst->child_nodes_ids[j] = child_index;
		}
	}
}
