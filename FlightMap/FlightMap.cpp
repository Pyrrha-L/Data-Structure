/*
**
项目名称：航空公司航线图 
作者：李梓童
时间：2018.12. 
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

//函数返回状态 
typedef int Status;
//机场数量 
typedef int StPortNum;

//路线信息结构体 
typedef struct RouteInfo
{
	int DprtTime; //出发时间 
	int FlgtLength; //飞行时间 
	int ArivTime;
	int Cost; //航班费用 
	int StrtPoint; //出发点 
	int EndPoint; //目的地 
	int PlaneType; //飞机型号 
	int FlgtID; //航班ID 
}RouteInfo;

//机场结点结构体 
typedef struct RouteNode{
	int RouteEnd; //航班终点 
	RouteNode *nextroute; //下一个航班 
	RouteInfo info; //航班信息 
}RouteNode;
 
//出发机场结点 
typedef struct StPortNode{
	StPortNum data; //出发机场的编号 
	RouteNode *frstroute; //第一个航班 
}StPortNode,StPortList[PortNum];

//邻接表结构体 
typedef struct{
	StPortList Ports;
	int _RouteNum; //航班数 
	int _PortNum; //机场数 
}PortGraph;

PortGraph _Gf;//系统中的邻接表 
PortGraph _Gb;//系统中的逆邻接表 

//用于邻接矩阵边的存储 
typedef struct ArcCell { //弧的定义
	int rlt; // 顶点关系类型,1或0示相邻否
	RouteInfo info; // 该弧相关信息的指针
} ArcCell, RouteMatrix[PortNum][PortNum];

//邻接矩阵结构体 
typedef struct { // 图的定义
	int Ports2[PortNum]; // 顶点向量
	RouteMatrix RouteMap; // 邻接矩阵
	int _PortNum2, _RouteNum2; // 机场数和路线数 
} RMap;

RMap _Rm;//系统中的邻接矩阵 

//定位元素在邻接表中的位置。G：邻接表；v：待定位元素的值。 
int LocatePort(PortGraph *G, StPortNum v)
{
	int i;
	for (i = 0; i < (G->_PortNum); i++)
	{
		if (G->Ports[i].data == v)
			return i;
	}
}

//时间转换：00:00->24小时制的分钟计数。
//time：格式为“4:00”的待转化字符串。 
int TimeTransform(char *time)
{
	int i=0,ans=0,k=0; 
	//i为循环变量，ans用于存储最终结果，k用于buff计数。 
	int length=strlen(time);
	char buff[5];
	
	//读取小时 
	for(;i<length;i++)
	{
		if((time[i]<'0'||time[i]>'9')&&time[i]!=':'){
			return -1;
		}
		
		//把有效字符载入buffer 
		if(time[i]!=':'){
			buff[k]=time[i];
			k++;
		}
		else{
			buff[k]=0;//添加休止符 
			i++;
			break;
		}
	}
	
	//格式转换 
	sscanf(buff,"%d",&ans);
	
	if(ans>23||ans<0){
		return -1; 
	}
	
	ans*=60; //完成小时的转化
	k=0; 
	
	//读取分钟 
	for(;time[i]!=0;i++)
	{
		if(time[i]<'0'||time[i]>'9'){
			return -1;
		}
		//把有效字符载入buffer 
		buff[k]=time[i];
		k++;
	}
	
	//这里k已经没有用了，暂时拿来存一下分钟的数值。 
	sscanf(buff,"%d",&k); 
	if(k>59||k<0){
//		printf("时间有误！");
		return -1;
	}
	
	ans+=k;
	
	return ans;
}

//对数据文件能否正常打开的判断。
//filename：文件名 
FILE * loadDataSrcFile(char *filename) {
	FILE *fp;
	if ((fp = fopen(filename, "r")) == NULL) {
		printf("无法打开文件！\n");
		exit(0);//退出程序
	}
	return fp; 
}

//从每一行读取一条航线信息，存入Dest指向的内存中 
//fp：待读取文件的指针；Dest：目的结构体指针 
int rowscan(FILE *fp, RouteInfo *Dest)
{
	memset(Dest,0,sizeof(RouteInfo));
	char fields[7][10]; //每一块数据的内容 
	char buff[30] = {0}, c; //缓存字符数组
	
	int field_count = 0 ,len; 		
	for(int i = 0; !feof(fp) && i<FieldNum; i++)
	{	
		//固定顺序的7个字段		
		int idx = 0; //重置缓存空间
		while((c = fgetc(fp)))
		{	//扫描读取一个字段， EOF表示文件已结束 							
			if(c == ',' || c == '\n')
			{	//字段数据结束
				buff[idx] = 0;
				break;
			}
			else if(c == EOF)
			{
				break;
			 } 
			else
			{   //把每个字段的数据放入缓存
				buff[idx++] = c;
			}
		}
		
		len = strlen(buff);		
		char buff2[30]={0};
		
		//实现字符串与具体内容的转化 
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
	
	//计算到达时间 
	Dest->ArivTime=Dest->DprtTime+Dest->FlgtLength;
	
	return field_count;
}

//读取数据，同时创建邻接表。
//filename:文件名。Gf：正向邻接表。Gb：反向邻接表。M：邻接矩阵。 
int LoadFlightData(char *filename, PortGraph *Gf, PortGraph *Gb, RMap *M)
{
	//判断文件能否打开。 
	FILE *fp = loadDataSrcFile(filename);
	
	char fields[7][10];
	char buff[30] = {0}, c; //缓存字符数组
	int row_count=0;
	int i,j;
	
	//读第一行的字段名
	for(i=0; i<FieldNum-1; i++)
	{
		fscanf(fp, "%[^,],", fields[i]); 
		//%[^,]表示一直读取到逗号,为止
	}
	fscanf(fp, "%s", fields[9]);	
	fgetc(fp); //跳过第一行的回车符
	
	//对邻接表和邻接矩阵进行基本信息的设置 
	Gf->_PortNum=PortNum;
	Gf->_RouteNum=RouteNum; 
	Gb->_PortNum=PortNum;
	Gb->_RouteNum=RouteNum; 
	M->_PortNum2=PortNum;
	M->_RouteNum2=RouteNum;
	
	//邻接表初赋值 
	for (i=1;i<=PortNum; i++)
	{
		Gf->Ports[i-1].data=i;
		Gf->Ports[i-1].frstroute=NULL;
		M->Ports2[i-1]=i;
		Gb->Ports[i-1].data=i;
		Gb->Ports[i-1].frstroute=NULL;	
	}
	
	//邻接矩阵初赋值 
	for(i=0;i<PortNum;i++)
	{
		for(j=0;j<PortNum;j++)
			M->RouteMap[i][j].rlt=0;
	}
	
	//分配存储空间 
	RouteInfo *p;
	p=(RouteInfo *)malloc(sizeof(RouteInfo));
	
	//用于两个邻接表的创建 
	RouteNode *r1, *r2;
	
	while(rowscan(fp,p))
	{//扫描文件中的每行数据
		
		//排除文件末尾的干扰行
		if(p->EndPoint==0){
			break;
		} 
		
		int i, j, k;
		StPortNum v1, v2;
		
		//锁定元素位置 
		i = LocatePort(Gf, p->StrtPoint);
		j = LocatePort(Gf, p->EndPoint);
		
		//为新增元素分配存储空间 
		r1=(RouteNode *)malloc(sizeof(RouteNode));
		r2=(RouteNode *)malloc(sizeof(RouteNode));
		
		//邻接表的衔接 
		r1->info = *p;
		r1->RouteEnd = p->EndPoint ;
		r1->nextroute = Gf->Ports[i].frstroute;
		Gf->Ports[i].frstroute = r1;
		
		//逆邻接表衔接 
		r2->info = *p;
		r2->RouteEnd = p->StrtPoint ;
		r2->nextroute = Gb->Ports[j].frstroute;
		Gb->Ports[j].frstroute = r2;
		
		//邻接矩阵设置
		
		M->RouteMap[i][j].rlt=1; //仅仅表示可以中转 
		M->RouteMap[i][j].info = *p; 
		//把新的航班信息放入RouteMap中。 
		
		row_count++;
	}
	
	fclose(fp);
	
	return row_count;
}

int lsttime=4000; //为最短飞行时间设置上界 
int tmpdprttime=0; //暂存出发时间 

//深度优先求解最短路线
//p2：目的地。p：遍历指针 
void DFS_lsttime(int p2,RouteNode *p){
	
	//搜索到终点 
	if(p->info.EndPoint==p2){
		if(p->info.ArivTime-tmpdprttime<lsttime) {			
			lsttime=p->info.ArivTime-tmpdprttime;
		}
		return;
	}
	
	//到叶子节点的终止条件 
	if(p==NULL){
		return;
	}
	
	RouteNode *tmp;
	tmp=_Gf.Ports[p->info.EndPoint-1].frstroute;
	while(tmp)
	{	
		//可以中转：递归过滤条件 
		if(tmp->info.DprtTime>=p->info.ArivTime&&
		tmp->RouteEnd!=p->info.StrtPoint)//保证飞机不回飞 
		{
			
			DFS_lsttime(p2,tmp);
		}
		tmp=tmp->nextroute;
	}
	
}

//查询任意两机场的最短飞行时间 PortGraph *G
void Choice1()
{
	int port1,port2;
	printf("系统机场编号为 1 - %d \n", PortNum );
	printf("请输入您想查询的机场编号（中间空格隔开，如“2 3”）: \n");
	
	scanf("%d %d",&port1,&port2);
	if(port1<1||port1>PortNum||port2<1||port2>PortNum)
	{
		printf("您输入的机场编号有误！\n");
		return; 
	}
	else{
		lsttime=4000;//每次对lsttime重新赋值 
		RouteNode *p;//遍历指针 
		p=_Gf.Ports[port1-1].frstroute;
		while(p)
		{	
			tmpdprttime=p->info.DprtTime;
			DFS_lsttime(port2,p);
			p=p->nextroute;			
		}
		
		//如果lsttime没有改变，则说明没有相关的航班 
		if(lsttime!=4000){
			printf("从机场%d到机场%d的最短时间为：%d分钟\n",port1,port2,
			lsttime);
		} 
		else{
			printf("没有符合要求的航线。\n");
		}
		
		return;
	}
} 

//对第二项功能的实现。
//p1:出发机场。p2：目的机场。 
void solvechoice2(int p1, int p2)
{
	int i,j,k;
	int ans1,ans2,ans3;
	int tmptime,tmpend,tmptime2,tmpend2; 
	RouteNode *cursor;
	
	//直飞
	if(_Rm.RouteMap[p1-1][p2-1].rlt){
		i=0;
		while(i<_Rm.RouteMap[p1-1][p2-1].rlt)
		{
			printf("直飞航班ID：%d\n",_Rm.RouteMap[p1-1][p2-1].info.FlgtID);
			i++;
		}
	}
	
	//1次中转，矩阵内部一次循环。 
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
						printf("中转一次航班ID：%d %d \n",
						_Rm.RouteMap[p1-1][i].info.FlgtID,
						_Rm.RouteMap[tmpend][j].info.FlgtID); 
					}
			} 		
	}
	
	//2次中转，在矩阵内多加一次循环。 
	for(i=0;i<PortNum;i++)
	{
		//排除没有通路的情况。 
		if(i==p1-1||_Rm.RouteMap[p1-1][i].rlt==0) continue;
		
		tmptime=_Rm.RouteMap[p1-1][i].info.ArivTime;
		tmpend=_Rm.RouteMap[p1-1][i].info.EndPoint-1;
				
			for(j=0;j<PortNum;j++)
			{	
				//排除没有通路的情况并保证可中转性。 
				if((!_Rm.RouteMap[tmpend][j].rlt)||
				_Rm.RouteMap[tmpend][j].info.DprtTime<tmptime||j==tmpend) continue; 
				
				tmptime2=_Rm.RouteMap[tmpend][j].info.ArivTime;
				tmpend2=_Rm.RouteMap[tmpend][j].info.EndPoint-1;
				
				for(k=0;k<PortNum;k++)
				{
					//排除没有通路的情况，并保证可中转性。 
					if((!_Rm.RouteMap[tmpend2][k].rlt)||
					_Rm.RouteMap[tmpend2][k].info.DprtTime<tmptime2||k==tmpend2) continue;
							
					if(_Rm.RouteMap[tmpend2][k].info.EndPoint==p2&&
					_Rm.RouteMap[tmpend2][k].info.DprtTime>tmptime2)
					{
						printf("中转二次航班ID：%d %d %d\n",
						_Rm.RouteMap[p1-1][i].info.FlgtID,
						_Rm.RouteMap[tmpend][j].info.FlgtID,
						_Rm.RouteMap[tmpend2][k].info.FlgtID); 
					}
				}
				
			} 		
	}
	
	return;
}

//第二项功能的接口 
void Choice2()
{
	int p1,p2;
	printf("请输入你想查询航班的出发机场编号和目的机场编号，格式为“1 2”：\n");
	scanf("%d %d",&p1,&p2);
	
	printf("查询结果如下：\n"); 
	solvechoice2(p1,p2); 
} 
 
int result[ResSize][ResSize]={0};//存储查询结果 
int res_count=0;//查询结果的数量 
int step=0;//递归深度 

//初始化查询信息 
void InitRes()
{
	memset(result,0,sizeof(int)*ResSize*ResSize);
	res_count=0;
	step=0;
	return; 
} 

//计算有关费用。
//res：结果存储数组。sum:符合要求的结果个数。 
int CalFee(int *res,int sum)
{
	int i,j,k;//循环变量 
	int ans=0;
	
	for(i=0;i<PortNum;i++)
	{
		for(j=0;j<PortNum;j++)
		{
			for(k=1;k<=sum;k++)
			{	
				//根据查询结果中存储的航班ID，找出相应的费用并相加。		
				if(_Rm.RouteMap[i][j].info.FlgtID==res[k]){
					ans+=_Rm.RouteMap[i][j].info.Cost;
				}
			}
		}
	}
	return ans;
}

//求出最小的费用
//res：存出结果的二维数组。sum：查询结果的个数。 
int GetMinFee(int res[][ResSize],int sum)
{
	int tmp=10000;
	int i,ans=0;
	
	//求最小值。 
	for(i=0;i<sum;i++)
	{
		ans=(res[i][0]<tmp)?i:ans;
		tmp=(res[i][0]<tmp)?res[i][0]:tmp;
	}
	
	//打印相应的结果
	if(tmp==10000){
		printf("\n没有找到符合要求的航线。\n");
		return 1;
	}
	
	printf("\n最少花销为：%d\n",tmp);
	printf("对应的路线航班ID为："); 
	for(i=1;res[ans][i]!=0;i++)
	{
		printf("%d ",res[ans][i]);
	}
	printf("\n");
	
	return 0;
}

//深度优先遍历（正向邻接表）
//p2:目的地。p：遍历指针 
void DFS_f(int p2,RouteNode *p){
	//深度加一 
	step++;
	if(p->info.EndPoint==p2){
		result[res_count][step]=p->info.FlgtID;
	
		//输出结果
		printf("\n符合要求的航线为（航班ID-航班次序）："); 	
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
		printf("该航线的开销为：%d \n",result[res_count][0]);
		
		res_count++;
		
		//为分叉路线拷贝原有路线
		for(i=1;i<10;i++)
		{
			result[res_count][i]=result[res_count-1][i];
		} 
		return;
	}
	
	//终止递归条件 
	if(p==NULL){
		return;
	}
	
	//把当前航班ID存入结果数组 
	result[res_count][step]=p->info.FlgtID;
	
	RouteNode *tmp;
	tmp=_Gf.Ports[p->info.EndPoint-1].frstroute;
	while(tmp)
	{	
		if(tmp->info.DprtTime>=p->info.ArivTime&&
		tmp->RouteEnd!=p->info.StrtPoint)//保证飞机不回飞以及可以中转 
		{
			DFS_f(p2,tmp);
			step--;	
		}
		tmp=tmp->nextroute;
	}
	
	result[res_count][step]=0;
}

//逆邻接表的深度遍历
//p2:出发点。p：遍历指针 
void DFS_b(int p2,RouteNode *p){
	step++;
	
	if(p->info.StrtPoint==p2){
		result[res_count][step]=p->info.FlgtID;
	
		//输出结果
		printf("\n符合要求的航线为（航班ID-航班次序）："); 	
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
		printf("该航线的开销为：%d \n",result[res_count][0]);
		
		res_count++;
		
		//为分叉路线拷贝原有路线
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
	//这里变成了逆邻接表 
	tmp=_Gb.Ports[p->info.StrtPoint-1].frstroute;
	while(tmp)
	{	
		if(tmp->info.ArivTime<=p->info.DprtTime&&
		tmp->info.StrtPoint!=p->info.EndPoint)//保证飞机不回飞 
		{
			DFS_b(p2,tmp);
			step--; 
		}
		tmp=tmp->nextroute;
	}
	
	//恢复原值 
	result[res_count][step]=0;	
}

//针对限定机型的遍历
//p2:目的地。p：遍历指针。t：限定机型 
void DFS_t(int p2,RouteNode *p,int t){
	step++;
	
	if(p->info.EndPoint==p2){
		result[res_count][step]=p->info.FlgtID;
	
		//输出结果
		printf("\n符合要求的航线为（航班ID-航班次序）："); 	
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
		printf("该航线的开销为：%d \n",result[res_count][0]);
		
		res_count++;
		
		//为分叉路线拷贝原有路线
		for(i=1;i<10;i++)
		{
			result[res_count][i]=result[res_count-1][i];
		} 
		return;
	}
	
	if(p==NULL){
		return;
	}
	//记录当前航班ID 
	result[res_count][step]=p->info.FlgtID;
	
	RouteNode *tmp;
	tmp=_Gf.Ports[p->info.EndPoint-1].frstroute;
	while(tmp)
	{	
		if(tmp->info.PlaneType==t&&
		tmp->info.DprtTime>=p->info.ArivTime&&
		tmp->RouteEnd!=p->info.StrtPoint)//保证飞机不回飞且有机型限定 
		{
			DFS_t(p2,tmp,t); 
			step--;
		}
		tmp=tmp->nextroute;
	}
	
	//恢复航班ID 
	result[res_count][step]=0;
}

//第三项功能接口 
//s：出发机场。d：目的机场。t1：起始时间/机型。t2：终止时间。type：选择功能。 
void solveChoice3(int s, int d, int t1, int t2, int type)
{	
	InitRes();
	
	int i,j;
	
	//根据type的值进入不同的递归 
	if(type==1){
		RouteNode *p;
		//递归起点 
		p=_Gf.Ports[s-1].frstroute;
		while(p)
		{	
			//限定出发时间 
			if(p->info.DprtTime>=t1&&p->info.DprtTime<=t2)
			{	
				step=0;
				DFS_f(d,p); 
			}
			p=p->nextroute;
		}
		//计算最少费用 
		GetMinFee(result,res_count);
	}	
	else if(type==2)
	{
		RouteNode *p;
		p=_Gb.Ports[d-1].frstroute;
		while(p)
		{	
		//	限定到达时间 
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
			//限定机型 
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

//第三项功能的接口
//对输入值有效无效的判断 
void Choice3()
{
	int k;
	int p1,p2,type,timea2,timeb2;
	char timea[6],timeb[6];
	
	printf("限定起飞时段，请输入1；限定降落时段，请输入2；限定机型，请输入3。\n");	
	scanf("%d",&k);
	
	if(k==1)
	{
		printf("请按照'起始时间 终止时间'的格式输入起飞时段，如‘6:00 8:00’:\n"); 
		scanf("%s %s",&timea,&timeb);
		timea2=TimeTransform(timea);
		timeb2=TimeTransform(timeb);
		if(timea2<0||timeb2<0){
			printf("时间有误！！\n");
			return; 
		}
		
		printf("请按照'出发机场编号 目的机场编号'的格式输入机场编号，如‘1 3’：\n");
		scanf("%d %d",&p1,&p2);
		if(p1<1||p1>PortNum||p2<1||p2>PortNum){
			printf("输入有误！\n");
			return;
		}
		solveChoice3(p1, p2, timea2, timeb2, 1);
	}
	else if(k==2)
	{
		printf("请按照'起始时间 终止时间'的格式输入降落时段，如‘6:00 8:00’:\n"); 
		scanf("%s %s",&timea,&timeb);
		timea2=TimeTransform(timea);
		timeb2=TimeTransform(timeb);
		if(timea2<0||timeb2<0){
			printf("时间有误！\n");
			return; 
		}
		
		printf("请按照'出发机场编号 目的机场编号'的格式输入机场编号，如‘1 3’：\n");
		scanf("%d %d",&p1,&p2);
		if(p1<1||p1>PortNum||p2<1||p2>PortNum){
			printf("输入有误！\n");
			return;
		}
		solveChoice3(p1, p2, timea2, timeb2, 2);
	}
	else if(k==3)
	{
		printf("请输入限定的机型:\n"); 
		scanf("%d",&type);
		if(type!=321&&type!=350&&type!=429&&type!=747){
			printf("机型有误！\n");
			return; 
		}
		
		printf("请按照'出发机场编号 目的机场编号'的格式输入机场编号，如‘1 3’：\n");
		scanf("%d %d",&p1,&p2);
		if(p1<1||p1>PortNum||p2<1||p2>PortNum){
			printf("输入有误！\n");
			return;
		}
		solveChoice3(p1, p2, type, 0, 3);
	}
	else{
		printf("无效输入！\n");
	}
	return;
}

//功能四所需要的递归
//p2：目的地。p：遍历指针。t：时间。tmax：时间上界。k：选择功能 
void DFS_4(int p2, RouteNode *p,int t,int tmax,int k)
{
	step++;
	if(p->info.EndPoint==p2){
		result[res_count][step]=p->info.FlgtID;
	
		//输出结果
		printf("符合要求的航线为：\n"); 	
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
		
		//为分叉路线拷贝原有路线
		for(i=1;i<10;i++)
		{
			result[res_count][i]=result[res_count-1][i];
		} 
		return;
	}
	
	//终止条件 
	if(p==NULL){
		return;
	}
	
	result[res_count][step]=p->info.FlgtID;
	
	RouteNode *tmp;
	tmp=_Gf.Ports[p->info.EndPoint-1].frstroute;
	
	//对所使用飞行时间的更新 
	if(k==1&&t==0) t+=p->info.FlgtLength;

	while(tmp)
	{	
		//选择了飞行时间的功能，需保证当前已用飞行时间不超过上界 
		if(k==1&&tmp->info.FlgtLength+t<=tmax&&
		tmp->info.DprtTime>=p->info.ArivTime&&
		tmp->RouteEnd!=p->info.StrtPoint)//保证飞机不回飞 
		{
			DFS_4(p2,tmp,tmp->info.FlgtLength+t,tmax,1); 
			step--;
		}
		
		//选择了中转时间的功能，需保证当前中转时间不超过上界 
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

//第四项功能设置递归起点 
//s:出发点。d：目的地。t：时间上界。k：选择功能 
void SolveChoice4(int s,int d,int t,int k)
{
	RouteNode *p;
	p=_Gf.Ports[s-1].frstroute;
	while(p)
	{	
		step=0;
		DFS_4(d,p,0,t,k); //根据k不同进入相应递归 
		p=p->nextroute;
	}
}

//第四项功能的接口
//输入合法的判断 
void Choice4()
{
	int k,key,p1,p2; 
	printf("限定飞行时长：1；限定中转时间：2。请输入：");
	scanf("%d",&k);
	
	if(k==1){
		
		printf("请输入飞行时长：");
		scanf("%d",&key);
		if(key<=0){
			printf("时间有误！");
			return;
		}
		
		printf("请输入起始机场与结束机场，格式‘1 2’：");
		scanf("%d %d",&p1,&p2);
		if(p1<1||p1>PortNum||p2<1||p2>PortNum){
			printf("输入有误！");
			return;
		}
		
		SolveChoice4(p1,p2,key,1);
	}
	else if(k==2){
		printf("请输入中转时间：");
		scanf("%d",&key);
		if(key<=0){
			printf("时间有误！");
			return;
		}
		
		printf("请输入起始机场与结束机场，格式‘1 2’：");
		scanf("%d %d",&p1,&p2);
		if(p1<1||p1>PortNum||p2<1||p2>PortNum){
			printf("输入有误！");
			return;
		}
		
		SolveChoice4(p1,p2,key,2);
	}
	else{
		printf("无效输入！");
		return; 
	}
} 

//广度优先遍历
//G：邻接表 
void BFSGraph(PortGraph *G)
{
    int head = 0;
    int rear = 0;
    int queue[PortNum];     // 辅组队列
    int visited[PortNum];   // 顶点访问标记
    int i, j, k;
    RouteNode *node;

	printf("BFS: \n");
	
	//初始化访问数组 
    for (i = 0; i < PortNum ; i++)
        visited[i] = 0;
        
    for (i = 0; i < PortNum ; i++)
    {
        if (!visited[i])
        {
            visited[i] = 1;
            printf("未访问机场：%d \n",G->Ports[i].data); 
            queue[rear++] = i;  // 入队列
        }
        while (head != rear) 
        {
            j = queue[head++];  // 出队列
            node = G->Ports[j].frstroute;
            while (node != NULL)
            {
                k = node -> RouteEnd-1; //进入下一层 
                if (!visited[k])
                {
                    visited[k] = 1;
                    printf("ID:%d, %d -> %d\n",node->info.FlgtID,
						node->info.StrtPoint,node->info.EndPoint);
                    queue[rear++] = k; //入队列 
                }
                node = node->nextroute;//横向访问 
            }
        }
    }
    printf("\n");
}

//菜单 
void menu()
{
	printf("\n1:查询任意两机场的最短飞行时间。\n");
	printf("2:查询任意两机场的航线（直飞、一次中转、两次中转）。\n");
	printf("3:根据起飞时段/降落时段/机型要求，查询任意两个机场的航线及航费最低的航线。\n");
	printf("4:根据飞行时长/中转时间，查询任意两个机场的航线。\n");
	printf("5:广度优先遍历打印机场节点。\n");
	printf("6:退出系统。\n");
	printf("请根据您的需求输入相应的数字：\n");
	
	return; 
}

int main()
{
	int choice;
	int k=1; //k控制while循环 
	
	printf("欢迎使用航班查询系统，本系统数据基于'flightdata.csv'.");
	printf("共有%d项数据。\n",LoadFlightData("F://flightdata2.csv", &_Gf, &_Gb, &_Rm));
	
	printf("\n说明: \n");
	printf("本系统采用24小时制。机场总数：%d，机场编号从1-%d。\n",PortNum,PortNum);
	printf("机型范围：321,350,429,755。\n");
	
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
			default:printf("您输入的值无效，请重新输入。\n");
				break; 
		}
	}
	
	return 0;
 } 
