#include<stdio.h>
#include<stdlib.h>
#define INF 32767
#define MAX 100
//邻接表的数据结构 
typedef char InfoType ;
typedef struct ANode
{
	int adjvex;		//该边终点的编号
	struct ANode *nextarc;		//指向下一条边指针
	InfoType info;		//该边的相关信息 
}ArcNode;
typedef struct Vnode
{
	int data ;//Vertex data;		//顶点信息
	ArcNode *firstarc;		//指向第一条边 
}VNode;
typedef VNode AdjList[MAX] ;
typedef struct
{
	AdjList adjlist;//邻接表 
	int n,e;	//图中顶点数n 和 边数e 
}ALGraph;


//邻接矩阵的数据结构
typedef struct
{
	int no;		//顶点编号 
	InfoType info;//顶点其他信息 
} VertexType;//顶点类型 
typedef struct
{
	int edges[MAX][MAX];//邻接矩阵的变数组;
	int n,e;//顶点数，边数 
	VertexType vexs[MAX];//存放顶点信息 
}MGraph;//完整的图邻接矩阵类型 
/*
3
5 5 0 0
0 1 0 1 1
1 0 1 1 0
0 1 0 1 1
1 1 1 0 1
1 0 1 1 0
5 5 1 0
0 1 0 1 0
0 0 1 1 0
0 0 0 1 1
0 0 0 0 0
1 0 0 1 0
6 6 1 1
0 5 32767 7 32767 32767
32767 0 4 32767 32767 32767
8 32767 0 32767 32767 9
32767 32767 5 0 32767 6
32767 32767 32767 5 0 32767
3 32767 32767 32767 1 0*/
int visited[MAX];
void DFS(ALGraph *G,int v) 
{
	ArcNode *p;
	visited[v]=1;		//标志已访问
	printf("%d ",v);		//输出已被访问的顶点的编号 
	p=G->adjlist[v].firstarc;		//p指向顶点v的第一个邻接点 
	while(p!=NULL)
	{
		if(visited[p->adjvex]==0)
			DFS(G,p->adjvex);
		p=p->nextarc;
	}
}
void MatToList(MGraph g,ALGraph *&G)		//将邻接矩阵g转换为邻接表G  无向图？ 
{
	int i,j;
	ArcNode *p;
	G=(ALGraph*)malloc(sizeof(ALGraph));
	for(i=0;i<g.n;i++)		//给邻接表中所有头节点的指针域置初值
		G->adjlist[i].firstarc=NULL; 
	for(i=0;i<g.n;i++)
		for(j=g.n-1;j>=0;j--)		//检查邻接矩阵中的每一个元素 
		if(g.edges[i][j]!=0) 		//邻接矩阵的当前元素不为0  
		{
			p=(ArcNode*)malloc(sizeof(ArcNode));//创建一个节点*p 
			p->adjvex=j;
			p->nextarc=G->adjlist[i].firstarc;		//采用头插法插入*p 
			G->adjlist[i].firstarc=p;
		}
	G->n=g.n;G->e=g.e; 
}
void ListToMat(ALGraph *G,MGraph &g) //将邻接表G转化为邻接矩阵g 
{
	int i,j;
	ArcNode *p;						//初始化邻接矩阵g.edges[i][j]
	for(i=0;i<G->n;i++)
		for(j=0;j<G->n;j++)
			g.edges[i][j]=0;
	for(i=0;i<G->n;i++)
		{
			p=G->adjlist[i].firstarc;
			while(p!=NULL)
			{
				g.edges[i][p->adjvex]=1;
				p=p->nextarc;
			}
		}
			g.n=G->n;g.e=G->e; 
}
void DispAdj(ALGraph *G)
{
	int i;
	ArcNode *p;
	for(i=0;i<G->n;i++)
	{
		p=G->adjlist[i].firstarc;
		printf("%3d:",i);
		while(p!=NULL)
		{
			printf("%3d",p->adjvex);
			p=p->nextarc;
		}
		printf("\n");
	}
}
void DispMat(MGraph *g)
{
	int j,i;
	for(i=0;i<g->n;i++ )
	{
		printf("%d:",i);
		for(j=0;j<g->n;j++)
			printf("%d ",g->edges[i][j]);
		printf("\n");
	}
}
//----------------------------------带权图的算法
void MatToList1(MGraph g,ALGraph *&G) 
{
	int i,j;
	ArcNode *p;
	G=(ALGraph*)malloc(sizeof(ALGraph));
	for(i=0;i<g.n;i++)
		G->adjlist[i].firstarc=NULL;
	for(i=0;i<g.n;i++)
		for(j=g.n-1;j>=0;j--)
		if(g.edges[i][j]!=0&&g.edges[i][j]!=INF)
		{
			p=(ArcNode*)malloc(sizeof(ArcNode));
			p->adjvex=j;
			p->info=g.edges[i][j];
			p->nextarc=G->adjlist[i].firstarc;
			G->adjlist[i].firstarc=p;
		}
	G->n=g.n;G->e=g.e;
}
void ListToMat1(ALGraph *G,MGraph &g)
{
	int i,j;
	ArcNode *p;
	for(i=0;i<G->n;i++)
		for(j=0;j<G->n;j++)
			if(i==j)
				g.edges[i][j]=0;
			else
				g.edges[i][j]=INF;
	for(i=0;i<G->n;i++)
	{
		p=G->adjlist[i].firstarc;
		while(p!=NULL)
		{
			g.edges[i][p->adjvex]=p->info;
			p=p->nextarc;
		}
	}
	g.n=G->n;g.e=G->e;
}
void BFS(ALGraph *G,int v)
{
	ArcNode *p;
	int queue[MAX],front=0,rear=0;
	int visited[MAX];
	int w,i;
	for(i=0;i<G->n;i++)	visited[i]=0;
	printf("%d ",v);
	visited[v]=1;
	rear=(rear+1)%MAX;
	queue[rear]=v;
	while(front!=rear)
	{
		front=(front+1)%MAX;
		w=queue[front];
		p=G->adjlist[w].firstarc;
		while(p!=NULL)
		{
			if(visited[p->adjvex]==0)
			{
				printf("%d ",p->adjvex);
				visited[p->adjvex]=1;
				rear=(rear+1)%MAX;
				queue[rear]=p->adjvex;
			}
			p=p->nextarc;
		}
	}
	printf("\n");
}
typedef struct input {int m,n,o,p;}INPUT; 
int main()
{
	INPUT inp[MAX];
	int x,i,j,k;
	MGraph *graph[MAX]={NULL};
	ALGraph *algraph[MAX]={NULL};
	scanf("%d",&x); 			//总循环次数  
	for(i=0;i<x;i++)
	{
		graph[i]=(MGraph*)malloc(sizeof(MGraph));		//需要几个生成几个 
		scanf("%d%d%d%d",&inp[i].m,&inp[i].n,&inp[i].o,&inp[i].p);		//m为行数，n为列数，o为是否有向 p为是否加权 
		for(j=0;j<inp[i].m;j++)		//先把邻接矩阵生成 
		{
			for(k=0;k<inp[i].n;k++)
			scanf("%d",&graph[i]->edges[j][k]);
		}
		/*if(inp[i].o==0) 
		graph[i]->n=inp[i].m*inp[i].n;
		else*/
		graph[i]->n=inp[i].n;
		graph[i]->e=0;
	}
	for(i=0;i<x;i++)
	{
		algraph[i]=(ALGraph*)malloc(sizeof(ALGraph));
		if(inp[i].p==0)//不带权的邻接矩阵 
			{
				MatToList(*graph[i],algraph[i]);
				DFS(algraph[i],0);printf("\n");
				BFS(algraph[i],0);//printf("\n");
				for(j=0;j<MAX;j++)
				visited[j]=0;
			//	DispAdj(algraph[i]);
			//	DispMat(graph[i]);printf("\n");
			}
		else if(inp[i].p==1)		//带权的邻接矩阵
			{
				MatToList1(*graph[i],algraph[i]);
				DFS(algraph[i],0);printf("\n");
				BFS(algraph[i],0);//printf("\n");
				for(j=0;j<MAX;j++)
				visited[j]=0;
			//	DispAdj(algraph[i]);
			//	DispMat(graph[i]);printf("\n");
			} 
	}
	return 0;
}
