#include <stdio.h>
#include <memory.h>

#define MAX_LEN 1000

int nume; //numerator分子
int deno; //denominator分母
int quot[MAX_LEN]; //quotient商	
int rem[MAX_LEN]; //remainder余数
int negative_flag; //负数标志

void save_quot_rem() //存取商和余数
{
	int i;
	negative_flag = nume/(double)deno < 0 ? 1 : 0; //若是负数则标志为1
	nume = nume < 0 ? -nume : nume; //若是负数则转换为正数,下同
	deno = deno < 0 ? -deno : deno;
	for(i=0; i<MAX_LEN; i++)
	{
		quot[i] = nume/deno;
		rem[i] = nume%deno;
		nume = 10*rem[i];
		if(!rem[i])
			break;
	} //for
}

//判断是否是循环小数,若是则保存循环节起始和结束位置
int is_circu(int *start, int *end)
{
	int i, j;
	for(i=0; i<MAX_LEN; i++)
	{
		if(-1 == rem[i])
			return 0;
	}
	for(i=0; i<MAX_LEN; i++)
	{
		for(j=i+1; j<MAX_LEN; j++)
		{
			if(rem[i] == rem[j])
			{
				*start = i;
				*end = j;
				return 1;
			} //if
		} //for
	} //for
	return 0;
}

void show_circu(int start, int end) //显示循环小数
{
	int i;
	//printf(negative_flag ? "-%d." : "%d.", quot[0]); //整数部分
	for(i=1; i<=start; i++) //小数非循环节部分
	{
		printf("%d", quot[i]);
	}
	//printf("(");
	for(i=start+1; i<=end; i++) //小数循环节部分
	{
		printf("%d", quot[i]);
	}
    printf("\n");
	//printf(")\n\n");
}

void show_not_circu() //显示非循环小数
{
	int i;
	if(-1 == quot[1]) //无小数部分
	{
		printf(negative_flag ? "-%d" : "%d", quot[0]); 
		printf("\n\n");
		return;
	}
	//printf(negative_flag ? "-%d." : "%d.", quot[0]); 
	for(i=1; (i < MAX_LEN) && (-1 != quot[i]); i++) //小数部分
	{
		printf("%d", quot[i]);
	}
	printf("\n");
}

int main()
{
	int start, end;
	printf("请输入分子和分母,用空格分开(输入两个0离开):\n");
	while(1)
	{
		fflush(stdin);
		scanf("%d%d", &nume, &deno);
		if(!nume && !deno)
			return 0;
		memset(quot, -1, sizeof(quot));
		memset(rem, -1, sizeof(rem));
		if(!deno)
		{
			printf("除数不能为0,请重新输入:\n");
			continue;
		}
		save_quot_rem();
		is_circu(&start, &end) ? show_circu(start, end) : show_not_circu();
	}
	return 0;
}

