#include "stdio.h"
#include "stdlib.h"


//顶点
typedef struct VNode {
    int index;
    struct edge* pEdge;
} VNode;

//边
typedef struct edge {
    VNode* from;
    VNode* to;
    int weight;
    struct edge* next;
} UEdge;

//邻接表表头
typedef struct graph {
    VNode* graph;
    int capacity;
} graphStruct;
graphStruct graph;

//广度遍历用Queue,写成链表形式(
typedef struct QNode {
    VNode* vNode;
    struct QNode* next;
} QNode;
QNode head;

//创建队列节点
QNode* createQNode(VNode* vNode) {
    QNode* qNode = malloc(sizeof(QNode));
    qNode->vNode = vNode;
    qNode->next = NULL;
    return qNode;
}

//找队尾
QNode* findLast() {
    QNode* qNode = &head;
    while (qNode->next != NULL) {
        qNode = qNode->next;
    }
    return qNode;
}

//判断队是否为空
int empty() {
    if (head.next == NULL) {
        return 0;
    }
    return 1;
}

//初始化
int initQueue() {
    head.vNode = NULL;
    head.next = NULL;
    return 1;
}

//入队
int enQueue(VNode* vNode) {
    QNode* qNode = findLast();
    qNode->next = createQNode(vNode);
    return 1;
}

//出队
int deQueue() {
    QNode* qNode = head.next;
    if (head.next->next == NULL) {
        head.next = NULL;
        free(qNode);
        return 1;
    }
    head.next = head.next->next;
    free(qNode);
    return 1;
}

//最短路径算法用节点，visit表示访问状态，1为访问过，0为未访问
typedef struct DNode {
    int visit;
    int distance;
    VNode* preVNode;
} DNode;

//创建邻接表本体，需指定大小(节点数)
graphStruct createGraph(int capacity) {
    graph.graph = malloc(capacity * sizeof(VNode));
    int i = 0;
    //所有VNode的索引都初始化成-1来进行标记
    while (i != capacity) {
        graph.graph[i].index = -1;
        graph.graph[i].pEdge = NULL;
        i++;
    }
    //经典顺序表
    graph.capacity = capacity;
    return graph;
}


//向邻接表中添加未添加过的节点,并赋索引，初始化为-1
int addNodeToAdjacencyList(VNode* vNode) {
    int i = 0;
    while (i < graph.capacity) {
        if (graph.graph[i].index == -1) {
            vNode->index = i;
            graph.graph[i] = *vNode;
            return i;
        }
        i++;
    }
    //放入邻接表失败返回负值
    return -1;
}

//检查邻接表中是否含有节点,如果有返回索引值
int checkInList(VNode* vNode) {
    int i = 0;
    while (i != graph.capacity - 1) {
        if (graph.graph[i].index == vNode->index) {
            return vNode->index;
        }
        i++;
    }
    return -1;
}

//创建并初始化节点，索引初值为-1，之后添加进邻接表
VNode* createNode() {
    VNode* vNode = malloc(sizeof(VNode));
    vNode->index = -1;
    vNode->pEdge = NULL;
    addNodeToAdjacencyList(vNode);
    return vNode;
}

//根据edge的from节点创建并初始化边
UEdge* createEdge(int fromIndex, int toIndex, int weight) {
    UEdge* uEdge = malloc(sizeof(UEdge));
    uEdge->from = &graph.graph[fromIndex];
    uEdge->to = &graph.graph[toIndex];
    uEdge->weight = weight;
    uEdge->next = NULL;
    return uEdge;
}


UEdge* findLastGNode(VNode* vNode) {
    if (checkInList(vNode) < 0) {
        addNodeToAdjacencyList(vNode);
    }
    UEdge* uEdge = vNode->pEdge;
    while (uEdge->next != NULL) {
        uEdge = uEdge->next;
    }
    return uEdge;
}

//dir用于表示边的有向无向，0表示有向，1表示无向(双向)
//通过递归调用实现无向图(双向边)
int addEdgeToAdjacencyList(int fromIndex, int toIndex, int weight, int dir) {
    VNode* from = &graph.graph[fromIndex];
    VNode* to = &graph.graph[toIndex];
    if (checkInList(to) < 0) {
        addNodeToAdjacencyList(to);
    }
    //新建一条edge并放入from节点存储edge的表的末尾
    //判断from是否已有UEdge节点,如果有就直接加入
    if (from->pEdge == NULL) {
        from->pEdge = createEdge(from->index, to->index, weight);
    }
    else {
        findLastGNode(from)->next = createEdge(from->index, to->index, weight);
    }
    //实现无向图双向边
    if (dir) {
        addEdgeToAdjacencyList(toIndex, fromIndex, weight, 0);
    }
    return 1;
}

//广度遍历
int breathFirstSearch(int* arr, VNode* vNode) {
    printf("%d >> ", vNode->index);
    UEdge* uEdge = vNode->pEdge;
    while (uEdge != NULL) {
        if (arr[uEdge->to->index] == 0) {
            enQueue(uEdge->to);
            arr[uEdge->to->index] = 1;
        }
        uEdge = uEdge->next;
    }
    deQueue();
    if (!empty()) {
        return 1;
    }
    breathFirstSearch(arr, head.next->vNode);
    return 1;
}

//深度遍历
int depthFirstSearch(int* arr, VNode* vNode) {
    arr[vNode->index] = 1;
    printf("%d >> ", vNode->index);
    UEdge* uEdge = vNode->pEdge;
    while (uEdge != NULL) {
        if (!arr[uEdge->to->index]) {
            depthFirstSearch(arr, uEdge->to);
        }
        uEdge = uEdge->next;
    }
    return 1;
}

//两种遍历的中转函数，为广度遍历，1为深度遍历
int traversalGraph(int type) {
    //存储访问状态的数组
    //1为已经被访问过
    //0为未被访问
    int* arr = malloc(graph.capacity * sizeof(int));
    for (int i = 0; i < graph.capacity; ++i) {
        arr[i] = 0;
    }
    switch (type) {
    case 0:
        printf("广度优先遍历:\n");
        initQueue();
        enQueue(graph.graph);
        breathFirstSearch(arr, &graph.graph[0]);
        printf("\n\n");
        break;
    case 1:
        printf("深度优先遍历:\n");
        depthFirstSearch(arr, &graph.graph[0]);
        printf("\n\n");
        break;
    default: break;
    }
    return 1;

}

//基于深度优先算法的拓扑排序
int topologicalSort() {
    printf("拓扑排序基于\n");
    traversalGraph(1);
    return 1;
}

//Dijkstra算法，做一个广度遍历,并计算权
int dijkstra(DNode* dis, VNode* vNode) {
    UEdge* uEdge = vNode->pEdge;
    while (uEdge != NULL) {
        //记录暂时存储从此节点到后继的距离值,后续进行比较
        int TDis = dis[vNode->index].distance + uEdge->weight;
        //未访问过就将暂时值赋为初值,并记录当前节点为前驱
        if (dis[uEdge->to->index].visit == 0) {
            enQueue(uEdge->to);
            dis[uEdge->to->index].distance = TDis;
            dis[uEdge->to->index].preVNode = vNode;
            dis[uEdge->to->index].visit = 1;
        }
        else {
            //访问过则判断后继节点的距离值和暂时值进行比较,并选择小的负值并记录前驱节点
            if (TDis < dis[uEdge->to->index].distance) {
                dis[uEdge->to->index].distance = TDis;
                dis[uEdge->to->index].preVNode = vNode;
            }
        }
        uEdge = uEdge->next;
    }
    deQueue();
    if (!empty()) {
        return 1;
    }
    dijkstra(dis, head.next->vNode);

    return 1;
}

//Dijkstra算法前置
int preDijkstra() {
    printf("Dijkstra算法\n");
    DNode* dis = malloc(graph.capacity * sizeof(DNode));
    //赋初值
    for (int i = 0; i < graph.capacity; ++i) {
        dis[i].visit = 0;
        dis[i].preVNode = &graph.graph[i];
    }
    initQueue();
    enQueue(graph.graph);
    dis[0].distance = 0;
    dijkstra(dis, dis->preVNode);
    //打印最短路径和前驱
    for (int i = 1; i < graph.capacity; ++i) {
        printf("节点:%d->[%d,%d]  ", i + 1, dis[i].preVNode->index + 1, dis[i].distance);
    }
    printf("\n");
    return 1;
}

//打印邻接表
int printAdjacencyList() {
    printf("打印邻接表\n");
    for (int i = 0; i < graph.capacity; ++i) {
        printf("%d-> ", graph.graph[i].index);
        if (graph.graph[i].pEdge == NULL) {
            printf("^\n");
            continue;
        }
        UEdge* uEdge = graph.graph[i].pEdge;
        while (uEdge != NULL) {
            printf("%d-> ", uEdge->to->index);
            uEdge = uEdge->next;
        }
        printf(" ^\n");
    }
    printf("\n");
    return 0;
}

//往邻接表里塞东西
//有向图
int initialGraph() {
    createGraph(5);
    //添加节点
    createNode();//0
    createNode();//1
    createNode();//2
    createNode();//3
    createNode();//4
    //添加边
    //1节点的边
    addEdgeToAdjacencyList(0, 1, 4, 0);
    addEdgeToAdjacencyList(0, 3, 5, 0);
    //2节点的边
    addEdgeToAdjacencyList(1, 4, 1, 0);
    addEdgeToAdjacencyList(1, 2, 3, 0);
    //3节点的边
    //4节点的边
    addEdgeToAdjacencyList(3, 2, 2, 0);
    //5节点的边
    addEdgeToAdjacencyList(4, 2, 3, 0);
    return 0;
}

int main() {
    //初始化图＆打印邻接表
    initialGraph();
    printAdjacencyList();
    //两种遍历方式
    traversalGraph(1);
    traversalGraph(0);
    //拓扑排序
    topologicalSort();
    //单源最短路径算法
    preDijkstra();
    return 0;
}
