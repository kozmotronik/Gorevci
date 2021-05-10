


#include <stdio.h>
#include <pthread.h> // Sürevciyi öykünmek için
#include <unistd.h>

#include "cekirdek/gorev.h"


void *sistemTikiUreteci(void *v){
	printf("Sistem tiki üreteci başladı\n");
	while(1){
		usleep(TIK_SURESI_MS * 1000); // 1ms uyu
		sistemTikKesmeServisi(); // Sistem tikini artır
	}
}


/**
 * İlk görev işlevi. Görev işlevleri her zaman tamsayı döndürmelidirler.
 * Ancak asla açıkça dönmemelidirler; dönüş görev ifadelerinin içinde
 * gerçeklenmiştir.
 */
static int gorev1(gorev_t *g){
	/* Bir görev ana görev döngüsünden hemen önce her zaman 
	 * görev yapısına başvuru olarak parametre alan BASLA()
	 * ile başlamalıdır. */
	GOREV_BASLA(g);
	
	/* Burada sonsuz döngüde kalırız. */
	while(1){
		printf("Görev 1 çalışıyor.\n");
		GOREV_GECIK_MS(g, 3000);
	}
	
	/* Tüm görev işlevleri görev yapısına başvuru olarak parametre
	 * alan BITIR() ile sonlanmalıdır. */
	GOREV_BITIR(g);
}

/**
 * İkinci görev işlevi. Neredeyse ilki ile bire bir aynıdır.
 */
static int gorev2(gorev_t *g){
	GOREV_BASLA(g);
	
	while(1){
		// Diğer görev bayrağını kurana dek bekle.
		printf("Görev 2 çalışıyor.\n");
		GOREV_GECIK_MS(g, 5000);
	}
	
	GOREV_BITIR(g);
}

static gorev_t g1, g2;

int main(void){
	// Görev durum değişkenlerini  ILKLE() ile ilkle.
	GOREV_ILKLE(&g1);
	GOREV_ILKLE(&g2);
	
	// Sistem tiki üreteci threadini ilkle
	pthread_t tTikUreteci;
	int tret = pthread_create(&tTikUreteci, NULL, sistemTikiUreteci, NULL);
	if(tret){
		printf("Sistem tiki üretici threadi yaratılamadı\n");
	}
	
	/*
	 * Sonrasında iki görevi; görevlere başvuru olarak parametre olarak
	 * geçirdiğimiz işlevlerini yinelemeli olarak çağırıp dizelgeliyoruz
	 * (schedule).
	 */
	while(1){
		gorev1(&g1);
		gorev2(&g2);
		sleep(1);
	}
}
