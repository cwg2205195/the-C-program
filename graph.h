#include<stdio.h>
#include<stdlib.h>
#define MAX 100
#define INF 32767
//邻接表的数据结构 
typedef int InfoType ;
typedef struct ANode
{
	int adjvex;		//该边终点的编号
	struct ANode *nextarc;		//指向下一条边指针
	InfoType info;		//该边的相关信息 
}ArcNode;
typedef int Vertex;
typedef struct Vnode
{
	Vertex data ;//Vertex data;		//顶点信息
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
