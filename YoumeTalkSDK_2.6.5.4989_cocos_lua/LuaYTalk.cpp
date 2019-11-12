#include "LuaYTalk.h"
//#include "json/document.h"
#include<IYouMeVoiceEngine.h>

#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/CCLuaStack.h"
#include "scripting/lua-bindings/manual/CCLuaValue.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

//#include "json/writer.h"
//#include "json/stringbuffer.h"
USING_NS_CC;


int  EVENT_YTALK_EVENT=0;
int  EVENT_YTALK_RestAPI = 0;
int  EVENT_YTALK_MemberChange = 0;
int  EVENT_YTALK_Broadcast = 0;

bool g_FirstVisit = true;

//监听初始化完成情况
void cocos2d::TalkClass::onEvent (const YouMeEvent event, const YouMeErrorCode error, const char * channel, const char * param)
{
    std::string strChannel = channel;
    std::string strParam = param;

    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)TalkClass::getInstance(), (ScriptHandlerMgr::HandlerType)EVENT_YTALK_EVENT);
        if (0 != handler) {
            LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
            if (NULL == pStack) {
                return;
            }
            
            lua_State *tolua_s = pStack->getLuaState();
            if (NULL == tolua_s) {
                return;
            }
       
            pStack->pushInt(event);
            pStack->pushInt(error);
            pStack->pushString( strChannel.c_str() );
            pStack->pushString( strParam.c_str() );

            pStack->executeFunctionByHandler(handler, 4);
            pStack->clean();
        }
    });
}

//远端pcm数据回调
void cocos2d::TalkClass::onPcmDataRemote(int channelNum, int samplingRateHz, int bytesPerSample, void* data, int dataSizeInByte)
{
    //todo:暂时不暴露
}

//本地录音pcm数据回调
void cocos2d::TalkClass::onPcmDataRecord(int channelNum, int samplingRateHz, int bytesPerSample, void* data, int dataSizeInByte)
{
    //todo:暂时不暴露
}

//远端pcm和本地录音pcm的混合pcm数据回调
void cocos2d::TalkClass::onPcmDataMix(int channelNum, int samplingRateHz, int bytesPerSample, void* data, int dataSizeInByte)
{
    //todo:暂时不暴露
}

void cocos2d::TalkClass::onRequestRestAPI( int requestID, const YouMeErrorCode &iErrorCode, const char* strQuery, const  char*  strResult )
{
    std::string query = strQuery;
    std::string result = strResult;
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)TalkClass::getInstance(), (ScriptHandlerMgr::HandlerType)EVENT_YTALK_RestAPI);
        if (0 != handler) {
            LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
            if (NULL == pStack) {
                return;
            }
            
            lua_State *tolua_s = pStack->getLuaState();
            if (NULL == tolua_s) {
                return;
            }
            
            pStack->pushInt(requestID);
            pStack->pushInt(iErrorCode);
            pStack->pushString( query.c_str() );
            pStack->pushString( result.c_str() );
            
            pStack->executeFunctionByHandler(handler, 4);
            pStack->clean();
        }
    });
}

void cocos2d::TalkClass::onMemberChange( const char* channel, const char* listMemberChange , bool bUpdate )
{
    std::string strChannel = channel;
    std::string memberChangeJsonStr = listMemberChange;

    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)TalkClass::getInstance(), (ScriptHandlerMgr::HandlerType)EVENT_YTALK_MemberChange);
        if (0 != handler) {
            LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
            if (NULL == pStack) {
                return;
            }
            
            lua_State *tolua_s = pStack->getLuaState();
            if (NULL == tolua_s) {
                return;
            }
            
            pStack->pushString( strChannel.c_str() );
            pStack->pushString( memberChangeJsonStr.c_str() );
            pStack->pushBoolean( bUpdate );
            
            pStack->executeFunctionByHandler(handler, 3);
            pStack->clean();
        }
    });
}

void cocos2d::TalkClass::onBroadcast(const YouMeBroadcast bc, const  char* channel,
    const  char* param1, const  char* param2, const char* content)
{
	if (EVENT_YTALK_Broadcast == 0){
		return;
	}
	
	int nbc = bc;
    std::string strChannel = channel;
    std::string strParam1 = param1;
    std::string strParam2 = param2;
    std::string strContent = content;
	
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
		int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)TalkClass::getInstance(), (ScriptHandlerMgr::HandlerType)EVENT_YTALK_Broadcast);
		if (0 != handler) {
			LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
			if (NULL == pStack) {
				return;
			}

			lua_State *tolua_s = pStack->getLuaState();
			if (NULL == tolua_s) {
				return;
			}

			pStack->pushInt(nbc);
			pStack->pushString(strChannel.c_str());
			pStack->pushString(strParam1.c_str());
			pStack->pushString(strParam2.c_str());
			pStack->pushString(strContent.c_str());
			
			pStack->executeFunctionByHandler(handler, 5);
			pStack->clean();
		}
	});	
}


TalkClass::TalkClass(){
    
}

TalkClass::~TalkClass(){
    
}

TalkClass* cocos2d::TalkClass::getInstance()
{
    static TalkClass* s_instance = NULL;
    if (NULL == s_instance)
    {
        s_instance = new TalkClass;
    }
    return s_instance;
}

static int tolua_Cocos2d_YTalk_create(lua_State* tolua_S)
{
    TalkClass *wSocket = TalkClass::getInstance();
    
    tolua_pushusertype(tolua_S, (void*)wSocket, "cc.YTalk");
    
    return 1;
}


static int tolua_Cocos2d_YTalk_init(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszAppKey = ((const char*)tolua_tostring(tolua_S, 2, 0));
    const char *pszAppSec = ((const char*)tolua_tostring(tolua_S, 3, 0));
    int serverRegionID = lua_tointeger(tolua_S, 4);
    const char *pszExtRegionName = ((const char*)tolua_tostring(tolua_S, 5, 0));


    int iErrorcode = IYouMeVoiceEngine::getInstance()->init(self, pszAppKey, pszAppSec,
                                                            (YOUME_RTC_SERVER_REGION)serverRegionID, pszExtRegionName);
    
    // IYouMeVoiceEngine::getInstance()->setPcmCallback( self, true );
    IYouMeVoiceEngine::getInstance()->setRestApiCallback( self );
    IYouMeVoiceEngine::getInstance()->setMemberChangeCallback( self );
    IYouMeVoiceEngine::getInstance()->setNotifyCallback(self);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_unInit(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int iErrorcode = IYouMeVoiceEngine::getInstance()->unInit();
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setToken(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pToken = ((const char*)tolua_tostring(tolua_S, 2, 0));

    IYouMeVoiceEngine::getInstance()->setToken( pToken  );
    return 0;
}

static int tolua_Cocos2d_YTalk_setServerRegion(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int region = lua_tointeger(tolua_S, 2);
    const char *pszExtRegionName = ((const char*)tolua_tostring(tolua_S, 3, 0));
    bool bAppend = lua_toboolean(tolua_S, 4);
    
    IYouMeVoiceEngine::getInstance()->setServerRegion((YOUME_RTC_SERVER_REGION)region,
                                                      pszExtRegionName, bAppend);
    return 0;
}


static int tolua_Cocos2d_YTalk_setOutputToSpeaker(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool bOutputToSpeaker = lua_toboolean(tolua_S, 2);
    
    int iErrorcode = IYouMeVoiceEngine::getInstance()->setOutputToSpeaker(bOutputToSpeaker);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setSpeakerMute(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool bOn = lua_toboolean(tolua_S, 2);
    IYouMeVoiceEngine::getInstance()->setSpeakerMute(bOn);
    return 0;
}

static int tolua_Cocos2d_YTalk_setMicrophoneMute(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool bOn = lua_toboolean(tolua_S, 2);
    IYouMeVoiceEngine::getInstance()->setMicrophoneMute(bOn);
    return 0;
}

static int tolua_Cocos2d_YTalk_setUseMobileNetworkEnabled(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool bOn = lua_toboolean(tolua_S, 2);
    IYouMeVoiceEngine::getInstance()->setUseMobileNetworkEnabled(bOn);
    return 0;
}

static int tolua_Cocos2d_YTalk_setVolume(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int vol = lua_tointeger(tolua_S, 2);
    
    //接口原型void setVolume (const unsigned int &uiVolume)
    IYouMeVoiceEngine::getInstance()->setVolume(vol);
    
    return 0;
}

static int tolua_Cocos2d_YTalk_setBackgroundMusicVolume(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int vol = lua_tointeger(tolua_S, 2);
    int iErrorcode = IYouMeVoiceEngine::getInstance()->setBackgroundMusicVolume( vol);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_getBackgroundMusicVolume(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int vol = IYouMeVoiceEngine::getInstance()->getBackgroundMusicVolume();
    lua_pushinteger(tolua_S, vol);
    return 1;
}

static int tolua_Cocos2d_YTalk_setHeadsetMonitorOn(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool bMicEnable = lua_toboolean(tolua_S, 2);
    bool bBgmEnable = lua_toboolean(tolua_S, 3);
    IYouMeVoiceEngine::getInstance()->setHeadsetMonitorOn(bMicEnable, bBgmEnable);
    return 0;
}

static int tolua_Cocos2d_YTalk_setReverbEnabled(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool bEnabled = lua_toboolean(tolua_S, 2);
    IYouMeVoiceEngine::getInstance()->setReverbEnabled(bEnabled);
    return 0;
}

static int tolua_Cocos2d_YTalk_setVadCallbackEnabled(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool bEnabled = lua_toboolean(tolua_S, 2);
    IYouMeVoiceEngine::getInstance()->setVadCallbackEnabled(bEnabled);
    return 0;
}

static int tolua_Cocos2d_YTalk_setAutoSendStatus(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool bEnabled = lua_toboolean(tolua_S, 2);
    IYouMeVoiceEngine::getInstance()->setAutoSendStatus(bEnabled);
    return 0;
}


static int tolua_Cocos2d_YTalk_getSpeakerMute(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool result = IYouMeVoiceEngine::getInstance()->getSpeakerMute();
    lua_pushboolean(tolua_S, result);
    return 1;
}

static int tolua_Cocos2d_YTalk_getMicrophoneMute(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool result = IYouMeVoiceEngine::getInstance()->getMicrophoneMute();
    lua_pushboolean(tolua_S, result);
    return 1;
}

static int tolua_Cocos2d_YTalk_getUseMobileNetworkEnabled(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool result = IYouMeVoiceEngine::getInstance()->getUseMobileNetworkEnabled();
    lua_pushboolean(tolua_S, result);
    return 1;
}


static int tolua_Cocos2d_YTalk_getVolume(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int vol = IYouMeVoiceEngine::getInstance()->getVolume();
    lua_pushinteger(tolua_S, vol);
    return 1;
}


static int tolua_Cocos2d_YTalk_joinChannelSingleMode(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszUserID = ((const char*)tolua_tostring(tolua_S, 2, 0));
    const char *pszRoomID = ((const char*)tolua_tostring(tolua_S, 3, 0));
    int roleType = lua_tointeger(tolua_S, 4);
    bool bCheckRoomExist = lua_toboolean(tolua_S, 5);

    int iErrorcode = IYouMeVoiceEngine::getInstance()->joinChannelSingleMode( pszUserID, pszRoomID, (YouMeUserRole_t)roleType, bCheckRoomExist);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_joinChannelMultiMode(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszUserID = ((const char*)tolua_tostring(tolua_S, 2, 0));
    const char *pszRoomID = ((const char*)tolua_tostring(tolua_S, 3, 0));
    int roleType = lua_tointeger(tolua_S, 4);
    bool bCheckRoomExist = lua_toboolean(tolua_S, 5);
    
    int iErrorcode = IYouMeVoiceEngine::getInstance()->joinChannelMultiMode( pszUserID, pszRoomID, (YouMeUserRole_t)roleType, bCheckRoomExist);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_speakToChannel(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszRoomID = ((const char*)tolua_tostring(tolua_S, 2, 0));
    int iErrorcode = IYouMeVoiceEngine::getInstance()->speakToChannel(pszRoomID);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}


static int tolua_Cocos2d_YTalk_leaveChannelMultiMode(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszRoomID = ((const char*)tolua_tostring(tolua_S, 2, 0));
    int iErrorcode = IYouMeVoiceEngine::getInstance()->leaveChannelMultiMode(pszRoomID);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_leaveChannelAll(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int iErrorcode = IYouMeVoiceEngine::getInstance()->leaveChannelAll();
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setWhiteUserList(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszChannelID = ((const char*)tolua_tostring(tolua_S, 2, 0));
    std::string strWhiteUserList = tolua_tostring(tolua_S, 3, 0);

    std::vector<std::string> vecWhiteUserList;
    std::string::size_type pos1, pos2;
    pos2 = strWhiteUserList.find_first_of('|');
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        if(pos2-pos1) {
            vecWhiteUserList.push_back(strWhiteUserList.substr(pos1, pos2 - pos1));
        }
        pos1 = pos2 + 1;
        pos2 = strWhiteUserList.find_first_of('|', pos1);
    }
    if (pos1 != strWhiteUserList.length())
        vecWhiteUserList.push_back(strWhiteUserList.substr(pos1));

    
    int iErrorcode = IYouMeVoiceEngine::getInstance()->setWhiteUserList( pszChannelID, vecWhiteUserList);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setUserRole(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int userRole = lua_tointeger(tolua_S, 2);
    int iErrorcode = IYouMeVoiceEngine::getInstance()->setUserRole( (YouMeUserRole_t)userRole );
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_getUserRole(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int userRole = (int)(IYouMeVoiceEngine::getInstance()->getUserRole());
    lua_pushinteger(tolua_S, userRole);
    return 1;
}

static int tolua_Cocos2d_YTalk_getChannelUserList(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszRoomID = ((const char*)tolua_tostring(tolua_S, 2, 0));
    int maxCount = lua_tointeger(tolua_S, 3);
    bool notifyMemChange = lua_toboolean(tolua_S, 4);
    
    int iErrorcode = IYouMeVoiceEngine::getInstance()->getChannelUserList( pszRoomID, maxCount, notifyMemChange );
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setOtherMicMute(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszUserID = ((const char*)tolua_tostring(tolua_S, 2, 0));
    bool mute = lua_toboolean(tolua_S, 3);
    
    int iErrorcode = IYouMeVoiceEngine::getInstance()->setOtherMicMute( pszUserID, mute);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setOtherSpeakerMute(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszUserID = ((const char*)tolua_tostring(tolua_S, 2, 0));
    bool mute = lua_toboolean(tolua_S, 3);
    
    int iErrorcode = IYouMeVoiceEngine::getInstance()->setOtherSpeakerMute( pszUserID, mute);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setListenOtherVoice(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszUserID = ((const char*)tolua_tostring(tolua_S, 2, 0));
    bool bOn = lua_toboolean(tolua_S, 3);

    int iErrorcode = IYouMeVoiceEngine::getInstance()->setListenOtherVoice( pszUserID, bOn);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}


static int tolua_Cocos2d_YTalk_playBackgroundMusic(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszFilePath = ((const char*)tolua_tostring(tolua_S, 2, 0));
    bool bRepeat = tolua_toboolean(tolua_S, 3, 0);
    int iErrorcode = IYouMeVoiceEngine::getInstance()->playBackgroundMusic(pszFilePath,bRepeat );
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_stopBackgroundMusic(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int iErrorcode = IYouMeVoiceEngine::getInstance()->stopBackgroundMusic();
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_pauseChannel(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int iErrorcode = IYouMeVoiceEngine::getInstance()->pauseChannel();
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_resumeChannel(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int iErrorcode = IYouMeVoiceEngine::getInstance()->resumeChannel();
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_getSoundtouchPitchSemiTones(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int pitchSemiTones = (int)(IYouMeVoiceEngine::getInstance()->getSoundtouchPitchSemiTones());
    lua_pushinteger(tolua_S, pitchSemiTones);
    return 1;
}

static int tolua_Cocos2d_YTalk_setSoundtouchPitchSemiTones(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    double pitchSemiTones = lua_tonumber(tolua_S, 2);
    int iErrorcode = IYouMeVoiceEngine::getInstance()->setSoundtouchPitchSemiTones((float)pitchSemiTones);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setMicLevelCallback(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int maxLevel = lua_tointeger(tolua_S, 2);
    
    int iErrorcode = IYouMeVoiceEngine::getInstance()->setMicLevelCallback( maxLevel );
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setReleaseMicWhenMute(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool enabled = lua_toboolean(tolua_S,2);
    
    int iErrorcode = IYouMeVoiceEngine::getInstance()->setReleaseMicWhenMute( enabled);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setExitCommModeWhenHeadsetPlugin(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    bool enabled = lua_toboolean(tolua_S,2);
    
    int iErrorcode = IYouMeVoiceEngine::getInstance()->setExitCommModeWhenHeadsetPlugin( enabled);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

static int tolua_Cocos2d_YTalk_setRecordingTimeMs(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int timeMs = lua_tointeger(tolua_S, 2);
    
    IYouMeVoiceEngine::getInstance()->setRecordingTimeMs( timeMs );
    return 0;
}

static int tolua_Cocos2d_YTalk_setPlayingTimeMs(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int timeMs = lua_tointeger(tolua_S, 2);
    
    IYouMeVoiceEngine::getInstance()->setPlayingTimeMs( timeMs );
    return 0;
}

static int tolua_Cocos2d_YTalk_requestRestApi(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszCommand = ((const char*)tolua_tostring(tolua_S, 2, 0));
    const char *pszQuery = ((const char*)tolua_tostring(tolua_S, 3, 0));
    
    int requestID = 0;
    int iErrorcode = IYouMeVoiceEngine::getInstance()->requestRestApi( pszCommand , pszQuery, &requestID  );
    
    if( iErrorcode < 0 ){
        lua_pushinteger(tolua_S, iErrorcode);
    }
    else{
        lua_pushinteger(tolua_S, requestID);
    }
    return 1;
}

static int tolua_Cocos2d_YTalk_sendMessage(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszChannelID = ((const char*)tolua_tostring(tolua_S, 2, 0));
    const char *pszContent = ((const char*)tolua_tostring(tolua_S, 3, 0));
    int requestID = 0;
    int iErrorcode = IYouMeVoiceEngine::getInstance()->sendMessage(pszChannelID, pszContent, &requestID );
    if( iErrorcode < 0 ){
        lua_pushinteger(tolua_S, iErrorcode);
    }
    else{
        lua_pushinteger(tolua_S, requestID);
    }
    return 1;
}

static int tolua_Cocos2d_YTalk_kickOtherFromChannel(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    const char *pszUser = ((const char*)tolua_tostring(tolua_S, 2, 0));
    const char *pszChannel = ((const char*)tolua_tostring(tolua_S, 3, 0));
    int lastTime = lua_tointeger(tolua_S, 4);
    int iErrorcode = IYouMeVoiceEngine::getInstance()->kickOtherFromChannel(pszUser, pszChannel,lastTime);
    lua_pushinteger(tolua_S, iErrorcode);
    return 1;
}

int tolua_Cocos2d_YTalk_setGrabMicOption(lua_State* tolua_S)
{
    int argc = 0;
    IYouMeVoiceEngine* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"YouMeTalk",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (IYouMeVoiceEngine*)tolua_tousertype(tolua_S,1,0);
    
    cobj = IYouMeVoiceEngine::getInstance();
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_Cocos2d_YTalk_setGrabMicOption'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 5)
    {
        const char* arg0;
        int arg1;
        int arg2;
        int arg3;
        unsigned int arg4;
        
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "YouMeTalk:setGrabMicOption"); arg0 = arg0_tmp.c_str();
        
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "YouMeTalk:setGrabMicOption");
        
        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "YouMeTalk:setGrabMicOption");
        
        ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3, "YouMeTalk:setGrabMicOption");
        
        ok &= luaval_to_uint32(tolua_S, 6,&arg4, "YouMeTalk:setGrabMicOption");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'tolua_Cocos2d_YTalk_setGrabMicOption'", nullptr);
            return 0;
        }
        int ret = cobj->setGrabMicOption(arg0, arg1, arg2, arg3, arg4);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "YouMeTalk:setGrabMicOption",argc, 5);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_Cocos2d_YTalk_setGrabMicOption'.",&tolua_err);
#endif
    
    return 0;

}

int tolua_Cocos2d_YTalk_requestGrabMic(lua_State* tolua_S)
{
    int argc = 0;
    IYouMeVoiceEngine* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"YouMeTalk",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (IYouMeVoiceEngine*)tolua_tousertype(tolua_S,1,0);
    
    cobj = IYouMeVoiceEngine::getInstance();
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_Cocos2d_YTalk_requestGrabMic'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 4)
    {
        const char* arg0;
        int arg1;
        bool arg2;
        const char* arg3;
        
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "YouMeTalk:requestGrabMic"); arg0 = arg0_tmp.c_str();
        
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "YouMeTalk:requestGrabMic");
        
        ok &= luaval_to_boolean(tolua_S, 4,&arg2, "YouMeTalk:requestGrabMic");
        
        std::string arg3_tmp; ok &= luaval_to_std_string(tolua_S, 5, &arg3_tmp, "YouMeTalk:requestGrabMic"); arg3 = arg3_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'tolua_Cocos2d_YTalk_requestGrabMic'", nullptr);
            return 0;
        }
        int ret = cobj->requestGrabMic(arg0, arg1, arg2, arg3);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "YouMeTalk:requestGrabMic",argc, 4);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_Cocos2d_YTalk_requestGrabMic'.",&tolua_err);
#endif
    
    return 0;
}

int tolua_Cocos2d_YTalk_startGrabMicAction(lua_State* tolua_S)
{
    int argc = 0;
    IYouMeVoiceEngine* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"YouMeTalk",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (IYouMeVoiceEngine*)tolua_tousertype(tolua_S,1,0);
    
    cobj = IYouMeVoiceEngine::getInstance();
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_Cocos2d_YTalk_startGrabMicAction'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 2)
    {
        const char* arg0;
        const char* arg1;
        
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "YouMeTalk:startGrabMicAction"); arg0 = arg0_tmp.c_str();
        
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "YouMeTalk:startGrabMicAction"); arg1 = arg1_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'tolua_Cocos2d_YTalk_startGrabMicAction'", nullptr);
            return 0;
        }
        int ret = cobj->startGrabMicAction(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "YouMeTalk:startGrabMicAction",argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_Cocos2d_YTalk_startGrabMicAction'.",&tolua_err);
#endif
    
    return 0;
}

int tolua_Cocos2d_YTalk_stopGrabMicAction(lua_State* tolua_S)
{
    int argc = 0;
    IYouMeVoiceEngine* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"YouMeTalk",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (IYouMeVoiceEngine*)tolua_tousertype(tolua_S,1,0);
    
    cobj = IYouMeVoiceEngine::getInstance();
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_Cocos2d_YTalk_stopGrabMicAction'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 2)
    {
        const char* arg0;
        const char* arg1;
        
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "YouMeTalk:stopGrabMicAction"); arg0 = arg0_tmp.c_str();
        
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "YouMeTalk:stopGrabMicAction"); arg1 = arg1_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'tolua_Cocos2d_YTalk_stopGrabMicAction'", nullptr);
            return 0;
        }
        int ret = cobj->stopGrabMicAction(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "YouMeTalk:stopGrabMicAction",argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_Cocos2d_YTalk_stopGrabMicAction'.",&tolua_err);
#endif
    
    return 0;

}

int tolua_Cocos2d_YTalk_releaseGrabMic(lua_State* tolua_S)
{
    int argc = 0;
    IYouMeVoiceEngine* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"YouMeTalk",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (IYouMeVoiceEngine*)tolua_tousertype(tolua_S,1,0);
    
    cobj = IYouMeVoiceEngine::getInstance();
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_Cocos2d_YTalk_releaseGrabMic'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        const char* arg0;
        
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "YouMeTalk:releaseGrabMic"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'tolua_Cocos2d_YTalk_releaseGrabMic'", nullptr);
            return 0;
        }
        int ret = cobj->releaseGrabMic(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "YouMeTalk:releaseGrabMic",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_Cocos2d_YTalk_releaseGrabMic'.",&tolua_err);
#endif
    
    return 0;
}

int tolua_Cocos2d_YTalk_setInviteMicOption(lua_State* tolua_S)
{
    int argc = 0;
    IYouMeVoiceEngine* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"YouMeTalk",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (IYouMeVoiceEngine*)tolua_tousertype(tolua_S,1,0);
    
    cobj = IYouMeVoiceEngine::getInstance();
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_Cocos2d_YTalk_setInviteMicOption'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 3)
    {
        const char* arg0;
        int arg1;
        int arg2;
        
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "YouMeTalk:setInviteMicOption"); arg0 = arg0_tmp.c_str();
        
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "YouMeTalk:setInviteMicOption");
        
        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "YouMeTalk:setInviteMicOption");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'tolua_Cocos2d_YTalk_setInviteMicOption'", nullptr);
            return 0;
        }
        int ret = cobj->setInviteMicOption(arg0, arg1, arg2);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "YouMeTalk:setInviteMicOption",argc, 3);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_Cocos2d_YTalk_setInviteMicOption'.",&tolua_err);
#endif
    
    return 0;

}

int tolua_Cocos2d_YTalk_stopInviteMic(lua_State* tolua_S)
{
    int argc = 0;
    IYouMeVoiceEngine* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"YouMeTalk",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (IYouMeVoiceEngine*)tolua_tousertype(tolua_S,1,0);
    
    cobj = IYouMeVoiceEngine::getInstance();
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_Cocos2d_YTalk_stopInviteMic'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'tolua_Cocos2d_YTalk_stopInviteMic'", nullptr);
            return 0;
        }
        int ret = cobj->stopInviteMic();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "YouMeTalk:stopInviteMic",argc, 0);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_Cocos2d_YTalk_stopInviteMic'.",&tolua_err);
#endif
    
    return 0;
}

int tolua_Cocos2d_YTalk_responseInviteMic(lua_State* tolua_S)
{
    int argc = 0;
    IYouMeVoiceEngine* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"YouMeTalk",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (IYouMeVoiceEngine*)tolua_tousertype(tolua_S,1,0);
    
    cobj = IYouMeVoiceEngine::getInstance();
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_Cocos2d_YTalk_responseInviteMic'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 3)
    {
        const char* arg0;
        bool arg1;
        const char* arg2;
        
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "YouMeTalk:responseInviteMic"); arg0 = arg0_tmp.c_str();
        
        ok &= luaval_to_boolean(tolua_S, 3,&arg1, "YouMeTalk:responseInviteMic");
        
        std::string arg2_tmp; ok &= luaval_to_std_string(tolua_S, 4, &arg2_tmp, "YouMeTalk:responseInviteMic"); arg2 = arg2_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'tolua_Cocos2d_YTalk_responseInviteMic'", nullptr);
            return 0;
        }
        int ret = cobj->responseInviteMic(arg0, arg1, arg2);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "YouMeTalk:responseInviteMic",argc, 3);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_Cocos2d_YTalk_responseInviteMic'.",&tolua_err);
#endif
    
    return 0;
}

int tolua_Cocos2d_YTalk_requestInviteMic(lua_State* tolua_S)
{
    int argc = 0;
    IYouMeVoiceEngine* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"YouMeTalk",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (IYouMeVoiceEngine*)tolua_tousertype(tolua_S,1,0);
    
    cobj = IYouMeVoiceEngine::getInstance();
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_Cocos2d_YTalk_requestInviteMic'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 3)
    {
        const char* arg0;
        const char* arg1;
        const char* arg2;
        
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "YouMeTalk:requestInviteMic"); arg0 = arg0_tmp.c_str();
        
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "YouMeTalk:requestInviteMic"); arg1 = arg1_tmp.c_str();
        
        std::string arg2_tmp; ok &= luaval_to_std_string(tolua_S, 4, &arg2_tmp, "YouMeTalk:requestInviteMic"); arg2 = arg2_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'tolua_Cocos2d_YTalk_requestInviteMic'", nullptr);
            return 0;
        }
        int ret = cobj->requestInviteMic(arg0, arg1, arg2);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "YouMeTalk:requestInviteMic",argc, 3);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_Cocos2d_YTalk_requestInviteMic'.",&tolua_err);
#endif
    
    return 0;
}

static int tolua_Cocos2d_YTalk_releaseMicSync (lua_State *tolua_S)
{
    TalkClass *self = (TalkClass *)tolua_tousertype (tolua_S, 1, 0);
    bool result = IYouMeVoiceEngine::getInstance ()->releaseMicSync ();
    lua_pushboolean (tolua_S, result);
    return 1;
}

static int tolua_Cocos2d_YTalk_resumeMicSync (lua_State *tolua_S)
{
    TalkClass *self = (TalkClass *)tolua_tousertype (tolua_S, 1, 0);
    bool result = IYouMeVoiceEngine::getInstance ()->resumeMicSync ();
    lua_pushboolean (tolua_S, result);
    return 1;
}


static ScriptHandlerMgr::HandlerType _bindLuaFunction(lua_State *tolua_S, int index, void *cobj)
{
    int handler = toluafix_ref_function(tolua_S, index, 0);
    return cocos2d::ScriptHandlerMgr::getInstance()->addCustomHandler(cobj, handler);
}



static int tolua_Cocos2d_YIM_registerScriptHandler(lua_State* tolua_S)
{
    TalkClass *self = (TalkClass*)tolua_tousertype(tolua_S, 1, 0);
    int iCallbackCount = lua_gettop(tolua_S) - 1;
    assert(iCallbackCount == 4);
    if (iCallbackCount != 4)
    {
        return 0;
    }
    
    EVENT_YTALK_EVENT = (int)_bindLuaFunction(tolua_S, 2, self);
    EVENT_YTALK_RestAPI = (int)_bindLuaFunction(tolua_S, 3, self);
    EVENT_YTALK_MemberChange = (int)_bindLuaFunction(tolua_S, 4, self);
    EVENT_YTALK_Broadcast = (int)_bindLuaFunction(tolua_S, 5, self);

    return 0;
}


TOLUA_API int register_youmetalk_manual(lua_State* tolua_S)
{
    tolua_open(tolua_S);
    tolua_usertype(tolua_S, "cc.YTalk");
    
    
    tolua_module(tolua_S, "cc", 0);
    tolua_beginmodule(tolua_S, "cc");
    tolua_cclass(tolua_S, "YTalk", "cc.YTalk", "", NULL);
    tolua_beginmodule(tolua_S, "YTalk");
    tolua_function(tolua_S, "create", tolua_Cocos2d_YTalk_create);
    
    tolua_function(tolua_S, "init", tolua_Cocos2d_YTalk_init);
    tolua_function(tolua_S, "unInit", tolua_Cocos2d_YTalk_unInit);
    
    tolua_function(tolua_S, "setServerRegion", tolua_Cocos2d_YTalk_setServerRegion);
    tolua_function(tolua_S, "setToken", tolua_Cocos2d_YTalk_setToken);
    tolua_function(tolua_S, "setAutoSendStatus", tolua_Cocos2d_YTalk_setAutoSendStatus);

    tolua_function(tolua_S, "setOutputToSpeaker", tolua_Cocos2d_YTalk_setOutputToSpeaker);
    tolua_function(tolua_S, "setSpeakerMute", tolua_Cocos2d_YTalk_setSpeakerMute);
    tolua_function(tolua_S, "setMicrophoneMute", tolua_Cocos2d_YTalk_setMicrophoneMute);
    tolua_function(tolua_S, "setUseMobileNetworkEnabled", tolua_Cocos2d_YTalk_setUseMobileNetworkEnabled);
    tolua_function(tolua_S, "setVolume", tolua_Cocos2d_YTalk_setVolume);
    tolua_function(tolua_S, "setBackgroundMusicVolume", tolua_Cocos2d_YTalk_setBackgroundMusicVolume);
    tolua_function(tolua_S, "getBackgroundMusicVolume", tolua_Cocos2d_YTalk_getBackgroundMusicVolume);

    
    tolua_function(tolua_S, "setHeadsetMonitorOn", tolua_Cocos2d_YTalk_setHeadsetMonitorOn);
    tolua_function(tolua_S, "setReverbEnabled", tolua_Cocos2d_YTalk_setReverbEnabled);
    tolua_function(tolua_S, "setVadCallbackEnabled", tolua_Cocos2d_YTalk_setVadCallbackEnabled);
    
    tolua_function(tolua_S, "getSpeakerMute", tolua_Cocos2d_YTalk_getSpeakerMute);
    tolua_function(tolua_S, "getMicrophoneMute", tolua_Cocos2d_YTalk_getMicrophoneMute);
    tolua_function(tolua_S, "getUseMobileNetworkEnabled", tolua_Cocos2d_YTalk_getUseMobileNetworkEnabled);
    tolua_function(tolua_S, "getVolume", tolua_Cocos2d_YTalk_getVolume);
    
    tolua_function(tolua_S, "joinChannelSingleMode", tolua_Cocos2d_YTalk_joinChannelSingleMode);
    tolua_function(tolua_S, "joinChannelMultiMode", tolua_Cocos2d_YTalk_joinChannelMultiMode);
    tolua_function(tolua_S, "speakToChannel", tolua_Cocos2d_YTalk_speakToChannel);
    tolua_function(tolua_S, "leaveChannelMultiMode", tolua_Cocos2d_YTalk_leaveChannelMultiMode);
    tolua_function(tolua_S, "leaveChannelAll", tolua_Cocos2d_YTalk_leaveChannelAll);
    tolua_function(tolua_S, "setWhiteUserList", tolua_Cocos2d_YTalk_setWhiteUserList);
    
    tolua_function(tolua_S, "setUserRole", tolua_Cocos2d_YTalk_setUserRole);
    tolua_function(tolua_S, "getUserRole", tolua_Cocos2d_YTalk_getUserRole);

    tolua_function(tolua_S, "playBackgroundMusic", tolua_Cocos2d_YTalk_playBackgroundMusic);
    tolua_function(tolua_S, "stopBackgroundMusic", tolua_Cocos2d_YTalk_stopBackgroundMusic);
    tolua_function(tolua_S, "pauseChannel", tolua_Cocos2d_YTalk_pauseChannel);
    tolua_function(tolua_S, "resumeChannel", tolua_Cocos2d_YTalk_resumeChannel);

    tolua_function(tolua_S, "getSoundtouchPitchSemiTones", tolua_Cocos2d_YTalk_getSoundtouchPitchSemiTones);
    tolua_function(tolua_S, "setSoundtouchPitchSemiTones", tolua_Cocos2d_YTalk_setSoundtouchPitchSemiTones);
    
    tolua_function(tolua_S, "setOtherMicMute", tolua_Cocos2d_YTalk_setOtherMicMute);
    tolua_function(tolua_S, "setOtherSpeakerMute", tolua_Cocos2d_YTalk_setOtherSpeakerMute);
    tolua_function(tolua_S, "setListenOtherVoice", tolua_Cocos2d_YTalk_setListenOtherVoice);
    
    tolua_function(tolua_S, "setMicLevelCallback", tolua_Cocos2d_YTalk_setMicLevelCallback);
    tolua_function(tolua_S, "setReleaseMicWhenMute", tolua_Cocos2d_YTalk_setReleaseMicWhenMute);
    tolua_function(tolua_S, "setExitCommModeWhenHeadsetPlugin", tolua_Cocos2d_YTalk_setExitCommModeWhenHeadsetPlugin);
    
    tolua_function(tolua_S, "setRecordingTimeMs", tolua_Cocos2d_YTalk_setRecordingTimeMs);
    tolua_function(tolua_S, "setPlayingTimeMs", tolua_Cocos2d_YTalk_setPlayingTimeMs);
    
    tolua_function(tolua_S, "getChannelUserList", tolua_Cocos2d_YTalk_getChannelUserList);
    tolua_function(tolua_S, "requestRestApi", tolua_Cocos2d_YTalk_requestRestApi);
    
    
    tolua_function(tolua_S, "sendMessage", tolua_Cocos2d_YTalk_sendMessage);
    tolua_function(tolua_S, "kickOtherFromChannel", tolua_Cocos2d_YTalk_kickOtherFromChannel);

    
    tolua_function(tolua_S,"setGrabMicOption",tolua_Cocos2d_YTalk_setGrabMicOption);
	tolua_function(tolua_S,"requestGrabMic",tolua_Cocos2d_YTalk_requestGrabMic);
	tolua_function(tolua_S,"startGrabMicAction",tolua_Cocos2d_YTalk_startGrabMicAction);
	tolua_function(tolua_S,"stopGrabMicAction",tolua_Cocos2d_YTalk_stopGrabMicAction);
	tolua_function(tolua_S,"releaseGrabMic",tolua_Cocos2d_YTalk_releaseGrabMic);
	tolua_function(tolua_S,"setInviteMicOption",tolua_Cocos2d_YTalk_setInviteMicOption);
	tolua_function(tolua_S,"stopInviteMic",tolua_Cocos2d_YTalk_stopInviteMic);
	tolua_function(tolua_S,"responseInviteMic",tolua_Cocos2d_YTalk_responseInviteMic);
	tolua_function(tolua_S,"requestInviteMic",tolua_Cocos2d_YTalk_requestInviteMic);
    tolua_function(tolua_S, "releaseMicSync", tolua_Cocos2d_YTalk_releaseMicSync);
    tolua_function(tolua_S, "resumeMicSync", tolua_Cocos2d_YTalk_resumeMicSync);


    tolua_function(tolua_S, "registerScriptHandler", tolua_Cocos2d_YIM_registerScriptHandler);
    
    tolua_endmodule(tolua_S);
    
    tolua_endmodule(tolua_S);
    
    return 1;
}

