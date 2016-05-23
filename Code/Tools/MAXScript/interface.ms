fileIn "ExportMesh.ms"
fileIn "ExportTextures.ms"
fileIn "StaticMeshManager.ms"
fileIn "ExportLights.ms"
fileIn "ExportTriggersAndObjects.ms"
fileIn "SetProperties.ms"
fileIn "SetLightProperties.ms"

global data_path=undefined 

--data_path= "D:\\UAB\\Projects\\videogame\\trunk\\Run\\Viewer"
--data_path= "C:\\iVaN\\UAB\\Master\\Projects\\videogame\\trunk\\Run\\Viewer"
--heapSize = 32*1024*1024

utility UABUtility "UAB Utility" width:160 height:378
(
	button btn1 "Set Path" pos:[13,13] width:129 height:20
	label lbl1 "Path" pos:[15,43] width:126 height:58
	checkbox chk1 "Export Mesh" pos:[16,112] width:120 height:16
	checkbox chk2 "Export Textures" pos:[16,128] width:128 height:16
	checkbox chk3 "Sobreescribir partículas" pos:[16,96] width:128 height:16
	GroupBox grp1 "Mesh" pos:[8,184] width:136 height:152
	dropdownList ddlLevel "" pos:[8,152] width:144 height:21 items:#("Level01","Level02")
	button btn2 "Lights" pos:[16,208] width:56 height:24
	button btn3 "Mesh" pos:[80,208] width:56 height:24
	button btn7 "Get/Set Properties" pos:[12,344] width:130 height:21
	button btn5 "Static Mesh Manager" pos:[16,240] width:120 height:24
	button btn6 "Scene" pos:[16,272] width:120 height:24
	button btn8 "Only XML" pos:[16,304] width:52 height:23
	button btn9 "Logic" pos:[77,304] width:58 height:23
	
	-- Contenido de la utility

	function ExportaLucesDinamicas =
	(
		format "\nExportando luces dinamicas... F8 en el Viewer para recargar!\n"
		
		if data_path!=undefined then 
			lbl1.text=data_path
		
		if data_path==undefined then 
			data_path= getSavePath caption: "Export Lights Path" initialDir:"d:\\"
		
		if data_path!=undefined then
		(
			lbl1.text=data_path
			ExportLights data_path ddlLevel.items[ddlLevel.Selection]
		)		
	)
	
	function ExportaObjetos =
	(
		format "\nExportando triggers y objetos logicos... F6 en el Viewer para recargar!\n"
		
		if data_path!=undefined then 
			lbl1.text=data_path
		
		if data_path==undefined then 
			data_path= getSavePath caption: "Export Objects Path" initialDir:"d:\\"
		
		if data_path!=undefined then
		(
			lbl1.text=data_path
			ExportObjects data_path ddlLevel.items[ddlLevel.Selection] chk3.checked
		)		
	)
	
	function ExportaStaticMeshManager only_xml =
	(
		local l_Err=false
		if data_path!=undefined then 
			lbl1.text=data_path
		
		local l_Export=false
		
		if (only_xml==true) then
		(
			l_Export=true
		)
		else
		(
			if 	chk1.checked or chk2.checked then
			(
				l_Export = true
			)
			else
			(
				Messagebox "ERROR: Hay que seleccionar Export Mesh o Export Textures para la acción Static Mesh Manager!"
			)
		)
		
		if 	(l_Export==true) then
		(
			if (data_path==undefined) then 
			(
				data_path= getSavePath caption: "Export Static Meshes Path" initialDir:"d:\\"
			)
			
			if data_path!=undefined then
			(
				lbl1.text=data_path
				
				--if chk1.checked then
					l_Err=StaticMeshManager data_path ddlLevel.items[ddlLevel.Selection] only_xml 
				
				if (only_xml==false) then
				(
					if l_Err==false then
					(
						if chk2.checked then
							ExportAllTextures data_path false
					)
					else
					(
						Messagebox "ERROR: Error en la exportación!"
					)
				)
			)
		)
	
		if data_path!=undefined then 
			lbl1.text=data_path
	)

	-- Set Path button
	on btn1 pressed do
	(
		data_path= getSavePath caption: "Data Path" initialDir:"d:\\"
		if data_path!=undefined then 
			lbl1.text=data_path
		format "Path actual: %\n" data_path
	)
	
	-- Lights button
	on btn2 pressed do
	(
		ExportaLucesDinamicas()
	)
	
	-- Mesh button
	on btn3 pressed do
	(
		if data_path!=undefined then 
			lbl1.text=data_path
		
		local l_Export = true
		
		if ($==undefined) then
		(
			Messagebox "ERROR: Hay que seleccionar un objeto para la acción 'Mesh'!"
			l_Export = false
		)
		if (l_Export==true) then
		(
			if chk1.checked==false and chk2.checked==false then
			(
				Messagebox "ERROR: Hay que marcar la casilla 'Export Mesh' o 'Export Texture'!"
				l_Export = false
			)
		)		
		
		if (l_Export==true) then
		(
			--Para elegir fichero
			if 	chk1.checked or chk2.checked then
			(
				--Pide nombre fichero
				if chk1.checked then
				(
					data_path=getSaveFileName caption: "Export Mesh" \
					types:"Mesh (*.m3d)|*.m3d|All|*.*|" 
				
				)
				--No pide nombre fichero
				else
				(
					if data_path==undefined then
					(
						data_path= getSavePath caption: "Export Textures" initialDir:"d:\\"
						if data_path!=undefined then 
							lbl1.text=data_path
					)
				)
			)  
			
			-- Export Mesh
			if 	chk1.checked == true then
			(
				if data_path!=undefined then 
				(
					format "%\n" data_path
					CreateMesh $ data_path
				)
			)
		 
			--Export Textures
			if 	chk2.checked == true then
			(
				if data_path!=undefined then
				(	 
					ExportTextures data_path $ chk1.checked
				)
			)
		)
		
		--if data_path!=undefined then 
		--	lbl1.text=data_path
	)
	
	-- Set/Get Properties button
	on btn7 pressed do
	(
		if $ == undefined then
		(
			Messagebox "ERROR: Hay que seleccionar un objeto para poder acceder a sus propiedades!"
		)
		else if isMesh $ or isPointHelper $ then
		(
			createDialog GetSetObjProperties
			GetSetObjProperties.btnGet.pressed()
		)
		else if isLight $ then
		(
			--if classof $ == TargetDirectionallight or classof $ == targetSpot then
			--(
				createDialog GetSetLightProperties
				GetSetLightProperties.btnGet.pressed()
			--)
			--else
			--(
				--Messagebox "ERROR: Get/Set Properties is only available for TargetDirectionalLights and TargetSpot!"
			--)
		)
		else
		(
			Messagebox "ERROR: Tipo de objeto desconocido!"
		)
	)
	

	-- Static Mesh Manager button
	on btn5 pressed do
	(
		ExportaStaticMeshManager(false)
	)

	-- Scene button
	on btn6 pressed do
	(
		format "\nExportando nivel...\n"
		
		ExportaStaticMeshManager(false)
		ExportaLucesDinamicas()
		ExportaObjetos()
		
		format "OK! (F5 en el Viewer para recargar todo)\n"
	)

	-- XML button
	on btn8 pressed do
	(
		ExportaStaticMeshManager(true)
	)
	
	-- Logic button
	on btn9 pressed do
	(
		ExportaObjetos()
	)
	
)




