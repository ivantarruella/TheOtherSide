-- Creación dinamica de enemigos

-- Variables trigger

enemies01_level1 = false

enemies_level2 = false
enemies01_level2 = false
enemies02_level2 = false
enemies03_level2 = false
enemies04_level2 = false
enemies05_level2 = false


-- Metodos restart

function level1_restart_enemies()
	enemies01_level1 = false
end

function level2_restart_enemies()
	enemies_level2 = false
	enemies01_level2 = false
	enemies02_level2 = false
	enemies03_level2 = false
	enemies04_level2 = false
	enemies05_level2 = false
end


function level1_enemigos()
	if enemies01_level1 == false then
		local position_MR = Vect3f(7.68321,0.00,4.29533)
		local position_ME = Vect3f(-7.68321,0.00,4.29533)
		_ENEMYM:create_enemy('Soldier001', position_MR, 'Monster001', position_ME)
		
		--local position_MR = Vect3f(19.7242,0.00, 28.6705)
		--local position_ME = Vect3f(-19.7242,0.00, 28.6705)
		--_ENEMYM:create_enemy('Soldier002', position_MR, 'Monster002', position_ME)
		
		enemies01_level1 = true
	end
end

function level2_enemigos_pasillos_entrada()
	if enemies01_level2 == false then
		--local position_MR = Vect3f(34.9448,0.00,-10.5629)
		--local position_ME = Vect3f(-34.9448,0.00,-10.5629)
		--_ENEMYM:create_enemy('Soldier001a', position_MR, 'Monster001a', position_ME)

		local position_MR = Vect3f(40.8783,0.00,-9.53521)
		local position_ME = Vect3f(-40.8783,0.00,-9.53521)
		_ENEMYM:create_enemy('Soldier001b', position_MR, 'Monster001b', position_ME)
		
		enemies01_level2 = true
	end
	
	level2_enemigos_hall_piso1()
end

function level2_enemigos_hall_piso1()
	if enemies02_level2 == false then
		local position_MR = Vect3f(39.2831,0.00,10.4767)
		local position_ME = Vect3f(-39.2831,0.00,10.4767)
		_ENEMYM:create_enemy('Soldier002', position_MR, 'Monster002', position_ME)
		
		position_MR = Vect3f(16.0906,0.00,-14.59991)
		position_ME = Vect3f(-16.0906,0.00,-14.59991)
		_ENEMYM:create_enemy('Soldier003', position_MR, 'Monster003', position_ME)
		
		position_MR = Vect3f(27.7563,0.00,-22.7989)
		position_ME = Vect3f(-27.7563,0.00,-22.7989)
		_ENEMYM:create_enemy('Soldier004', position_MR, 'Monster004', position_ME)

		position_MR = Vect3f(17.0178,0.00,5.25064)
		position_ME = Vect3f(-17.0178,0.00,5.25064)
		_ENEMYM:create_enemy('Soldier005', position_MR, 'Monster005', position_ME)
		
		enemies02_level2 = true
	end
end

function level2_enemigos_hall_piso2()
	if enemies03_level2 == false then
		local position_MR = Vect3f(19.9585,3.24342,-16.1943)
		local position_ME = Vect3f(-19.9585,3.24342,-16.1943)
		_ENEMYM:create_enemy('Soldier006', position_MR, 'Monster006', position_ME)

		position_MR = Vect3f(12.8495,3.24342,9.53032)
		position_ME = Vect3f(-12.8495,3.24342,9.53032)
		_ENEMYM:create_enemy('Soldier007', position_MR, 'Monster007', position_ME)
		
		position_MR = Vect3f(26.5414,3.24342,9.53032)
		position_ME = Vect3f(-26.5414,3.24342,9.53032)
		_ENEMYM:create_enemy('Soldier008', position_MR, 'Monster008', position_ME)

		position_MR = Vect3f(39.7759,3.24342,9.53032)
		position_ME = Vect3f(-39.7759,3.24342,9.53032)
		_ENEMYM:create_enemy('Soldier009', position_MR, 'Monster009', position_ME)
	
		position_MR = Vect3f(38.7261,3.24342,-3.78584)
		position_ME = Vect3f(-38.7261,3.24342,-3.78584)
		_ENEMYM:create_enemy('Soldier010', position_MR, 'Monster010', position_ME)
		
		position_MR = Vect3f(26.3129,3.24342,22.0939)
		position_ME = Vect3f(-26.3129,3.24342,22.0939)
		_ENEMYM:create_enemy('Soldier011', position_MR, 'Monster011', position_ME)
	
		enemies03_level2 = true
	end
end

function create_final_enemies()
	if enemies04_level2 == false then
		sound_action_music_on()
		
		local position_MR = Vect3f(15.4891,0.00,4.95024)
		local position_ME = Vect3f(-15.4891,0.00,4.95024)
		_ENEMYM:create_enemy('Soldier012', position_MR, 'Monster012', position_ME)

		position_MR = Vect3f(29.8728,0.00,4.95024)
		position_ME = Vect3f(-29.8728,0.00,4.95024)
		_ENEMYM:create_enemy('Soldier013', position_MR, 'Monster013', position_ME)

		position_MR = Vect3f(29.8728,0.00,-11.8063)
		position_ME = Vect3f(-29.8728,0.00,-11.8063)
		_ENEMYM:create_enemy('Soldier014', position_MR, 'Monster014', position_ME)

		position_MR = Vect3f(15.7048,0.00,-11.8063)
		position_ME = Vect3f(-15.7048,0.00,-11.8063)
		_ENEMYM:create_enemy('Soldier015', position_MR, 'Monster015', position_ME)
		
		position_MR = Vect3f(21.9331,0.00,-2.19154)
		position_ME = Vect3f(-21.9331,0.00,-2.19154)
		_ENEMYM:create_enemy('Soldier016', position_MR, 'Monster016', position_ME)

		position_MR = Vect3f(28.5671,0.00,-2.19154)
		position_ME = Vect3f(-28.5671,0.00,-2.19154)
		_ENEMYM:create_enemy('Soldier017', position_MR, 'Monster017', position_ME)
		
		objetivo10()
		_TRIGGERM:set_active('Trigger_Espejo_Montado_MR', true)
		_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_ME', true)
		
		enemies04_level2 = true
	end
end

function level2_enemigos_hall_piso2_boton()
	if enemies05_level2 == false then
		local position_MR = Vect3f(42.0426,3.24342,9.58284)
		local position_ME = Vect3f(-42.0426,3.24342,9.58284)
		_ENEMYM:create_enemy('Soldier006b', position_MR, 'Monster006b', position_ME)
		
		local position_MR = Vect3f(42.0426,3.24342,3.67617)
		local position_ME = Vect3f(-42.0426,3.24342,3.67617)
		_ENEMYM:create_enemy('Soldier007b', position_MR, 'Monster007b', position_ME)
		
		local position_MR = Vect3f(38.3931,3.24342,-5.07529)
		local position_ME = Vect3f(-38.3931,3.24342,-5.07529)
		_ENEMYM:create_enemy('Soldier008b', position_MR, 'Monster008b', position_ME)

		local position_MR = Vect3f(9.88905,3.24342,9.71019)
		local position_ME = Vect3f(-9.88905,3.24342,9.71019)
		_ENEMYM:create_enemy('Soldier009b', position_MR, 'Monster009b', position_ME)
		
		local position_MR = Vect3f(31.5837,3.24342,-18.3187)
		local position_ME = Vect3f(-31.5837,3.24342,-18.3187)
		_ENEMYM:create_enemy('Soldier010b', position_MR, 'Monster010b', position_ME)
		
		local position_MR = Vect3f(31.5837,3.24342,-23.9387)
		local position_ME = Vect3f(-31.5837,3.24342,-23.9387)
		_ENEMYM:create_enemy('Soldier011b', position_MR, 'Monster011b', position_ME)

		local position_MR = Vect3f(10.0418,3.24342,-13.5889)
		local position_ME = Vect3f(-10.0418,3.24342,-13.5889)
		_ENEMYM:create_enemy('Soldier012b', position_MR, 'Monster012b', position_ME)
		
		enemies05_level2 = true
	end
end

function level2_enemigos()
	if enemies_level2 == false then
		level2_enemigos_pasillos_entrada()
		level2_enemigos_hall_piso1()
		level2_enemigos_hall_piso2()
		
		enemies_level2 = true
	end
end

