#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <malloc.h>
using namespace std;

#define VecSize 50 //���������ߴ� 

typedef struct
{
	int * component; //�����ķ��� 
	int length; //������ά�� 
}Vector;

bool GetVectors(Vector &V) 
{
	int i; //ѭ������ 
	printf("ά��:\n");
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
		printf("�ڴ����ʧ��.\n");
		exit(1); 
	}
	
	printf("����:\n");
	for(i=0;i<V.length;i++)
	{
		cin >> V.component[i];
	}

	return 0;
}

bool AddVectors( Vector V1, Vector V2, Vector &ans )
{
	if(V1.length!=V2.length){
		printf("ά�Ȳ�ͬ�����ɼ��㡣\n");
		return 1; 
	}
	ans.length=V1.length;
	ans.component=( int * )malloc( ans.length*sizeof(int) ); //����洢�ռ� 
	
	int i;
	
	for(i=0;i<ans.length;i++) //��������� 
	{
		ans.component[i]=V1.component[i]+V2.component[i];
	}
	
	return 0;
}

//������� 
bool SubVectors( Vector V1, Vector V2, Vector &ans )
{
	if(V1.length!=V2.length){
		printf("ά�Ȳ�ͬ�����ɼ��㡣\n");
		return 1; 
	}
	ans.length=V1.length;
	ans.component=( int * )malloc( ans.length*sizeof(int) );
	
	int i;
	
	for(i=0;i<ans.length;i++) //��������� 
	{
		ans.component[i]=V1.component[i]-V2.component[i];
	}
	
	return 0;	
}

double CalCos( Vector V1, Vector V2 )
{
	double ans;
	
	//������ά�Ȳ����򱨴� 
	if(V1.length!=V2.length)
	{
		printf("error!");
		return 1;
	}
	
	//��ʼ�� 
	double nume=0;
	double domi1=0,domi2=0,domi=0;
	int i;
	
	for(i=0;i<V1.length;i++)
	{
		nume+=V1.component[i]*V2.component[i]; //���������ĵ�� 
		domi1+=V1.component[i]*V1.component[i]; //����������ģ��ƽ�� 
		domi2+=V2.component[i]*V2.component[i]; //����������ģ��ƽ�� 
	}
	
	domi1=sqrt(domi1); //����������ģ 
	domi2=sqrt(domi2); //����������ģ 
	
	return nume/(domi1*domi2); //��˳���ģ�ĳ˻� 
}

//������� 
bool ClearVectors( Vector &V )
{
	V.length=0;
	return 0;
}

//��ӡ���� 
void PrintVectors( Vector V )
{
	cout<<"���� : ";
	
	int i;
	for(i=0;i<V.length;i++) //iС��������ά�� 
	{
		cout<<V.component[i]<<' ';
	}
	cout<<endl;
	
	return;
}

//�������� 
bool DestroyVectors( Vector &V )
{
	if (V.component)  free(V.component); 
	return 0;
}

int vector_all()
{
	Vector V1,V2,ans;
	
	printf("\n----------����������----------\n");
	printf("��һ������:\n");
	GetVectors(V1);
	printf("�ڶ�������:\n");
	GetVectors(V2);
	printf("\n�ӷ���� :  \n");
	AddVectors( V1, V2, ans );
	PrintVectors( ans );
	printf("\n������� :  \n");
	SubVectors( V1, V2, ans );
	PrintVectors( ans );
	printf("\n����ֵ : %.3f\n", CalCos( V1,V2 ));
	return 0;
}

//��������

typedef struct Xiang{
	int Xishu; //ϵ�� 
	int Cishu; //���� 
	struct Xiang * next; //��һ���ָ�� 
}Xiang;

//��ʼ������ʽ 
Xiang * InitDXS()
{
	Xiang * head;
	head = (Xiang *)malloc(sizeof(Xiang)); //����洢�ռ� 

	if (head==NULL) return NULL;
	
	head->next=NULL;
	return head;
}

//�ͷŶ���ʽ 
void FreeDXS(Xiang * head)
{

	Xiang * Tmp1, *Tmp2;

	Tmp1 = head->next;

	while(Tmp1 != NULL){
		Tmp2 = Tmp1->next;
		free(Tmp1); //�����ͷ��ڴ� 
		Tmp1 = Tmp2; //cursor���� 
	} 
	head->next = NULL;//��Ϊ��ָ�� 
}

//���ٶ���ʽ 
void DesDXS(Xiang * head)
{
	FreeDXS(head);
	free(head);//�ͷ�ͷָ�� 
}

//�������ʽ 
void InputDXS(Xiang * head)
{

	int Xishu,Cishu;
	Xiang * end, * xNew;

	end = head; //��ʼ�� 
	
	while(1){
		printf("�밴�ա�ϵ��,ָ������ʽ��������˳������,�������0��������:\n");
		scanf("%d,%d", &Xishu, &Cishu);
		if (Xishu!= 0){
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if (xNew == NULL)
				exit(-1);
			
			xNew->Xishu = Xishu;
			xNew->Cishu = Cishu;
			end->next = xNew;
			end = xNew;//��β���������� 
		}
		else{
			break;
		}
	}
	end->next = NULL;//����βָ�� 
}

void OutputDXS(Xiang *head)
{
	Xiang * Tmp;
	Tmp = head->next;
 
	if (Tmp == NULL){
		printf("\n");
		return;
	}
	
	printf("%dx^%d", Tmp->Xishu, Tmp->Cishu); //����ϵ��^�����ĸ�ʽ��ӡ 
	Tmp = Tmp->next;//cursor���� 

	while(Tmp != NULL){
		printf("%+dx^%d", Tmp->Xishu, Tmp->Cishu);
		Tmp = Tmp->next;
	}
	printf("\n");
}

//���ʽ���
Xiang *AddDXS (Xiang * X1, Xiang * X2)
{
	int tmp;
	Xiang * ans, *xNew, *end;	//����� 
	Xiang * Tmp1, *Tmp2;		//������
	
	Tmp1 = X1->next;
	Tmp2 = X2->next;
	ans = InitDXS();
	end = ans;
	//����������ʽɨ�������������
	while(Tmp1 != NULL && Tmp2 != NULL){
		if (Tmp1->Cishu == Tmp2->Cishu){
			tmp = Tmp1->Xishu + Tmp2->Xishu;	
			if ( tmp == 0){ //��ϵ��Ϊ0�������� 
				Tmp1 = Tmp1->next;
				Tmp2 = Tmp2->next;
				continue;
			}
			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL ) exit(1);
			
			xNew->Xishu = tmp;
			xNew->Cishu = Tmp1->Cishu;
			end->next = xNew; //��β���������� 
			xNew->next = NULL;
			end = xNew;
			
			Tmp1 = Tmp1->next;
			Tmp2 = Tmp2->next;
		}//������ʽ1ָ��С��2
		else if(Tmp1->Cishu < Tmp2->Cishu){
			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL) exit(1);

			*xNew = *Tmp1; 
			xNew->next = NULL;
			end->next = xNew;
			end = xNew;
			
			Tmp1 = Tmp1->next;
		}//������ʽ1ָ������2
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

	//��ʣ���δɨ���������������
	while(Tmp1 != NULL){
		xNew = (Xiang *)malloc(sizeof(Xiang));
		if ( xNew == NULL) exit(1);

		*xNew = *Tmp1; //β���������� 
		xNew->next = NULL;
		end->next = xNew;
		end = xNew;
		Tmp1 = Tmp1->next;
	}

	while(Tmp2!= NULL){				
		xNew = (Xiang *)malloc(sizeof(Xiang));
		if ( xNew == NULL) exit(1);
		
		*xNew = *Tmp2; //β���������� 
		xNew->next = NULL;
		end->next = xNew;
		end = xNew;
		Tmp2 = Tmp2->next;
	}
	return ans;
}

//����ʽ��� 
Xiang *SubDXS (Xiang * X1, Xiang * X2)
{
	int tmp;
	Xiang * ans, *xNew, *end;
	Xiang * Tmp1, *Tmp2;		
	
	Tmp1 = X1->next;
	Tmp2 = X2->next;
	ans = InitDXS();
	end = ans;
	//����������ʽɨ�������������
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
		}//������ʽ1ָ��С��2
		else if(Tmp1->Cishu < Tmp2->Cishu){
			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL) exit(1);

			xNew->Xishu = Tmp1->Xishu;
			xNew->Cishu = Tmp1->Cishu;
			xNew->next = NULL;
			end->next = xNew;
			end = xNew;
			
			Tmp1 = Tmp1->next;
		}//������ʽ1ָ������2
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

	//��ʣ���δɨ���������������
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

//�˷� 
Xiang * MultDXS(Xiang * X1, Xiang * X2)
{
	Xiang *ans, *Tmp1, *Tmp2, *Mult1, *Mult2, *xNew, *end;

	Mult1 = InitDXS(); //�˷�����ʱ���м���� 
	Mult2 = InitDXS();
	Tmp1 = X1->next;

	while(Tmp1 != NULL){
		FreeDXS(Mult1);
		end = Mult1;
		Tmp2 = X2->next;

		while(Tmp2 != NULL){			
			xNew = (Xiang *)malloc(sizeof(Xiang));
			if ( xNew == NULL) exit(1);
			
			xNew->Xishu = Tmp1->Xishu * Tmp2->Xishu; //ϵ����� 
			xNew->Cishu = Tmp1->Cishu + Tmp2->Cishu; //ָ����� 
			xNew->next = NULL;
			end->next = xNew;//��β���������� 
			end = xNew;
			Tmp2 = Tmp2->next;
		}
		
		ans = AddDXS(Mult1, Mult2); //�õ��Ķ���ʽ��� 
		FreeDXS(Mult2);
		
		Mult2->next = ans->next;
		Tmp1 = Tmp1->next;
	}
	
	DesDXS(Mult1);
	free(Mult2);
	
	return ans;
}

//��
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

		xNew->Xishu = Tmp->Xishu * Tmp->Cishu; //������ϵ����� 
		xNew->Cishu = Tmp->Cishu - 1; //������1 

		end->next = xNew;
		xNew->next = NULL;
		end = xNew; //β���������� 
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
	printf("���ʽ1��\n");
	OutputDXS(X1);
	printf("�ڶ������ʽ��\n"); 
	printf("���ʽ2��\n");
	OutputDXS(X2);
	
	ans = AddDXS(X1, X2);
	printf("���ʽ1 + ���ʽ2 = ");
	OutputDXS(ans);
	DesDXS(ans);

	ans = SubDXS(X1, X2);
	printf("���ʽ1 - ���ʽ2 = ");
	OutputDXS(ans);
	DesDXS(ans);

	ans = DerDXS(X1);
	printf("���ʽ1�� = ");
	OutputDXS(ans);
	DesDXS(ans);
	
	ans = MultDXS(X1, X2);
	printf("���ʽ1 * ���ʽ2 = ");
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

//�������ʽ 
bool GetPlnm(Plnm &P) 
{
	int i,j;
	printf("������ö���ʽ����ߴ���:\n");
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
		printf("�����������\n");
		scanf("%d",&i);
		if(i<0||i>P.ZuiGaoCi)
		{
			printf("error!\n");
			continue;
		}
		else
		{
			printf("������ϵ����\n");
			cin>>P.Xishu[i];
		}
				
		printf("������ɶ���ʽ���ã�������0, ��Ҫ�����������ʽ��������1.\n");
		scanf("%d",&j); 
	}

	return 0;
}

//����ʽ��� 
bool AddPlnms( Plnm P1, Plnm P2, Plnm &ans )
{
	int i;
	ans.ZuiGaoCi=(P1.ZuiGaoCi>=P2.ZuiGaoCi)?P1.ZuiGaoCi:P2.ZuiGaoCi; //�������ߴ�Ϊ�����нϴ����ߴ� 
	ans.Xishu=( int * )malloc( (ans.ZuiGaoCi+1) * sizeof(int) );
	for(i=0;i<=ans.ZuiGaoCi;i++) ans.Xishu[i]=0; //��ʼ�� 
	
	int lmin=(P1.ZuiGaoCi<=P2.ZuiGaoCi)?P1.ZuiGaoCi:P2.ZuiGaoCi;	
	
	for(i=0;i<=lmin;i++)
	{
		ans.Xishu[i]=P1.Xishu[i]+P2.Xishu[i]; //��� 
	}
	
	//����һ������ʣ���ʱ�򣬰�ʣ������������ 
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

//����ʽ��� 
bool SubPlnms( Plnm P1, Plnm P2, Plnm &ans )
{
	int i;
	ans.ZuiGaoCi=(P1.ZuiGaoCi>=P2.ZuiGaoCi)?P1.ZuiGaoCi:P2.ZuiGaoCi; //ȡ�ϴ����ߴ� 
	ans.Xishu=( int * )malloc( (ans.ZuiGaoCi+1) * sizeof(int) );
	for(i=0;i<=ans.ZuiGaoCi;i++) ans.Xishu[i]=0; //��ʼ�� 
	
	int lmin=(P1.ZuiGaoCi<=P2.ZuiGaoCi)?P1.ZuiGaoCi:P2.ZuiGaoCi;
	
	for(i=0;i<=lmin;i++)
	{
		ans.Xishu[i]=P1.Xishu[i]-P2.Xishu[i];//��� 
	}
	
	//��һ����ʣ���ʱ����ӵ������ȥ 
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

//�� 
bool DePlnms( Plnm &P )
{
	int i;
	for(i=0;i<=P.ZuiGaoCi;i++)
	{
		if(i==0&&P.Xishu[0]!=0) P.Xishu[0]=0; //�����һ�� 
		
		if(P.Xishu[i]!=0)
		{
			P.Xishu[i-1]=P.Xishu[i]*i;
			P.Xishu[i]=0;
		}		
	}
	
	P.ZuiGaoCi--;//��ߴμ�1 
	
	return 0;

}

//��� 
bool MulPlnms( Plnm P1, Plnm P2, Plnm &ans )
{
	int i,j;
	ans.ZuiGaoCi=P1.ZuiGaoCi+P2.ZuiGaoCi; //ȡ��������ʽ��ߴ�֮����Ϊ�������ߴ� 
	ans.Xishu=( int * )malloc( (ans.ZuiGaoCi+1) * sizeof(int) );
	for(i=0;i<=ans.ZuiGaoCi;i++) ans.Xishu[i]=0; //��ʼ�� 

	for(i=0;i<=P1.ZuiGaoCi;i++)
	{
		if(P1.Xishu[i]!=0)
		{
			for(j=0;j<=P2.ZuiGaoCi;j++)
			{
				ans.Xishu[i+j]+=P1.Xishu[i]*P2.Xishu[j];//������˺��ۼ� 
			}
		}
		else continue;
	}
	
	return 0;
}

//��ն���ʽ 
bool ClearPlnm( Plnm &P )
{
	P.ZuiGaoCi=-1;
	return 0;
}

//��ӡ����ʽ 
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
	
	printf("----------����ʽ���׼�����----------\n");
	printf("�������һ������ʽ:\n");
	GetPlnm(P1);
	printf("������ڶ�������ʽ:\n");
	GetPlnm(P2);

	printf("�ӷ���\n"); 
	AddPlnms( P1, P2, ans );
	PrintPlnm( ans );
	
	printf("������\n"); 
	SubPlnms( P1, P2, ans );
	PrintPlnm( ans );

	printf("�˷���\n"); 
	MulPlnms( P1, P2, ans );
	PrintPlnm( ans );
	
	printf("��һ������ʽ�󵼣�\n"); 
	DePlnms( P1 );
	PrintPlnm( P1 );
	
	printf("�ڶ�������ʽ�󵼣�\n"); 
	DePlnms( P2 );
	PrintPlnm( P2 );
	
	return 0;
}

const int MAXSIZE=30;
int x=0;//������ֵ 
int ShuChu=0; //���������� 
int YouXianJi[8]={0,0,2,1,0,1,0,2};//��������ȼ�
int HaShHao=0;
char HanShuMing[10][5];
char HanShuTi[10][MAXSIZE];

//top�洢ջ���ǵڼ���Ԫ�أ�ջ�׼�Ϊdata[0] 
typedef struct 
{
	int  data[MAXSIZE];
	int  top; 
} Stack;

Stack  sta,sta2;

int HouJiSuan(Stack &sta, int *Hou, int length);
void ZhongtoHou(Stack &sta, char *Zhong, int *Hou, int *length);

//��ʼջ 
void initStack(Stack *sta)
{
	int  i;

	for (i=0;i<MAXSIZE;i++)
	{
		sta->data[i] = 0; //����ֵ��Ϊ0 
	}
	sta->top = -1;

}

//���ջ
void EmptyStack(Stack &sta)
{
	int i;
	
	for(i=0;i<sta.top;i++)
		sta.data[i] = 0; //��ֵ��Ϊ0 
	
	sta.top=-1; //ջ����Ϊ-1 
	
	return;
}

//����ջ
bool Pop(Stack &sta, int &e)
{
	if(sta.top==0) return 1;
	
	e=sta.data[sta.top];
	sta.top--;
	
	return 0;
}

//��ջ 
bool Push(Stack &sta, int e)
{

	sta.data[sta.top++]=e;

	return 0;
}

//���������ʽ�Ŀ�ͷΪ��DEF�� 
int Def(char *Zhong)
{
	ShuChu=0;
	int i=4;
	int HanShu=0;
			
	//��һ�γ��������ţ���ʼ��ȡ������ 
	while(Zhong[i]!='(')
	{
		HanShuMing[HaShHao][HanShu]=Zhong[i];
		i++;
		HanShu++;
	}
		
	HanShu=0; 
			
	while(Zhong[i-1]!='=') i++;
			
	//���ֵ��ں�֮�󣬿�ʼ��ȡ������ 
	while(Zhong[i]!=0)
	{
		HanShuTi[HaShHao][HanShu]=Zhong[i];
		i++;
		HanShu++;
	}
			
	//��������ż�1 
	HaShHao++;
			
	printf("������ĺ�����Ϊ��%s��������Ϊ��%s\n",HanShuMing[HaShHao-1],HanShuTi[HaShHao-1]);
	return 0;
}

//������ʽ�Ŀ�ͷΪ��RUN�� 
int Run(char *Zhong,int func) //func: 0,�ڲ㺯��;1:����㺯�� 
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
		tmp[j]=0;//��ʼ��tmp 
	}
	j=0;
						
	while(Zhong[i]!='(')
	{
		tmp[j]=Zhong[i];//��ȡ������ 
		i++;
		j++;
	}
			
	for(j=0;j<HaShHao;j++)
	{
		if(strcmp(tmp,HanShuMing[j])==0) {
			printf("�ҵ�Ŀ�꺯����\n");
			break;
		}
		//���������� 
	}
			
	if(j>=HaShHao&&func)
	{
		printf("û���ҵ��ú���\n");
		ShuChu=0; 
		return -1;
	}
			
	i++;
			
	//��ȡ�Ա�����ֵ 
	while(Zhong[i]!=')')
	{				
		x=x*10+(Zhong[i]-'0');
		i++;
	}
	
	//�Ѻ����忽������׺���ʽ�У�Ȼ��ʼ������׺���ʽ 
	strcpy(Zhong2,HanShuTi[j]);
	
	initStack(&sta);
	ZhongtoHou(sta, Zhong2, Hou, &length); //������׺���ʽ 
	initStack(&sta);
	ans=HouJiSuan(sta, Hou, length);
	
	if(ShuChu) printf("������Ϊ : %d\n",ans);
	return ans;
}

//�жϷ�֧ 
void DEForRUN(char *Zhong)
{
	int i;
	for (i=0;i<strlen(Zhong);)
	{
		//DEFΪ���뺯���ı�־���Ѻ������ͺ�����ֱ����������ά������ 
		if(Zhong[i]=='D'&&Zhong[i+1]=='E'&&Zhong[i+2]=='F') 
		{
			Def(Zhong);
			return;
		}
		
		//RUN Ϊ���к����ı�־��ͨ����HaShHao��������������������Ӧ�ĺ��� 
		else if(Zhong[i]=='R'&&Zhong[i+1]=='U'&&Zhong[i+2]=='N') 
		{	
			Run(Zhong,1);
			return;
		}
		//����Ȳ���DEFҲ����RUN��������ͨ������������ʽ���� 
		else{
			int  length,ans;
			int  Hou[MAXSIZE]; 

			initStack(&sta);
			ZhongtoHou(sta, Zhong, Hou, &length); //������׺���ʽ 
			initStack(&sta);
			ans=HouJiSuan(sta, Hou, length); //�����׺���ʽ 
			
			printf("������Ϊ : %d\n",ans);
			return;
		}
	}

}

//��׺���ʽ����׺���ʽ 
void ZhongtoHou(Stack &sta, char *Zhong, int *Hou, int *length)
{
	int  i;				
	int  b = 0;			
	int  j = 0;	
	int k = 0;		
	int  priority = 0;
	char BianL[10]; //�洢��һ������ 
	char BianL2[10]; //�洢�ڶ������� 
	int DiYiCi=0; //�ж��Ƿ��һ�γ��ֱ��� 
	int bi=0;
	int HanShu=0; //��ѭ�������±�ʹ�ã������൱��i��j 
	int  length2;
	int  Hou2[MAXSIZE]; 
	
	for (i=0;i<strlen(Zhong);)
	{
		//�����Ա���ʱ���Զ����������ֵ�� 
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
				if(strcmp(BianL,HanShuMing[k])==0) //����������
				{
					initStack(&sta2);
					ZhongtoHou(sta2, HanShuTi[k], Hou2, &length2); 
					//����������ڲ㺯�� �����ȼ����ڲ㺯�� 
					initStack(&sta2);
					Hou[j]=HouJiSuan(sta2, Hou2, length2);
//					printf("found another:%d", Hou[j]);
					j++;
					i+=3;
					break; 
				}
			}
			//���Ա�����ֵ�����׺���ʽ 
			
			if(k>=HaShHao) //���û���������ڲ㺯�������յ��������� 
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
				printf("��������һ�£�");
				exit(1); 
			}
			
			Hou[j]=x;
			j++;
			
			continue;
		}

		
		//�������֣��������׺���ʽ 
		if (Zhong[i] >= '0' && Zhong[i] <= '9')
		{
			b = 0;		
			while (Zhong[i] >= '0' && Zhong[i] <= '9')
			{
				b = b * 10 + (Zhong[i] - '0');//���ַ�ת��Ϊ���� 
				i++;
			}
			Hou[j] = b;
			j++;
			continue;
		}
		
		if (Zhong[i] == 41)//���������ţ�û����������ʱһֱ���� 
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

			priority = YouXianJi[sta.data[sta.top] % 10];//�������ȼ� 

			i++;
			continue;
		}
		//������
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
				//�����ջԪ�����ȼ�����ջ��Ԫ�أ��򵯳� 
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
				priority = YouXianJi[sta.data[sta.top] % 10];//�������ȼ� 
				i++;
			}
			else 
			{
				//���������������-�� 
				if (Zhong[i] == 45 && sta.data[sta.top] == 40)
				{
					b = 0;
					while (Zhong[i+1] >= '0' && Zhong[i+1] <= '9')
					{
						b = b * 10 + (Zhong[i+1] - '0');
						i++;
					}
					Hou[j] = b * -1;
					sta.data[sta.top] = 0;//��ջ 
					sta.top--;
					j++;
					i += 2;
					priority = YouXianJi[sta.data[sta.top] % 10];//�������ȼ� 

					continue;

				}

				sta.top++;
				sta.data[sta.top] = Zhong[i];
				priority = YouXianJi[sta.data[sta.top] % 10];
				i++;
			} 
		}
	}

	//����ʣ��Ԫ�� 
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
	*length=j;//���³��� 
}

//�����׺���ʽ 
int HouJiSuan(Stack &sta, int *Hou, int length)
{
	int i,j,ans;
	ans=0;

	for (i=0;i<length;i++)
	{
		//������׺���ʽ��������������ǣ����ans����Ӧ�Ĳ�����ѹջ 
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
		default://���������������Ѻ�׺���ʽ��ֵѹջ 
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
	int k2=1;//k���ƽ������ 

	printf("ʹ��ϸ��\n");	
	printf("��������Ϊ���ͼ�������֧�ֺ������幦�ܡ�\n");
	printf("�����ʹ�á�DEF f(x)=5*x+1����DEF g(x)=f(x)+4+x/2����RUN g(4)����������������к���\n");
	printf("���룺\n");	
	getchar();
	
	while(k2)
	{
		gets(Zhong);//��ȡ��׺���ʽ 
		strcpy(Zhong2,Zhong);
		
		DEForRUN(Zhong);
		printf("����������1���뿪������0��\n");
		scanf("%d",&k2);
		getchar();
	}
	
	return 0;

}

//���������
 
void matrix_output(float dis[][25],int m,int n);
int determinant(float a[25][25],int n);
void cofactor(float num[25][25],int f);
void transpose(float num[25][25],float fac[25][25],int r);

void transpose_input(float a[][25],int m,int n) //ת�ü��� 
{
	int i,j;
	float a2[25][25]; 
	for (i=0;i<n;i++)
    {
     for (j=0;j<m;j++)
       {
         a2[i][j]=a[j][i]; //���жԵ� 
        }
    }
    printf("�����ת��Ϊ:\n");
    matrix_output(a2,n,m);
}

 //����ʽ���� 
void determinant_input(float a[25][25],int m, int n)
{
  if(m!=n){
  	printf("�Ƿ���������ʽ��\n");
	return; 
  }
  int d;
  int i,j;
  d=determinant(a,n); //��������ʽ 
  printf("����ʽΪ %d \n",d);
}

//������� 
void matrixadd(float a[][25],float b[][25],int ma,int na,int mb,int nb)
{
	if(ma!=mb||na!=nb){
		printf("ά����ͬ���޷����\n");
		return; 
	}
	int i,j;
	float ans[25][25];
	for(i=0;i<ma;i++)
	{
		for(j=0;j<na;j++)
		ans[i][j]=a[i][j] +b[i][j];
	}
	printf("������� \n");

	matrix_output(ans,ma,na);
}

//������� 
void matrixsub(float a[][25],float b[][25],int ma,int na,int mb,int nb) //������� 
{
	if(ma!=mb||na!=nb){
		printf("ά����ͬ���޷����\n");
		return; 
	}
	int i,j;
	float ans[25][25]; //���ս�� 
	for(i=0;i<ma;i++)
	{
		for(j=0;j<na;j++)
		ans[i][j]=a[i][j] - b[i][j];
	}
	printf("������� \n");

	matrix_output(ans,ma,na);
}

//��ӡ���� 
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


//�������棺�������������ԭ����İ�������������ʽ 
void inverse(float a[25][25],int m,int n) //�������� 
{
  int k,d;
  k=n;
  if(m!=n){
  	printf("�Ƿ����޷�������ʽ���޷����档");
	return; 
  }
  d=determinant(a,k);
  printf("����ʽΪ %d",d);
  if (d==0)
   printf("\n����󲻴���\n");
  else
   cofactor(a,k); //��������ʽ��������� 
   printf("�������!\n");
   
   return;
}

//��������ʽ������չ�����ݹ���� 
int determinant(float a[25][25],int k)
{
  int s=1,det=0;
  float b[25][25];
  int i,j,m,n,c;
  if (k==1)
  {
     return (a[0][0]);//һ�׷��������ʽΪ��ΨһԪ�� 
  }
  else
  {
     det=0;
     for (c=0;c<k;c++) //���յ�һ��չ�� 
     {
        m=0;
        n=0;
        for (i=0;i<k;i++)
        {
            for (j=0;j<k;j++)
            {
				//��������ʽ 
                b[i][j]=0;//������ֵ 
                if (i != 0 && j != c) //��ȥ��һ������Ԫ�ص��к��� 
                {
                   b[m][n]=a[i][j];
                   if (n<(k-2))
                     n++;
                   else
                   {
                     n=0;
                     m++;//���� 
                    }
                }
            }
        }
        
		det=det + s * (a[0][c] * determinant(b,k-1)); //�ݹ麯�� 
        s=-1 * s;
        }
    } 

    return (det);
}

//��������� 
void cofactor(float num[25][25],int f) 
{
 float b[25][25],fac[25][25]; //b����Ű������ÿ��ֵ��Ӧ������ʽ��fac����Ű������ 
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
            b[m][n]=num[i][j]; //��������ʽ 
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
      fac[q][p]=pow(-1,q + p) * determinant(b,f-1); //�����������Ӧ������ֵ 
    }
  }
  transpose(num,fac,f);
}

//�������յ������
void transpose(float num[25][25],float fac[25][25],int r)
{
  int i,j;
  float b[25][25],inverse[25][25],d;
  //inverse��������,b��Ű�������ת�� 
  
  //����������
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
        inverse[i][j]=b[i][j] / d; //������Ӧ��ֵ���ڰ������ת�õ�ֵ��������ʽ 
        }
    }
   printf("\n�����Ϊ : \n");

   matrix_output(inverse,r,r);

}

void matrixmultiply(float a[25][25],float b[25][25],int ma,int na,int mb,int nb)
{
	if(na!=mb){
		printf("ά�������ϳ˷�Ҫ��\n");
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
	
	printf("������ˣ�\n"); 
	matrix_output(multi,ma,nb);
}

int matrix_all()
{
	float _a[25][25],_b[25][25];
	int _ma,_na,_mb,_nb,i,j;
	printf("--------------���������--------------\n");
	printf("�������������󣬸ü������������������ļӷ���������\n�Լ���һ�����������ʽ��ת�ú������\n");
	printf("�����һ�������ά�ȣ���*�У�\n");
	scanf("%d %d",&_ma,&_na);
	printf("�����һ������\n"); 
	for(i=0;i<_ma;i++)
	{
		for(j=0;j<_na;j++)
		scanf("%f",&_a[i][j]);
	}
	
	printf("����ڶ��������ά�ȣ���*�У�\n");
	scanf("%d %d",&_mb,&_nb);	
	printf("����ڶ�������\n"); 
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

//top�洢ջ���ǵڼ���Ԫ�أ�ջ�׼�Ϊdata[0] 
typedef struct 
{
	float  dataf[MAXSIZE];
	int  top; 
} Stackf;

//��ʼջ 
void initStackf(Stackf *sta)
{
	int  i;

	for (i=0;i<MAXSIZE;i++)
	{
		sta->dataf[i] = 0;
	}
	sta->top = -1;

}

//��׺���ʽת��Ϊ��׺���ʽ 
void ZhongtoHouf(Stackf &sta, char *Zhong, float *Hou, int *length)
{
	int  i;				
	int  b = 0;			
	int  j = 0;
	int  bi = 0; //bi��¼buff�е�λ��		
	int  priority = 0; //���ȼ� 
	char buff[10]; //��ȡ���ֶ�ʱ�����ݴ����� 

	for (i=0;i<strlen(Zhong);)
	{
		
		if (Zhong[i] >= '0' && Zhong[i] <= '9')
		{
			b = 0;
					
			while ((Zhong[i] >= '0' && Zhong[i] <= '9')||Zhong[i]=='.')
			{
				buff[bi] = Zhong[i];
	//			b = b * 10 + (Zhong[i] - '0'); //�����Ǹ��������㣬�����д���������� 
				i++;
				bi++;
			}
			buff[bi]=0;
							
			sscanf(buff, "%f", &Hou[j]);//���ַ�����ת��Ϊ������ 
			
			if(Zhong[i]=='e')
			{
				i++;
				Hou[j]*=pow(10,Zhong[i]-'0');
				i++;
			}
			
			memset(buff,0,bi);
			bi=0; //bi���� 
			j++;
			continue;
		}
		
		//��������ֵ 
		if (Zhong[i] == 'c'&&Zhong[i+1] == 'o'&&Zhong[i+2] == 's')
		{
			i+=3;
			b = 0;
					
			while ((Zhong[i] >= '0' && Zhong[i] <= '9')||Zhong[i]=='.')
			{
				b = b * 10 + (Zhong[i] - '0'); //�����Ǹ��������㣬�����д���������� 
				i++;
			}
			
			Hou[j]=cos((b/180.0)*PI);
			j++;
			continue;
		}
		
		//��������ֵ 
		if (Zhong[i] == 's'&&Zhong[i+1] == 'i'&&Zhong[i+2] == 'n')
		{
			i+=3;
			b = 0;
					
			while ((Zhong[i] >= '0' && Zhong[i] <= '9')||Zhong[i]=='.')
			{
				b = b * 10 + (Zhong[i] - '0'); //�����Ǹ��������㣬�����д���������� 
				i++;
			}
			
			Hou[j]=sin((b/180.0)*PI);
			j++;
			continue;
		}
		
		if (Zhong[i] == 41) //���������� 
		{
			while (sta.dataf[sta.top] != 40) //û����������֮ǰһֱ���� 
			{
				Hou[j] = sta.dataf[sta.top]; 
				sta.dataf[sta.top] = 0;

				sta.top--;

				j++;
			}
			sta.dataf[sta.top] = 0;
			sta.top--;

			priority = YouXianJi[((int)sta.dataf[sta.top] )% 10];//�������ȼ� 

			i++;
			continue;
		}
		
		//������
		if (Zhong[i] == 40)
		{
			sta.top++;
			sta.dataf[sta.top] = Zhong[i]; //������ѹջ 
			
			priority = YouXianJi[((int)sta.dataf[sta.top] ) % 10]; //�������ȼ� 
			i++;
			continue;

		}
		
		if (Zhong[i] >= 42 && Zhong[i] <= 47)
		{
			if (priority >= YouXianJi[Zhong[i] % 10])
			{
				while (priority >= YouXianJi[Zhong[i] % 10] && sta.dataf[sta.top] != 40)
				//���ȼ�С���������ջ 
				{
					Hou[j] = sta.dataf[sta.top];
					sta.dataf[sta.top] = 0;
					sta.top--;//��������׺���ʽ 
					priority = YouXianJi[((int)sta.dataf[sta.top] )% 10];

					j++;

				}

				//һ������µ�ѹջ 
				sta.top++; 
				sta.dataf[sta.top] = Zhong[i];
				priority = YouXianJi[((int)sta.dataf[sta.top] )% 10];
				i++;
			}
			else 
			{
				//���������������- �� 
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
					sscanf(buff, "%f", &Hou[j]);//����buff 
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

	//����ʣ���Ԫ�� 
	while (sta.top != -1)
	{
		Hou[j] = sta.dataf[sta.top];
		sta.top--;
		j++;
	}

	*length=j;
}

//��׺���ʽ�ļ��� 
float HouJiSuanf(Stackf &sta, float *Hou, int length)
{
	int i,j;
	float ans;
	ans=0;

	for (i=0;i<length;i++)
	{
		float tmp=Hou[i]; //�����׺���ʽʱ���棨int��Hou[i]֮ǰ�ĸ���ֵ��
		switch ((int)Hou[i])
		{
		case 42:
			ans=sta.dataf[sta.top-1]*sta.dataf[sta.top];//��ans������Ӧ�����ѹջ 
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
			sta.dataf[sta.top]=tmp; //����ԭʼֵ 
			break;
		}
	}
	return ans;
}

int SiZeYunSuan_float_all()
{
	Stackf  sta;
	int  length;
	float ans;         //���ܺ�׺���ʽת���Ľ��
	float  Hou[MAXSIZE];  //�洢��׺���ʽ
	char Zhong[MAXSIZE];  //�洢��׺���ʽ

	printf("--------�������������������--------\n");
	printf("�����ʹ������1e1��cos45����-1��������ʽ\n");	
	printf("��������������ʽ������������ţ���");	
	scanf("%s",Zhong);
	initStackf(&sta); 
	ZhongtoHouf(sta,Zhong,Hou,&length);//��׺����׺ת�� 
	
	initStackf(&sta); 
	ans=HouJiSuanf(sta,Hou,length);
	printf("%.2f\n",ans);
	return 0;
}

void menu()
{
	printf("\n------------CALCULATOR BY 2017202121-----------\n");
	printf("1 �������㣨�ӡ����������ң�\n");
	printf("2 ����ʵ�ֶ���ʽ���㣨�ӡ������ˡ��󵼣�\n");
	printf("3 �������������������\n");
	printf("4 ���������(�ӡ������ˡ����桢ת�á�������ʽ)\n");
	printf("5 �������������������\n");
	printf("6 ���Ա�ʵ�ֶ���ʽ���㣨�ӡ������ˡ��󵼣�\n");
	printf("0 �˳�\n");
}

int main()
{
	int k=8;//���ƽ��� 
	
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
			printf("����ֵ��Ч��\n");
			k=1;
		}
	}
}
