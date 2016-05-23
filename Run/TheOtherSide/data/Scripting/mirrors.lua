-- Mirror functions

-- LEVEL 01
function espejo_largo_MR_level01_ON()
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_MR', true)
	_LOGICOBJECTSM:set_mirror_active('Box_EspejoEST_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Box_EspejoROT_MR', false)	
	
	luces_sala_inicial_MR_level01_ON()
end

function espejo_largo_ME_level01_ON()
	_LOGICOBJECTSM:set_mirror_active('Trigger_espejo_grande_ME', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Box_EspejoROT_ME', false)	
	
	luces_sala_inicial_ME_level01_ON()
end

function espejos_fin_MR_level01()
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Box_EspejoEST_MR', true)
	_LOGICOBJECTSM:set_mirror_active('Box_EspejoROT_MR', true)
	
	luces_pasillo_MR_level01_ON()
end

function espejos_fin_ME_level01()
	_LOGICOBJECTSM:set_mirror_active('Trigger_espejo_grande_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_EspejoEstatico_ME', true)
	_LOGICOBJECTSM:set_mirror_active('Box_EspejoROT_ME', true)	
	
	luces_pasillo_ME_level01_ON()
end

-- LEVEL 02 MUNDO ESPEJO
function espejo_inicial_ME_level02_ON()
	_LOGICOBJECTSM:set_mirror_active('Trigger_Pasillo01_EspejoEstatico_ME', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall01_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoRotatorio_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Final_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_ME_Level02', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_ME', true)
end

function espejos_hall01_ME_level02_ON()
	_LOGICOBJECTSM:set_mirror_active('Trigger_Pasillo01_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall01_EspejoEstatico_ME', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoRotatorio_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Final_ME', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_ME_Level02', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_ME', true)
end

function espejos_hall02_ME_level02_ON()
	_LOGICOBJECTSM:set_mirror_active('Trigger_Pasillo01_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall01_EspejoEstatico_ME', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoRotatorio_ME', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoEstatico_ME', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Final_ME', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_ME_Level02', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_ME', true)
	luces_entrada_piso1_ME_level02_OFF()
end

function espejo_svigilancia_ME_level02_ON()
	_LOGICOBJECTSM:set_mirror_active('Trigger_Pasillo01_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall01_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoRotatorio_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Final_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_ME_Level02', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_ME', false)
end

function espejos_ME_level02_OFF()
	_LOGICOBJECTSM:set_mirror_active('Trigger_Pasillo01_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall01_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoRotatorio_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoEstatico_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Final_ME', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_ME_Level02', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_ME', false)
end

-- LEVEL 02 MUNDO REAL
function espejo_inicial_MR_level02_ON()
	_LOGICOBJECTSM:set_mirror_active('Trigger_Pasillo01_EspejoEstatico_MR', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall01_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoRotatorio_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Final_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_MR_Level02', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_MR', true)
end

function espejos_hall01_MR_level02_ON()
	_LOGICOBJECTSM:set_mirror_active('Trigger_Pasillo01_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall01_EspejoEstatico_MR', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoRotatorio_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Final_MR', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_MR_Level02', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_MR', true)
end

function espejos_hall02_MR_level02_ON()
	_LOGICOBJECTSM:set_mirror_active('Trigger_Pasillo01_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall01_EspejoEstatico_MR', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoRotatorio_MR', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoEstatico_MR', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Final_MR', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_MR_Level02', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_MR', true)
	
	luces_entrada_piso1_MR_level02_OFF()
end

function espejo_svigilancia_MR_level02_ON()
	_LOGICOBJECTSM:set_mirror_active('Trigger_Pasillo01_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall01_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoRotatorio_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Final_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_MR_Level02', true)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_MR', false)
end

function espejos_MR_level02_OFF()
	_LOGICOBJECTSM:set_mirror_active('Trigger_Pasillo01_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall01_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoRotatorio_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Hall02_EspejoEstatico_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Final_MR', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_SVigilado_Espejo_MR_Level02', false)
	_LOGICOBJECTSM:set_mirror_active('Trigger_Espejo_Montado_MR', false)
end