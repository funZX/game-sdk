-- lua/lua.lua

premake.modules.lua = {}
local m = premake.modules.lua

local p = premake

newaction {
	trigger = "lua",
	description = "Export project information as Lua tables",

	onStart = function()
		print("Starting Lua generation")
	end,

	onWorkspace = function(wks)
		printf("Generating Lua for workspace '%s'", wks.name)
	end,

	onProject = function(prj)
		printf("Generating Lua for project '%s'", prj.name)
	end,

	execute = function()
		print("Executing Lua action")
	end,

	onEnd = function()
		print("Lua generation complete")
	end
}

return m