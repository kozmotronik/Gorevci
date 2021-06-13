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
 * Basit bir byte kuyruğu gerçeklemesidir. Kuyruklar uygulamalarda statik olarak
 * oluşturulur, işlemler referanslar üzerinden yapılır. Kuyruklarda bilindiği gibi
 * öğeler kuyruğun sonuna girer, başından alınır. En fazla veri uzunluğu 255'tir.
 * 
 */

#include "uckuyruk.h"

void uckuyrukIlkle(const unsigned char kap,
        uckuyruk_t *kuyruk, unsigned char *veri) {
	if(kuyruk == NULL || veri == NULL) return;
	kuyruk->kapasite = kap;
	kuyruk->bas = 0; // Kuyruk başını ilkle
	kuyruk->sayim = 0; // Kuyruk boş
	kuyruk->veri = veri; // Uygulamada oluşturulan bir diziye başvuru
}

char uckuyrukKuyrukla(uckuyruk_t *kuyruk, const unsigned char veri) {
    // Kuyruk doluysa işlem yapma
	if(kuyruk == NULL || uckuyrukDolu(kuyruk)) return 0;
	unsigned char sayim = kuyruk->sayim;
	const unsigned char kap = kuyruk->kapasite;
	/* Verinin saklanacağı sıradaki konumu hesapla */
	unsigned char konum = kuyruk->bas + sayim;
	if(konum >= kap) konum -= kap;
	/* Veriyi kuyruğa al */
	*( (kuyruk->veri) + konum) = veri;
	/* Sayımı artır ve kuyruğun sayım değişkenine geri sakla */
	kuyruk->sayim = ++sayim;
	return 1; // Öğe kuyruklandı
}

char uckuyrukCokluKuyrukla(uckuyruk_t *kuyruk,
        const unsigned char *v, const unsigned char kac) {
	unsigned char sayac;
	if(kuyruk == NULL || v == NULL) return 0;
	/* Kuyrukta yeterince yer var mı? */
	if(uckuyrukKalanKapasite(kuyruk) < kac) return 0;
    
	unsigned char kap = kuyruk->kapasite;
	unsigned char *veri = kuyruk->veri;
	unsigned char sayim = kuyruk->sayim;
	/* Verinin saklanacağı sıradaki konumu hesapla */
	unsigned char konum = (kuyruk->bas + sayim);
	if(konum >= kap) konum -= kap;
	const char baslangic = konum; // Başlangıç konumunu sakla
    
	/* Eklenmek istenen öğeleri kuyruğa çoğalt */
	for(sayac = 0; sayac < kac; sayac++) {
		*(veri + konum) = *(v + sayac);
		/* Sonraki sıraya geç */
		konum++;
		if(konum >= kap) konum -= kap;
		sayim++;
	}
	kuyruk->sayim = sayim;
	
	return sayac; // Öğeler kuyruklandı
}

unsigned char uckuyrukBastakiOge(uckuyruk_t *kuyruk) {
	return kuyruk == NULL ? 0 : *( (kuyruk->veri) + (kuyruk->bas) );
}

unsigned char uckuyrukKuyruktanAl(uckuyruk_t *kuyruk) {
	/* Kuyrukta hiç öğe var mı? */
	if(kuyruk == NULL || uckuyrukBos(kuyruk)) return 0;
	unsigned char bas = kuyruk->bas;
	unsigned char b;
	
	b = *( (kuyruk->veri) + bas);
	bas++;
	kuyruk->sayim--;
	if(bas >= kuyruk->kapasite) bas = 0;
	kuyruk->bas = bas;
	
	return b;
}

char uckuyrukCokluAl(uckuyruk_t *kuyruk,
        unsigned char *v, const unsigned char kac) {
	unsigned char sayac;
	if(kuyruk == NULL || v == NULL) return 0;
	/* Kuyrukta @kac kadar öğe var mı? */
	if(uckuyrukKacOgeVar(kuyruk) < kac) return 0;
    
	const unsigned char kap = kuyruk->kapasite;
	const unsigned char *veri = kuyruk->veri;
	unsigned char bas = kuyruk->bas;
	unsigned char sayim = kuyruk->sayim;
	
	/* Önce verileri referansı verilen diziye çoğalt */
	for(sayac = 0; sayac < kac; sayac++) {
		*(v+sayac) = *(veri + bas);
		bas++; sayim--;
		if(bas >= kap) bas = 0;
	}
	kuyruk->bas = bas;
	kuyruk->sayim = sayim;
	
	return sayac;
}

unsigned char uckuyrukKacOgeVar(uckuyruk_t *kuyruk) {
	return kuyruk == NULL ? 0 : kuyruk->sayim;
}

char uckuyrukDolu(uckuyruk_t *kuyruk) {
	return kuyruk == NULL ? 1 : (kuyruk->sayim >= kuyruk->kapasite ? 1 : 0);
}

char uckuyrukBos(uckuyruk_t *kuyruk) {
	return kuyruk == NULL ? 1 : kuyruk->sayim == 0;
}

unsigned char uckuyrukKalanKapasite(uckuyruk_t *kuyruk) {
	return kuyruk == NULL ? 0 : kuyruk->kapasite - kuyruk->sayim;
}
