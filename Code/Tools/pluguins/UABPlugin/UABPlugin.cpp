#include "maxscrpt/maxscrpt.h"
#include "MAXScrpt/Numbers.h"
#include "maxscrpt/definsfn.h"

#include <vector>
#include <map>

//def_visible_primitive( HelloWorld, "hello_world");
def_visible_primitive( UABMapClean, "uab_map_clean");
def_visible_primitive( UABMapAdd, "uab_map_add");

/*Value* HelloWorld_cf(Value **arg_list, int count)
{
	::MessageBox(NULL, "Hello World", "MAX Script Function", MB_OK);
	return &ok;
}*/

typedef std::map<std::vector<float>, int> MapVtxs;
std::vector<MapVtxs> g_MapVtxsByMaterial;

Value* UABMapClean_cf(Value** arg_list, int count)
{
	//Borraremos los elementos del mapa
	g_MapVtxsByMaterial.clear();
	return &true_value;
}

/*
Esta función recibirá dos parámetros
1 : Array de floats que contendrá el vértice
2 : Integer que será el Id del Material
Devolverá el índice del vértice según el mapa, si no existe el vértice lo introducirá en el mapa
*/
Value* UABMapAdd_cf(Value** arg_list, int count)
{
	//Contenido de la función

	check_arg_count(UABMapAdd, 2, count);

	type_check(arg_list[0], Array, "uab_map_add");
	type_check(arg_list[1], Integer, "uab_map_add");

	Value *v1 = arg_list[ 0 ];
	Value *v2 = arg_list[ 1 ];

	int l_IdMaterial=v2->to_int();

	Array *vtx=(Array *)v1;
	//Convierte el primer parámetro a Array
	std::vector<float> l_Vtx;
	for(int i=0;i<vtx->size;++i)
	{
		Value *l_Value=vtx->get(1+i);
		float l_ValueFloat=l_Value->to_float();
		l_Vtx.push_back(l_ValueFloat);
	}

	int l_Idx=0;

	while(g_MapVtxsByMaterial.size()<=l_IdMaterial)
		g_MapVtxsByMaterial.push_back(MapVtxs());

	MapVtxs::iterator l_It=g_MapVtxsByMaterial[l_IdMaterial].find(l_Vtx);
	if(l_It==g_MapVtxsByMaterial[l_IdMaterial].end())
	{
		l_Idx=g_MapVtxsByMaterial[l_IdMaterial].size();
		g_MapVtxsByMaterial[l_IdMaterial][l_Vtx]=l_Idx;
	}
	else	
		l_Idx=l_It->second;

	
	return Integer::intern(l_Idx);
}
