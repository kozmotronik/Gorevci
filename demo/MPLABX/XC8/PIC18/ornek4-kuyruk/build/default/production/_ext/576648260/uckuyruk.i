# 1 "../../../../../kaynak/uckuyruk.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/opt/microchip/mplabx/v5.50/packs/Microchip/PIC18F-K_DFP/1.4.87/xc8/pic/include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "../../../../../kaynak/uckuyruk.c" 2
# 29 "../../../../../kaynak/uckuyruk.c"
# 1 "../../../../../kaynak/include/uckuyruk.h" 1
# 127 "../../../../../kaynak/include/uckuyruk.h"
typedef struct {
 unsigned char kapasite;
 unsigned char sayim;
 unsigned char bas;
 unsigned char *veri;
} uckuyruk_t;
# 143 "../../../../../kaynak/include/uckuyruk.h"
void uckuyrukIlkle(const unsigned char, uckuyruk_t*, unsigned char*);
# 156 "../../../../../kaynak/include/uckuyruk.h"
char uckuyrukKuyrukla(uckuyruk_t*, const unsigned char);
# 172 "../../../../../kaynak/include/uckuyruk.h"
char uckuyrukCokluKuyrukla(uckuyruk_t*, const unsigned char*, const unsigned char);
# 186 "../../../../../kaynak/include/uckuyruk.h"
unsigned char uckuyrukBastakiOge(uckuyruk_t*);
# 200 "../../../../../kaynak/include/uckuyruk.h"
unsigned char uckuyrukKuyruktanAl(uckuyruk_t*);
# 219 "../../../../../kaynak/include/uckuyruk.h"
char uckuyrukCokluAl(uckuyruk_t*, unsigned char*, const unsigned char);
# 230 "../../../../../kaynak/include/uckuyruk.h"
unsigned char uckuyrukKacOgeVar(uckuyruk_t*);
# 241 "../../../../../kaynak/include/uckuyruk.h"
char uckuyrukDolu(uckuyruk_t*);
# 252 "../../../../../kaynak/include/uckuyruk.h"
unsigned char uckuyrukKalanKapasite(uckuyruk_t*);
# 263 "../../../../../kaynak/include/uckuyruk.h"
char uckuyrukBos(uckuyruk_t*);
# 30 "../../../../../kaynak/uckuyruk.c" 2

void uckuyrukIlkle(const unsigned char kap,
        uckuyruk_t *kuyruk, unsigned char *veri) {
 if(kuyruk == ( (void*) 0 ) || veri == ( (void*) 0 )) return;
 kuyruk->kapasite = kap;
 kuyruk->bas = 0;
 kuyruk->sayim = 0;
 kuyruk->veri = veri;
}

char uckuyrukKuyrukla(uckuyruk_t *kuyruk, const unsigned char veri) {

 if(kuyruk == ( (void*) 0 ) || uckuyrukDolu(kuyruk)) return 0;
 unsigned char sayim = kuyruk->sayim;
 const unsigned char kap = kuyruk->kapasite;

 unsigned char konum = kuyruk->bas + sayim;
 if(konum >= kap) konum -= kap;

 *( (kuyruk->veri) + konum) = veri;

 kuyruk->sayim = ++sayim;
 return 1;
}

char uckuyrukCokluKuyrukla(uckuyruk_t *kuyruk,
        const unsigned char *v, const unsigned char kac) {
 unsigned char sayac;
 if(kuyruk == ( (void*) 0 ) || v == ( (void*) 0 )) return 0;

 if(uckuyrukKalanKapasite(kuyruk) < kac) return 0;

 unsigned char kap = kuyruk->kapasite;
 unsigned char *veri = kuyruk->veri;
 unsigned char sayim = kuyruk->sayim;

 unsigned char konum = (kuyruk->bas + sayim);
 if(konum >= kap) konum -= kap;
 const char baslangic = konum;


 for(sayac = 0; sayac < kac; sayac++) {
  *(veri + konum) = *(v + sayac);

  konum++;
  if(konum >= kap) konum -= kap;
  sayim++;
 }
 kuyruk->sayim = sayim;

 return sayac;
}

unsigned char uckuyrukBastakiOge(uckuyruk_t *kuyruk) {
 return kuyruk == ( (void*) 0 ) ? 0 : *( (kuyruk->veri) + (kuyruk->bas) );
}

unsigned char uckuyrukKuyruktanAl(uckuyruk_t *kuyruk) {

 if(kuyruk == ( (void*) 0 ) || uckuyrukBos(kuyruk)) return 0;
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
 if(kuyruk == ( (void*) 0 ) || v == ( (void*) 0 )) return 0;

 if(uckuyrukKacOgeVar(kuyruk) < kac) return 0;

 const unsigned char kap = kuyruk->kapasite;
 const unsigned char *veri = kuyruk->veri;
 unsigned char bas = kuyruk->bas;
 unsigned char sayim = kuyruk->sayim;


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
 return kuyruk == ( (void*) 0 ) ? 0 : kuyruk->sayim;
}

char uckuyrukDolu(uckuyruk_t *kuyruk) {
 return kuyruk == ( (void*) 0 ) ? 1 : (kuyruk->sayim >= kuyruk->kapasite ? 1 : 0);
}

char uckuyrukBos(uckuyruk_t *kuyruk) {
 return kuyruk == ( (void*) 0 ) ? 1 : kuyruk->sayim == 0;
}

unsigned char uckuyrukKalanKapasite(uckuyruk_t *kuyruk) {
 return kuyruk == ( (void*) 0 ) ? 0 : kuyruk->kapasite - kuyruk->sayim;
}
