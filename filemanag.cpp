#include "filemanang.h"

#pragma warning(disable:4996)

using namespace std;

long long int size_file(const char* name)
{
	if (name == NULL) {
		return 0;
	}
	// 这是一个存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
	struct stat statbuf;

	// 提供文件名字符串，获得文件属性结构体
	stat(name, &statbuf);

	// 获取文件大小
	size_t filesize = statbuf.st_size;

	return long long int(filesize);
}

void mkfolder(string name,string username)
{
	ofstream ofs;
	switch (_mkdir(name.c_str()))
	{
	case 0: {
				ofs.open(".\\Data\\Logs\\SystemLogs.log", ios::app);
				ofs << "Create folder for\"" << username << "\"/OK/" << get_time() << get_hour_min_sec() << endl;
				ofs.close();
				
			}
	case 1: {
				ofs.open(".\\Data\\Users\\" + username + "\\Common\\UserLogs.log", ios::app);
				ofs << "Create folder for\"" << username << "\"/Permission error/" << get_time() << get_hour_min_sec() << endl;
				ofs.close();
				break;
			}
	case 2: {
				ofs.open(".\\Data\\Users\\" + username + "\\Common\\UserLogs.log", ios::app);
				ofs << "Create folder for\"" << username << "\"/Spcae error/" << get_time() << get_hour_min_sec() << endl;
				ofs.close();
				break;
			}
	case 3: {
				ofs.open(".\\Data\\Users\\" + username + "\\Common\\UserLogs.log", ios::app);
				ofs << "Create folder for\"" << username << "\"/File name conflict/" << get_time() << get_hour_min_sec() << endl;
				ofs.close();
				break;
			}
	case 4: {
				ofs.open(".\\Data\\Users\\" + username + "\\Common\\UserLogs.log", ios::app);
				ofs << "Create folder for\"" << username << "\"/Char error/" << get_time() << get_hour_min_sec() << endl;
				ofs.close();
				break;
			}
	}
	return;
}