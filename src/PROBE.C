int NG;
int arr[100];
int psx;
void f1()
{
	int cnt;
	for(cnt=0;cnt<NG;cnt++) arr[cnt]=0;
}
void f2()
{
	int cnt;
	for(cnt=0;NG>cnt;cnt++) arr[cnt]=0;
}
void f3()
{
	int cnt;
	for(cnt=0;cnt<NG;cnt++) { arr[cnt]=cnt; psx+=cnt; }
}
