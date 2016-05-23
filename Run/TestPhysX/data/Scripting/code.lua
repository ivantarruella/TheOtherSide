

function reload_effects()
	CCore.GetSingletonPtr():get_effect_manager():reload()
end

function reload_scripts()
	CCore.GetSingletonPtr():get_script_manager():reload()
end
function reload_rotm()
	CCore.GetSingletonPtr():get_renderable_objects_technique_manager():reload()
end


