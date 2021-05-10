 /**
  * Üretici - tüketici örneği
  */
 
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include "cekirdek/bayrak.h"

#define OGE_SAYISI 32
#define TAMPON_BOYUTU 8

static int tampon[TAMPON_BOYUTU];
static int tamponBasvuru;

static void tamponaEkle(int oge){
	printf("%d öğesi %d yerine tampona eklendi.\n", oge, tamponBasvuru);
	tampon[tamponBasvuru] = oge;
	tamponBasvuru = (tamponBasvuru+1) % TAMPON_BOYUTU;
}

static int tampondanAl(void){
	int oge;
	oge = tampon[tamponBasvuru];
	printf("%d yerindeki %d öğesi tampondan alındı.\n", tamponBasvuru, oge);
	tamponBasvuru = (tamponBasvuru+1) % TAMPON_BOYUTU;
	return oge;
}

static int ogeUret(void){
	static int oge = 0;
	printf("%d öğesi üretildi:\n",oge);
	return oge++;
}

static void ogeTuket(int oge){
	printf("%d öğesi tüketildi.\n",oge);
}


/* Bayraklar ve görevler */
static bayrak_t dolu, bos;

/* Üretici görevi */
static GOREV(uretici(gorev_t *g)){
	static int uretilen;
	
	GOREV_BASLA(g);
	
	for(uretilen=0; uretilen<OGE_SAYISI; ++uretilen){
		
		BAYRAK_BEKLE(g, &dolu); // Tampon doluysa blokla
		
		tamponaEkle(ogeUret()); // Tampona koy
		
		BAYRAK_IMLE(g, &bos); // Tampon boşalınca bloklanan görevleri uyandır.
	}
	
	GOREV_BITIR(g);
}

/* Tüketici Görevi */
static GOREV(tuketici(gorev_t *g)){
	static int tuketilen;
	
	GOREV_BASLA(g);
	
	for(tuketilen=0; tuketilen<OGE_SAYISI; ++tuketilen){
		
		BAYRAK_BEKLE(g, &bos); // Tampon boşsa blokla
		
		ogeTuket(tampondanAl()); // Tampondaki öğeyi al
		
		BAYRAK_IMLE(g, &dolu); // Tampon dolduğunda bloklanan görevleri uyandır.
	}
	
	GOREV_BITIR(g);
}

/* Sürücü Görev */
static GOREV(surucuGorev(gorev_t *g)){
	static gorev_t gUretici, gTuketici;
	
	GOREV_BASLA(g);
	
	BAYRAK_ILKLE(&bos, 0); // Tampon boş bayrağını ilkle
	BAYRAK_ILKLE(&dolu, TAMPON_BOYUTU); // Tampon dolu bayrağını tampon boyutu kadar ilkle
	
	// Görevleri ilkle
	GOREV_ILKLE(&gUretici);
	GOREV_ILKLE(&gTuketici);
	
	// Yukarıdaki görevlerin bitmesini bekle
	GOREV_BEKLE(g, uretici(&gUretici) & tuketici(&gTuketici));
	
	GOREV_BITIR(g);
}


int main(void){
	gorev_t gSurucu;
	
	GOREV_ILKLE(&gSurucu);
	
	while(GOREV_DIZELGELE(surucuGorev(&gSurucu))){
		/*
		 * Bu örneği bir çoklu görevleme sisteminde çalıştırırken, diğer
		 * süreçlere de çalışması için şans vermeliyiz ve bu yüzden usleep()
		 * ve Sleep() APIlerini burada çağırırız ki işletim sisteminin
		 * dizelgeleyicisi bu prosesi bolklayıp diğer proseslere çalışma
		 * şansı versin. Bir gömülü sistemde genellikle buna gerek duyulmaz.
		 */
#ifdef _WIN32
	Sleep(0);
#else
	usleep(10);
#endif
	}
	return 0;
}
