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

	exe.root_module.addCSourceFile(.{
        .file = b.path("src/assets/asset0.c"),.flags = &.{
                    "-std=c23",
                },
        
    });
    exe.root_module.addCSourceFile(.{
        .file = b.path("src/util/gm_array.c"),.flags = &.{
                    "-std=c23",
                },
    });
    exe.root_module.addCSourceFile(.{
        .file = b.path("src/util/gm_list.c"),.flags = &.{
                    "-std=c23",
                },
    });
    exe.root_module.addCSourceFile(.{
        .file = b.path("src/app.c"),.flags = &.{
                    "-std=c23",
                },
    });
    exe.root_module.addCSourceFile(.{
        .file = b.path("src/descriptor.c"),.flags = &.{
                    "-std=c23",
                },
    });


	exe.root_module.addCSourceFile(.{
		.file = b.path("src/device2.c"),.flags = &.{
            "-std=c23",
        },
	});
	
	exe.root_module.addCSourceFile(.{
		.file = b.path("src/input.c"),.flags = &.{
            "-std=c23",
        },
	});
	exe.root_module.addCSourceFile(.{
		.file = b.path("src/instance.c"),.flags = &.{
            "-std=c23",
        },
	});	
	exe.root_module.addCSourceFile(.{
		.file = b.path("src/loader/loader.c"),.flags = &.{
            "-std=c23",
        },
	});
	exe.root_module.addCSourceFile(.{
		.file = b.path("src/main2.c"),.flags = &.{
            "-std=c23",
        },
	});
	exe.root_module.addCSourceFile(.{
		.file = b.path("src/pipeline.c"),.flags = &.{
            "-std=c23",
        },
	});
	exe.root_module.addCSourceFile(.{
		.file = b.path("src/platform.c"),.flags = &.{
            "-std=c23",
        },
	});
    exe.root_module.addCSourceFile(.{
        .file = b.path("src/renderer.c"),.flags = &.{
                    "-std=c23",
                },
    });
    exe.root_module.addCSourceFile(.{
        .file = b.path("src/resource.c"),.flags = &.{
                    "-std=c23",
                },
    });
	exe.root_module.addCSourceFile(.{
		.file = b.path("src/scene.c"),.flags = &.{
            "-std=c23",
        },
	});
	exe.root_module.addCSourceFile(.{
		.file = b.path("src/shader.c"),.flags = &.{
            "-std=c23",
        },
	});
	exe.root_module.addCSourceFile(.{
		.file = b.path("src/swapchain.c"),
		.flags = &.{
            "-std=c23",
        },
	});
	
	
	
    // Include paths
    exe.root_module.addIncludePath(.{
        .cwd_relative = "C:/VulkanSDK/1.4.341.1/Include",
    });

    exe.root_module.addIncludePath(.{
        .cwd_relative = "c:/libs/glfw-ucrt/include",
    });

	exe.root_module.addIncludePath(.{
		.cwd_relative = ".",
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
