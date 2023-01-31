project "TestFramework"
    kind "StaticLib"
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

    includedirs {
        "Source/",
        "%{IncludeDirs.shaderc}",
        "%{IncludeDirs.SPIRV_Cross}",
        "%{IncludeDirs.VulkanMemoryAllocator}",
        "%{IncludeDirs.GLFW}",
        --"%{IncludeDirs.Tracy}"
    }

    defines {
        "GLFW_INCLUDE_NONE"
    }

    filter "system:windows"
        systemversion "latest"

        externalincludedirs {
            (os.getenv("VULKAN_SDK") or "") .. "/Include"
        }

        buildoptions { "/EHsc", "/Zc:preprocessor" }

    filter "system:linux"
        pic "On"
        debugformat "Dwarf-4"

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
