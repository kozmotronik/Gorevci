/**
 * Görevci v1.0.0
 * Copyright (C) 2021 İsmail Sahillioğlu (aka Kozmotronik). Tüm Hakları Saklıdır.
 * Hiçbir ücret talep edilmeden burada işbu* yazılımın bir kopyasını ve belgelendirme
 * dosyalarını (“Yazılım”) elde eden herkese verilen izin; kullanma, kopyalama,
 * değiştirme, birleştirme, yayımlama, dağıtma, alt lisanslama, ve/veya yazılımın
 * kopyalarını satma eylemleri de dahil olmak üzere ve bununla kısıtlama olmaksızın,
 * yazılımın sınırlama olmadan ticaretini yapmak için verilmiş olup, bunları yapmaları
 * için yazılımın sağlandığı kişilere aşağıdakileri yapmak koşuluyla sunulur:
 * 
 * Yukarıdaki telif hakkı bildirimi ve işbu izin bildirimi yazılımın tüm kopyalarına
 * veya önemli parçalarına eklenmelidir.
 * 
 * YAZILIM “HİÇBİR DEĞİŞİKLİK YAPILMADAN” ESASINA BAĞLI OLARAK, TİCARETE ELVERİŞLİLİK,
 * ÖZEL BİR AMACA UYGUNLUK VE İHLAL OLMAMASI DA DAHİL VE BUNUNLA KISITLI OLMAKSIZIN
 * AÇIKÇA VEYA ÜSTÜ KAPALI OLARAK HİÇBİR TEMİNAT OLMAKSIZIN SUNULMUŞTUR. HİÇBİR KOŞULDA
 * YAZARLAR VEYA TELİF HAKKI SAHİPLERİ HERHANGİ BİR İDDİAYA, HASARA VEYA DİĞER
 * YÜKÜMLÜLÜKLERE KARŞI, YAZILIMLA VEYA KULLANIMLA VEYA YAZILIMIN BAŞKA BAĞLANTILARIYLA
 * İLGİLİ, BUNLARDAN KAYNAKLANAN VE BUNLARIN SONUCU BİR SÖZLEŞME DAVASI, HAKSIZ FİİL
 * VEYA DİĞER EYLEMLERDEN SORUMLU DEĞİLDİR.
 ***************************************************************************************
 * 
 */

#include "gorev.h"
#include "portmacro.h"
#include "gorevciypl.h"

static unsigned int tikSayimi=0;

// Görev yönetici kullanılmak isteniyorsa derlemeye ekle
#if CALISMA_KIPI == 1

#ifndef MAX_GOREV_SAYISI
#warning Maksimum gorev sayisi MAX_GOREV_SAYISI tanimlanmamis. Varsayilan 8 olacak.
#define MAX_GOREV_SAYISI 8
#endif

typedef gorev_t *gorevListeOgesi_t;

static gorev_t gorevHeap[MAX_GOREV_SAYISI];
static gorevListeOgesi_t calisanListesi[MAX_GOREV_SAYISI];

/* Maksimum görev sayısının 1/3 ü kadar CIKTI durumuna geçen görev listesi */
#if MAX_GOREV_SAYISI >= 24
#define MAX_CIKAN_GOREV_SAYISI 6
#elif MAX_GOREV_SAYISI >= 16
#define MAX_CIKAN_GOREV_SAYISI 5
#elif MAX_GOREV_SAYISI >= 12
#define MAX_CIKAN_GOREV_SAYISI 4
#elif MAX_GOREV_SAYISI >= 8
#define MAX_CIKAN_GOREV_SAYISI 3
#elif MAX_GOREV_SAYISI >= 4
#define MAX_CIKAN_GOREV_SAYISI 2
#elif MAX_GOREV_SAYISI >= 2
#define MAX_CIKAN_GOREV_SAYISI 1
#elif MAX_GOREV_SAYISI < 2 
#warning Yalnizca bir gorev icin Gorevciyi kullanmaya degmez. En az iki gorev olmali.
#define MAX_CIKAN_GOREV_SAYISI 1
#endif

static gorevListeOgesi_t cikanListesi[MAX_CIKAN_GOREV_SAYISI];

static char gorevSayimi = 0, calisanSayimi = 0, cikanSayimi = 0;


/* Görev listelerini yönetmek için basit liste işlevsellikleri */
static void listeyeEkle(gorevListeOgesi_t liste[], 
        char *sayim, char max, gorevListeOgesi_t glo) {
    
    const char cSayim = *sayim;
    
    // Liste dolu işlem yapma.
    if(cSayim >= max) return;
    
    liste[cSayim] = glo;
    (*sayim)++;
}

static void listedenSil(gorevListeOgesi_t liste[], 
        char *sayim, const char konum) {
    
    const char cSayim = *sayim;
    
    // Sınır kontrolü yap.
    if(konum >= cSayim || konum == 0) return;
    
    if(konum == (cSayim - 1)) {
        /* Silinmek istenen öğe listenin son öğesi, öyleyse yalnızca sil ve 
         * sayımı azalt */
        liste[konum] = NULL;
        (*sayim)--;
        return;
    }
    // Arada bir öğeyse, sildikten sonra başa doğru hizala
    gorevListeOgesi_t sonrakiOge;
    char k = konum;
    liste[konum] = NULL;
    while(k < cSayim) {
        // Sonraki öğeyi bir önceye al
        liste[k] = liste[k+1];
        k++; // Sonraki satıra geç
    }
    // Hizalama bitince sayımı bir azalt
    (*sayim)--;
}


gorev_t *gorevOlustur(is_t is) {
    gorev_t *gkb;
	// Görev listesi dolu, NULL döndür.
	if(gorevSayimi >= MAX_GOREV_SAYISI) return NULL;
    gkb = &gorevHeap[gorevSayimi++];
	gkb->is = is;
	GOREV_ILKLE(gkb);
    listeyeEkle(calisanListesi, &calisanSayimi, MAX_GOREV_SAYISI, gkb);
	return gkb;
}

void gorevciyiBaslat() {
	while(1) {
		for(char s = 0; s < gorevSayimi; s++) {
			char donus = calisanListesi[s]->is(calisanListesi[s]); // Sıradaki görevi çalıştır.
            if(donus == CIKTI) {
                // Görevi çalışma listesinden çıkar
                
                /* Görev kontrol bloğuna başvuruyu al */
                gorevListeOgesi_t glo = calisanListesi[s];
                
                /* GKB'nu çalışma listesinden sil */
                listedenSil(calisanListesi, &calisanSayimi, s);
                
                /* GKB'nu çıkan listesine ekle */
                listeyeEkle(cikanListesi, &cikanSayimi, MAX_CIKAN_GOREV_SAYISI, glo);
            }
		}
	}
}

gorev_t *gorevBlogunuAl(is_t is) {
    for(int s = 0; s < gorevSayimi; s++) {
        if(gorevHeap[s].is == is) return &gorevHeap[s];
    }
    return NULL;
}
#endif

unsigned int tikSayiminiAl() {
	unsigned int sayim;
	// Atomik bölge giriş
	portKRITIK_BOLUM_GIRISI();
	sayim = tikSayimi;
	// Atomik bölge çıkış
	portKRITIK_BOLUM_CIKISI();
	return sayim;
}

/**
 * Bu işlev sistem sürevcisi kesmesinden çağrılır.
 */
void sisTikKesmeIsleyici() {
	tikSayimi++;
}

