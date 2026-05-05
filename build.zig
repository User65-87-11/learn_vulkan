const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "main",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .root_source_file = null,
        }),
    });

    exe.root_module.addCSourceFile(.{
        .file = b.path("main.c"),
    });
    exe.root_module.addCSourceFile(.{
        .file = b.path("util/gm_array.c"),
    });
    exe.root_module.addCSourceFile(.{
        .file = b.path("util/gm_list.c"),
    });

    // Include paths
    exe.root_module.addIncludePath(.{
        .cwd_relative = "C:/VulkanSDK/1.4.341.1/Include",
    });

    exe.root_module.addIncludePath(.{
        .cwd_relative = "c:/libs/glfw-ucrt/include",
    });

  //  exe.root_module.addIncludePath(.{
   //     .cwd_relative = ".",
  //  });

    // Library paths
    exe.root_module.addLibraryPath(.{ .cwd_relative = "C:/VulkanSDK/1.4.341.1/Lib" });
    exe.root_module.addLibraryPath(.{ .cwd_relative = "c:/libs/glfw-ucrt/lib" });

    // Link libraries
    exe.linkSystemLibrary("glfw3");
    exe.linkSystemLibrary("vulkan-1");

    exe.linkSystemLibrary("gdi32");
    exe.linkSystemLibrary("user32");
    exe.linkSystemLibrary("kernel32");

    exe.linkLibC();

    // const install = b.addInstallArtifact(exe, .{
    //     .dest_dir = .{ .override = .{ .custom = "out" } },
    // });

    // b.getInstallStep().dependOn(&install.step);

    b.installArtifact(exe);
}
