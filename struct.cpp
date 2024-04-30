#include <tchar.h>
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <io.h>
#include "sendmail.h"
#include "password.h"

#pragma warning(disable:4996)

#pragma comment(lib,"ws2_32.lib")

using namespace std;

const string key = "57211457941480306503939286961845";
const string iv = "3711408647380926";

user user_list[100];

void user_in()
{
	string gtemp;
	ifstream ifs;
	ifs.open(".\\Data\\Users\\UsersList.list", ios::in);
	int i = 0;
	while (getline(ifs, gtemp))
	{
		int tplace1 = 0, tplace2 = gtemp.find('/', 0);
		::user_list[i].name = gtemp.substr(tplace1, tplace2);
		tplace1 = tplace2 + 1;
		tplace2 = gtemp.find('/', tplace1);
		::user_list[i].password = decrypt(gtemp.substr(tplace1, tplace2 - tplace1), key, iv);
		i++;
	}
}

static void ReceiveAllClients(LPVOID para)
{
	user loginuser;
	/*这一步很重要，拿到了客户端的SOCKET套接字地址，要先解引用把它的值取出来，
	不要直接对这个指针操作，因为所有的客户端套接字地址都是通过这个指针传进来的*/
	SOCKET newClient = *((SOCKET*)para);
	char msg[1000] = { 0 };
	sockaddr_in clientMsg;
	int len = sizeof(sockaddr);

	getpeername(newClient, (sockaddr*)&clientMsg, &len);/*获取客户端的socket的ip和端口信息，方便查看谁连了上来*/
	ofstream ofs;
	ifstream ifs;
	ofs.open(".\\Data\\Logs\\ConnectedLogs.log", ios::app);
	ofs << "Connect in" << '/' << clientMsg.sin_addr.S_un.S_addr << '/' << clientMsg.sin_port << '/' << "OK" << '/' << get_time() << get_hour_min_sec() << endl;
	ofs.close();
	std::cout << clientMsg.sin_addr.S_un.S_addr << ":" << clientMsg.sin_port << " has connected!\n";

	while (1)
	{
		memset(msg, '\0', sizeof(msg));
		if (recv(newClient, msg, sizeof(msg), NULL) < 0)
		{
			std::cout << "receive message fail!\n";
			ofs.open(".\\Data\\Logs\\ConnectedLogs.log", ios::app);
			ofs << "Receive message from" << '/' << clientMsg.sin_addr.S_un.S_addr << '/' << clientMsg.sin_port << '/' << "Fail" << '/' << get_time() << get_hour_min_sec() << endl;
			ofs.close();
			return;
		}
		std::cout << "Client(" << clientMsg.sin_port << "): " << msg << std::endl;
		string in = msg;
		int place1 = 0, place2 = in.find('/', 0);
		string model = in.substr(0, place2);
		string temp;
		if (model == "signin")
		{
			string mail_addr, name, password;
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			mail_addr = in.substr(place1, place2 - place1);
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			name = in.substr(place1, place2 - place1);
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			decrypt(in.substr(place1, place2 - place1), key, iv);
			if (password.find('/', 0) != -1)
			{
				send(newClient, "ES0001", 7, NULL);
				ofs.open(".\\Data\\Logs\\SigninLogs.log", ios::app);
				ofs << "Sign in user\"" << name << "\"/Password error/" << get_time() << get_hour_min_sec() << endl;
				ofs.close();
				continue;
			}
			string temp;
			temp = ".\\Data\\Users\\" + name;
			if (_access(temp.c_str(), 0) == 0)
			{
				send(newClient, "ES0002", 7, NULL);
				ofs.open(".\\Data\\Logs\\SigninLogs.log", ios::app);
				ofs << "Sign in user\"" << name << "\"/User exists/" << get_time() << get_hour_min_sec() << endl;
				ofs.close();
				continue;
			}
			ofs.open(".\\Data\\Users\\" + name + "sign.temp", ios::app);
			string x;
			for (int i = 0; i < 8; i++)
			{
				int a = rand() % 10;
				ofs << int_to_string(a);
				x = x + int_to_string(a);
			}
			ofs << '/';
			ofs.close();
			sendmail(name, mail_addr, "注册验证", "验证码:" + x);
			send(newClient, "YS0001", 7, NULL);
			ofs.open(".\\Data\\Logs\\SigninLogs.log", ios::app);
			ofs << "Send auth code for sign in user\"" << name << "\"/OK/" << get_time() << get_hour_min_sec() << endl;
			ofs.close();
		}
		else if (model == "sighincheck")
		{
			string mail_addr, name, password, auth_code,user_code;
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			mail_addr = in.substr(place1, place2 - place1);
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			name = in.substr(place1, place2 - place1);
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			password = decrypt(in.substr(place1, place2 - place1), key, iv);
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			user_code = in.substr(place1, place2 - place1);
			ifs.open(".\\Data\\Users\\" + name + "sign.temp", ios::in);
			getline(ifs, auth_code);
			auth_code = auth_code.substr(0, auth_code.find('/', 0));
			ifs.close();
			if (user_code == auth_code)
			{
				temp = ".\\Data\\Users\\" + name;
				mkfolder(temp.c_str(),name);
				temp = temp + "\\Common";
				mkfolder(temp.c_str(),name);
				temp = temp + "\\Files";
				mkfolder(temp.c_str(), name);
				temp = { NULL };
				ofs.open(".\\Data\\Users\\" + name + "\\Common\\Password.pw", 0x02);
				ofs << encrypt(password, key, iv) << '/';
				ofs.close();
				ofs.open(".\\Data\\Users\\" + name + "\\Common\\Mailaddr.addr", 0x02);
				ofs << mail_addr << '/';
				ofs.close();
				ofs.open(".\\Data\\Users\\" + name + "\\Common\\UserLogs.log", 0x02);
				ofs << "Sign in" << '/' << get_time() << get_hour_min_sec << endl;
				ofs.close();
				ofs.open(".\\Data\\Users\\" + name + "\\Common\\Permissiblesize.size", 0x02);
				ofs << "1073741824" << '/';
				ofs.close();
				ofs.open(".\\Data\\Users\\UsersList.list", ios::app);
				ofs << name << '/' << encrypt(password, key, iv) << '/' << endl;
				ofs.close();
				send(newClient, "YS0002", 7, NULL);
				ofs.open(".\\Data\\Logs\\SigninLogs.log", ios::app);
				ofs << "Sign in user\"" << name << "\"/" << "OK" << '/' << "1073741824" << '/' << get_time() << get_hour_min_sec() << endl;
				ofs.close();
			}
			else
			{
				send(newClient, "ES0003", 7, NULL);
				ofs.open(".\\Data\\Logs\\SigninLogs.log", ios::app);
				ofs << "Sign in user\"" << name << "\"/Auth code error/" << get_time() << get_hour_min_sec() << endl;
				ofs.close();
			}
		}

		else if (model == "login")
		{
			string name, password;
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			name = in.substr(place1, place2 - place1);
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			password = decrypt(in.substr(place1, place2 - place1), key, iv);
			place1 = place2 + 1;
			/*
			ifstream ifs;
			ifs.open(".\\Data\\Users\\UsersList.list", ios::in);
			user user_list[100];
			string gtemp;
			int i = 0;
			while (std::getline(ifs, gtemp))
			{
				int tplace1 = 0, tplace2 = gtemp.find('/', 0);
				user_list[i].name = gtemp.substr(tplace1, tplace2);
				tplace1 = tplace2 + 1;
				tplace2 = gtemp.find('/', tplace1);
				user_list[i].password = decrypt(gtemp.substr(tplace1, tplace2 - tplace1), key, iv);
				i++;
			}
			//delete& gtemp;
			*/
			bool login = false;
			for (int i = 0; i < 100; i++)
			{
				if (name == ::user_list[i].name)
				{
					if (password == user_list[i].password)
					{
						loginuser.name = name;
						loginuser.password = password;
						ofs.open(".\\Data\\Logs\\LoginLogs.log", ios::app);
						ofs << "Log in user\"" << name << "\"/OK/" << get_time() << get_hour_min_sec() << endl;
						ofs.close();
						send(newClient, "YL0001", 7, NULL);
						ifs.open(".\\Data\\Users\\" + name + "\\Common\\Permissiblesize.size", ios::in);
						string temp;
						getline(ifs, temp);
						temp = temp.substr(0, temp.find('/'));
						loginuser.file_max_size = stoi(temp);
						temp = ".\\Data\\Users\\" + name;
						loginuser.file_size = size_file(temp.c_str());
						if (loginuser.file_size >= loginuser.file_max_size)
						{
							loginuser.state = "No space available";
							send(newClient, "EU0001", 7, NULL);
						}
						else
						{
							loginuser.available_space = loginuser.file_max_size - loginuser.file_size;
							loginuser.state = "Available";
							temp = int_to_string(loginuser.available_space) + "B available";
							send(newClient, temp.c_str(), 7, NULL);
						}
					}
					else
					{
						ofs.open(".\\Data\\Logs\\LoginLogs.log", ios::app);
						ofs << "Log in user\"" << name << "\"/Password error/" << get_time() << get_hour_min_sec() << endl;
						ofs.close();
						send(newClient, "EL0001", 7, NULL);
					}
					login = true;
					break;
				}
			}
			if (!login)
			{
				ofs.open(".\\Data\\Logs\\LoginLogs.log", ios::app);
				ofs << "Log in user\"" << name << "\"/User does not exist/" << get_time() << get_hour_min_sec() << endl;
				ofs.close();
				send(newClient, "EL0002", 7, NULL);
			}
		}
		else if (model == "create file")
		{
			string name, password, path;
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			name = in.substr(place1, place2 - place1);
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			password = in.substr(place1, place2 - place1);
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			path = in.substr(place1, place2 - place1);
			ofs.open(".\\Data\\Users\\" + loginuser.name + "\\Files" + path, 0x02);
			ofs.close();

			/*
			string temp;
			getline(ifs, temp);
			temp = temp.substr(0, temp.find('/'));
			loginuser.file_max_size = stoi(temp);
			temp = ".\\Data\\Users\\" + loginuser.name;
			loginuser.file_size = size_file(temp.c_str());
			if (loginuser.file_size >= loginuser.file_max_size)
			{
				loginuser.state = "No space available";
				send(newClient, "EU0001", 7, NULL);
			}
			else
			{
				loginuser.available_space = loginuser.file_max_size - loginuser.file_size;
				loginuser.state = "available";
				temp = int_to_string(loginuser.available_space) + "B available";
				send(newClient, temp.c_str(), 1000, NULL);

			}
			*/
		}
		/*
		else if (model == "create folder")
		{
			string temp;
			getline(ifs, temp);
			temp = temp.substr(0, temp.find('/'));
			loginuser.file_max_size = stoi(temp);
			temp = ".\\Data\\Users\\" + loginuser.name;
			loginuser.file_size = size_file(temp.c_str());
			if (loginuser.file_size >= loginuser.file_max_size)
			{
				loginuser.state = "No space available";
				send(newClient, "EU0001", 7, NULL);
			}
			else
			{
				loginuser.available_space = loginuser.file_max_size - loginuser.file_size;
				loginuser.state = "available";
				temp = int_to_string(loginuser.available_space) + "B available";
				send(newClient, temp.c_str(), 1000, NULL);

			}
		}
		*/

		else if (model == "space check")
		{
			temp = ".\\Data\\Users\\" + loginuser.name;
			loginuser.file_size = size_file(temp.c_str());
			loginuser.available_space = loginuser.file_max_size - loginuser.file_size;
		}
		else if (model == "delete file")
		{
			string path;
			place1 = place2 + 1;
			place2 = in.find('/', place1);
			path = in.substr(place1, place2 - place1);
			temp = "del .\\Data\\Users\\" + loginuser.name + "\\Files\\" + path;
			system(const_cast<char*>(temp.c_str()));
		}
		else if (model == "delete folder")
		{

		}
		else if (model == "open file")
		{

		}
		else if (model == "open folder")
		{

		}
		temp = { NULL };
		//send(newClient, "收到，请加大力度", 1000, NULL);
		/*其实一般项目，服务端收到客户端消息后，都是需要解析这个消息并返回对应报文的，肯定不会这么随意*/
	}
}

int main(int argc, _TCHAR* argv[])
{
	ofstream ofs;
	ifstream ifs;
	ofs.open(".\\Data\\Logs\\SystemLogs.log", ios::app);
	ofs << "Start system" << '/' << "OK" << '/' << get_time() << get_hour_min_sec() << endl;
	ofs.close();
	system("NetSh Advfirewall set allprofiles state off");
	srand((unsigned)time(NULL));
	string ip;
	u_short port;
	Sleep(2000);
	system("cls");
	printf("####################################################################\n");
	printf("####################################################################\n");
	printf("###   ##   ## ##### ##   ###   ###     ## ### ##    ####   ###   ###\n");
	printf("#### #### ###  ###  ### ### ######## #### ### ## ### #### ### ### ##\n");
	printf("#### #### ### # # # ### ####   ##### #### ### ## ### #### ### ### ##\n");
	printf("## # #### ### ## ## ### ####### #### #### ### ## ### #### ### ### ##\n");
	printf("### ####   ## ##### ##   ###   ##### #####   ###    ####   ###  ####\n");
	printf("####################################################################\n");
	printf("####################################################################\n");
	Sleep(2000);
	system("cls");
	cout << "请输入服务器地址" << endl;
	cin >> ip;
	system("cls");
	cout << "请输入端口" << endl;
	cin >> port;
	system("cls");
	SOCKET server;
	SOCKET client;
	sockaddr_in serverAddr;
	WSADATA wsaData;
	int wsaRet = WSAStartup(MAKEWORD(2, 2), &wsaData);

	/*windows编译器必须要启动这个网络库，否则后面的步骤都会执行失败,但是linux操作系统好像不需要这个*/
	if (wsaRet)
	{
		std::cout << "错误:WSA启动失败" << std::endl;
		ofs.open(".\\Data\\Logs\\SystemLogs.log", ios::app);
		ofs << "Run WSA" << '/' << "Fail" << '/' << get_time() << get_hour_min_sec() << endl;
		ofs.close();
		return 0;
	}
	else
	{
		std::cout << "WSA启动成功" << std::endl;
		ofs.open(".\\Data\\Logs\\SystemLogs.log", ios::app);
		ofs << "Run WSA" << '/' << "OK" << '/' << get_time() << get_hour_min_sec() << endl;
		ofs.close();
	}

	server = socket(AF_INET, SOCK_STREAM, 0);

	//bind
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.S_un.S_addr = inet_addr(ip.string::c_str());

	if (::bind(server, (sockaddr*)&serverAddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		std::cout << "错误:绑定失败!" << std::endl;
		ofs.open(".\\Data\\Logs\\SystemLogs.log", ios::app);
		ofs << "Bind" << '/' << "Fail" << '/' << get_time() << get_hour_min_sec() << endl;
		ofs.close();
		return 0;
	}
	else
	{
		std::cout << "绑定成功!" << std::endl;
		ofs.open(".\\Data\\Logs\\SystemLogs.log", ios::app);
		ofs << "Bind" << '/' << "OK" << '/' << get_time() << get_hour_min_sec() << endl;
		ofs.close();
	}

	//listen
	if (listen(server, 20) == SOCKET_ERROR)
	{
		std::cout << "错误:监听失败!" << std::endl;
		ofs.open(".\\Data\\Logs\\SystemLogs.log", ios::app);
		ofs << "Listening" << '/' << "Fail" << '/' << get_time() << get_hour_min_sec() << endl;
		ofs.close();
		return 0;
	}
	else
	{
		std::cout << "监听成功!" << std::endl;
		ofs.open(".\\Data\\Logs\\SystemLogs.log", ios::app);
		ofs << "Listening" << '/' << "OK" << '/' << get_time() << get_hour_min_sec() << endl;
		ofs.close();
	}

	//accept
	while (1)
	{
		client = accept(server, NULL, NULL);

		if (client >= 0)
		{
			/*accept和recv函数都是阻塞函数,这种情况基本上都要开多线程的*/
			_beginthread(ReceiveAllClients, NULL, &client);
		}
		else
		{
			std::cout << "错误:接受连接失败!" << std::endl;
			ofs.open(".\\Data\\Logs\\ConnectedLogs.log", ios::app);
			ofs << "Accept connection" << '/' << "Fail" << '/' << get_time() << get_hour_min_sec() << endl;
			ofs.close();
		}
	}
	system("pause");
	return 0;
}