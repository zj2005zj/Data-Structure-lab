#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100
typedef struct DoubleNode {//双链表 
	int data;
	struct DoubleNode* last;
	struct DoubleNode* next;
}node;

typedef struct Num {//数据类型
	node* head;
	node* tail;
	int n;        //位数
	int inte;   //整数部分位数
	int dec;   //小数部分位数
	int flag;   //正负
}num;

//创建链表
node* InitNode() {
	node* p = (node*)malloc(sizeof(node));
	if (!p) {
		printf("分配内存空间失败\n");
		exit(0);
	}
	return p;
}

num* NewHead() {
	num* list = (num*)malloc(sizeof(num));
	if (!list) {
		printf("分配内存空间失败\n");
		exit(0);
	}
	list->tail = NULL;
	list->head = NULL;
	list->n = 0;
	return list;
}

//尾部追加数字
void AppendList(num* list, int data) {
	node* p = (node*)malloc(sizeof(node));
	if (!p) {
		printf("内存分配失败\n");
		exit(0);
	}
	p->data = data;
	p->next = NULL; // 新节点的 next 指向 NULL
	p->last = NULL;  // 新节点的 last 初始为 NULL

	if (!list->tail) {       // 如果链表为空
		list->head = p; // 将新节点设为头节点
	}
	else {
		p->last = list->tail; // 新节点的 last 指向当前尾节点
		list->tail->next = p;  // 当前尾节点的 next 指向新节点
	}
	list->tail = p; // 更新链表的尾节点
	list->n++;      // 增加节点计数
}

//头部追加数字
void HeadInsert(num* list, int data) {
	node* p = InitNode();
	p->data = data;
	if (!list->tail) {// 如果链表为空
		list->tail = p;  // 将新节点设为尾节点
		p->next = NULL;
		p->last = NULL;
	}
	else {
		p->last = list->head->last;
		p->next = list->head;
		list->head->last = p;
	}
	list->head = p;// 将新节点设为头节点
	list->n++;       // 增加节点计数
}

//打印数字
void Print(num* list) {
	node* p = list->tail;
	int count = 1;       //位数
	if (list->flag == 0)//负数
		printf("-");

 while (p) {
		if ((count == list->inte) && p->last) {//整数部分结束
			printf("%d.", p->data);                     //打小数点
			count++;
		}
		else {
			count++;
			printf("%d", p->data);
		}
		p = p->last;
	}
}

//删除链表
void delete(num* list){
	node* p = list->head;
	while (p) {
		node* cur = p;
		p = p->next;
		free(cur);
	}
	free(list);
}
//读取数字
char ReadWrite(num* list) {
	char c = getchar();
	list->flag = 1, list->inte = 0;
	if (c == '-')
		list->flag = 0;
	else 
		HeadInsert(list, c - '0');
	while ((c = getchar()) >= '0' && c <= '9' || c == '.') {
		if (c == '.') {
			list->inte = list->n;
		}
		else {
			HeadInsert(list, c - '0');
		}
	}
	if (!list->inte) {//只有整数没有小数部分
		list->inte = list->n;
	}
	list->dec = list->n - list->inte;
	return c;
}
//比大小
int Compare(num* a, num* b) {
	if (a->inte != b->inte)//整数部分位数不同
		return a->inte - b->inte;
	node* pa = a->tail;//从最高位开始比
	node* pb = b->tail;
	while (pa && pb && pa->data == pb->data) {
		pa = pa->last, pb = pb->last;
	}
	if (pa && pb)	
		return pa->data - pb->data;
	else if (pa) 
		return 1;
	else if (pb) 
		return -1;
	else 
		return 0;
}
//加法

num* Minus(num* a, num* b);
num* Add(num* a, num* b) {
	if (a->flag == 0 && b->flag == 1) {
		a->flag = 1; return Minus(b, a); 
	}
	else if (a->flag == 1 && b->flag == 0) {
		b->flag = 1; return Minus(a, b);
	}

	num* ans = NewHead();

	if (a->flag == 0 && b->flag == 0)
		ans->flag = 0;
	else 
		ans->flag = 1;

	if (a->dec < b->dec) { 
		num* q = a; a = b; b = q; 
	}
	ans->dec = a->dec;
	for (int i = 0; i < a->dec - b->dec; i++) { 
		HeadInsert(b, 0); 
	}
	a->dec = b->dec;
	if (a->inte < b->inte) {
		num* q = a; a = b; b = q; 
	}
	int da, db, temp = 0;
	node* pa = a->head;
	node* pb = b->head;
	for (int i = 0; i < a->n; i++) {
		if (pa) { 
			da = pa->data; pa = pa->next; 
		}
		else 
			da = 0;
		if (pb) { 
			db = pb->data; pb = pb->next; 
		}
		else 
			db = 0;
		temp += da + db;
		AppendList(ans, temp % 10);
		temp /= 10;
	}
	ans->inte = a->inte;
	if (temp) {
		AppendList(ans, 1);
		ans->inte++;
	}
	return ans;
}

//减法
num* Minus(num* a, num* b) {
	num* ans = NewHead();
	if (a->flag == 0 && b->flag == 1){
		b->flag = 0;
		return Add(a, b);
	}
	else if (a->flag == 1 && b->flag == 0) {
		b->flag = 1;
		return Add(a, b);
	}
	else if (a->flag == 0 && b->flag == 0) {
		a->flag = 1, b->flag = 1;
		Minus(b, a);
	}
	ans->flag = 1;
	int flag = Compare(a, b);
	if (flag < 0) { 
		num* p = a; 
		a = b; b = p; 
		ans->flag = 0;
	}
	else if (flag == 0) { 
		HeadInsert(ans, 0); 
		return ans; 
	}
	if (a->dec < b->dec) { 
		num* q = a; a = b; b = q; 
	}
	ans->dec = a->dec;
	for (int i = 0; i < a->dec - b->dec; i++) { 
		HeadInsert(b, 0); 
	}
	a->dec = b->dec;
	if (a->inte < b->inte) {
		num* q = a; a = b; b = q;
	}
	int da, db, temp = 0;
	node* pa = a->head;
	node* pb = b->head;
	for (int i = 0; i < a->n; i++) {
		if (pa) {
			da = pa->data; pa = pa->next;
		}
		else
			da = 0;
		if (pb) {
			db = pb->data; pb = pb->next;
		}
		else
			db = 0;
		temp += da - db;
		if (temp < 0) {
			temp += 10;
			AppendList(ans, temp % 10);
			temp = -1;
		}
		else {
			AppendList(ans, temp);
			temp = 0;
		}
	}
	ans->inte = a->inte;
	ans->dec = ans->n - ans->inte;
	while (ans->dec > 0 && ans->head != NULL && !ans->head->data) {
		node* old_head = ans->head;
		ans->head = ans->head->next; 

		
		if (ans->head != NULL) {
			ans->head->last = NULL; 
		}

		free(old_head); 
		ans->dec--; 
		ans->inte--; 
	}
	while (ans->inte > 1 && !ans->tail->data) { 
		pa = ans->tail; 
		pb = ans->tail->last;
		ans->tail->last->next = NULL; 
		free(pa); 
		ans->tail = pb;
		ans->inte--; 
		ans->n--; 
	}
	return ans;
}

//乘法
num* Multi(num* a, num* b) {
	num* ans = NewHead();
	ans->flag = 1;
	ans->flag = (a->flag == b->flag) ? 1 : 0;
	ans->dec = a->dec + b->dec; ans->inte = a->inte + b->inte;
	for (int i = 0; i < ans->inte + ans->dec; i++)
		HeadInsert(ans, 0);
	node* pa = a->head, * p = ans->head;
	for (int i = 0; i < a->n; i++) {
		node* q = p, * pb = b->head;
		for (int j = 0; j < b->n; j++) {
			q->data += pa->data * pb->data;
			pb = pb->next;
			q = q->next;
		}
		p = p->next;
		pa = pa->next;
	}
	pa = ans->head;
	while (pa->next) {
		if (pa->data > 10) {
			int temp = pa->data / 10;
			pa->data %= 10;
			pa->next->data += temp;
		}
		pa = pa->next;
	}
	node* pb = NULL;
	while (ans->dec > 0 && !ans->head->data) { 
		pa = ans->head;
		pb = ans->head->next;
		ans->head->next->last = NULL;
		free(pa); 
		ans->head = pb; 
		ans->dec--; 
		ans->n--; 
	}
	while (ans->inte > 1 && !ans->tail->data) { 
		pa = ans->tail; 
		pb = ans->tail->last; 
		ans->tail->last->next = NULL;
		free(pa);
		ans->tail = pb;
		ans->inte--;
		ans->n--; 
	}
	return ans;
}
//乘方
num* Pow(num* a, int n) {
	int count = 1;
	num* ans = NewHead();
	ans = a;
	while (count < n) {
		ans = Multi(ans, a);
		count++;
	}
	return ans;
}
//除法
num* divide(num* a, num* b, int n) {
	num* ans = NewHead(), * leave = NewHead();
	ans->flag = (a->flag == b->flag) ? 1 : 0;
	ans->inte = 0, ans->dec = 0, leave->inte = 0, leave->dec = 0, leave->flag = 1;
	a->flag = 1, b->flag = 1;
	if (a->n == 1 && a->head->data == 0) { HeadInsert(ans, 0), ans->dec++; return ans; }
	while (b->dec) {
		b->dec--, b->inte++;
		if (a->dec) { a->dec--, a->inte++; }
		else { HeadInsert(a, 0), a->inte++; }
	}
	int dec = a->dec;
	if (a->dec < n)for (int i = 0; i < n - dec; i++) { HeadInsert(a, 0), a->dec++; }
	node* pa = a->tail, * pb;
	for (int i = 0; i < a->inte + n; i++, pa = pa->last) {
		HeadInsert(ans, 0), HeadInsert(leave, pa->data), leave->inte++;
		i >= a->inte ? ans->dec++ : ans->inte++;
		while (Compare(leave, b) >= 0) {
			leave = Minus(leave, b);
			ans->head->data++;
		}
	}
	while (ans->dec > 0 && !ans->head->data) {
		pa = ans->head;
		pb = ans->head->next; 
		ans->head->next->last = NULL; 
		free(pa); 
		ans->head = pb; 
		ans->dec--; 
		ans->n--;
}
	while (ans->inte > 1 && !ans->tail->data) {
		pa = ans->tail; 
		pb = ans->tail->last; 
		ans->tail->last->next = NULL;
		free(pa);
		ans->tail = pb; 
		ans->inte--;
		ans->n--; 
	}
	return ans;
}
//取余
num* Mod(num* a, num* b) {
	num* temp = divide(a, b, 0);//整除部分
	num* ans = Minus(a, Multi(b, temp));
	free(temp);
	return ans;
}
num* intnum(num* a) { // 求整数部分
	num* integerPart = NewHead();
	node* pt = a->tail;
	int count = 0; // 用于计数

	// 从尾部开始遍历，直到整数部分结束
	while (pt != NULL && count < a->inte) {
		HeadInsert(integerPart, pt->data);
		pt = pt->last;
		count++;
	}

	integerPart->inte = a->inte;
	integerPart->dec = 0;
	integerPart->flag = a->flag;
	return integerPart;
}
// 将整数部分转化为int型
int intdnum(num* a) {
	num* integerPart = NewHead();
	node* pt = a->tail;
	int count = a->inte; // 用于计数
	int ans = 0;
	// 从尾部开始遍历，直到整数部分结束
	while (pt != NULL && count>0) {
		ans += pt->data * pow(10, count - 1);
		pt = pt->last;
		count--;
	}
	return ans;
}
double decdnum(num* a,double w) { // 将小数部分转化为实型
	num* decimalPart = NewHead();
	node* pt = a->tail;
	int count = 0; // 用于计数
	double ans = 0;
	while (pt != NULL && count < a->dec) {
		ans += pt->data * pow(w, count+1);
		pt = pt->last;
		count++;
	}
	return ans;
}

num* decnum(num* a) { // 求小数部分
	num* decimalPart = NewHead();
	node* ph = a->head;
	int count = 0; // 用于计数

	// 从头开始遍历，直到小数部分结束
	while (ph != NULL && count < a->dec) {
		AppendList(decimalPart, ph->data);
		ph = ph->next;
		count++;
	}

	decimalPart->inte = 0;
	decimalPart->dec = a->dec;
	decimalPart->flag = a->flag;
	return decimalPart;
}
// 函数将十进制整数转换为 n 进制整数
int decimalToNBase(int a, int N) {
	int ans = 0;
	int temp = 0;
	int multiplier = 1; 
	while (a > 0) {
		temp = a % N; 
		ans = ans + temp * multiplier; 
		a = a / N; 
		multiplier *= 10; 
	}

	return ans;
}
int nBaseToDecimal(int nBaseNum, int n) {
	int decimalValue = 0;
	int multiplier = 1; // 用于从最低位开始计算权值
	while (nBaseNum > 0) {
		int currentDigit = nBaseNum % 10; // 获取当前位的值
		decimalValue += currentDigit * multiplier; // 累加加权值
		nBaseNum /= 10; // 更新 n 进制数
		multiplier *= n; // 更新权值
	}
	return decimalValue;
}
void Con1(num* a, int n, num* N) { // 十进制转换为 N 进制
	num* ans = NewHead(); // 用于存储小数部分的转换结果
	num* integerPart = intnum(a); // 提取整数部分
	num* decimalPart = decnum(a); // 提取小数部分
	num* n1 = NewHead();
	int count = 0; // 用于控制小数部分的转换精度
	if (a->flag == 0) {
		printf("-");
	}
	n1 = intnum(N);
	int p = intdnum(integerPart);
	int n2 = intdnum(n1);
	int result = decimalToNBase(p, n2);
	printf("%d.", result);
	// 进制转换，小数部分
	ans->inte = 0; // 小数部分的整数长度为0
	ans->dec = 0;
	while (decimalPart->dec != 0 && count < n) {
		HeadInsert(ans, intnum(Multi(decimalPart, N))->head->data); // 将整数部分添加到结果中
		decimalPart = decnum(Multi(decimalPart, N)); // 更新小数部分
		ans->dec++;
		count++; // 增加转换精度计数
	}
	Print(ans);
	
	// 释放临时变量
	delete(integerPart);
	delete(decimalPart);
	delete(ans);
}

void Con2(num* a,num* N) {
	num* ans1 = NewHead(); // 用于存储整数部分的转换结果
	num* ans2 = NewHead(); // 用于存储小数部分的转换结果
	num* integerPart = intnum(a); // 提取整数部分
	num* decimalPart = decnum(a); // 提取小数部分
	num* n1 = NewHead();
	int count = 0; // 用于控制小数部分的转换精度
	if (a->flag == 0) {
		printf("-");
	}
	n1 = intnum(N);
	int p = intdnum(integerPart);
	int n2 = intdnum(n1);
	int result1 = nBaseToDecimal(p, n2);
	double w = 1.0 / n2;
	double result2 = decdnum(decimalPart, w);
	double result = result1 + result2;
	printf("%lf", result);

}





num* input(double x, int n) {//将实数转化为num型数据
	num* ans = NewHead();
	int count = 0;
	int inte = (int)x;
	double dec = x - inte;

	// 处理整数部分
	int temp = inte;
	while (temp > 0) {
		HeadInsert(ans, temp % 10);
		temp /= 10;
		count++;
	}
	ans->inte = count;

	// 处理小数部分
	count = 0;
	while (count < n && dec > 0) {
		dec *= 10;
		HeadInsert(ans, (int)dec);
		dec -= (int)dec;
		count++;
	}
	ans->dec = count;
	if (x > 0)
		ans->flag = 1;
	else
		ans->flag = 0;

	return ans;
}

num* PolyCal(num* x,int n) {
	num* ans = NewHead();
	num* a1 = NewHead();
	num* a2 = NewHead();
	num* a3 = NewHead();

	double x1 = 3.0 / 7;
	double x2 = 1.0 / 3;
	a1 = input(x1, n);
	a2 = input(x2, n);
	a3 = input(4.0, n);

	num* temp1 = Multi(x, x);
	temp1 = Multi(temp1, x);
	temp1 = Multi(temp1, a1);
	num* temp2 = Multi(x, x);
	temp2 = Multi(temp2, a2);
	ans = Add(Minus(temp1, temp2), a3);
	 
	delete(a1);
	delete(a2);
	delete(a3);
	delete(temp1);
	delete(temp2);
	return ans;
}
#if 1
void cal(int n) {
	while(1){
		num* x = NewHead(), * y = NewHead(), * ans = NewHead();
		num* N1 = NewHead();
		char op;
		// 先输入第一个数
		
		printf("请输入第一个数：");
		ReadWrite(x);
		// 输入运算符
		printf("请输入运算符 (+, -, *, /)：");
		op = getchar(); // 读取运算符
		while (getchar() != '\n'); // 消耗输入行中的剩余字符（包括换行符）

		// 输入第二个数
		printf("请输入第二个数：");
		ReadWrite(y);

		printf("请输入进制：\n");
		ReadWrite(N1);
		// 根据运算符执行相应的运算
		if (op == '+') {
			Print(x);
			printf(" + ");
			Print(y);
			printf(" = ");
			ans = Add(x, y);
		}
		else if (op == '-') {
			Print(x);
			printf(" - ");
			Print(y);
			printf(" = ");
			ans = Minus(x, y);
		}
		else if (op == '*') {
			Print(x);
			printf(" * ");
			Print(y);
			printf(" = ");
			ans = Multi(x, y);
		}
		else if (op == '/') {
			Print(x);
			printf(" / ");
			Print(y);
			printf(" = ");
			ans = Mod(x, y);
		}
		else {
			printf("无效的运算符！\n");
		}
		
		Print(ans);
		printf("\n");
		Print(N1);
		printf("进制下:");
		Con1(ans, n, N1);
		printf("\n");

		// 清理内存
		delete(x);
		delete(y);
		delete(ans);
		delete(N1);
		
	}
}
#endif
#if 0
void cal(int n) {
	num* x = NewHead(), * y = NewHead(), * ans = NewHead();
	num* N1 = NewHead();
	printf("请输入进制：\n");
	ReadWrite(N1);
	char c;
	while (1) {
		printf("请输入式子：");
		c = ReadWrite(x), ReadWrite(y);
		if (c == '+') {
			Print(x), printf(" + "), Print(y), printf(" = ");
			ans = Add(x, y);
		}
		else if (c == '-') {
			Print(x), printf(" - "), Print(y), printf(" = ");
			ans = Minus(x, y);
		}
		else if (c == '*') {
			Print(x), printf(" * "), Print(y), printf(" = ");
			ans = Multi(x, y);
		}
		
		Print(ans);
		printf("\n");
		
		Print(N1);
		printf("进制下:");
		Con1(ans, n, N1);
		printf("\n");	
		break;
	}
	delete(x);
	delete(y);
	delete(ans);
	delete(N1);
}
#endif
void parsePolynomial(const char* poly, double* coeffs, int* exps, char* ops, int* length) {
	int i = 0, j = 0;
	char temp[MAX_LENGTH];
	int tempIndex = 0;
	double coeffSign = 1.0; // 系数的符号，默认为正

	*length = 0;

	while (poly[i] != '\0') {
		// 跳过空格
		if (isspace(poly[i])) {
			i++;
			continue;
		}

		// 读取运算符（如果是第一个字符，默认运算符为 +）
		if (poly[i] == '+' || poly[i] == '-') {
			if (*length > 0) { // 非首项时，存储运算符
				ops[*length - 1] = poly[i];
			}
			coeffSign = (poly[i] == '-') ? -1.0 : 1.0; // 更新系数符号
			i++;
			continue;
		}
		else if (*length == 0) {
			ops[*length] = '+'; // 第一个项的默认运算符为 '+'
		}

		// 解析系数
		tempIndex = 0;
		memset(temp, 0, sizeof(temp));
		while (isdigit(poly[i]) || poly[i] == '.' || (poly[i] == '-' && tempIndex == 0)) {
			if (poly[i] == '-' && tempIndex == 0 && (*length == 0 || ops[*length - 1] == '+')) {
				temp[tempIndex++] = poly[i++];
				continue;
			}
			temp[tempIndex++] = poly[i++];
		}
		temp[tempIndex] = '\0'; // 结束字符串
		coeffs[*length] = fabs(atof(temp)); // 存储系数的正值

		// 解析指数
		if (poly[i] == 'x') {
			i++; // 跳过 'x'
			if (poly[i] == '^') {
				i++; // 跳过 '^'
				tempIndex = 0;
				memset(temp, 0, sizeof(temp));
				while (isdigit(poly[i])) {
					temp[tempIndex++] = poly[i++];
				}
				temp[tempIndex] = '\0';
				exps[*length] = atoi(temp);
			}
			else {
				exps[*length] = 1; // 默认指数为 1
			}
		}
		else {
			exps[*length] = 0; // 常数项的指数为 0
		}

		(*length)++;
	}

	// 确保最后一个运算符不存储
	if (*length > 0) {
		ops[*length - 1] = '\0';
	}
}
#if 1
num* calculatePolynomial(double x, double* coeffs, int* exps, char* ops, int length, int n) {
	num* xn = input(x, n);
	num* v1 = NewHead();
	num* v2 = NewHead();
	num* m = NewHead();
	num* value = NewHead();
	num* temp = NewHead();
	num* term = NewHead();
	v1 = input(coeffs[0], n);
	v2 = Pow(xn, exps[0]);
	// 第一个项处理
	 // 计算第一个项
	value = Multi(v1, v2);
	delete(v1);
	delete(v2);
	// 处理后续项
	for (int i = 1; i < length; i++) {
		m = input(coeffs[i], n); // 直接获取内存指针，无需再初始化
		temp = Pow(xn, exps[i]); // 直接获取内存指针，无需再初始化
		term = Multi(m, temp);   // 计算当前项
		// 根据前一个运算符调整当前项的符号
		if (ops[i - 1] == '-') {
			value = Minus(value, term); // 当前运算符为 '-'，从总值中减去当前项
		}
		else if((ops[i - 1] == '+')) {
			value = Add(value, term); // 当前运算符为 '+'，添加当前项到总值
			}
		}
	delete(term);
	delete(m);
	delete(temp);
	return value; // 返回多项式的值
}
#endif
int main (){
	int n = 200;
	
	while (getchar() != '\n');
#if 0
	printf("十转N\n");
	num* a1 = NewHead();
	num* N1 = NewHead();
	printf("请输入一个数：");
	ReadWrite(a1);
	printf("请输入进制：\n");
	ReadWrite(N1);
	Con1(a1, n, N1);
	printf("\n");
	delete(a1);
	delete(N1);
#endif	
	
#if 0
	printf("N转十\n");
	num* a2 = NewHead();
	num* N2= NewHead();
	printf("请输入第一个数：");
	ReadWrite(a2);
	printf("请输入进制：\n");
	ReadWrite(N2);
	Con2(a2,N2);
	printf("\n");
	delete(a2);
	delete(N2);
#endif 
#if 0
	printf("四则运算:\n");
	cal(n);
	
#endif
	
#if 0
	printf("多项式1:\n");
	double x;
	printf("请输入x：\n");
	scanf("%lf", &x);
	num* nx = input(x, n);
	Print(PolyCal(nx, n));
	printf("\n");
	delete(nx);
#endif
#if 1
	    double y;
	    printf("多项式终极版:\n");
		char poly[MAX_LENGTH]; // 存储用户输入的多项式
		double coeffs[MAX_LENGTH]; // 系数数组
		int exps[MAX_LENGTH];      // 指数数组
		char ops[MAX_LENGTH];      // 运算符数组
		int length;                // 数组长度
		num* result = NewHead();
		// 获取用户输入的多项式
		printf("请输入多项式: ");
		fgets(poly, MAX_LENGTH, stdin);
		poly[strcspn(poly, "\n")] = 0; // 去除换行符

		// 解析多项式
		parsePolynomial(poly, coeffs, exps, ops, &length);

		// 输出结果
		printf("多项式: %s\n", poly);
		printf("系数: ");
		for (int i = 0; i < length; i++) {
			printf("%f ", coeffs[i]);
		}
		printf("\n");

		printf("指数: ");
		for (int i = 0; i < length; i++) {
			printf("%d ", exps[i]);
		}
		printf("\n");

		printf("运算符: ");
		for (int i = 0; i < length - 1; i++) {
			printf("%c ", ops[i]);
		}
		printf("\n");

		// 获取用户输入的x值
		printf("请输入x的值: ");
		scanf("%lf", &y);
		while (getchar() != '\n');
		// 计算多项式的值
		
		result = calculatePolynomial(y, coeffs, exps, ops, length,n);
		Print(result);
		delete(result);
#endif
    return 0;
}
//
//                      _oo0oo_
//                    o8888888o
//                      88" . "88
//                      (|  -_  - |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \\|     |// '.
//                 / \\|||  :  |||// \
//                / _||||| -:- |||||- \
//               |   | \\\  - /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          ."" '<  `.___\_<|>_/___.' >' "".
//         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//         \  \ `_.   \_ __\ /__ _/   .-` /  /
//     =====`-.____`.___ \_____/___.-`___.-'=====
//                       `=---='
//
//
//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//           佛祖保佑       永不宕机     永无BUG
//








