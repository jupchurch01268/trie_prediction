#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *stripPunctuators(char *str)
{
	int len = strlen(str), i = 0;
	char *temp = malloc(sizeof(char) * len), test = *str;

	while(test != '\0')
	{
		if (isalpha(test))
		{
			temp[i] = test;
			i++;
		}
		test = *(++str);
	}

	return temp;
}

void stripPunctuatorsFancy(char *str)
{
	int i, k = 0, len = strlen(str);
	char *temp = str, *temp2;

	for (i = 0; i < len; i++)
	{

		if (isalpha(temp[i]))
		{
			temp2 = str + k;
			temp2 = temp + i;
			k++;
		}
	}

	*(str + k) = '\0';

}



int main(void)
{
	char *str = "robot.";
	char *str2 = "robot.";

	str = stripPunctuators(str);
	//stripPunctuatorsFancy(str2);
	
	printf(" %s ", str);
	printf(" %s ", str2);

	return 0;
}