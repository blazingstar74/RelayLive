#pragma once
#include "util_public.h"
#include <string>
using namespace std;

namespace Settings
{
    /**
     * ���������ļ�
     * @param strFileName[in] �����ļ�·��
     * @return �ɹ�true,ʧ��false;
     */
    bool   UTIL_API loadFromProfile(const string &strFileName);

    /**
     * ��ȡ����ֵ
     * @param section[in] ���ö���
     * @param key[in] ���ùؼ���
     * @return ����ֵ
     */
    string UTIL_API getValue(const string &section,const string &key);

    /**
     * ��ȡ����ֵ
     * @param section[in] ���ö���
     * @param key[in] ���ùؼ���
     * @param default[in] Ĭ��ֵ
     * @return ����ֵ
     */
    string UTIL_API getValue(const string &section,const string &key,const string &default);

    /**
     * ��ȡ��������ֵ
     * @param section[in] ���ö���
     * @param key[in] ���ùؼ���
     * @param default[in] Ĭ��ֵ
     * @return ����ֵ
     */
    int UTIL_API getValue(const string &section,const string &key,const int &default);
};
