#ifndef News_CPP
#define News_CPP

#define BODYMAX 10000
#define HEADMAX 10000
#define GROUPNAMEMAX 10000

class New
{
	public:
		long IdNew;
		char NewsGroup[GROUPNAMEMAX];
		char Head[HEADMAX];
		char Body[BODYMAX];
		int MAXLENGTH;
};

#endif