#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 1024

char *concat(char *s1, char *s2) {
	int len1 = strlen(s1);
	int len2 = strlen(s2);
	char *new_s = malloc(sizeof(char) * (len1 + len2 + 1));

	size_t k = 0;
	for (size_t i = 0; i < len1; ++i) {
	       new_s[k++] = s1[i];
	}
	for (size_t i = 0; i <= len2; ++i) {
		new_s[k++] = s2[i];
	}
	return new_s;
}

char *mulstr1(char *s1, int n){
    if(n < 0) {
        printf("* [Runtime Error] non-negative value expected for string multi.\n");
        exit(1);
    }
    int len = strlen(s1);
    char *new_s = malloc(sizeof(char) * (len * n + 1));
    char *pt = new_s;

    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < len; j++) {
            *pt++ = s1[j];
        }
    }
    *pt = 0;
    return new_s;
}

char *mulstr2(int n, char *s1) {
    return mulstr1(s1, n);
}

int main() {
    int i = 0;
    int j = 0;
    int k = 0;

    i = 3;
    while (i) {
        j = 3;
        while (j) {
            k = 3;
            while (k) {
                printf("%d", ((i+k)+j));
                printf("%s", "\n");
                k = (k-1);
            }
            j = (j-1);
        }
        i = (i-1);
    }

    exit(0);
}
