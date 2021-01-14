#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct pv{
    int* encoded;
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

int** countSort(int** A, int n, int sortIndex){
    int max = A[0][sortIndex];
    for (int i=1; i<n; i++){
        if (A[i][sortIndex] > max){
            max = A[i][sortIndex];
        }
    }

    int* count = (int*) malloc((max + 1) * sizeof(int));
    int* cumulative = (int*) malloc((max + 1) * sizeof(int));
    for (int i=0; i<max + 1; i++){
        count[i] = 0;
        cumulative[i] = 0;
    }

    for (int i=0; i<n; i++){
        count[A[i][sortIndex]] = count[A[i][sortIndex]] + 1;
    }

    int c = 0;
    for (int i=0; i<max + 1; i++){
        cumulative[i] = c + count[i];
        c = c + count[i];
    }

    int** sorted = malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++)
        sorted[i] = malloc(sizeof(int) * n);

    for (int i=n-1; i>-1; i--){
        sorted[cumulative[A[i][sortIndex]] - 1] = A[i];
        cumulative[A[i][sortIndex]]--;
    }

    return sorted;
}

int** raddixSort(int** A, int n){
    int** sort = A;
    for (int i=n-1; i>-1; i--){
        sort = countSort(sort, n, i);
    }
    return sort;
}


int main() {

    int** test = malloc(6 * sizeof(int*));
    for (int i=0; i<6; i++){
        test[i] = malloc(6 * sizeof(int));
    }
    test[0][0]=3;test[0][1]=4;test[0][2]=1;test[0][3]=2;test[0][4]=5;test[0][5]=1;
    test[1][0]=4;test[1][1]=5;test[1][2]=3;test[1][3]=0;test[1][4]=2;test[1][5]=1;
    test[2][0]=2;test[2][1]=4;test[2][2]=1;test[2][3]=5;test[2][4]=0;test[2][5]=3;
    test[3][0]=0;test[3][1]=2;test[3][2]=1;test[3][3]=4;test[3][4]=5;test[3][5]=3;
    test[4][0]=3;test[4][1]=0;test[4][2]=5;test[4][3]=4;test[4][4]=2;test[4][5]=1;
    test[5][0]=1;test[5][1]=2;test[5][2]=3;test[5][3]=4;test[5][4]=0;test[5][5]=5;

    test = raddixSort(test, 6);

    for(int i = 0; i<6; i++){
        for (int j = 0; j<6; j++){
            printf("%d ",test[i][j]);
        }
        printf("\n");
    }

    return 0;
}