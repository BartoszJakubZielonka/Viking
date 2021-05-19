include "./Vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Viking"
	architecture "x86_64"
	startproject "VikingEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Vendor/premake"
	include "Viking/Vendor/GLFW"
	include "Viking/Vendor/Glad"
	include "Viking/Vendor/imgui"
	include "Viking/Vendor/yaml-cpp"
group ""

include "Viking"
-- include "Sandbox"
include "VikingEditor"