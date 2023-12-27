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
 * \ingroup uckuyruk
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
    if(uckuyrukOgeSayimi(kuyruk) < nicelik) return 0;

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

char uckuyrukKuyrugaAktar(puck_t kaynak, puck_t hedef) {
    if(kaynak == NULL || hedef == NULL
        || uckuyrukOgeSayimi(kaynak) < 1 || uckuyrukKalanKapasite(hedef) < 1) {
        // NULL veya sınır ihlali
        return 0;
    }
    
    /* Verinin saklanacağı sıradaki konumu hesapla */
    unsigned char kap = hedef->kapasite;
    unsigned char konum = hedef->bas + hedef->sayim;
    if(konum >= kap) konum -= kap;
    /* Veriyi kuyruğa al */
    *( (hedef->tampon) + konum ) = *((kaynak->tampon) + kaynak->bas++);
    /* Sayımı artır ve kuyruğun sayım değişkenine geri sakla */
    hedef->sayim++;
    kaynak->sayim--;
    return 1;
}

unsigned char uckuyrukKuyrugaCokluAktar(
    puck_t kaynak, puck_t hedef, const unsigned char nicelik) {
    if(kaynak == NULL || hedef == NULL || nicelik == 0
        || uckuyrukOgeSayimi(kaynak) < nicelik
        || uckuyrukKalanKapasite(hedef) < nicelik) {
        // NULL veya sınır ihlali veya nicelik 0
        return 0;
    }
    unsigned char s;
    const unsigned char kkap = kaynak->kapasite, hkap = hedef->kapasite;
    unsigned char kbas = kaynak->bas;
    unsigned char konum = hedef->bas + hedef->sayim;
    unsigned char* kaynakTampon = kaynak->tampon;
    unsigned char* hedefTampon = hedef->tampon;
    
    for(s= 0; s < nicelik; s++) {
        // Dairesel taşma olduysa kuyruklamaya baştan devam et
        if(konum >= hkap) konum -= hkap;
        // Dairesel taşma olduysa kuyruktan almaya baştan devam et
        if(kbas >= kkap) kbas = 0;
        /* Veriyi kaynak kuyruktan hedef kuyruğa al */
        *(hedefTampon + konum++) = *(kaynakTampon + kbas++);
    }
    // Kaynak kuyruğun sayım ve bas değerlerini güncelle
    kaynak->sayim -= s;
    kaynak->bas = kbas;
    // Hedef kuyruğun yalnızca sayım değerini güncelle
    hedef->sayim += s;
    return s;
}

char uckuyrukKuyrugaKopyala(puck_t kaynak, puck_t hedef) {
    if(kaynak == NULL || hedef == NULL
        || uckuyrukOgeSayimi(kaynak) < 1 || uckuyrukKalanKapasite(hedef) < 1) {
        // NULL veya sınır ihlali
        return 0;
    }
    
    /* Verinin saklanacağı sıradaki konumu hesapla */
    unsigned char kap = hedef->kapasite;
    unsigned char konum = hedef->bas + hedef->sayim;
    if(konum >= kap) konum -= kap;
    // Kaynak kuyruğun başındaki veriyi kaynaktan çıkarmadan hedefe kopyala.
    *( (hedef->tampon) + konum ) = *((kaynak->tampon) + kaynak->bas);
    /* Sayımı artır ve kuyruğun sayım değişkenine geri sakla */
    hedef->sayim++;
    return 1;
}

unsigned char uckuyrukKuyrugaCokluKopyala(
    puck_t kaynak, puck_t hedef, const unsigned char nicelik) {
    if(kaynak == NULL || hedef == NULL || nicelik == 0
        || uckuyrukOgeSayimi(kaynak) < nicelik
        || uckuyrukKalanKapasite(hedef) < nicelik) {
        // NULL veya sınır ihlali veya nicelik 0
        return 0;
    }
    unsigned char s;
    const unsigned char kkap = kaynak->kapasite, hkap = hedef->kapasite;
    unsigned char kbas = kaynak->bas;
    unsigned char konum = hedef->bas + hedef->sayim;
    unsigned char* kaynakTampon = kaynak->tampon;
    unsigned char* hedefTampon = hedef->tampon;
    
    for(s= 0; s < nicelik; s++) {
        // Dairesel taşma olduysa kuyruklamaya baştan devam et
        if(konum >= hkap) konum -= hkap;
        // Dairesel taşma olduysa kuyruktan almaya baştan devam et
        if(kbas >= kkap) kbas = 0;
        // Kaynak kuyruktan nicelik kadar veriyi çıkarmadan hedefe kopyala.
        *(hedefTampon + konum++) = *(kaynakTampon + kbas++);
    }
    // Hedef kuyruğun yalnızca sayım değerini güncelle
    hedef->sayim += s;
    return s;
}

void uckuyrukBosalt(puck_t kuyruk) {
    unsigned char s = kuyruk->kapasite - 1;
    while(s--) {
        kuyruk->tampon[s] = 0;
    }
    kuyruk->sayim = 0;
    kuyruk->bas = 0;
}

void uckuyrukNBosalt(puck_t kuyruk, unsigned char n) {
    if(kuyruk == NULL || n > uckuyrukOgeSayimi(kuyruk) || n == 0) return;
    if(n == 1) {
        *((kuyruk->tampon) + kuyruk->bas++) = 0;
        kuyruk->sayim--;
    }
    else {
        unsigned char kap = kuyruk->kapasite;
        unsigned char bas = kuyruk->bas;
        unsigned char sayim = kuyruk->sayim;
        unsigned char* tampon = kuyruk->tampon;
        while(n--) {
            *(tampon + bas++) = 0;
            if(bas >= kap) bas -= kap;
            sayim--;
        }
        kuyruk->bas = bas;
        kuyruk->sayim = sayim;
    }
}

unsigned char uckuyrukDoldur(
    puck_t kuyruk, const unsigned char deger, const unsigned char nicelik) {
    if(kuyruk == NULL
        || nicelik > uckuyrukKalanKapasite(kuyruk) || nicelik < 1)
        return 0;
    const unsigned char kap = kuyruk->kapasite;
    unsigned char *hedef = kuyruk->tampon;

    /* Başlanacak konumu hesapla */
    unsigned char konum = kuyruk->bas + kuyruk->sayim;
    unsigned char sayac = nicelik;
    while(sayac--) {
        if(konum >= kap) konum -= kap;
        /* Sıradakini doldur ve sonrakine geç */
        *( hedef + konum++ ) = deger;
    }
    kuyruk->sayim += nicelik;
    return nicelik;
}

unsigned char uckuyrukOgeSayimi(puck_t kuyruk) {
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
