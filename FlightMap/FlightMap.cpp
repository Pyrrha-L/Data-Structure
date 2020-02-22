/*
**
��Ŀ���ƣ����չ�˾����ͼ 
���ߣ�����ͯ
ʱ�䣺2018.12. 
**
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PortNum 10
#define RouteNum 120
#define FieldNum 7
#define ResSize 20

//��������״̬ 
typedef int Status;
//�������� 
typedef int StPortNum;

//·����Ϣ�ṹ�� 
typedef struct RouteInfo
{
	int DprtTime; //����ʱ�� 
	int FlgtLength; //����ʱ�� 
	int ArivTime;
	int Cost; //������� 
	int StrtPoint; //������ 
	int EndPoint; //Ŀ�ĵ� 
	int PlaneType; //�ɻ��ͺ� 
	int FlgtID; //����ID 
}RouteInfo;

//�������ṹ�� 
typedef struct RouteNode{
	int RouteEnd; //�����յ� 
	RouteNode *nextroute; //��һ������ 
	RouteInfo info; //������Ϣ 
}RouteNode;
 
//����������� 
typedef struct StPortNode{
	StPortNum data; //���������ı�� 
	RouteNode *frstroute; //��һ������ 
}StPortNode,StPortList[PortNum];

//�ڽӱ�ṹ�� 
typedef struct{
	StPortList Ports;
	int _RouteNum; //������ 
	int _PortNum; //������ 
}PortGraph;

PortGraph _Gf;//ϵͳ�е��ڽӱ� 
PortGraph _Gb;//ϵͳ�е����ڽӱ� 

//�����ڽӾ���ߵĴ洢 
typedef struct ArcCell { //���Ķ���
	int rlt; // �����ϵ����,1��0ʾ���ڷ�
	RouteInfo info; // �û������Ϣ��ָ��
} ArcCell, RouteMatrix[PortNum][PortNum];

//�ڽӾ���ṹ�� 
typedef struct { // ͼ�Ķ���
	int Ports2[PortNum]; // ��������
	RouteMatrix RouteMap; // �ڽӾ���
	int _PortNum2, _RouteNum2; // ��������·���� 
} RMap;

RMap _Rm;//ϵͳ�е��ڽӾ��� 

//��λԪ�����ڽӱ��е�λ�á�G���ڽӱ�v������λԪ�ص�ֵ�� 
int LocatePort(PortGraph *G, StPortNum v)
{
	int i;
	for (i = 0; i < (G->_PortNum); i++)
	{
		if (G->Ports[i].data == v)
			return i;
	}
}

//ʱ��ת����00:00->24Сʱ�Ƶķ��Ӽ�����
//time����ʽΪ��4:00���Ĵ�ת���ַ����� 
int TimeTransform(char *time)
{
	int i=0,ans=0,k=0; 
	//iΪѭ��������ans���ڴ洢���ս����k����buff������ 
	int length=strlen(time);
	char buff[5];
	
	//��ȡСʱ 
	for(;i<length;i++)
	{
		if((time[i]<'0'||time[i]>'9')&&time[i]!=':'){
			return -1;
		}
		
		//����Ч�ַ�����buffer 
		if(time[i]!=':'){
			buff[k]=time[i];
			k++;
		}
		else{
			buff[k]=0;//�����ֹ�� 
			i++;
			break;
		}
	}
	
	//��ʽת�� 
	sscanf(buff,"%d",&ans);
	
	if(ans>23||ans<0){
		return -1; 
	}
	
	ans*=60; //���Сʱ��ת��
	k=0; 
	
	//��ȡ���� 
	for(;time[i]!=0;i++)
	{
		if(time[i]<'0'||time[i]>'9'){
			return -1;
		}
		//����Ч�ַ�����buffer 
		buff[k]=time[i];
		k++;
	}
	
	//����k�Ѿ�û�����ˣ���ʱ������һ�·��ӵ���ֵ�� 
	sscanf(buff,"%d",&k); 
	if(k>59||k<0){
//		printf("ʱ������");
		return -1;
	}
	
	ans+=k;
	
	return ans;
}

//�������ļ��ܷ������򿪵��жϡ�
//filename���ļ��� 
FILE * loadDataSrcFile(char *filename) {
	FILE *fp;
	if ((fp = fopen(filename, "r")) == NULL) {
		printf("�޷����ļ���\n");
		exit(0);//�˳�����
	}
	return fp; 
}

//��ÿһ�ж�ȡһ��������Ϣ������Destָ����ڴ��� 
//fp������ȡ�ļ���ָ�룻Dest��Ŀ�Ľṹ��ָ�� 
int rowscan(FILE *fp, RouteInfo *Dest)
{
	memset(Dest,0,sizeof(RouteInfo));
	char fields[7][10]; //ÿһ�����ݵ����� 
	char buff[30] = {0}, c; //�����ַ�����
	
	int field_count = 0 ,len; 		
	for(int i = 0; !feof(fp) && i<FieldNum; i++)
	{	
		//�̶�˳���7���ֶ�		
		int idx = 0; //���û���ռ�
		while((c = fgetc(fp)))
		{	//ɨ���ȡһ���ֶΣ� EOF��ʾ�ļ��ѽ��� 							
			if(c == ',' || c == '\n')
			{	//�ֶ����ݽ���
				buff[idx] = 0;
				break;
			}
			else if(c == EOF)
			{
				break;
			 } 
			else
			{   //��ÿ���ֶε����ݷ��뻺��
				buff[idx++] = c;
			}
		}
		
		len = strlen(buff);		
		char buff2[30]={0};
		
		//ʵ���ַ�����������ݵ�ת�� 
		switch(i)
		{
			case 0:
				strcpy(buff2,buff);
				Dest->DprtTime=TimeTransform(buff2);
				break;
			case 1:
				sscanf(buff,"%d", &Dest->FlgtLength);
				break;
			case 2:
				sscanf(buff,"%d", &Dest->Cost);
				break;
			case 3:
				sscanf(buff,"%d", &Dest->StrtPoint);
				break;
			case 4:
				sscanf(buff,"%d", &Dest->EndPoint);
				break;
			case 5:
				sscanf(buff,"%d", &Dest->PlaneType);
				break;
			case 6:
				sscanf(buff,"%d", &Dest->FlgtID);
				break;
			case 7:break;
		}
		
		field_count++;
	}
	
	//���㵽��ʱ�� 
	Dest->ArivTime=Dest->DprtTime+Dest->FlgtLength;
	
	return field_count;
}

//��ȡ���ݣ�ͬʱ�����ڽӱ�
//filename:�ļ�����Gf�������ڽӱ�Gb�������ڽӱ�M���ڽӾ��� 
int LoadFlightData(char *filename, PortGraph *Gf, PortGraph *Gb, RMap *M)
{
	//�ж��ļ��ܷ�򿪡� 
	FILE *fp = loadDataSrcFile(filename);
	
	char fields[7][10];
	char buff[30] = {0}, c; //�����ַ�����
	int row_count=0;
	int i,j;
	
	//����һ�е��ֶ���
	for(i=0; i<FieldNum-1; i++)
	{
		fscanf(fp, "%[^,],", fields[i]); 
		//%[^,]��ʾһֱ��ȡ������,Ϊֹ
	}
	fscanf(fp, "%s", fields[9]);	
	fgetc(fp); //������һ�еĻس���
	
	//���ڽӱ���ڽӾ�����л�����Ϣ������ 
	Gf->_PortNum=PortNum;
	Gf->_RouteNum=RouteNum; 
	Gb->_PortNum=PortNum;
	Gb->_RouteNum=RouteNum; 
	M->_PortNum2=PortNum;
	M->_RouteNum2=RouteNum;
	
	//�ڽӱ����ֵ 
	for (i=1;i<=PortNum; i++)
	{
		Gf->Ports[i-1].data=i;
		Gf->Ports[i-1].frstroute=NULL;
		M->Ports2[i-1]=i;
		Gb->Ports[i-1].data=i;
		Gb->Ports[i-1].frstroute=NULL;	
	}
	
	//�ڽӾ������ֵ 
	for(i=0;i<PortNum;i++)
	{
		for(j=0;j<PortNum;j++)
			M->RouteMap[i][j].rlt=0;
	}
	
	//����洢�ռ� 
	RouteInfo *p;
	p=(RouteInfo *)malloc(sizeof(RouteInfo));
	
	//���������ڽӱ�Ĵ��� 
	RouteNode *r1, *r2;
	
	while(rowscan(fp,p))
	{//ɨ���ļ��е�ÿ������
		
		//�ų��ļ�ĩβ�ĸ�����
		if(p->EndPoint==0){
			break;
		} 
		
		int i, j, k;
		StPortNum v1, v2;
		
		//����Ԫ��λ�� 
		i = LocatePort(Gf, p->StrtPoint);
		j = LocatePort(Gf, p->EndPoint);
		
		//Ϊ����Ԫ�ط���洢�ռ� 
		r1=(RouteNode *)malloc(sizeof(RouteNode));
		r2=(RouteNode *)malloc(sizeof(RouteNode));
		
		//�ڽӱ���ν� 
		r1->info = *p;
		r1->RouteEnd = p->EndPoint ;
		r1->nextroute = Gf->Ports[i].frstroute;
		Gf->Ports[i].frstroute = r1;
		
		//���ڽӱ��ν� 
		r2->info = *p;
		r2->RouteEnd = p->StrtPoint ;
		r2->nextroute = Gb->Ports[j].frstroute;
		Gb->Ports[j].frstroute = r2;
		
		//�ڽӾ�������
		
		M->RouteMap[i][j].rlt=1; //������ʾ������ת 
		M->RouteMap[i][j].info = *p; 
		//���µĺ�����Ϣ����RouteMap�С� 
		
		row_count++;
	}
	
	fclose(fp);
	
	return row_count;
}

int lsttime=4000; //Ϊ��̷���ʱ�������Ͻ� 
int tmpdprttime=0; //�ݴ����ʱ�� 

//�������������·��
//p2��Ŀ�ĵء�p������ָ�� 
void DFS_lsttime(int p2,RouteNode *p){
	
	//�������յ� 
	if(p->info.EndPoint==p2){
		if(p->info.ArivTime-tmpdprttime<lsttime) {			
			lsttime=p->info.ArivTime-tmpdprttime;
		}
		return;
	}
	
	//��Ҷ�ӽڵ����ֹ���� 
	if(p==NULL){
		return;
	}
	
	RouteNode *tmp;
	tmp=_Gf.Ports[p->info.EndPoint-1].frstroute;
	while(tmp)
	{	
		//������ת���ݹ�������� 
		if(tmp->info.DprtTime>=p->info.ArivTime&&
		tmp->RouteEnd!=p->info.StrtPoint)//��֤�ɻ����ط� 
		{
			
			DFS_lsttime(p2,tmp);
		}
		tmp=tmp->nextroute;
	}
	
}

//��ѯ��������������̷���ʱ�� PortGraph *G
void Choice1()
{
	int port1,port2;
	printf("ϵͳ�������Ϊ 1 - %d \n", PortNum );
	printf("�����������ѯ�Ļ�����ţ��м�ո�������硰2 3����: \n");
	
	scanf("%d %d",&port1,&port2);
	if(port1<1||port1>PortNum||port2<1||port2>PortNum)
	{
		printf("������Ļ����������\n");
		return; 
	}
	else{
		lsttime=4000;//ÿ�ζ�lsttime���¸�ֵ 
		RouteNode *p;//����ָ�� 
		p=_Gf.Ports[port1-1].frstroute;
		while(p)
		{	
			tmpdprttime=p->info.DprtTime;
			DFS_lsttime(port2,p);
			p=p->nextroute;			
		}
		
		//���lsttimeû�иı䣬��˵��û����صĺ��� 
		if(lsttime!=4000){
			printf("�ӻ���%d������%d�����ʱ��Ϊ��%d����\n",port1,port2,
			lsttime);
		} 
		else{
			printf("û�з���Ҫ��ĺ��ߡ�\n");
		}
		
		return;
	}
} 

//�Եڶ���ܵ�ʵ�֡�
//p1:����������p2��Ŀ�Ļ����� 
void solvechoice2(int p1, int p2)
{
	int i,j,k;
	int ans1,ans2,ans3;
	int tmptime,tmpend,tmptime2,tmpend2; 
	RouteNode *cursor;
	
	//ֱ��
	if(_Rm.RouteMap[p1-1][p2-1].rlt){
		i=0;
		while(i<_Rm.RouteMap[p1-1][p2-1].rlt)
		{
			printf("ֱ�ɺ���ID��%d\n",_Rm.RouteMap[p1-1][p2-1].info.FlgtID);
			i++;
		}
	}
	
	//1����ת�������ڲ�һ��ѭ���� 
	for(i=0;i<PortNum;i++)
	{
		if(_Rm.RouteMap[p1-1][i].rlt==0) continue;
		
		tmptime=_Rm.RouteMap[p1-1][i].info.ArivTime;
		tmpend=_Rm.RouteMap[p1-1][i].info.EndPoint-1;
			
			for(j=0;j<PortNum;j++)
			{	
				if(_Rm.RouteMap[tmpend][j].info.EndPoint==p2&&
					_Rm.RouteMap[tmpend][j].info.DprtTime>tmptime)
					{
						printf("��תһ�κ���ID��%d %d \n",
						_Rm.RouteMap[p1-1][i].info.FlgtID,
						_Rm.RouteMap[tmpend][j].info.FlgtID); 
					}
			} 		
	}
	
	//2����ת���ھ����ڶ��һ��ѭ���� 
	for(i=0;i<PortNum;i++)
	{
		//�ų�û��ͨ·������� 
		if(i==p1-1||_Rm.RouteMap[p1-1][i].rlt==0) continue;
		
		tmptime=_Rm.RouteMap[p1-1][i].info.ArivTime;
		tmpend=_Rm.RouteMap[p1-1][i].info.EndPoint-1;
				
			for(j=0;j<PortNum;j++)
			{	
				//�ų�û��ͨ·���������֤����ת�ԡ� 
				if((!_Rm.RouteMap[tmpend][j].rlt)||
				_Rm.RouteMap[tmpend][j].info.DprtTime<tmptime||j==tmpend) continue; 
				
				tmptime2=_Rm.RouteMap[tmpend][j].info.ArivTime;
				tmpend2=_Rm.RouteMap[tmpend][j].info.EndPoint-1;
				
				for(k=0;k<PortNum;k++)
				{
					//�ų�û��ͨ·�����������֤����ת�ԡ� 
					if((!_Rm.RouteMap[tmpend2][k].rlt)||
					_Rm.RouteMap[tmpend2][k].info.DprtTime<tmptime2||k==tmpend2) continue;
							
					if(_Rm.RouteMap[tmpend2][k].info.EndPoint==p2&&
					_Rm.RouteMap[tmpend2][k].info.DprtTime>tmptime2)
					{
						printf("��ת���κ���ID��%d %d %d\n",
						_Rm.RouteMap[p1-1][i].info.FlgtID,
						_Rm.RouteMap[tmpend][j].info.FlgtID,
						_Rm.RouteMap[tmpend2][k].info.FlgtID); 
					}
				}
				
			} 		
	}
	
	return;
}

//�ڶ���ܵĽӿ� 
void Choice2()
{
	int p1,p2;
	printf("�����������ѯ����ĳ���������ź�Ŀ�Ļ�����ţ���ʽΪ��1 2����\n");
	scanf("%d %d",&p1,&p2);
	
	printf("��ѯ������£�\n"); 
	solvechoice2(p1,p2); 
} 
 
int result[ResSize][ResSize]={0};//�洢��ѯ��� 
int res_count=0;//��ѯ��������� 
int step=0;//�ݹ���� 

//��ʼ����ѯ��Ϣ 
void InitRes()
{
	memset(result,0,sizeof(int)*ResSize*ResSize);
	res_count=0;
	step=0;
	return; 
} 

//�����йط��á�
//res������洢���顣sum:����Ҫ��Ľ�������� 
int CalFee(int *res,int sum)
{
	int i,j,k;//ѭ������ 
	int ans=0;
	
	for(i=0;i<PortNum;i++)
	{
		for(j=0;j<PortNum;j++)
		{
			for(k=1;k<=sum;k++)
			{	
				//���ݲ�ѯ����д洢�ĺ���ID���ҳ���Ӧ�ķ��ò���ӡ�		
				if(_Rm.RouteMap[i][j].info.FlgtID==res[k]){
					ans+=_Rm.RouteMap[i][j].info.Cost;
				}
			}
		}
	}
	return ans;
}

//�����С�ķ���
//res���������Ķ�ά���顣sum����ѯ����ĸ����� 
int GetMinFee(int res[][ResSize],int sum)
{
	int tmp=10000;
	int i,ans=0;
	
	//����Сֵ�� 
	for(i=0;i<sum;i++)
	{
		ans=(res[i][0]<tmp)?i:ans;
		tmp=(res[i][0]<tmp)?res[i][0]:tmp;
	}
	
	//��ӡ��Ӧ�Ľ��
	if(tmp==10000){
		printf("\nû���ҵ�����Ҫ��ĺ��ߡ�\n");
		return 1;
	}
	
	printf("\n���ٻ���Ϊ��%d\n",tmp);
	printf("��Ӧ��·�ߺ���IDΪ��"); 
	for(i=1;res[ans][i]!=0;i++)
	{
		printf("%d ",res[ans][i]);
	}
	printf("\n");
	
	return 0;
}

//������ȱ����������ڽӱ�
//p2:Ŀ�ĵء�p������ָ�� 
void DFS_f(int p2,RouteNode *p){
	//��ȼ�һ 
	step++;
	if(p->info.EndPoint==p2){
		result[res_count][step]=p->info.FlgtID;
	
		//������
		printf("\n����Ҫ��ĺ���Ϊ������ID-������򣩣�"); 	
		int i=1;
		for(i;i<=step;i++)
		{
			if(result[res_count][i]) printf("%d-%d ",result[res_count][i],i);
		}
		for(i;i<ResSize;i++)
		{
			result[res_count][i]=0;
		}
		printf("\n");
		
		result[res_count][0]=CalFee(result[res_count],step);
		printf("�ú��ߵĿ���Ϊ��%d \n",result[res_count][0]);
		
		res_count++;
		
		//Ϊ�ֲ�·�߿���ԭ��·��
		for(i=1;i<10;i++)
		{
			result[res_count][i]=result[res_count-1][i];
		} 
		return;
	}
	
	//��ֹ�ݹ����� 
	if(p==NULL){
		return;
	}
	
	//�ѵ�ǰ����ID���������� 
	result[res_count][step]=p->info.FlgtID;
	
	RouteNode *tmp;
	tmp=_Gf.Ports[p->info.EndPoint-1].frstroute;
	while(tmp)
	{	
		if(tmp->info.DprtTime>=p->info.ArivTime&&
		tmp->RouteEnd!=p->info.StrtPoint)//��֤�ɻ����ط��Լ�������ת 
		{
			DFS_f(p2,tmp);
			step--;	
		}
		tmp=tmp->nextroute;
	}
	
	result[res_count][step]=0;
}

//���ڽӱ����ȱ���
//p2:�����㡣p������ָ�� 
void DFS_b(int p2,RouteNode *p){
	step++;
	
	if(p->info.StrtPoint==p2){
		result[res_count][step]=p->info.FlgtID;
	
		//������
		printf("\n����Ҫ��ĺ���Ϊ������ID-������򣩣�"); 	
		int i=step;
		for(i;i>=1;i--)
		{
			if(result[res_count][i]) printf("%d-%d ",result[res_count][i],step-i+1);
		}
		for(i=step+1;i<ResSize;i++)
		{
			result[res_count][i]=0;
		}
		printf("\n");
		
		result[res_count][0]=CalFee(result[res_count],step);
		printf("�ú��ߵĿ���Ϊ��%d \n",result[res_count][0]);
		
		res_count++;
		
		//Ϊ�ֲ�·�߿���ԭ��·��
		for(i=1;i<10;i++)
		{
			result[res_count][i]=result[res_count-1][i];
		} 
		return;
	}
	
	if(p==NULL){
		return;
	}
	
	result[res_count][step]=p->info.FlgtID;
	
	RouteNode *tmp;
	//�����������ڽӱ� 
	tmp=_Gb.Ports[p->info.StrtPoint-1].frstroute;
	while(tmp)
	{	
		if(tmp->info.ArivTime<=p->info.DprtTime&&
		tmp->info.StrtPoint!=p->info.EndPoint)//��֤�ɻ����ط� 
		{
			DFS_b(p2,tmp);
			step--; 
		}
		tmp=tmp->nextroute;
	}
	
	//�ָ�ԭֵ 
	result[res_count][step]=0;	
}

//����޶����͵ı���
//p2:Ŀ�ĵء�p������ָ�롣t���޶����� 
void DFS_t(int p2,RouteNode *p,int t){
	step++;
	
	if(p->info.EndPoint==p2){
		result[res_count][step]=p->info.FlgtID;
	
		//������
		printf("\n����Ҫ��ĺ���Ϊ������ID-������򣩣�"); 	
		int i=1;
		for(i;i<=step;i++)
		{
			if(result[res_count][i]) printf("%d-%d ",result[res_count][i],i);
		}
		for(i;i<ResSize;i++)
		{
			result[res_count][i]=0;
		}
		printf("\n");
		
		result[res_count][0]=CalFee(result[res_count],step);
		printf("�ú��ߵĿ���Ϊ��%d \n",result[res_count][0]);
		
		res_count++;
		
		//Ϊ�ֲ�·�߿���ԭ��·��
		for(i=1;i<10;i++)
		{
			result[res_count][i]=result[res_count-1][i];
		} 
		return;
	}
	
	if(p==NULL){
		return;
	}
	//��¼��ǰ����ID 
	result[res_count][step]=p->info.FlgtID;
	
	RouteNode *tmp;
	tmp=_Gf.Ports[p->info.EndPoint-1].frstroute;
	while(tmp)
	{	
		if(tmp->info.PlaneType==t&&
		tmp->info.DprtTime>=p->info.ArivTime&&
		tmp->RouteEnd!=p->info.StrtPoint)//��֤�ɻ����ط����л����޶� 
		{
			DFS_t(p2,tmp,t); 
			step--;
		}
		tmp=tmp->nextroute;
	}
	
	//�ָ�����ID 
	result[res_count][step]=0;
}

//������ܽӿ� 
//s������������d��Ŀ�Ļ�����t1����ʼʱ��/���͡�t2����ֹʱ�䡣type��ѡ���ܡ� 
void solveChoice3(int s, int d, int t1, int t2, int type)
{	
	InitRes();
	
	int i,j;
	
	//����type��ֵ���벻ͬ�ĵݹ� 
	if(type==1){
		RouteNode *p;
		//�ݹ���� 
		p=_Gf.Ports[s-1].frstroute;
		while(p)
		{	
			//�޶�����ʱ�� 
			if(p->info.DprtTime>=t1&&p->info.DprtTime<=t2)
			{	
				step=0;
				DFS_f(d,p); 
			}
			p=p->nextroute;
		}
		//�������ٷ��� 
		GetMinFee(result,res_count);
	}	
	else if(type==2)
	{
		RouteNode *p;
		p=_Gb.Ports[d-1].frstroute;
		while(p)
		{	
		//	�޶�����ʱ�� 
			if(p->info.ArivTime>=t1&&p->info.ArivTime<=t2)
			{					
				step=0;
				DFS_b(s,p); 
			}
			p=p->nextroute;
		}
		GetMinFee(result,res_count);
	}
	else if(type==3)
	{	
		RouteNode *p;
		p=_Gf.Ports[s-1].frstroute;
		while(p)
		{	
			//�޶����� 
			if(p->info.PlaneType==t1)
			{
				step=0;
				DFS_t(d,p,t1); 
			}
			p=p->nextroute;
		}
		GetMinFee(result,res_count);
	}
}

//������ܵĽӿ�
//������ֵ��Ч��Ч���ж� 
void Choice3()
{
	int k;
	int p1,p2,type,timea2,timeb2;
	char timea[6],timeb[6];
	
	printf("�޶����ʱ�Σ�������1���޶�����ʱ�Σ�������2���޶����ͣ�������3��\n");	
	scanf("%d",&k);
	
	if(k==1)
	{
		printf("�밴��'��ʼʱ�� ��ֹʱ��'�ĸ�ʽ�������ʱ�Σ��确6:00 8:00��:\n"); 
		scanf("%s %s",&timea,&timeb);
		timea2=TimeTransform(timea);
		timeb2=TimeTransform(timeb);
		if(timea2<0||timeb2<0){
			printf("ʱ�����󣡣�\n");
			return; 
		}
		
		printf("�밴��'����������� Ŀ�Ļ������'�ĸ�ʽ���������ţ��确1 3����\n");
		scanf("%d %d",&p1,&p2);
		if(p1<1||p1>PortNum||p2<1||p2>PortNum){
			printf("��������\n");
			return;
		}
		solveChoice3(p1, p2, timea2, timeb2, 1);
	}
	else if(k==2)
	{
		printf("�밴��'��ʼʱ�� ��ֹʱ��'�ĸ�ʽ���뽵��ʱ�Σ��确6:00 8:00��:\n"); 
		scanf("%s %s",&timea,&timeb);
		timea2=TimeTransform(timea);
		timeb2=TimeTransform(timeb);
		if(timea2<0||timeb2<0){
			printf("ʱ������\n");
			return; 
		}
		
		printf("�밴��'����������� Ŀ�Ļ������'�ĸ�ʽ���������ţ��确1 3����\n");
		scanf("%d %d",&p1,&p2);
		if(p1<1||p1>PortNum||p2<1||p2>PortNum){
			printf("��������\n");
			return;
		}
		solveChoice3(p1, p2, timea2, timeb2, 2);
	}
	else if(k==3)
	{
		printf("�������޶��Ļ���:\n"); 
		scanf("%d",&type);
		if(type!=321&&type!=350&&type!=429&&type!=747){
			printf("��������\n");
			return; 
		}
		
		printf("�밴��'����������� Ŀ�Ļ������'�ĸ�ʽ���������ţ��确1 3����\n");
		scanf("%d %d",&p1,&p2);
		if(p1<1||p1>PortNum||p2<1||p2>PortNum){
			printf("��������\n");
			return;
		}
		solveChoice3(p1, p2, type, 0, 3);
	}
	else{
		printf("��Ч���룡\n");
	}
	return;
}

//����������Ҫ�ĵݹ�
//p2��Ŀ�ĵء�p������ָ�롣t��ʱ�䡣tmax��ʱ���Ͻ硣k��ѡ���� 
void DFS_4(int p2, RouteNode *p,int t,int tmax,int k)
{
	step++;
	if(p->info.EndPoint==p2){
		result[res_count][step]=p->info.FlgtID;
	
		//������
		printf("����Ҫ��ĺ���Ϊ��\n"); 	
		int i=1;
		for(i;i<=step;i++)
		{
			if(result[res_count][i]) printf("%d-%d ",result[res_count][i],i);
		}
		for(i;i<ResSize;i++)
		{
			result[res_count][i]=0;
		}
		printf("\n");

		res_count++;
		
		//Ϊ�ֲ�·�߿���ԭ��·��
		for(i=1;i<10;i++)
		{
			result[res_count][i]=result[res_count-1][i];
		} 
		return;
	}
	
	//��ֹ���� 
	if(p==NULL){
		return;
	}
	
	result[res_count][step]=p->info.FlgtID;
	
	RouteNode *tmp;
	tmp=_Gf.Ports[p->info.EndPoint-1].frstroute;
	
	//����ʹ�÷���ʱ��ĸ��� 
	if(k==1&&t==0) t+=p->info.FlgtLength;

	while(tmp)
	{	
		//ѡ���˷���ʱ��Ĺ��ܣ��豣֤��ǰ���÷���ʱ�䲻�����Ͻ� 
		if(k==1&&tmp->info.FlgtLength+t<=tmax&&
		tmp->info.DprtTime>=p->info.ArivTime&&
		tmp->RouteEnd!=p->info.StrtPoint)//��֤�ɻ����ط� 
		{
			DFS_4(p2,tmp,tmp->info.FlgtLength+t,tmax,1); 
			step--;
		}
		
		//ѡ������תʱ��Ĺ��ܣ��豣֤��ǰ��תʱ�䲻�����Ͻ� 
		if(k==2&&tmp->info.DprtTime-p->info.ArivTime+t<=tmax&&
		tmp->info.DprtTime>=p->info.ArivTime&&
		tmp->RouteEnd!=p->info.StrtPoint)
		{
			DFS_4(p2,tmp,tmp->info.DprtTime-p->info.ArivTime+t,tmax,2); 
		}
		
		tmp=tmp->nextroute;
	}

	result[res_count][step]=0;	
}

//����������õݹ���� 
//s:�����㡣d��Ŀ�ĵء�t��ʱ���Ͻ硣k��ѡ���� 
void SolveChoice4(int s,int d,int t,int k)
{
	RouteNode *p;
	p=_Gf.Ports[s-1].frstroute;
	while(p)
	{	
		step=0;
		DFS_4(d,p,0,t,k); //����k��ͬ������Ӧ�ݹ� 
		p=p->nextroute;
	}
}

//������ܵĽӿ�
//����Ϸ����ж� 
void Choice4()
{
	int k,key,p1,p2; 
	printf("�޶�����ʱ����1���޶���תʱ�䣺2�������룺");
	scanf("%d",&k);
	
	if(k==1){
		
		printf("���������ʱ����");
		scanf("%d",&key);
		if(key<=0){
			printf("ʱ������");
			return;
		}
		
		printf("��������ʼ�����������������ʽ��1 2����");
		scanf("%d %d",&p1,&p2);
		if(p1<1||p1>PortNum||p2<1||p2>PortNum){
			printf("��������");
			return;
		}
		
		SolveChoice4(p1,p2,key,1);
	}
	else if(k==2){
		printf("��������תʱ�䣺");
		scanf("%d",&key);
		if(key<=0){
			printf("ʱ������");
			return;
		}
		
		printf("��������ʼ�����������������ʽ��1 2����");
		scanf("%d %d",&p1,&p2);
		if(p1<1||p1>PortNum||p2<1||p2>PortNum){
			printf("��������");
			return;
		}
		
		SolveChoice4(p1,p2,key,2);
	}
	else{
		printf("��Ч���룡");
		return; 
	}
} 

//������ȱ���
//G���ڽӱ� 
void BFSGraph(PortGraph *G)
{
    int head = 0;
    int rear = 0;
    int queue[PortNum];     // �������
    int visited[PortNum];   // ������ʱ��
    int i, j, k;
    RouteNode *node;

	printf("BFS: \n");
	
	//��ʼ���������� 
    for (i = 0; i < PortNum ; i++)
        visited[i] = 0;
        
    for (i = 0; i < PortNum ; i++)
    {
        if (!visited[i])
        {
            visited[i] = 1;
            printf("δ���ʻ�����%d \n",G->Ports[i].data); 
            queue[rear++] = i;  // �����
        }
        while (head != rear) 
        {
            j = queue[head++];  // ������
            node = G->Ports[j].frstroute;
            while (node != NULL)
            {
                k = node -> RouteEnd-1; //������һ�� 
                if (!visited[k])
                {
                    visited[k] = 1;
                    printf("ID:%d, %d -> %d\n",node->info.FlgtID,
						node->info.StrtPoint,node->info.EndPoint);
                    queue[rear++] = k; //����� 
                }
                node = node->nextroute;//������� 
            }
        }
    }
    printf("\n");
}

//�˵� 
void menu()
{
	printf("\n1:��ѯ��������������̷���ʱ�䡣\n");
	printf("2:��ѯ�����������ĺ��ߣ�ֱ�ɡ�һ����ת��������ת����\n");
	printf("3:�������ʱ��/����ʱ��/����Ҫ�󣬲�ѯ�������������ĺ��߼�������͵ĺ��ߡ�\n");
	printf("4:���ݷ���ʱ��/��תʱ�䣬��ѯ�������������ĺ��ߡ�\n");
	printf("5:������ȱ�����ӡ�����ڵ㡣\n");
	printf("6:�˳�ϵͳ��\n");
	printf("�������������������Ӧ�����֣�\n");
	
	return; 
}

int main()
{
	int choice;
	int k=1; //k����whileѭ�� 
	
	printf("��ӭʹ�ú����ѯϵͳ����ϵͳ���ݻ���'flightdata.csv'.");
	printf("����%d�����ݡ�\n",LoadFlightData("F://flightdata2.csv", &_Gf, &_Gb, &_Rm));
	
	printf("\n˵��: \n");
	printf("��ϵͳ����24Сʱ�ơ�����������%d��������Ŵ�1-%d��\n",PortNum,PortNum);
	printf("���ͷ�Χ��321,350,429,755��\n");
	
	while(k)
	{
		menu();
		
		scanf("%d",&choice);
		
		switch(choice)
		{
			case 1:Choice1();
				break;
			case 2:Choice2();
				break;
			case 3:Choice3();
				break;
			case 4:Choice4();
				break;
			case 5:BFSGraph(&_Gf);
				break;
			case 6:k=0;
				break;
			default:printf("�������ֵ��Ч�����������롣\n");
				break; 
		}
	}
	
	return 0;
 } 
