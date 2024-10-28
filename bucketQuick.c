#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

struct Node {
  int data;
  struct Node *next;
};

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    int j;
    for (j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quick_sort(arr, low, pivot - 1);
        quick_sort(arr, pivot + 1, high);
    }
}

int getBucketIndex(int value, int interval) {
  return value / interval;
}

void BucketSort(int arr[], int n, int nBucket, int interval) {
  int i, j;
  struct Node **buckets;

  buckets = (struct Node **)malloc(sizeof(struct Node *) * nBucket);

  for (i = 0; i < nBucket; ++i) {
    buckets[i] = NULL;
  }

  for (i = 0; i < n; ++i) {
    struct Node *current;
    int pos = getBucketIndex(arr[i], interval);
    current = (struct Node *)malloc(sizeof(struct Node));
    current->data = arr[i];
    current->next = buckets[pos];
    buckets[pos] = current;
  }

  for (i = 0; i < nBucket; i++) {
    printf("Bucket[%d]: ", i);
    printBuckets(buckets[i]);
    printf("\n");
  }

  for (i = 0; i < nBucket; ++i) {
    buckets[i] = quick_sort(buckets[i], i*interval, ((i+1)*interval)-1);
  }

  printf("-------------\n");
  printf("Bucktets after sorting\n");
  for (i = 0; i < nBucket; i++) {
    printf("Bucket[%d]: ", i);
    printBuckets(buckets[i]);
    printf("\n");
  }

  for (j = 0, i = 0; i < nBucket; ++i) {
    struct Node *node;
    node = buckets[i];
    while (node) {
      arr[j++] = node->data;
      node = node->next;
    }
  }

  return;
}



void print(int ar[], int n) {
  int i;
  for (i = 0; i < n; ++i) {
    printf("%d ", ar[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
    int *array;  /* Array dinâmico, tamanho será definido pelo usuário */
    int n = 0;   /* Contador de elementos lidos */
    int NARRAY;  /* Número máximo de elementos no array */

    /* Verifica se foram passados os argumentos corretos */
    if (argc != 3) {
        printf("Uso: %s <tamanho_do_array> <arquivo_com_numeros>\n", argv[0]);
        return 1;
    }

    /* Converte o primeiro argumento para o tamanho máximo do array */
    NARRAY = atoi(argv[1]);
    if (NARRAY <= 0) {
        printf("O tamanho do array deve ser um número positivo.\n");
        return 1;
    }

    int NBUCKET = (int)sqrt(NARRAY);
    int INTERVAL = (int)(NARRAY/NBUCKET)+1;

    /* Aloca memória para o array */
    array = (int *)malloc(NARRAY * sizeof(int));
    if (array == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    /* Abre o arquivo para leitura */
    FILE *file = fopen(argv[2], "r");
    if (!file) {
        printf("Erro ao abrir o arquivo %s\n", argv[2]);
        free(array);  /* Libera a memória em caso de erro */
        return 1;
    }

    /* Lê números do arquivo até o final ou até atingir o limite de NARRAY */
    while (fscanf(file, "%d", &array[n]) == 1 && n < NARRAY) {
        n++;
    }
    fclose(file);

    printf("Initial array: ");
    print(array, n);
    printf("-------------\n");

    BucketSort(array, n, NBUCKET, INTERVAL);
    
    printf("-------------\n");
    printf("Sorted array: ");
    print(array, n);

    /* Libera a memória alocada */
    free(array);

    return 0;
}