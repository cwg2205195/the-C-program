#include<stdio.h>
#include<stdlib.h>
#define MAX 100
typedef int InfoType; 
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
void Prim(MGraph g,int v)
{
	int lowcost[MAX],min,i,j,k;
	int closest[MAX],n=g.n;
	for(i=0;i<n;i++)		//��ʼ��lowcost ��	closest 
	{
		lowcost[i]=g.edges[v][i];
		closest[i]=v;
	}
	for(i=1;i<n;i++)			//�ҳ�n-1 ������ 
	{
		min=MAX;
		for(j=0;j<n;j++)			//�ڣ�V-U�� ���ҳ���U����Ķ��� 
			if(lowcost[j]!=0&&lowcost[j]<min)
			{
				min=lowcost[j];
				k=j;
			}
		printf("%d ",min);
		lowcost[k]=0;		//���k�Ѽ���U 
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
	scanf("%d%d",&n,&m);	//������ٸ���+ ��������
	for(i=0;i<n;i++) 		//�����ڽӾ��� 
		for(j=0;j<n;j++)
			scanf("%d",&graph.edges[i][j]);
	graph.n=n;graph.e=m;
	Prim(graph,0);
	return 0;
}
