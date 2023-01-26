project "Fission"
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

    includedirs { "Source/" }

    filter "action:vs*"
        pchheader "FissionPCH.hpp"
        pchsource "Source/FissionPCH.cpp"

    filter "action:not vs*"
        pchheader "FissionPCH.hpp"

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/EHsc", "/Zc:preprocessor" }

    filter "system:linux"
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

    filter "configurations:Distribution"
        defines "FISSION_DIST"
        runtime "Release"
        optimize "Full"
        conformancemode "On"
