#include "stdafx.h"
#include "LiveClient.h"
#include "LiveIpc.h"
#include "LiveWorker.h"

namespace LiveClient
{
    uv_loop_t *g_uv_loop = NULL;

    string g_strRtpIP;            //< RTP����IP
    int    g_nRtpBeginPort;       //< RTP��������ʼ�˿ڣ�������ż��
    int    g_nRtpPortNum;         //< RTPʹ�õĸ�������strRTPPort��ʼÿ�μ�2����strRTPNum��
    int    g_nRtpCatchPacketNum;  //< rtp����İ�������
    int    g_nRtpStreamType;      //< rtp�������ͣ�����libLive��ps h264
    int    g_nNodelay;            //< ��Ƶ��ʽ����Ƿ��������� 1:ÿ��֡����������  0:ÿ���ؼ�֡�������ķǹؼ�֡�յ���һ����

    vector<int>     m_vecRtpPort;     //< RTP���ö˿ڣ�ʹ��ʱ����ȡ����ʹ�ý������·���
    CriticalSection m_csRTP;          //< RTP�˿���

    extern LIVECLIENT_CB ipc_cb;

    void Init(void* uv){
        g_uv_loop = (uv_loop_t *)uv;

        /** ���̼�ͨ�� */
        LiveIpc::Init();

        /** �������� */
        g_strRtpIP           = Settings::getValue("RtpClient","IP");                    //< RTP����IP
        g_nRtpBeginPort      = Settings::getValue("RtpClient","BeginPort",10000);       //< RTP��������ʼ�˿ڣ�������ż��
        g_nRtpPortNum        = Settings::getValue("RtpClient","PortNum",1000);          //< RTPʹ�õĸ�������strRTPPort��ʼÿ�μ�2����strRTPNum��
        g_nRtpCatchPacketNum = Settings::getValue("RtpClient", "CatchPacketNum", 100);  //< rtp����İ�������
        g_nRtpStreamType     = Settings::getValue("RtpClient", "Filter", 0);            //< rtp���ͣ���ps����h264
        g_nNodelay           = Settings::getValue("RtpClient", "NoDelay", 0);           //< �Ƿ���������

        Log::debug("RtpConfig IP:%s, BeginPort:%d,PortNum:%d,CatchPacketNum:%d"
            , g_strRtpIP.c_str(), g_nRtpBeginPort, g_nRtpPortNum, g_nRtpCatchPacketNum);
        m_vecRtpPort.clear();
        for (int i=0; i<g_nRtpPortNum; ++i) {
            m_vecRtpPort.push_back(g_nRtpBeginPort+i*2);
        }

        if(g_nRtpStreamType == 0)
            g_stream_type = STREAM_PS;
        else if(g_nRtpStreamType == 1)
            g_stream_type = STREAM_H264;
    }

    string GetClientsInfo() 
    {
        return GetAllWorkerClientsInfo();
    }

    void GetDevList(){
        LiveIpc::GetDevList();
    }

    void QueryDirtionary(){
        LiveIpc::QueryDirtionary();
    }

	void DeviceControl(string strDev, int nInOut, int nUpDown, int nLeftRight) {
		LiveIpc::DeviceControl(strDev, nInOut, nUpDown, nLeftRight);
	}

    void SetCallBack(LIVECLIENT_CB cb){
        ipc_cb = cb;
    }

    ILiveWorker* GetWorker(string strCode){
        CLiveWorker* worker = GetLiveWorker(strCode);
        if(nullptr == worker)
            worker = CreatLiveWorker(strCode);
        return (ILiveWorker*)worker;
    }
}