
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "cekirdek/gorev.h"

char cikis, tikSon = 0;

void *tikUreteci(void *v){
	printf("Tik üreteci başladı\n");
	while(1){
		usleep(TIK_SURESI_MS * 1000); // 1s uyu
		sistemTikKesmeServisi(); // Tik sayımını arttır.
		if(tikSon) break;
	}
	printf("Tik üreteci sonlandı\n");
}


GOREV(ledCakarlama1(gorev_t *g)){
	
	GOREV_BASLA(g);
	
	while(1){
		printf("LED 1 Yandı\n");
		GOREV_GECIK_MS(g, 2000);
		printf("LED 1 Söndü\n");
		GOREV_GECIK_MS(g, 4000);

		if(cikis){
			printf("Led çakarlama 1 sonlanıyor\n");
			break;
		}
	}
	
	GOREV_BITIR(g);
}


GOREV(ledCakarlama2(gorev_t *g)){
	
	GOREV_BASLA(g);
	
	while(1){
		printf("LED 2 Yandı\n");
		GOREV_GECIK_MS(g, 500);
		printf("LED 2 Söndü\n");
		GOREV_GECIK_MS(g, 1000);
		printf("LED 2 Yandı\n");
		GOREV_GECIK_MS(g, 2500);
		printf("LED 2 Söndü\n");
		GOREV_GECIK_MS(g, 2000);

		if(cikis){
			printf("Led çakarlama 2 sonlanıyor\n");
			break;
		}
	}
	
	GOREV_BITIR(g);
}


GOREV(karasimsek(gorev_t *g)){
#define hicbiri 0
#define saga 1
#define sola 2
	static char yon=sola, uc=0;
	
	GOREV_BASLA(g);
	
	while(1){
		while(yon == sola){
			printf("Karasimsek uç: %d\n",uc);
			uc++;
			if(uc == 7){ yon = saga; }
			GOREV_GECIK_MS(g, 100);

			if(cikis){
				yon = hicbiri;
				break;
			}
		}
		while(yon == saga){
			printf("Karasimsek uç: %d\n",uc);
			uc--;
			if(uc == 0){ yon = sola; }
			GOREV_GECIK_MS(g, 100);

			if(cikis){
				yon = hicbiri;
				break;
			}
		}

		if(cikis){
			printf("Karaşimşek sonlanıyor\n");
			yon = hicbiri;
			break;
		}
	}
	
	GOREV_BITIR(g);
}

/**
 * Bu görev 40 milisaniyede bir tuş girişini yoklar ve 
 * "x" veya "X" basılmışsa tüm prosesi sonlandırır.
 */
void *girdiAl(void *v){
	printf("Girdi okuyucu başladı\n");
	while(1){
		char c = getchar();
		printf("Girdi: %c\n",c);
		if(c == 'x' || c =='X'){
			cikis = 1;
			printf("Girdi al sonlanıyor\n");
		}
	}
}


int main(){
	gorev_t lc1, lc2, ks, ga;
	GOREV_ILKLE(&lc1);
	GOREV_ILKLE(&lc2);
	GOREV_ILKLE(&ks);
	
	pthread_t tTik, tGirdi;
	int tret = pthread_create(&tTik, NULL, tikUreteci, NULL);
	if(tret){
		printf("Tik üreteci threadi başarısız.\n");
	}
	
	int tret2 = pthread_create(&tGirdi, NULL, girdiAl, NULL);
	if(tret2){
		printf("Girdi okuyucu threadi başarısız.\n");
	}
	
	while(1){
		int d3 = karasimsek(&ks);
		int d1 = ledCakarlama1(&lc1);
		int d2 = ledCakarlama2(&lc2);
		if(d1 == BITTI && d2 == BITTI && d3 == BITTI){
			tikSon = 1;
			break;
		} 
	}
	printf("Ana proses sonlanıyor\n");
}

// 		int don = gecikmeli(&geciken);
// 		switch(don){
// 			case BEKLIYOR:
// 				printf("Beklemede\n");
// 				break;
// 			case CIKTI:
// 				printf("Çıktı\n");
// 				break;
// 			case BITTI:
// 				printf("Bitti\n");
// 				break;
// 			case VAZGECTI:
// 				printf("Vazgeçti\n");
// 				break;
// 		}
// 		sleep(1);
