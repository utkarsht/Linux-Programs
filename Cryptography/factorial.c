#include <stdio.h>
#include <gmp.h>

int main() {

	mpz_t fact;
	mpz_init(fact);
	mpz_set_si(fact, 1);

	mpz_t one;
	mpz_init(one);
	mpz_set_si(one, 1);

	mpz_t num;
	mpz_init(num);
	gmp_scanf("%Zd", &num);

	mpz_t temp;
	mpz_init(temp);
	mpz_set_si(temp, 1);

	while (mpz_cmp(temp, num) != 0) {
		mpz_add(temp, temp, one);
		mpz_mul(fact, fact, temp);
	}

	gmp_printf("%Zd\n", fact);
}
