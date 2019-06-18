#include "bentley_final.hpp"
int main(int argc,char *argv[])
{
	if(argc > 1)
	{
		freopen(argv[1],"r",stdin);
		freopen(argv[2],"w",stdout);
	}
	else
	{
		freopen("input.txt","r",stdin);
		freopen("outputb.txt","w",stdout);	
	}
	Bentley ob;
	ob.run();
	
}