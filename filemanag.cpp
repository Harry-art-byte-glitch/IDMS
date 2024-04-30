#include "filemanang.h"

#pragma warning(disable:4996)

using namespace std;

long long int size_file(const char* name)
{
	if (name == NULL) {
		return 0;
	}
	// ����һ���洢�ļ�(��)��Ϣ�Ľṹ�壬�������ļ���С�ʹ���ʱ�䡢����ʱ�䡢�޸�ʱ���
	struct stat statbuf;

	// �ṩ�ļ����ַ���������ļ����Խṹ��
	stat(name, &statbuf);

	// ��ȡ�ļ���С
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