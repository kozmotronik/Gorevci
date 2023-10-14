/*
 * Copyright (C) 2023 Kozmotronik (İsmail Sahillioğlu)
 * Hiçbir ücret talep edilmeden burada işbu yazılımın bir kopyasını ve 
 * belgelendirme dosyalarını (“Yazılım”) elde eden herkese verilen izin; 
 * kullanma, kopyalama, değiştirme, birleştirme, yayımlama, dağıtma, 
 * alt lisanslama, ve/veya yazılımın kopyalarını satma eylemleri de dahil 
 * olmak üzere ve bununla kısıtlama olmaksızın, yazılımın sınırlama olmadan 
 * ticaretini yapmak için verilmiş olup, bunları yapmaları için yazılımın 
 * sağlandığı kişilere aşağıdakileri yapmak koşuluyla sunulur:
 * 
 * Yukarıdaki telif hakkı bildirimi ve işbu izin bildirimi yazılımın tüm 
 * kopyalarına veya önemli parçalarına eklenmelidir.
 * 
 * YAZILIM “HİÇBİR DEĞİŞİKLİK YAPILMADAN” ESASINA BAĞLI OLARAK, TİCARETE 
 * ELVERİŞLİLİK, ÖZEL BİR AMACA UYGUNLUK VE İHLAL OLMAMASI DA DAHİL VE BUNUNLA 
 * KISITLI OLMAKSIZIN AÇIKÇA VEYA ÜSTÜ KAPALI OLARAK HİÇBİR TEMİNAT OLMAKSIZIN 
 * SUNULMUŞTUR. 
 * HİÇBİR KOŞULDA YAZARLAR VEYA TELİF HAKKI SAHİPLERİ HERHANGİ BİR İDDİAYA, 
 * HASARA VEYA DİĞER YÜKÜMLÜLÜKLERE KARŞI, YAZILIMLA VEYA KULLANIMLA VEYA 
 * YAZILIMIN BAŞKA BAĞLANTILARIYLA İLGİLİ, BUNLARDAN KAYNAKLANAN VE BUNLARIN 
 * SONUCU BİR SÖZLEŞME DAVASI, HAKSIZ FİİL VEYA DİĞER EYLEMLERDEN SORUMLU 
 * DEĞİLDİR.
 */

/**
 * \file
 * \author İsmail Sahillioğlu (Kozmotronik)
 * \copyright \ref lisans "MIT Lisansı"
 * 
 * \ingroup gorev
 * @{
 */

#include "gorev.h"
#include "gorevciypl.h"
#include "port.h"
#include "portmacro.h"

static unsigned int tikSayimi=0;

// Görev yönetici kullanılmak isteniyorsa derlemeye ekle
#if grvCALISMA_KIPI == 1

#ifndef grvMAX_GOREV_SAYISI
#warning Maksimum gorev sayisi MAX_GOREV_SAYISI tanimlanmamis. Varsayilan 8 olacak.
#define grvMAX_GOREV_SAYISI 8
#endif


static gorev_t gorevHavuzu[grvMAX_GOREV_SAYISI];

static unsigned char gorevSayimi = 0;


pgkb_t grvOlustur(is_t is) {
    pgkb_t gkb;
    // Görev listesi dolu, NULL döndür.
    if(gorevSayimi >= grvMAX_GOREV_SAYISI) return NULL;
    /* Aynı iş parçasına sahip bir görev oluşturulmuş mu? */
    for(char s = 0; s < gorevSayimi; s++) {
        if(is == gorevHavuzu[s].is) {
            /* Aynı iş parçasına sahip bir görev zaten var */
            return &gorevHavuzu[s];
        }
    }
    /* Her şey tamam, yeni görevi oluştur ve ilkle */
    gkb = &gorevHavuzu[gorevSayimi];
    gkb->kimlik = gorevSayimi;
    gkb->durum = grvBEKLIYOR;
    gkb->is = is;
    grvILKLE(gkb);
    gorevSayimi++;
    return gkb;
}

void grvBaslat(const unsigned char kimlik) {
    pgkb_t gkb = grvKimlikIleGorevBlogunuAl(kimlik);
    if(gkb == NULL) return;
    grvILKLE(gkb);
    gkb->durum = grvBEKLIYOR;
}

void grvDurdur(const unsigned char kimlik) {
    pgkb_t gkb = grvKimlikIleGorevBlogunuAl(kimlik);
    if(gkb == NULL) return;
    gkb->durum = grvBITTI;
}

void grvGorevciyiBaslat() {
    while(1) {
        for(unsigned char s = 0; s < gorevSayimi; s++) {
            /* Sıradaki Görev Kontrol Bloğunu al */
            pgkb_t gkb = &gorevHavuzu[s];
            /* Bir şekilde NULL olmuş görev kontrol bloğu, sonrakine geç */
            if(gkb == NULL) continue;
            /* Görevin iş parçasını al */
            is_t is = gkb->is;
            /* Bir şekilde NULL olmuş iş parçası, sonrakine geç */
            if(is == NULL) continue;
            /* Görev etkin mi? Değilse sonrakini başlat */
            char durum = gkb->durum;
            if(durum == grvBITTI || durum == grvCIKTI) continue;
            durum = is(gkb); // Sıradaki görevi çalıştır.
            /* Görevin durumunu sakla */
            gkb->durum = durum;
        }
    }
}

pgkb_t grvKimlikIleGorevBlogunuAl(const unsigned char kimlik) {
    if(kimlik >= grvMAX_GOREV_SAYISI) return NULL;
    /* Blok havuzda var öyleyse GKB'na başvuru döndür */
    return &gorevHavuzu[kimlik];
}


#endif

unsigned int grvTikSayimini() {
    unsigned int sayim;
    // Atomik bölge giriş
    portKritikBolumGirisi();
    sayim = tikSayimi;
    // Atomik bölge çıkış
    portKritikBolumCikisi();
    return sayim;
}

/**
 * Bu işlev sistem sürevcisi kesmesinden çağrılır.
 */
void grvTikKesmeIsleyici() {
    tikSayimi++;
}

/// @}
