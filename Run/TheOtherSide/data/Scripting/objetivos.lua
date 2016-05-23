-- Objetivos

function objetivo1()
	_TRIGGERM:set_active('Trigger_Objetivo_Espejo_MR_Level1',false)
	_GUI:set_visible_gui_element('objetivo_1', true)
	_GUI:set_visible_gui_element('objetivo_2', false)
	_GUI:set_visible_gui_element('objetivo_3', false)
	_GUI:set_visible_gui_element('objetivo_4', false)
	_GUI:set_visible_gui_element('objetivo_5', false)
	_GUI:set_visible_gui_element('objetivo_6', false)
	_GUI:set_visible_gui_element('objetivo_7', false)
end

function objetivo2()
	_GUI:set_visible_gui_element('objetivo_1', false)
	_GUI:set_visible_gui_element('objetivo_2', true)
	_GUI:set_visible_gui_element('objetivo_3', false)
	_GUI:set_visible_gui_element('objetivo_4', false)
	_GUI:set_visible_gui_element('objetivo_5', false)
	_GUI:set_visible_gui_element('objetivo_6', false)
	_GUI:set_visible_gui_element('objetivo_7', false)
	_TRIGGERM:set_active('Trigger_Objetivo_Espejo_MR_Level1',true)
	
	_GUI:set_active_gui_element('objetivo_1', false)
end

function objetivo3()
	_GUI:set_visible_gui_element('objetivo_1', false)
	_GUI:set_visible_gui_element('objetivo_2', false)
	_GUI:set_visible_gui_element('objetivo_3', true)
	_GUI:set_visible_gui_element('objetivo_4', false)
	_GUI:set_visible_gui_element('objetivo_5', false)
	_GUI:set_visible_gui_element('objetivo_6', false)
	_GUI:set_visible_gui_element('objetivo_7', false)
	
	_GUI:set_active_gui_element('objetivo_2', false)
end

function objetivo4()
	_TRIGGERM:set_active('Trigger_Objetivo_Espejo_MR_Level1',false)
	
	_GUI:set_visible_gui_element('objetivo_1', false)
	_GUI:set_visible_gui_element('objetivo_2', false)
	_GUI:set_visible_gui_element('objetivo_3', false)
	_GUI:set_visible_gui_element('objetivo_4', true)
	_GUI:set_visible_gui_element('objetivo_5', false)
	_GUI:set_visible_gui_element('objetivo_6', false)
	_GUI:set_visible_gui_element('objetivo_7', false)
	
	_GUI:set_active_gui_element('objetivo_3', false)
end

function objetivo5()
	_GUI:set_visible_gui_element('objetivo_1', false)
	_GUI:set_visible_gui_element('objetivo_2', false)
	_GUI:set_visible_gui_element('objetivo_3', false)
	_GUI:set_visible_gui_element('objetivo_4', false)
	_GUI:set_visible_gui_element('objetivo_5', true)
	_GUI:set_visible_gui_element('objetivo_6', false)
	_GUI:set_visible_gui_element('objetivo_7', false)
	
	_GUI:set_active_gui_element('objetivo_4', false)
end

function objetivo6()
	_GUI:set_visible_gui_element('objetivo_1', false)
	_GUI:set_visible_gui_element('objetivo_2', false)
	_GUI:set_visible_gui_element('objetivo_3', false)
	_GUI:set_visible_gui_element('objetivo_4', false)
	_GUI:set_visible_gui_element('objetivo_5', false)
	_GUI:set_visible_gui_element('objetivo_6', true)
	_GUI:set_visible_gui_element('objetivo_7', false)
	
	_GUI:set_active_gui_element('objetivo_5', false)
end

function objetivo7()
	_GUI:set_visible_gui_element('objetivo_1', false)
	_GUI:set_visible_gui_element('objetivo_2', false)
	_GUI:set_visible_gui_element('objetivo_3', false)
	_GUI:set_visible_gui_element('objetivo_4', false)
	_GUI:set_visible_gui_element('objetivo_5', false)
	_GUI:set_visible_gui_element('objetivo_6', false)
	_GUI:set_visible_gui_element('objetivo_7', true)
	
	_GUI:set_active_gui_element('objetivo_6', false)
end

function objetivo8()
	_GUI:set_active_gui_element('objetivo_1', false)
	_GUI:set_active_gui_element('objetivo_2', false)
	_GUI:set_active_gui_element('objetivo_3', false)
	_GUI:set_active_gui_element('objetivo_4', false)
	_GUI:set_active_gui_element('objetivo_5', false)
	_GUI:set_active_gui_element('objetivo_6', false)
	_GUI:set_active_gui_element('objetivo_7', false)

	_GUI:set_visible_gui_element('objetivo_7', false)
	_GUI:set_active_gui_element('objetivo_8', true)
	_GUI:set_visible_gui_element('objetivo_8', true)
end

function objetivo9()
	_GUI:set_active_gui_element('objetivo_1', false)
	_GUI:set_active_gui_element('objetivo_2', false)
	_GUI:set_active_gui_element('objetivo_3', false)
	_GUI:set_active_gui_element('objetivo_4', false)
	_GUI:set_active_gui_element('objetivo_5', false)
	_GUI:set_active_gui_element('objetivo_6', false)
	_GUI:set_active_gui_element('objetivo_7', false)
	_GUI:set_active_gui_element('objetivo_8', false)
	_GUI:set_visible_gui_element('objetivo_8', false)
	
	_GUI:set_active_gui_element('objetivo_9', true)
	_GUI:set_visible_gui_element('objetivo_9', true)
end

function objetivo10()
	_GUI:set_active_gui_element('objetivo_1', false)
	_GUI:set_active_gui_element('objetivo_2', false)
	_GUI:set_active_gui_element('objetivo_3', false)
	_GUI:set_active_gui_element('objetivo_4', false)
	_GUI:set_active_gui_element('objetivo_5', false)
	_GUI:set_active_gui_element('objetivo_6', false)
	_GUI:set_active_gui_element('objetivo_7', false)
	_GUI:set_active_gui_element('objetivo_8', false)
	_GUI:set_active_gui_element('objetivo_9', false)
	_GUI:set_visible_gui_element('objetivo_9', false)
	
	_GUI:set_active_gui_element('objetivo_10', true)
	_GUI:set_visible_gui_element('objetivo_10', true)
end

function objetivos_OFF()
	_GUI:set_active_gui_element('objetivo_1', false)
	_GUI:set_active_gui_element('objetivo_2', false)
	_GUI:set_active_gui_element('objetivo_3', false)
	_GUI:set_active_gui_element('objetivo_4', false)
	_GUI:set_active_gui_element('objetivo_5', false)
	_GUI:set_active_gui_element('objetivo_6', false)
	_GUI:set_active_gui_element('objetivo_7', false)
	_GUI:set_active_gui_element('objetivo_8', false)
	_GUI:set_active_gui_element('objetivo_9', false)
	_GUI:set_active_gui_element('objetivo_10', false)
	
	_GUI:set_visible_gui_element('objetivo_1', false)
	_GUI:set_visible_gui_element('objetivo_2', false)
	_GUI:set_visible_gui_element('objetivo_3', false)
	_GUI:set_visible_gui_element('objetivo_4', false)
	_GUI:set_visible_gui_element('objetivo_5', false)
	_GUI:set_visible_gui_element('objetivo_6', false)
	_GUI:set_visible_gui_element('objetivo_7', false)
	_GUI:set_visible_gui_element('objetivo_8', false)
	_GUI:set_visible_gui_element('objetivo_9', false)
	_GUI:set_visible_gui_element('objetivo_10', false)

end