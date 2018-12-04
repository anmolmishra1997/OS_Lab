#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
	int ret;
	int i;


	init_counters();

	for(i=0; i<10; i++)
	{
		ret = fork();
		if (ret == 0)
			break;
	}
	if (ret == 0)
	{
		for(int j=0; j<1000; j++)
		{
			acquire_lock(i);
			set_var(i, get_var(i)+1);
			release_lock(i);
		}
	}
	else
	{
		for(int i=0; i<10; i++)
		{
			for(int j=0; j<1000; j++)
			{
				acquire_lock(i);
				set_var(i, get_var(i)+1);
				release_lock(i);				
			}
		}
	}

	if(ret == 0)
	{
		exit();
	}
	else
	{
		for(int i=0; i<10; i++)
			wait();
		for(int i=0; i<10; i++)
		{
			int val = get_var(i);
			printf(1, "data[%d] = %d\n", i, val);			
		}

		exit();
	}
}