-- Sound functions

-- Global variables
helpme_ok = false
respiracion_ok = false
monstruo_ok = false
pisadas_monstruo = false
risa_ok = false
whatareudoinghere = false
grito_ok = false
scream_ok = false
laugh_ok = false

-- Global sources
source_intro = -1
source_ME = -1
source_MR = -1
source_laser = -1
source_vida = -1
source_energia = -1
source_chispas = -1
source_action_music = -1

function restart_sound()

	-- Reset ambient music
	if source_MR == -1 then
		-- nada
	else
		_SOUNDM:pause_source(source_MR)
		_SOUNDM:delete_source(source_MR)
	end
	
	if source_ME == -1 then
	-- nada
	else
		_SOUNDM:pause_source(source_ME)
		_SOUNDM:delete_source(source_ME)
	end
	
	-- Reset sound loops
	source_intro = -1
	source_ME = -1
	source_MR = -1
	source_laser = -1
	source_vida = -1
	source_energia = -1
	source_chispas = -1
	source_action_music = -1
	
	-- Reset one time actions
	helpme_ok = false
	respiracion_ok = false
	monstruo_ok = false
	pisadas_monstruo = false
	risa_ok = false
	whatareudoinghere = false
	grito_ok = false
	laugh_ok = false
	
end


-- Intro
function sound_intro()

	if source_intro == -1 then
		source_intro = _SOUNDM:create_source()
		_SOUNDM:play_source_2D(source_intro, 'intro', true)
	else
		_SOUNDM:pause_source(source_intro)
	end
	
end

function sound_game()
	--if _PROC:get_start_game() then
		if _GAMEPROCESS:is_in_real_world() then
			sound_MR()
		else
			sound_ME()
		end
	--end
end

-- Mundo espejo
function sound_ME()
	
	if source_MR == -1 then
		-- nada que hacer aqui
	else
		_SOUNDM:pause_source(source_MR)
	end
	
	if source_ME == -1 then
		source_ME = _SOUNDM:create_source()
		_SOUNDM:play_source_2D(source_ME, 'musica_ME', true)
	else
		_SOUNDM:pause_source(source_ME)
	end
	
end

-- Mundo real
function sound_MR()

	if source_ME == -1 then
		-- nada que hacer aqui
	else
		_SOUNDM:pause_source(source_ME)
	end
	
	if source_MR == -1 then
		source_MR = _SOUNDM:create_source()
		_SOUNDM:play_source_2D(source_MR, 'musica_MR', true)
	else
		_SOUNDM:pause_source(source_MR)
	end
	
end

-- Musica action
function sound_action_music_on()
	if source_MR == -1 then
		-- nada que hacer aqui
	else
		_SOUNDM:pause_source(source_MR)
		_SOUNDM:delete_source(source_MR)
		source_MR = -1
	end
	if source_ME == -1 then
		-- nada que hacer aqui
	else
		_SOUNDM:pause_source(source_ME)
		_SOUNDM:delete_source(source_MR)
		source_ME = -1
	end	
	if source_action_music == -1 then
		source_action_music = _SOUNDM:create_source()
		_SOUNDM:play_source_2D(source_action_music, 'action_music', true)
	end
end

function sound_action_music_off()
	if source_action_music == -1 then
		-- nada
	else
		_SOUNDM:delete_source(source_action_music)
		source_action_music = -1
	end
end

-- Efectos
function sound_shot()
	_SOUNDM:play_action_2D('disparo', 1.0)
end

function sound_laser_on()
	if source_laser == -1 then
		source_laser = _SOUNDM:create_source()
	end
	_SOUNDM:play_source_2D(source_laser, 'laser', true)
end

function sound_laser_off()
	if source_laser == -1 then
		-- nada
	else
		_SOUNDM:delete_source(source_laser)
		source_laser = -1
	end
end

function sound_bip()
	_SOUNDM:play_action_2D('bip', 1.0)
end

function sound_delete()
	_SOUNDM:play_action_2D('delete', 1.0)
end

function sound_teleport()
	_SOUNDM:play_action_2D('teleport', 1.0)
end

function sound_puerta()
	_SOUNDM:play_action_2D('puerta', 1.0)
end

function sound_ascensor()
	_SOUNDM:play_action_2D('ascensor', 1.0)
end


function sound_respiracion()
	if respiracion_ok == false then
		_SOUNDM:play_action_2D('respiracion', 1.0)
		respiracion_ok = true
	end
end

function sound_monstruo()
	if monstruo_ok == false then
		_SOUNDM:play_action_2D('monstruo', 1.0)
		monstruo_ok = true
	end
end

function sound_risa()
	if risa_ok == false then
		_SOUNDM:play_action_2D('helpus', 1.0)
		risa_ok = true
	end
end

function sound_andar_MR()
	_SOUNDM:play_action_2D('andar_MR', 1.0)
end

function sound_andar_ME()
	_SOUNDM:play_action_2D('andar_ME', 1.0)
end

function sound_helpme()
	if helpme_ok == false then
		_SOUNDM:play_action_2D('helpme', 1.0)
		helpme_ok = true
	end
end

function sound_carga_vida_on()
	if source_vida == -1 then
		source_vida = _SOUNDM:create_source()
		_SOUNDM:play_source_2D(source_vida, 'carga_vida_on', true)
	end
end

function sound_carga_vida_off()
	if source_vida == -1 then
		-- nada
	else
		_SOUNDM:delete_source(source_vida)
		source_vida = -1
	end
end

function sound_carga_energia_on()
	if source_energia == -1 then
		source_energia = _SOUNDM:create_source()
		_SOUNDM:play_source_2D(source_energia, 'carga_arma', true)
	end
end

function sound_carga_energia_off()
	if source_energia == -1 then
		-- nada
	else
		_SOUNDM:delete_source(source_energia)
		source_energia = -1
	end
end

function sound_pisadas_monstruo()
	if pisadas_monstruo == false then
		_SOUNDM:play_action_2D('pisadas_monstruo', 1.0)
		pisadas_monstruo = true
	end
end

function sound_player_hit()
	_SOUNDM:play_action_2D('blood', 0.5)
end

function sound_player_dying()
	_SOUNDM:play_action_2D('dying', 1.0)
end

function sound_chispas_on()
	if source_chispas == -1 then
		source_chispas = _SOUNDM:create_source()
		local pos = Vect3f(-15.1594,2.93715,0.501158)
		_SOUNDM:set_source_position(source_chispas, pos)
		_SOUNDM:play_source_3D(source_chispas, 'chispa', true)
	end
end

function sound_chispas_off()
	if source_chispas == -1 then
		-- nada
	else
		_SOUNDM:delete_source(source_chispas)
		source_chispas = -1
	end
end

function sound_what_are_you_doing_here()
	if whatareudoinghere == false then
		_SOUNDM:play_action_2D('whatareudoing', 1.0)
		whatareudoinghere = true
	end
end

function sound_grito()
	if grito_ok == false then
		_SOUNDM:play_action_2D('death-scream', 1.0)
		grito_ok = true
	end
end

function sound_scream()
	if scream_ok == false then
		_SOUNDM:play_action_2D('breathofdeath', 1.0)
		scream_ok = true
	end
end

function sound_laugh()
	if laugh_ok == false then
		_SOUNDM:play_action_2D('laugh', 1.0)
		laugh_ok = true
	end
end

function sound_monster_hit()
	_SOUNDM:play_action_2D('monster2', 1.0)
end

function sound_soldier_hit(_volume)
	_SOUNDM:play_action_2D('soldier_hit', _volume)
end

function sound_disparo_soldado(_volume)
	_SOUNDM:play_action_2D('disparo_enemigo', _volume)
end

function sound_soldier_die(_volume)
	_SOUNDM:play_action_2D('soldier_die', _volume)
end

function sound_soldier_die_headshot(_volume)
	_SOUNDM:play_action_2D('soldier_die_headshot', _volume)
end

function sound_soldier_hit(_volume)
	_SOUNDM:play_action_2D('blood', _volume)
end