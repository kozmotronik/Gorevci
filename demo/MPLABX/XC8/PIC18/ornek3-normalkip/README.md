## Port: MPLABX > XC8 > PIC18 Örnek 3

![Örnek 3](ornek3.png)

Bu örnekte toplamda 6 ayrı görev çalışmaktadır. Bu görevler sırasıyla:  
* Led çakarlama 1
* Led çakarlama 2
* Karaşimsek
* Karaşimşek hızı okuma
* Gösterge değeri güncelleme
* Gösterge güncelleme

Şimdi sırasıyla bu görevlerin ne yaptığına kısaca değinelim.

### Görev LED Çakarlama 1
Bu görevde tek bir LEDde türlü sinyal desenleri gösterilmektedir. Çıkışa bağlı bir LED dinamik olarak değişen süre aralıklarıyla yanıp söner. Bu aralıklar milisaniye türünden sırasıyla şöyledir: *50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000*. `char` türünde bir seçici değişken sırayla bu listeden süre değerlerini alır ve bir sonraki gecikmeye uygular. Görev 50ms gecikme ile başlayıp 1000ms gecikmeye doğru çalışır, ardından 1000ms gecikmeden 50ms gecikmeye doğru çalışır ve bu sonsuza dek bu şekilde sürer.

### Görev LED Çakarlama 2
Bu görev de ilk görevdekine benzer bir uygulama yapılmıştır ancak çakarlama deseni farklıdır. LED 50ms aralıklarla 3 kez yanar ve söner, son sönüşte 1sn bekler ve bunu yineler.

### Görev Karaşimşek
8 adet LED üzerinde çok basit karaşimşek animasyonu gösterir. Ancak animasyon hızı analog girişten okunan değere bağlıdır. Çalışma hızı aralığı 10ms - 200ms arasındadır.

### Görev Karaşimşek Hızı Okuma (Analog)
Karaşimşek görevine çalışma hızını sağlayan görevdir. Analog girişine bağlı bir POT ile okunan 10 bit analog-dijital gerilim dönüşüm değeri bir hesaplama işlemiyle 10ms - 200ms süre aralığına ölçeklenir ve karaşimşek görevine çalışma hızı olarak sunulur.  
Bu değer değişimi içerik değiştirme tuşuna basılarak karaşimşek animasyonunun çalıştığı 8 LED üzerinde gösterilebilir.

### Görev Gösterge Değeri Güncelleme
Göstergede gösterilecek süre değerini hesaplar. Karaşimşek animasyon hızı okunana dek bu görev bloklanır. Analog girişten yeni hız değeri okunduğunda bu görev çalışıp 0 - 1024 değerini 10 - 200 ms süre değerine ölçekler ve ardından bu değeri BCD formatında gösterge hane hücrelerine saklar.

### Gösterge Güncelleme
7 parçalı (7 segment) LED çoğullayıcı (multiplexer) olarak çalışır. *Gösterge değeri güncelleme* görevinin hesaplayıp BCD formatında hane hücrelerine sakladığı süre değerini ekranda gösterir.
<br/><br/>

***
Örneğin çalışmasını gösteren video:
[![Örnek uygulama](https://i9.ytimg.com/vi/1wrOdZQG20g/mq2.jpg?sqp=CJTciYYG&rs=AOn4CLDOtCvm3LvXbODhyUdcp10dgLwO9A)](https://youtu.be/1wrOdZQG20g)  

Örnek uygulamanın her iki çalışma kipi için derleme sonrası bellek tablosu özetleri aşağıdadır:

Normal kip:
```text
Memory Summary:
    Program space        used   C5Ch (  3164) of 10000h bytes   (  4.8%)
    Data space           used    83h (   131) of   F38h bytes   (  3.4%)
    Configuration bits   used     7h (     7) of     7h words   (100.0%)
    EEPROM space         used     0h (     0) of   400h bytes   (  0.0%)
    ID Location space    used     8h (     8) of     8h bytes   (100.0%)
```
Hafif kip:
```text
Memory Summary:
    Program space        used   ABEh (  2750) of 10000h bytes   (  4.2%)
    Data space           used    59h (    89) of   F38h bytes   (  2.3%)
    Configuration bits   used     7h (     7) of     7h words   (100.0%)
    EEPROM space         used     0h (     0) of   400h bytes   (  0.0%)
    ID Location space    used     8h (     8) of     8h bytes   (100.0%)
```
