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
MYSQL *mysql = new MYSQL; //mysql连接  
MYSQL_FIELD *fd;    //字段列数组  
char field[32][32];    //存字段名二维数组  
MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列  
char query[150]; //查询语句  
string q;
void linkMysql() {
	mysql_init(mysql);//分配对象 p4302
	if (!(mysql_real_connect(mysql, "localhost", "root", "123456", "forum", 3306, NULL, 0))) {
		printf("Failed to access to the database...Error: %s\n", mysql_error(mysql));
	}
	mysql_query(mysql, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码 
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
	if (mysql_query(mysql, query)) {   //执行SQL语句 
        printf("Query failed (%s)\n", mysql_error(mysql));
        Sleep(2000);
        return 2;
    }
    if (!(res = mysql_store_result(mysql))) {  //获得sql语句结束后返回的结果集  
        //printf("Couldn't get result from %s\n", mysql_error(mysql));
        return 0;
    }
    return 1;
}

string getTime() {
	time_t now = time(0); 
    tm *ltm = localtime(&now);
 	string t = "";
    // 输出 tm 结构的各个组成部分
    t = toString(1900 + ltm->tm_year) + '-' + toString(1 + ltm->tm_mon) + '-' + toString(ltm->tm_mday) + ' ' 
		+ toString(ltm->tm_hour) + ":" + toString(ltm->tm_min) + ":" + toString(ltm->tm_sec);
    return t;
}

void reply(string postNumber, int f) {
	cout << "\n请输入回复内容：\n";
    string content;
    cin >> content;
    q = "insert into comment(author, time, content, post, likes_num, floor) values('"
    	+ (user -> name) + "','" +  getTime() + "','" + content + "','" + postNumber + "',0," + toString(f) + ")";
    if (sqlUtils() == 2) {
    	cout << "啊啊啊啊啊啊啊啊啊啊\n"; 
    }
    cout << "回复成功！\n";
    q = "update post set reply_num = reply_num + 1 where id = " + postNumber;
	sqlUtils(); 
	Sleep(1000); 
}

void isRead(string postNumber) {
	q = "insert into user_post_map(user, post) values('" + (user -> name) + "'," + postNumber + ")";
	sqlUtils();
}

bool deleteContent(string postNumber) {
	cout << "请输入您想要删除的楼层编号："; 
	string floor;
	cin >> floor;
	q = "select author from post where id = " + postNumber;
	sqlUtils();
	while (column = mysql_fetch_row(res))
		if ((user -> name) != column[0]) {
			cout << "您没有权限！\n";
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
	cout << "\n\n      删除成功！";
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
    	printf("【%s楼】 %-10s 发表时间：%s\n", column[5], column[0], column[1]);
    	printf("%s\n", column[2]);
    	cout << "--------------------------------------------------------------------------------------------------\n";
    	f = max(f, toInt(column[5]));
    } 
    cout << "【输入R回复，输入E返回，输入D删除】：";
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
	cout << "状态   编号      主题名                          作者          回复      发布时间       \n";
	cout << "=============================================================================================\n";
    for (int i = 0; i < post.size(); ++i) {
    	p = post[i];
		q = "select * from user_post_map where user = '" + user -> name + "' and post = '" + toString(p.id) + "'";
    	if (sqlUtils() != 1) return;
	    res = mysql_store_result(mysql);
	    char c = mysql_affected_rows(mysql) == 0 ? '+' : '-';
        printf(" %-6c%-10s%-32s%-14s%-10s%-20s\n", c, p.id, p.name, p.author, p.rep, p.time);  //column是列数组  
    }
	cout << "==============================================================================================\n";
	cout << "【查看帖子请输入帖子编号，发帖请输入C，退出请输入E，删除请输入D，查找请输入S，排序请输入O：】\n";
	
}
void deletePost() {
	cout << "请输入删帖编号："; 
	string postNumber;
	cin >> postNumber;
	q = "select author from post where id = " + postNumber;
	sqlUtils();
	while (column = mysql_fetch_row(res))
		if ((user -> name) != column[0]) {
			cout << "您没有权限！\n";
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
	cout << "\n\n      删除成功！";
	Sleep(1000);
}

void publishPost() {
	string name, content;
	cout << "请输入帖子名称：\n";
	cin >> name;
	cout << "请输入帖子内容：\n";
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
    	cout << "啊啊啊啊啊啊啊啊啊啊\n";
		return; 
    }
}

void searchPost() {
	q = "select * from post";
    if (sqlUtils() != 1) return;
    char c = '+';
    //打印获取的数据  
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
	cout << "请选择搜索模式：A.关键字搜索  B.用户名搜索\n";
	char c = getchar();
	while (c != 'A' && c != 'B') c = getchar();
	if (c == 'A') {
		cout << "请输入搜索关键字：";
		string keyword;
		cin >> keyword;
		q = "select name, comment.time, floor, content from comment inner join post on id = comment.post where content like '%" + keyword + "%'";
	} 
	else {
		cout << "请输入用户名：";
		string author;
		cin >> author;
		q = "select name, comment.time, floor, content from comment inner join post on id = comment.post where comment.author like '%" + author + "%'";
	}
	if (sqlUtils() != 1) return;
	while (column = mysql_fetch_row(res)) {
		cout << "----------------------------------------------\n";
		cout << "【帖子名称】：" << column[0] << endl;
		cout << "【发表时间】：" << column[1] << endl;
		cout << "【楼层】：" << column[2] << endl;
		cout << "【内容】" << column[3] << endl;
	}
	cout << "----------------------------------------------\n";
	cout << "【输入E返回】\n";
	c = getchar();
	while (c != 'E') c = getchar();
	return; 
}

void order() {
	cout << "请选择排序关键字：A.编号  B.名称  C.回复数  D.创建时间\n";
	char c = getchar();
	while (c < 'A' || c > 'D') c = getchar();
	cout << "请选择升序还是降序：A.升序 B.降序\n";
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
	cout << "------------------欢迎来到XXX论坛！------------------\n";
	cout << "\n\n"; 
	cout << "登录：\n\n";
	string name, password;
	string ls;
	vector<string> q;
	q = user -> logIn();
	int b = 2;
	while (b) {
		cout << "请输入用户名：\n";
		cin >> name;
		cout << "请输入密码：\n";
		cin >> password;
		for (int i = 0; i < q.size(); i += 2) {
			ls = q[i];
			if (ls == name) {
				ls = q[i + 1];
				if (ls == password) {
					b = 0;
					cout <<  name << "登陆成功!\n";
					user -> name = name;
					Sleep(1000);
					showPost(); 
					break;
				}
				if (b) cout << "密码错误！\n", b = 1;
				break;
			}
		}
		if (b == 2) cout << "用户名不存在！\n";
	}
}

char drawStart() {
	system("cls");
	cout << "------------------欢迎来到XXX论坛！------------------\n";
	cout << "\n\n"; 
	cout << "            请选择您需要进行的操作：\n";
	cout << "\n";
	cout << "                  登录：请输入 L \n";
	cout << "                  注册：请输入 R \n";
	cout << "                  退出：请输入 E \n";
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

