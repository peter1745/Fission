project "FissionTests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    targetdir (BuildDir .. "/%{prj.name}")
    objdir (IntermediatesDir .. "/%{prj.name}")

    files {
        "Source/**.cpp",
        "Source/**.hpp",
        "Source/**.h"
    }

    links {
        "TestFramework",
        "Fission",
        "GLFW",
        "ImGui",
        --"Tracy"
    }

    includedirs {
        "%{wks.location}/Tests/Source",
        "%{wks.location}/Tests/TestFramework/Source",
        
        "%{wks.location}/Fission/Source",
        
        "%{IncludeDirs.VulkanMemoryAllocator}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.ImGui}",
        "%{IncludeDirs.ImGui}/ImGui", -- Fix for ImGui includes
        --"%{IncludeDirs.Tracy}"
    }

    libdirs {
        "%{LibraryDir.VulkanMemoryAllocator}"
    }

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/EHsc", "/Zc:preprocessor" }

        externalincludedirs {
            (os.getenv("VULKAN_SDK") or "") .. "/Include"
        }

        libdirs {
            (os.getenv("VULKAN_SDK") or "") .. "/Lib/"
        }

        links {
            "vulkan-1.lib",
            "VulkanMemoryAllocator.lib",

            "Ws2_32.lib",
            "Dbghelp.lib"
        }

    filter { "system:windows", "configurations:Release" }
        links { "shaderc_combined.lib", "spirv-cross-core.lib", "spirv-cross-cpp.lib", "spirv-cross-glsl.lib" }

    filter { "system:windows", "configurations:Debug" }
        links { "shaderc_combinedd.lib", "spirv-cross-cored.lib", "spirv-cross-cppd.lib", "spirv-cross-glsld.lib" }

    filter "system:linux"
        pic "On"

        debugformat "Dwarf-4"

        libdirs {
            "%{LibraryDir.shaderc}",
            "%{LibraryDir.SPIRV_Cross}"
        }

        links {
            "vulkan",
            "shaderc_combined",
            "spirv-cross",
            "VulkanMemoryAllocator",
        }

        result, err = os.outputof("pkg-config --libs gtk+-3.0")
        linkoptions { result }

    filter "configurations:Debug"
        defines "FISSION_DEBUG"
        runtime "Debug"
        symbols "On"
        conformancemode "On"

    filter "configurations:Release"
        defines "FISSION_RELEASE"
        runtime "Release"
        optimize "On"
        conformancemode "On"

        --[[defines {
            "TRACY_ENABLE",
            "TRACY_ON_DEMAND"
        }]]--

    filter "configurations:Distribution"
        defines "FISSION_DIST"
        runtime "Release"
        optimize "Full"
        conformancemode "On"

        --[[defines {
            "TRACY_ENABLE",
            "TRACY_ON_DEMAND"
        }]]--
