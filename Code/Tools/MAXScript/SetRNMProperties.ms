--filein "KGFunctions.ms" quiet:true

rollout KGSetRNMProperties "KGSetRNMProperties" width:312 height:184
(
	button btnRNMX "X" pos:[16,16] width:24 height:24
	button btnRNMY "Y" pos:[16,48] width:24 height:24
	button btnRNMZ "Z" pos:[17,80] width:23 height:24
	button btnCubemapSpecular "S" pos:[16,112] width:24 height:24
	button btnGenerateX "Generate" pos:[240,17] width:56 height:24
	button btnGenerateY "Generate" pos:[240,48] width:56 height:24
	button btnGenerateZ "Generate" pos:[240,80] width:56 height:24
	button btnGetRNMsXYZ "GetRNMXYZ" pos:[16,144] width:128 height:24
	button btnSetRNMsXYZ "SetRNMXYZ" pos:[168,144] width:128 height:24
	editText edtRNMX "" pos:[48,16] width:184 height:24 enabled:false
	editText edtRNMY "" pos:[48,48] width:184 height:24 enabled:false
	editText edtRNMZ "" pos:[48,80] width:184 height:24 enabled:false
	editText edtRNMSpecular "" pos:[48,112] width:184 height:24 enabled:false
	
	function CanObjectUseRNM Obj =
	(
		if Obj!=undefined and ((classof Obj)==Editable_Mesh or (classof Obj)==Editable_Poly) then
			return true
		else 
			return false
	)
	function GenerateRNMMaterial _color _Material =
	(
		_Material.bumpMapAmount=100
		_Material.bumpMap=normal_bump()
		_Material.bumpMap.method=3	--World Method
		_Material.bumpMap.map2on=false		--Additional Bump no activo
		_Material.bumpMap.normal_map=Checker()	--Crea el checker

		_Material.bumpMap.normal_map.color1=_color
		_Material.bumpMap.normal_map.color2=_color
	)
	function GenerateRNM _color PlaneStr =
	(
		if (CanObjectUseRNM  $) then
		(
			local l_Material=$.material
			if (classof l_Material)==Multimaterial then
			(
				for i=1 to l_Material.count do
				(
					GenerateRNMMaterial _color l_Material[i]
				)
			)
			else if (classof l_Material)==Shell_Material then
			(
				for i=1 to l_Material.originalMaterial.materialList.count do
				(
					GenerateRNMMaterial _color l_Material.originalMaterial.materialList[i]
				)
			)
			else			
				GenerateRNMMaterial _color l_Material
			--KGShowInfo ("You can generate RNM "+PlaneStr+" on Render to Texture Menu")
		)
		--else
			--KGShowInfo "You must select a correct object to create its RNM"
	)
	on btnGenerateX pressed do
	(
		GenerateRNM (color 231 127 201) "X"
	)
	on btnGenerateY pressed do
	(
		GenerateRNM (color 75 217 201) "Y"
	)
	on btnGenerateZ pressed do
	(
		GenerateRNM (color 75 37 201) "Z"
	)
	function SetRNM editBox PlaneStr =
	(
		local l_FileName=selectBitMap()
		--local l_FileName=getOpenFileName caption:("Load RNM "+PlaneStr)
		if l_FileName!=undefined then
		(
			editBox.text=l_FileName.filename
			return true
		)
		return false
	)
	on btnRNMX pressed do
	(
		SetRNM edtRNMX "X"
	)
	on btnRNMY pressed do
	(
		SetRNM edtRNMY "Y"
	)
	on btnRNMZ pressed do
	(
		SetRNM edtRNMZ "Z"
	)
	on btnCubemapSpecular pressed do
	(
		SetRNM edtRNMSpecular "Spcular cube map"
	)
	function CreateTextureBitmap Filename =
	(
		local l_BitmapTexture=bitmapTexture()
		l_BitmapTexture.bitmap=openBitMap Filename
		return l_BitmapTexture
	)
	function SetRNMsXYZOnMaterial _Material =
	(
		_Material.selfIllumMap=Multi_Sub_Map()
		_Material.selfIllumMap.num_colors=4 --3 ó 4 (3 lightmpas + 1 cubempa specular)
		_Material.selfIllumMap.id_0_color_shader=(CreateTextureBitmap edtRNMX.text)
		_Material.selfIllumMap.id_1_color_shader=(CreateTextureBitmap edtRNMY.text)
		_Material.selfIllumMap.id_2_color_shader=(CreateTextureBitmap edtRNMZ.text)
		_Material.selfIllumMap.id_3_color_shader=(CreateTextureBitmap edtRNMSpecular.text)
		
		_Material.selfIllumMap.id_0_color_shader.coords.mapChannel = 2
		_Material.selfIllumMap.id_1_color_shader.coords.mapChannel = 2
		_Material.selfIllumMap.id_2_color_shader.coords.mapChannel = 2
		_Material.selfIllumMap.id_3_color_shader.coords.mapChannel = 2
	)
	on btnSetRNMsXYZ pressed do
	(
		if (CanObjectUseRNM  $) then
		(
			local l_Material=$.material
			if (classof l_Material)==Multimaterial then
			(
				for i=1 to l_Material.count do
				(
					SetRNMsXYZOnMaterial l_Material[i]
				)
			)
			else if (classof l_Material)==Shell_Material then
			(
				for i=1 to l_Material.originalMaterial.materialList.count do
				(
					SetRNMsXYZOnMaterial l_Material.originalMaterial.materialList[i]
				)
			)
			else
				SetRNMsXYZOnMaterial l_Material
		)
	)
	function GetRNM _BitmapTexture editBox =
	(
		if (classof _BitmapTexture)==BitmapTexture and (classof _BitmapTexture.bitmap)==bitmap then
		(
			editBox.text=_BitmapTexture.bitmap.filename
		)
	)
	on btnGetRNMsXYZ pressed do
	(
		if (CanObjectUseRNM  $) then
		(
			local l_Material=$.material
			if (classof l_Material)==Multimaterial then
				l_Material=l_Material[1]
			else if (classof l_Material)==Shell_Material then
				l_Material=l_Material.originalMaterial.materialList[1]
			
			if l_Material!=undefined and (classof l_Material.selfIllumMap)==Multi_Sub_Map then
			(
				GetRNM l_Material.selfIllumMap.id_0_color_shader edtRNMX --textura 0, es un mapa
				GetRNM l_Material.selfIllumMap.id_1_color_shader edtRNMY --textura 1, es un mapa
				GetRNM l_Material.selfIllumMap.id_2_color_shader edtRNMZ --textura 2, es un mapa
				GetRNM l_Material.selfIllumMap.id_3_color_shader edtRNMSpecular --textura 3, es un mapa
			)
		)
	)
)

createDialog KGSetRNMProperties