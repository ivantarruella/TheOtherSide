-- Reload functions

-- KEY_F6
function reload_scripts()
	_SCRIPTM:reload()
end

function reload_languages()
	_LANGM:reload()
end

function reload_fonts()
	_FONTM:reload()
end

-- KEY_F7
function reload_actions()
	_INPUTM:reload()
end

-- KEY_F11
function reload_textures()
	--_TEXTUREM:reload()
	_PARTM:reload()
	_BILLBM:reload()	
end

-- KEY_F9
function reload_effects()
	_EFFECTM:reload()
end

function reload_rotm()
	_ROTM:reload()
end

function reload_static_meshes()
	_STATICMESHM:reload()
end

function reload_animated_models()
	_ANIMATEDMODELSM:reload()
end
	
function reload_rolm()
	_ROLM:reload()
end

-- KEY_F8
function reload_lights()
	_LIGHTM:reload()
	_GAMEPROCESS:reload_player_weapon()
end

-- KEY_F12
function reload_scene_renderer_commands()
	_SCENERENDERERCOMMANDM:reload()
end	

function reload_triggers()
	_TRIGGERM:reload()
end

function reload_logic_objects()
	_LOGICOBJECTSM:reload()
end
	
function reload_process()
	_PROC:reload()
end

function reload_player()
	_PLAYER:reload()
end

function reload_physX()
	_PHYSXM:release()
	_PHYSXM:init()
end

-- KEY_F5
function reload_ALL()			
	reload_scripts()
	reload_languages()
	reload_fonts()
	reload_actions()
	reload_textures()
	reload_effects()
	reload_rotm()
	reload_physX()
	reload_static_meshes()
	reload_animated_models()
	reload_rolm()
	reload_lights()
	reload_scene_renderer_commands()
	reload_scripts()
	reload_triggers()
	reload_logic_objects()
	reload_process()
end



		




