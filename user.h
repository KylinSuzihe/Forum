#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;
class User {
private :
	vector<string> q;
	void init() {
		//�������û�����
		FILE* fp = fopen("user.txt", "r");
		char ls[100];
		q.clear();
		while (!feof(fp)) {
			fscanf(fp, "%s", ls);
			q.push_back(ls);
			fscanf(fp, "%s", ls);
			fscanf(fp, "%s", ls);
			fscanf(fp, "%s", ls);
		}
		fclose(fp);
	}
public :
	string name;
	User() {}
	~User() {}
	bool reg() {
		system("cls");
		init();
		//�����û���Ϣ���ж��Ƿ�Ϸ�
		cout << "�������û�����\n";
		string name;
		bool b = 1;
		while (b) {
			cin >> name;
			for (int i = 0; i < q.size(); ++i) 
				if (q[i] == name) {
					cout << "�û����Ѵ��ڣ�����������\n";
					b = 0; break;
				}
			b ^= 1;
		}
		cout << "���������룺\n";
		string password;
		b = 1;
		while (b) {
			cin >> password;
			if (password.size() < 6) {
				cout << "����λ��̫�٣�������������λ������\n";
				continue;
			}
			for (int i = 0; i < password.size(); ++i) 
				if (password[i] < '0' || password[i] > '9') b = 0;
			if (b) cout << "�������ٰ��������ַ�������������\n";
		}
		cout << "�����绰��\n";
		string phone;
		b = 1;
		while (b) {
			cin >> phone;
			if (phone.size() != 11) cout << "�绰���벻�Ϸ���\n", b = 0;
			else {
				for (int i = 0; i < 11; ++i)
					if (phone[i] < '0' || phone[i] > '9') {
						cout << "�绰���벻�Ϸ���\n";
						b = 0;
						break;
					}
			}
			b ^= 1;
		}
		cout << "���������䣺\n";
		string email;
		b = 1;
		while (b) {
			cin >> email;
			if (email[0] == '@' || email[email.size()] == '@') cout << "���䲻�Ϸ���\n", b = 0;
			else {
				int cnt = 0;
				for (int i = 1; i < email.size() - 1; ++i)
					cnt += email[i] == '@';
				if (cnt != 1) b = 0, cout << "���䲻�Ϸ���\n";
			}
			b ^= 1;
		}
		FILE* fp = fopen("user.txt", "a");
		//freopen("user.txt", "a", stdout);
		fprintf(fp, "%s %s %s %s\n", name.c_str(), password.c_str(), phone.c_str(), email.c_str());
		//cout << name << ' ' << password << ' ' << phone << ' ' << email << endl;
		fclose(fp);
		cout << name << "ע��ɹ���";
		Sleep(2000); 
		return 1;
	}
	vector<string> logIn() {
		FILE* fp = fopen("user.txt", "r");
		char* ls;
		q.clear();
		while (!feof(fp)) {
			fscanf(fp, "%s", ls);
			q.push_back(ls);
			fscanf(fp, "%s", ls);
			q.push_back(ls);
			fscanf(fp, "%s", ls);
			fscanf(fp, "%s", ls);
		}
		fclose(fp);
		return q;
	}
};
