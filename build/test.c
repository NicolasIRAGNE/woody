#include <unistd.h>

int main()
{
	int i = 0;

	while (i < 10)
		i++;

    write(1, "test\n", 5);
}
