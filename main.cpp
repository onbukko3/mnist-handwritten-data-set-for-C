/*#####################################################################################
 * Hand-written recognition
 *
 *
 *
 *
 * 2019
######################################################################################*/

#include <stdio.h>
#include "include/userdef.h"
#include "include/fr.h"
#include "include/bmpinfo.h"

void _bmpProcess(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	int ret = 0;

	printf("Hello Hand-written recognition world\n");

	//ret = _process(argc, argv);
	_bmpProcess(argc, argv);

	return ret;
}