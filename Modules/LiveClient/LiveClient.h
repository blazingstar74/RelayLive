/**
* 该文件是libLive模块唯一对外导出的头文件
*/
#pragma once

#include "avtypes.h"

#ifdef LIVECLIENT_EXPORTS
#define LIVECLIENT_API __declspec(dllexport)
#else
#define LIVECLIENT_API
#endif

enum HandleType
{
    unknown_handle,
    flv_handle,
    fmp4_handle,
    h264_handle,
    ts_handle,
    rtp_handle,
};



namespace LiveClient
{
    struct ClientInfo {
        string devCode; //设备id
        string connect; //连接协议 http/websocket/rtsp/hls
        string media;   //媒体类型 h264/flv/mp4/ts
        string clientIP; //客户端ip
        int    channel; //通道号
    };
    /**
    * 数据回调处理接口
    * 由上层实现一个继承该接口的类，live worker通过该接口访问其实例的方法
    */
    struct ILiveHandle
    {
		virtual ~ILiveHandle(){};

        /**
         * 播放请求结果回调
         * @param ret 错误码 成功是0
         * @param error_info 成功为下级平台返回的sdp，失败是失败原因
         */
        virtual void play_answer(int ret, string error_info) = 0;

        /**
         * 视频数据发送回调
         */
        virtual void push_video_stream(AV_BUFF buff) = 0;

       /**
        * rtp接收端结束，目前已知只有接收超时引起
        */
        virtual void stop() = 0;

        /**
         * 获取客户端信息
         */
        virtual ClientInfo get_clients_info() = 0;
    };

    struct ILiveHandleRtp : public ILiveHandle {
        virtual void push_rtcp_stream(char*, int) = 0;
    };

    /**
     * 对上层导出live worker的接口
     */
    struct ILiveWorker
    {
        /**
        * 向liveworker中添加一个回调句柄。
        * liveworker对应多个livehandle
        * @param pWorker Liveworker负责接收rtp数据并解析成各种格式
        * @param h 需要视频包数据来执行各种处理，如作为服务发送到客户端
        */
        virtual bool AddHandle(ILiveHandle* h, HandleType t, int c) = 0;

        /**
        * 从liveworker中移除一个livehandle。
        * 当liveworker中的handle全部移除时，liveworker将会自杀
        */
        virtual bool RemoveHandle(ILiveHandle* h) = 0;

        //virtual string GetSDP() = 0;
    };

    /**
    * liveclient初始化
    */
    void LIVECLIENT_API Init(void* uv);

    /**
    * 获取客户端内容。这是异步操作，此处只发出请求。
    */
    void LIVECLIENT_API GetClientsInfo();

    /**
    * 获取设备信息。这是异步操作，此处只发出请求。
    * @param h 发起请求的句柄
    */
    void LIVECLIENT_API GetDevList();

    /**
     * 通知sip模块订阅设备目录
     */
    void LIVECLIENT_API QueryDirtionary();

	/**
	 * 通知sip模块发送设备控制信息
	 */
	 void LIVECLIENT_API DeviceControl(string strDev,
        int nInOut = 0, int nUpDown = 0, int nLeftRight = 0);

    /**
     * 设置回调，获取信息完成后,通知调用者
     * 参数1：命令名称
     * 参数2：命令运算结果返回值
     */
    typedef void(*LIVECLIENT_CB)(string, string);
    void LIVECLIENT_API SetCallBack(LIVECLIENT_CB cb);

    /**
    * 获取一个设备的直播句柄，若已存在直接使用，若不存在则创建一个新的
    * @param strCode 设备的国标编码
    */
    ILiveWorker* LIVECLIENT_API GetWorker(string strCode);
}