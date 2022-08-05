/**
 * Görevci v1.0.0
 * Copyright (C) 2021 İsmail Sahillioğlu (aka Kozmotronik). Tüm Hakları
 * Saklıdır. Hiçbir ücret talep edilmeden burada işbu* yazılımın bir kopyasını
 * ve belgelendirme dosyalarını (“Yazılım”) elde eden herkese verilen izin;
 * kullanma, kopyalama, değiştirme, birleştirme, yayımlama, dağıtma, alt
 * lisanslama, ve/veya yazılımın kopyalarını satma eylemleri de dahil olmak
 * üzere ve bununla kısıtlama olmaksızın, yazılımın sınırlama olmadan
 * ticaretini yapmak için verilmiş olup, bunları yapmaları için yazılımın
 * sağlandığı kişilere aşağıdakileri yapmak koşuluyla sunulur:
 *
 * Yukarıdaki telif hakkı bildirimi ve işbu izin bildirimi yazılımın tüm
 * kopyalarına veya önemli parçalarına eklenmelidir.
 *
 * YAZILIM “HİÇBİR DEĞİŞİKLİK YAPILMADAN” ESASINA BAĞLI OLARAK, TİCARETE
 * ELVERİŞLİLİK, ÖZEL BİR AMACA UYGUNLUK VE İHLAL OLMAMASI DA DAHİL VE BUNUNLA
 * KISITLI OLMAKSIZIN AÇIKÇA VEYA ÜSTÜ KAPALI OLARAK HİÇBİR TEMİNAT OLMAKSIZIN
 * SUNULMUŞTUR. HİÇBİR KOŞULDA YAZARLAR VEYA TELİF HAKKI SAHİPLERİ HERHANGİ BİR
 * İDDİAYA, HASARA VEYA DİĞER YÜKÜMLÜLÜKLERE KARŞI, YAZILIMLA VEYA KULLANIMLA
 * VEYA YAZILIMIN BAŞKA BAĞLANTILARIYLA İLGİLİ, BUNLARDAN KAYNAKLANAN VE
 * BUNLARIN SONUCU BİR SÖZLEŞME DAVASI, HAKSIZ FİİL VEYA DİĞER EYLEMLERDEN
 * SORUMLU DEĞİLDİR.
 */

/**
 * \ingroup veri
 * @{
 */

#include "uckuyruk.h"

void uckuyrukIlkle(
    const unsigned char kapasite, puck_t kuyruk, unsigned char *const tampon) {
    if(kuyruk == NULL || tampon == NULL) return;
    kuyruk->kapasite = kapasite;
    kuyruk->bas = 0; // Kuyruk başını ilkle
    kuyruk->sayim = 0; // Kuyruk boş
    kuyruk->tampon = tampon; // Uygulamada oluşturulan bir diziye başvuru
}

char uckuyrukKuyrukla(puck_t kuyruk, const unsigned char deger) {
    // Kuyruk doluysa işlem yapma
    if(kuyruk == NULL || uckuyrukDolu(kuyruk)) return 0;
    const unsigned char kap = kuyruk->kapasite;
    /* Verinin saklanacağı sıradaki konumu hesapla */
    unsigned char konum = kuyruk->bas + kuyruk->sayim;
    if(konum >= kap) konum -= kap;
    /* Veriyi kuyruğa al */
    *( (kuyruk->tampon) + konum ) = deger;
    /* Sayımı artır ve kuyruğun sayım değişkenine geri sakla */
    kuyruk->sayim++;
    return 1; // Öğe kuyruklandı
}

unsigned char uckuyrukCokluKuyrukla(
    puck_t kuyruk, const unsigned char *kaynak, const unsigned char nicelik) {
    unsigned char sayac;
    if(kuyruk == NULL || kaynak == NULL || nicelik < 1) return 0;
    /* Kuyrukta yeterince yer var mı? */
    if(uckuyrukKalanKapasite(kuyruk) < nicelik) return 0;
    unsigned char kap = kuyruk->kapasite;
    unsigned char *hedef = kuyruk->tampon;
    /* Verinin saklanacağı sıradaki konumu hesapla */
    unsigned char konum = (kuyruk->bas + kuyruk->sayim);
    /* Eklenmek istenen öğeleri kuyruğa çoğalt */
    sayac = nicelik;
    while(sayac--) {
        if(konum >= kap) konum -= kap;
        /* Sıradakini yaz ve sonraki sıraya geç */
        *( hedef + konum++ ) = *( kaynak++ );
    }
    kuyruk->sayim += nicelik;
    return nicelik; // Öğeler kuyruklandı
}

unsigned char uckuyrukBastakiOge(puck_t kuyruk) {
    return kuyruk == NULL ? 0 : *( (kuyruk->tampon) + (kuyruk->bas) );
}

unsigned char uckuyrukKuyruktanAl(puck_t kuyruk) {
    /* Kuyrukta hiç öğe var mı? */
    if(kuyruk == NULL || uckuyrukBos(kuyruk)) return 0;
    unsigned char bas = kuyruk->bas;
    unsigned char b;
    b = *( (kuyruk->tampon) + bas);
    bas++;
    kuyruk->sayim--;
    if(bas >= kuyruk->kapasite) bas = 0;
    kuyruk->bas = bas;
    return b;
}

unsigned char uckuyrukCokluAl(
    puck_t kuyruk, unsigned char *hedef, const unsigned char nicelik) {
    unsigned char sayac;
    if(kuyruk == NULL || hedef == NULL || nicelik < 1) return 0;
    /* Kuyrukta @nicelik kadar öğe var mı? */
    if(uckuyrukKacOgeVar(kuyruk) < nicelik) return 0;

    const unsigned char kap = kuyruk->kapasite;
    const unsigned char *kaynak = kuyruk->tampon;
    unsigned char bas = kuyruk->bas;
    unsigned char sayim = kuyruk->sayim;

    /* Önce verileri referansı verilen diziye çoğalt */
    for(sayac = 0; sayac < nicelik; sayac++) {
        *( hedef + sayac ) = *( kaynak + bas );
        bas++; sayim--;
        if(bas >= kap) bas = 0;
    }
    kuyruk->bas = bas;
    kuyruk->sayim = sayim;
    return sayac;
}

void uckuyrukBosalt(puck_t kuyruk) {
    unsigned char s = kuyruk->kapasite - 1;
    while(s--) {
        kuyruk->tampon[s] = 0;
    }
    kuyruk->sayim = 0;
    kuyruk->bas = 0;
}

unsigned char uckuyrukDoldur(
    puck_t kuyruk, const unsigned char deger, const unsigned char nicelik) {
    if(kuyruk == NULL
        || nicelik > uckuyrukKalanKapasite(kuyruk) || nicelik < 1)
        return 0;
    const unsigned char kap = kuyruk->kapasite;
    unsigned char *hedef = kuyruk->tampon;

    /* Başlanacak konumu hesapla */
    unsigned char konum = kuyruk->bas + sayim;
    unsigned char sayac = nicelik;
    while(sayac--) {
        if(konum >= kap) konum -= kap;
        /* Sıradakini doldur ve sonrakine geç */
        *( hedef + konum++ ) = deger;
    }
    kuyruk->sayim += nicelik;
    return nicelik;
}

unsigned char uckuyrukKacOgeVar(puck_t kuyruk) {
    return kuyruk == NULL ? 0 : kuyruk->sayim;
}

char uckuyrukDolu(puck_t kuyruk) {
    return kuyruk == NULL ? 1 : (kuyruk->sayim >= kuyruk->kapasite ? 1 : 0);
}

char uckuyrukBos(puck_t kuyruk) {
    return kuyruk == NULL ? 1 : kuyruk->sayim == 0;
}

unsigned char uckuyrukKalanKapasite(puck_t kuyruk) {
    return kuyruk == NULL ? 0 : kuyruk->kapasite - kuyruk->sayim;
}
/// @}
