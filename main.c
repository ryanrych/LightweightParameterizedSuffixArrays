#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct pv{
    int* encoded;
    int zlen;
    int length;
    int startingIndex;
    int firstNonZero;
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

struct pv getpv(char* str, int pi, int n, int startingIndex){
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
    r.length = n;
    r.startingIndex = startingIndex;
    r.encoded = encoded;
    r.zlen = zlen;
    r.firstNonZero = encoded[zlen];
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

struct pv* countSortByZlen(struct pv* A, int start, int n){
    int max = A[start].zlen;
    for (int i=start + 1; i<n; i++){
        if (A[i].zlen > max){
            max = A[i].zlen;
        }
    }

    int* count = (int*) malloc((max + 1) * sizeof(int));
    int* cumulative = (int*) malloc((max + 1) * sizeof(int));
    for (int i=0; i<max + 1; i++){
        count[i] = 0;
        cumulative[i] = 0;
    }

    for (int i=start; i<n; i++){
        count[A[i].zlen] = count[A[i].zlen] + 1;
    }

    int c = 0;
    for (int i=max; i>=0; i--){
        cumulative[i] = c + count[i];
        c = c + count[i];
    }

    struct pv* sorted = malloc(n * sizeof(struct pv));

    for (int i=0; i<start; i++){
        sorted[i] = A[i];
    }

    for (int i=n-1; i>=start; i--){
        sorted[cumulative[A[i].zlen] - 1 + start] = A[i];
        cumulative[A[i].zlen]--;
    }

    return sorted;
}

struct pv* countSortByFirstNonZero(struct pv* A, int start, int n){
    int max = A[start].firstNonZero;
    for (int i=start + 1; i<n; i++){
        if (A[i].firstNonZero > max){
            max = A[i].firstNonZero;
        }
    }

    int* count = (int*) malloc((max + 1) * sizeof(int));
    int* cumulative = (int*) malloc((max + 1) * sizeof(int));
    for (int i=0; i<max + 1; i++){
        count[i] = 0;
        cumulative[i] = 0;
    }

    for (int i=start; i<n; i++){
        count[A[i].firstNonZero] = count[A[i].firstNonZero] + 1;
    }

    int c = 0;
    for (int i=0; i<max + 1; i++){
        cumulative[i] = c + count[i];
        c = c + count[i];
    }

    struct pv* sorted = malloc(n * sizeof(struct pv));

    for (int i=0; i<start; i++){
        sorted[i] = A[i];
    }

    for (int i=n-1; i>=start; i--){
        sorted[cumulative[A[i].firstNonZero] - 1 + start] = A[i];
        cumulative[A[i].firstNonZero]--;
    }

    return sorted;
}

struct pv* countSortByPV(struct pv* A, int sortIndex, int n, int start, int end){
    int max = INT_MIN;
    for (int i = start; i < end; i++){
        if (A[i].length > sortIndex){
            if (A[i].encoded[sortIndex] > max){
                max = A[i].encoded[sortIndex];
            }
        }
    }

    int* count = malloc((max + 1) * sizeof(int));
    int* cumulative = malloc((max + 1) * sizeof(int));
    for (int i=0; i<max + 1; i++){
        count[i] = 0;
        cumulative[i] = 0;
    }

    for (int i = start; i < end; i++){
        if (A[i].length > sortIndex){
            count[A[i].encoded[sortIndex]]++;
        }else{
            count[0]++;
        }
    }

    int c = 0;
    for (int i=0; i<max + 1; i++){
        cumulative[i] = count[i] + c;
        c = c + count[i];
    }

    struct pv* sorted = malloc(n * sizeof(struct pv));

    for (int i=0; i<start; i++){
        sorted[i] = A[i];
    }
    for (int i=end; i<n; i++){
        sorted[i] = A[i];
    }

    for (int i=end-1; i>=start; i--){
        if (A[i].length > sortIndex){
            sorted[cumulative[A[i].encoded[sortIndex]] - 1 + start] = A[i];
            cumulative[A[i].encoded[sortIndex]]--;
        }else{
            sorted[cumulative[0] - 1 + start] = A[i];
            cumulative[0]--;
        }
    }

    return sorted;
}

struct pv* raddixSort(struct pv* A, int n, int start, int end){
    struct pv* sorted = countSortByPV(A, end - 1, n, start, end);
    for (int i = end-2; i>=start; i--){
        sorted = countSortByPV(sorted, i, n, start, end);
    }
    return sorted;
}

struct pv* sortPrevs(struct pv* A, int n){
    struct pv* sorted = malloc(sizeof(struct pv) * n);

    int startIndex = 0;
    for(int i=0; i<n; i++){
        if (A[i].type == 0){
            struct pv temp = A[A[i].zlen - 1];
            A[A[i].zlen - 1] = A[i];
            A[i] = temp;
            startIndex++;
        }
    }

    A = countSortByFirstNonZero(A, startIndex, n);
    A = countSortByZlen(A, startIndex, n);

    int endIndex = n-1;
    while (A[endIndex].type == 2){
        endIndex--;
    }

    A = raddixSort(A, n, startIndex, endIndex + 1);

    return A;

}

//fix this
char* substring(char* s, int start, int end){
    char* r = malloc((end - start) * sizeof(char));
    for(int i=start; i<end; i++){
        r[i - start] = s[i];
    }
    return r;
}

int main() {

    char* s = "babbcacaabcb";
    int n = 12;

    char** suffixes = malloc(n * sizeof(char*));

    suffixes[0] = "babbcacaabcb";
    suffixes[1] = "abbcacaabcb";
    suffixes[2] = "bbcacaabcb";
    suffixes[3] = "bcacaabcb";
    suffixes[4] = "cacaabcb";
    suffixes[5] = "acaabcb";
    suffixes[6] = "caabcb";
    suffixes[7] = "aabcb";
    suffixes[8] = "abcb";
    suffixes[9] = "bcb";
    suffixes[10] = "cb";
    suffixes[11] = "b";

    struct pv* pvs = malloc(n * sizeof(struct pv));

    for (int i=0; i<n; i++){
        pvs[i] = getpv(suffixes[i], 4, n - i, i);
    }

    struct pv* test = sortPrevs(pvs, 12);

    for (int i=0; i<n; i++){
        for (int j = 0; j<test[i].length; j++){
            printf("%d ",test[i].encoded[j]);
        }
        printf("  %d",test[i].type);
        printf("\n");
    }

    return 0;
}