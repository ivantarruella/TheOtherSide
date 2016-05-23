fileIn "functions.ms" 
fileIn "ExportMesh.ms"
fileIn "ExportLights.ms"

fn CreateXmlFiles Cores Instances InstancesAndCores DataPath Level only_xml =
(
	-- Creamos fichero static_meshes.xml
	local file1=createfile (DataPath+"\\"+g_XMLPath+Level+"\\"+"static_meshes.xml")
	
	--format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file1
	format "<Static_Meshes>\n" to:file1
	for i=1 to Cores.count do
	(
		local l_ExportType = getUserProp Cores[i] "export_type"
		local l_LogicType = getUserProp InstancesAndCores[i] "generate_logic"
		--format "DEBUG> l_LogicType % \n" l_LogicType
		if (l_ExportType != "animated_model_instance" or l_LogicType=="Elevator") then
		(
			local l_CoreName = Cores[i].name
			local l_FileName = l_CoreName + ".m3d"
			local l_FilePath = DataPath+"\\"+g_MeshesPath+"\\"+l_FileName
						
			if (only_xml == false) then
			(
				CreateMesh Cores[i] l_FilePath
			)
			
			--format "DEBUG> CreateMesh % \n" Cores[i]
			local l_RelPath = g_MeshesPath + l_FileName
			format "\t<static_mesh name=\"%\" filename=\"%\"/>\n" l_CoreName l_RelPath to:file1
		)
	)
	format "</Static_Meshes>\n" to:file1

	close file1	
	
	-- Creamos fichero renderable_objects.xml
	local file1=createfile (DataPath+"\\"+g_XMLPath+Level+"\\"+"renderable_objects.xml")
	
	--format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file1
	format "<Renderable_Objects>\n" to:file1
	-- Layers :
	for i=1 to g_ddlLayers.count do
	(
		if i==1 then
		(
			-- Layer por defecto es la primera siempre ??????????
			format "\t<layer name=\"%\" default=\"true\"/>\n" g_ddlLayers[i] to:file1
		)
		else
		(
			format "\t<layer name=\"%\"/>\n" g_ddlLayers[i] to:file1
		)
	)
	
	-- Renderable objects:
	for i=1 to Instances.count do
	(
		local l_ExportType = getUserProp Instances[i] "export_type"
		
		if l_ExportType == undefined then
		(
			l_ExportType = g_ddlExportType[1]
		) 
		if  l_ExportType != false then
		(
			--local l_InstanceName = Instances[i].name + "_" + (i as string)
			local l_InstanceName = Instances[i].name
			local l_CoreName = InstancesAndCores[i].name
			local l_Layer = getUserProp Instances[i] "layer"
			if l_Layer == undefined then
			(
				l_Layer = g_ddlLayers[1]
			)
			local l_Position_x = Instances[i].position.x
			local l_Position_y = Instances[i].position.z
			local l_Position_z = Instances[i].position.y
			local l_Yaw = GetYaw Instances[i].transform
			local l_Pitch = GetPitch Instances[i].transform
			local l_Roll = GetRoll Instances[i].transform
			local l_Visible = getUserProp Instances[i] "visible"
			
			if l_Visible == undefined then
			(
				l_Visible = true	-- por defecto los objetos seran visibles
			)
			
			local l_CastShadows = getUserProp Instances[i] "cast_shadows"
			if l_CastShadows == undefined then
			(
				l_CastShadows = true	-- por defecto los objetos generaran sombra
			)
			
			-- Caso especial de exportacion de modelos animados
			if (l_ExportType == "animated_model_instance") then
			(
				local l_PropName = getUserProp Instances[i] "anim_model"
				if (l_PropName != "") then
					l_InstanceName = l_PropName
				
				l_CoreName = getUserProp Instances[i] "generate_logic"
				l_Yaw = -3.14159
				l_Pitch = 0.0
				l_Roll = 0.0
				if (l_PropName == "PLAYER") then
				(
					l_Yaw = -1.047
					l_Position_y = 0
				)
				
				if (l_CoreName == "Elevator") then
				(
					l_Yaw = getUserProp Instances[i] "elevator_orientation"
				)				
			)
			
			format "\t<% name=\"%\" core=\"%\" layer=\"%\" pos=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" visible=\"%\" cast_shadows=\"%\" " l_ExportType l_InstanceName l_CoreName l_Layer l_Position_x l_Position_y l_Position_z l_Yaw l_Pitch l_Roll l_Visible l_CastShadows to:file1
			
			local l_GeneratePhysics = getUserProp Instances[i] "generate_physics"
			if l_GeneratePhysics==false then
			(
				format "generate_physics=\"false\" " to:file1
			)
			else 
			(
				if l_GeneratePhysics==undefined then
					l_GeneratePhysics=g_ddlGeneratePhysics[1]
				format "generate_physics=\"true\" physics_type=\"%\" " l_GeneratePhysics to:file1
			)
			
			-- por defecto no se pone
			--se pone en el xml si esta la fisica activada
			local l_LayerPhysics = getUserProp Instances[i] "layer_physics"
			if l_GeneratePhysics!=false and l_LayerPhysics!=false and l_LayerPhysics!=undefined then
			(
				--if l_LayerPhysics==undefined then
				--	l_LayerPhysics=g_ddlLayerPhysics[1]
				format "physics_layer=\"%\" " l_LayerPhysics to:file1
			)
				
			
			--local l_GenerateLogic = getUserProp Instances[i] "generate_logic"
			--if l_GenerateLogic==false or l_GenerateLogic==undefined then
			--(
			--	format "generate_logic=\"false\" " to:file1
			--)
			--else 
			--(
				--if l_GenerateLogic==undefined then
				--	l_GenerateLogic=g_ddlGenerateLogic[1]
			--	format "generate_logic=\"true\" logic_type=\"%\" " l_GenerateLogic to:file1
			--)
			
			format "/>\n" to:file1
		)
	)
	format "</Renderable_Objects>\n" to:file1

	close file1
)

fn PrintDebugInfo Cores Instances InstancesAndCores=
(
	format "Cores: %\n" Cores.count
	for i=1 to Cores.count do
	(
		format "Core[%] : % \n" i Cores[i]
	)
	format "Instances: %\n" Instances.count
	for i=1 to Instances.count do
	(
		format "Instance[%] : % " i Instances[i]
		format "from Core : % \n" InstancesAndCores[i]
	)
	format "\n"
)

fn GetInstance Objs Obj =
(
	for i = 1 to Objs.count do
	(
		if (areNodesInstances Obj Objs[i]) then
			return Objs[i]
	)
	
	return undefined
)

fn StaticMeshManager DataPath Level only_xml =
(
	local l_Cores=#()
	local l_Instances=#() 
	local l_InstancesAndCores=#()
	
	local l_Objs = $*
	local l_NumObjects = l_Objs.count
	local l_Error = false
	
	if (only_xml == false) then
	(
		--format "DEBUG> Núm. objects in scene: %\n" l_NumObjects
	)
	
	for i= 1 to l_NumObjects do
	(
		if l_Error==false then
		(
			l_Object = l_Objs[i]
				
			--if classof l_Object==Editable_Mesh or classof l_Object==Editable_Poly or classof l_Object==PolyMeshObject then
			if isMesh l_Object or isPointHelper l_Object then
			(
				if (only_xml == false) then
				(
					--format "DEBUG> Scanning object %:\n" i
					--format "DEBUG> % found!\n" l_Object	
				)
				
				-- Si no s'ha d'exportar no el guarda i evitem problemes
				local l_ExportType = getUserProp l_Object "export_type"
				if l_ExportType!=false then
				(
				
					local l_Core=undefined
					if l_Cores.count==0 then
					(
						if (only_xml == false) then
						(
							--format "DEBUG> Core: %\n" l_Object
						)
						l_Core = l_Object
						append l_Cores l_Core
					)
					else
					(
						l_Core = GetInstance l_Cores l_Object
						if l_Core==undefined then
						(
							l_Core=l_Object
							append l_Cores l_Core
							if (only_xml == false) then
							(
								--format "DEBUG> Core: %\n" l_Core					
							)
						)
					)
					append l_Instances l_Object
					append l_InstancesAndCores l_Core
					if (only_xml == false) then
					(
						--format "DEBUG> Instance: %\n" l_Object
					)
				)
			)
			else if isLight l_Object or classof l_Object == Targetobject then
			(
				-- Aquí no hay que hacer nada, las luces se exportan en ExportLights.ms
			)
			else if isBone l_Object then
			(
				-- Tampoco hay que hacer nada con los bones, ya se exportan en las mallas animadas
			)
			else
			(
				format "ERROR: No se puede exportar objeto %!\n" l_Object.name
				Messagebox "ERROR: No se puede exportar uno de los objetos ya que no se reconoce su tipo!\n"
				l_Error = true
			)
		)
	)

	if l_Error==false then
	(
		CreateXmlFiles l_Cores l_Instances l_InstancesAndCores DataPath Level only_xml
		if (only_xml == false) then
		(
			--PrintDebugInfo l_Cores l_Instances l_InstancesAndCores
		)
		else
		(
			format "XML regenerado satisfactoriamente, F5 en el Viewer para recargar!\n"
		)
	)
	
	return l_Error
)

--CreateCoresAndInstances "D:\\UAB\\Projects\\TheOtherSide\\trunk\\Run\\Viewer\\data\\Meshes"