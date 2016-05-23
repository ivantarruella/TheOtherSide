#include "xml/XMLTreeNode.h"
#include "ActionToInput.h"
#include "InputManager.h"
#include "InputDefs.h"
#include "Logger.h"
#include "Exception.h"


bool CActionToInput::Init (CInputManager* inputManager)
{
	if (inputManager != NULL)
	{
		m_pInputManager = inputManager;
		m_bIsOk = true;
		InitString2Input();
	}
                
	return m_bIsOk;
}

bool CActionToInput::ReLoadXML()
{
	if (m_sXMLFile != "")
	{
		return LoadXML (m_sXMLFile);
	}

	return false;
}


bool CActionToInput::LoadXML(const std::string& pathFile)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(pathFile.c_str()))
	{
		std::string msg_error = "CActionToInput::LoadConfig->Error al intentar leer el archivo de Input: " + pathFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	m_sXMLFile = pathFile;

	// Obtenemos el nodo "Actions"
	CXMLTreeNode  l_ActionsNode = parser["Actions"];
	if (l_ActionsNode.Exists())
	{
		// Limpiar datos previos
		m_Actions.clear();
		
		// Obtenemos los datos para todos los action
		int l_count_actions = l_ActionsNode.GetNumChildren();
		for (int i = 0; i < l_count_actions; ++i)
		{
			const char* l_name = l_ActionsNode(i).GetPszProperty("name","");
			if (l_ActionsNode(i).Exists() && !l_ActionsNode(i).IsComment() )
			{
				// Obtenemos datos para todos los inputs de este action
				int l_count_inputs = l_ActionsNode(i).GetNumChildren();
				vInputs l_vInput; 
				for (int j = 0; j < l_count_inputs; ++j)
				{
					sInput l_input;
					l_input.DeviceType = l_ActionsNode(i)(j).GetPszProperty("deviceType","");
					l_input.EventType = l_ActionsNode(i)(j).GetPszProperty("EventType","");
					l_input.AxisType = l_ActionsNode(i)(j).GetPszProperty("AxisType","");
					l_input.Code = l_ActionsNode(i)(j).GetPszProperty("Code","");
					l_input.Delta = l_ActionsNode(i)(j).GetFloatProperty("Delta",0.0f);

					l_vInput.push_back(l_input);
				}
				
				m_Actions.insert(std::pair< std::string, vInputs>(l_name, l_vInput));
			}
		}
	}

	return true;
}

bool CActionToInput::GetCodeFromString(const std::string& input_type, uint32& code) const 
{
	std::map<std::string, uint32>::const_iterator it = m_String2Code.find(input_type);

	if (it != m_String2Code.end())
	{
		code = it->second;
		return true;
	}

	return false;
}


bool CActionToInput::DoAction (const std::string& action_name)
{
	float delta =0.0f;
	return DoAction(action_name, delta);
}

bool CActionToInput::DoAction (const std::string& action_name, float& delta)
{
	bool doAction = false;

	mActions::const_iterator it = m_Actions.find(action_name);
	if (it != m_Actions.end())
	{
		// Se ha encontrado una acción, realizamos la operación correspondiente para todos sus inputs
		vInputs l_vInputs = it->second;
		vInputs::const_iterator tmp = l_vInputs.begin();
		vInputs::const_iterator it_end = l_vInputs.end();

		for (; tmp!=it_end; tmp++)
		{
			uint32 device_type, event_type, axis_type, code;
			if (GetCodeFromString(tmp->EventType, event_type))
			{
				if (GetCodeFromString(tmp->DeviceType, device_type))
				{
					if (GetCodeFromString(tmp->Code, code))
					{
						switch (event_type)
						{
						case EVENT_DOWN:
							doAction = m_pInputManager->IsDown((INPUT_DEVICE_TYPE)device_type, code);
							break;
						case EVENT_DOWN_UP:
							doAction = m_pInputManager->IsDownUp((INPUT_DEVICE_TYPE)device_type, code);
							break;
						case EVENT_UP_DOWN:
							doAction = m_pInputManager->IsUpDown((INPUT_DEVICE_TYPE)device_type, code);
							break;
						}
					}
				}
			}
			else
			{
				if (GetCodeFromString(tmp->AxisType, axis_type)) 
				{
					float x=0.f,y=0.f;
					float right=0.f, left=0.f;
					switch (axis_type)
					{
						case AXIS_MOUSE_X:
							delta = (float)m_pInputManager->GetMouseDelta().x * tmp->Delta;
							break;
						case AXIS_MOUSE_Y:
							delta = (float)m_pInputManager->GetMouseDelta().y * tmp->Delta;
							break;
						case AXIS_MOUSE_Z:
							delta = (float)m_pInputManager->GetMouseDelta().z * tmp->Delta;
							break;
                        case AXIS_LEFT_THUMB_X:
							m_pInputManager->GetGamePadLeftThumbDeflection(&x,&y);
                            delta = x;
                            break;
                        case AXIS_LEFT_THUMB_Y:
                            m_pInputManager->GetGamePadLeftThumbDeflection(&x,&y);
                            delta = y;
                            break;
                        case AXIS_RIGHT_THUMB_X:
                            m_pInputManager->GetGamePadRightThumbDeflection(&x,&y);
                            delta = x;
                            break;
                        case AXIS_RIGHT_THUMB_Y:
                            m_pInputManager->GetGamePadRightThumbDeflection(&x,&y);
                            delta = y;
                            break;
                        case AXIS_DELTA_TRIGGER_RIGHT:
                            m_pInputManager->GetGamePadDeltaTriggers(&left, &right );
                            delta = right;
                            break;
                        case AXIS_DELTA_TRIGGER_LEFT:
                            m_pInputManager->GetGamePadDeltaTriggers(&left, &right );
                            delta = left;
                            break;
					}
					doAction = (delta != 0.0f);
				}
			}

			if (!doAction)
				break;
		}
	}

	return doAction;
}


void CActionToInput::GetActionInfo(const std::string& action_name, std::string& info, std::string& shortInfo)
{
	info.clear();
	shortInfo.clear();

	mActions::const_iterator it = m_Actions.find(action_name);
	if (it != m_Actions.end())
	{
		// Se ha encontrado una acción, realizamos la operación correspondiente para todos sus inputs
		vInputs l_vInputs = it->second;
		vInputs::const_iterator tmp = l_vInputs.begin();
		vInputs::const_iterator it_end = l_vInputs.end();

		for (; tmp!=it_end; tmp++)
		{
			uint32 event_type, axis_type;

			if (tmp != l_vInputs.begin())
			{
				shortInfo += " + ";
			}

			if ( GetCodeFromString(tmp->EventType, event_type))
			{
				shortInfo += tmp->Code;
			}
			else if (GetCodeFromString(tmp->AxisType, axis_type))
			{
				shortInfo += tmp->AxisType;
			}
		}
	}
}

void CActionToInput::InitString2Input ()
{
	typedef std::pair<std::string, uint32> PairString2Code;

	//----------------INPUT_DEVICE_TYPE---------------//
	m_String2Code.insert( PairString2Code("IDV_KEYBOARD",			  IDV_KEYBOARD) );
	m_String2Code.insert( PairString2Code("IDV_MOUSE",				  IDV_MOUSE) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD1",			  IDV_GAMEPAD1) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD2",			  IDV_GAMEPAD2) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD3",			  IDV_GAMEPAD3) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD4",			  IDV_GAMEPAD4) );
	m_String2Code.insert( PairString2Code("IDV_NOTHING",			  IDV_NOTHING) );

	//----------------INPUT_AXIS_TYPE-----------------//
	m_String2Code.insert( PairString2Code("AXIS_MOUSE_X",             AXIS_MOUSE_X) );
	m_String2Code.insert( PairString2Code("AXIS_MOUSE_Y",             AXIS_MOUSE_Y) );
	m_String2Code.insert( PairString2Code("AXIS_MOUSE_Z",             AXIS_MOUSE_Z) );
	m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_X",        AXIS_LEFT_THUMB_X) );
	m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_Y",        AXIS_LEFT_THUMB_Y) );
	m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_X",       AXIS_RIGHT_THUMB_X) );
	m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_Y",       AXIS_RIGHT_THUMB_Y) );
	m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_RIGHT", AXIS_DELTA_TRIGGER_RIGHT) );
	m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_LEFT",  AXIS_DELTA_TRIGGER_LEFT) );
	m_String2Code.insert( PairString2Code("AXIS_NOTHING",             AXIS_NOTHING) );
   	 
	//----------------INPUT_EVENT_TYPE-----------------//      	 
	m_String2Code.insert( PairString2Code("EVENT_DOWN",               EVENT_DOWN) );
	m_String2Code.insert( PairString2Code("EVENT_UP_DOWN",            EVENT_UP_DOWN) );
	m_String2Code.insert( PairString2Code("EVENT_DOWN_UP",            EVENT_DOWN_UP) );
	m_String2Code.insert( PairString2Code("EVENT_NOTHING",            EVENT_NOTHING) );

	//----------------MOUSE_BUTTON---------------------//
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_LEFT",        MOUSE_BUTTON_LEFT) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_RIGHT",       MOUSE_BUTTON_RIGHT) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE",      MOUSE_BUTTON_MIDDLE) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE",      MOUSE_BUTTON_MIDDLE) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_NOTHING",     MOUSE_BUTTON_NOTHING) );


	//----------------Gamepad codes from XInput.h------//
	m_String2Code.insert( PairString2Code("PAD_DPAD_UP",              PAD_DPAD_UP) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_DOWN",            PAD_DPAD_DOWN) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_LEFT",            PAD_DPAD_LEFT) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_RIGHT",           PAD_DPAD_RIGHT) );
	/*m_String2Code.insert( PairString2Code("PAD_DPAD_UP_LEFT",         PAD_DPAD_UP_LEFT) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_UP_RIGHT",        PAD_DPAD_UP_RIGHT) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_DOWN_LEFT",       PAD_DPAD_DOWN_LEFT) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_DOWN_RIGHT",      PAD_DPAD_DOWN_RIGHT) );*/

	m_String2Code.insert( PairString2Code("PAD_DPAD_START",           PAD_DPAD_START) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_BACK",            PAD_DPAD_BACK) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_THUMB",    PAD_BUTTON_LEFT_THUMB) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_THUMB",   PAD_BUTTON_RIGHT_THUMB) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_SHOULDER", PAD_BUTTON_LEFT_SHOULDER) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_SHOULDER",PAD_BUTTON_RIGHT_SHOULDER) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_A",             PAD_BUTTON_A) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_B",             PAD_BUTTON_B) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_X",             PAD_BUTTON_X) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_Y",             PAD_BUTTON_Y) );

	//-----------Keyboard scan codes from dinput.h ------//
	m_String2Code.insert( PairString2Code("KEY_ESCAPE",               KEY_ESCAPE) );
	m_String2Code.insert( PairString2Code("KEY_TAB",                  KEY_TAB) );
	m_String2Code.insert( PairString2Code("KEY_SPACE",                KEY_SPACE) );
	m_String2Code.insert( PairString2Code("KEY_RETURN",               KEY_RETURN) );
	m_String2Code.insert( PairString2Code("KEY_BACK",                 KEY_BACK) );
	m_String2Code.insert( PairString2Code("KEY_CAPITAL",              KEY_CAPITAL) );
	m_String2Code.insert( PairString2Code("KEY_MINUS",                KEY_MINUS) );
	m_String2Code.insert( PairString2Code("KEY_EQUALS",               KEY_EQUALS) );
	m_String2Code.insert( PairString2Code("KEY_LBRACKET",             KEY_LBRACKET) );
	m_String2Code.insert( PairString2Code("KEY_RBRACKET",             KEY_RBRACKET) );
	m_String2Code.insert( PairString2Code("KEY_SEMICOLON",            KEY_SEMICOLON) );
	m_String2Code.insert( PairString2Code("KEY_APOSTROPHE",           KEY_APOSTROPHE) );
	m_String2Code.insert( PairString2Code("KEY_GRAVE",                KEY_GRAVE) );
	m_String2Code.insert( PairString2Code("KEY_BACKSLASH",            KEY_BACKSLASH) );
	m_String2Code.insert( PairString2Code("KEY_COMMA",                KEY_COMMA) );
	m_String2Code.insert( PairString2Code("KEY_PERIOD",               KEY_PERIOD) );
	m_String2Code.insert( PairString2Code("KEY_SLASH",                KEY_SLASH) );
	m_String2Code.insert( PairString2Code("KEY_A",                    KEY_A) );
	m_String2Code.insert( PairString2Code("KEY_S",                    KEY_S) );
	m_String2Code.insert( PairString2Code("KEY_D",                    KEY_D) );
	m_String2Code.insert( PairString2Code("KEY_F",                    KEY_F) );
	m_String2Code.insert( PairString2Code("KEY_G",                    KEY_G) );
	m_String2Code.insert( PairString2Code("KEY_H",                    KEY_H) );
	m_String2Code.insert( PairString2Code("KEY_J",                    KEY_J) );
	m_String2Code.insert( PairString2Code("KEY_K",                    KEY_K) );
	m_String2Code.insert( PairString2Code("KEY_L",                    KEY_L) );
	m_String2Code.insert( PairString2Code("KEY_X",                    KEY_X) );
	m_String2Code.insert( PairString2Code("KEY_C",                    KEY_C) );
	m_String2Code.insert( PairString2Code("KEY_B",                    KEY_B) );
	m_String2Code.insert( PairString2Code("KEY_N",                    KEY_N) );
	m_String2Code.insert( PairString2Code("KEY_M",                    KEY_M) );
	m_String2Code.insert( PairString2Code("KEY_Q",                    KEY_Q) );
	m_String2Code.insert( PairString2Code("KEY_W",                    KEY_W) );
	m_String2Code.insert( PairString2Code("KEY_E",                    KEY_E) );
	m_String2Code.insert( PairString2Code("KEY_R",                    KEY_R) );
	m_String2Code.insert( PairString2Code("KEY_T",                    KEY_T) );
	m_String2Code.insert( PairString2Code("KEY_Y",                    KEY_Y) );
	m_String2Code.insert( PairString2Code("KEY_U",                    KEY_U) );
	m_String2Code.insert( PairString2Code("KEY_I",                    KEY_I) );
	m_String2Code.insert( PairString2Code("KEY_O",                    KEY_O) );
	m_String2Code.insert( PairString2Code("KEY_P",                    KEY_P) );
	m_String2Code.insert( PairString2Code("KEY_1",                    KEY_1) );
	m_String2Code.insert( PairString2Code("KEY_2",                    KEY_2) );
	m_String2Code.insert( PairString2Code("KEY_3",                    KEY_3) );
	m_String2Code.insert( PairString2Code("KEY_4",                    KEY_4) );
	m_String2Code.insert( PairString2Code("KEY_5",                    KEY_5) );
	m_String2Code.insert( PairString2Code("KEY_6",                    KEY_6) );
	m_String2Code.insert( PairString2Code("KEY_7",                    KEY_7) );
	m_String2Code.insert( PairString2Code("KEY_8",                    KEY_8) );
	m_String2Code.insert( PairString2Code("KEY_9",                    KEY_9) );
	m_String2Code.insert( PairString2Code("KEY_0",                    KEY_0) );
	m_String2Code.insert( PairString2Code("KEY_F1",                   KEY_F1) );
	m_String2Code.insert( PairString2Code("KEY_F2",                   KEY_F2) );
	m_String2Code.insert( PairString2Code("KEY_F3",                   KEY_F3) );
	m_String2Code.insert( PairString2Code("KEY_F4",                   KEY_F4) );
	m_String2Code.insert( PairString2Code("KEY_F5",                   KEY_F5) );
	m_String2Code.insert( PairString2Code("KEY_F6",                   KEY_F6) );
	m_String2Code.insert( PairString2Code("KEY_F7",                   KEY_F7) );
	m_String2Code.insert( PairString2Code("KEY_F8",                   KEY_F8) );
	m_String2Code.insert( PairString2Code("KEY_F9",                   KEY_F9) );
	m_String2Code.insert( PairString2Code("KEY_F10",                  KEY_F10) );
	m_String2Code.insert( PairString2Code("KEY_F11",                  KEY_F11) );
	m_String2Code.insert( PairString2Code("KEY_F12",                  KEY_F12) );
	m_String2Code.insert( PairString2Code("KEY_NPPLUS",               KEY_NPPLUS) );
	m_String2Code.insert( PairString2Code("KEY_NPMINUS",              KEY_NPMINUS) );
	m_String2Code.insert( PairString2Code("KEY_NPDECIMAL",            KEY_NPDECIMAL) );
	m_String2Code.insert( PairString2Code("KEY_NPCOMMA",              KEY_NPCOMMA) );
	m_String2Code.insert( PairString2Code("KEY_NPDIVIDE",             KEY_NPDIVIDE) );
	m_String2Code.insert( PairString2Code("KEY_NPMULTIPLY",           KEY_NPMULTIPLY) );
	m_String2Code.insert( PairString2Code("KEY_NPENTER",              KEY_NPENTER) );
	m_String2Code.insert( PairString2Code("KEY_NUMLOCK",              KEY_NUMLOCK) );
	m_String2Code.insert( PairString2Code("KEY_NP1",                  KEY_NP1) );
	m_String2Code.insert( PairString2Code("KEY_NP2",                  KEY_NP2) );
	m_String2Code.insert( PairString2Code("KEY_NP3",                  KEY_NP3) );
	m_String2Code.insert( PairString2Code("KEY_NP4",                  KEY_NP4) );
	m_String2Code.insert( PairString2Code("KEY_NP5",                  KEY_NP5) );
	m_String2Code.insert( PairString2Code("KEY_NP6",                  KEY_NP6) );
	m_String2Code.insert( PairString2Code("KEY_NP7",                  KEY_NP7) );
	m_String2Code.insert( PairString2Code("KEY_NP8",                  KEY_NP8) );
	m_String2Code.insert( PairString2Code("KEY_NP9",                  KEY_NP9) );
	m_String2Code.insert( PairString2Code("KEY_NP0",                  KEY_NP0) );
	m_String2Code.insert( PairString2Code("KEY_RSHIFT",               KEY_RSHIFT) );
	m_String2Code.insert( PairString2Code("KEY_LSHIFT",               KEY_LSHIFT) );
	m_String2Code.insert( PairString2Code("KEY_RCTRL",                KEY_RCTRL) );
	m_String2Code.insert( PairString2Code("KEY_LCTRL",                KEY_LCTRL) );
	m_String2Code.insert( PairString2Code("KEY_RALT",                 KEY_RALT) );
	m_String2Code.insert( PairString2Code("KEY_LALT",                 KEY_LALT) );
	m_String2Code.insert( PairString2Code("KEY_LWIN",                 KEY_LWIN) );
	m_String2Code.insert( PairString2Code("KEY_RWIN",                 KEY_RWIN) );
	m_String2Code.insert( PairString2Code("KEY_UP",                   KEY_UP) );
	m_String2Code.insert( PairString2Code("KEY_DOWN",                 KEY_DOWN) );
	m_String2Code.insert( PairString2Code("KEY_LEFT",                 KEY_LEFT) );
	m_String2Code.insert( PairString2Code("KEY_RIGHT",                KEY_RIGHT) );
	m_String2Code.insert( PairString2Code("KEY_INSERT",               KEY_INSERT) );
	m_String2Code.insert( PairString2Code("KEY_DELETE",               KEY_DELETE) );
	m_String2Code.insert( PairString2Code("KEY_HOME",                 KEY_HOME) );
	m_String2Code.insert( PairString2Code("KEY_END",                  KEY_END) );
	m_String2Code.insert( PairString2Code("KEY_PGDOWN",               KEY_PGDOWN) );
	m_String2Code.insert( PairString2Code("KEY_PGUP",                 KEY_PGUP) );
	m_String2Code.insert( PairString2Code("KEY_PAUSE",                KEY_PAUSE) );
	m_String2Code.insert( PairString2Code("KEY_SCROLL",               KEY_SCROLL) );  
}

