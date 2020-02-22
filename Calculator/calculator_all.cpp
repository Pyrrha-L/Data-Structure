#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <malloc.h>
using namespace std;

#define VecSize 50 //向量的最大尺寸 

typedef struct
{
	int * component; //向量的分量 
	int length; //向量的维度 
}Vector;

bool GetVectors(Vector &V) 
{
	int i; //循环变量 
	printf("维度:\n");
	scanf("%d",&V.length);
	
	if(V.length<0)
	{
		printf("error!");
		return 1;
	}
	else{
		V.component = (int *) malloc ( V.length * sizeof(int) );
	}
	
	if(V.component==0){
		printf("内存分配失败.\n");
		exit(1); 
	}
	
	printf("分量:\n");
	for(i=0;i<V.length;i++)
	{
		cin >> V.component[i];
	}

	return 0;
}

bool AddVectors( Vector V1, Vector V2, Vector &ans )
{
	if(V1.length!=V2.length){
		printf("维度不同，不可计算。\n");
		return 1; 
	}
	ans.length=V1.length;
	ans.component=( int * )malloc( ans.length*sizeof(int) ); //分配存储空间 
	
	int i;
	
	for(i=0;i<ans.length;i++) //按次序相加 
	{
		ans.component[i]=V1.component[i]+V2.component[i];
	}
	
	return 0;
}

//向量相减 
bool SubVectors( Vector V1, Vector V2, Vector &ans )
{
	if(V1.length!=V2.length){
		printf("维度不同，不可计算。\n");
		return 1; 
	}
	ans.length=V1.length;
	ans.component=( int * )malloc( ans.length*sizeof(int) );
	
	int i;
	
	for(i=0;i<ans.length;i++) //按次序相减 
	{
		ans.component[i]=V1.component[i]-V2.component[i];
	}
	
	return 0;	
}

double CalCos( Vector V1, Vector V2 )
{
	double ans;
	
	//两向量维度不等则报错 
	if(V1.length!=V2.length)
	{
		printf("error!");
		return 1;
	}
	
	//初始化 
	double nume=0;
	double domi1=0,domi2=0,domi=0;
	int i;
	
	for(i=0;i<V1.length;i++)
	{
		nume+=V1.component[i]*V2.component[i]; //计算向量的点乘 
		domi1+=V1.component[i]*V1.component[i]; //计算向量的模的平方 
		domi2+=V2.component[i]*V2.component[i]; //计算向量的模的平方 
	}
	
	domi1=sqrt(domi1); //计算向量的模 
	domi2=sqrt(domi2); //计算向量的模 
	
	return nume/(domi1*domi2); //点乘除以模的乘积 
}

//清空向量 
bool ClearVectors( Vector &V )
{
	V.length=0;
	return 0;
}

//打印向量 
void PrintVectors( Vector V )
{
	cout<<"向量 : ";
	
	int i;
	for(i=0;i<V.length;i++) //i小于向量的维度 
	{
		cout<<V.component[i]<<' ';
	}
	cout<<endl;
	
	return;
}

//销毁向量 
bool DestroyVectors( Vector &V )
{
	if (V.component)  free(V.component); 
	return 0;
}

int vector_all()
{
	Vector V1,V2,ans;
	
	printf("\n----------向量计算器----------\n");
	printf("第一个向量:\n");
	GetVectors(V1);
	printf("第二个向量:\n");
	GetVectors(V2);
	printf("\n加法结果 :  \n");
	AddVectors( V1, V2, ans );
	PrintVectors( ans );
	printf("\n减法结果 :  \n");
	SubVectors( V1, V2, ans );
	PrintVectors( ans );
	printf("\n余弦值 : %.3f\n", CalCos( V1,V2 ));
	return 0;
}

//数据类型

typedef struct Xiang{
	int Xishu; //系数 
	int Cishu; //次数 
	struct Xiang * next; //下一项的指针 
}Xiang;

//初始化多项式 
Xiang * InitDXS()
{
	Xiang * head;
	head = (Xiang *)malloc(sizeof(Xiang)); //分配存储空间 

	if (head==NULL) return NULL;
	
	head->next=NULL;
	return head;
}

//释放多项式 
void FreeDXS(Xiang * head)
{

	Xiang * Tmp1, *Tmp2;

	Tmp1 = head->next;

	while(Tmp1 != NULL){
		Tmp2 = Tmp1->next;
		free(Tmp1); //依次释放内存 
		Tmp1 = Tmp2; //cursor后移 
	} 
	head->next = NULL;//设为空指针 
}

//销毁多项式 
void DesDXS(Xiang * head)
{
	FreeDXS(head);
	free(head);//释放头指针 
}

//输入多项式 
void InputDXS(Xiang * head)
{

	int Xishu,Cishu;
	Xiang * end, * xNew;

	end = head; //初始化 
	
	while(1){
		printf("请按照“系数,指数”格式、按递增顺序输入,最后输入0结束输入:\n");
		scanf("%d,%d", &Xishu, &Cishu);
		if (Xishu!= 0){
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if (xNew == NULL)
				exit(-1);
			
			xNew->Xishu = Xishu;
			xNew->Cishu = Cishu;
			end->next = xNew;
			end = xNew;//从尾部建立链表 
		}
		else{
			break;
		}
	}
	end->next = NULL;//设置尾指针 
}

void OutputDXS(Xiang *head)
{
	Xiang * Tmp;
	Tmp = head->next;
 
	if (Tmp == NULL){
		printf("\n");
		return;
	}
	
	printf("%dx^%d", Tmp->Xishu, Tmp->Cishu); //按照系数^次数的格式打印 
	Tmp = Tmp->next;//cursor后移 

	while(Tmp != NULL){
		printf("%+dx^%d", Tmp->Xishu, Tmp->Cishu);
		Tmp = Tmp->next;
	}
	printf("\n");
}

//表达式相加
Xiang *AddDXS (Xiang * X1, Xiang * X2)
{
	int tmp;
	Xiang * ans, *xNew, *end;	//结果用 
	Xiang * Tmp1, *Tmp2;		//运算用
	
	Tmp1 = X1->next;
	Tmp2 = X2->next;
	ans = InitDXS();
	end = ans;
	//将两个多项式扫描放入结果链表中
	while(Tmp1 != NULL && Tmp2 != NULL){
		if (Tmp1->Cishu == Tmp2->Cishu){
			tmp = Tmp1->Xishu + Tmp2->Xishu;	
			if ( tmp == 0){ //若系数为0，则跳过 
				Tmp1 = Tmp1->next;
				Tmp2 = Tmp2->next;
				continue;
			}
			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL ) exit(1);
			
			xNew->Xishu = tmp;
			xNew->Cishu = Tmp1->Cishu;
			end->next = xNew; //从尾部建立链表 
			xNew->next = NULL;
			end = xNew;
			
			Tmp1 = Tmp1->next;
			Tmp2 = Tmp2->next;
		}//如果表达式1指数小于2
		else if(Tmp1->Cishu < Tmp2->Cishu){
			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL) exit(1);

			*xNew = *Tmp1; 
			xNew->next = NULL;
			end->next = xNew;
			end = xNew;
			
			Tmp1 = Tmp1->next;
		}//如果表达式1指数大于2
		else{
			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL) exit(1);

			*xNew = *Tmp2; 
			xNew->next = NULL;
			end->next = xNew;
			end = xNew;
			Tmp2 = Tmp2->next;
		}
	}

	//将剩余的未扫描项放入结果链表中
	while(Tmp1 != NULL){
		xNew = (Xiang *)malloc(sizeof(Xiang));
		if ( xNew == NULL) exit(1);

		*xNew = *Tmp1; //尾部建立链表 
		xNew->next = NULL;
		end->next = xNew;
		end = xNew;
		Tmp1 = Tmp1->next;
	}

	while(Tmp2!= NULL){				
		xNew = (Xiang *)malloc(sizeof(Xiang));
		if ( xNew == NULL) exit(1);
		
		*xNew = *Tmp2; //尾部建立链表 
		xNew->next = NULL;
		end->next = xNew;
		end = xNew;
		Tmp2 = Tmp2->next;
	}
	return ans;
}

//多项式相减 
Xiang *SubDXS (Xiang * X1, Xiang * X2)
{
	int tmp;
	Xiang * ans, *xNew, *end;
	Xiang * Tmp1, *Tmp2;		
	
	Tmp1 = X1->next;
	Tmp2 = X2->next;
	ans = InitDXS();
	end = ans;
	//将两个多项式扫描放入结果链表中
	while(Tmp1 != NULL && Tmp2 != NULL){
		if (Tmp1->Cishu == Tmp2->Cishu){
			tmp = Tmp1->Xishu - Tmp2->Xishu;	
			if ( tmp == 0){
				Tmp1 = Tmp1->next;
				Tmp2 = Tmp2->next;
				continue;
			}
			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL ) exit(1);
			
			xNew->Xishu = tmp;
			xNew->Cishu = Tmp1->Cishu;
			end->next = xNew;
			xNew->next = NULL;
			end = xNew;
			
			Tmp1 = Tmp1->next;
			Tmp2 = Tmp2->next;
		}//如果表达式1指数小于2
		else if(Tmp1->Cishu < Tmp2->Cishu){
			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL) exit(1);

			xNew->Xishu = Tmp1->Xishu;
			xNew->Cishu = Tmp1->Cishu;
			xNew->next = NULL;
			end->next = xNew;
			end = xNew;
			
			Tmp1 = Tmp1->next;
		}//如果表达式1指数大于2
		else{
			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL) exit(1);

			xNew->Xishu = -Tmp2->Xishu;
			xNew->Cishu = Tmp2->Cishu;
			xNew->next = NULL;
			end->next = xNew;
			end = xNew;
			Tmp2 = Tmp2->next;
		}
	}

	//将剩余的未扫描项放入结果链表中
	while(Tmp1 != NULL){
		xNew = (Xiang *)malloc(sizeof(Xiang));
		if ( xNew == NULL) exit(1);

		xNew->Xishu = Tmp1->Xishu;
		xNew->Cishu = Tmp1->Cishu;
		xNew->next = NULL;
		end->next = xNew;
		end = xNew;
		Tmp1 = Tmp1->next;
	}

	while(Tmp2!= NULL){				
		xNew = (Xiang *)malloc(sizeof(Xiang));
		if ( xNew == NULL) exit(1);
		
		xNew->Xishu = -Tmp2->Xishu;
		xNew->Cishu = Tmp2->Cishu;
		xNew->next = NULL;
		end->next = xNew;
		end = xNew;
		Tmp2 = Tmp2->next;
	}
	
	return ans;
}

//乘法 
Xiang * MultDXS(Xiang * X1, Xiang * X2)
{
	Xiang *ans, *Tmp1, *Tmp2, *Mult1, *Mult2, *xNew, *end;

	Mult1 = InitDXS(); //乘法运算时的中间变量 
	Mult2 = InitDXS();
	Tmp1 = X1->next;

	while(Tmp1 != NULL){
		FreeDXS(Mult1);
		end = Mult1;
		Tmp2 = X2->next;

		while(Tmp2 != NULL){			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL) exit(1);
			
			xNew->Xishu = Tmp1->Xishu * Tmp2->Xishu; //系数相乘 
			xNew->Cishu = Tmp1->Cishu + Tmp2->Cishu; //指数相加 
			xNew->next = NULL;
			end->next = xNew;//从尾部放入链表 
			end = xNew;
			Tmp2 = Tmp2->next;
		}
		
		ans = AddDXS(Mult1, Mult2); //得到的多项式相加 
		FreeDXS(Mult2);
		
		Mult2->next = ans->next;
		Tmp1 = Tmp1->next;
	}
	
	DesDXS(Mult1);
	free(Mult2);
	
	return ans;
}

//求导
Xiang *DerDXS(Xiang * head)
{

	Xiang *Tmp = head->next;
	Xiang *xNew, *X1, *end;

	X1 = InitDXS();
	end = X1;
	
	while(Tmp != NULL){
		
		if (Tmp->Cishu == 0){
			Tmp = Tmp->next;
			continue;
		}

		xNew = (Xiang *)malloc(sizeof(Xiang));
		if ( xNew == NULL) exit(1);

		xNew->Xishu = Tmp->Xishu * Tmp->Cishu; //次数与系数相乘 
		xNew->Cishu = Tmp->Cishu - 1; //次数减1 

		end->next = xNew;
		xNew->next = NULL;
		end = xNew; //尾部建立链表 
		Tmp = Tmp->next;
	}
	return X1;
}

int DXS_all()
{

	Xiang *X1, *X2, *ans;
	X1 = InitDXS();
	InputDXS(X1);
	X2 = InitDXS();
	InputDXS(X2);
	printf("表达式1：\n");
	OutputDXS(X1);
	printf("第二个表达式：\n"); 
	printf("表达式2：\n");
	OutputDXS(X2);
	
	ans = AddDXS(X1, X2);
	printf("表达式1 + 表达式2 = ");
	OutputDXS(ans);
	DesDXS(ans);

	ans = SubDXS(X1, X2);
	printf("表达式1 - 表达式2 = ");
	OutputDXS(ans);
	DesDXS(ans);

	ans = DerDXS(X1);
	printf("表达式1求导 = ");
	OutputDXS(ans);
	DesDXS(ans);
	
	ans = MultDXS(X1, X2);
	printf("表达式1 * 表达式2 = ");
	OutputDXS(ans);
	
	DesDXS(ans);
	DesDXS(X1);
	DesDXS(X2); 
	
	return 0;
}

typedef struct
{
	int * Xishu;
	int ZuiGaoCi;
}Plnm;

//输入多项式 
bool GetPlnm(Plnm &P) 
{
	int i,j;
	printf("请输入该多项式的最高次项:\n");
	scanf("%d",&P.ZuiGaoCi);
	
	if(P.ZuiGaoCi<0)
	{
		printf("error!");
		return 1;
	}
	else{
		P.Xishu = (int *) malloc ( (P.ZuiGaoCi+1) * sizeof(int) );
		for(i=0;i<=P.ZuiGaoCi;i++) P.Xishu[i]=0;
	}
	
	j=1;
	while(j)
	{
		printf("请输入次数：\n");
		scanf("%d",&i);
		if(i<0||i>P.ZuiGaoCi)
		{
			printf("error!\n");
			continue;
		}
		else
		{
			printf("请输入系数：\n");
			cin>>P.Xishu[i];
		}
				
		printf("若已完成多项式设置，请输入0, 若要继续输入多项式，请输入1.\n");
		scanf("%d",&j); 
	}

	return 0;
}

//多项式相加 
bool AddPlnms( Plnm P1, Plnm P2, Plnm &ans )
{
	int i;
	ans.ZuiGaoCi=(P1.ZuiGaoCi>=P2.ZuiGaoCi)?P1.ZuiGaoCi:P2.ZuiGaoCi; //结果的最高次为两者中较大的最高次 
	ans.Xishu=( int * )malloc( (ans.ZuiGaoCi+1) * sizeof(int) );
	for(i=0;i<=ans.ZuiGaoCi;i++) ans.Xishu[i]=0; //初始化 
	
	int lmin=(P1.ZuiGaoCi<=P2.ZuiGaoCi)?P1.ZuiGaoCi:P2.ZuiGaoCi;	
	
	for(i=0;i<=lmin;i++)
	{
		ans.Xishu[i]=P1.Xishu[i]+P2.Xishu[i]; //相加 
	}
	
	//当有一方还有剩余的时候，把剩余的项加入结果中 
	if(P1.ZuiGaoCi<=P2.ZuiGaoCi)
	{
		for(i=lmin+1;i<=P2.ZuiGaoCi;i++)
		{
		ans.Xishu[i]=P2.Xishu[i];
		}
		return 0;
	}
	else
	{
		for(i=lmin+1;i<=P1.ZuiGaoCi;i++)
		{
		ans.Xishu[i]=P1.Xishu[i];
		}
		return 0;
	}
}

//多项式相减 
bool SubPlnms( Plnm P1, Plnm P2, Plnm &ans )
{
	int i;
	ans.ZuiGaoCi=(P1.ZuiGaoCi>=P2.ZuiGaoCi)?P1.ZuiGaoCi:P2.ZuiGaoCi; //取较大的最高次 
	ans.Xishu=( int * )malloc( (ans.ZuiGaoCi+1) * sizeof(int) );
	for(i=0;i<=ans.ZuiGaoCi;i++) ans.Xishu[i]=0; //初始化 
	
	int lmin=(P1.ZuiGaoCi<=P2.ZuiGaoCi)?P1.ZuiGaoCi:P2.ZuiGaoCi;
	
	for(i=0;i<=lmin;i++)
	{
		ans.Xishu[i]=P1.Xishu[i]-P2.Xishu[i];//相减 
	}
	
	//有一方有剩余的时候，添加到结果中去 
	if(P1.ZuiGaoCi<=P2.ZuiGaoCi)
	{
		for(i=lmin+1;i<=P2.ZuiGaoCi;i++)
		{
		ans.Xishu[i]=-P2.Xishu[i];
		}
		return 0;
	}
	else
	{
		for(i=lmin+1;i<=P1.ZuiGaoCi;i++)
		{
		ans.Xishu[i]=-P1.Xishu[i];
		}
		return 0;
	}
}

//求导 
bool DePlnms( Plnm &P )
{
	int i;
	for(i=0;i<=P.ZuiGaoCi;i++)
	{
		if(i==0&&P.Xishu[0]!=0) P.Xishu[0]=0; //处理第一项 
		
		if(P.Xishu[i]!=0)
		{
			P.Xishu[i-1]=P.Xishu[i]*i;
			P.Xishu[i]=0;
		}		
	}
	
	P.ZuiGaoCi--;//最高次减1 
	
	return 0;

}

//相乘 
bool MulPlnms( Plnm P1, Plnm P2, Plnm &ans )
{
	int i,j;
	ans.ZuiGaoCi=P1.ZuiGaoCi+P2.ZuiGaoCi; //取两个多项式最高次之和作为结果的最高次 
	ans.Xishu=( int * )malloc( (ans.ZuiGaoCi+1) * sizeof(int) );
	for(i=0;i<=ans.ZuiGaoCi;i++) ans.Xishu[i]=0; //初始化 

	for(i=0;i<=P1.ZuiGaoCi;i++)
	{
		if(P1.Xishu[i]!=0)
		{
			for(j=0;j<=P2.ZuiGaoCi;j++)
			{
				ans.Xishu[i+j]+=P1.Xishu[i]*P2.Xishu[j];//依次相乘后累加 
			}
		}
		else continue;
	}
	
	return 0;
}

//清空多项式 
bool ClearPlnm( Plnm &P )
{
	P.ZuiGaoCi=-1;
	return 0;
}

//打印多项式 
void PrintPlnm( Plnm P )
{
	int i;
	for(i=0;i<=P.ZuiGaoCi;i++)
	{
		if(P.Xishu[i]!=0) cout<<P.Xishu[i]<<"x^"<<i;
		if(i>0&&i!=P.ZuiGaoCi&&P.Xishu[i+1]>0) cout<<'+';
	}
	cout<<endl;
	return;
}

bool DestroyPlnm( Plnm &P )
{
	if (P.Xishu)  free(P.Xishu); 
	return 0;
}

int DXS_sql_all()
{
	Plnm P1,P2,ans;
	int control=1;
	
	printf("----------多项式简易计算器----------\n");
	printf("请输入第一个多项式:\n");
	GetPlnm(P1);
	printf("请输入第二个多项式:\n");
	GetPlnm(P2);

	printf("加法：\n"); 
	AddPlnms( P1, P2, ans );
	PrintPlnm( ans );
	
	printf("减法：\n"); 
	SubPlnms( P1, P2, ans );
	PrintPlnm( ans );

	printf("乘法：\n"); 
	MulPlnms( P1, P2, ans );
	PrintPlnm( ans );
	
	printf("第一个多项式求导：\n"); 
	DePlnms( P1 );
	PrintPlnm( P1 );
	
	printf("第二个多项式求导：\n"); 
	DePlnms( P2 );
	PrintPlnm( P2 );
	
	return 0;
}

const int MAXSIZE=30;
int x=0;//单变量值 
int ShuChu=0; //控制输出与否 
int YouXianJi[8]={0,0,2,1,0,1,0,2};//运算符优先级
int HaShHao=0;
char HanShuMing[10][5];
char HanShuTi[10][MAXSIZE];

//top存储栈顶是第几个元素，栈底即为data[0] 
typedef struct 
{
	int  data[MAXSIZE];
	int  top; 
} Stack;

Stack  sta,sta2;

int HouJiSuan(Stack &sta, int *Hou, int length);
void ZhongtoHou(Stack &sta, char *Zhong, int *Hou, int *length);

//初始栈 
void initStack(Stack *sta)
{
	int  i;

	for (i=0;i<MAXSIZE;i++)
	{
		sta->data[i] = 0; //所有值设为0 
	}
	sta->top = -1;

}

//清空栈
void EmptyStack(Stack &sta)
{
	int i;
	
	for(i=0;i<sta.top;i++)
		sta.data[i] = 0; //数值设为0 
	
	sta.top=-1; //栈顶设为-1 
	
	return;
}

//弹出栈
bool Pop(Stack &sta, int &e)
{
	if(sta.top==0) return 1;
	
	e=sta.data[sta.top];
	sta.top--;
	
	return 0;
}

//入栈 
bool Push(Stack &sta, int e)
{

	sta.data[sta.top++]=e;

	return 0;
}

//如果输入表达式的开头为“DEF” 
int Def(char *Zhong)
{
	ShuChu=0;
	int i=4;
	int HanShu=0;
			
	//第一次出现左括号，开始读取函数名 
	while(Zhong[i]!='(')
	{
		HanShuMing[HaShHao][HanShu]=Zhong[i];
		i++;
		HanShu++;
	}
		
	HanShu=0; 
			
	while(Zhong[i-1]!='=') i++;
			
	//出现等于号之后，开始读取函数体 
	while(Zhong[i]!=0)
	{
		HanShuTi[HaShHao][HanShu]=Zhong[i];
		i++;
		HanShu++;
	}
			
	//函数的序号加1 
	HaShHao++;
			
	printf("你输入的函数名为：%s，函数体为：%s\n",HanShuMing[HaShHao-1],HanShuTi[HaShHao-1]);
	return 0;
}

//输入表达式的开头为“RUN” 
int Run(char *Zhong,int func) //func: 0,内层函数;1:最外层函数 
{
	if(func) ShuChu=1;
	
	int  length,ans;
	int  Hou[MAXSIZE]; 
	char Zhong2[MAXSIZE];
	int i=4;
	int j=0;
	x=0;
	char tmp[5];
			
	for(j=0;j<5;j++)
	{
		tmp[j]=0;//初始化tmp 
	}
	j=0;
						
	while(Zhong[i]!='(')
	{
		tmp[j]=Zhong[i];//读取函数名 
		i++;
		j++;
	}
			
	for(j=0;j<HaShHao;j++)
	{
		if(strcmp(tmp,HanShuMing[j])==0) {
			printf("找到目标函数。\n");
			break;
		}
		//搜索函数名 
	}
			
	if(j>=HaShHao&&func)
	{
		printf("没有找到该函数\n");
		ShuChu=0; 
		return -1;
	}
			
	i++;
			
	//读取自变量的值 
	while(Zhong[i]!=')')
	{				
		x=x*10+(Zhong[i]-'0');
		i++;
	}
	
	//把函数体拷贝到中缀表达式中，然后开始解析中缀表达式 
	strcpy(Zhong2,HanShuTi[j]);
	
	initStack(&sta);
	ZhongtoHou(sta, Zhong2, Hou, &length); //解析中缀表达式 
	initStack(&sta);
	ans=HouJiSuan(sta, Hou, length);
	
	if(ShuChu) printf("计算结果为 : %d\n",ans);
	return ans;
}

//判断分支 
void DEForRUN(char *Zhong)
{
	int i;
	for (i=0;i<strlen(Zhong);)
	{
		//DEF为输入函数的标志，把函数名和函数体分别存在两个二维数组里 
		if(Zhong[i]=='D'&&Zhong[i+1]=='E'&&Zhong[i+2]=='F') 
		{
			Def(Zhong);
			return;
		}
		
		//RUN 为运行函数的标志，通过用HaShHao来搜索函数名，锁定相应的函数 
		else if(Zhong[i]=='R'&&Zhong[i+1]=='U'&&Zhong[i+2]=='N') 
		{	
			Run(Zhong,1);
			return;
		}
		//如果既不是DEF也不是RUN，就是普通的四则运算表达式计算 
		else{
			int  length,ans;
			int  Hou[MAXSIZE]; 

			initStack(&sta);
			ZhongtoHou(sta, Zhong, Hou, &length); //解析中缀表达式 
			initStack(&sta);
			ans=HouJiSuan(sta, Hou, length); //计算后缀表达式 
			
			printf("计算结果为 : %d\n",ans);
			return;
		}
	}

}

//中缀表达式到后缀表达式 
void ZhongtoHou(Stack &sta, char *Zhong, int *Hou, int *length)
{
	int  i;				
	int  b = 0;			
	int  j = 0;	
	int k = 0;		
	int  priority = 0;
	char BianL[10]; //存储第一个变量 
	char BianL2[10]; //存储第二个变量 
	int DiYiCi=0; //判断是否第一次出现变量 
	int bi=0;
	int HanShu=0; //在循环中做下标使用，作用相当于i，j 
	int  length2;
	int  Hou2[MAXSIZE]; 
	
	for (i=0;i<strlen(Zhong);)
	{
		//读出自变量时，自动代入变量的值。 
		if (!DiYiCi&&((Zhong[i] >= 'A' && Zhong[i] <= 'Z')||
			(Zhong[i] >= 'a' && Zhong[i] <= 'z')||
			(Zhong[i] == '_' )))
		{
			while ((Zhong[i] >= 'A' && Zhong[i] <= 'Z')||
			(Zhong[i] >= 'a' && Zhong[i] <= 'z')||
			(Zhong[i] >= '0' && Zhong[i] <= '9')|| 
			(Zhong[i] == '_' ))
			{
				BianL[bi] = Zhong[i];
				i++;
				bi++;
			}
			
			BianL[bi]=0;
			bi=0;
			
			for(k=0;k<HaShHao;k++)
			{
				if(strcmp(BianL,HanShuMing[k])==0) //搜索函数名
				{
					initStack(&sta2);
					ZhongtoHou(sta2, HanShuTi[k], Hou2, &length2); 
					//如果搜索到内层函数 ，则先计算内层函数 
					initStack(&sta2);
					Hou[j]=HouJiSuan(sta2, Hou2, length2);
//					printf("found another:%d", Hou[j]);
					j++;
					i+=3;
					break; 
				}
			}
			//把自变量的值代入后缀表达式 
			
			if(k>=HaShHao) //如果没有搜索到内层函数，则按照单变量处理 
			{
				Hou[j]=x;
				j++;
				DiYiCi=1;
			}
			
			continue;
		}
		
		if (DiYiCi&&((Zhong[i] >= 'A' && Zhong[i] <= 'Z')||
			(Zhong[i] >= 'a' && Zhong[i] <= 'z')||
			(Zhong[i] == '_' )))
		{
			while ((Zhong[i] >= 'A' && Zhong[i] <= 'Z')||
			(Zhong[i] >= 'a' && Zhong[i] <= 'z')||
			(Zhong[i] >= '0' && Zhong[i] <= '9')|| 
			(Zhong[i] == '_' ))
			{
				BianL2[bi] = Zhong[i];
				i++;
				bi++;
			}
			BianL2[bi]=0;
			bi=0;
			
			if(strcmp(BianL,BianL2)!=0){
				printf("变量名不一致！");
				exit(1); 
			}
			
			Hou[j]=x;
			j++;
			
			continue;
		}

		
		//读到数字，解析入后缀表达式 
		if (Zhong[i] >= '0' && Zhong[i] <= '9')
		{
			b = 0;		
			while (Zhong[i] >= '0' && Zhong[i] <= '9')
			{
				b = b * 10 + (Zhong[i] - '0');//把字符转化为数字 
				i++;
			}
			Hou[j] = b;
			j++;
			continue;
		}
		
		if (Zhong[i] == 41)//读到右括号，没遇到左括号时一直弹出 
		{
			while (sta.data[sta.top] != 40)
			{
				Hou[j] = sta.data[sta.top];
				sta.data[sta.top] = 0;

				sta.top--;

				j++;
			}
			sta.data[sta.top] = 0;
			sta.top--;

			priority = YouXianJi[sta.data[sta.top] % 10];//更新优先级 

			i++;
			continue;
		}
		//左括号
		if (Zhong[i] == 40)
		{
			sta.top++;
			sta.data[sta.top] = Zhong[i];
			
			priority = YouXianJi[sta.data[sta.top] % 10];
			i++;
			continue;

		}
		
		if (Zhong[i] >= 42 && Zhong[i] <= 47)
		{
			if (priority >= YouXianJi[Zhong[i] % 10])
			{
				//如果入栈元素优先级低于栈顶元素，则弹出 
				while (priority >= YouXianJi[Zhong[i] % 10] && sta.data[sta.top] != 40)
				{
					Hou[j] = sta.data[sta.top];
					sta.data[sta.top] = 0;
					sta.top--;
					priority = YouXianJi[sta.data[sta.top] % 10];

					j++;

				}

				sta.top++;

				sta.data[sta.top] = Zhong[i];
				priority = YouXianJi[sta.data[sta.top] % 10];//更新优先级 
				i++;
			}
			else 
			{
				//负数的情况，“（-” 
				if (Zhong[i] == 45 && sta.data[sta.top] == 40)
				{
					b = 0;
					while (Zhong[i+1] >= '0' && Zhong[i+1] <= '9')
					{
						b = b * 10 + (Zhong[i+1] - '0');
						i++;
					}
					Hou[j] = b * -1;
					sta.data[sta.top] = 0;//出栈 
					sta.top--;
					j++;
					i += 2;
					priority = YouXianJi[sta.data[sta.top] % 10];//更新优先级 

					continue;

				}

				sta.top++;
				sta.data[sta.top] = Zhong[i];
				priority = YouXianJi[sta.data[sta.top] % 10];
				i++;
			} 
		}
	}

	//弹出剩余元素 
	while (sta.top != -1)
	{
		Hou[j] = sta.data[sta.top];
		sta.top--;
		j++;
	}

/*	for( i=0 ; i<j ; i++)
	{
		printf("~%d ",Hou[i]);
	}
	printf("\n");
*/	
	*length=j;//更新长度 
}

//计算后缀表达式 
int HouJiSuan(Stack &sta, int *Hou, int length)
{
	int i,j,ans;
	ans=0;

	for (i=0;i<length;i++)
	{
		//解析后缀表达式里的运算符，如果是，则对ans做相应的操作并压栈 
		switch (Hou[i])
		{
		case 42:  
			ans=sta.data[sta.top-1]*sta.data[sta.top];
			sta.top-=1;
			sta.data[sta.top]=ans;
			break;
		case 43:
			ans=sta.data[sta.top-1]+sta.data[sta.top];
			sta.top-=1;
			sta.data[sta.top]=ans;
			break;
		case 45:
			ans=sta.data[sta.top-1]-sta.data[sta.top];
			sta.top-=1;
			sta.data[sta.top]=ans;
			break;
		case 47:
			ans=sta.data[sta.top-1]/sta.data[sta.top];
			sta.top-=1;
			sta.data[sta.top]=ans;
			break;
		default://如果不是运算符，把后缀表达式的值压栈 
			sta.top++;
			sta.data[sta.top]=Hou[i];
			break;
		}
	}
	return ans;
}

int SiZeYunSuan_func_all()
{	  
	char Zhong[MAXSIZE],Zhong2[MAXSIZE];
	int k2=1;//k控制结束与否 

	printf("使用细则：\n");	
	printf("本计算器为整型计算器，支持函数定义功能。\n");
	printf("你可以使用‘DEF f(x)=5*x+1’‘DEF g(x)=f(x)+4+x/2’‘RUN g(4)’这样的语句来运行函数\n");
	printf("输入：\n");	
	getchar();
	
	while(k2)
	{
		gets(Zhong);//获取中缀表达式 
		strcpy(Zhong2,Zhong);
		
		DEForRUN(Zhong);
		printf("继续，输入1；离开，输入0：\n");
		scanf("%d",&k2);
		getchar();
	}
	
	return 0;

}

//矩阵计算器
 
void matrix_output(float dis[][25],int m,int n);
int determinant(float a[25][25],int n);
void cofactor(float num[25][25],int f);
void transpose(float num[25][25],float fac[25][25],int r);

void transpose_input(float a[][25],int m,int n) //转置计算 
{
	int i,j;
	float a2[25][25]; 
	for (i=0;i<n;i++)
    {
     for (j=0;j<m;j++)
       {
         a2[i][j]=a[j][i]; //行列对调 
        }
    }
    printf("矩阵的转置为:\n");
    matrix_output(a2,n,m);
}

 //行列式计算 
void determinant_input(float a[25][25],int m, int n)
{
  if(m!=n){
  	printf("非方阵，无行列式。\n");
	return; 
  }
  int d;
  int i,j;
  d=determinant(a,n); //计算行列式 
  printf("行列式为 %d \n",d);
}

//矩阵相加 
void matrixadd(float a[][25],float b[][25],int ma,int na,int mb,int nb)
{
	if(ma!=mb||na!=nb){
		printf("维数不同，无法相加\n");
		return; 
	}
	int i,j;
	float ans[25][25];
	for(i=0;i<ma;i++)
	{
		for(j=0;j<na;j++)
		ans[i][j]=a[i][j] +b[i][j];
	}
	printf("矩阵相加 \n");

	matrix_output(ans,ma,na);
}

//矩阵相减 
void matrixsub(float a[][25],float b[][25],int ma,int na,int mb,int nb) //矩阵相减 
{
	if(ma!=mb||na!=nb){
		printf("维数不同，无法相减\n");
		return; 
	}
	int i,j;
	float ans[25][25]; //最终结果 
	for(i=0;i<ma;i++)
	{
		for(j=0;j<na;j++)
		ans[i][j]=a[i][j] - b[i][j];
	}
	printf("矩阵相减 \n");

	matrix_output(ans,ma,na);
}

//打印矩阵 
void matrix_output(float dis[][25],int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("%.3f ",dis[i][j]);
		}
		printf("\n");
	}
}


//矩阵求逆：矩阵的逆矩阵等于原矩阵的伴随矩阵除以行列式 
void inverse(float a[25][25],int m,int n) //矩阵求逆 
{
  int k,d;
  k=n;
  if(m!=n){
  	printf("非方阵，无法求行列式，无法求逆。");
	return; 
  }
  d=determinant(a,k);
  printf("行列式为 %d",d);
  if (d==0)
   printf("\n逆矩阵不存在\n");
  else
   cofactor(a,k); //利用余子式计算逆矩阵 
   printf("计算完成!\n");
   
   return;
}

//计算行列式：按行展开，递归求解 
int determinant(float a[25][25],int k)
{
  int s=1,det=0;
  float b[25][25];
  int i,j,m,n,c;
  if (k==1)
  {
     return (a[0][0]);//一阶方阵的行列式为其唯一元素 
  }
  else
  {
     det=0;
     for (c=0;c<k;c++) //按照第一行展开 
     {
        m=0;
        n=0;
        for (i=0;i<k;i++)
        {
            for (j=0;j<k;j++)
            {
				//构造余子式 
                b[i][j]=0;//初步赋值 
                if (i != 0 && j != c) //划去第一行锁定元素的行和列 
                {
                   b[m][n]=a[i][j];
                   if (n<(k-2))
                     n++;
                   else
                   {
                     n=0;
                     m++;//换行 
                    }
                }
            }
        }
        
		det=det + s * (a[0][c] * determinant(b,k-1)); //递归函数 
        s=-1 * s;
        }
    } 

    return (det);
}

//求解伴随矩阵 
void cofactor(float num[25][25],int f) 
{
 float b[25][25],fac[25][25]; //b：存放伴随矩阵每个值对应的余子式，fac：存放伴随矩阵 
 int p,q,m,n,i,j;
 for (q=0;q<f;q++)
 {
   for (p=0;p<f;p++)
    {
     m=0;
     n=0;
     for (i=0;i<f;i++)
     {
       for (j=0;j<f;j++)
        {
          if (i != q && j != p)
          {
            b[m][n]=num[i][j]; //构造余子式 
            if (n<(f-2))
             n++;
            else
             {
               n=0;
               m++;
               }
            }
        }
      }
      fac[q][p]=pow(-1,q + p) * determinant(b,f-1); //计算伴随矩阵对应分量的值 
    }
  }
  transpose(num,fac,f);
}

//计算最终的逆矩阵
void transpose(float num[25][25],float fac[25][25],int r)
{
  int i,j;
  float b[25][25],inverse[25][25],d;
  //inverse存放逆矩阵,b存放伴随矩阵的转置 
  
  //处理伴随矩阵
  for (i=0;i<r;i++)
    {
     for (j=0;j<r;j++)
       {
         b[i][j]=fac[j][i];
        }
    }
  d=determinant(num,r);
  
  for (i=0;i<r;i++)
    {
     for (j=0;j<r;j++)
       {
        inverse[i][j]=b[i][j] / d; //逆矩阵对应的值等于伴随矩阵转置的值除以行列式 
        }
    }
   printf("\n逆矩阵为 : \n");

   matrix_output(inverse,r,r);

}

void matrixmultiply(float a[25][25],float b[25][25],int ma,int na,int mb,int nb)
{
	if(na!=mb){
		printf("维数不符合乘法要求。\n");
		return;
	}
	float multi[25][25];
	
	int i,j,k;
	for (i=0;i<ma;i++)
	{
		for(j=0;j<nb;j++)
		{
			multi[i][j]=0;
			for(k=0;k<nb;k++)
			multi[i][j]=multi[i][j]+(a[i][k]*b[k][j]);
		}
	}
	
	printf("矩阵相乘：\n"); 
	matrix_output(multi,ma,nb);
}

int matrix_all()
{
	float _a[25][25],_b[25][25];
	int _ma,_na,_mb,_nb,i,j;
	printf("--------------矩阵计算器--------------\n");
	printf("请输入两个矩阵，该计算器将会计算两矩阵的加法、减法，\n以及第一个矩阵的行列式、转置和逆矩阵\n");
	printf("输入第一个矩阵的维度（行*列）\n");
	scanf("%d %d",&_ma,&_na);
	printf("输入第一个矩阵\n"); 
	for(i=0;i<_ma;i++)
	{
		for(j=0;j<_na;j++)
		scanf("%f",&_a[i][j]);
	}
	
	printf("输入第二个矩阵的维度（行*列）\n");
	scanf("%d %d",&_mb,&_nb);	
	printf("输入第二个矩阵\n"); 
	for(i=0;i<_mb;i++)
	{
		for(j=0;j<_nb;j++)
		scanf("%f",&_b[i][j]);
	}
	
	transpose_input(_a,_ma,_na);
	determinant_input(_a,_ma,_na);
	matrixadd(_a,_b,_ma,_na,_mb,_nb);
	matrixsub(_a,_b,_ma,_na,_mb,_nb);
	matrixmultiply(_a,_b,_ma,_na,_mb,_nb);
	inverse(_a,_ma,_na);

	return 0;
}

#define PI 3.1415926

//top存储栈顶是第几个元素，栈底即为data[0] 
typedef struct 
{
	float  dataf[MAXSIZE];
	int  top; 
} Stackf;

//初始栈 
void initStackf(Stackf *sta)
{
	int  i;

	for (i=0;i<MAXSIZE;i++)
	{
		sta->dataf[i] = 0;
	}
	sta->top = -1;

}

//中缀表达式转化为后缀表达式 
void ZhongtoHouf(Stackf &sta, char *Zhong, float *Hou, int *length)
{
	int  i;				
	int  b = 0;			
	int  j = 0;
	int  bi = 0; //bi记录buff中的位数		
	int  priority = 0; //优先级 
	char buff[10]; //读取数字段时用作暂存数组 

	for (i=0;i<strlen(Zhong);)
	{
		
		if (Zhong[i] >= '0' && Zhong[i] <= '9')
		{
			b = 0;
					
			while ((Zhong[i] >= '0' && Zhong[i] <= '9')||Zhong[i]=='.')
			{
				buff[bi] = Zhong[i];
	//			b = b * 10 + (Zhong[i] - '0'); //若不是浮点数运算，用这行代码解析即可 
				i++;
				bi++;
			}
			buff[bi]=0;
							
			sscanf(buff, "%f", &Hou[j]);//把字符数组转化为浮点数 
			
			if(Zhong[i]=='e')
			{
				i++;
				Hou[j]*=pow(10,Zhong[i]-'0');
				i++;
			}
			
			memset(buff,0,bi);
			bi=0; //bi置零 
			j++;
			continue;
		}
		
		//考虑余弦值 
		if (Zhong[i] == 'c'&&Zhong[i+1] == 'o'&&Zhong[i+2] == 's')
		{
			i+=3;
			b = 0;
					
			while ((Zhong[i] >= '0' && Zhong[i] <= '9')||Zhong[i]=='.')
			{
				b = b * 10 + (Zhong[i] - '0'); //若不是浮点数运算，用这行代码解析即可 
				i++;
			}
			
			Hou[j]=cos((b/180.0)*PI);
			j++;
			continue;
		}
		
		//考虑正弦值 
		if (Zhong[i] == 's'&&Zhong[i+1] == 'i'&&Zhong[i+2] == 'n')
		{
			i+=3;
			b = 0;
					
			while ((Zhong[i] >= '0' && Zhong[i] <= '9')||Zhong[i]=='.')
			{
				b = b * 10 + (Zhong[i] - '0'); //若不是浮点数运算，用这行代码解析即可 
				i++;
			}
			
			Hou[j]=sin((b/180.0)*PI);
			j++;
			continue;
		}
		
		if (Zhong[i] == 41) //遇到右括号 
		{
			while (sta.dataf[sta.top] != 40) //没遇到左括号之前一直弹出 
			{
				Hou[j] = sta.dataf[sta.top]; 
				sta.dataf[sta.top] = 0;

				sta.top--;

				j++;
			}
			sta.dataf[sta.top] = 0;
			sta.top--;

			priority = YouXianJi[((int)sta.dataf[sta.top] )% 10];//更新优先级 

			i++;
			continue;
		}
		
		//左括号
		if (Zhong[i] == 40)
		{
			sta.top++;
			sta.dataf[sta.top] = Zhong[i]; //左括号压栈 
			
			priority = YouXianJi[((int)sta.dataf[sta.top] ) % 10]; //更新优先级 
			i++;
			continue;

		}
		
		if (Zhong[i] >= 42 && Zhong[i] <= 47)
		{
			if (priority >= YouXianJi[Zhong[i] % 10])
			{
				while (priority >= YouXianJi[Zhong[i] % 10] && sta.dataf[sta.top] != 40)
				//优先级小的运算符入栈 
				{
					Hou[j] = sta.dataf[sta.top];
					sta.dataf[sta.top] = 0;
					sta.top--;//弹出到后缀表达式 
					priority = YouXianJi[((int)sta.dataf[sta.top] )% 10];

					j++;

				}

				//一般情况下的压栈 
				sta.top++; 
				sta.dataf[sta.top] = Zhong[i];
				priority = YouXianJi[((int)sta.dataf[sta.top] )% 10];
				i++;
			}
			else 
			{
				//负数的情况：“（- ” 
				if (Zhong[i] == 45 && sta.dataf[sta.top] == 40)
				{
					b = 0;
					while ((Zhong[i+1] >= '0' && Zhong[i+1] <= '9')||Zhong[i+1]=='.')
					{
						buff[bi] = Zhong[i+1];
						i++;
						bi++;
					}
					bi=0;
					sscanf(buff, "%f", &Hou[j]);//解析buff 
					Hou[j] = Hou[j] * -1;
					j++;

					sta.dataf[sta.top] = 0;
					sta.top--;
					i += 2;
					priority = YouXianJi[((int)sta.dataf[sta.top] )% 10];

					continue;

				}

				sta.top++;
				sta.dataf[sta.top] = Zhong[i];
				priority = YouXianJi[((int)sta.dataf[sta.top])%10];
				i++;
			} 
		}
	}

	//弹出剩余的元素 
	while (sta.top != -1)
	{
		Hou[j] = sta.dataf[sta.top];
		sta.top--;
		j++;
	}

	*length=j;
}

//后缀表达式的计算 
float HouJiSuanf(Stackf &sta, float *Hou, int length)
{
	int i,j;
	float ans;
	ans=0;

	for (i=0;i<length;i++)
	{
		float tmp=Hou[i]; //计算后缀表达式时保存（int）Hou[i]之前的浮点值。
		switch ((int)Hou[i])
		{
		case 42:
			ans=sta.dataf[sta.top-1]*sta.dataf[sta.top];//对ans进行相应处理后压栈 
			sta.top-=1;
			sta.dataf[sta.top]=ans;
			break;
		case 43:
			ans=sta.dataf[sta.top-1]+sta.dataf[sta.top];
			sta.top-=1;
			sta.dataf[sta.top]=ans;
			break;
		case 45:
			ans=sta.dataf[sta.top-1]-sta.dataf[sta.top];
			sta.top-=1;
			sta.dataf[sta.top]=ans;
			break;
		case 47:
			ans=sta.dataf[sta.top-1]/sta.dataf[sta.top];
			sta.top-=1;
			sta.dataf[sta.top]=ans;
			break;
		default:
			sta.top++;
			sta.dataf[sta.top]=tmp; //采用原始值 
			break;
		}
	}
	return ans;
}

int SiZeYunSuan_float_all()
{
	Stackf  sta;
	int  length;
	float ans;         //接受后缀表达式转换的结果
	float  Hou[MAXSIZE];  //存储后缀表达式
	char Zhong[MAXSIZE];  //存储中缀表达式

	printf("--------浮点数四则运算计算器--------\n");
	printf("你可以使用形如1e1、cos45、（-1）此类表达式\n");	
	printf("输入四则运算表达式（负数添加括号）：");	
	scanf("%s",Zhong);
	initStackf(&sta); 
	ZhongtoHouf(sta,Zhong,Hou,&length);//后缀到中缀转换 
	
	initStackf(&sta); 
	ans=HouJiSuanf(sta,Hou,length);
	printf("%.2f\n",ans);
	return 0;
}

void menu()
{
	printf("\n------------CALCULATOR BY 2017202121-----------\n");
	printf("1 向量计算（加、减、求余弦）\n");
	printf("2 链表实现多项式计算（加、减、乘、求导）\n");
	printf("3 整数型四则运算计算器\n");
	printf("4 矩阵计算器(加、减、乘、求逆、转置、求行列式)\n");
	printf("5 浮点数四则运算计算器\n");
	printf("6 线性表实现多项式计算（加、减、乘、求导）\n");
	printf("0 退出\n");
}

int main()
{
	int k=8;//控制进程 
	
	while(k)
	{
		menu();
		scanf("%d",&k);
		
		if(k==1) vector_all(); 
		else if(k==2) DXS_all();
		else if(k==3) SiZeYunSuan_func_all();
		else if(k==4) matrix_all();
		else if(k==5) SiZeYunSuan_float_all(); 
		else if(k==6) DXS_sql_all(); 
		else if(k==0) break;
		else{
			printf("输入值无效。\n");
			k=1;
		}
	}
}
