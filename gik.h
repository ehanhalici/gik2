#include "pngProcess.h"

typedef struct matris
{
	int deger;
	struct matris *assagi;
	struct matris *yukari;
	struct matris *sonraki;
	struct matris *onceki;
} matris;

typedef struct matrisQueue
{
	struct matris *queue;
	struct matrisQueue *sonraki;
} matrisQueue;

int satirOgren(matris *mat)
{
	matris *satir = mat;
	int i = 1;

	while (satir->assagi != NULL)
	{
		satir = satir->assagi;
		i++;
	}

	return i;
}

int sutunOgren(matris *mat)
{
	matris *sutun = mat;
	int i = 1;

	while (sutun->sonraki != NULL)
	{
		sutun = sutun->sonraki;
		i++;
	}

	return i;
}

void baglantiYap(matris *ilk)
{
	matris *ptr, *gecici;
	int satirSayisi = satirOgren(ilk);
	ptr = ilk;
	gecici = ilk;
	gecici = gecici->assagi;
	gecici->yukari = ptr;

	for (int i = 1; i < satirSayisi; i++)
	{
		while (ptr->sonraki != NULL)
		{
			ptr->sonraki->onceki = ptr;
			gecici->sonraki->onceki = gecici;
			ptr = ptr->sonraki;
			gecici = gecici->sonraki;
			ptr->assagi = gecici;
			gecici->yukari = ptr;
		}

		ptr = ilk;
		for (int j = 0; j < i; j++)
		{
			ptr = ptr->assagi;
		}
		gecici = ptr->assagi;
	}
}

matris *matrisOlustur(int satirSayisi, int sutunSayisi)
{
	matris *ilk, *ptr, *gecici;
	ilk = (matris *)malloc(sizeof(matris));
	ilk->deger = 0;
	ilk->yukari = NULL;
	ilk->assagi = NULL;
	ilk->onceki = NULL;
	ilk->sonraki = NULL;
	ptr = ilk;
	//satirOlustur
	for (int i = 1; i < sutunSayisi; i++)
	{
		ptr->sonraki = (matris *)malloc(sizeof(matris));
		ptr = ptr->sonraki;
		ptr->deger = 0;
		ptr->yukari = NULL;
		ptr->assagi = NULL;
		ptr->onceki = NULL;
		ptr->sonraki = NULL;
	}
	//--
	gecici = ilk;
	for (int i = 1; i < satirSayisi; i++)
	{
		gecici->assagi = (matris *)malloc(sizeof(matris));
		gecici = gecici->assagi;
		ptr = gecici;
		ptr->deger = 0;
		ptr->yukari = NULL;
		ptr->assagi = NULL;
		ptr->onceki = NULL;
		ptr->sonraki = NULL;
		//satirOlustur
		for (int i = 1; i < sutunSayisi; i++)
		{
			ptr->sonraki = (matris *)malloc(sizeof(matris));
			ptr = ptr->sonraki;
			ptr->deger = 0;
			ptr->yukari = NULL;
			ptr->assagi = NULL;
			ptr->onceki = NULL;
			ptr->sonraki = NULL;
		}
		//--
	}
	baglantiYap(ilk);
	return ilk;
}

void matToMatris(matris **ilk, png_bytep *row_pointers)
{
	matris *ptr = *ilk;
	matris *gecici = *ilk;
	for (int y = 0; y < height; y++)
	{
		unsigned char *Or = row_pointers[y];
		for (int x = 0; x < width; x++)
		{
			ptr->deger = Or[x];
			ptr = ptr->sonraki;
		}
		gecici = gecici->assagi;
		ptr = gecici;
	}
}

matris *resimOku(char *dosyaYolu)
{

	read_png_file(dosyaYolu);
	setColorGray();
	//kücuk matrısı ustune ekle kı kenarları bos bırakacagız
	matris *ilk = matrisOlustur(height, width);
	matToMatris(&ilk, row_pointers);
	return ilk;
}

//bu fonksiyon libpng nin kullandığı row_pointer global değişkenini düzenler
void matOlustur(int satir, int sutun)
{
	/* row pointer ı temizle */
	for (int y = 0; y < height; y++)
	{
		free(row_pointers[y]);
	}

	width = sutun;
	height = satir;

	row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * satir);

	for (int y = 0; y < satir; y++)
	{
		row_pointers[y] = (png_byte *)malloc(sizeof(png_bytep) * sutun);
	}
}

void matrisToMat(matris *mat)
{
	int satir = satirOgren(mat); //height
	int sutun = sutunOgren(mat); //width
	matris *satirPtr = mat;
	matris *sutunPtr = mat;

	matOlustur(satir, sutun);

	for (int y = 0; sutunPtr->assagi != NULL; y++, sutunPtr = sutunPtr->assagi)
	{

		unsigned char *Or = row_pointers[y];
		for (int x = 0; satirPtr->sonraki != NULL; x++, satirPtr = satirPtr->sonraki)
		{
			Or[x] = satirPtr->deger;
		}

		satirPtr = sutunPtr;
	}
}

void resimYaz(char *yazmaYolu, matris *mat)
{
	matrisToMat(mat);

	write_png_file(yazmaYolu);
}

matris *matrisBuyut(matris *anaMat)
{

	matris *buyukMat = matrisOlustur(2 * satirOgren(anaMat), 2 * sutunOgren(anaMat));

	matris *anaMatYatay = anaMat, *anaMatDikey = anaMat;
	matris *buyukMatYatay = buyukMat, *buyukMatDikey = buyukMat;

	while (anaMatDikey != NULL)
	{
		while (anaMatYatay->sonraki != NULL)
		{
			buyukMatYatay->deger = anaMatYatay->deger;
			buyukMatYatay->sonraki->deger = (anaMatYatay->deger + anaMatYatay->sonraki->deger) / 2;
			buyukMatYatay = buyukMatYatay->sonraki->sonraki;
			anaMatYatay = anaMatYatay->sonraki;
		}
		buyukMatDikey = buyukMatDikey->assagi->assagi;
		buyukMatYatay = buyukMatDikey;
		anaMatDikey = anaMatDikey->assagi;
		anaMatYatay = anaMatDikey;
	}

	buyukMatYatay = buyukMat;
	buyukMatDikey = buyukMat;

	while (buyukMatDikey->assagi->assagi != NULL)
	{
		while (buyukMatYatay->sonraki != NULL)
		{
			buyukMatYatay->assagi->deger = (buyukMatYatay->deger + buyukMatYatay->assagi->assagi->deger) / 2;
			buyukMatYatay = buyukMatYatay->sonraki;
		}
		buyukMatDikey = buyukMatDikey->assagi->assagi;
		buyukMatYatay = buyukMatDikey;
	}

	return buyukMat;
}

matris *matrisKucult(matris *anaMat)
{
	matris *kucukMat = matrisOlustur(satirOgren(anaMat) / 2, sutunOgren(anaMat) / 2);

	matris *kucukMatYatay = kucukMat, *kucukMatDikey = kucukMat;
	matris *anaMatYatay = anaMat, *anaMatDikey = anaMat;

	while (kucukMatDikey != NULL)
	{
		while (kucukMatYatay != NULL)
		{
			kucukMatYatay->deger = anaMatYatay->deger;
			kucukMatYatay = kucukMatYatay->sonraki;
			anaMatYatay = anaMatYatay->sonraki->sonraki;
		}
		anaMatDikey = anaMatDikey->assagi->assagi;
		anaMatYatay = anaMatDikey;
		kucukMatDikey = kucukMatDikey->assagi;
		kucukMatYatay = kucukMatDikey;
	}
	return kucukMat;
}

matris *matrisKes(matris *anaMat, int x1, int x2, int y1, int y2)
{
	matris *kesilenMat = matrisOlustur(y2 - y1, x2 - x1);

	matris *anaMatSatir = anaMat, *anaMatSutun = anaMat;
	matris *kesilenMatSatir = kesilenMat, *kesilenMatSutun = kesilenMat;

	for (int i = 0; i < x1; i++)
	{
		anaMatSatir = anaMatSatir->sonraki;
	}
	for (int i = 0; i < y1; i++)
	{
		anaMatSatir = anaMatSatir->assagi;
	}
	anaMatSutun = anaMatSatir;
	for (int j = y1; j < y2; j++)
	{
		for (int i = x1; i < x2; i++)
		{
			kesilenMatSatir->deger = anaMatSatir->deger;
			anaMatSatir = anaMatSatir->sonraki;
			kesilenMatSatir = kesilenMatSatir->sonraki;
		}
		anaMatSutun = anaMatSutun->assagi;
		anaMatSatir = anaMatSutun;
		kesilenMatSutun = kesilenMatSutun->assagi;
		kesilenMatSatir = kesilenMatSutun;
	}
	return kesilenMat;
}

void elemanEkle(int sayi, matris **ptr, matris **gecici, matris **satirSonu)
{
	if (*ptr == *satirSonu)
	{
		(*ptr)->deger = sayi;
		*gecici = (*gecici)->assagi;
		*ptr = *gecici;
		*satirSonu = (*satirSonu)->assagi;
	}

	else
	{
		(*ptr)->deger = sayi;
		*ptr = (*ptr)->sonraki;
	}
}

void bulaniklastirmaIslemi(matris *ptrSatir, matris *ikinciPtr, matris *maske, matris **ptr, matris **gecici, matris **satirSonu)
{
	int sonuc = 0;
	matris *birinciSatir = ptrSatir;
	matris *birinciSutun = ptrSatir;
	matris *ikinciSatir = ikinciPtr;
	matris *ikinciSutun = ikinciPtr;
	while (ikinciSutun != NULL)
	{
		while (ikinciSatir != NULL)
		{
			sonuc = sonuc + ((ikinciSatir->deger) * (birinciSatir->deger));
			ikinciSatir = ikinciSatir->sonraki;
			birinciSatir = birinciSatir->sonraki;
		}
		ikinciSutun = ikinciSutun->assagi;
		ikinciSatir = ikinciSutun;
		birinciSutun = birinciSutun->assagi;
		birinciSatir = birinciSutun;
	}
	int bol = sutunOgren(maske) * satirOgren(maske);
	sonuc = sonuc / bol;
	//printf("%d\t", sonuc);
	elemanEkle(sonuc, ptr, gecici, satirSonu);
}

void laplasIslemi(matris *ptrSatir, matris *ikinciPtr, matris *maske, matris **ptr, matris **gecici, matris **satirSonu)
{

	int sonuc = 0;
	matris *birinciSatir = ptrSatir;
	matris *birinciSutun = ptrSatir;
	matris *ikinciSatir = ikinciPtr;
	matris *ikinciSutun = ikinciPtr;
	while (ikinciSutun != NULL)
	{
		while (ikinciSatir != NULL)
		{
			sonuc = sonuc + ((ikinciSatir->deger) * (birinciSatir->deger));
			ikinciSatir = ikinciSatir->sonraki;
			birinciSatir = birinciSatir->sonraki;
		}
		ikinciSutun = ikinciSutun->assagi;
		ikinciSatir = ikinciSutun;
		birinciSutun = birinciSutun->assagi;
		birinciSatir = birinciSutun;
	}

	elemanEkle(sonuc, ptr, gecici, satirSonu);
}

void ortancaIslemi(matris *ptrSatir, matris *ikinciPtr, matris *maske, matris **ptr, matris **gecici, matris **satirSonu)
{

	int vectSize = satirOgren(maske) * sutunOgren(maske);
	int vect[vectSize];
	int vectCount = 0;
	matris *birinciSatir = ptrSatir;
	matris *birinciSutun = ptrSatir;
	matris *ikinciSatir = ikinciPtr;
	matris *ikinciSutun = ikinciPtr;
	while (ikinciSutun != NULL)
	{
		while (ikinciSatir != NULL)
		{
			vect[vectCount] = birinciSatir->deger;
			vectCount++;
			ikinciSatir = ikinciSatir->sonraki;
			birinciSatir = birinciSatir->sonraki;
		}
		ikinciSutun = ikinciSutun->assagi;
		ikinciSatir = ikinciSutun;
		birinciSutun = birinciSutun->assagi;
		birinciSatir = birinciSutun;
	}
	for (int i = 0; i < vectSize; i++)
	{
		for (int j = 0; j < vectSize - 1; j++)
		{
			if (vect[j] > vect[j + 1])
			{
				int g = vect[j];
				vect[j] = vect[j + 1];
				vect[j + 1] = g;
			}
		}
	}
	int ortanca = vect[vectSize / 2];
	elemanEkle(ortanca, ptr, gecici, satirSonu);
}

void asindirmaElemanEkle(int sayi, matris **ptr, matris **gecici, matris **satirSonu)
{
	if (sayi == 0)
	{
		if (*ptr == (*satirSonu))
		{
			(*ptr)->deger = sayi;
			*gecici = (*gecici)->assagi;
			(*ptr) = (*gecici);
			(*satirSonu) = (*satirSonu)->assagi;
		}

		else
		{
			(*ptr)->deger = sayi;
			(*ptr) = (*ptr)->sonraki;
		}
	}
	if (sayi == 255)
	{
		if ((*ptr) == (*satirSonu))
		{
			if ((*ptr)->yukari != NULL)
				(*ptr)->yukari->deger = 255;
			if ((*ptr)->assagi != NULL)
				(*ptr)->assagi->deger = 255;
			if ((*ptr)->sonraki != NULL)
				(*ptr)->sonraki->deger = 255;
			if ((*ptr)->onceki != NULL)
				(*ptr)->onceki->deger = 255;
			(*gecici) = (*gecici)->assagi;
			(*ptr) = (*gecici);
			(*satirSonu) = (*satirSonu)->assagi;
		}

		else
		{
			if ((*ptr)->yukari != NULL)
				(*ptr)->yukari->deger = 255;
			if ((*ptr)->assagi != NULL)
				(*ptr)->assagi->deger = 255;
			if ((*ptr)->sonraki != NULL)
				(*ptr)->sonraki->deger = 255;
			if ((*ptr)->onceki != NULL)
				(*ptr)->onceki->deger = 255;
			(*ptr) = (*ptr)->sonraki;
		}
	}
}

void asindirmaIslemi(matris *ptrSatir, matris *ikinciPtr, matris *maske, matris **ptr, matris **gecici, matris **satirSonu)
{
	int asindir = 0;
	matris *birinciSatir = ptrSatir;
	matris *birinciSutun = ptrSatir;
	matris *ikinciSatir = ikinciPtr;
	matris *ikinciSutun = ikinciPtr;
	while (ikinciSutun != NULL)
	{
		while (ikinciSatir != NULL)
		{
			asindir += birinciSatir->deger;
			ikinciSatir = ikinciSatir->sonraki;
			birinciSatir = birinciSatir->sonraki;
		}
		ikinciSutun = ikinciSutun->assagi;
		ikinciSatir = ikinciSutun;
		birinciSutun = birinciSutun->assagi;
		birinciSatir = birinciSutun;
	}

	if (asindir == 0)
	{
		asindirmaElemanEkle(0, ptr, gecici, satirSonu);
	}
	else
	{

		asindirmaElemanEkle(255, ptr, gecici, satirSonu);
	}
}

void genisletmeElemanEkle(int sayi, matris **ptr, matris **gecici, matris **satirSonu)
{
	if (sayi == 0)
	{
		if ((*ptr) == (*satirSonu))
		{
			(*ptr)->deger = sayi;
			(*gecici) = (*gecici)->assagi;
			(*ptr) = (*gecici);
			(*satirSonu) = (*satirSonu)->assagi;
		}

		else
		{
			(*ptr)->deger = sayi;
			(*ptr) = (*ptr)->sonraki;
		}
	}
	if (sayi == 255)
	{
		if ((*ptr) == (*satirSonu))
		{
			(*ptr)->deger = sayi;
			(*gecici) = (*gecici)->assagi;
			(*ptr) = (*gecici);
			(*satirSonu) = (*satirSonu)->assagi;
		}

		else
		{
			(*ptr)->deger = sayi;
			(*ptr) = (*ptr)->sonraki;
		}
	}
}

void genisletmeIslemi(matris *ptrSatir, matris *ikinciPtr, matris *maske, matris **ptr, matris **gecici, matris **satirSonu)
{
	int genislet = 0;
	matris *birinciSatir = ptrSatir;
	matris *birinciSutun = ptrSatir;
	matris *ikinciSatir = ikinciPtr;
	matris *ikinciSutun = ikinciPtr;
	while (ikinciSutun != NULL)
	{
		while (ikinciSatir != NULL)
		{
			if (birinciSatir->deger == 0)
			{
				genislet = 1;
			}

			ikinciSatir = ikinciSatir->sonraki;
			birinciSatir = birinciSatir->sonraki;
		}
		ikinciSutun = ikinciSutun->assagi;
		ikinciSatir = ikinciSutun;
		birinciSutun = birinciSutun->assagi;
		birinciSatir = birinciSutun;
	}

	if (genislet == 1)
	{
		genisletmeElemanEkle(0, ptr, gecici, satirSonu);
	}
	else
	{

		genisletmeElemanEkle(255, ptr, gecici, satirSonu);
	}
}

matris *konvolusyon(int sayi, matris *ana, matris *maske)
{
	//konvolusyon işleminin sonucunu tutar
	matris *konv = matrisOlustur(satirOgren(ana), sutunOgren(ana));

	matris *gecici = konv;
	for (int i = 0; i < satirOgren(maske) / 2; i++)
	{
		gecici = gecici->assagi;
	} //bukadar yukardan basla, 0 lara dokuunamamk icin

	for (int i = 0; i < sutunOgren(maske) / 2; i++)
	{
		gecici = gecici->sonraki;
	} //bukadar soldan basla, 0 lara dokuunamamk icin

	matris *satirSonu = gecici;
	for (int i = 0; i < sutunOgren(ana) - sutunOgren(maske); i++)
	{
		satirSonu = satirSonu->sonraki;
	} //birincinin dogrudan en sonuna degil "0 lara bulasmadan" birnicinin en sona git
	matris *ptr = gecici;

	matris *birinciPtr = ana;
	matris *ikinciPtr = maske;
	matris *ptrSatir = birinciPtr;

	int sagaGit = sutunOgren(ana) - sutunOgren(maske);
	int assagiGit = satirOgren(ana) - satirOgren(maske);
	for (int i = 0; i <= assagiGit; i++)
	{
		for (int j = 0; j <= sagaGit; j++)
		{
			if (sayi == 0)
			{
				bulaniklastirmaIslemi(ptrSatir, ikinciPtr, maske, &ptr, &gecici, &satirSonu);
			}
			else if (sayi == 1)
			{
				laplasIslemi(ptrSatir, ikinciPtr, maske, &ptr, &gecici, &satirSonu);
			}
			else if (sayi == 2)
			{
				ortancaIslemi(ptrSatir, ikinciPtr, maske, &ptr, &gecici, &satirSonu);
			}
			else if (sayi == 3)
			{
				genisletmeIslemi(ptrSatir, ikinciPtr, maske, &ptr, &gecici, &satirSonu);
			}
			else if (sayi == 4)
			{
				asindirmaIslemi(ptrSatir, ikinciPtr, maske, &ptr, &gecici, &satirSonu);
			}
			ptrSatir = ptrSatir->sonraki;
		}
		birinciPtr = birinciPtr->assagi;
		ptrSatir = birinciPtr;
	}
	return konv;
}

matris *maskeOlustur(int dizi[], int x, int y)
{
	matris *maske = matrisOlustur(x, y);
	matris *ptr = maske, *gecici = maske;
	for (int j = 0; j < y; j++)
	{
		for (int i = 0; i < x; i++, ptr = ptr->sonraki)
		{
			ptr->deger = dizi[j * x + i];
		}
		gecici = gecici->assagi;
		ptr = gecici;
	}
	return maske;
}

int esikDegeri(matris *ana)
{
	int histogramSize = 256;
	int histogram[histogramSize];
	for (int i = 0; i < histogramSize; i++)
	{
		histogram[i] = 0;
	}

	matris *satirMat = ana, *sutunMat = ana;
	while (sutunMat != NULL)
	{
		while (satirMat != NULL)
		{

			histogram[satirMat->deger]++;
			satirMat = satirMat->sonraki;
		}
		sutunMat = sutunMat->assagi;
		satirMat = sutunMat;
	}

	double ag1, ag2, ort1 = 0, ort2 = 0, sigma = 0, agtop = 0, toplam = 0, toplam2 = 0;
	unsigned char esik;

	agtop = sutunOgren(ana) * satirOgren(ana);
	for (int j = 0; j < histogramSize; j++)
	{
		toplam2 += j * histogram[j];
	}

	for (int j = 0; j < histogramSize; j++)
	{
		ag1 = 0;
		for (int k = 0; k <= j; k++)
		{
			ag1 += histogram[k];
		}
		ag2 = (agtop - ag1);

		toplam += (double)j * histogram[j];

		ort1 = toplam / ag1;
		ort2 = (toplam2 - toplam) / ag2;

		double sigmakare = ag1 * ag2 * (ort1 - ort2) * (ort1 - ort2);

		if (sigmakare > sigma)
		{
			sigma = sigmakare;
			esik = j;
		}
	}
	return esik;
}

matris *siyahBeyaz(matris *ana)
{
	matris *sb = matrisOlustur(satirOgren(ana), sutunOgren(ana));

	int esik = esikDegeri(ana);
	matris *anaSatir = ana, *anaSutun = ana;
	matris *siyahBeyazSatir = sb, *siyahBeyazSutun = sb;

	while (anaSutun != NULL)
	{
		while (anaSatir != NULL)
		{
			if (anaSatir->deger >= esik)
			{
				siyahBeyazSatir->deger = 255;
			}
			else
			{
				siyahBeyazSatir->deger = 0;
			}
			anaSatir = anaSatir->sonraki;
			siyahBeyazSatir = siyahBeyazSatir->sonraki;
		}
		anaSutun = anaSutun->assagi;
		anaSatir = anaSutun;
		siyahBeyazSutun = siyahBeyazSutun->assagi;
		siyahBeyazSatir = siyahBeyazSutun;
	}
	return sb;
}

matris *bulaniklastirma(matris *mat, matris *maske)
{
	matris *bulaniklastirma = matrisOlustur(satirOgren(mat), sutunOgren(mat));
	bulaniklastirma = konvolusyon(0, mat, maske);
	return bulaniklastirma;
}

matris *laplas(matris *mat, matris *maske)
{
	matris *laplas = matrisOlustur(satirOgren(mat), sutunOgren(mat));
	laplas = konvolusyon(1, mat, maske);
	return laplas;
}

matris *ortanca(matris *mat, matris *maske)
{
	matris *ortanca = matrisOlustur(satirOgren(mat), sutunOgren(mat));
	ortanca = konvolusyon(2, mat, maske);
	return ortanca;
}

matris *keskinlestirmeIslemi(matris *bulanik, matris *mat)
{

	matris *satirBulanik = bulanik, *sutunBulanik = bulanik;
	matris *satirmat = mat, *sutunmat = mat;
	matris *fark = matrisOlustur(satirOgren(mat), sutunOgren(mat));
	matris *satirFark = fark, *sutunFark = fark;

	while (sutunFark != NULL)
	{
		while (satirFark != NULL)
		{

			int sayi = (int)(satirmat->deger - satirBulanik->deger);
			if (sayi < 10)
			{
				sayi = 0;
			}
			satirFark->deger = sayi;
			satirmat = satirmat->sonraki;
			satirBulanik = satirBulanik->sonraki;
			satirFark = satirFark->sonraki;
		}
		sutunBulanik = sutunBulanik->assagi;
		satirBulanik = sutunBulanik;
		sutunmat = sutunmat->assagi;
		satirmat = sutunmat;
		sutunFark = sutunFark->assagi;
		satirFark = sutunFark;
	}
	satirmat = mat;
	sutunmat = mat;
	//fark =SiyahBeyaz(fark);
	satirFark = fark;
	sutunFark = fark;
	//MatrisGoster(fark);
	matris *keskin = matrisOlustur(satirOgren(mat), sutunOgren(mat));
	matris *satirKeskin = keskin, *sutunKeskin = keskin;

	while (sutunmat != NULL)
	{
		while (satirmat != NULL)
		{
			int sayi = (int)(satirmat->deger + satirFark->deger);
			if (sayi > 255)
			{
				sayi = 255;
			}
			satirKeskin->deger = sayi;
			//satirKeskin->deger = satirmat->deger;
			//if (satirFark->deger == 255) { satirKeskin->deger = 255; }
			satirmat = satirmat->sonraki;
			satirFark = satirFark->sonraki;
			satirKeskin = satirKeskin->sonraki;
		}
		sutunFark = sutunFark->assagi;
		satirFark = sutunFark;
		sutunmat = sutunmat->assagi;
		satirmat = sutunmat;
		sutunKeskin = sutunKeskin->assagi;
		satirKeskin = sutunKeskin;
	}
	return keskin;
	//return fark;
}

matris *keskinlestirme(matris *mat, matris *maske)
{
	matris *bulanik = bulaniklastirma(mat, maske);
	matris *keskin = matrisOlustur(satirOgren(mat), sutunOgren(mat));
	keskin = keskinlestirmeIslemi(bulanik, mat);
	return keskin;
}

matris *asindirma(matris *mat, matris *maske)
{
	//asindirma veya genisletme icin siyahbeyaz resim yapmalisin
	//genisletmede ayni resmi kopyala asindirmada koplayama
	matris *mat2 = siyahBeyaz(mat);
	matris *asindirma = matrisOlustur(satirOgren(mat), sutunOgren(mat));
	asindirma = konvolusyon(4, mat2, maske);
	return asindirma;
}

void matKopyala(matris *ana, matris *cikti)
{
	matris *satirAna = ana, *sutunAna = ana, *satirCikti = cikti, *sutunCikti = cikti;

	while (sutunAna != NULL)
	{
		while (satirAna != NULL)
		{
			satirCikti->deger = satirAna->deger;
			satirCikti = satirCikti->sonraki;
			satirAna = satirAna->sonraki;
		}
		sutunAna = sutunAna->assagi;
		satirAna = sutunAna;
		sutunCikti = sutunCikti->assagi;
		satirCikti = sutunCikti;
	}
}

matris *genisletme(matris *mat, matris *maske)
{

	matris *sb = siyahBeyaz(mat);
	matris *genisletme = matrisOlustur(satirOgren(mat), sutunOgren(mat));
	matKopyala(sb, genisletme);
	genisletme = konvolusyon(3, genisletme, maske);
	return genisletme;
}

int stckLengt(matrisQueue **stck)
{
	matrisQueue *ptr = *stck;
	int lengt = 0;
	while (ptr != NULL)
	{
		lengt++;
		ptr = ptr->sonraki;
	}
	return lengt;
}

void eklensin(matrisQueue **stck, matris *ara)
{
	matrisQueue *ptr = *stck;
	for (int i = 0; i < stckLengt(stck); i++)
	{

		if (ptr->queue == ara)
		{
			return;
		}
		ptr = ptr->sonraki;
	}

	if (stckLengt(stck) == 0)
	{
		*stck = (matrisQueue *)malloc(sizeof(matrisQueue));
		(*stck)->queue = ara;
		(*stck)->sonraki = NULL;
	}
	else
	{
		matrisQueue *qptr = *stck;
		while (qptr->sonraki != NULL)
		{
			qptr = qptr->sonraki;
		}

		qptr->sonraki = (matrisQueue *)malloc(sizeof(matrisQueue));
		qptr = qptr->sonraki;
		qptr->queue = ara;
		qptr->sonraki = NULL;
	}
}

int nesneBul(matris *olasiNesne, int nesneSayisi)
{
	//std::vector<matris*> stck;
	matrisQueue *stck;
	stck = (matrisQueue *)malloc(sizeof(matrisQueue));
	//stck.push_back(olasiNesne);
	stck->queue = olasiNesne;
	stck->sonraki = NULL;
	//while (!stck.empty())
	while (stckLengt(&stck) != 0)
	{

		//matris *ara = stck.front();
		matris *ara = stck->queue;
		//stck.erase(stck.begin());
		if (stckLengt(&stck) == 1)
		{
			free(stck);
			stck = NULL;
		}
		else
		{
			matrisQueue *qptr = stck;
			stck = stck->sonraki;
			qptr->queue = NULL;
			free(qptr);
		}

		ara->deger = nesneSayisi;

		if (ara->assagi != NULL)
		{
			if (ara->assagi->deger == 0)
			{
				eklensin(&stck, ara->assagi);
			}
		}
		if (ara->yukari != NULL)
		{
			if (ara->yukari->deger == 0)
			{
				eklensin(&stck, ara->yukari);
			}
		}
		if (ara->sonraki != NULL)
		{
			if (ara->sonraki->deger == 0)
			{
				eklensin(&stck, ara->sonraki);
			}
		}
		if (ara->onceki != NULL)
		{
			if (ara->onceki->deger == 0)
			{
				eklensin(&stck, ara->onceki);
			}
		}
	}
	nesneSayisi += 30;
	return nesneSayisi;
}

matris *siyahBeyazNesneBul(matris *mat, matris *maske)
{
	int nesneSayisi = 30;
	mat = siyahBeyaz(mat);
	int satir = sutunOgren(mat);
	int sutun = satirOgren(mat);
	int yanKay = sutunOgren(maske);
	int assagiKay = satirOgren(maske);
	matris *matPtr = mat, *matSutun = mat;
	for (int i = 0; i < yanKay; i++)
	{
		matPtr = matPtr->sonraki;
	}
	for (int i = 0; i < assagiKay; i++)
	{
		matPtr = matPtr->assagi;
	}
	matSutun = matPtr;

	for (int i = assagiKay; i < sutun - (assagiKay); i++)
	{
		for (int j = yanKay; j < satir - (yanKay); j++)
		{
			if (matPtr->deger == 0)
			{
				nesneSayisi = nesneBul(matPtr, nesneSayisi);
			}
			matPtr = matPtr->sonraki;
		}
		matSutun = matSutun->assagi;
		matPtr = matSutun;
	}
	return mat;
}
