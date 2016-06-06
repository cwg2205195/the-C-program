#include<stdio.h>
#include<stdlib.h>
#define MAX 100
typedef int InfoType; 
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
void Prim(MGraph g,int v)
{
	int lowcost[MAX],min,i,j,k;
	int closest[MAX],n=g.n;
	for(i=0;i<n;i++)		//初始化lowcost 和	closest 
	{
		lowcost[i]=g.edges[v][i];
		closest[i]=v;
	}
	for(i=1;i<n;i++)			//找出n-1 个顶点 
	{
		min=MAX;
		for(j=0;j<n;j++)			//在（V-U） 中找出离U最近的顶点 
			if(lowcost[j]!=0&&lowcost[j]<min)
			{
				min=lowcost[j];
				k=j;
			}
		printf("%d ",min);
		lowcost[k]=0;		//标记k已加入U 
		for(j=0;j<n;j++) 
			if(g.edges[k][j]!=0&&g.edges[k][j]<lowcost[j])
			{
				lowcost[j]=g.edges[k][j];
				closest[j]=k;
			}
	}
}
int main()
{
	MGraph graph;
	int i,j,n,m;
	scanf("%d%d",&n,&m);	//输入多少个点+ 多少条边
	for(i=0;i<n;i++) 		//生成邻接矩阵 
		for(j=0;j<n;j++)
			scanf("%d",&graph.edges[i][j]);
	graph.n=n;graph.e=m;
	Prim(graph,0);
	return 0;
}
