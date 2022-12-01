#include<stdio.h>

int main() {

    unsigned int num;
    printf("정수를 입력해주세요\n");
    scanf("%d", &num);

    printf("2진수 변환 : ");
    //이진수 출력
    for (int i = 7; i >= 0; --i) { //8자리 숫자까지 나타냄
        int result = num >> i & 1;
        printf("%d", result);
    }
	printf("\n");		
}
