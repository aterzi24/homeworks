#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX  100
#define ORI(x)  ((x)->orig)
#define OT(x)  ((x)->tp_or)
#define DER(x)  ((x)->deri)
#define DT(x)  ((x)->tp_de)

struct stack{
	char *orig, tp_or;
	char *deri, tp_de;
	struct stack *back;
};

char * dec(char *str)
{
	int i,size;
	size = strlen(str);
	i = size - 1;
	while(1)
	{
		if( str[i] == '0' )
		{
			str[i] = '9';
			i--;
			continue;
		}
		str[i] -= 1;
		break;
	}
	if( str[0] == '0' )
		str = str + 1;
	return str;
}

struct stack *arrange(char *str, int i)
{
	struct stack *temp;
	int j = 0;
	temp = malloc(sizeof(struct stack));
	if( str[i] == 'X' )
	{
		ORI(temp)="X";
		OT(temp) = 2;
		DER(temp) = "1";
		DT(temp) = 1;
	}
	else if( str[i] == 's' )
	{
		ORI(temp)="s";
		OT(temp) = 2;
		DER(temp) = "c";
		DT(temp) = 2;
	}
	else if( str[i] == 'a' )
	{
		ORI(temp)="a";
		OT(temp) = 2;
		DER(temp) = "e";
		DT(temp) = 2;
	}
	else if( str[i] == 'e' )
	{
		ORI(temp)="e";
		OT(temp) = 2;
		DER(temp) = "a";
		DT(temp) = 2;
	}
	else if( str[i] == 't' )
	{
		ORI(temp)="t";
		OT(temp) = 2;
		DER(temp) = "t^2+1";
		DT(temp) = 3;
	}
	else if( str[i] == 'l' )
	{
		ORI(temp)="l";
		OT(temp) = 2;
		DER(temp) = "1/X";
		DT(temp) = 4;
	}
	else if( str[i] == '.' )
	{
		ORI(temp) = malloc(5);
		i++;
		while( str[i] != '.' )
			ORI(temp)[j++] = str[i++];
		ORI(temp)[j] = 0;
		OT(temp) = 1;
		DER(temp) = "0";
		DT(temp) = 1;
	}
	return temp;
}

char * alg(char *str, int size)
{
	struct stack *end, *temp, *fr;
	int i=1;
	temp = arrange(str,0);
	temp->back = NULL;
	end = temp;
	if( OT(temp) == 1 )
		i += strlen(ORI(temp)) + 1;
	while( i < size )
	{
		if( str[i] == '+' || str[i] == '-' )
		{
			temp = malloc(sizeof(struct stack));
			ORI(temp) = malloc(strlen(ORI(end->back))+strlen(ORI(end))+5);
			DER(temp) = malloc(strlen(DER(end->back))+strlen(DER(end))+5);
			strcpy(ORI(temp), ORI(end->back));
			ORI(temp)[strlen(ORI(temp))] = str[i];
			ORI(temp)[strlen(ORI(temp))+1] = 0;
			strcat(ORI(temp), ORI(end));
			OT(temp) = 3;
			if( !strcmp("0", DER(end->back)) )
			{
				strcpy(DER(temp), DER(end));
				DT(temp) = DT(end);
			}
			else if( !strcmp("0", DER(end)) )
			{
				strcpy(DER(temp), DER(end->back));
				DT(temp) = DT(end->back);
			}
			else
			{
				strcpy(DER(temp), DER(end->back));
				DER(temp)[strlen(DER(temp))] = str[i];
				DER(temp)[strlen(DER(temp))+1] = 0;
				strcat(DER(temp), DER(end));
				DT(temp) = 3;
			}
			fr = end;
			end = end->back;
			free(fr);
			fr = end;
			end = end->back;
			free(fr);
			temp->back = end;
			end = temp;
		}
		
		else if( str[i] == '*' )
		{
			temp = malloc(sizeof(struct stack));
			ORI(temp) = malloc(strlen(ORI(end->back))+strlen(ORI(end))+5);
			DER(temp) = malloc(strlen(DER(end->back))+strlen(DER(end))+5);
			DER(temp)[0] =  0;
			DT(temp) = 1;
			if( OT(end->back) == 3 )
			{
				ORI(temp)[0] = '(';
				strcpy(ORI(temp)+1, ORI(end->back));
				strcat(ORI(temp), ")*");
				if( OT(end) == 3 )
				{
					strcat(ORI(temp), "(");
					strcat(ORI(temp), ORI(end));
					strcat(ORI(temp), ")");
				}
				else
					strcat(ORI(temp), ORI(end));
				OT(temp) = 4;
			}
			else
			{
				strcpy(ORI(temp), ORI(end->back));
				strcat(ORI(temp), "*");
				if( OT(end) == 3 )
				{
					strcat(ORI(temp), "(");
					strcat(ORI(temp), ORI(end));
					strcat(ORI(temp), ")");
				}
				else
					strcat(ORI(temp), ORI(end));
				OT(temp) = 4;
			}
			if(DT(end->back) == 3)
			{
				strcpy(DER(temp), "(");
				strcat(DER(temp), DER(end->back));
				strcat(DER(temp), ")*");
				if(OT(end) == 3)
				{
					strcat(DER(temp), "(");
					strcat(DER(temp), ORI(end));
					strcat(DER(temp), ")");
				}
				else
					strcat(DER(temp), ORI(end));
				DT(temp) = 4;
			}
			else if(DT(end->back) != 1 || (strcmp("0",DER(end->back)) && strcmp("1",DER(end->back))) )
			{
				strcpy(DER(temp), DER(end->back));
				strcat(DER(temp), "*");
				if(OT(end) == 3)
				{
					strcat(DER(temp), "(");
					strcat(DER(temp), ORI(end));
					strcat(DER(temp), ")");
				}
				else
					strcat(DER(temp), ORI(end));
				DT(temp) = 4;
			}
			else if(!strcmp("1",DER(end->back)))
				strcpy(DER(temp), ORI(end));
			if( strcmp("0",DER(end->back)) && strcmp("0",DER(end)) )
				strcat(DER(temp), "+");
			if(DT(end) == 3)
			{
				strcat(DER(temp), "(");
				strcat(DER(temp), DER(end));
				strcat(DER(temp), ")*");
				if(OT(end->back) == 3)
				{
					strcat(DER(temp), "(");
					strcat(DER(temp), ORI(end->back));
					strcat(DER(temp), ")");
				}
				else
					strcat(DER(temp), ORI(end->back));
				DT(temp) = 4;
			}
			else if(DT(end) != 1 || (strcmp("0",DER(end)) && strcmp("1",DER(end))) )
			{
				strcat(DER(temp), DER(end));
				strcat(DER(temp), "*");
				if(OT(end->back) == 3)
				{
					strcat(DER(temp), "(");
					strcat(DER(temp), ORI(end->back));
					strcat(DER(temp), ")");
				}
				else
					strcat(DER(temp), ORI(end->back));
				DT(temp) = 4;
			}
			else if(!strcmp("1",DER(end)))
				strcat(DER(temp), ORI(end->back));
			if( strcmp("0",DER(end->back)) && strcmp("0",DER(end)) )
				DT(temp) = 3;		
			fr = end;
			end = end->back;
			free(fr);
			fr = end;
			end = end->back;
			free(fr);
			temp->back = end;
			end = temp;
		}
		
		else if( str[i] == '/' )
		{
			temp = malloc(sizeof(struct stack));
			ORI(temp) = malloc(strlen(ORI(end->back))+strlen(ORI(end))+5);
			DER(temp) = malloc(strlen(DER(end->back))+strlen(DER(end))+5);
			DER(temp)[0] =  0;
			DT(temp) = 1;
			if( OT(end->back) == 3 )
			{
				ORI(temp)[0] = '(';
				strcpy(ORI(temp)+1, ORI(end->back));
				strcat(ORI(temp), ")/");
				if( OT(end) == 3 )
				{
					strcat(ORI(temp), "(");
					strcat(ORI(temp), ORI(end));
					strcat(ORI(temp), ")");
				}
				else
					strcat(ORI(temp), ORI(end));
				OT(temp) = 4;
			}
			else
			{
				strcpy(ORI(temp), ORI(end->back));
				strcat(ORI(temp), "/");
				if( OT(end) == 3 )
				{
					strcat(ORI(temp), "(");
					strcat(ORI(temp), ORI(end));
					strcat(ORI(temp), ")");
				}
				else
					strcat(ORI(temp), ORI(end));
				OT(temp) = 4;
			}
			strcpy(DER(temp), "(");
			if(DT(end->back) == 3)
			{
				strcat(DER(temp), "(");
				strcat(DER(temp), DER(end->back));
				strcat(DER(temp), ")*");
				if(OT(end) == 3)
				{
					strcat(DER(temp), "(");
					strcat(DER(temp), ORI(end));
					strcat(DER(temp), ")");
				}
				else
					strcat(DER(temp), ORI(end));
				DT(temp) = 4;
			}
			else if(DT(end->back) != 1 || (strcmp("0",DER(end->back)) && strcmp("1",DER(end->back))) )
			{
				strcat(DER(temp), DER(end->back));
				strcat(DER(temp), "*");
				if(OT(end) == 3)
				{
					strcat(DER(temp), "(");
					strcat(DER(temp), ORI(end));
					strcat(DER(temp), ")");
				}
				else
					strcat(DER(temp), ORI(end));
				DT(temp) = 4;
			}
			else if(!strcmp("1",DER(end->back)))
				strcat(DER(temp), ORI(end));
			if( strcmp("0",DER(end->back)) && strcmp("0",DER(end)) )
				strcat(DER(temp), "-");
			if(DT(end) == 3)
			{
				strcat(DER(temp), "(");
				strcat(DER(temp), DER(end));
				strcat(DER(temp), ")*");
				if(OT(end->back) == 3)
				{
					strcat(DER(temp), "(");
					strcat(DER(temp), ORI(end->back));
					strcat(DER(temp), ")");
				}
				else
					strcat(DER(temp), ORI(end->back));
				DT(temp) = 4;
			}
			else if(DT(end) != 1 || (strcmp("0",DER(end)) && strcmp("1",DER(end))) )
			{
				strcat(DER(temp), DER(end));
				strcat(DER(temp), "*");
				if(OT(end->back) == 3)
				{
					strcat(DER(temp), "(");
					strcat(DER(temp), ORI(end->back));
					strcat(DER(temp), ")");
				}
				else
					strcat(DER(temp), ORI(end->back));
				DT(temp) = 4;
			}
			else if(!strcmp("1",DER(end)))
				strcat(DER(temp), ORI(end->back));
			strcat(DER(temp), ")/");
			if( OT(end) == 3 || OT(end) == 4 )
			{
				strcat(DER(temp), "(");
				strcat(DER(temp), ORI(end));
				strcat(DER(temp), ")");
			}
			else
				strcat(DER(temp), ORI(end));
			strcat(DER(temp), "^2");
			fr = end;
			end = end->back;
			free(fr);
			fr = end;
			end = end->back;
			free(fr);
			temp->back = end;
			end = temp;
		}
		
		
		else if( str[i] == '^' )
		{
			temp = malloc(sizeof(struct stack));
			ORI(temp) = malloc(strlen(ORI(end->back))+strlen(ORI(end))+5);
			DER(temp) = malloc(strlen(DER(end->back))+strlen(DER(end))+5);
			if(OT(end->back) == 1 || OT(end->back) == 2)
			{
				strcpy(ORI(temp), ORI(end->back));
				strcat(ORI(temp), "^");
				strcat(ORI(temp), ORI(end));
			}
			else
			{
				strcpy(ORI(temp), "(");
				strcat(ORI(temp), ORI(end->back));
				strcat(ORI(temp), ")^");
				strcat(ORI(temp), ORI(end));
			}
			OT(temp) = 4;
			if( !strcmp("0", DER(end->back)) )
			{
				strcpy(DER(temp), "0");
				DT(temp) = 1;
			}
			else
			{
				strcpy(DER(temp), ORI(end));
				strcat(DER(temp), "*");
				if( DT(end->back) == 3 )
				{
					strcat(DER(temp), "(");
					strcat(DER(temp), DER(end->back));
					strcat(DER(temp), ")");
				}
				else
					strcat(DER(temp), DER(end->back));
				strcat(DER(temp), "*");
				if( OT(end->back) == 3 || ( OT(temp) == 4 && strcmp("2", ORI(end)) ) )
				{
					strcat(DER(temp), "(");
					strcat(DER(temp), ORI(end->back));
					strcat(DER(temp), ")");
				}
				else
					strcat(DER(temp), ORI(end->back));
				if( strcmp("2", ORI(end)) )
				{
					strcat(DER(temp), "^");
					strcat(DER(temp), dec(ORI(end)));
				}
				
			}
			fr = end;
			end = end->back;
			free(fr);
			fr = end;
			end = end->back;
			free(fr);
			temp->back = end;
			end = temp;
		}
		else
		{
			temp = arrange(str,i);
			temp->back = end;
			end = temp;
			if( OT(temp) == 1 )
				i += strlen(ORI(temp)) + 1;
		}
		i++;
	}
	return DER(end);
}




char * postfix(char *p, int size)
{
	char arr[MAX], *newp;
	int i, ar_size=0, np_size=0;
	newp = malloc(MAX);
	for( i = 0 ; i < size ; i++ )
	{
		if( p[i] == 'X' )
			newp[np_size++] = 'X';
		else if( p[i] == 's' && p[i+1] == 'i' )
		{
			newp[np_size++] = 's';
			i += 2;
		}
		else if( p[i] == 's' )
		{
			newp[np_size++] = 'a';
			i++;
		}
		else if( p[i] == 'c' )
		{
			newp[np_size++] = 'e';
			i++;
		}
		else if( p[i] == 't' )
		{
			newp[np_size++] = 't';
			i += 2;
		}
		else if( p[i] == 'l' )
		{
			newp[np_size++] = 'l';
			i++;
		}
		else if( p[i] < 58 && p[i] > 47 )
		{
			newp[np_size++] = '.';
			newp[np_size++] = p[i];
			while( p[i+1] < 58 && p[i+1] > 47 )
			{
				newp[np_size++] = p[i];
				i++;
			}
			newp[np_size++] = '.';
		}
		else if( p[i] == '^' )
			arr[ar_size++] = '^';
		else if( p[i] == '+' || p[i] == '-' )
		{
			while( ar_size != 0 && arr[ar_size-1] != '(' )
				newp[np_size++] = arr[--ar_size];
			arr[ar_size++] = p[i];
		}
		else if( p[i] == '(' )
			arr[ar_size++] = '(';
		else if( p[i] == '*' || p[i] == '/' )
		{
			while( ar_size != 0 && arr[ar_size-1] != '(' && arr[ar_size-1] != '+' && arr[ar_size-1] != '-' )
				newp[np_size++] = arr[--ar_size];
			arr[ar_size++] = p[i];
		}
		else
		{
			while( arr[ar_size-1] != '(' )
				newp[np_size++] = arr[--ar_size];
			ar_size--;
		}
	}
	for( i = ar_size - 1 ; i >= 0 ; i-- )
		newp[np_size++] = arr[i];
	newp[np_size] = 0;
	return newp;
}

int main()
{
	char c, *str;
	int size = 0,i;
	str = malloc(MAX);
	while( (c = getchar()) != EOF )
		if( c != '\n' && c != ' ' )
			str[size++] = c;
	str[size] = 0;
	str = postfix(str, size);
	
	str = alg(str, strlen(str));
	size = strlen(str);
	for( i = 0 ; i < size ; i++ )
	{
		if( str[i] == 'a' )
			printf("sh");
		else if( str[i] == 'e' )
			printf("ch");
		else if( str[i] == 't' )
			printf("tan");
		else if( str[i] == 'l' )
			printf("ln");
		else if( str[i] == 's' )
			printf("sin");
		else if( str[i] == 'c' )
			printf("cos");
		else
			printf("%c",str[i]);
	}
	return 0;
}
