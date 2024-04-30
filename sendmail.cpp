#include "sendmail.h"

using namespace std;

bool sendmail(string receiver_name, string receiver_addr, string title, string text)
{
	CSendMail sMailer;
	sMailer.setServerName("smtp.qq.com");                                                        //邮箱smtp,如"smtp.126.com"
	sMailer.setUserName("1634803825@qq.com");                                                                //邮箱账号名，如"****@126.com"
	sMailer.setUserPwd("bofgibtlzkmtfaec");                                                                //邮箱密码
	sMailer.setSenderName("JIMI STUDIO");                                                                //发件人名字
	sMailer.setSenderAddress("1634803825@qq.com");                                                //发送邮箱地址，填你账号的地址,上面的邮箱账号名"****@126.com"，同上面的邮箱账号名

	sMailer.setReceiver(receiver_name, receiver_addr);                                            //添加邮件接收者

	//发送第一份邮件
	if (sMailer.Connent())                                                                        //每次发邮件前都需要connect
	{
		if (sMailer.SendMail(title, text))            //第一个字符串是邮件标题，第二个是邮件内容
			return true;
	}
	return false;
}