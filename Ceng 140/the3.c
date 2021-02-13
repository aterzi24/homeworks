#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N(x)	((x)->na)
#define T(x)	((x)->type)
#define IA(x)	((x)->is_array)
#define S(x)	((x)->size)
#define NE(x)	((x)->next)
#define P(x)	((x)->position)

typedef struct var{
	char *na;
	char type;			/* 0 for char, 1 for uchar, 2 for int, 3 for uint, 4 for long, 5 for float, 6 for double */
	char is_array;		/* 0: not, 1: 1 dimension, 2: 2 dimensions */
	int size;			/* if it is array, then it holds last dimension length,	otherwise 0 */
	long int position;
	struct var *next;
}var;

FILE *f;

char st_to_type(char *p)			/* returns the type number corresponding to type which is in string form */
{
	if( !strcmp(p,"char") )
		return 0;
	
	if( !strcmp(p,"uchar") )
		return 1;
	
	if( !strcmp(p,"int") )
		return 2;
	
	if( !strcmp(p,"uint") )
		return 3;
	
	if( !strcmp(p,"long") )
		return 4;
	
	if( !strcmp(p,"float") )
		return 5;
	
	if( !strcmp(p,"double") )
		return 6;
		
	return 56;
}

int type_to_size(char t)
{
	if( t == 0 )
		return sizeof(char);
	
	if( t == 1 )
		return sizeof(unsigned char);
	
	if( t == 2 )
		return sizeof(int);
	
	if( t == 3 )
		return sizeof(unsigned int);
	
	if( t == 4 )
		return sizeof(long);
	
	if( t == 5 )
		return sizeof(float);
	
	if( t == 6 )
		return sizeof(double);
	
}

int st_to_int( char *p )
{
	int i, sum, base;
	sum = 0;
	base = 1;
	for( i = strlen(p) - 1 ; i >= 0 ; i-- )
	{
		sum += ( p[i] - '0' ) * base;
		base *= 10;
	}
	return sum;
}

int extract(var *head, char *name)		/* finds the value of variable called name by searching list from head */
{
	int value, cur;
	var *temp;
	temp = head;
	cur = ftell(f);
	
	while(temp)
	{
		if( !strcmp(N(temp), name) )
		{
			fseek(f, P(temp), SEEK_SET);
			fread(&value, sizeof(int), 1, f);
			fseek(f, cur, SEEK_SET);
			return value;
		}
		
		temp = NE(temp);
	}
}

var *func_var(var *head, char *name)
{
	var *temp;
	temp = head;
	while(temp)
	{
		if( !strcmp(N(temp), name) )
			return temp;
		
		temp = NE(temp);
	}
}

void print_func(void *ptr, char t)
{
	if( t == 0 )
		printf("%d\n", *((char *) ptr));
	
	else if( t == 1 )
		printf("%u\n", *((unsigned char *) ptr));
	
	else if( t == 2 )
		printf("%d\n", *((int *) ptr));
	
	else if( t == 3 )
		printf("%u\n", *((unsigned int *) ptr));
	
	else if( t == 4 )
		printf("%ld\n", *((long *) ptr));
	
	else if( t == 5 )
		printf("%f\n", *((float *) ptr));
	
	else if( t == 6 )
		printf("%lf\n", *((double *) ptr));
}

int main()
{
	char *file_name, c;
	char *name = (char *) malloc(25);
	int i, control, s2, s1;
	var *head, *temp;
	void *ptr;
	
	file_name = (char *) malloc(30);
	scanf("%s",file_name);
	c = getchar();					/* prevents code below from reading '\n' after scanf */
	f = fopen(file_name,"r");
	
	/* end of the first line of input */
	control = 1;
	head = NULL;
	
	while( control )
	{
		i = 0;
		while( (c = getchar()) == ' ' ) ;
		name[i++] = c;
		while( (c = getchar()) != ' ' )
			name[i++] = c;
		name[i] = 0;
		
		temp = (var *) malloc(sizeof(var));
		T(temp) = st_to_type(name);
		
		i = 0;
		while( (c = getchar()) == ' ' ) ;
		name[i++] = c;
		while( (c = getchar()) != ',' && c != '\n' && c != '[' )
			name[i++] = c;
		name[i] = 0;
		
		N(temp) = strdup(name);
		P(temp) = ftell(f);
		
		if( c == '\n' )
			control = 0;
		
		if( c != '[' )
		{
			IA(temp) = 0;
			S(temp) = 0;
			fseek(f, type_to_size(T(temp)), SEEK_CUR);
		}
		else
		{
			IA(temp) = 1;
			
			i = 0;
			while( (c = getchar()) != ']' )
				name[i++] = c;
			name[i] = 0;
			
			if( '0' <= name[0] && name[0] <= '9' )
				S(temp) = st_to_int(name);
			else
				S(temp) = extract(head, name);		/* after this function the position of file must remains the same */
			
			c = getchar();
			if( c == '[' )
			{
				IA(temp) = 2;
				
				i = 0;
				while( (c = getchar()) != ']' )
					name[i++] = c;
				name[i] = 0;
				
				if( '0' <= name[0] && name[0] <= '9' )
					s2 = st_to_int(name);
				else
					s2 = extract(head, name);
					
				fseek(f, type_to_size(T(temp))*S(temp)*s2, SEEK_CUR);
				S(temp) = s2;
				
				c = getchar();
			}
			else
				fseek(f, type_to_size(T(temp))*S(temp), SEEK_CUR);
			
			if( c == ' ' )
				while( (c = getchar()) == ' ' ) ;
			
			if( c == '\n' )
				control = 0;
				
		}
		
		NE(temp) = head;
		head = temp;
	}
	/* end of the second line of input */
	control = 1;
	
	while( control )
	{
		i = 0;
		while( (c = getchar()) == ' ' ) ;
		name[i++] = c;
		while( (c = getchar()) != ',' && c != '\n' && c != '[' && c != ' ' )
			name[i++] = c;
		name[i] = 0;
		
		/* printf("%s ",name); */
		
		if( c == ' ' )
			while( (c = getchar()) == ' ' ) ;
		
		if( c == '\n' )
			control = 0;
		
		temp = func_var(head, name);
		if( IA(temp) == 0 )
		{
			fseek(f, P(temp), SEEK_SET);
			fread(ptr, type_to_size(T(temp)), 1, f);
			print_func(ptr, T(temp));
		}
		
		else
		{
			i = 0;
			while( (c = getchar()) != ']' )
				name[i++] = c;
			name[i] = 0;
			
			if( '0' <= name[0] && name[0] <= '9' )
				s1 = st_to_int(name);
			else
				s1 = extract(head, name);
			
			if( (c = getchar()) == '[' )
			{
				i = 0;
				while( (c = getchar()) != ']' )
					name[i++] = c;
				name[i] = 0;
			
				if( '0' <= name[0] && name[0] <= '9' )
					s2 = st_to_int(name);
				else
					s2 = extract(head, name);
					
				fseek(f, P(temp)+type_to_size( T(temp) )*( s1 * S(temp) + s2 ), SEEK_SET);
				fread(ptr, type_to_size(T(temp)), 1, f);
				print_func(ptr, T(temp));
				
				c = getchar();
				
				if( c == ' ' )
					while( (c = getchar()) == ' ' ) ;
				
				if( c == '\n' )
					control = 0;
			}
			else
			{
				fseek(f, P(temp)+type_to_size( T(temp) )*s1, SEEK_SET);
				fread(ptr, type_to_size(T(temp)), 1, f);
				print_func(ptr, T(temp));
				
				if( c == ' ' )
					while( (c = getchar()) == ' ' ) ;
				
				if( c == '\n' )
					control = 0;
			}
		}
	}
	/* end of the third line of input */
	
	
	
	/*		TESTING
	temp = head;
	while(temp)
	{
		printf("type:%d\tname:%s\tis_array:%d\tposition:%d\tsize:%d\n",T(temp),N(temp),IA(temp),P(temp),S(temp));
		temp = NE(temp);
	}
	*/
	
	
	fclose(f);
	return 0;
}
