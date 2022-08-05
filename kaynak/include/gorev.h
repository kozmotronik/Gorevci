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
 * \defgroup gorev Görevci
 * \ingroup gorev
 * @{
 */

#ifndef GOREV_H
#define GOREV_H

#ifndef NULL
#define NULL ( (void*) 0 )
#endif

#include "sn.h"

// Porta özel tanımlamaları içeren başlık
#include "portmacro.h"
#include "gorevciypl.h"

// Görev yöneticisi kullanılsın?
#ifndef grvCALISMA_KIPI
#define grvCALISMA_KIPI 1
#endif

// Varsayılan tik süresi 1 ms.
#ifndef grvSISTEM_TIK_SURESI_uS
#define grvTIK_SURESI_uS	1000u
#else
#define grvTIK_SURESI_uS	grvSISTEM_TIK_SURESI_uS
#endif

#define grvTIK_SURESI_MS	(grvTIK_SURESI_uS / 1000u)

// Süre - tik - süre dönüşümleri
#define grvuS_TIK_CEVIR(us)		(us / grvTIK_SURESI_uS)
#define grvMS_TIK_CEVIR(ms)		(ms / grvTIK_SURESI_MS)
#define grvTIK_uS_CEVIR(tik)	(tik * grvTIK_SURESI_uS)
#define grvTIK_MS_CEVIR(tik)	(tik * grvTIK_SURESI_MS)

// Görev durumları.
#define	grvBEKLIYOR	0
#define	grvCIKTI	1
#define	grvBITTI	2
#define	grvVAZGECTI	3

typedef void *gorevTutucu_t;
typedef char (*is_t)(gorevTutucu_t);

/**
 * Görev Kontrol Bloğu.
 * @kimlik	Görevi yönetmek için bir kimlik bilgisi tutar.
 * @durum	Görevin çalışma durumu bilgisini tutar.
 * @sn	Görevin bir sonraki çalışmasında kaldığı yerden sürmesini sağlamak için
 *		sürdürme noktası tutucu.
 * @is	Görevin/işlevin referansını tutan işlev gösterici (function pointer).
 */
#if grvCALISMA_KIPI == 1
typedef struct{
	unsigned char kimlik;
	char durum;
	sn_t sn;
    is_t is;
} gorev_t;
#else
typedef struct{
	sn_t sn;
} gorev_t;
#endif

typedef gorev_t *pgkb_t;

/**
 * Görevleri geciktirmek için süre tutucu yapısı.
 */
typedef struct{
	/// Gecikme istendiği anki sistem tiki sayımını tutar.
	unsigned int baslangic;
	/// GOREV_GECIK_MS makrosuna parametre olarak verilen milisaniye
	/// türnden süre değerinin sistem tiki türünden karşılığını tutar.
	unsigned int kacTik;
} sure_t;

/**
 * Normalde Görevci tarafından süre takibi yapmak için kullanılır. Nitekim
 * istendiği takdirde bu başlık dosyasını ekleme koşulu ile uygulama
 * içinden de anlık sistem tiki sayımını almak için kullanılabilir.
 * Sistem tiki sayımını atomik olarak okuyup döndürür. Sistem tiki geçen
 * sürenin milisaniye türünden değeri değil, gorevciypl.h dosyasında tanımlanan
 * SISTEM_TIK_SURESI_uS süre değerinin kaç kez oluştuğunu sayan bir birimdir.
 * Diğer bir deyişle sistemin kalp atışının sayısıdır. Ancak mikrosaniye ve 
 * milisaniye birimlerine kolayca dönüştürülebilir. 
 * 
 * \return Anlık sistem tiki sayımı
 */
unsigned int grvTikSayiminiAl(void);


/**
 * Görevleri ilkler, ilk kullanım için hazırlar.
 *
 * Görevler çalışmaya başlamadan önce bu makro ile ilklenmelidir. Bu yapılmazsa
 * görevler beklendiği gibi çalışmayabilir. Dahili olarak Görevci tarafından
 * kullanılır. Uygulama katmanından direk çağrılmamalıdır.
 * 
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \hideinitializer
 */
#define grvILKLE(g) SN_ILKLE(( (gorev_t*) g )->sn)


/**
 * Görev başlangıç noktası.
 *
 * Görevlerin içinde sonsuz döngüye girmeden önce çağrılmalıdır. Bu yapılmazsa
 * derleme hatalarıyla karşılaşılabilir veya görev çalışmaz.
 * Her görevin parametresi olan gorevTutucu_t parametresi verilerek çağrılır:
 * \code
 * grvBASLA(tutucu);
 * \endcode
 * 
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \hideinitializer
 */
#define grvBASLA(g)             \
    char VAZGECIS_BAYRAGI = 1;  \
    SN_BASLAT(( (gorev_t*) g )->sn)


/**
 * Görev bitiş noktası.
 *
 * Görevlerin içinde sonsuz döngünün bitiminden sonra çağrılmalıdır. Bu
 * yapılmazsa derleme hatalarıyla karşılaşılabilir veya görev çalışmaz.
 * Her görevin parametresi olan gorevTutucu_t parametresi verilerek çağrılır:
 * \code
 * grvBITIR(tutucu);
 * \endcode
 * 
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \hideinitializer
 */
#define grvBITIR(g)                 \
    VAZGECIS_BAYRAGI = 0;           \
    SN_KUR(( (gorev_t*) g )->sn)    \
    return grvBITTI;                \
    default: grvSIFIRLA(g);         \
    SN_BITIR(( (gorev_t*) g )->sn)  \
    return grvCIKTI


/**
 * Bir görevi verilen milisaniye gecikme süresi kadar geciktirir.
 *
 * Görevlerin içinde görevin ertelenmesi, geciktirilmesi veya belli bir süre
 * sonra çalıştırılması gereken durumlarda çağrılır. Bu makro çağrıldığında
 * görev, verilen süre kadar bloklanır (artık çalışmaz).
 * Her görevin parametresi olan gorevTutucu_t parametresi, süre takibini yapmak
 * için kullanılacak sure_t türünden değişkenin başvurusu ve milisaniye
 * türünden gecikme süresi verilerek çağrılır:
 * \code
 *	static sure_t sureTutucu;
 *	// ...
 *	grvGECIK_MS(tutucu, &sureTutucu, 1000);
 * \endcode
 * 
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \param s sure_t türünde bir süre tutucu değişkene başvuru.
 * \param gecikme Milisaniye türünden gecikme süresi.
 * \hideinitializer
 */
#define grvGECIK_MS(g, s, gecikme)                                          \
    ( (sure_t*) s )->kacTik = grvMS_TIK_CEVIR(gecikme);                     \
    ( (sure_t*) s )->baslangic = grvTikSayiminiAl();                        \
    grvKOSUL_BEKLE( g, ( (grvTikSayiminiAl() - ((sure_t*) s )->baslangic)   \
        >= ( (sure_t*) s )->kacTik ) )


/**
 * Bir görevi koşullu geciktirir.
 *
 * Görevlerin içinde bir koşula bağlı olarak görevin ertelenmesi,
 * geciktirilmesi veya belli bir süre sonra çalıştırılması gereken durumlarda
 * çağrılır.
 * Bu makro çağrıldığında belirtilen koşul sağlandıkça görev, verilen süre 
 * kadar bloklanır (artık çalışmaz). Herhangi bir gecikme aşamasında koşulun
 * sağlanmadığı saptanır saptanmaz gecikme iptal edilir ve görev kaldığı
 * yerden çalışmayı sürdürür.
 * 
 * Her görevin parametresi olan gorevTutucu_t parametresi, süre takibini yapmak
 * için kullanılacak sure_t türünden değişkenin başvurusu, milisaniye türünden
 * gecikme süresi ve son olarak gecikmenin bağlı çalışacağı koşul verilerek
 * çağrılır:
 * \code
 * static sure_t sureTutucu;
 * static int a;
 * // ...
 * // a değişkeni 0'dan farklı bir değer aldığında gecikme hangi
 * // aşamada olursa olsun görev, bekleme durumundan çıkıp
 * // çalışma durumuna geçecek ve kaldığı yerden devam edecektir.
 * grvKOSULLU_GECIK_MS(tutucu, &sureTutucu, 1000, a == 0);
 * \endcode
 * 
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \param s sure_t türünde bir süre tutucu değişkene başvuru.
 * \param gecikme Milisaniye türünden gecikme süresi.
 * \param kosul Gecikmenin bağlı çalışacağı koşul.
 * \hideinitializer
 */
#define grvKOSULLU_GECIK_MS(g, s, gecikme, kosul)                           \
    ( (sure_t*) s )->kacTik = grvMS_TIK_CEVIR(gecikme);                     \
    ( (sure_t*) s )->baslangic = grvTikSayiminiAl();                        \
    grvKOSUL_BEKLE( g, ( (grvTikSayiminiAl() - ( (sure_t*) s )->baslangic)  \
        >= ( (sure_t*) s )->kacTik ) || !(kosul))

/**
 * Koşul gerçekleşene dek görevi bloklar.
 * 
 * Her görevin parametresi olan @gorevTutucu_t parametresi ve koşul ifadesi
 * verilerek çağrılır:
 * \code
 * int miktar = 0;
 * //...
 * grvKOSUL_BEKLE(tutucu, miktar > 0);
 * \endcode
 *
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \param kosul Gerçekleşmesi beklenecek bir koşul belirten ifade.
 * \hideinitializer
 */
#define grvKOSUL_BEKLE(g, kosul)    \
    SN_KUR(( (gorev_t*) g )->sn);   \
    if(!(kosul)){                   \
        return grvBEKLIYOR;         \
    }


/**
 * Koşul sağlanıyorken görevi bloklar.
 * 
 * Verilen bir koşul sağlandığı sürece görevi bloklar. Koşul başka bir görev
 * tarafından değiştirilip artık sağlanmadığında bloklanan görev kaldığı yerden
 * çalışmayı sürdürür.
 * Örneğin bir kuyruk API'si olduğunu varsayalım ve aşağıdaki işlev prototipi
 * basitçe kuyruk doluysa true değilse false döndürüyor.
 * \code
 * bool kuyrukDolumu(void)
 * \endcode
 * Bu durumda kuyrukDolumu() işlevi false döndürene dek, yani kuyruk boşalana
 * dek görev bloklanacaktır.
 * \code
 * grvBU_KOSULDA_BEKLE(gorevTutucu, kuyrukDolumu());
 * \endcode
 * 
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \param kosul O anda sağlanan ve sağlanmaması için beklenecek bir koşul
 * belirten ifade.
 * \hideinitializer
 */
#define grvBU_KOSULDA_BEKLE(g, kosul)   \
    SN_KUR(( (gorev_t*) g )->sn);       \
    if((kosul)){                        \
        return grvBEKLIYOR;             \
    }


/**
 * Bir görevi sıfırlar (yeniden başlatır).
 *
 * Görevi sıfırlar (yeniden başlatır). Dolayısıyla görev bir sonraki çalışmada
 * kaldığı yerden değil, en baştan çalışmaya başlar.
 * Her görevin parametresi olan gorevTutucu_t parametresi ve koşul ifadesi
 * verilerek çağrılır:
 * \code
 * grvSIFIRLA(gorevTutucu);
 * \endcode
 *
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \hideinitializer
 */
#define grvSIFIRLA(g)   \
    grvILKLE((g));      \
    return grvBEKLIYOR

/**
 * Bir görevden çıkar.
 *
 * Bir kez çıkış yapıldı mı artık bu görev çalıştırılmak için çağrılmaz.
 * Yeniden çağrılması için görevin gorevOlustur() API'si ile yeniden çalışacak
 * görevler listesine eklenmesi gerekir.
 * Her görevin parametresi olan @gorevTutucu_t parametresi ve koşul ifadesi
 * verilerek çağrılır:
 * \code
 * grvCIK(gorevTutucu);
 * \endcode
 *
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \hideinitializer
 */
#define grvCIK(g)   \
    grvILKLE((g));  \
    return grvCIKTI


/**
 * Bir görevin sırasını başka bir göreve vermek için kullanılır.
 * 
 * Bazen bir görev bir işlemi tamamlayıp başka önemli zaman kısıtı olan bir
 * işlem yapmayacaksa, bu makroyu çalıştırarak CPU kontrolünü çalışmak için
 * bekleyen diğer görevlere gönüllü olarak verir. Bu makro diğer sistemlerdeki
 * yield işlevine benzer çalışır. Her görevin parametresi olan gorevTutucu_t
 * parametresi verilerek çağrılır:
 * \code
 * grvVAZGEC(gorevTutucu);
 * \endcode
 *
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \hideinitializer
 */
#define grvVAZGEC(g)                \
    VAZGECIS_BAYRAGI = 0;           \
    SN_KUR(( (gorev_t*) g )->sn);   \
    if(VAZGECIS_BAYRAGI == 0){      \
        return grvVAZGECTI;         \
    }

/**
 * Bir görevin sırasını bir koşul sağlanana dek diğer görevlere vermek için
 * kullanılır.
 * 
 * Bir görev yaptığı işlemde belli bir aşamaya gelip bir koşul beklemesi
 * gerekiyorsa grvKOSUL_BEKLE'ye alternatif olarak bu makroyu kullanabilir.
 * Bu durumda görev; belirtilen koşul sağlanana dek CPU kontrolünü diğer
 * görevlere gönüllü olarak verecektir. Her görevin parametresi olan
 * gorevTutucu_t parametresi ve koşul ifadesi verilerek çağrılır:
 * \code
 * bool mesajAlindi; // Global bir değişken
 * //...
 * grvKOSULA_DEK_VAZGEC(gorevTutucu, mesajAlindi == true);
 * \endcode
 *
 * \param g gorevTutucu_t türünde bir tutucu (handle) alır. Bu, görev kontrol
 * bloğuna bir başvurudur.
 * \hideinitializer
 */
#define grvKOSULA_DEK_VAZGEC(g, kosul)          \
    VAZGECIS_BAYRAGI = 0;                       \
    SN_KUR(( (gorev_t*) g )->sn)                \
    if((VAZGECIS_BAYRAGI) == 0 || !(kosul)){    \
        return grvVAZGECTI;                     \
    }


/**
 * Sistem tik işleyicisi.
 *
 * Görevlerin süre takibi yapabilmesi için bu işlevin bir sistem tik timer
 * kesmesi tarafından çağrılması gereklidir.
 */
void grvTikKesmeIsleyici(void);

#if grvCALISMA_KIPI == 1
/**
 * Görev işlevini alıp bir Görev Kontrol Bloğu (GKB) oluşturur.
 *
 * Görev olarak tanımlanmış işlev referansını alıp yeni bir görev olarak
 * görevler listesine ekler. Görevler uygulamanın main bölümünde bu
 * işlev kullanılarak oluşturulmalıdır.
 * 
 * \param[in] is_t işi yapacak görev kodu bloğu (bir function pointer).
 * \return gorev_t türünde görev kontrol bloğuna referans, yoksa NULL.
 */
pgkb_t grvOlustur(is_t is);

/**
 * Görevlerin çalışacağı sonsuz döngüyü başlatır.
 *
 * Listedeki herbir görev sırasıyla çalıştırılır. Görevler işbirlikçi
 * (cooperative) olarak çalışırlar, çalışan göreve, kendisi kontrolü teslim
 * edinceye dek müdahale edilmez.
 *
 * \warning Bu işlev main işlevinden doğrudan çağrılmamalıdır. Bunun yerine
 * hangi platform için geliştirme yapılıyorsa, o platformun
 * portGorevciyiBaslat() işlevi ile görevci başlatılmalıdır.
 * portGorevciyiBaslat() işlevi platforma özel gereken ilklemeleri yapacak ve
 * ardından grvGorevciyiBaslat işlevini çağıracaktır. Doğru uygulama bu
 * şekildedir. Aksi takdirde beklenmedik sonuçlar elde edilebilir.
 *
 * \sa portGorevciyiBaslat().
 */
void grvGorevciyiBaslat(void);

/**
 * Kimliği verilen görev kontrol bloğuna referans döndürür.
 *
 * \param[in] kimlik GKB kimliği.
 * \return gorev_t türünde görev kontrol bloğuna referans, yoksa NULL.
 */
pgkb_t grvKimlikIleGorevBlogunuAl(const unsigned char kimlik);

/**
 * Kimliği verilen görevi uyandırır / çalıştırır.
 *
 * Verilen kimliğe sahip GKB bulunamazsa işlem sürdürülmez.
 *
 * \param[in] kimlik GKB kimliği.
 */
void grvBaslat(const unsigned char kimlik);

/**
 * Kimliği verilen görevi uyutur / durdurur.
 *
 * Verilen kimliğe sahip GKB bulunamazsa işlem sürdürülmez.
 *
 * \param[in] kimlik GKB kimliği.
 */
void grvDurdur(const unsigned char kimlik);

#endif
#endif
/// @}
