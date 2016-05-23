-- Light functions

-- LEVEL 01
function luces_sala_inicial_MR_level01_ON()
	_LIGHTM:set_light_active('Light01_Omni_MR', true)
	_LIGHTM:set_light_active('Light02_Omni_MR', true)
	_LIGHTM:set_light_active('Light03_Omni_MR', true)
	_LIGHTM:set_light_active('Light04_Omni_MR', false)
	_LIGHTM:set_light_active('Light05_Omni_MR', false)
	_LIGHTM:set_light_active('Light06_Omni_MR', false)
	_LIGHTM:set_light_active('Light07_Omni_MR', false)
	_LIGHTM:set_light_active('Light08_Omni_MR', false)
	_LIGHTM:set_light_active('Light09_Omni_MR', false)
	_LIGHTM:set_light_active('Light10_Omni_MR', false)
	_LIGHTM:set_light_active('Light11_Omni_MR', false)
	_LIGHTM:set_light_active('Light12_Omni_MR', false)
end

function luces_pasillo_MR_level01_ON()
	_LIGHTM:set_light_active('Light01_Omni_MR', false)
	_LIGHTM:set_light_active('Light02_Omni_MR', false)
	_LIGHTM:set_light_active('Light03_Omni_MR', false)
	_LIGHTM:set_light_active('Light04_Omni_MR', true)
	_LIGHTM:set_light_active('Light05_Omni_MR', true)
	_LIGHTM:set_light_active('Light06_Omni_MR', true)
	_LIGHTM:set_light_active('Light07_Omni_MR', true)
	_LIGHTM:set_light_active('Light08_Omni_MR', true)
	_LIGHTM:set_light_active('Light09_Omni_MR', true)
	_LIGHTM:set_light_active('Light10_Omni_MR', true)
	_LIGHTM:set_light_active('Light11_Omni_MR', true)
	_LIGHTM:set_light_active('Light12_Omni_MR', true)
end


function luces_sala_inicial_ME_level01_ON()
	_LIGHTM:set_light_active('Light01_Omni_ME', true)
	_LIGHTM:set_light_active('Light02_Omni_ME', true)
	_LIGHTM:set_light_active('Light03_Omni_ME', false)
	_LIGHTM:set_light_active('Light04_Omni_ME', false)
	_LIGHTM:set_light_active('Light05_Omni_ME', false)
end

function luces_pasillo_ME_level01_ON()
	_LIGHTM:set_light_active('Light01_Omni_ME', false)
	_LIGHTM:set_light_active('Light02_Omni_ME', true)
	_LIGHTM:set_light_active('Light03_Omni_ME', true)
	_LIGHTM:set_light_active('Light04_Omni_ME', true)
	_LIGHTM:set_light_active('Light05_Omni_ME', true)
end

-- LEVEL 02

-- Mundo real
function luces_entrada_piso1_MR_level02_OFF()
	_LIGHTM:set_light_active('Omni_hall1_aplique001', false)
	_LIGHTM:set_light_active('Omni_hall1_aplique002', false)
	_LIGHTM:set_light_active('Omni_hall1_03', false)
	_LIGHTM:set_light_active('Omni_hall1_008', false)
end

function luces_entrada_piso1_MR_level02_ON()
	_LIGHTM:set_light_active('Omni_hall1_aplique001', true)
	_LIGHTM:set_light_active('Omni_hall1_aplique002', true)
	_LIGHTM:set_light_active('Omni_hall1_03', true)
	_LIGHTM:set_light_active('Omni_hall1_008', true)
end

---------------------------------------------------------------------------

function luces_pasillos_entrada_level2_ON()
	_LIGHTM:set_light_active('P_omni_MR012', true)
	_LIGHTM:set_light_active('P_omni_MR', true)
	_LIGHTM:set_light_active('P_omni_MR002', true)
	_LIGHTM:set_light_active('P_omni_MR003', true)
	_LIGHTM:set_light_active('Omni_SControl01', true)
	_LIGHTM:set_light_active('Omni_SControl004', true)
end	

function luces_hall_piso1_level2_ON()
	_LIGHTM:set_light_active('llum_sol', true)
	_LIGHTM:set_light_active('Omni_hall1_aplique005', true)
	_LIGHTM:set_light_active('Omni_hall1_aplique007', true)
	_LIGHTM:set_light_active('Omni_hall3_002', true)
	_LIGHTM:set_light_active('Omni_hall1_01', true)
	_LIGHTM:set_light_active('Omni_hall2_016', true)
	_LIGHTM:set_light_active('Omni_hall1_00', true)
	_LIGHTM:set_light_active('Omni_hall3', true)
	_LIGHTM:set_light_active('Omni_hall1_006', true)
	_LIGHTM:set_light_active('Omni_hall1_010', true)
	_LIGHTM:set_light_active('Omni_hall1_007', true)
end

function luces_ascensor_hall_piso1_ON()
	_LIGHTM:set_light_active('Omni_hall1_aplique004', true)
	_LIGHTM:set_light_active('Omni_hall2_015', true)
	_LIGHTM:set_light_active('Omni_hall1_aplique002', true)
	_LIGHTM:set_light_active('Omni_hall2_017', true)
	_LIGHTM:set_light_active('Omni_hall1_03', true)
	_LIGHTM:set_light_active('Omni_hall1_008', true)
end

function luces_hall_debajo_piso2_level2_ON()
	_LIGHTM:set_light_active('Omni_hall1_aplique003', true)
	_LIGHTM:set_light_active('Omni_hall2_018', true)
	_LIGHTM:set_light_active('Omni_hall1_009', true)
	_LIGHTM:set_light_active('Omni_hall1_aplique', true)
	_LIGHTM:set_light_active('Omni_hall2_011', true)
end

function luces_hab_piso2_level2_ON()
	_LIGHTM:set_light_active('P_omni_MR007', true)
	_LIGHTM:set_light_active('P_omni_MR009', true)
	_LIGHTM:set_light_active('P_omni_MR008', true)
	_LIGHTM:set_light_active('P_omni_MR005', true)
	_LIGHTM:set_light_active('P_omni_MR010', true)
	_LIGHTM:set_light_active('Omni_SControl003', true)
	_LIGHTM:set_light_active('Omni_SControl002', true)
	_LIGHTM:set_light_active('P_omni_MR006', true)
	_LIGHTM:set_light_active('P_omni_MR009', true)
	_LIGHTM:set_light_active('P_omni_MR008', true)
end
	
function luces_pasillos_entrada_level2_OFF()
	_LIGHTM:set_light_active('P_omni_MR012', false)
	_LIGHTM:set_light_active('P_omni_MR', false)
	_LIGHTM:set_light_active('P_omni_MR002', false)
	_LIGHTM:set_light_active('P_omni_MR003', false)
	_LIGHTM:set_light_active('Omni_SControl01', false)
	_LIGHTM:set_light_active('Omni_SControl004', false)
	
end	

function luces_hall_piso1_level2_OFF()
	_LIGHTM:set_light_active('llum_sol', false)
	_LIGHTM:set_light_active('Omni_hall1_aplique005', false)
	_LIGHTM:set_light_active('Omni_hall1_aplique007', false)
	_LIGHTM:set_light_active('Omni_hall3_002', false)
	_LIGHTM:set_light_active('Omni_hall1_01', false)
	_LIGHTM:set_light_active('Omni_hall2_016', false)
	_LIGHTM:set_light_active('Omni_hall1_00', false)
	_LIGHTM:set_light_active('Omni_hall3', false)
	_LIGHTM:set_light_active('Omni_hall1_006', false)
	_LIGHTM:set_light_active('Omni_hall1_010', false)
	_LIGHTM:set_light_active('Omni_hall1_007', false)
end

function luces_ascensor_hall_piso1_OFF()
	_LIGHTM:set_light_active('Omni_hall1_aplique004', false)
	_LIGHTM:set_light_active('Omni_hall2_015', false)
	_LIGHTM:set_light_active('Omni_hall1_aplique002', false)
	_LIGHTM:set_light_active('Omni_hall2_017', false)
	_LIGHTM:set_light_active('Omni_hall1_03', false)
	_LIGHTM:set_light_active('Omni_hall1_008', false)
end

function luces_hall_debajo_piso2_level2_OFF()
	_LIGHTM:set_light_active('Omni_hall1_aplique003', false)
	_LIGHTM:set_light_active('Omni_hall2_018', false)
	_LIGHTM:set_light_active('Omni_hall1_009', false)
	_LIGHTM:set_light_active('Omni_hall1_aplique', false)
	_LIGHTM:set_light_active('Omni_hall2_011', false)
end

function luces_hab_piso2_level2_OFF()
	_LIGHTM:set_light_active('P_omni_MR007', false)
	_LIGHTM:set_light_active('P_omni_MR009', false)
	_LIGHTM:set_light_active('P_omni_MR008', false)
	_LIGHTM:set_light_active('P_omni_MR005', false)
	_LIGHTM:set_light_active('P_omni_MR010', false)
	_LIGHTM:set_light_active('Omni_SControl003', false)
	_LIGHTM:set_light_active('Omni_SControl002', false)
	_LIGHTM:set_light_active('P_omni_MR006', false)
	_LIGHTM:set_light_active('P_omni_MR009', false)
	_LIGHTM:set_light_active('P_omni_MR008', false)
end


function opt_luces01_level2()
	luces_pasillos_entrada_level2_ON()
	luces_hall_piso1_level2_OFF()
	luces_ascensor_hall_piso1_OFF()
	luces_hall_debajo_piso2_level2_OFF()
	luces_hab_piso2_level2_OFF()
end

function opt_luces02_level2()
	luces_pasillos_entrada_level2_OFF()
	luces_hall_piso1_level2_ON()
	luces_ascensor_hall_piso1_ON()
	luces_hall_debajo_piso2_level2_ON()
	luces_hab_piso2_level2_ON()
end


-- Mundo espejo
function luces_entrada_piso1_ME_level02_OFF()
	_LIGHTM:set_light_active('ApliqueOmni ME003', false)
	_LIGHTM:set_light_active('ApliqueOmni ME001', false)
	_LIGHTM:set_light_active('Omni_hall001', false)
	_LIGHTM:set_light_active('ApliqueOmni ME007', false)
end

function luces_entrada_piso1_ME_level02_ON()
	_LIGHTM:set_light_active('ApliqueOmni ME003', true)
	_LIGHTM:set_light_active('ApliqueOmni ME001', true)
	_LIGHTM:set_light_active('Omni_hall001', true)
	_LIGHTM:set_light_active('ApliqueOmni ME007', true)
end

function luces_piso1_superior_ME_level02_ON()
	_LIGHTM:set_light_active('ApliqueOmni ME', true)
end

function luces_piso1_superior_ME_level02_OFF()
	_LIGHTM:set_light_active('ApliqueOmni ME', false)
end

---------------------------------------------------------------------------

function luces_pasillo_inicial_level2_ME_ON()
	_LIGHTM:set_light_active('Omni_Espejo01_ME_Level02', true)
	_LIGHTM:set_light_active('Omni_pasillo', true)
	_LIGHTM:set_light_active('Omni019', true)
end

function luces_entrada_hall_level2_ME_ON()
	_LIGHTM:set_light_active('ApliqueOmni ME_attenuation', true)
	_LIGHTM:set_light_active('ApliqueOmni ME_attenuation002', true)
	_LIGHTM:set_light_active('Omni_hall005', true)
	_LIGHTM:set_light_active('Omni_recepcio', true)
	_LIGHTM:set_light_active('Omni_hall008', true)
end

function luces_hall_level2_ME_ON()
	_LIGHTM:set_light_active('llum_lluna', true)
	_LIGHTM:set_light_active('ApliqueOmni ME', true)
	_LIGHTM:set_light_active('Omni_hall', true)
	_LIGHTM:set_light_active('Omni_hall002', true)
	_LIGHTM:set_light_active('Omni_hall003', true)
end

function luces_ascensor_hall_piso1_ME_level2_ON()
	_LIGHTM:set_light_active('ApliqueOmni ME007', true)
	_LIGHTM:set_light_active('Omni_hall001', true)
	_LIGHTM:set_light_active('Omni_hall009', true)
	_LIGHTM:set_light_active('ApliqueOmni ME001', true)
	_LIGHTM:set_light_active('ApliqueOmni ME005', true)
	_LIGHTM:set_light_active('ApliqueOmni ME004', true)
end

function luces_pasillo_inicial_level2_ME_OFF()
	_LIGHTM:set_light_active('Omni_pasillo', false)
	_LIGHTM:set_light_active('Omni019', false)
	_LIGHTM:set_light_active('Omni pasillo ME_attenuation', false)
	_LIGHTM:set_light_active('Omni_Espejo01_ME_Level02', false)
	_LIGHTM:set_light_active('P_omni_ME002', false)
end

function luces_pasillo_inicial_level2_ME_ON()
	_LIGHTM:set_light_active('Omni pasillo ME_attenuation', true)
	_LIGHTM:set_light_active('Omni_Espejo01_ME_Level02', true)
	_LIGHTM:set_light_active('P_omni_ME002', true)
end

function luces_entrada_hall_level2_ME_OFF()
	_LIGHTM:set_light_active('ApliqueOmni ME_attenuation', false)
	_LIGHTM:set_light_active('ApliqueOmni ME_attenuation002', false)
	_LIGHTM:set_light_active('Omni_hall005', false)
	_LIGHTM:set_light_active('Omni_recepcio', false)
	_LIGHTM:set_light_active('Omni_hall008', false)
end

function luces_hall_level2_ME_OFF()
	_LIGHTM:set_light_active('llum_lluna', false)
	_LIGHTM:set_light_active('ApliqueOmni ME', false)
	_LIGHTM:set_light_active('Omni_hall', false)
	_LIGHTM:set_light_active('Omni_hall002', false)
	_LIGHTM:set_light_active('Omni_hall003', false)
end

function luces_ascensor_hall_piso1_ME_level2_OFF()
	_LIGHTM:set_light_active('ApliqueOmni ME007', false)
	_LIGHTM:set_light_active('Omni_hall001', false)
	_LIGHTM:set_light_active('Omni_hall009', false)
	_LIGHTM:set_light_active('ApliqueOmni ME001', false)
	_LIGHTM:set_light_active('ApliqueOmni ME005', false)
	_LIGHTM:set_light_active('ApliqueOmni ME004', false)
end


function opt_luces03_level2()
	luces_pasillo_inicial_level2_ME_ON()
	luces_entrada_hall_level2_ME_OFF()
	luces_hall_level2_ME_OFF()
	luces_ascensor_hall_piso1_ME_level2_OFF()
end

function opt_luces04_level2()
	luces_pasillo_inicial_level2_ME_ON()
	luces_entrada_hall_level2_ME_ON()
	luces_hall_level2_ME_ON()
	luces_ascensor_hall_piso1_ME_level2_ON()
end

function opt_luces05_level2()
	luces_pasillo_inicial_level2_ME_OFF()
	luces_entrada_hall_level2_ME_ON()
	luces_hall_level2_ME_ON()
	luces_ascensor_hall_piso1_ME_level2_ON()
end

function opt_luces06_level2()
	luces_pasillo_inicial_level2_ME_ON()
end


