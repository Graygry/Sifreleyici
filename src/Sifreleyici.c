#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>


char* Random_Anahtar_Olustur(int encbyte)
{
	int i;
	char* return_val;
	return_val = (char*)malloc(encbyte*sizeof(char));
	srand((unsigned) time(NULL));
	for(i=0; i<encbyte;i++)
	{
		return_val[i] = (rand() % 90); //0-90 arasi byte olustur
	}
	return return_val;
}

void HATA_SON() //Syntax hatasi
{
		printf("\n\t\t KULLANIM: Sifreleyici --sifrele [DOSYA] --sif-metod [METOD] --cikti [DOSYA] --eskisil\n");
		exit(0);
}
void HATA_MAX_DOSYA_BOYUTU(int dosyaboyut) //maksimum dosya boyutu hatasi
{
	printf("\n\tSU ANKI DOSYA %d BYTE BOYUTUNDADIR. MAX DOSYA BOYUTUNDAN %d BYTE FAZLADIR.\n",dosyaboyut,(dosyaboyut-1000000000));
		exit(0);
}
void HATA_MIN_DOSYA_BOYUTU(int dosyaboyut) //minimum dosya boyutu hatasi
{
	printf("\n\tSU ANKI DOSYA %d BYTE BOYUTUNDADIR. MINIMUM DOSYA BOYUTU 512 BYTEDIR.\n",dosyaboyut);
		exit(0);
}
void HATA_SON_SIFRECOZ() //sifrecozme hatasi
{
		printf("\n\t\t KULLANIM: Sifreleyici --sifrecoz [DOSYA] --anahatar [ANAHTARDOSYA]\n");
		exit(0);
}

void main(int argc, char* argv[])
{

	if (argv[1] == NULL)
	{
		HATA_SON();
	}
	if(strcmp(argv[1],"--yardim") == 0)
	{
		printf("\n\t\t KULLANIM: Sifreleyici --sifrele [DOSYA] --sifrecoz [ANAHTAR]");
		printf("\t\t Sifreleyici --versiyon = program versiyonunu gosterir.\n");
		printf("\t\t Sifreleyici --yardim = yardimlari gosterir.\n");
		printf("\t\t Sifreleyici --sifrele [DOSYA] = sifrelenecek dosyanin secimi.\n");
		printf("\t\t Sifreleyici --sifrecoz [DOSYA] --anahtar [ANAHTAR] = sifrelenecek dosyanin secimi.\n");
		//printf("\t\t --sif-metod RAST-OTP = Random OTP sifreleme metodu secimi.\n"); //gelecek update icin
		//printf("\t\t --sif-metod BEL-OTP [ANAHTAR] = Belirlenmis anahtar ile sifreleme metodu secimi.\n");//gelecek update icin
	}

	if(strcmp(argv[1],"--versiyon") == 0)
	{
		printf("\n\tSifreleyici v1.0 OTP File Encrypter ©2020 Guray Turkoglu\n");
	}

	FILE *fp, *fp2, *keyfile;
	int i,filelength,keylength;
	char temp_file[512];
	char filename[200];
	char* temp_filep2;
	char* byte_to_encrypt;
    char* byte_to_decrypt;
	char* encryption_keys;
	const int numberwrite = 1048576;
    if(strcmp(argv[1],"--sifrecoz") == 0)
	{
		if(argv[2] == NULL || argv[4] == NULL)
	    {
			HATA_SON_SIFRECOZ();
	    }

		if(strcmp(argv[3],"--anahtar") == 0)
		{
    	   //printf("\n\tDOSYA= %s",argv[2]); //komut argv pointer yazdirma
		   fopen_s(&fp,argv[2], "rb"); //RB binary mod dosya
		   fseek(fp,0,SEEK_END); //dosyanin 0,SEEK_END arası cursor ayarlar
		   filelength = ftell(fp); //dosya pointeri boyutu
		   fclose(fp);		
		   	if (filelength > 1000000000)
			{
				HATA_MAX_DOSYA_BOYUTU(filelength); //EXCEPTION_HATA_2
			}
			if (filelength<512)
		   {
			   HATA_MIN_DOSYA_BOYUTU(filelength); //EXCEPTION_HATA_1
		   }
		   if (filelength>numberwrite+512)
		   {
		   encryption_keys = (char *) malloc(numberwrite*sizeof(char)); //encryption byte allokasyon
		   byte_to_decrypt = (char *) malloc((numberwrite)*sizeof(char));  //decryption anahtari byte allakoasyon
		   temp_filep2 = (char *) malloc((filelength-numberwrite)*sizeof(char)); //hafizada temporary bytenin hafiza adresi allokasyonu
		   //printf("\t BOYUT= %d BYTES",filelength);


			fopen_s(&fp2,argv[2],"rb"); 
			fread(temp_file, 1, 512, fp2); //512 byte temp_file pointer adresine okur
			fseek(fp, 512,SEEK_SET); //512, SEEK_SET arasi cursor cek
			fread(byte_to_decrypt, 1, numberwrite, fp2);
		    fseek(fp, -512,SEEK_CUR); //-512, SEEK_CUR arasi cursor ceker
		    fseek(fp, numberwrite+512,SEEK_SET); //byte replacement için numberwrite+512,SEEK_SET arasi cursor ceker

			 fread(temp_filep2, 1, filelength-numberwrite+512, fp2); //decryption edilecek kismi temp_filep2 pointerine okur ve aktarir
	
	
			fclose(fp2);
		 
			fopen_s(&keyfile,argv[4],"rb");
		    fseek(keyfile, 0, SEEK_END);
			keylength = ftell(keyfile);
		    fseek(keyfile, -keylength, SEEK_CUR);
	        fread(encryption_keys, 1, keylength, keyfile);
			fopen_s(&fp,argv[2],"wb");
			fputs("",fp);
			fclose(fp);
		    fopen_s(&fp,argv[2],"ab");

	    	fwrite(temp_file, 1, 512, fp);
			for(i=0; i<numberwrite; i++)
			{
				byte_to_decrypt[i]=byte_to_decrypt[i]-encryption_keys[i];
			
		    }
				
			fwrite(byte_to_decrypt,1,numberwrite,fp);

		    fwrite(temp_filep2, 1, (filelength-(numberwrite+512)), fp);
			
			free(byte_to_decrypt);
			free(encryption_keys);
			free(temp_filep2);
			fclose(fp);
			printf("\n\t\tSIFRE COZME BASARILI. ISLEM SONU.");
	        //_getch();
		  }

		   if (filelength<=numberwrite+512) //1.5 MB'tan Kucuk dosyalar için bu metod uygulanir
		   {
			encryption_keys = (char *) malloc(filelength*sizeof(char));
		   byte_to_decrypt = (char *) malloc((filelength)*sizeof(char));
		   //printf("\t BOYUT= %d BYTES",filelength);

			fopen_s(&fp2,argv[2],"rb");
			fread(temp_file, 1, 512, fp2);
			fseek(fp, 512,SEEK_SET); 
			fread(byte_to_decrypt, 1, filelength, fp2);
		    fseek(fp, -512,SEEK_CUR); 
		    fseek(fp, filelength+512,SEEK_SET); 

			fclose(fp2);
		 
			fopen_s(&keyfile,argv[4],"rb");
		    fseek(keyfile, 0, SEEK_END);
			keylength = ftell(keyfile);
		    fseek(keyfile, -keylength, SEEK_CUR);
	        fread(encryption_keys, 1, keylength, keyfile);
			fopen_s(&fp,argv[2],"wb");
			fputs("",fp);
			fclose(fp);
		    fopen_s(&fp,argv[2],"ab");

	    	fwrite(temp_file, 1, 512, fp);
			for(i=0; i<filelength-512; i++)
			{
				byte_to_decrypt[i]=byte_to_decrypt[i]-encryption_keys[i];
		    }
			fwrite(byte_to_decrypt,1,filelength-512,fp);
			free(byte_to_decrypt);
			free(encryption_keys);
			fclose(fp);
			printf("\n\t\tSIFRE COZME BASARILI. ISLEM SONU.");
	        //_getch();
		  }
		}	
	}

	if(strcmp(argv[1],"--sifrele") == 0)
	{
		if(argv[2] == NULL)
	    {
			HATA_SON();
	    }
		printf("\n\tDOSYA= %s",argv[2]);
		fopen_s(&fp,argv[2], "rb");
		fseek(fp,0,SEEK_END);
		filelength = ftell(fp);
		printf("\t BOYUT= %d BYTES",filelength);
		fclose(fp);	
		
		if(filelength>1000000000)
		{
			 HATA_MAX_DOSYA_BOYUTU(filelength);
		}
		if (filelength<512)
		{
			   HATA_MIN_DOSYA_BOYUTU(filelength);
		}

		if (filelength>numberwrite+512)
		{
		encryption_keys = (char *) malloc(numberwrite*sizeof(char)); //sifreleme anahtar malloc edilir.
		byte_to_encrypt = (char *) malloc((numberwrite)*sizeof(char)); //sifrelenecek bit malloc edilir
	    temp_filep2 = (char *) malloc((filelength-numberwrite)*sizeof(char)); //dosyanin sifrelenmeyecek kisminin tutuldugu yer malloc edilir.

	     fopen_s(&fp2,argv[2],"rb");
		 fread(temp_file, 1, 512, fp2); //ilk 512 byte oku
		 
		 fseek(fp, 512,SEEK_SET); //512, SEEK_SET arasina cursoru cek
		 fread(byte_to_encrypt, 1, numberwrite, fp2);

		 fseek(fp, -512,SEEK_CUR); //-512, SEEK_CUR arasina cursoru cek
		 fseek(fp, numberwrite+512,SEEK_SET); 
		 fread(temp_filep2, 1, (filelength-(numberwrite+512)), fp2);//fp2 ye sifrelenecek kismi aktar
		 
		 fclose(fp2); //fp2 kapat
		 encryption_keys = Random_Anahtar_Olustur(numberwrite); //boyut kadar random anahtar olusturur
		 //PRINT ENCRYPTED 
		// for(i=0;i<numberwrite;i++)
		 //{ 
		  /// if (i%50 ==0)
		  // {
		//	   printf("\n%c",byte_to_encrypt[i]);
		 //  }
		  // else
		   //{
		   // printf("%c",byte_to_encrypt[i]);
		  //} 
		 //}
		 
		 fopen_s(&fp,argv[2], "wb");
		 fputs("",fp);
		 fclose(fp);
		 fopen_s(&fp,argv[2], "ab");
	     
		 fwrite(temp_file, 1, 512, fp); //ilk 512 byte header degismemesi icin ayni birakir.
		 
		 for(i=0; i<numberwrite; i++) 
		 {
			byte_to_encrypt[i]=(byte_to_encrypt[i]+encryption_keys[i]); //XOR sifreleme islemini yapar.
		 }
		 
		 fwrite(byte_to_encrypt,1,numberwrite,fp); //dosyaya sifreleme yapilmis degiskeni yazar.
		
		 fwrite(temp_filep2, 1, (filelength-(numberwrite+512)), fp); //dosyanin sifrelenmemis kismini header ve sifreleme yapilmis kisminin sonrasina yazar.
		 
		 fclose(fp);
		 free(byte_to_encrypt);
		 printf("\n\t\tISLEM SONU. Anahtari kaydetmek icin bir tusa basin...");
	    //_getch(); //GEREKLI DEGIL
		strcpy_s(filename,200, argv[2]); //sifrelenmis dosyanin ismini alir ve filename degiskenine yazar
		strcat_s(filename,200, ".KEY"); //sifrelenmis dosyanin isminin arkasina .KEY ekleyerek anahtarin yeni adini olusturur.
		fopen_s(&fp,filename,"wb");
		
		fwrite(encryption_keys,1,numberwrite,fp); //anahtari yazar
		fclose(fp);
		free(temp_filep2);
		free(encryption_keys);
		}

		if (filelength<=numberwrite+512) //1.5 MB'tan kucuk dosyalar icin ayni islemi yapar
		{
		encryption_keys = (char *) malloc(filelength*sizeof(char));
		byte_to_encrypt = (char *) malloc((filelength)*sizeof(char));

	     fopen_s(&fp2,argv[2],"rb");
		 fread(temp_file, 1, 512, fp2);
		 
		 fseek(fp, 512,SEEK_SET); 
		 fread(byte_to_encrypt, 1, filelength, fp2);

		 fseek(fp, -512,SEEK_CUR); 
		 fseek(fp, filelength+512,SEEK_SET); 

	
		 fclose(fp2);
		 encryption_keys = Random_Anahtar_Olustur(filelength);
		 //PRINT ENCRYPTED
		// for(i=0;i<numberwrite;i++)
		 //{ 
		  /// if (i%50 ==0)
		  // {
		//	   printf("\n%c",byte_to_encrypt[i]);
		 //  }
		  // else
		   //{
		   // printf("%c",byte_to_encrypt[i]);
		  //} 
		 //}
		 
		 fopen_s(&fp,argv[2], "wb");
		 fputs("",fp);
		 fclose(fp);
		 fopen_s(&fp,argv[2], "ab");
	     
		 fwrite(temp_file, 1, 512, fp);
		 
		for(i=0; i<filelength-512; i++)
			{
				byte_to_encrypt[i]=byte_to_encrypt[i]+encryption_keys[i];
		    }
			fwrite(byte_to_encrypt,1,filelength-512,fp);

		 fclose(fp);
		 free(byte_to_encrypt);
		
		printf("\n\t\tISLEM SONU. Anahtari kaydetmek icin bir tusa basin...");
	    //_getch();
		strcpy_s(filename,200, argv[2]);
		strcat_s(filename,200, ".KEY");
		fopen_s(&fp,filename,"wb");
		fwrite(encryption_keys,1,filelength,fp);
		
		fclose(fp);
		free(encryption_keys);
		}
	}

	}
	


