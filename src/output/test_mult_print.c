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
    char name[BUFSIZE] = "";
    int age = 0;
    float height = 0.0;
    int cool = 0;
    char beans[BUFSIZE] = "";

    strcpy(name, "keith");
    age = 23;
    height = 6.200000;
    printf("%s", mulstr1(name, 3));

    exit(0);
}
