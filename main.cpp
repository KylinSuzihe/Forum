#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <cstdlib>
#include <winsock.h>
#include <mysql.h>
#include <ctime>
#include <sstream>
#include "user.h"
#pragma comment (lib, "libmysql.lib")
using namespace std;
MYSQL *mysql = new MYSQL; //mysql����  
MYSQL_FIELD *fd;    //�ֶ�������  
char field[32][32];    //���ֶ�����ά����  
MYSQL_RES *res; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char query[150]; //��ѯ���  
string q;
void linkMysql() {
	mysql_init(mysql);//������� p4302
	if (!(mysql_real_connect(mysql, "localhost", "root", "123456", "forum", 3306, NULL, 0))) {
		printf("Failed to access to the database...Error: %s\n", mysql_error(mysql));
	}
	mysql_query(mysql, "set names gbk"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd���������� 
}
User *user = new User();
string toString(char* x) {
	string s = "";
	for (int i = 0; x[i]; ++i) s += x[i];
	return s;
}
string toString(int x) {
	int c = 1;
	while (x / c) c *= 10;
	c /= 10;
	string s = "";
	while (x) {
		s += (x / c) + '0';
		x %= c;
		c /= 10;
	}
	return s;
}
int toInt(char* s) {
	int x = 0;
	for (int i = 0; s[i]; ++i) {
		x = x * 10 + (s[i] - '0');
	}
	return x;
}
void clearRes() {
	do {
	    res = mysql_use_result(mysql);
	    mysql_free_result(res);
	} while (!mysql_next_result(mysql));
}

int sqlUtils() {
	clearRes();
    for (int i = 0; i < 150; ++i) query[i] = ' ';
	for (int i = 0; i < q.size(); ++i) query[i] = q[i];
	if (mysql_query(mysql, query)) {   //ִ��SQL��� 
        printf("Query failed (%s)\n", mysql_error(mysql));
        Sleep(2000);
        return 2;
    }
    if (!(res = mysql_store_result(mysql))) {  //���sql�������󷵻صĽ����  
        //printf("Couldn't get result from %s\n", mysql_error(mysql));
        return 0;
    }
    return 1;
}

string getTime() {
	time_t now = time(0); 
    tm *ltm = localtime(&now);
 	string t = "";
    // ��� tm �ṹ�ĸ�����ɲ���
    t = toString(1900 + ltm->tm_year) + '-' + toString(1 + ltm->tm_mon) + '-' + toString(ltm->tm_mday) + ' ' 
		+ toString(ltm->tm_hour) + ":" + toString(ltm->tm_min) + ":" + toString(ltm->tm_sec);
    return t;
}

void reply(string postNumber, int f) {
	cout << "\n������ظ����ݣ�\n";
    string content;
    cin >> content;
    q = "insert into comment(author, time, content, post, likes_num, floor) values('"
    	+ (user -> name) + "','" +  getTime() + "','" + content + "','" + postNumber + "',0," + toString(f) + ")";
    if (sqlUtils() == 2) {
    	cout << "��������������������\n"; 
    }
    cout << "�ظ��ɹ���\n";
    q = "update post set reply_num = reply_num + 1 where id = " + postNumber;
	sqlUtils(); 
	Sleep(1000); 
}

void isRead(string postNumber) {
	q = "insert into user_post_map(user, post) values('" + (user -> name) + "'," + postNumber + ")";
	sqlUtils();
}

bool deleteContent(string postNumber) {
	cout << "����������Ҫɾ����¥���ţ�"; 
	string floor;
	cin >> floor;
	q = "select author from post where id = " + postNumber;
	sqlUtils();
	while (column = mysql_fetch_row(res))
		if ((user -> name) != column[0]) {
			cout << "��û��Ȩ�ޣ�\n";
			return 0;
		}
	q = "delete from comment where post = " + postNumber + " and floor = " + floor;
	sqlUtils();
	q = "select floor from comment where post = " + postNumber + " and floor > " + floor;
	sqlUtils();
	vector<int> floorList;
	while (column = mysql_fetch_row(res)) {
		floorList.push_back(toInt(column[0]));
	}
	for (int i = 0; i < floorList.size(); ++i) {
		q = "update comment set floor = " + toString(floorList[i] - 1) + " where post = " + postNumber + " and floor = " + toString(floorList[i]);
		sqlUtils();
	}
	cout << "\n\n      ɾ���ɹ���";
	Sleep(1000);
	return 1; 
}

int listContent(string postNumber) {
	system("cls");
    q = "select name from post where id = '" + postNumber + "'";
    if (sqlUtils() != 1) return -1;
    while (column = mysql_fetch_row(res)) {
    	cout << column[0] << endl;
    } 	
    cout << "--------------------------------------------------------------------------------------------------\n";
    q = "select * from comment where post = '" + postNumber + "' order by floor";
    if (sqlUtils() != 1) return -1;
    int f = 0;
    while (column = mysql_fetch_row(res)) {
    	printf("��%s¥�� %-10s ����ʱ�䣺%s\n", column[5], column[0], column[1]);
    	printf("%s\n", column[2]);
    	cout << "--------------------------------------------------------------------------------------------------\n";
    	f = max(f, toInt(column[5]));
    } 
    cout << "������R�ظ�������E���أ�����Dɾ������";
    return f;
}

void check(string postNumber) {
	isRead(postNumber); 
	int f = listContent(postNumber);
	if (f == -1) return;
    char c = getchar();
    while (1) {
	    while (c != 'R' && c != 'E' && c != 'D') c = getchar();
	    if (c == 'R') reply(postNumber, f + 1);
	    if (c == 'E') return;
	    if (c == 'D') deleteContent(postNumber);
	    f = listContent(postNumber);
	    c = getchar();
    }
}
struct Post{
	char status;
	char* id;
	char* name;
	char* author;
	char* rep;
	char* col;
	char* time;
}p;

bool cmpByIdAsc(Post a, Post b) {
	return a.id < b.id;
}
bool cmpByIdDes(Post a, Post b) {
	return a.id > b.id;
}
bool cmpByNameAsc(Post a, Post b) {
	return toString(a.name) < toString(b.name);
}
bool cmpByNameDes(Post a, Post b) {
	return toString(a.name) > toString(b.name);
}
bool cmpByRepAsc(Post a, Post b) {
	return toString(a.rep) < toString(b.rep);
}
bool cmpByRepDes(Post a, Post b) {
	return toString(a.rep) > toString(b.rep);
}
bool cmpByTimeAsc(Post a, Post b) {
	return toString(a.time) < toString(b.time);
}
bool cmpByTimeDes(Post a, Post b) {
	return toString(a.time) > toString(b.time);
}


vector<Post> post;
void listPost() {
	system("cls");
	cout << "״̬   ���      ������                          ����          �ظ�      ����ʱ��       \n";
	cout << "=============================================================================================\n";
    for (int i = 0; i < post.size(); ++i) {
    	p = post[i];
		q = "select * from user_post_map where user = '" + user -> name + "' and post = '" + toString(p.id) + "'";
    	if (sqlUtils() != 1) return;
	    res = mysql_store_result(mysql);
	    char c = mysql_affected_rows(mysql) == 0 ? '+' : '-';
        printf(" %-6c%-10s%-32s%-14s%-10s%-20s\n", c, p.id, p.name, p.author, p.rep, p.time);  //column��������  
    }
	cout << "==============================================================================================\n";
	cout << "���鿴�������������ӱ�ţ�����������C���˳�������E��ɾ��������D������������S������������O����\n";
	
}
void deletePost() {
	cout << "������ɾ����ţ�"; 
	string postNumber;
	cin >> postNumber;
	q = "select author from post where id = " + postNumber;
	sqlUtils();
	while (column = mysql_fetch_row(res))
		if ((user -> name) != column[0]) {
			cout << "��û��Ȩ�ޣ�\n";
			return;
		}
	q = "delete from post where id = " + postNumber;
	sqlUtils();
	q = "delete from comment where post = " + postNumber;
	sqlUtils();
	q = "select id from post where id > " + postNumber;
	sqlUtils();
	vector<int> id;
	while (column = mysql_fetch_row(res)) {
		id.push_back(toInt(column[0]));
	}
	for (int i = 0; i < id.size(); ++i) {
		q = "update post set id = " + toString(id[i] - 1) + " where id = " + toString(id[i]);
		sqlUtils();
		q = "update comment set post = " +  toString(id[i] - 1) + " where post = " + toString(id[i]);
		sqlUtils();
	}
	cout << "\n\n      ɾ���ɹ���";
	Sleep(1000);
}

void publishPost() {
	string name, content;
	cout << "�������������ƣ�\n";
	cin >> name;
	cout << "�������������ݣ�\n";
	cin >> content;
	
	q = "select max(id) from post";
	if (sqlUtils() != 1) return;
	char* postNumber;
	while (column = mysql_fetch_row(res)) postNumber = column[0];
	q = "insert into post(id, name, author, reply_num, col_num, time) values(" + toString(toInt(postNumber) + 1)
	    + ", '" + name + "', '" + user -> name + "', 1, 0, '" + getTime() + "')";
	if (sqlUtils() == 2) return;
	
	q = "insert into comment(author, time, content, post, likes_num, floor) values('"
    	+ (user -> name) + "','" +  getTime() + "','" + content + "','" + toString(toInt(postNumber) + 1) + "',0,1)";
    if (sqlUtils() == 2) {
    	cout << "��������������������\n";
		return; 
    }
}

void searchPost() {
	q = "select * from post";
    if (sqlUtils() != 1) return;
    char c = '+';
    //��ӡ��ȡ������  
    post.clear();
    while (column = mysql_fetch_row(res)) {
    	p.id = column[0];
    	p.name = column[1];
    	p.author = column[2];
    	p.rep = column[3];
    	p.col = column[4];
    	p.time = column[5];
    	post.push_back(p);
    } 	
}

void searchByKeyword() {
	system("cls");
	cout << "��ѡ������ģʽ��A.�ؼ�������  B.�û�������\n";
	char c = getchar();
	while (c != 'A' && c != 'B') c = getchar();
	if (c == 'A') {
		cout << "�����������ؼ��֣�";
		string keyword;
		cin >> keyword;
		q = "select name, comment.time, floor, content from comment inner join post on id = comment.post where content like '%" + keyword + "%'";
	} 
	else {
		cout << "�������û�����";
		string author;
		cin >> author;
		q = "select name, comment.time, floor, content from comment inner join post on id = comment.post where comment.author like '%" + author + "%'";
	}
	if (sqlUtils() != 1) return;
	while (column = mysql_fetch_row(res)) {
		cout << "----------------------------------------------\n";
		cout << "���������ơ���" << column[0] << endl;
		cout << "������ʱ�䡿��" << column[1] << endl;
		cout << "��¥�㡿��" << column[2] << endl;
		cout << "�����ݡ�" << column[3] << endl;
	}
	cout << "----------------------------------------------\n";
	cout << "������E���ء�\n";
	c = getchar();
	while (c != 'E') c = getchar();
	return; 
}

void order() {
	cout << "��ѡ������ؼ��֣�A.���  B.����  C.�ظ���  D.����ʱ��\n";
	char c = getchar();
	while (c < 'A' || c > 'D') c = getchar();
	cout << "��ѡ�������ǽ���A.���� B.����\n";
	char d = getchar();
	while (d != 'A' && d != 'B') d = getchar();
	if (c == 'A')
		if (d == 'A') sort(post.begin(), post.end(), cmpByIdAsc);
		else sort(post.begin(), post.end(), cmpByIdDes);
	if (c == 'B')
		if (d == 'A') sort(post.begin(), post.end(), cmpByNameAsc);
		else sort(post.begin(), post.end(), cmpByNameDes);
	if (c == 'C')
		if (d == 'A') sort(post.begin(), post.end(), cmpByRepAsc);
		else sort(post.begin(), post.end(), cmpByRepDes);
	if (c == 'D')
		if (d == 'A') sort(post.begin(), post.end(), cmpByTimeAsc);
		else sort(post.begin(), post.end(), cmpByTimeDes);
}

void showPost() { 
    searchPost();
    listPost();
	string postNumber;
	cin >> postNumber;
	while (postNumber != "E") {
		if (postNumber == "D") {
			deletePost();
			searchPost();
		} 
		else if (postNumber == "C") {
			publishPost();
			searchPost();
		}
		else if (postNumber == "S") searchByKeyword();
		else if (postNumber == "O") order();
		else {
			check(postNumber); 
			searchPost();
		} 
		listPost();
		cin >> postNumber;
	}
	
	return;
}
void logIn() {
	system("cls");
	cout << "------------------��ӭ����XXX��̳��------------------\n";
	cout << "\n\n"; 
	cout << "��¼��\n\n";
	string name, password;
	string ls;
	vector<string> q;
	q = user -> logIn();
	int b = 2;
	while (b) {
		cout << "�������û�����\n";
		cin >> name;
		cout << "���������룺\n";
		cin >> password;
		for (int i = 0; i < q.size(); i += 2) {
			ls = q[i];
			if (ls == name) {
				ls = q[i + 1];
				if (ls == password) {
					b = 0;
					cout <<  name << "��½�ɹ�!\n";
					user -> name = name;
					Sleep(1000);
					showPost(); 
					break;
				}
				if (b) cout << "�������\n", b = 1;
				break;
			}
		}
		if (b == 2) cout << "�û��������ڣ�\n";
	}
}

char drawStart() {
	system("cls");
	cout << "------------------��ӭ����XXX��̳��------------------\n";
	cout << "\n\n"; 
	cout << "            ��ѡ������Ҫ���еĲ�����\n";
	cout << "\n";
	cout << "                  ��¼�������� L \n";
	cout << "                  ע�᣺������ R \n";
	cout << "                  �˳��������� E \n";
	char c = getchar();
	while (c != 'L' && c != 'R' && c != 'E') c = getchar();
	return c;
}

void regi() {
	user->reg();
	return;
}

int main() {
	linkMysql();
	while (1) {
		char c = drawStart();
		if (c == 'L') logIn();
		else if (c == 'R') regi();
		else if (c == 'E') return 0;
	}
}

