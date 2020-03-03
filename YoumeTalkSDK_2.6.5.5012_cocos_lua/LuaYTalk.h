// TalkClass.h

#ifndef __TALK__CLASS

#define __TALK__CLASS

#include "IYouMeVoiceEngine.h"


#include "cocos2d.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif
namespace cocos2d {
    class TalkClass : public IYouMeEventCallback,
        public IYouMePcmCallback,
        public IRestApiCallback,
        public IYouMeMemberChangeCallback,
		public IYouMeChannelMsgCallback
    {
    public:
        ~TalkClass();
        static TalkClass* getInstance();
    private:
        TalkClass();
    public:
        
        bool m_bUninit=false;
        
        void onEvent ( const YouMeEvent event, const YouMeErrorCode error, const char * channel, const char * param ) override ;

        //远端pcm数据回调
        void onPcmDataRemote(int channelNum, int samplingRateHz, int bytesPerSample, void* data, int dataSizeInByte) override;
        //本地录音pcm数据回调
        void onPcmDataRecord(int channelNum, int samplingRateHz, int bytesPerSample, void* data, int dataSizeInByte) override;
        //远端pcm和本地录音pcm的混合pcm数据回调
        void onPcmDataMix(int channelNum, int samplingRateHz, int bytesPerSample, void* data, int dataSizeInByte) override;

        void onRequestRestAPI( int requestID, const YouMeErrorCode &iErrorCode, const  char*  strQuery, const  char*   strResult )override;
        
        void onMemberChange( const char* channel, const char* listMemberChange , bool bUpdate ) override ;

        void onBroadcast(const YouMeBroadcast bc, const char* channel, const char*  param1, const char*  param2, const char*  strContent)override;
    };
} //namespace cocos2d

TOLUA_API int register_youmetalk_manual(lua_State* tolua_S);
#endif // __TALK__CLASS
