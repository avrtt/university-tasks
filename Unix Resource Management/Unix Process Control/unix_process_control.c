#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void compute_ln(double x, int n) {
    unsigned step_count;
    FILE *result;

    if (fork() == 0)
    {
        char str[50];
        char *estr;
        const char *stop = "STOP";
        result = fopen("result", "r");

        sleep(1);

        while (1) 
        {
            estr = fgets(str, sizeof(str), result);
            int v = strcmp(estr, stop);

            if (v == 0)
            {
                printf("Exit keyword passed.\n");
                break;
            }
            printf("%s", str);
        } 
        
        fclose(result);
        exit(0);
    }

    result = fopen("result", "wa");

    if (x <= 0.0)
    {
        printf("Error: The argument must be greater than 0!\n");
        exit(0);
    }
    
    x = x - 1.0;

    for (int i = 0; i < n; ++i)
    {
        double sum = 0.0, m = x, nom = x;
        step_count = 1;
  
        while (fabs(m) > 1e-11)
        {
            sum += m; 
            nom *= - x;
            m = nom / ++step_count;
        };
        
        fprintf(result, "ln(%.3lg) = %f\n", x + 1.0, sum);
    }
    
    fprintf(result, "STOP");
    fclose(result);
}

int main() {
    srand(time(NULL));
    int n = 1000 + rand() % 1000;
    double x = (double)rand() / RAND_MAX*2.0;

    compute_ln(x, n);

    return 0;
}

