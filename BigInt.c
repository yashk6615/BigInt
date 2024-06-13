#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 128

typedef struct BIG_INT
{
	int *num;
	int bit_signed; // 1 for negative, 0 for positive
	int len;
} BigInt;



// CONVERTS INPUT STRING TO BIGINT DATA TYPE
BigInt String_BigInt(char *s)
{
	BigInt b;
	int size = strlen(s);
	if (s[0] == '-')
	{
		b.bit_signed = 1;
	}
	else
	{
		b.bit_signed = 0;
	}
	b.len = size - b.bit_signed;
	b.num = (int *)malloc(sizeof(int) * (b.len));
	int *i;
	i = b.num;
	size--;
	while (size >= b.bit_signed)
	{
		*(i) = s[size] - 48; // CONVERTING FROM ASCII TO INTEGER
		i++;
		size--;
	}
	return b;
}

//PRINTS THE BIG INT
void printBigInt(BigInt *b)
{
	int i;
	if (b->bit_signed == 1)
		printf("-");
	for (i = b->len - 1; i >= 0; i--)
		printf("%d", b->num[i]);
}


// COMPARES 2 BIGINT
// RETURNS 0 IF SAME
//		<0	IF B2>B1
//		>0 IF B1>B2
int compare(BigInt *b1, BigInt *b2)
{
	int i, retval;
	if (b1->len == b2->len)
	{
		i = (b1->len) - 1;
		while (i >= 0 && b1->num[i] == b2->num[i])
		{
			i--;
		}
		if (i < 0)
		{
			retval = 0;
		}
		else
		{
			retval = (b1->num[i]) - (b2->num[i]);
		}
	}
	else
	{
		retval = (b1->len) - (b2->len);
	}
	return retval;
}

BigInt *addBigInts(BigInt *b1, BigInt *b2);
BigInt *subtractBigInts(BigInt *b1, BigInt *b2);

// ADDITION OF BIG INT
BigInt *addBigInts(BigInt *b1, BigInt *b2)
{
	int n1 = b1->len;
	int n2 = b2->len;
	int carry;
	BigInt *sum = (BigInt *)malloc(sizeof(BigInt));
	if (n2 > n1)
		sum->num = (int *)malloc(sizeof(int) * (n2 + 1));
	else
		sum->num = (int *)malloc(sizeof(int) * (n1 + 1));

	// IF BOTH THE numBERS ARE OF SAME SIGN
	if (b1->bit_signed == b2->bit_signed)
	{
		sum->bit_signed = b1->bit_signed;

		carry = 0;
		int i = 0, digit;
		while (i < n1 && i < n2)
		{
			digit = (b1->num[i]) + (b2->num[i]) + carry;
			carry = digit / 10;
			digit = digit % 10;
			sum->num[i] = digit;
			i++;
		}
		while (i < n1)
		{
			digit = (b1->num[i]) + carry;
			carry = digit / 10;
			digit = digit % 10;
			sum->num[i] = digit;
			i++;
		}
		while (i < n2)
		{
			digit = (b2->num[i]) + carry;
			carry = digit / 10;
			digit = digit % 10;
			sum->num[i] = digit;
			i++;
		}
		while (carry != 0)
		{
			sum->num[i] = (carry % 10);
			carry /= 10;
			i++;
		}
		sum->len = i;
	}
	// IF BOTH numBERS ARE OF DIFFERENT SIGN
	else
	{
		if (b1->bit_signed == 1)
		{
			b1->bit_signed = 0;
			sum = subtractBigInts(b2, b1);
			b1->bit_signed = 1;
		}
		else
		{
			b2->bit_signed = 0;
			sum = subtractBigInts(b1, b2);
			b2->bit_signed = 1;
		}
	}
	return sum;
}

// SUBTRACTION OF MAGNITUDE
BigInt *subtraction(BigInt *b1, BigInt *b2)
{
	BigInt *diff = (BigInt *)malloc(sizeof(BigInt));
	diff->num = (int *)malloc(sizeof(int) * (b1->len));
	int borrow = 0;
	int i = 0, digit;
	while (i < b2->len)
	{
		digit = (b1->num[i]) - (b2->num[i]) - borrow;
		if (digit < 0)
		{
			borrow = 1;
			digit += 10;
		}
		else
		{
			borrow = 0;
		}
		diff->num[i] = digit;
		i++;
	}
	while (i < b1->len)
	{
		digit = (b1->num[i]) - borrow;
		if (digit < 0)
		{
			borrow = 1;
			digit += 10;
		}
		else
		{
			borrow = 0;
		}
		diff->num[i] = digit;
		i++;
	}
	diff->len = i;
	return diff;
}

// SUBTRACTION
BigInt *subtractBigInts(BigInt *b1, BigInt *b2)
{
	int n1 = b1->len;
	int n2 = b2->len;
	int borrow;
	BigInt *diff = (BigInt *)malloc(sizeof(BigInt));
	if (n2 > n1)
		diff->num = (int *)malloc(sizeof(int) * n2);
	else
		diff->num = (int *)malloc(sizeof(int) * n1);

	// if the signed bits are not eqaul
	if (b1->bit_signed != b2->bit_signed)
	{
		if (b2->bit_signed == 1)
		{
			b2->bit_signed = 0;
			diff = addBigInts(b1, b2);
			b2->bit_signed = 1;
			diff->bit_signed = 0;
		}
		else
		{
			b2->bit_signed = 1;
			diff = addBigInts(b1, b2);
			b2->bit_signed = 0;
			diff->bit_signed = 1;
		}
	}
	else
	{
		if (compare(b1, b2) > 0) // magnitude of b1>b2
		{
			diff = subtraction(b1, b2);		   // subtraction of magnitudes
			diff->bit_signed = b1->bit_signed; // b1-b2------gets sign of b1
		}
		else
		{
			diff = subtraction(b2, b1);
			if (b1->bit_signed == 0) // b1-b2 and b1<b2 ---> diff has opp sign of b1
				diff->bit_signed = 1;
			else
				diff->bit_signed = 0;
		}
	}
	return diff;
}

// MULTIPLICATION
BigInt *multBigInts(BigInt *b1, BigInt *b2)
{
	BigInt *mult = (BigInt *)malloc(sizeof(BigInt));
	mult->len = b1->len + b2->len;
	mult->num = (int *)malloc(sizeof(int) * (mult->len));
	int i, j, carry, prod;

	for (i = 0; i < mult->len; i++) // initialising
		mult->num[i] = 0;

	for (i = 0; i < b1->len; i++)
	{
		carry = 0;
		for (j = 0; j < b2->len; j++)
		{
			prod = (b1->num[i] * b2->num[j]) + carry;
			carry = prod / 10;
			mult->num[i + j] += (prod % 10);
			if (mult->num[i + j] > 9)
			{
				mult->num[i + j + 1] += 1;
				(mult->num[i + j]) %= 10;
			}
		}
		while (carry != 0)
		{
			mult->num[i + j] += (carry % 10);
			if (mult->num[i + j] > 9)
			{
				mult->num[i + j + 1] += 1;
				(mult->num[i + j]) %= 10;
			}
			carry /= 10;
			j++;
		}
	}

	if (b1->bit_signed == b2->bit_signed)
		mult->bit_signed = 0;
	else
		mult->bit_signed = 1;

	i = mult->len - 1;
	while (mult->num[i] == 0)
	{
		i--;
		(mult->len)--;
	}
	if (mult->len == 0) // if input in 0, product is 0
	{
		mult->bit_signed = 0; // if mult by negative number, - sign comes
		mult->len = 1;
	}

	return mult;
}

int main()
{
	BigInt num1, num2;
	char *n1, *n2;
	n1 = (char *)malloc(sizeof(char) * SIZE); // DYNAMIC ALLOCATION OF THE STRINGS
	n2 = (char *)malloc(sizeof(char) * SIZE);

	printf("Enter number 1 : ");
	scanf("%s", n1);
	printf("Enter number 2 : ");
	scanf("%s", n2);

	num1 = String_BigInt(n1); // CONVERTING STRING TO BIG INT
	num2 = String_BigInt(n2);

	BigInt *ans = addBigInts(&num1, &num2);
	printf("SUM OF 2 NUMBERS : ");
	printBigInt(ans);

	ans = subtractBigInts(&num1, &num2);
	printf("\nDIFFERENCE OF 2 NUMBERS : ");
	printBigInt(ans);

	ans = multBigInts(&num1, &num2);
	printf("\nPRODUCT OF 2 NUMBERS : ");
	printBigInt(ans);

	return 0;
}