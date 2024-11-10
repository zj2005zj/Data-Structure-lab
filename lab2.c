#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100
typedef struct DoubleNode {//˫���� 
	int data;
	struct DoubleNode* last;
	struct DoubleNode* next;
}node;

typedef struct Num {//��������
	node* head;
	node* tail;
	int n;        //λ��
	int inte;   //��������λ��
	int dec;   //С������λ��
	int flag;   //����
}num;

//��������
node* InitNode() {
	node* p = (node*)malloc(sizeof(node));
	if (!p) {
		printf("�����ڴ�ռ�ʧ��\n");
		exit(0);
	}
	return p;
}

num* NewHead() {
	num* list = (num*)malloc(sizeof(num));
	if (!list) {
		printf("�����ڴ�ռ�ʧ��\n");
		exit(0);
	}
	list->tail = NULL;
	list->head = NULL;
	list->n = 0;
	return list;
}

//β��׷������
void AppendList(num* list, int data) {
	node* p = (node*)malloc(sizeof(node));
	if (!p) {
		printf("�ڴ����ʧ��\n");
		exit(0);
	}
	p->data = data;
	p->next = NULL; // �½ڵ�� next ָ�� NULL
	p->last = NULL;  // �½ڵ�� last ��ʼΪ NULL

	if (!list->tail) {       // �������Ϊ��
		list->head = p; // ���½ڵ���Ϊͷ�ڵ�
	}
	else {
		p->last = list->tail; // �½ڵ�� last ָ��ǰβ�ڵ�
		list->tail->next = p;  // ��ǰβ�ڵ�� next ָ���½ڵ�
	}
	list->tail = p; // ���������β�ڵ�
	list->n++;      // ���ӽڵ����
}

//ͷ��׷������
void HeadInsert(num* list, int data) {
	node* p = InitNode();
	p->data = data;
	if (!list->tail) {// �������Ϊ��
		list->tail = p;  // ���½ڵ���Ϊβ�ڵ�
		p->next = NULL;
		p->last = NULL;
	}
	else {
		p->last = list->head->last;
		p->next = list->head;
		list->head->last = p;
	}
	list->head = p;// ���½ڵ���Ϊͷ�ڵ�
	list->n++;       // ���ӽڵ����
}

//��ӡ����
void Print(num* list) {
	node* p = list->tail;
	int count = 1;       //λ��
	if (list->flag == 0)//����
		printf("-");

 while (p) {
		if ((count == list->inte) && p->last) {//�������ֽ���
			printf("%d.", p->data);                     //��С����
			count++;
		}
		else {
			count++;
			printf("%d", p->data);
		}
		p = p->last;
	}
}

//ɾ������
void delete(num* list){
	node* p = list->head;
	while (p) {
		node* cur = p;
		p = p->next;
		free(cur);
	}
	free(list);
}
//��ȡ����
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
	if (!list->inte) {//ֻ������û��С������
		list->inte = list->n;
	}
	list->dec = list->n - list->inte;
	return c;
}
//�ȴ�С
int Compare(num* a, num* b) {
	if (a->inte != b->inte)//��������λ����ͬ
		return a->inte - b->inte;
	node* pa = a->tail;//�����λ��ʼ��
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
//�ӷ�

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

//����
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

//�˷�
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
//�˷�
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
//����
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
//ȡ��
num* Mod(num* a, num* b) {
	num* temp = divide(a, b, 0);//��������
	num* ans = Minus(a, Multi(b, temp));
	free(temp);
	return ans;
}
num* intnum(num* a) { // ����������
	num* integerPart = NewHead();
	node* pt = a->tail;
	int count = 0; // ���ڼ���

	// ��β����ʼ������ֱ���������ֽ���
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
// ����������ת��Ϊint��
int intdnum(num* a) {
	num* integerPart = NewHead();
	node* pt = a->tail;
	int count = a->inte; // ���ڼ���
	int ans = 0;
	// ��β����ʼ������ֱ���������ֽ���
	while (pt != NULL && count>0) {
		ans += pt->data * pow(10, count - 1);
		pt = pt->last;
		count--;
	}
	return ans;
}
double decdnum(num* a,double w) { // ��С������ת��Ϊʵ��
	num* decimalPart = NewHead();
	node* pt = a->tail;
	int count = 0; // ���ڼ���
	double ans = 0;
	while (pt != NULL && count < a->dec) {
		ans += pt->data * pow(w, count+1);
		pt = pt->last;
		count++;
	}
	return ans;
}

num* decnum(num* a) { // ��С������
	num* decimalPart = NewHead();
	node* ph = a->head;
	int count = 0; // ���ڼ���

	// ��ͷ��ʼ������ֱ��С�����ֽ���
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
// ������ʮ��������ת��Ϊ n ��������
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
	int multiplier = 1; // ���ڴ����λ��ʼ����Ȩֵ
	while (nBaseNum > 0) {
		int currentDigit = nBaseNum % 10; // ��ȡ��ǰλ��ֵ
		decimalValue += currentDigit * multiplier; // �ۼӼ�Ȩֵ
		nBaseNum /= 10; // ���� n ������
		multiplier *= n; // ����Ȩֵ
	}
	return decimalValue;
}
void Con1(num* a, int n, num* N) { // ʮ����ת��Ϊ N ����
	num* ans = NewHead(); // ���ڴ洢С�����ֵ�ת�����
	num* integerPart = intnum(a); // ��ȡ��������
	num* decimalPart = decnum(a); // ��ȡС������
	num* n1 = NewHead();
	int count = 0; // ���ڿ���С�����ֵ�ת������
	if (a->flag == 0) {
		printf("-");
	}
	n1 = intnum(N);
	int p = intdnum(integerPart);
	int n2 = intdnum(n1);
	int result = decimalToNBase(p, n2);
	printf("%d.", result);
	// ����ת����С������
	ans->inte = 0; // С�����ֵ���������Ϊ0
	ans->dec = 0;
	while (decimalPart->dec != 0 && count < n) {
		HeadInsert(ans, intnum(Multi(decimalPart, N))->head->data); // ������������ӵ������
		decimalPart = decnum(Multi(decimalPart, N)); // ����С������
		ans->dec++;
		count++; // ����ת�����ȼ���
	}
	Print(ans);
	
	// �ͷ���ʱ����
	delete(integerPart);
	delete(decimalPart);
	delete(ans);
}

void Con2(num* a,num* N) {
	num* ans1 = NewHead(); // ���ڴ洢�������ֵ�ת�����
	num* ans2 = NewHead(); // ���ڴ洢С�����ֵ�ת�����
	num* integerPart = intnum(a); // ��ȡ��������
	num* decimalPart = decnum(a); // ��ȡС������
	num* n1 = NewHead();
	int count = 0; // ���ڿ���С�����ֵ�ת������
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





num* input(double x, int n) {//��ʵ��ת��Ϊnum������
	num* ans = NewHead();
	int count = 0;
	int inte = (int)x;
	double dec = x - inte;

	// ������������
	int temp = inte;
	while (temp > 0) {
		HeadInsert(ans, temp % 10);
		temp /= 10;
		count++;
	}
	ans->inte = count;

	// ����С������
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
		// �������һ����
		
		printf("�������һ������");
		ReadWrite(x);
		// ���������
		printf("����������� (+, -, *, /)��");
		op = getchar(); // ��ȡ�����
		while (getchar() != '\n'); // �����������е�ʣ���ַ����������з���

		// ����ڶ�����
		printf("������ڶ�������");
		ReadWrite(y);

		printf("��������ƣ�\n");
		ReadWrite(N1);
		// ���������ִ����Ӧ������
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
			printf("��Ч���������\n");
		}
		
		Print(ans);
		printf("\n");
		Print(N1);
		printf("������:");
		Con1(ans, n, N1);
		printf("\n");

		// �����ڴ�
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
	printf("��������ƣ�\n");
	ReadWrite(N1);
	char c;
	while (1) {
		printf("������ʽ�ӣ�");
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
		printf("������:");
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
	double coeffSign = 1.0; // ϵ���ķ��ţ�Ĭ��Ϊ��

	*length = 0;

	while (poly[i] != '\0') {
		// �����ո�
		if (isspace(poly[i])) {
			i++;
			continue;
		}

		// ��ȡ�����������ǵ�һ���ַ���Ĭ�������Ϊ +��
		if (poly[i] == '+' || poly[i] == '-') {
			if (*length > 0) { // ������ʱ���洢�����
				ops[*length - 1] = poly[i];
			}
			coeffSign = (poly[i] == '-') ? -1.0 : 1.0; // ����ϵ������
			i++;
			continue;
		}
		else if (*length == 0) {
			ops[*length] = '+'; // ��һ�����Ĭ�������Ϊ '+'
		}

		// ����ϵ��
		tempIndex = 0;
		memset(temp, 0, sizeof(temp));
		while (isdigit(poly[i]) || poly[i] == '.' || (poly[i] == '-' && tempIndex == 0)) {
			if (poly[i] == '-' && tempIndex == 0 && (*length == 0 || ops[*length - 1] == '+')) {
				temp[tempIndex++] = poly[i++];
				continue;
			}
			temp[tempIndex++] = poly[i++];
		}
		temp[tempIndex] = '\0'; // �����ַ���
		coeffs[*length] = fabs(atof(temp)); // �洢ϵ������ֵ

		// ����ָ��
		if (poly[i] == 'x') {
			i++; // ���� 'x'
			if (poly[i] == '^') {
				i++; // ���� '^'
				tempIndex = 0;
				memset(temp, 0, sizeof(temp));
				while (isdigit(poly[i])) {
					temp[tempIndex++] = poly[i++];
				}
				temp[tempIndex] = '\0';
				exps[*length] = atoi(temp);
			}
			else {
				exps[*length] = 1; // Ĭ��ָ��Ϊ 1
			}
		}
		else {
			exps[*length] = 0; // �������ָ��Ϊ 0
		}

		(*length)++;
	}

	// ȷ�����һ����������洢
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
	// ��һ�����
	 // �����һ����
	value = Multi(v1, v2);
	delete(v1);
	delete(v2);
	// ���������
	for (int i = 1; i < length; i++) {
		m = input(coeffs[i], n); // ֱ�ӻ�ȡ�ڴ�ָ�룬�����ٳ�ʼ��
		temp = Pow(xn, exps[i]); // ֱ�ӻ�ȡ�ڴ�ָ�룬�����ٳ�ʼ��
		term = Multi(m, temp);   // ���㵱ǰ��
		// ����ǰһ�������������ǰ��ķ���
		if (ops[i - 1] == '-') {
			value = Minus(value, term); // ��ǰ�����Ϊ '-'������ֵ�м�ȥ��ǰ��
		}
		else if((ops[i - 1] == '+')) {
			value = Add(value, term); // ��ǰ�����Ϊ '+'����ӵ�ǰ���ֵ
			}
		}
	delete(term);
	delete(m);
	delete(temp);
	return value; // ���ض���ʽ��ֵ
}
#endif
int main (){
	int n = 200;
	
	while (getchar() != '\n');
#if 0
	printf("ʮתN\n");
	num* a1 = NewHead();
	num* N1 = NewHead();
	printf("������һ������");
	ReadWrite(a1);
	printf("��������ƣ�\n");
	ReadWrite(N1);
	Con1(a1, n, N1);
	printf("\n");
	delete(a1);
	delete(N1);
#endif	
	
#if 0
	printf("Nתʮ\n");
	num* a2 = NewHead();
	num* N2= NewHead();
	printf("�������һ������");
	ReadWrite(a2);
	printf("��������ƣ�\n");
	ReadWrite(N2);
	Con2(a2,N2);
	printf("\n");
	delete(a2);
	delete(N2);
#endif 
#if 0
	printf("��������:\n");
	cal(n);
	
#endif
	
#if 0
	printf("����ʽ1:\n");
	double x;
	printf("������x��\n");
	scanf("%lf", &x);
	num* nx = input(x, n);
	Print(PolyCal(nx, n));
	printf("\n");
	delete(nx);
#endif
#if 1
	    double y;
	    printf("����ʽ�ռ���:\n");
		char poly[MAX_LENGTH]; // �洢�û�����Ķ���ʽ
		double coeffs[MAX_LENGTH]; // ϵ������
		int exps[MAX_LENGTH];      // ָ������
		char ops[MAX_LENGTH];      // ���������
		int length;                // ���鳤��
		num* result = NewHead();
		// ��ȡ�û�����Ķ���ʽ
		printf("���������ʽ: ");
		fgets(poly, MAX_LENGTH, stdin);
		poly[strcspn(poly, "\n")] = 0; // ȥ�����з�

		// ��������ʽ
		parsePolynomial(poly, coeffs, exps, ops, &length);

		// ������
		printf("����ʽ: %s\n", poly);
		printf("ϵ��: ");
		for (int i = 0; i < length; i++) {
			printf("%f ", coeffs[i]);
		}
		printf("\n");

		printf("ָ��: ");
		for (int i = 0; i < length; i++) {
			printf("%d ", exps[i]);
		}
		printf("\n");

		printf("�����: ");
		for (int i = 0; i < length - 1; i++) {
			printf("%c ", ops[i]);
		}
		printf("\n");

		// ��ȡ�û������xֵ
		printf("������x��ֵ: ");
		scanf("%lf", &y);
		while (getchar() != '\n');
		// �������ʽ��ֵ
		
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
//           ���汣��       ����崻�     ����BUG
//








