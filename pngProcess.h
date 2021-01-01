#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <png.h>

//piksel sayısını tutar RBG de olsa GRAY de olsa aynıdır
//dizi sayısını tutamz dizi sayısına erişmek için
//RGB de width*3 GRAY de width
int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep *row_pointers;

void read_png_file(char *file_name)
{
	//resim dosyasını açar 
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
	{
		printf("resim dosya işaretçisi ile açılmadı");
		exit(1);
	}

	//dosyanın ilk 8 byet ı onun png olup olmadığını anlamak için
    //kullanılır. Bu byte lar  png_sig_cmp () fonksiyonu ile kontrol
    //edilirler
	unsigned char header[8];
	//1 ve 8 bytlar arasını oku
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
	{
		printf("bu dosya PNG değil");
		exit(1);
	}

	// yapıları başlatır 
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
	{
		printf("read struck yapısı oluşturulamadı");
		exit(1);
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		printf("info struck yapısı oluşturulamadı");
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		exit(1);
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		printf("init_io sırasında hata");
		exit(1);
	}
	png_init_io(png_ptr, fp);
	//dosyayı başta açıp 8 bit okumuştuk bunu bildiriyoruz
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);
	//PNG nin özelliklerini öğren
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	// resmi oku 
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("Error during read_image");
		exit(1);
	}
	
    //    |       
    //    |
    //    V
    //Yukarıdan aşağı dizilecek şekilde pointer dizisi oluşturulur


	row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);

	
    //    |---->       
    //    |----> 
    //    V
	//Yukarıdan aşağı dizilen pointer dizisini 
    //matrise çevirecek işlem yapılır
        
	for (int y = 0; y < height; y++)
		row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));

	
    //ayrılan bu yapıya resim okunur. row_pointers resmin kökü
        
	png_read_image(png_ptr, row_pointers);

	fclose(fp);
}

void write_png_file(char *file_name)
{
	// dosya oluştur 
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
	{
		printf("dosya oluşturulamadı");
		exit(1);
	}

	// yapıları oluştur 
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
	{
		printf("png_create_write_struct işlemi hatalı");
		exit(1);
	}

	info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr)
	{
		printf("png_create_info_struct işlemi hatalı");
		exit(1);
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("init_io işlemi hatalı");
		exit(1);
	}

	png_init_io(png_ptr, fp);

	// başlığı yaz 
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("başlık yazılırken hata");
	}

	png_set_IHDR(png_ptr, info_ptr, width, height,
				 bit_depth, color_type, PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	// baytları yaz 
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("byte lar yazılırken hata");
	}

	png_write_image(png_ptr, row_pointers);
	//png_write_image(png_ptr, gray_pointers);

	// yazmayı bitir. 
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("yazma bitirilirken hata");
	}

	png_write_end(png_ptr, NULL);

	// row pointer ı temizle 
	for (int y = 0; y < height; y++)
		free(row_pointers[y]);
	free(row_pointers);

	fclose(fp);
}

void setColorGray()
{

	//if (!(png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB))
	//{
	//	printf("resim RGB olarak 3 kanallı değil");
	//	exit(1);
	//}

	if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_GRAY)
	{
		return;
	}

	for (int y = 0; y < height; y++)
	{
		png_byte *row = row_pointers[y];

		for (int x = 0; x < width; x++)
		{
			png_byte *ptr = &(row[x * 3]);
			//griye çevir ptr[0] - RED | ptr[1] - GREEN |  ptr[2] - BLUE
			row[x] = (ptr[0] * 0.3 + ptr[1] * 0.59 + ptr[2] * 0.11);
		}
	}

	color_type = PNG_COLOR_TYPE_GRAY;

	//gereksiz bilgiden kurtulmak için temizlik yap
	for (int y = 0; y < height; y++)
	{
		row_pointers[y] = (png_byte *)realloc(row_pointers[y], width);
	}
}