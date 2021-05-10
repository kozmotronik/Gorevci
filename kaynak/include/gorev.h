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

#ifndef GOREV_H
#define GOREV_H

#include "sn.h"

// Porta özel tanımlamaları içeren başlık
#include "portmacro.h"
#include "gorevciypl.h"

// Görev yöneticisi kullanılsın?
#ifndef CALISMA_KIPI
#define CALISMA_KIPI 1
#endif

// Varsayılan tik süresi 1 ms.
#ifndef SISTEM_TIK_SURESI_uS
#define TIK_SURESI_uS	1000u
#else
#define TIK_SURESI_uS   SISTEM_TIK_SURESI_uS
#endif

#define TIK_SURESI_MS   (TIK_SURESI_uS / 1000u)

// Süre - tik - süre dönüşümleri
#define uS_TIK_CEVIR(us) (us / TIK_SURESI_uS)
#define MS_TIK_CEVIR(ms) (ms / TIK_SURESI_MS)
#define TIK_uS_CEVIR(tik) (tik * TIK_SURESI_uS)
#define TIK_MS_CEVIR(tik) (tik * TIK_SURESI_MS)

// Görev durumları.
#define	BEKLIYOR	0
#define	CIKTI		1
#define	BITTI		2
#define	VAZGECTI	3

typedef void *gorevTutucu_t;
typedef char (*is_t)(gorevTutucu_t);

/**
 * Görev Kontrol Bloğu.
 * @sn  Görevin bir sonraki çalışmasında kaldığı yerden sürmesini sağlamak için
 *      sürdürme noktası tutucu.
 * @is  Görevin/işlevin referansını tutan işlev gösterici (function pointer).
 */
#if CALISMA_KIPI == 1
typedef struct{
	sn_t sn;
    is_t is;
} gorev_t;
#else
typedef struct{
	sn_t sn;
} gorev_t;
#endif

/**
 * Görevleri geciktirmek için süre tutucu yapısı.
 * @baslangic   Gecikme istendiği anki sistem tiki sayımını tutar.
 * @kacTik      #GOREV_GECIK_MS makrosuna parametre olarak verilen milisaniye
 *              türnden süre değerinin sistem tiki türünden karşılığını tutar.
 */
typedef struct{
	unsigned int baslangic;
	unsigned int kacTik;
} sure_t;

/**
 * Normalde Görevci tarafından süre takibi yapmak için kullanılır. Nitekim
 * istendiği takdirde bu başlık dosyasını ekleme koşulu ile uygulama
 * içinden de anlık sistem tiki sayımını almak için kullanılabilir.
 * Sistem tiki sayımını atomik olarak okuyup döndürür. Sistem tiki geçen sürenin
 * milisaniye türünden değeri değil, gorevciypl.h dosyasında tanımlanan
 * @SISTEM_TIK_SURESI_us süre değerinin kaç kez oluştuğunu sayan bir birimdir.
 * Diğer bir deyişle sistemin kalp atışının sayısıdır. Ancak mikrosaniye ve 
 * milisaniye birimlerine kolayca dönüştürülebilir. 
 * 
 * @return Anlık sistem tiki sayımı
 */
unsigned int tikSayiminiAl(void);


/****************** Görev yönetim ve organizasyon makroları. ******************/

/**
 * Görevler çalışmaya başlamadan önce bu makro ile ilklenmelidir. Bu yapılmazsa
 * görevler beklendiği gibi çalışmayabilir. Dahili olarak Görevci tarafından
 * kullanılır. Uygulama katmanından direk çağrılmamalıdır.
 * 
 * @param   Parametre olarak @gorevTutucu_t türünde bir tutucu (handle) alır.
 *          Bu, görev kontrol bloğuna bir başvurudur.
 */
#define GOREV_ILKLE(g)	SN_ILKLE(( (gorev_t*) g )->sn)


/**
 * Görevlerin içinde sonsuz döngüye girmeden önce çağrılmalıdır. Bu yapılmazsa
 * derleme hatalarıyla karşılaşılabilir veya görev çalışmaz.
 * 
 * @kullanim    Her görevin parametresi olan @gorevTutucu_t parametresi verilerek
 *              çağrılır:
 *              
 *              GOREV_BASLA(tutucu);
 * 
 * @param  Parametre olarak @gorevTutucu_t türünde bir tutucu (handle) alır.
 *          Bu, görev kontrol bloğuna bir başvurudur.
 */
#define	GOREV_BASLA(g)                                      \
        char VAZGECIS_BAYRAGI = 1;                          \
        SN_BASLAT(( (gorev_t*) g )->sn)


/**
 * Görevlerin içinde sonsuz döngünün bitiminden sonra çağrılmalıdır. Bu yapılmazsa
 * derleme hatalarıyla karşılaşılabilir veya görev çalışmaz.
 * 
 * @kullanim    Her görevin parametresi olan @gorevTutucu_t parametresi verilerek
 *              çağrılır:
 *              
 *              GOREV_BITIR(tutucu);
 * 
 * @param  Parametre olarak @gorevTutucu_t türünde bir tutucu (handle) alır.
 *          Bu, görev kontrol bloğuna bir başvurudur.
 */
#define	GOREV_BITIR(g)	VAZGECIS_BAYRAGI = 0;               \
        SN_KUR(( (gorev_t*) g )->sn)                        \
        return BITTI;                                       \
        SN_BITIR(( (gorev_t*) g )->sn)                      \
        return CIKTI


/**
 * Görevlerin içinde görevin ertelenmesi, geciktirilmesi veya belli bir süre
 * sonra çalıştırılması gereken durumlarda çağrılır. Bu makro çağrıldığında
 * görev, verilen süre kadar bloklanır (artık çalışmaz).
 * 
 * @kullanim    Her görevin parametresi olan @gorevTutucu_t parametresi, süre
 *              takibini yapmak için kullanılacak @sure_t türünden değişkenin
 *              başvurusu ve milisaniye türünden gecikme süresi verilerek
 *              çağrılır:
 * 
 *              static sure_t sureTutucu;
 *              ...
 *              GOREV_GECIK_MS(tutucu, &sureTutucu, 1000);
 * 
 * @param g:    @gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev
 *              kontrol bloğuna bir başvurudur.
 * @param s:    @sure_t türünde bir süre tutucu değişkene başvuru.
 * @param gecikme:  Milisaniye türünden gecikme süresi.
 */
#define GOREV_GECIK_MS(g, s, gecikme)						\
		( (sure_t*) s )->kacTik = MS_TIK_CEVIR(gecikme);	\
		( (sure_t*) s )->baslangic = tikSayiminiAl();		\
		KOSUL_BEKLE( g, ( (tikSayiminiAl() - ( (sure_t*) s )->baslangic) >= ( (sure_t*) s )->kacTik ) )


/**
 * Koşul gerçekleşene dek görevi bloklar.
 * 
 * @kullanim    Her görevin parametresi olan @gorevTutucu_t parametresi ve
 *              koşul ifadesi verilerek çağrılır:
 * 
 *              GOREV_SIFIRLA(tutucu);
 * 
 * @param g:    @gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev
 *              kontrol bloğuna bir başvurudur.
 * @param kosul:    Gerçekleşmesi beklenecek bir koşul belirten ifade.
 */
#define KOSUL_BEKLE(g, kosul)								\
		SN_KUR(( (gorev_t*) g )->sn);						\
		if(!(kosul)){										\
			return BEKLIYOR;								\
		}


/**
 * Koşul sağlanıyorken görevi bloklar.
 * 
 * @kullanim    Her görevin parametresi olan @gorevTutucu_t parametresi ve
 *              koşul ifadesi verilerek çağrılır:
 * 
 *              * Örneğin bir kuyruk API'si olduğunu varsayalım ve aşağıdaki
 *              işlev prototipi basitçe kuyruk doluysa @true değilse @false
 *              döndürüyor.
 *              bool kuyrukDolumu(void)
 *              ...
 *              * Bu durumda kuyrukDolumu() işlevi @false döndürene dek, yani
 *              kuyruk boşalana dek görev bloklanacaktır.
 *              BU_KOSULDA_BEKLE(tutucu, kuyrukDolumu());
 * 
 * @param g:    @gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev
 *              kontrol bloğuna bir başvurudur.
 * @param kosul:    O anda sağlanan ve sağlanmaması için beklenecek bir 
 *                  koşul belirten ifade.
 */
#define	BU_KOSULDA_BEKLE(g, kosul)	KOSUL_BEKLE((g), !(kosul))


/**
 * Görevi sıfırlar (yeniden başlatır). Dolayısıyla görev bir sonraki çalışmada
 * kaldığı yerden değil, en baştan çalışmaya başlar.
 * 
 * @kullanim    Her görevin parametresi olan @gorevTutucu_t parametresi ve
 *              koşul ifadesi verilerek çağrılır:
 * 
 *              GOREV_SIFIRLA(tutucu);
 * 
 * @param g:    @gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev
 *              kontrol bloğuna bir başvurudur.
 */
#define GOREV_SIFIRLA(g)									\
        GOREV_ILKLE((g));									\
		return BEKLIYOR

/**
 * Görevden çıkar. Bir kez çıkış yapıldı mı artık bu görev çalıştırılmak için
 * çağrılmaz. Yenden çağrılması için görevin @gorevOlustur() API'si ile
 * yeniden çalışacak görevler listesine eklenmesi gerekir.
 * 
 * @kullanim    Her görevin parametresi olan @gorevTutucu_t parametresi ve
 *              koşul ifadesi verilerek çağrılır:
 * 
 *              GOREV_CIK(tutucu);
 * 
 * @param g:    @gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev
 *              kontrol bloğuna bir başvurudur.
 */
#define	GOREV_CIK(g)										\
        GOREV_ILKLE((g));									\
		return CIKTI


/**
 * Görevden vazgeçer (yield).
 * 
 * @kullanim    Her görevin parametresi olan @gorevTutucu_t parametresi ve
 *              koşul ifadesi verilerek çağrılır:
 * 
 *              GOREV_VAZGEC(tutucu);
 * 
 * @param g:    @gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev
 *              kontrol bloğuna bir başvurudur.
 */
#define GOREV_VAZGEC(g)										\
        VAZGECIS_BAYRAGI = 0;								\
		SN_KUR(( (gorev_t*) g )->sn);						\
		if(VAZGECIS_BAYRAGI == 0){							\
			return VAZGECTI;								\
		}

/**
 * Belirtilen koşul sağlanana dek görevden vazgeçer.
 * 
 * \kullanim    Her görevin parametresi olan @gorevTutucu_t parametresi ve
 *              koşul ifadesi verilerek çağrılır:
 * 
 *              bool mesajAlindi; // Global bir değişken
 *              ...
 *              GOREV_VAZGEC(tutucu, mesajAlindi);
 * 
 * @param g:    @gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev
 *              kontrol bloğuna bir başvurudur.
 */
#define	KOSULA_DEK_VAZGEC(g, kosul)							\
        VAZGECIS_BAYRAGI = 0;								\
		SN_KUR(( (gorev_t*) g )->sn)						\
		if((VAZGECIS_BAYRAGI) == 0 || !(kosul)){			\
			return VAZGECTI;								\
		}


/**
 * Görevlerin süre takibi yapabilmesi için bu işlevin bir sistem timerı
 * tarafından çağrılması gereklidir.
 */
void sisTikKesmeIsleyici(void);

#if CALISMA_KIPI == 1
/**
 * Görev olarak tanımlanmış işlev referansını alıp yeni bir görev olarak
 * görevler listesine ekler. Görevler uygulamanın main bölümünde bu
 * işlev kullanılarak oluşturulmalıdır.
 * 
 * @param	is_t işi yapacak görev kodu bloğu (bir function pointer)
 * @return	gorev_t türünde görev kontrol bloğuna referans, yoksa NULL
 */
gorev_t *gorevOlustur(is_t);

/**
 * Görevlerin çalışacağı sonsuz döngüyü başlatır. Listedeki herbir görev
 * sırasıyla çalıştırılır. Görevler işbirlikçi (cooperative) olarak çalışırlar,
 * çalışan göreve, kendisi kontrolü teslim edinceye dek müdahale edilmez.
 */
void gorevciyiBaslat(void);

/**
 * İş referansı verilen işe sahip görev kontrol bloğuna referans döndürür.
 * @param	is_t türünde iş bloğuna referans
 * @return	gorev_t türünde görev kontrol bloğuna referans, yoksa NULL
 */
gorev_t *gorevBlogunuAl(is_t);
#endif

#endif
