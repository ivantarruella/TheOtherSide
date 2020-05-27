-- Creación dinamica de enemigos

-- Variables trigger

level1_enemies01_spawned = false

level2_floor1_enemy00_spawned = false
level2_floor1_enemy000_spawned = false
level2_floor1_enemy01_spawned = false
level2_floor1_enemy02_spawned = false
level2_floor1_enemy03_spawned = false
level2_floor1_enemy04_spawned = false
level2_floor2_enemies01_spawned = false
level2_floor2_enemies02_spawned = false
level2_floor2_enemies03_spawned = false

level2_final_enemies_spawned = false

-- Metodos restart

function level1_restart_enemies()
	level1_enemies01_spawned = false
end

function level2_restart_enemies()

	level2_floor1_enemy00_spawned = false
	level2_floor1_enemy000_spawned = false
	level2_floor1_enemy01_spawned = false
	level2_floor1_enemy02_spawned = false
	level2_floor1_enemy03_spawned = false
	level2_floor1_enemy04_spawned = false
	
	level2_floor2_enemies01_spawned = false	
	level2_floor2_enemies02_spawned = false
	level2_floor2_enemies03_spawned = false
	
	level2_final_enemies_spawned = false
end

-- LEVEL 01

function level1_activar_enemigo()
	if level1_enemies01_spawned == false then
		local position_MR = Vect3f(6.38321,0.00,8.29533)
		local position_ME = Vect3f(-6.38321,0.00,8.29533)
		_ENEMYM:create_enemy('Soldier001', position_MR, 'Monster001', position_ME)
		
		level1_enemies01_spawned = true
	end
end

-- LEVEL 02
function level2_enemigo0_hall_piso1()
	if level2_floor1_enemy00_spawned == false then
		--enemic 0, entrada hall, pis1
		local position_MR = Vect3f(34.0863, 0.00, -6.51555)
		local position_ME = Vect3f(-34.0863, 0.00, -6.51555)
		_ENEMYM:create_enemy('Soldier001a', position_MR, 'Monster001a', position_ME)
		
		level2_floor1_enemy00_spawned = true
	end
end
function level2_enemigo00_hall_piso1()
	if level2_floor1_enemy000_spawned == false then
		--enemic 0, entrada hall, pis1
		local position_MR = Vect3f(46.988, 0.0, -13.539)
		local position_ME = Vect3f(-46.988, 0.0, -13.539)
		_ENEMYM:create_enemy('Soldier001b', position_MR, 'Monster001b', position_ME)
		
		level2_floor1_enemy000_spawned = true
	end
end
function level2_enemigo1_hall_piso1()
	if level2_floor1_enemy01_spawned == false then
		--enemic 1, entrada hall, pis1
		local position_MR = Vect3f(39.16,0.00,7.15)
		local position_ME = Vect3f(-39.16,0.00,7.15)
		_ENEMYM:create_enemy('Soldier001', position_MR, 'Monster001', position_ME)
		
		position_MR = Vect3f(34.0863, 0.00, -6.51555)
		position_ME = Vect3f(-34.0863, 0.00, -6.51555)
		_ENEMYM:create_enemy('Soldier001a', position_MR, 'Monster001a', position_ME)
		
		level2_floor1_enemy01_spawned = true
	end
end
function level2_enemigo2_hall_piso1()
	if level2_floor1_enemy02_spawned == false then
		--enemic 2, porta ascensor mon mirall, pis1
		position_MR = Vect3f(29.41,0.00,-19.18)
		position_ME = Vect3f(-29.41,0.00,-19.18)
		_ENEMYM:create_enemy('Soldier002', position_MR, 'Monster002', position_ME)
		level2_floor1_enemy02_spawned = true
	end
end
function level2_enemigo3_hall_piso1()
	if level2_floor1_enemy03_spawned == false then
		--enemic 3, mirall hall, pis 1 
		position_MR = Vect3f(16.32,0.00,-16.30)
		position_ME = Vect3f(-16.32,0.00,-16.30)
		_ENEMYM:create_enemy('Soldier003', position_MR, 'Monster003', position_ME)
		level2_floor1_enemy03_spawned = true
	end
end
function level2_enemigo4_hall_piso1()
	if level2_floor1_enemy04_spawned == false then
		--enemic 4, finestra hall, pis 1
		position_MR = Vect3f(17.55,0.00,-16.34)
		position_ME = Vect3f(-17.55,0.00,-16.34)
		_ENEMYM:create_enemy('Soldier004', position_MR, 'Monster004', position_ME)
		level2_floor1_enemy04_spawned = true
	end
end

function level2_enemigos01_hall_piso2()
	if level2_floor2_enemies01_spawned == false then
		local position_MR = Vect3f(10.82,3.24342,-13.59)
		local position_ME = Vect3f(-10.82,3.24342,-13.59)
		_ENEMYM:create_enemy('Soldier005', position_MR, 'Monster005', position_ME)

		position_MR = Vect3f(12.8495,3.24342,9.53032)
		position_ME = Vect3f(-12.8495,3.24342,9.53032)
		_ENEMYM:create_enemy('Soldier006', position_MR, 'Monster006', position_ME)
		
		position_MR = Vect3f(26.5414,3.24342,9.53032)
		position_ME = Vect3f(-26.5414,3.24342,9.53032)
		_ENEMYM:create_enemy('Soldier007', position_MR, 'Monster007', position_ME)

		position_MR = Vect3f(39.7759,3.24342,9.53032)
		position_ME = Vect3f(-39.7759,3.24342,9.53032)
		_ENEMYM:create_enemy('Soldier008', position_MR, 'Monster008', position_ME)
	
		position_MR = Vect3f(38.7261,3.24342,-3.78584)
		position_ME = Vect3f(-38.7261,3.24342,-3.78584)
		_ENEMYM:create_enemy('Soldier009', position_MR, 'Monster009', position_ME)
		
		level2_floor2_enemies01_spawned = true
	end
end

function level2_enemigos_hall_piso2_boton()
	if level2_floor2_enemies02_spawned == false then
		position_MR = Vect3f(33.08,3.24342,15.44)
		position_ME = Vect3f(-33.08,3.24342,15.44)
		_ENEMYM:create_enemy('Soldier010', position_MR, 'Monster010', position_ME)
		
		local position_MR = Vect3f(28.23,3.24342,15.45)
		local position_ME = Vect3f(-28.23,3.24342,15.45)
		_ENEMYM:create_enemy('Soldier011', position_MR, 'Monster011', position_ME)
		
		--local position_MR = Vect3f(42.0426,3.24342,3.67617)
		--local position_ME = Vect3f(-42.0426,3.24342,3.67617)
		--_ENEMYM:create_enemy('Soldier012', position_MR, 'Monster012', position_ME)
		
		local position_MR = Vect3f(38.3931,3.24342,-5.07529)
		local position_ME = Vect3f(-38.3931,3.24342,-5.07529)
		_ENEMYM:create_enemy('Soldier013', position_MR, 'Monster013', position_ME)

		local position_MR = Vect3f(9.88905,3.24342,9.71019)
		local position_ME = Vect3f(-9.88905,3.24342,9.71019)
		_ENEMYM:create_enemy('Soldier014', position_MR, 'Monster014', position_ME)
		
		local position_MR = Vect3f(31.5837,3.24342,-18.3187)
		local position_ME = Vect3f(-31.5837,3.24342,-18.3187)
		_ENEMYM:create_enemy('Soldier015', position_MR, 'Monster015', position_ME)
		
		local position_MR = Vect3f(31.5837,3.24342,-23.9387)
		local position_ME = Vect3f(-31.5837,3.24342,-23.9387)
		_ENEMYM:create_enemy('Soldier016', position_MR, 'Monster016', position_ME)

		local position_MR = Vect3f(10.0418,3.24342,-13.5889)
		local position_ME = Vect3f(-10.0418,3.24342,-13.5889)
		_ENEMYM:create_enemy('Soldier017', position_MR, 'Monster017', position_ME)
	
		level2_floor2_enemies02_spawned = true
	end
end

function level2_enemigos02_hall_piso2_ME()
	if level2_floor2_enemies03_spawned == false then
		-- al cambiar de mundo en piso 2 tras activar espejo y coger el fragmento de encima de la camilla
		local position_MR = Vect3f(17.20,3.24342,25.66)
		local position_ME = Vect3f(-17.20,3.24342,25.66)
		_ENEMYM:create_enemy('Soldier018', position_MR, 'Monster018', position_ME)
		
		local position_MR = Vect3f(11.84,3.24342,15.15)
		local position_ME = Vect3f(-11.84,3.24342,15.15)
		_ENEMYM:create_enemy('Soldier019', position_MR, 'Monster019', position_ME)
		
		local position_MR = Vect3f(31.22,3.24342,-21.23)
		local position_ME = Vect3f(-31.22,3.24342,-21.23)
		_ENEMYM:create_enemy('Soldier020', position_MR, 'Monster020', position_ME)

		local position_MR = Vect3f(15.48,3.24342,-15.08)
		local position_ME = Vect3f(-15.48,3.24342,-15.08)
		_ENEMYM:create_enemy('Soldier021', position_MR, 'Monster021', position_ME)
	
		level2_floor2_enemies03_spawned = true
	end
end

function create_final_enemies()
	if level2_final_enemies_spawned == false then
		objetivo10()
		_TRIGGERM:set_active('Trigger_Espejo_Montado_MR', true)
		_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_ME', true)
		
		local position_MR = Vect3f(15.4891,0.00,4.95024)
		local position_ME = Vect3f(-15.4891,0.00,4.95024)
		_ENEMYM:create_enemy('Soldier100', position_MR, 'Monster100', position_ME)

		position_MR = Vect3f(29.8728,0.00,4.95024)
		position_ME = Vect3f(-29.8728,0.00,4.95024)
		_ENEMYM:create_enemy('Soldier101', position_MR, 'Monster101', position_ME)

		position_MR = Vect3f(29.8728,0.00,-11.8063)
		position_ME = Vect3f(-29.8728,0.00,-11.8063)
		_ENEMYM:create_enemy('Soldier102', position_MR, 'Monster102', position_ME)

		position_MR = Vect3f(15.7048,0.00,-11.8063)
		position_ME = Vect3f(-15.7048,0.00,-11.8063)
		_ENEMYM:create_enemy('Soldier103', position_MR, 'Monster103', position_ME)
		
		position_MR = Vect3f(21.9331,0.00,-2.19154)
		position_ME = Vect3f(-21.9331,0.00,-2.19154)
		_ENEMYM:create_enemy('Soldier104', position_MR, 'Monster104', position_ME)

		position_MR = Vect3f(28.5671,0.00,-2.19154)
		position_ME = Vect3f(-28.5671,0.00,-2.19154)
		_ENEMYM:create_enemy('Soldier105', position_MR, 'Monster105', position_ME)
		
		level2_final_enemies_spawned = true
	end
end



