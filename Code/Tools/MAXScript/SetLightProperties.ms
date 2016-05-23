fileIn "functions.ms"

rollout GetSetLightProperties "GetSetLightProperties" width:579 height:558
(
	GroupBox grpRender "Light Properties" pos:[7,8] width:564 height:543
	button btnGet "Get" pos:[172,506] width:85 height:24
	button btnSet "Set" pos:[331,505] width:85 height:24
	dropdownList ddlLightWorld "" pos:[131,25] width:120 height:21 items:#("ALL WORLDS", "REAL WORLD", "MIRROR WORLD")
	GroupBox grpRSMDimension "Dinamic Shadow map" pos:[19,50] width:234 height:116
	checkbox chkGenerateShadowMap "Generate shadow map" pos:[38,69] width:128 height:14
	label lblSMFormat "Shadow map Format" pos:[40,95] width:126 height:16
	dropdownList ddlSMType "" pos:[175,93] width:70 height:21 items:#("R32F", "A8R8G8B8", "R8G8B8", "X8R8G8B8")
	label lblSMWidth "Shadow map width" pos:[40,119] width:150 height:16
	dropdownList ddlSMWidth "" pos:[176,116] width:60 height:21 items:#("128", "256", "512", "1024", "2048", "4096")
	label lblSMHeight "Shadow map height" pos:[40,143] width:150 height:16
	dropdownList ddlSMHeight "" pos:[176,140] width:60 height:21 items:#("128", "256", "512", "1024", "2048", "4096")
	GroupBox grpRStaticSM "Static Shadow map" pos:[16,173] width:238 height:132
	label lblSSMFormat "Static Shadow map Format" pos:[36,218] width:130 height:16
	dropdownList ddlSSMType "" pos:[171,215] width:70 height:21 items:#("R32F", "A8R8G8B8", "R8G8B8", "X8R8G8B8")
	checkbox chkGenerateStaticShadowMap "Generate static shadow map" pos:[32,187] width:160 height:16
	label lblSSMWidth "Static Shadow map width" pos:[38,246] width:150 height:16
	dropdownList ddlSSMWidth "" pos:[182,242] width:60 height:21 items:#("128", "256", "512", "1024", "2048", "4096")
	label lblSSMHeight "Static Shadow map height" pos:[39,273] width:150 height:16
	dropdownList ddlSSMHeight "" pos:[181,268] width:60 height:21 items:#("128", "256", "512", "1024", "2048", "4096")
	GroupBox grpOrtho "Orthogonal Shadow map (Directional Lights)" pos:[16,318] width:238 height:69
	label lblOrthoWidth "Orthogonal width" pos:[40,336] width:150 height:16
	dropdownList ddlOrthoWidth "" pos:[168,333] width:60 height:21 items:#("128", "256", "512", "1024", "2048", "4096")
	label lblOrthoHeight "Orthogonal height" pos:[39,359] width:150 height:16
	dropdownList ddlOrthoHeight "" pos:[167,357] width:60 height:21 items:#("128", "256", "512", "1024", "2048", "4096")
	label lblDynamicManager "Dynamic Shadow Manager" pos:[30,418] width:126 height:16
	dropdownList ddlDynamicManager "" pos:[167,418] width:70 height:21 items:#("solid", "solid_MR", "solid_ME", "alpha_objects", "alpha_blend_objects", "particles")
	label lblStaticManager "Static Shadow Manager" pos:[30,448] width:126 height:16
	dropdownList ddlStaticManager "" pos:[167,445] width:70 height:21 items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
	GroupBox grp21 "Shadow Manager" pos:[17,398] width:236 height:80
	GroupBox grp22 "Intermittent/Glow Properties" pos:[295,50] width:265 height:217
	checkbox chk11 "Intermittence" pos:[325,72] width:94 height:18
	label lbl43 "Min. intermittence/Attenuat. time:" pos:[322,97] width:165 height:18
	label lbl44 "Max. intermittence time:" pos:[322,127] width:127 height:19
	spinner spn3 "" pos:[485,98] width:53 height:16
	spinner spn5 "" pos:[485,126] width:52 height:16
	label lbl121 "World:" pos:[81,28] width:43 height:15
	checkbox chkAtt "Attenuation" pos:[445,72] width:98 height:17
	label lbl14 "Mesh asociado:" pos:[324,167] width:92 height:15
	label lbl15 "Label" pos:[333,217] width:0 height:0
	label lbl16 "Texture ON:" pos:[324,194] width:78 height:16
	label lbl17 "Texture OFF:" pos:[325,223] width:96 height:18
	editText edtMesh "" pos:[406,164] width:134 height:21
	editText edt2 "" pos:[439,277] width:0 height:0
	editText edtTextureON "" pos:[406,192] width:134 height:21
	editText edtTextureOFF "" pos:[406,222] width:134 height:20
	

	on btnGet pressed do
	(
		--Render Shadows
		--local l_RenderShadows = getUserProp $ "render_shadows"
		--if l_RenderShadows !=undefined then
		--(
		--	format "GET::RenderShadows = %\n" l_RenderShadows
		--	chkRenderShadows.checked = l_RenderShadows
		--)

		-- World
		local l_World = getUserProp $ "world"
		--format "GET::World = %\n" l_World
		for i=1 to ddlLightWorld.items.count do
		(
			if i == l_World then
			(
				ddlLightWorld.Selection = i
			)
		)

		--Generate Shadow Map
		local l_GenerateSM = getUserProp $ "generate_shadow_map"
		if l_GenerateSM != undefined and l_GenerateSM != "undefined" and l_GenerateSM!="" and l_GenerateSM!=false then
		(
			--format "GET::GenerateShadowMap = %\n" l_GenerateSM
			chkGenerateShadowMap.checked = l_GenerateSM
			if l_GenerateSM then
			(
				local l_SMFormat = getUserProp $ "shadow_map_format_type"
				--format "GET::ShadowMapFormat = %\n" l_SMFormat
				for i=1 to ddlSMType.items.count do
				(
					if ddlSMType.items[i] == l_SMFormat then
					(
						ddlSMType.Selection = i
					)
				)

				local l_SMWidth = getUserProp $ "shadow_map_width"
				--format "GET::ShadowMapWidth = %\n" l_SMWidth
				for i=1 to ddlSMWidth.items.count do
				(
					if ddlSMWidth.items[i] == l_SMWidth as string then
					(
						ddlSMWidth.Selection = i
					)
				)


				local l_SMHeight = getUserProp $ "shadow_map_height"
				--format "GET::ShadowMapHeight = %\n" l_SMHeight
				for i=1 to ddlSMHeight.items.count do
				(
					if ddlSMHeight.items[i] == l_SMHeight as string then
					(
						ddlSMHeight.Selection = i
					)
				)
			)
		)
		else
		(
			l_GenerateSM = false
			--format "GET::GenerateShadowMap = %\n" l_GenerateSM
			chkGenerateShadowMap.checked = l_GenerateSM
			ddlSMType.Selection = 1
			ddlSMWidth.Selection = 1
			ddlSMHeight.Selection = 1
		)
		

		--Generate Static Shadow Map
		local l_GenerateSSM = getUserProp $ "generate_static_shadow_map"
		if l_GenerateSSM != undefined and l_GenerateSSM != "undefined" and l_GenerateSSM!="" and l_GenerateSSM!=false then
		(
			--format "GET::GenerateStaticShadowMap = %\n" l_GenerateSSM
			chkGenerateStaticShadowMap.checked = l_GenerateSSM
			if l_GenerateSSM then
			(
				local l_SSMFormat = getUserProp $ "static_shadow_map_format_type"
				--format "GET::StaticShadowMapFormat = %\n" l_SSMFormat
				for i=1 to ddlSSMType.items.count do
				(
					if ddlSSMType.items[i] == l_SSMFormat then
					(
						ddlSSMType.Selection = i
					)
				)
				
				local l_SSMWidth = getUserProp $ "static_shadow_map_width"
				--format "GET::StaticShadowMapWidth = %\n" l_SSMWidth
				for i=1 to ddlSSMWidth.items.count do
				(
					if ddlSSMWidth.items[i] == l_SSMWidth as string then
					(
						ddlSSMWidth.Selection = i
					)
				)
				
				local l_SSMHeight = getUserProp $ "static_shadow_map_height"
				--format "GET::StaticShadowMapHeight = %\n" l_SSMHeight
				for i=1 to ddlSSMHeight.items.count do
				(
					if ddlSSMHeight.items[i] == l_SSMHeight as string then
					(
						ddlSSMHeight.Selection = i
					)
				)
			)
		)
		else
		(
			l_GenerateSSM = false
			--format "GET::GenerateStaticShadowMap = %\n" l_GenerateSSM
			chkGenerateStaticShadowMap.checked = l_GenerateSSM
			ddlSSMType.Selection = 1
			ddlSSMWidth.Selection = 1
			ddlSSMHeight.Selection = 1
		)
		
		--Orthogonal Shadow Map
		if classof $ == TargetDirectionallight and (l_GenerateSSM or l_GenerateSM) then
		(
			local l_OrthoWidth = getUserProp $ "ortho_shadow_map_size_width"
			if l_OrthoWidth != undefined and l_OrthoWidth != "undefined" and l_OrthoWidth!="" then
			(
				local l_OrthoHeight = getUserProp $ "ortho_shadow_map_size_height"
				if l_OrthoHeight != undefined and l_OrthoHeight != "undefined" and l_OrthoHeight!="" then
				(
					--format "GET::OrthoShadowMapWidth = %\n" l_OrthoWidth
					--format "GET::OrthoShadowMapHeight = %\n" l_OrthoHeight
					for i=1 to ddlOrthoWidth.items.count do
					(
						if ddlOrthoWidth.items[i] == l_OrthoWidth as string then
						(
							ddlOrthoWidth.Selection = i
						)
					)
					for i=1 to ddlOrthoHeight.items.count do
					(
						if ddlOrthoHeight.items[i] == l_OrthoHeight as string then
						(
							ddlOrthoHeight.Selection = i
						)
					)
				)
			)
		)
		else
		(
			ddlOrthoWidth.Selection = 1
			ddlOrthoHeight.Selection = 1
		)
		
		--Dynamic/Static Manager
		local l_DynamicManager = getUserProp $ "dynamic_manager"
		if l_DynamicManager != undefined and l_DynamicManager != "undefined" and l_DynamicManager!="" and l_GenerateSM then
		(
			--format "GET::DinamicManager = %\n" l_DynamicManager
			for i=1 to ddlDynamicManager.items.count do
			(
				if ddlDynamicManager.items[i] == l_DynamicManager then
				(
					ddlDynamicManager.Selection = i
				)
			)
		)
		else
		(
			ddlDynamicManager.Selection = 1
		)
		
		local l_StaticManager = getUserProp $ "static_manager"
		if l_StaticManager != undefined and l_StaticManager != "undefined" and l_StaticManager!="" and l_GenerateSSM then
		(
			--format "GET::StaticManager = %\n" l_StaticManager
			for i=1 to ddlStaticManager.items.count do
			(
				if ddlStaticManager.items[i] == l_StaticManager then
				(
					ddlStaticManager.Selection = i
				)
			)
		)
		else
		(
			ddlStaticManager.Selection = 1
		)
		
		-- Intermittence and intensity parameters
		local l_Intermittence = getUserProp $ "intermittence"
		--format "GET::Intermittence = %\n" l_Intermittence
		if (l_Intermittence==true and l_Intermittence != undefined and l_Intermittence != "undefined" and l_Intermittence!="")  then
		(
			chk11.checked = true
			chkAtt.checked =false
			local l_tmin = getUserProp $ "tmin"
			local l_tmax = getUserProp $ "tmax"
			--format "GET::TMin = %\n" l_tmin
			--format "GET::TMax = %\n" l_tmax
			if l_tmin != undefined and l_tmin != "undefined" then
			(
				spn3.value = l_tmin
			)
			if l_tmax != undefined and l_tmax != "undefined" then
			(
				spn5.value = l_tmax
			)
		)
		else
		(
			chk11.checked = false
			local l_Intensity = getUserProp $ "varible_intensity"
			--format "GET::Intensity = %\n" l_Intensity
			if (l_Intensity==true and l_Intensity != undefined and l_Intensity != "undefined" and l_Intensity!="") then
			(
				chkAtt.checked = true
				local l_tmin = getUserProp $ "tmin"
				--format "GET::TMin = %\n" l_tmin
				if l_tmin != undefined and l_tmin != "undefined" then
				(
					spn3.value = l_tmin
				)
			)
			else
			(
				chkAtt.checked = false
			)			
		)
		
		-- Glow parameters
		local l_LightMesh = getUserProp $ "mesh"
		local l_LightTextureON = getUserProp $ "texture_on"
		local l_LightTextureOFF = getUserProp $ "texture_off"
		
		if (l_LightMesh != undefined and l_LightMesh != "undefined" and l_LightMesh!="")  then
		(
			edtMesh.text = (l_LightMesh as string)
		)
		if (l_LightTextureON != undefined and l_LightTextureON != "undefined" and l_LightTextureON!="")  then
		(
			edtTextureON.text = (l_LightTextureON as string)
		)
		if (l_LightTextureOFF != undefined and l_LightTextureOFF != "undefined" and l_LightTextureOFF!="")  then
		(
			edtTextureOFF.text = (l_LightTextureOFF as string)
		)
	)


	on btnSet pressed do
	(
		--format "SET::RenderShadows = %\n" chkRenderShadows.checked
		setUserProp $ "render_shadows" false

		-- World
		local l_World = ddlLightWorld.Selection
		--format "SET::World = %\n" ddlLightWorld.items[l_World]
		setUserProp $ "world" l_World
		
		-- Export if we render shadows
		--if chkRenderShadows.checked then
		--(
		--format "SET::GenerateShadowMap = %\n" chkGenerateShadowMap.checked
		setUserProp $ "generate_shadow_map" chkGenerateShadowMap.checked
		if chkGenerateShadowMap.checked then
		(
			--format "SET::RenderShadows = %\n" true
			setUserProp $ "render_shadows" true
				
			local l_SMFormat = ddlSMType.items[ddlSMType.Selection]
			--format "SET::ShadowMapFormat = %\n" l_SMFormat
			setUserProp $ "shadow_map_format_type" l_SMFormat

			local l_SMWidth= ddlSMWidth.items[ddlSMWidth.Selection]
			--format "SET::ShadowMapWidth = %\n" l_SMWidth
			setUserProp $ "shadow_map_width" l_SMWidth

			local l_SMHeight = ddlSMHeight.items[ddlSMHeight.Selection]
			--format "SET::ShadowMapHeight = %\n" l_SMHeight
			setUserProp $ "shadow_map_height" l_SMHeight
		)
		else
		(
			setUserProp $ "shadow_map_format_type" undefined
			setUserProp $ "shadow_map_width" undefined
			setUserProp $ "shadow_map_height" undefined
		)

			
		--format "SET::GenerateStaticShadowMap = %\n" chkGenerateStaticShadowMap.checked
		setUserProp $ "generate_static_shadow_map" chkGenerateStaticShadowMap.checked
		if chkGenerateStaticShadowMap.checked then
		(
			--format "SET::RenderShadows = %\n" true
			setUserProp $ "render_shadows" true
				
			local l_SSMFormat = ddlSSMType.items[ddlSSMType.Selection]
			--format "SET::StaticShadowMapFormat = %\n" l_SSMFormat
			setUserProp $ "static_shadow_map_format_type" l_SSMFormat

			local l_SSMWidth= ddlSSMWidth.items[ddlSSMWidth.Selection]
			--format "SET::StaticShadowMapWidth = %\n" l_SSMWidth
			setUserProp $ "static_shadow_map_width" l_SSMWidth

			local l_SSMHeight = ddlSSMHeight.items[ddlSSMHeight.Selection]
			--format "SET::StaticShadowMapHeight = %\n" l_SSMHeight
			setUserProp $ "static_shadow_map_height" l_SSMHeight
		)
		else
		(
			setUserProp $ "static_shadow_map_format_type" undefined
			setUserProp $ "static_shadow_map_width" undefined
			setUserProp $ "static_shadow_map_height" undefined
		)

			
		local l_RenderShadows = getUserProp $ "render_shadows"
		if classof $ == TargetDirectionallight and l_RenderShadows then
		(
			local l_OrthoWidth= ddlOrthoWidth.items[ddlOrthoWidth.Selection]
			--format "SET::OrthogonalShadowMapWidth = %\n" l_OrthoWidth
			setUserProp $ "ortho_shadow_map_size_width" l_OrthoWidth

			local l_OrthoHeight = ddlOrthoHeight.items[ddlOrthoHeight.Selection]
			--format "SET::OrthogonalShadowMapHeight = %\n" l_OrthoHeight
			setUserProp $ "ortho_shadow_map_size_height" l_OrthoHeight
		)

		if l_RenderShadows then
		(
			local l_DynamicManager = ddlDynamicManager.items[ddlDynamicManager.Selection]
			--format "SET::DynamicManager = %\n" l_DynamicManager
			setUserProp $ "dynamic_manager" l_DynamicManager

			local l_StaticManager = ddlStaticManager.items[ddlStaticManager.Selection]
			--format "SET::StaticManager = %\n" l_StaticManager
			setUserProp $ "static_manager" l_StaticManager
		)		
		else
		(
			setUserProp $ "shadow_map_format_type" undefined
			setUserProp $ "shadow_map_width" undefined
			setUserProp $ "shadow_map_height" undefined
			setUserProp $ "static_shadow_map_format_type" undefined
			setUserProp $ "static_shadow_map_width" undefined
			setUserProp $ "static_shadow_map_height" undefined
			setUserProp $ "dynamic_manager" undefined
			setUserProp $ "static_manager" undefined
			setUserProp $ "generate_shadow_map" undefined
			setUserProp $ "generate_static_shadow_map" undefined
			setUserProp $ "ortho_shadow_map_size_width" undefined
			setUserProp $ "ortho_shadow_map_size_height" undefined
		)

		--format "SET::Intermittence = %\n" chk11.checked
		--format "SET::Intensity = %\n" chkAtt.checked
		setUserProp $ "intermittence" chk11.checked
		setUserProp $ "varible_intensity" chkAtt.checked

		if (chk11.checked or chkAtt.checked) then
		(
			--format "SET::TMin = %\n" spn3.value
			setUserProp $ "tmin" spn3.value
			--format "SET::TMax = %\n" spn5.value
			setUserProp $ "tmax" spn5.value			
		)
		
		local l_LightMesh = edtMesh.text
		local l_LightTextureON = edtTextureON.text
		local l_LightTextureOFF = edtTextureOFF.text
		setUserProp $ "mesh" l_LightMesh	
		setUserProp $ "texture_on" l_LightTextureON	
		setUserProp $ "texture_off" 	l_LightTextureOFF
		
		
		destroyDialog GetSetLightProperties
	)
)

--createDialog GetSetLightProperties