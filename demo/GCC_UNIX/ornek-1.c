

#include "cekirdek/gorev.h"

#include <stdio.h>
#include <unistd.h>

static char bayrak1, bayrak2;

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
		KOSUL_BEKLE(g, bayrak2 != 0);
		printf("Görev 1 çalışıyor.\n");
		
		/* Sonradan diğer görevin bayrağını sıfırlayıp bu görevin
		 * bayrağını kuruyoruz ki diğer görev çalışabilsin. */
		bayrak2 = 0;
		bayrak1 = 1;
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
		// Diğer görev çalışsın.
		bayrak2 = 1;
		
		// Diğer görev bayrağını kurana dek bekle.
		KOSUL_BEKLE(g, bayrak1 != 0);
		printf("Görev 2 çalışıyor.\n");
		
		// Sonra diğer görevin bayrağınız sıfırlarız.
		bayrak1 = 0;
	}
	
	GOREV_BITIR(g);
}

static gorev_t g1, g2;

int main(void){
	// Görev durum değişkenlerini  ILKLE() ile ilkle.
	GOREV_ILKLE(&g1);
	GOREV_ILKLE(&g2);
	
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
