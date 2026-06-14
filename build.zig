const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "main",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
            .root_source_file = null,
            
        }),
    });
    exe.root_module.addCSourceFiles(.{ .files = &.{
		"src/assets/asset0.c",
		"src/util/gm_array.c",
		"src/util/gm_list.c",
       "src/app.c",
       "src/descriptor.c",
       "src/device.c",
       "src/input.c",
       "src/instance.c",
       "src/loader/loader.c",
       "src/main2.c",
       "src/platform.c",
       "src/resource.c",
       "src/resource2.c",
       "src/shader.c",
        "src/mess.c",
        "src/pipelines/pipeline_main.c",
        "src/pipelines/pipeline_grid.c",
        "src/pipelines/pipeline_axis.c",
        "src/pipelines/pipeline_hud.c",
        "src/pipelines/pipeline.c",
     
    },
    .flags = &.{"-std=c23",}
    });
   
	
	
    // Include paths
    exe.root_module.addIncludePath(.{
        .cwd_relative = "C:/VulkanSDK/1.4.341.1/Include",
    });

    exe.root_module.addIncludePath(.{
        .cwd_relative = "c:/libs/glfw-ucrt/include",
    });

	exe.root_module.addIncludePath(.{
		.cwd_relative = "./external",
	});


    // exe.root_module.addIncludePath(.{
    //     .cwd_relative = "src",
    //  });

    // Library paths
    exe.root_module.addLibraryPath(.{ .cwd_relative = "C:/VulkanSDK/1.4.341.1/Lib" });
    exe.root_module.addLibraryPath(.{ .cwd_relative = "c:/libs/glfw-ucrt/lib" });

    // Link libraries
     exe.root_module.linkSystemLibrary("glfw3",.{});
     exe.root_module.linkSystemLibrary("vulkan-1",.{});

     exe.root_module.linkSystemLibrary("gdi32",.{});
     exe.root_module.linkSystemLibrary("user32",.{});
     exe.root_module.linkSystemLibrary("kernel32",.{});

    //exe.linkLibC();

    // const install = b.addInstallArtifact(exe, .{
    //     .dest_dir = .{ .override = .{ .custom = "out" } },
    // });

    // b.getInstallStep().dependOn(&install.step);

    b.installArtifact(exe);
}
