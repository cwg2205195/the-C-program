#include<stdio.h>
#include<stdlib.h>
#define INF 32767
#define MAX 100
//�ڽӱ�����ݽṹ 
typedef char InfoType ;
typedef struct ANode
{
	int adjvex;		//�ñ��յ�ı��
	struct ANode *nextarc;		//ָ����һ����ָ��
	InfoType info;		//�ñߵ������Ϣ 
}ArcNode;
typedef struct Vnode
{
	int data ;//Vertex data;		//������Ϣ
	ArcNode *firstarc;		//ָ���һ���� 
}VNode;
typedef VNode AdjList[MAX] ;
typedef struct
{
	AdjList adjlist;//�ڽӱ� 
	int n,e;	//ͼ�ж�����n �� ����e 
}ALGraph;


//�ڽӾ�������ݽṹ
typedef struct
{
	int no;		//������ 
	InfoType info;//����������Ϣ 
} VertexType;//�������� 
typedef struct
{
	int edges[MAX][MAX];//�ڽӾ���ı�����;
	int n,e;//������������ 
	VertexType vexs[MAX];//��Ŷ�����Ϣ 
}MGraph;//������ͼ�ڽӾ������� 
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
	visited[v]=1;		//��־�ѷ���
	printf("%d ",v);		//����ѱ����ʵĶ���ı�� 
	p=G->adjlist[v].firstarc;		//pָ�򶥵�v�ĵ�һ���ڽӵ� 
	while(p!=NULL)
	{
		if(visited[p->adjvex]==0)
			DFS(G,p->adjvex);
		p=p->nextarc;
	}
}
void MatToList(MGraph g,ALGraph *&G)		//���ڽӾ���gת��Ϊ�ڽӱ�G  ����ͼ�� 
{
	int i,j;
	ArcNode *p;
	G=(ALGraph*)malloc(sizeof(ALGraph));
	for(i=0;i<g.n;i++)		//���ڽӱ�������ͷ�ڵ��ָ�����ó�ֵ
		G->adjlist[i].firstarc=NULL; 
	for(i=0;i<g.n;i++)
		for(j=g.n-1;j>=0;j--)		//����ڽӾ����е�ÿһ��Ԫ�� 
		if(g.edges[i][j]!=0) 		//�ڽӾ���ĵ�ǰԪ�ز�Ϊ0  
		{
			p=(ArcNode*)malloc(sizeof(ArcNode));//����һ���ڵ�*p 
			p->adjvex=j;
			p->nextarc=G->adjlist[i].firstarc;		//����ͷ�巨����*p 
			G->adjlist[i].firstarc=p;
		}
	G->n=g.n;G->e=g.e; 
}
void ListToMat(ALGraph *G,MGraph &g) //���ڽӱ�Gת��Ϊ�ڽӾ���g 
{
	int i,j;
	ArcNode *p;						//��ʼ���ڽӾ���g.edges[i][j]
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
//----------------------------------��Ȩͼ���㷨
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
	scanf("%d",&x); 			//��ѭ������  
	for(i=0;i<x;i++)
	{
		graph[i]=(MGraph*)malloc(sizeof(MGraph));		//��Ҫ�������ɼ��� 
		scanf("%d%d%d%d",&inp[i].m,&inp[i].n,&inp[i].o,&inp[i].p);		//mΪ������nΪ������oΪ�Ƿ����� pΪ�Ƿ��Ȩ 
		for(j=0;j<inp[i].m;j++)		//�Ȱ��ڽӾ������� 
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
		if(inp[i].p==0)//����Ȩ���ڽӾ��� 
			{
				MatToList(*graph[i],algraph[i]);
				DFS(algraph[i],0);printf("\n");
				BFS(algraph[i],0);//printf("\n");
				for(j=0;j<MAX;j++)
				visited[j]=0;
			//	DispAdj(algraph[i]);
			//	DispMat(graph[i]);printf("\n");
			}
		else if(inp[i].p==1)		//��Ȩ���ڽӾ���
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
