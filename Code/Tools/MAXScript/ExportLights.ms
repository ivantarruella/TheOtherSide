fn CreateLightsXml  DataPath  Lights =
(
	-- Creamos fichero lights.xml
	local file=createfile (DataPath+"\\lights.xml")
	
	--format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file1
	format "<Lights>\n" to:file
	for i=1 to Lights.count do
	(
		format "\t<light" to:file
		format " name=\"%\"" Lights[i].name to:file
		
		local l_Type=undefined
		if classof Lights[i] == targetSpot then
			l_Type = "spot"
		else if classof Lights[i] == TargetDirectionallight then
			l_Type = "directional"
		else if classof Lights[i] == Omnilight then
			l_Type = "omni"

		local l_AttenStart = Lights[i].farAttenStart 
		local l_AttenEnd = Lights[i].farAttenEnd	
		
		if (Lights[i].name=="Ambient_MR" or Lights[i].name=="Ambient_ME") then
		(
			l_AttenStart=0.0
			l_AttenEnd=0.0
		)
		
		format " type=\"%\"" l_Type to:file
		format " pos=\"% % %\"" Lights[i].pos.x Lights[i].pos.z Lights[i].pos.y to:file
		format " color=\"% % %\"" Lights[i].rgb.r Lights[i].rgb.g Lights[i].rgb.b to:file
		format " att_start_range=\"%\"" l_AttenStart to:file
		format " att_end_range=\"%\"" l_AttenEnd to:file

		if classof Lights[i] != Omnilight then		-- Luz omni no tiene direccion 
		(
			--(Lights[i].target.pos-Lights[i].pos)
			local l_Dir=-Lights[i].dir
			format " dir=\"% % %\"\n\t" l_Dir.x l_Dir.z l_Dir.y to:file
		)
		
		-- Exportamos propiedades de mundo
		local l_World = getUserProp Lights[i] "world"
		if l_World!=undefined and l_World!="undefined" then
		(
			format " world=\"%\"" (l_World-1) to:file
		)
		
		local l_Intermittence = getUserProp Lights[i] "intermittence"
		local l_Intenstiy = getUserProp Lights[i] "varible_intensity"
		
		if l_Intermittence!=undefined and l_Intermittence!="undefined" and l_Intermittence!="" and l_Intermittence!=false then
		(
			-- Exportamos propiedades para intermitencia
			local l_tmin = getUserProp Lights[i] "tmin"
			local l_tmax = getUserProp Lights[i] "tmax"
			
			format " intermittent=\"true\"" to:file
			format " tmin=\"%\"" l_tmin to:file
			format " tmax=\"%\"" l_tmax to:file
		)
		else if l_Intenstiy!=undefined and l_Intenstiy!="undefined" and l_Intenstiy!="" and l_Intenstiy!=false then
		(
			-- Exportamos propiedades para atenuación variable
			local l_tmin = getUserProp Lights[i] "tmin"
			format " varible_intensity=\"true\"" to:file
			format " tmin=\"%\"" l_tmin to:file
		)
		
		-- Exportamos propiedades de glow
		local l_LightMesh = getUserProp Lights[i] "mesh"
		local l_LightTextureON = getUserProp Lights[i] "texture_on"
		local l_LightTextureOFF = getUserProp Lights[i] "texture_off"
		
		if l_LightMesh!=undefined and l_LightMesh!="undefined" and l_LightMesh!="" and l_LightMesh!=false then
		(
			format " light_mesh=\"%\"" l_LightMesh to:file
			
			local l_ObjName = "$" + l_LightMesh
			local l_Obj = execute (l_ObjName)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)

			local l_Layer = getUserProp l_Obj "layer"
			if l_Layer == undefined or l_Layer==false then
			(
				l_Layer = g_ddlLayers[1]
			)	
			
			format " layer=\"%\"" l_Layer to:file
		)
		
		if l_LightTextureON!=undefined and l_LightTextureON!="undefined" and l_LightTextureON!="" and l_LightTextureON!=false then
		(
			local l_TextON = g_MeshesPath+l_LightTextureON
			format " texture_on=\"%\"" l_TextON to:file
		)

		if l_LightTextureOFF!=undefined and l_LightTextureOFF!="undefined" and l_LightTextureOFF!="" and l_LightTextureOFF!=false then
		(
			local l_TextOFF = g_MeshesPath+l_LightTextureOFF
			format " texture_off=\"%\"" l_TextOFF to:file
		)		
		
		-- Exportamos propiedades para shadowmap
		local l_RenderShadows = getUserProp Lights[i] "render_shadows"
		local l_GenerateSM = getUserProp Lights[i] "generate_shadow_map"
		local l_GenerateSSM = getUserProp Lights[i] "generate_static_shadow_map"
		if classof Lights[i] == TargetDirectionallight or classof Lights[i] == targetSpot or classof Lights[i] == Omnilight then
		(
			
			if l_RenderShadows!=undefined and l_RenderShadows!="undefined" and l_RenderShadows!="" and l_RenderShadows!=false then
			(
				
				if l_GenerateSM or l_GenerateSSM then
				(
					format " render_shadows=\"%\"" l_RenderShadows to:file
					
					if l_GenerateSM then
					(
						format " generate_shadow_map=\"%\"" l_GenerateSM to:file
						local l_aux = getUserProp Lights[i] "shadow_map_format_type"
						format " shadow_map_format_type=\"%\"\n\t" l_aux to:file
						l_aux = getUserProp Lights[i] "shadow_map_width"
						format " shadow_map_width=\"%\"" l_aux to:file
						l_aux = getUserProp Lights[i] "shadow_map_height"
						format " shadow_map_height=\"%\"\n\t" l_aux to:file
					)
					
					if l_GenerateSSM then
					(
						format " generate_static_shadow_map=\"%\"" l_GenerateSSM to:file
						local l_aux = getUserProp Lights[i] "static_shadow_map_format_type"
						format " static_shadow_map_format_type=\"%\"\n\t" l_aux to:file
						l_aux = getUserProp Lights[i] "static_shadow_map_width"
						format " static_shadow_map_width=\"%\"" l_aux to:file
						l_aux = getUserProp Lights[i] "static_shadow_map_height"
						format " static_shadow_map_height=\"%\"" l_aux to:file
					)
					
					if classof Lights[i] == TargetDirectionallight then
					(
						local l_aux = getUserProp Lights[i] "ortho_shadow_map_size_width"
						format "\n\t ortho_shadow_map_size_width=\"%\"" l_aux to:file
						l_aux = getUserProp Lights[i] "ortho_shadow_map_size_height"
						format " ortho_shadow_map_size_height=\"%\"" l_aux to:file
					)
				)				
			)
		)
		if classof Lights[i] == targetSpot then
		(
			format " angle=\"%\"" Lights[i].hotspot to:file
			format " fall_off=\"%\"" Lights[i].Falloff to:file
		)
		format ">\n" to:file
		
		if classof Lights[i] == TargetDirectionallight or classof Lights[i] == targetSpot or classof Lights[i] == Omnilight then
		(
			if l_RenderShadows!=undefined and l_RenderShadows!="undefined" and l_RenderShadows!="" and l_RenderShadows!=false then
			(
				if l_GenerateSM then
				(
					local l_aux = getUserProp Lights[i] "dynamic_manager"
					format "\t\t<dynamic renderable_objects_manager=\"%\"/>\n" l_aux to:file
				)	
				if l_GenerateSSM then
				(
					local l_aux = getUserProp Lights[i] "static_manager"
					format "\t\t<static renderable_objects_manager=\"%\"/>\n" l_aux to:file
				)
			)
		)
		
		format "\t</light> \n\n" to:file
	)
	format "</Lights>\n" to:file

	close file
)

fn ExportLights DataPath Level =
(
	local l_Lights = #()
	
	local l_Objs = $*
	local l_NumObjects = l_Objs.count
	
	for i= 1 to l_NumObjects do
	(
		if l_NumObjects==1 then
			l_Object = l_Objs	
		else
			l_Object = l_Objs[i]
			
		if isLight l_Object then
		(
			--format "DEBUG> % found! Obj num: %\n" l_Object i
			append l_Lights l_Object
		)
	)

	local l_LightsPath = DataPath+"\\"+g_XMLPath+"\\"+Level
	CreateLightsXml  l_LightsPath  l_Lights
)

--ExportLights "D:\\iVaN\\UAB\\Master\\Projects\\TheOtherSide\\trunk\\Run\\Viewer\\data\\"