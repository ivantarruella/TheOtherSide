-- Game functions

LEVEL_SETUP = {}
LEVEL_SETUP["Level"] = 'LoadLev01.xml'

function change_setup_level()
	local setup_level = LEVEL_SETUP["Level"]
	if setup_level == 'LoadLev01.xml' then
		LEVEL_SETUP["Level"] = 'LoadLev02.xml'
	else
		LEVEL_SETUP["Level"] = 'LoadLev01.xml'
	end	
end

function play_game(new_level)

	_GUI:set_visible_pointer_mouse(false)
	
	local current_level = _LEVELM:get_level()
	
	if current_level == "" and new_level == "" then
		_GUI:active_windows(LEVEL_SETUP["Level"])		-- por defecto carga nivel 01
	else 
		if new_level == "" or current_level == new_level then
			sound_game()
			_GUI:active_windows('Game.xml')
		else
			_GUI:active_windows(new_level)
		end
	end
	
end

function replay_game()

	_PROC:set_start_game(false)
	
	_LOGICOBJECTSM:restart_objects()	
	_ENEMYM:reset_all()
	restart_sound()
	_GAMEPROCESS:reload()
		
	_LOGICOBJECTSM:change_state_all(1, true)
	_EFFECTM:set_mirror_dark_coef(0.8)	

	local current_level = _LEVELM:get_level()
	if current_level == "Level01-Tutorial" then
		level1_restart_enemies()
		level1_enemigos()
		espejo_largo_MR_level01_ON()
	else 
		if current_level == "Level02 - Aqueronte Corp." then
			level2_restart_enemies()
			level2_enemigos_pasillos_entrada()
			objetivos_OFF()
			espejo_inicial_MR_level02_ON()
			_LOGICOBJECTSM:set_mirror_on('Trigger_SVigilado_Espejo_MR_Level02', false)
			_TRIGGERM:set_active('Trigger_Espejo_Roto_MR', true)
			buton_pressed = false
		end	
	end

	_GUI:pop_windows()
	_GUI:set_visible_pointer_mouse(false)
	_GUI:active_windows('Game.xml')	
	
end


-- Boton transformación cristal => espejo nivel 02

buton_pressed = false

function button_level02_trigger_OnEnter()
	if buton_pressed==false then
		_GUI:set_visible_gui_element('rotar_espejo',false)
		_GUI:set_visible_gui_element('use_item',true)
		--_GUI:push_windows('UseItem.xml')
		_GAMEPROCESS:set_can_use_item(true)
	end
end

function button_level02_trigger_OnTrigger()
	if buton_pressed==false then
		if _GAMEPROCESS:use_item() then
			-- Lanzamos sonido
			_SOUNDM:play_action_2D('switch',1.0)
			
			-- Apagamos luz de la sala y del boton
			_LIGHTM:set_light_active('Omni_SControl002', false)
			_LIGHTM:set_light_active('Omni_boto_pis2_MR', false)
			
			-- Quitamos glows de los fluorescentes
			_STATICMESHM:set_texture('SViglancias_Fluerescente14_MR','data/Meshes/D_BlancoVerdosoBaseAlfaBlanco.dds', 1)
			_STATICMESHM:set_texture('SViglancias_Fluerescente13_MR','data/Meshes/D_BlancoVerdosoBaseAlfaBlanco.dds', 1)
			
			-- Activamos espejo
			_LOGICOBJECTSM:set_mirror_on('Trigger_SVigilado_Espejo_MR_Level02', true)
			
			-- Inhabilitamos trigger
			_GUI:set_visible_gui_element('use_item',false)
			buton_pressed = true
			
			-- Creamso más enemigos
			level2_enemigos_hall_piso2_boton()
		end
	end
end

function button_level02_trigger_OnLeave()
	if buton_pressed==false then
		--_GUI:pop_windows()
		_GUI:set_visible_gui_element('use_item',false)
	end
	_GAMEPROCESS:set_can_use_item(false)
end

-- Entrada hall nivel 2 (mundo espejo)
function level02_enter_hall_ME()
	espejos_hall01_ME_level02_ON()
	--level2_enemigos_hall_piso1()
	opt_luces05_level2()
	objetivo9()
	sound_helpme()
end

function level02_exit_hall_ME()
	level2_enemigos_hall_piso1()
	opt_luces06_level2()
end

function level02_enter_hall_MR()
	opt_luces02_level2()
	espejos_hall01_MR_level02_ON()
end

function level02_hall_piso2_MR()
	level2_enemigos_hall_piso2()
	espejos_hall02_MR_level02_ON()
end

function level02_hall_piso2_ME()
	level2_enemigos_hall_piso2()
	espejos_hall02_ME_level02_ON()
end