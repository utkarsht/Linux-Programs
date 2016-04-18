struct node {
	int data;
	struct node* next;
};

struct nodeptr {
	struct node* ptr;
};

program LINKLIST {
	version LINKLIST_1 {
		void addNode(int) = 1;
		struct nodeptr print() = 2;
		void addone() = 3;
	} = 1;
} = 0x1fffffff;
