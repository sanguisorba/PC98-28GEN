/* 28GEN REV.2 */

#include <stdio.h>
#include <conio.h>

int bitrev (int);
unsigned char ext[16384];

main()
{
	FILE * fp = NULL;
	int i,j,k,al,add;
	int offsetl;
	int ah[4];
	ah[0]=0x5C;
	ah[1]=0x59;
	ah[2]=0x5A;
	ah[3]=0x5B;
	add=2048;

	/* 漢字アクセスモード操作 */
	outp(0x68,0x8);
	outp(0x68,0xB);
	
	/* 本日の主役 */
	for (offsetl=0x20; offsetl<0x80; offsetl=offsetl+0x20)
	{
		for (j=0; j<4;j++)
		{
			outp(0xA3,ah[j]);
			for (al=0; al<32; al++)
			{
				outp(0xA1,offsetl+al);

				for (i=0; i<16; i++)
				{
					/* 左半分 */
					outp(0xA5,i+32);
					k=inp(0xA9);
					ext[add] = bitrev(k);

					/* 右半分 */
					outp(0xA5,i);
					k=inp(0xA9);
					ext[add+8192] = bitrev(k);
					add++;
				}
			}
		}
	}

	/* 漢字アクセスモードを戻す */
	outp(0x68,0xA);

	/* ファイル書き出し */
	fp = fopen("128.BIN", "wb");
	if(fp == NULL)
	{
		printf("128.BIN の書き込みに失敗しました");
		return 0;
	}
	fwrite(ext, sizeof(unsigned char), 16384, fp);
	fclose(fp);
	
	fp = fopen("256.BIN", "wb");
	if(fp == NULL)
	{
		printf("256.BIN の書き込みに失敗しました");
		return 0;
	}
	fwrite(ext, sizeof(unsigned char), 16384, fp);
	fwrite(ext, sizeof(unsigned char), 16384, fp);
	fclose(fp);

	printf("done!\n");
	return 0;
}

bitrev(k)
{
	/* スタック削減のためループ不使用 */
	/* データのビット桁を反転します */
	/* 7654 3210 -> 0123 4567 */
	/* 例. 0x53 = 0101 0011 -> 1100 1010 = 0xCA */
	int j = 0;
	j += k & 1;
	k = k >> 1;
	j = j << 1;
	j += k & 1;
	k = k >> 1;
	j = j << 1;
	j += k & 1;
	k = k >> 1;
	j = j << 1;
	j += k & 1;
	k = k >> 1;
	j = j << 1;
	j += k & 1;
	k = k >> 1;
	j = j << 1;
	j += k & 1;
	k = k >> 1;
	j = j << 1;
	j += k & 1;
	k = k >> 1;
	j = j << 1;
	j += k & 1;
	return j;
}
