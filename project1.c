#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const char ops[5] = {'+', '-', '*', '/', '^'};


char variables[29];
int variableCount = 0;
char postfix[100][3];
int count = 0;
int isVarNum[29];
int VarNum[29];
char VarOp[29];

typedef struct stack_node{
	int data;
	int isOperator; // 0 - num, 1 - op
	char op;
	struct stack_node *next;
	struct stack_node *left, *right;
}stacknode;

typedef stacknode *stacknodeptr;
//stacknodeptr nodeptr, top;

void push(stacknodeptr *topptr, stacknodeptr newnodeptr){
	newnodeptr->next = *topptr;
	*topptr = newnodeptr;
}

stacknodeptr pop (stacknodeptr *topptr){
	if(*topptr == NULL ) return 0;
	
	stacknodeptr temp = *topptr;
	*topptr =temp->next;
	
	return temp;
}

int stackSize(stacknodeptr top){
	int count =0;
	
	while(top) {
		count++;
		top = top->next;
	}
	
	return count;
}


typedef struct TreeStack{
	stacknodeptr root;
	struct TreeStack *next;
}treestack;

typedef treestack *treeptr;
treeptr foundtrees = NULL;

void pushTree(stacknodeptr root){
	treeptr t = malloc(sizeof(treestack));
	t->root = root;
	t->next = foundtrees;
	foundtrees = t;
	
}



int stringToInt(const char *s) {
	int num = 0;
	int i =0;
	int negative = 0;
	
	if(s[0] == '-') {
		negative = 1;
		i++;
	}
	
	while (s[i] != '\0'){
		if(s[i] < '0' || s[i] > '9') 
		break;
		
		num = num *10 + (s[i] - '0');
		i++;
	}
	
	if(negative) return -num;
	else return num;
}

int isAnOperator(const char *p){
	return (strcmp(p, "+")==0 || strcmp(p, "-")==0 ||strcmp(p, "*")==0 ||strcmp(p, "/")==0 ||strcmp(p, "^")==0 );
}

/*int poww(int base, int exp) {
    if (exp < 0) return 0;
    if (exp == 0) return 1;
    
    int result = 1;
    
    int i =0;
    for (i; i < exp; i++) {
    	result *= base;
	}

    return result;
}*/

int applyOperator(int a, int b, char op, int *result){
	if(!result) return 0;
	
	switch(op){
		case '+' : *result = a + b; return 1;
		case '-': *result = a-b; return 1;
		case '*': *result = a*b; return 1;
		case '/': {
			if (b==0) return 0;
			if (a%b != 0) return 0;
			*result = a / b; return 1;
		}
		case '^': {
			if(b<0) return 0;
			int base = a;
			int exp =b;
			int r = pow(a, b);
			*result = r; return 1;
		}
		default: return 0;
	}
}
void freeTree(stacknodeptr node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

stacknodeptr buildTree(char pf[][3], int count){
	stacknodeptr top = NULL;
	
	int i = 0;
	for(i; i< count; i++) {
		char *t = pf[i];
		
		stacknodeptr node = malloc(sizeof(stacknode));
		node->right = NULL;
		node->left = NULL;
		node->next = NULL;
		
		//for variables
		if(t[0] >= 'a' && t[0] <= 'z') {
			int j = -1; //the index
			
			int k = 0;
			for(k; k < variableCount; k++){
				if(variables[k] == t[0]){
					j = k;
					break;
				}
			}
			
			if (j < 0) return NULL;
			
			// operand ise
			if(isVarNum[j]) {
				node->isOperator = 0;
				node->data = VarNum[j];
				push(&top, node);
			} else { //operator ise
			
				stacknodeptr right = pop(&top);
				stacknodeptr left = pop(&top);
				
				if (!left || !right) {
					free(node); 
					return NULL;
				}
				
				node->isOperator = 1;
				node->op = VarOp[j];
				node->left = left;
				node->right = right;
				push(&top, node);
			}
			/*int value;
			char op;
			
			if(*t == 'a') {
				value = a1;
				op = ch1;
			} else if(*t == 'b') {
				value = a2;
				op = ch2;
			} else {
				value = a3;
				op = ch3;
			}
			
			//if n then suppose variable is number
			if (op == 'n') {
				node->isOperator = 0;
				node->data = value;
				push(&top, node);
			} else {   //operator
				stacknodeptr right = pop(&top);
				stacknodeptr left = pop(&top);
				if(!left || !right) return NULL;
				
				node->isOperator = 1;
				node->op = op;
				node->left =left;
				node->right = right;
				push(&top, node);
			}*/
		}
		
		//direkt operator
		else if(isAnOperator(t)) {
			stacknodeptr right = pop(&top);
			stacknodeptr left = pop(&top);
			if(!left || !right) {
				free(node);
				return NULL;
			} 
			
			node->isOperator = 1;
			node->op = t[0];	
			node->left = left;
			node->right = right;
			push(&top, node);
		}
		
		//direkt operand
		else {
			node->isOperator = 0;
			node->data = stringToInt(t);
			push(&top, node);
		}
	}
	
	stacknodeptr root = pop(&top);
	
	if (top != NULL){
		
		return NULL;
	}
	
	return root;
	
	
}



int postorder(stacknodeptr node, int *b){
	if(!node) {
		*b = 0; return 0;
	}
	
	if(!node->isOperator) return node->data;
	
	int nl = postorder(node->left, b);
	if(!*b) return 0;
	
	int nr = postorder(node->right, b);
	if(!*b) return 0;
	
	int r;
	if(!applyOperator(nl, nr, node->op, &r)) {
		*b = 0; return 0;
	}
	
	return r;
}


/*int solveExpTree(char postfix[][3], int count, int a1, int a2, int a3, char ch1, char ch2, char ch3, int *finalResult) {
	
	stacknodeptr root = buildTree(postfix, count, a1, a2,a3, ch1, ch2, ch3);
	
	if(!root) return 0;
	
	int i = 1;
	int r = postorder(root, &i);
	
	freeTree(root);
	
	if(!i) return 0;
	
	*finalResult = r;
	return 1;
}*/

void solveT(int f) {
	if(f == variableCount) {
		stacknodeptr root = buildTree(postfix, count);
		
		if(!root){
			 //printf("fail \n");
			 return;
		} 
		
		int b = 1; // bool 
		int result =postorder(root, &b);
		
		//printf("result: %d, b=%d\n", result, b);
		if((b && result == 0) && !isalreadyfound(root)) {
			
			
				
				printf("(");
				
				int i = 0;
				for(i; i< variableCount; i++) {
					
					if(isVarNum[i]) {
						printf("%d", VarNum[i]);
					} else {
						printf("%c", VarOp[i]);
					}
					
					if(i != variableCount -1) {
						printf(",");
					}
				}
				
				printf(")\n");
				
				pushTree(root);
				return;
			
		} 
		
		
		return;
    }
	//operand
	isVarNum[f] = 1;
	int m = 1;
	for(m; m <25; m++) {
		VarNum[f] = m;
		solveT(f +1);
	}
		
	//operator
	isVarNum[f] = 0;
	int v = 0;
	for(v; v< 5; v++) {
	VarOp[f] = ops[v];
	solveT(f + 1);
	}
		

} 

int sameTree(stacknodeptr s1, stacknodeptr s2){
	if (s1 == NULL && s2 == NULL) return 1;
	if (s1 == NULL || s2 == NULL) return 0;
	
	if (s1->isOperator != s2->isOperator) return 0;
	
	if (!s1->isOperator){
		return s1->data == s2->data;
	}
	
	if(s1->op != s2->op) return 0;
	
	return sameTree(s1->left, s2->left) && sameTree(s1->right, s2->right);
}


int isalreadyfound(stacknodeptr root){
	treeptr p = foundtrees;
	while(p) {
		if(sameTree(root, p->root)) {
			
			return 1;
		}
		
		p = p->next;
	}
	
	return 0;
}


int main(void){
	FILE *f = fopen("input.txt", "r");
	if(!f) return 1;
	
	int c = 0;
	while (fscanf(f, "%2s", postfix[c])==1) {
		if(strcmp(postfix[c], "=") == 0) {
			break;
		} 
		c++;
	}
	fclose(f);
	count = c;
	
	/*printf("count = %d\n", count);
	int i = 0;
	for(i; i< count; i++){
		printf("[%s] ", postfix[i]);
	}
	printf("\n");*/
	
	int a = 0;
	for(a; a< count; a++) {
		
		char c = postfix[a][0];
		if(c >= 'a' && c<= 'z') {
			
			int e = 0;
			int j = 0;
			for (j; j< variableCount; j++) {
				if(variables[j] == c) {
					e = 1;
				}
				}
			if(!e){
				variables[variableCount++] = c;
			}
				
			
		}
		
	}
	
	printf("Possible combination values for (a, b, c): \n");
	solveT(0);
	
	

	return 0;
}

