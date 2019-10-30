#include "gik.h"


char*  dosyaYolu = (char*)"resim8.png";
char*  yazmaYolu = (char*)"out.png";

int main(int argc, char const *argv[])
{

	system("clear");
	//matris *mat = NULL;
	matris *mat = resimOku( dosyaYolu);
	printf("islem basladi\n");
	//matris* mat = birinci;
	//matris *mat2 = matrisBuyut(mat);

	//matris *mat2 = matrisKucult(mat);

	//int x1 = 30, y1 = 30, x2 = 50, y2 = 50;
	//matris *mat2 = matrisKes(mat, x1, x2, y1, y2);


/*
	int dizi[] =   {0,0,0,0,0,
					0,0,1,0,0,
					0,1,-4,1,0,
					0,0,1,0,0,
					0,0,0,0,0};
*/

	int dizi[] =   {1,1,1,1,1,
					1,1,1,1,1,
					1,1,1,1,1,
					1,1,1,1,1,
					1,1,1,1,1};

	matris *maske = maskeOlustur(dizi,5,5);

	//matris* mat2 = Konvolusyon(0, mat, maske);

	//matris *mat2 = siyahBeyaz(mat);

	//matrisin buyu ve degerleri onemli
	//matris *mat2 = bulaniklastirma(mat,maske);

	//matrisin boyu ve degeri onemli ortaya -8 ver
	//matris *mat2 = laplas(mat,maske);

	//matrisin yalnizca boyu onemli degerler onemli degil anlasılması için matrisi biraz büyüt
	//matris *mat2 = ortanca(mat,maske);


	//bulaniklastirma filitresi kullanildigindan matris boy ve degerleri onemli
	//matris *mat2 = keskinlestirme(mat,maske);
	
	//matris *mat2 = asindirma(mat, maske);
	//matris *mat2 = genisletme(mat, maske);

	matris *mat2 = siyahBeyazNesneBul(mat, maske);
	
	printf("islem tamamlandi\n");
	getchar();

	resimYaz(yazmaYolu, mat2);
	
	return 0;
}