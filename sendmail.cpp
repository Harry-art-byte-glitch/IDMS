#include "sendmail.h"

using namespace std;

bool sendmail(string receiver_name, string receiver_addr, string title, string text)
{
	CSendMail sMailer;
	sMailer.setServerName("smtp.qq.com");                                                        //����smtp,��"smtp.126.com"
	sMailer.setUserName("1634803825@qq.com");                                                                //�����˺�������"****@126.com"
	sMailer.setUserPwd("bofgibtlzkmtfaec");                                                                //��������
	sMailer.setSenderName("JIMI STUDIO");                                                                //����������
	sMailer.setSenderAddress("1634803825@qq.com");                                                //���������ַ�������˺ŵĵ�ַ,����������˺���"****@126.com"��ͬ����������˺���

	sMailer.setReceiver(receiver_name, receiver_addr);                                            //����ʼ�������

	//���͵�һ���ʼ�
	if (sMailer.Connent())                                                                        //ÿ�η��ʼ�ǰ����Ҫconnect
	{
		if (sMailer.SendMail(title, text))            //��һ���ַ������ʼ����⣬�ڶ������ʼ�����
			return true;
	}
	return false;
}