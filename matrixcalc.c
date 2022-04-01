#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define coloumnNumber 20
#define rowNumber 20

pthread_mutex_t lockCreateMatrix;
pthread_mutex_t lockSumMatrix;

int matrix[coloumnNumber][rowNumber];

int reputationNumber;

void *sumMatrix()
{
    for (int k = 0; k < reputationNumber; k++)
    {
        pthread_mutex_lock(&lockSumMatrix);
        int total = 0;
        for (int i = 0; i < rowNumber; i++)
        {
            for (int j = 0; j < coloumnNumber; j++)

            {
                total += matrix[i][j];
            }
        }

        printf("Matris Toplamı = %d\n \n", total);
        pthread_mutex_unlock(&lockCreateMatrix);
    }
}

void *createMatrix()
{
    for (int k = 0; k < reputationNumber; k++)
    {
        pthread_mutex_lock(&lockCreateMatrix);

        for (int i = 0; i < rowNumber; i++)
        {
            for (int j = 0; j < coloumnNumber; j++)
            {

                int random = 10 + rand() % 50;
                matrix[i][j] = random;
            }
        }

        for (int i = 0; i < rowNumber; i++)
        {
            for (int j = 0; j < coloumnNumber; j++)
            {
                printf("%d   ", matrix[i][j]);
            }
            printf("\n");
        }
        pthread_mutex_unlock(&lockSumMatrix);
    }
}

void ctrl_c_Signal()
{
    printf("\nYou pressed CTRL+C\n");
    signal(SIGINT, ctrl_c_Signal);
    pthread_mutex_destroy(&lockCreateMatrix);
    pthread_mutex_destroy(&lockSumMatrix);
    printf("Çıkış Yapılıyor...\n");
    exit(0);
}
void ctrl_z_Signal()
{
    printf("\nYou pressed CTRL+Z\n");
    signal(SIGTSTP, ctrl_z_Signal);
    pthread_mutex_destroy(&lockCreateMatrix);
    pthread_mutex_destroy(&lockSumMatrix);
    printf("Çıkış Yapılıyor...\n");
    exit(0);
}

void controlValue(){
    if(reputationNumber < 1 ){
        printf("Çıkış Yapılıyor...\n");
        exit(0);
    }
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    pthread_mutex_init(&lockCreateMatrix, NULL);
    pthread_mutex_init(&lockSumMatrix, NULL);

    pthread_mutex_lock(&lockSumMatrix);

    pthread_t createMatrixThread;
    pthread_t sumMatrixThread;

    void *status;

    while (1)
    {
        signal(SIGINT, ctrl_c_Signal);
        signal(SIGTSTP, ctrl_z_Signal);
        printf("Lütfen Bir Sayı Giriniz:");
        scanf("%d", &reputationNumber);

        controlValue();

        pthread_create(&sumMatrixThread, NULL, sumMatrix, NULL);
        pthread_create(&createMatrixThread, NULL, createMatrix, NULL);

        pthread_join(sumMatrixThread, &status);
        pthread_join(createMatrixThread, &status);
    }

    return 0;
}
