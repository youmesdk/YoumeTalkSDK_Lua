cc.exports.youmetalk = cc.YTalk:create()
cc.exports.userID = ""
cc.exports.roomID = ""
cc.exports.roleType = 0

local addTips = function (  strTips )
    local event = cc.EventCustom:new("Tips")
    event._usedata = strTips 
    cc.Director:getInstance():getEventDispatcher():dispatchEvent( event ) 
end

cc.exports.youmetalk.OnEvent = function( event, errorcode, channel, param )
	print("收到Talk回调:"..event..","..errorcode..","..channel..","..param)
	if event == 0 then --YOUME_EVENT_INIT_OK:
		addTips( "初始化成功" )
	elseif event == 1 then --YOUME_EVENT_INIT_FAILED:
		event( "初始化失败" )
	elseif event == 2 then --YOUME_EVENT_JOIN_OK:
		addTips( "进入房间成功, ID: "..channel)
		print("why?")
	    local evt = cc.EventCustom:new("EnterRoom")
    	cc.Director:getInstance():getEventDispatcher():dispatchEvent( evt ) 
	elseif event == 3 then --YOUME_EVENT_JOIN_FAILED
		addTips("进入房间失败")
	elseif event == 4 then --YOUME_EVENT_LEAVED_ONE
		addTips("退出房间")
	elseif event == 5 then --YOUME_EVENT_LEAVED_ALL
		addTips("退出房间")
	elseif event == 6 then --YOUME_EVENT_PAUSED
		addTips("暂停")
	elseif event == 7 then --YOUME_EVENT_RESUMED
		addTips("恢复")
	elseif event == 8 then --YOUME_EVENT_SPEAK_SUCCESS:切换对指定频道讲话成功（适用于多频道模式）
		--addTips("进入房间失败")
	elseif event == 9 then --YOUME_EVENT_SPEAK_FAILED:切换对指定频道讲话失败（适用于多频道模式）
		--addTips("进入房间失败")
	elseif event == 10 then --YOUME_EVENT_RECONNECTING
		addTips("正在重连")
	elseif event == 11 then --YOUME_EVENT_RECONNECTED
		addTips("重连成功")
	elseif event == 12 and errorcode == -202 then --YOUME_EVENT_REC_PERMISSION_STATUS
		addTips("录音启动失败")
	elseif event == 13 then --YOUME_EVENT_BGM_STOPPED
		addTips("背景音乐播放结束")
	elseif event == 14 then --YOUME_EVENT_BGM_FAILED
		addTips("背景音乐播放失败")
	elseif event == 15 then --YOUME_EVENT_MEMBER_CHANGE
		print("频道成员变化:"..param)
	elseif event == 16 then --YOUME_EVENT_OTHERS_MIC_ON
		print("其他用户麦克风打开:"..param)
	elseif event == 17 then --YOUME_EVENT_OTHERS_MIC_OFF
		print("其他用户麦克风关闭:"..param)
	elseif event == 18 then --YOUME_EVENT_OTHERS_SPEAKER_ON
		print("其他用户扬声器打开"..param)
	elseif event == 19 then --YOUME_EVENT_OTHERS_SPEAKER_OFF
		print("其他用户扬声器关闭"..param)
	elseif event == 20 then --YOUME_EVENT_OTHERS_VOICE_ON
		print("其他用户进入讲话状态:"..param)
	elseif event == 21 then --YOUME_EVENT_OTHERS_VOICE_OFF
		print("其他用户进入静默状态:"..param)
	elseif event == 22 then --YOUME_EVENT_MY_MIC_LEVEL
		print("麦克风的语音级别:"..param)
	elseif event == 23 then --YOUME_EVENT_MIC_CTR_ON
		print("麦克风被其他用户打开:"..param)
	elseif event == 24 then --YOUME_EVENT_MIC_CTR_OFF
		print("麦克风被其他用户关闭:"..param)
	elseif event == 25 then --YOUME_EVENT_SPEAKER_CTR_ON
		print("扬声器被其他用户打开:"..param)
	elseif event == 26 then --YOUME_EVENT_SPEAKER_CTR_OFF
		print("扬声器被其他用户关闭:"..param)
	elseif event == 27 then --YOUME_EVENT_LISTEN_OTHER_ON
		print("取消屏蔽某人语音:"..param)
	elseif event == 28 then --YOUME_EVENT_LISTEN_OTHER_OFF
		print("屏蔽某人语音:"..param)
	end
end


cc.exports.youmetalk.OnMemberChange = function ( channel,  member_list_json, is_update)
    print("OnMemberChange:"..channel..",json:"..member_list_json)
    -- member_list_json 样例：
    -- {"channelid":"123","memchange":[{"isJoin":true,"userid":"u541"},{"isJoin":true,"userid":"u948"}]}
end

cc.exports.youmetalk.OnRequestRestApi = function( requestid, errorcode, command, result )
	print("OnRequestRestApi:"..requestid)
	print(errorcode)
	print(command)
	print(result)
end 

cc.exports.youmetalk.OnBroadcast = function( bctype, channel, param1, param2, content )
	print("OnBroadcast:"..bctype..",channel:"..channel..",P1:"..param1..",P2"..param2..",content:"..content)
end

cc.exports.youmetalk.registerScriptHandler(cc.exports.youmetalk,
	cc.exports.youmetalk.OnEvent,
	cc.exports.youmetalk.OnRequestRestApi,
	cc.exports.youmetalk.OnMemberChange,
	cc.exports.youmetalk.OnBroadcast);




















