#include "math.h"

long long pow(long long a, long long b)
{
	long long n = a;
	while (b > 1)
	{
		/**/
		a *= n;
		b--;
	}
	return a;
}

long long mod(long long a, long long b)
{
	/* remainder (stop when this hits 0)*/
	long long rem = a;

	long long x = b;
	/* current exponent*/
	long long currentExp;

	long long result = 0;

	while (rem > 0)
	{
		x = b;

		/* if x > rem (i.e. rem is smaller than a divisor, so it won't be easy)*/
		if (x > rem)
		{
			/* turn the result because not divisible*/
			return rem;
		}

		currentExp = 0;
		while (x <= rem)
		{
			/* bitshift to left so it multiplies by 2*/
			x <<= 1;
			/* we need to count bitshifts here in some way*/
			currentExp++;
			/* bitshifts as exponents of 2 adding to variable??*/
		}
		/* bitshift one back because multiplying by 2 each time makes it be too big*/
		rem -= x >> 1;

		/* add result as exponent of 2 back (-1 because we made one bitshift too much)*/
		result += pow(2, currentExp - 1);

		/* incomplete as of rn*/
	}
	return rem;
}
long long divm(long long a, long long b)
{
	/* remainder (stop when this hits 0)*/
	long long rem = a;

	long long x = b;
	/* current exponent*/
	long long currentExp;

	long long result = 0;

	while (rem > 0)
	{
		x = b;

		/* if x > rem (i.e. rem is smaller than a divisor, so it won't be easy)*/
		if (x > rem)
		{
			/* h*/
			return result;
		}

		currentExp = 0;
		while (x <= rem)
		{
			/* bitshift to left so it multiplies by 2*/
			x <<= 1;
			/* we need to count bitshifts here in some way*/
			currentExp++;
			/* bitshifts as exponents of 2 adding to variable??*/
		}
		/* bitshift one back because multiplying by 2 each time makes it be too big*/
		rem -= x >> 1;

		/* add result as exponent of 2 back (-1 because we made one bitshift too much)*/
		result += pow(2, currentExp - 1);

		/* incomplete as of rn*/
	}
	return result;
}
