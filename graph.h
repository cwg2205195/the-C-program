#include<stdio.h>
#include<stdlib.h>
#define MAX 100
#define INF 32767
//�ڽӱ�����ݽṹ 
typedef int InfoType ;
typedef struct ANode
{
	int adjvex;		//�ñ��յ�ı��
	struct ANode *nextarc;		//ָ����һ����ָ��
	InfoType info;		//�ñߵ������Ϣ 
}ArcNode;
typedef int Vertex;
typedef struct Vnode
{
	Vertex data ;//Vertex data;		//������Ϣ
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
