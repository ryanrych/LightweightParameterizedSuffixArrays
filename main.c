#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct pv{
    char* encoded;
    int zlen;
    int type;
};

struct node{
    char key;
    int val;
    struct node *next;
};
struct table{
    int size;
    struct node **list;
};
struct table *createTable(int size){
    struct table *t = (struct table*)malloc(sizeof(struct table));
    t->size = size;
    t->list = (struct node**)malloc(sizeof(struct node*)*size);
    int i;
    for(i=0;i<size;i++)
        t->list[i] = NULL;
    return t;
}
int hashCode(struct table *t,char key){
    return ((int) key)%t->size;
}
void insert(struct table *t,char key,int val){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
    newNode->key = key;
    newNode->val = val;
    newNode->next = list;
    t->list[pos] = newNode;
}
int lookup(struct table *t,char key){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            return temp->val;
        }
        temp = temp->next;
    }
    return -1;
}

struct pv getpv(char* str, int pi, int n){
    struct table* map = createTable(pi);
    int* encoded = (int*) malloc(n * sizeof(int));
    int zlen = 0;
    bool zflag = false;

    for (int i=0; i<n; i++){
        int lastOcc = lookup(map, str[i]);
        if (lastOcc == -1){
            if (!zflag){
                zlen++;
            }
            encoded[i] = 0;
            insert(map, str[i], i);
        }else{
            if (!zflag){
                zflag = true;
            }
            encoded[i] = i - lastOcc;
            insert(map, str[i], i);
        }
    }

    struct pv r;
    r.encoded = encoded;
    r.zlen = zlen;
    if (zlen == n){r.type = 0;}
    else{if (zlen > encoded[zlen]){r.type = 1;}
    else{r.type = 2;}}

    return r;
}

int* getfw(char* str, int pi, int n){
    struct table* map = createTable(pi);
    int* encoded = (int*) malloc(n * sizeof(int));
    for (int i=n-1; i>=0; i--){
        int lastOcc = lookup(map, str[i]);
        if (lastOcc == -1){
            encoded[i] = n;
            insert(map, str[i], i);
        }else{
            encoded[i] = lastOcc - i;
            insert(map, str[i], i);
        }
    }
    return encoded;
}

int partition(int left, int right, int pivot, int* A){
    int partitionIndex = left;
    for (int i=left; i<=right; i++){
        if (A[i] < A[pivot]){
            partitionIndex++;
        }
    }
    int temp = A[pivot];
    A[pivot] = A[partitionIndex];
    A[partitionIndex] = temp;
    for (int i=left; i<partitionIndex; i++){
        if (A[i] > A[partitionIndex]){
            for (int j=partitionIndex+1; j<=right; j++){
                if (A[j] <= A[partitionIndex]){
                    int temp = A[i];
                    A[i] = A[j];
                    A[j] = temp;
                }
            }
        }
    }
    return partitionIndex;
}

void quicksort(int left, int right, int* A) {
    if (left<right){
        int pivot = (left + right) / 2;
        int partitionIndex=partition(left, right, pivot, A);
        quicksort(left, partitionIndex - 1, A);
        quicksort(partitionIndex + 1, right, A);
    }
}

int main() {



    return 0;
}