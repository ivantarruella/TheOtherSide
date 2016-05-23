#include "SoundManager.h"
#include "Logger.h"
#include "XML\XMLTreeNode.h"
#include "Exception.h"

using namespace std; 

bool CSoundManager::Init()
{
	LOGGER->AddNewLog(ELL_INFORMATION,"CSoundManager::Intentamos iniciar el sonido");
	m_bIsOk = _initAL();

	if(!m_bIsOk)
	{
		LOGGER->AddNewLog(ELL_ERROR,"CSoundManager::Error al iniciar el sonido");
		Release();
        throw CException(__FILE__, __LINE__, "CSoundManager::Error al iniciar el sonido");
	}
	else{
		LOGGER->AddNewLog(ELL_INFORMATION,"CSoundManager::Sonido iniciado correctamente");
	}
	return true;
}

void CSoundManager::Done()
{
	if (IsOk())
    {
		Release();
        m_bIsOk = false;
	}
}

void CSoundManager::RegisterFunctions(CScriptManager* scriptManager)
{

}

void CSoundManager::Update(float deltaTime)
{	
	////Esta función servirá para actualizar los lerp animation
	/*std::map<uint32,CLerpAnimator1D>::reverse_iterator rev_it = m_SourcesFadeInOut.rbegin();
	std::map<uint32,CLerpAnimator1D>::reverse_iterator rev_itEnd = m_SourcesFadeInOut.rend();

	for(; rev_it != rev_itEnd; ++rev_it)
	{
		float val = 0.0f;
		if (rev_it->second.Update(deltaTime, val))
		{
			std::map<uint32,CLerpAnimator1D>::iterator it = m_SourcesFadeInOut.find(rev_it->first);
			if(it!= m_SourcesFadeInOut.end())
			{
				m_SourcesFadeInOut.erase(it);
				if(rev_it == m_SourcesFadeInOut.rend())
					--rev_it;
			}
		}
		else
		{
			SetSourceGain(rev_it->first, val);
		}
	}*/

	std::vector<uint32> l_finished;
	std::map<uint32,CLerpAnimator1D>::iterator it = m_SourcesFadeInOut.begin();
	std::map<uint32,CLerpAnimator1D>::iterator itEnd	= m_SourcesFadeInOut.end();
	for(; it != itEnd; ++it)
	{
		float value = 0.f;
		if (it->second.Update(deltaTime,value) )
		{
			//ha acabado de realizar el FadeIn/Out--> sacar del map
			l_finished.push_back(it->first);
		}
		SetSourceGain(it->first,value);
	}
	
	for(uint32 cont = 0; cont < l_finished.size(); cont++)
	{
		std::map<uint32,CLerpAnimator1D>::iterator it = m_SourcesFadeInOut.find(l_finished[cont]);
		if (it != m_SourcesFadeInOut.end())
		{
			m_SourcesFadeInOut.erase(	it );
		}
	}
}

bool CSoundManager::LoadSounds(const std::string& xmlSoundsFile)
{
	_clear();
	m_bIsOk=true;
	//ponemos un clear al principio, por si nos interesa hacer un reload
	CXMLTreeNode parser;
	if (!parser.LoadFile(xmlSoundsFile.c_str()))
	{
		m_bIsOk=false;
		std::string msg_error = "CSoundManager::Load->Error al intentar leer el archivo: " + xmlSoundsFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m_Path = xmlSoundsFile;


	int l_count_objects;
	// Obtenemos el nodo "Sounds"
	CXMLTreeNode  l_SoundManager = parser["Sounds"];
	if (l_SoundManager.Exists())
	{
		tAction l_Action;
		tSoundFile l_Path;
		tIdBuffer l_IdBuffer;

		l_count_objects = l_SoundManager.GetNumChildren();
		for (int i = 0; i < l_count_objects; ++i){


			//Sound Node
			l_Action=l_SoundManager(i).GetPszProperty("soundId","");
			l_Path=l_SoundManager(i).GetPszProperty("path","");

			//Miramos hay un sonido con el mismo nombre
			map<tAction, tIdBuffer>::iterator it=m_Buffers.find(l_Action);
			if(it==m_Buffers.end()){
				//No esta registrada la accion
				//Vemos si el sonido esta cargado
				map<tSoundFile,tIdBuffer>::iterator	it_LoadedFiles=m_LoadedFiles.find(m_Path);
				if(it_LoadedFiles!=m_LoadedFiles.end()){
					//Ya está cargado el sonido
					l_IdBuffer=it_LoadedFiles->second;
					m_Buffers.insert(pair<tAction, tIdBuffer>(l_Path,l_IdBuffer));

				}else{
					//No está cargado el sonido
					if(_loadSound(l_Path,l_IdBuffer)){

						m_Buffers.insert(pair<tAction, tIdBuffer>(l_Action,l_IdBuffer));
						m_LoadedFiles.insert(pair<tSoundFile,tIdBuffer>(l_Path,l_IdBuffer));

					}else{

						std::string msg_error = "CSoundManager::Load->Error al intentar cargar el sonido: " + l_Path;
						LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());

					}
				}
			}
		}
	}

	return m_bIsOk;
}

void CSoundManager::Reset()
{
	Stop();
	_clear();
}

void CSoundManager::Pause()
{
	ALint state;
	for(unsigned int i=0; i<m_Sources.size(); i++)
	{
		alGetSourcei( m_Sources[i].m_uSource, AL_SOURCE_STATE, &state);
		if(state==AL_PLAYING)
			alSourcePause(m_Sources[i].m_uSource);
		if(state==AL_PAUSED)
			alSourcePlay(m_Sources[i].m_uSource);
	}
}

void CSoundManager::Stop()
{
	for(unsigned int i=0; i<m_Sources.size(); i++)
	{
		alSourceStop(m_Sources[i].m_uSource);
	}
}

void CSoundManager::SetGain(float gain)
{
	alListenerf(AL_GAIN,gain);
}

float CSoundManager::GetGain()
{
	float l_gain;
	alGetListenerf(AL_GAIN,&l_gain);
	return l_gain;
}

bool CSoundManager::PlayAction2D(const std::string& action, float volume)
{
	if (m_bSoundON)
	{	
		if (volume < 0.0f) volume = 0.0f;
		else if (volume > 1.0f) volume = 1.0f;

		std::map<tAction, tIdBuffer>::iterator it = m_Buffers.find(action);
		if (it==m_Buffers.end())
		{
			std::string error = "Action " + action + " no encontrada!\n";
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlayAction2D : %s", error.c_str());
			return false;
		}

		tIdBuffer idBuf = (*it).second;

		int index = _getSource(false);
		if (index == -1)
		{
			std::string error = "ERROR: No se puede obtener un nuevo source!\n";
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlayAction2D : %s", error.c_str());
			return false;
		}

		//Hacemos play de un buffer en un source 
		ALfloat pos[3]={0.0f, 0.0f, 0.0f};
		alSourcei (m_Sources[index].m_uSource, AL_BUFFER, idBuf);
		alSourcei (m_Sources[index].m_uSource, AL_SOURCE_RELATIVE, AL_TRUE);
		alSourcef (m_Sources[index].m_uSource, AL_PITCH, 1.0);
		alSourcef (m_Sources[index].m_uSource, AL_GAIN, volume);
		alSourcefv(m_Sources[index].m_uSource, AL_POSITION, pos);
		alSourcefv(m_Sources[index].m_uSource, AL_VELOCITY, pos);
		alSourcei (m_Sources[index].m_uSource, AL_LOOPING, AL_FALSE/AL_TRUE);
		alSourcePlay(m_Sources[index].m_uSource);
	}
	return true;
}

bool CSoundManager::PlayAction3D(const std::string& action, const Vect3f& position)
{
	if (m_bSoundON)
	{	
		std::map<tAction, tIdBuffer>::iterator it = m_Buffers.find(action);
		if (it==m_Buffers.end())
		{
			std::string error = "Action " + action + " no encontrada!\n";
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlayAction3D : %s", error.c_str());
			return false;
		}

		tIdBuffer idBuf = (*it).second;

		int index = _getSource(false);
		if (index == -1)
		{
			std::string error = "ERROR: No se puede obtener un nuevo source!\n";
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlayAction3D : %s", error.c_str());
			return false;
		}

		//Hacemos play de un buffer en un source
		ALfloat pos[3]={position.x, position.y, position.z};
		ALfloat vel[3]={0.0f, 0.0f, 0.0f};
		alSourcei (m_Sources[index].m_uSource, AL_BUFFER, idBuf);
		alSourcef (m_Sources[index].m_uSource, AL_PITCH, 1.0);
		alSourcef (m_Sources[index].m_uSource, AL_GAIN, 1.0);
		alSourcefv(m_Sources[index].m_uSource, AL_POSITION, pos);
		alSourcefv(m_Sources[index].m_uSource, AL_VELOCITY, vel);
		alSourcei (m_Sources[index].m_uSource, AL_LOOPING, AL_FALSE/AL_TRUE);
		alSourcePlay(m_Sources[index].m_uSource);
	}
	return true;
}


uint32 CSoundManager::CreateSource()
{
	return _getSource(true);
}

bool CSoundManager::DeleteSource(uint32 source)
{

	if(source < m_Sources.size() && m_Sources[source].m_bReserved)
	{
		StopSource(source);
		m_Sources[source].m_bReserved=false;
	}
	return true;
}

bool CSoundManager::PlaySource2D(uint32 source, const std::string& action, bool loop)
{

	if(m_bSoundON)
	{
		if(source < m_Sources.size() && m_Sources[source].m_bReserved)
		{
			ALboolean alloop;

			if(loop)
				alloop = AL_TRUE;
			else
				alloop = AL_FALSE;

			StopSource(source);

			if(m_Buffers.find(action) == m_Buffers.end())
			{
				std::string description = "Action " + action + " not found";
				LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlaySource2D %s", description.c_str());
				return false;
			}

			ALfloat vector[3] = {0.0f, 0.0f, 0.0f};
			alSourcei(m_Sources[source].m_uSource, AL_BUFFER, m_Buffers[action]);
			alSourcei(m_Sources[source].m_uSource, AL_SOURCE_RELATIVE, AL_TRUE);
			alSourcef(m_Sources[source].m_uSource, AL_PITCH, 1.0);
			alSourcef(m_Sources[source].m_uSource, AL_GAIN, 1.0);
			alSourcefv(m_Sources[source].m_uSource, AL_POSITION, vector);
			alSourcefv(m_Sources[source].m_uSource, AL_VELOCITY, vector);
			alSourcefv(m_Sources[source].m_uSource, AL_DIRECTION, vector);
			alSourcei(m_Sources[source].m_uSource, AL_LOOPING, alloop);
			alSourcePlay(m_Sources[source].m_uSource);
			return true;
		}
	}
	else{
		std::string descrìption ="Can't play source. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", descrìption.c_str());
	}

	return false;
}

bool CSoundManager::PlaySource3D(uint32 source, const std::string& action, bool loop)
{

	//Hay q hacer un set position del source antes de llamar al Play y en el Update tb del Listener
	if(m_bSoundON)
	{
		if(source < m_Sources.size() && m_Sources[source].m_bReserved)
		{
			ALboolean alloop;

		 if(loop)
				alloop = AL_TRUE;
			else
				alloop = AL_FALSE;

			StopSource(source);

			if(m_Buffers.find(action) == m_Buffers.end())
			{
				std::string description = "Action " + action + " not found";
				LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::PlaySource3D %s", description.c_str());
				return false;
			}

			alSourcei(m_Sources[source].m_uSource, AL_BUFFER, m_Buffers[action]);
			alSourcef(m_Sources[source].m_uSource, AL_PITCH, 1.0);
			alSourcef(m_Sources[source].m_uSource, AL_GAIN, 1.0);
			alSourcei(m_Sources[source].m_uSource, AL_LOOPING, alloop);
			alSourcePlay(m_Sources[source].m_uSource);
			return true;
		}
	}
	else{
		std::string descrìption ="Can't play source. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", descrìption.c_str());
	}

	return false;
}

bool CSoundManager::PauseSource(uint32 source)
{

	ALint state; 	

	if(source < m_Sources.size() && m_Sources[source].m_bReserved)
	{
		alGetSourcei( m_Sources[source].m_uSource, AL_SOURCE_STATE, &state);
		if(state == AL_PLAYING)
			alSourcePause(m_Sources[source].m_uSource);
		if(state == AL_PAUSED)
			alSourcePlay(m_Sources[source].m_uSource);			
		return true;
	}
	

	return false;
}

bool CSoundManager::StopSource(uint32 source)
{

	if(source < m_Sources.size() && m_Sources[source].m_bReserved)
	{
		alSourceStop(m_Sources[source].m_uSource);			
		return true;
	}	

	return false;
}

bool CSoundManager::SetSourceGain(uint32 source, float inGain)
{

	if(source < m_Sources.size() && m_Sources[source].m_bReserved)
	{
		alSourcef(m_Sources[source].m_uSource, AL_GAIN, inGain);
		return true;
	}
	

	return false;
}

bool CSoundManager::GetSourceGain(uint32 source, float& outGain)
{

	if(source < m_Sources.size() && m_Sources[source].m_bReserved)
	{
		alGetSourcef(m_Sources[source].m_uSource, AL_GAIN, &outGain);
		return true;
	}
	

	return false;
}

bool CSoundManager::SetSourcePosition(uint32 source, const Vect3f& inPosition)
{

	if(source < m_Sources.size() && m_Sources[source].m_bReserved)
	{
		alSource3f(m_Sources[source].m_uSource, AL_POSITION, inPosition.x, inPosition.y, inPosition.z);
		return true;
	}
	

	return false;
}

bool CSoundManager::GetSourcePosition(uint32 source, Vect3f& outPosition)
{

	if(source < m_Sources.size() && m_Sources[source].m_bReserved)
	{
		alGetSource3f(m_Sources[source].m_uSource, AL_POSITION, &outPosition.x, &outPosition.y, &outPosition.z);
		return true;
	}
	

	return false;
}

bool CSoundManager::SetSourceVelocity(uint32 source, const Vect3f& inVelocity)
{

	if(source < m_Sources.size() && m_Sources[source].m_bReserved)
	{
		alSource3f(m_Sources[source].m_uSource, AL_VELOCITY, inVelocity.x, inVelocity.y, inVelocity.z);
		return true;
	}
	

	return false;
}

bool CSoundManager::GetSourceVelocity(uint32 source, Vect3f& outVelocity)
{

	if(source < m_Sources.size() && m_Sources[source].m_bReserved)
	{
		Vect3f kk;
		alGetSource3f(m_Sources[source].m_uSource, AL_VELOCITY, &kk.x, &kk.y, &kk.z);
		outVelocity = kk;
		return true;
	}
	

	return false;
}

bool CSoundManager::FadeInSource(uint32 source, float totalTime, float finalGain, ETypeFunction type)
{
	bool isOK = false;
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		//Primero de todo miramos que no haya ya un FadeIn/Out del source en cuestion:
		if(m_SourcesFadeInOut.find(source) == m_SourcesFadeInOut.end() )
		{
			CLerpAnimator1D animator;
			animator.SetValues(0.f,finalGain,totalTime,type);
			m_SourcesFadeInOut.insert(std::pair<uint32,CLerpAnimator1D> (source,animator) );
			isOK = true;
		}
		else
		{
			std::string description = "Ya se esta realizando un FadeIn/Out con el source" + source;
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
			isOK =  false;
		}
	}
	else
	{
		std::string description = "Can't do FadeIn with source. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		isOK =  false;
	}
	return isOK;
}

bool CSoundManager::FadeOutSource(uint32 source, float totalTime, ETypeFunction type)
{
	bool isOK = false;
	if (source<m_Sources.size() && m_Sources[source].m_bReserved)
	{
		//Primero de todo miramos que no haya ya un FadeIn/Out del source en cuestion:
		if(m_SourcesFadeInOut.find(source) == m_SourcesFadeInOut.end() )
		{
			CLerpAnimator1D animator;
			float currentGain = 0.f;
			GetSourceGain(source, currentGain);
			animator.SetValues(currentGain,0.f,totalTime,type);
			m_SourcesFadeInOut.insert(std::pair<uint32,CLerpAnimator1D> (source,animator) );
			isOK = true;
		}
		else
		{
			std::string description = "Ya se esta realizando un FadeIn/Out con el source" + source;
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
			isOK =  false;
		}
	}
	else
	{
		std::string description = "Can't do FadeOut with source. Invalid source " + source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		isOK =  false;
	}
	return isOK;
}


//-----LISTENER FUNCTIONS-------------------------
void CSoundManager::SetListenerPosition(const Vect3f& inPosition)
{

	ALfloat pos[3];
	pos[0] = inPosition.x;
	pos[1] = inPosition.y;
	pos[2] = inPosition.z;
	alListenerfv(AL_POSITION, pos); 

}

void CSoundManager::GetListenerPosition(Vect3f& outPosition)

{	
	ALfloat pos[3];
	alGetListenerfv(AL_POSITION, pos);
	outPosition.x = pos[0];
	outPosition.y = pos[1];
	outPosition.z = pos[2];

}

void CSoundManager::SetListenerVelocity(const Vect3f& inVelocity)
{

	ALfloat vel[3]; 
	vel[0] = inVelocity.x;
	vel[1] = inVelocity.y;
	vel[2] = inVelocity.z;	
	alListenerfv(AL_VELOCITY, vel);

}

void CSoundManager::GetListenerVelocity(Vect3f& outVelocity)
{

	ALfloat vel[3];
	alGetListenerfv(AL_VELOCITY, vel);
	outVelocity.x = vel[0];
	outVelocity.y = vel[1];
	outVelocity.z = vel[2];

}

void CSoundManager::SetListenerOrientation(const Vect3f& inAt, const Vect3f& inUp)
{

	float ori[6]; 
	ori[0] = inAt.x;
	ori[1] = inAt.y;
	ori[2] = inAt.z;
	ori[3] = inUp.x;
	ori[4] = inUp.y;
	ori[5] = inUp.z;
	alListenerfv(AL_ORIENTATION, ori);

}

void CSoundManager::GetListenerOrientation(Vect3f& outAt, Vect3f& outUp)

{	
	float ori[6];
	alGetListenerfv(AL_ORIENTATION, ori);
	outAt.x = ori[0];
	outAt.y = ori[1];
	outAt.z = ori[2];
	outUp.x = ori[3];
	outUp.y = ori[4];
	outUp.z = ori[5];

}

void CSoundManager::Release()
{
	_finalizeAL();
}

bool CSoundManager::_initAL()
{
	ALenum error;
	ALCdevice* pDevice;
	ALCcontext* pContext;

	// Get handle to default device.
	pDevice = alcOpenDevice(NULL);

	// Get the device specifier.
	//const ALCubyte* deviceSpecifier = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);

	// Create audio context.
	pContext = alcCreateContext(pDevice, NULL);

	// Set active context.
	alcMakeContextCurrent(pContext);

	// Check for an error.
	if ((error=alcGetError(pDevice)) != ALC_NO_ERROR)
	{
		std::string description = "Can't create openAL context (" + _getALErrorString(error) + ")";
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
	return true;
}

void CSoundManager::_finalizeAL()
{
	_clear();

	ALCcontext* pCurContext;
	ALCdevice* pCurDevice;

	// Get the current context.
	pCurContext = alcGetCurrentContext();

	// Get the device used by that context.
	pCurDevice = alcGetContextsDevice(pCurContext);

	// Reset the current context to NULL.
	alcMakeContextCurrent(NULL);

	// Release the context and the device.
	alcDestroyContext(pCurContext);
	alcCloseDevice(pCurDevice);
}

bool CSoundManager::_loadSound(const std::string& file, tIdBuffer& buffer)
{
	// Variables to load into.
	FILE *fd;
	ALenum format;
	ALenum error;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;


	// Load wav data into buffers.
	alGenBuffers(1, &buffer);

	if((error=alGetError()) != AL_NO_ERROR)
	{    
		alDeleteBuffers(1,&buffer);
		std::string description = "Error: Can't create openAL Buffer (" + _getALErrorString(error)  + ")";
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;    
	}

	// Check if the file exists
	if ((fd=fopen(file.c_str(),"r"))==NULL)
	{
		alDeleteBuffers(1,&buffer);
		std::string description = "Error: Can't open file " + file;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}
	else
	{
		fclose(fd);
	}
	alutLoadWAVFile((ALbyte*)file.c_str(), &format, &data, &size, &freq, &loop);
	alBufferData(buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);
	if ((error=alGetError()) != AL_NO_ERROR)
	{        
		alDeleteBuffers(1,&buffer);
		std::string description = "Error: Can't load sound file " + file + " (" + _getALErrorString(error)  + ")";
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		return false;
	}        
	return true;
}

void CSoundManager::_clear()
{
	// Delete AL objets
	ALuint aux;    
	std::map<tAction, tIdBuffer>::iterator itBuffer;
	for( itBuffer = m_Buffers.begin(); itBuffer != m_Buffers.end(); ++itBuffer )
	{
		aux = (*itBuffer).second;
		alDeleteBuffers (1,&aux);
	}

	std::vector<tInfoSource>::iterator itSource;
	for( itSource = m_Sources.begin(); itSource != m_Sources.end(); ++itSource )
	{
		aux = (*itSource).m_uSource;
		alDeleteSources (1,&aux);
	}

	m_Buffers.clear();
	m_Sources.clear();
	m_LoadedFiles.clear();
}

int  CSoundManager::_getSource(bool reserved)
{
	// Find a unused source
	uint32 i=0;
	bool found=false;
	ALint state;
	ALenum error;

	for( unsigned int i=0; i < m_Sources.size(); i++)
	{
		if (!m_Sources[i].m_bReserved)
		{
			// Check if it is being used
			alGetSourcei(m_Sources[i].m_uSource,AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING && state != AL_PAUSED)
			{
				m_Sources[i].m_bReserved = reserved;
				return i;
			}            
		}
	}

	tInfoSource info;
	alGenSources(1,&info.m_uSource);
	if ((error=alGetError())!=AL_NO_ERROR)
	{
		std::string description = "Can't create source (" + _getALErrorString(error) + ")";
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
		i=-1;
	}
	else
	{
		info.m_bReserved=reserved;
		m_Sources.push_back(info);
		i =(uint32) m_Sources.size() - 1;            
	}

	return i;
}

std::string CSoundManager::_getALErrorString(ALenum err)
{
	switch(err)
	{
	case AL_NO_ERROR:
		return std::string("AL_NO_ERROR");
		break;

	case AL_INVALID_NAME:
		return std::string("AL_INVALID_NAME");
		break;
	case AL_INVALID_ENUM:
		return std::string("AL_INVALID_ENUM");
		break;
	case AL_INVALID_VALUE:
		return std::string("AL_INVALID_VALUE");
		break;
	case AL_INVALID_OPERATION:
		return std::string("AL_INVALID_OPERATION");
		break;
	case AL_OUT_OF_MEMORY:
		return std::string("AL_OUT_OF_MEMORY");
		break;
	default:
		return std::string("JORLS!");
	};
}

