#include "types.h"
#include "stat.h"
#include "user.h"

#define ARRAY_SIZE 2048
int buf[ARRAY_SIZE];

#define EXTRA_SPACE 8182

int getpusz()
{
	uint pa;
	int flag;
	int KERNBASE = 0x80000;

	int count = 0;

	for(int i = 0; i < KERNBASE; i+= 1)
		if(get_va_to_pa(i << 12, &pa, &flag) == 1)
			count += 1;

	return count << 12;
}

void print_status()
{
	int vp;
	int pp;
	int sz;

	sz = getpsz();
	vp = sz / 4096;
	if((sz % 4096) != 0)
		vp += 1;
	pp = getpusz() / 4096;

	printf(1, "sz: %d, vp: %d, pp: %d\n", sz, vp, pp);
}


int
main(int argc, char *argv[])
{	
	buf[ARRAY_SIZE-1] = 1234;

	print_status();

	int * ptr = (int *)&buf[ARRAY_SIZE-1];
	printf(1, "addr: %d, value: %d\n", ((int)ptr), *ptr);

	sbrk(EXTRA_SPACE);

	print_status();

	ptr = (int *)20484;
	printf(1, "addr: %d, value: %d\n", ((int)ptr), *ptr);

	print_status();

	ptr = (int *)28658;
	printf(1, "addr: %d, value: %d\n", ((int)ptr), *ptr);

	print_status();

	ptr = (int *)28668;
	printf(1, "addr: %d, value: %d\n", ((int)ptr), *ptr);

	ptr = (int *)28670;
	printf(1, "addr: %d\n", ((int)ptr));
	printf(1, "value: %d\n", *ptr);

	print_status();
	
	exit();
}