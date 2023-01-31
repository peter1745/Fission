include "./Vendor/premake-dwarf.lua"
include "Dependencies.lua"

workspace "Fission"
    architecture "x86_64"
    startproject "FissionTests"

    configurations { "Debug", "Release", "Distribution" }

    flags { "MultiProcessorCompile" }

    warnings "High"

    filter "action:vs*"
        linkoptions { "/ignore:4099" } -- Disable no PDB found warning

BuildDir = "%{wks.location}/Build/%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
IntermediatesDir = "%{wks.location}/Build/Intermediates/%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "ThirdParty"
include "ThirdParty/GLFW"
include "ThirdParty/ImGui"
--include "ThirdParty/tracy/tracy"
group ""

include "Fission"
include "Tests/TestFramework"
include "Tests"
