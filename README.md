# Görevci

## Yazar
İsmail Sahillioğlu (Kozmotronik)

## Lisans
MIT

## Amaç
Başta küçük ölçekli gömülü sistemler olmak üzere farklı gömülü sistemler üzerinde çalışabilecek ANSI C ile yazılmış, görev organizasyonu yapan  ve birden çok görevin yazılmasını ve yönetilmesini kolaylaştıran bir uygulama geliştirmek.

## Tanıtım
Bu proje **görev organizasyonu** yapan bir üst yönetim uygulamasıdır. Görevci, bir işletim sistemi değil, bir çoklu görev (multi-tasking) uygulamasıdır. Uygulama tüm derleyicilere uyumluluk açısından ANSI C ile geliştirilir.
Bu proje [Protothreads](http://dunkels.com/adam/pt/index.html) projesinden esinlenerek yeniden yazılmıştır. API'ler Türkçeleştirilmiştir ve ayrıca Protohreads projesinde bulunmayan `grvOlustur()` ve `grvGECIK_MS()` gibi API'ler de geliştirilip mümkün olduğunca soyutlanıp bir katman haline getirilmeye çalışılmaktadır.  
Görevler için ayrı ayrı görev yığınları (thread stack) kullanılmadığı için küçük gömülü sistemlerde bile çalışacak kadar hafiftir[^1]. Ortalama bir RTOS sisteminde herbir thread/task için 64 - 256 byte arasında thread stack kullanılır. Herbir thread için bu kadar bellek kullanmanın yalnızca 256 byte RAM' e sahip bir aygıt için ne kadar maliyetli olacağı açıktır ki; bu RTOS sistemler bu yüzden böyle küçük aygıtlarda çalışamazlar.

Görevci iki farklı kipte kullanılabilir:
* Hafif kip
* Normal kip

Bellek bakımından çok kısıtlı; örneğin 2 KByte veya altı ROM ve 256 Byte veya altı RAMe sahip aygıtlar için *Hafif kip* idealdir.  
Nitekim bu değerlerin üstünde bellek donanımına sahip aygıtlarda *Normal kip* kullanılabilir. Elbette uygulamanın büyüklüğüne göre doğru ölçekte bir aygıtı seçmek tasarımcının sorumluluğundadır.  
Kip, projeye eklenecek *gorevciypl.h* adında bir başlık dosyası içerisinde `grvCALISMA_KIPI` tanımlanarak seçilir. `grvCALISMA_KIPI` 0 olarak tanımlanırsa Görevci *Hafif kip*te, 1 olarak tanımlanırsa *Normal kip*te yapılandırılacaktır. Kip olarak *Hafif kip* kullanıldığı durumda programcı, `main` döngüsü içinde sistem kesmesi ve sistem tik timer' ını ilklemeyi ve görev yönetimini kendisi yapar. Her iki kipin kullanımı port demolarında örneklendirilmiştir.

Bununla birlikte Görevcinin çalışması için de gerekli minimum bellek 
gereksinimleri vardır. Ancak bu gereksinimler üst düzey RTOS' larla 
kıyaslandığında birçok küçük ölçekli aygıt için oldukça kabul edilebilirdir. 
Görevci her bir *Görev Kontrol Bloğu* için *Hafif kip*te 1 word, *Normal kip*te 2 word RAM kullanımı gerektirir. Gereksinimlerle ilgili ayrıntılı bilgi için [Minimum Sistem Gereksinimleri](#minimum-sistem-gereksinimleri) bölümüne bakınız.  

Görevciyi kullanmanın en kolay yolu, Görevciyi Githubdan clon etmek, sonra (varsa) uygulamak istediğiniz portun örnek projesini açıp gerekli değişiklikleri yaparak kullanmaktır. Bir görevin normal bir C işlevinden pek bir farkı yoktur, yalnızca biraz daha yapılandırılmıştır ve sürekli çalışması gereken görevler içlerinde bir sonsuz döngü içerir. Bir görevin temel yapısı şu şekilde olmalıdır:

```c
char gorev(gorevTutucu_t tutucu) {
    /* Görev kapsamında (scope) kullanılacak değişkenler burada
     * tanımlanabilir. Değerini koruması gereken değişkenler "static"
     * niteleyicisiyle tanımlanmalıdır.
     */
    static char karakter;

    /* Bir görev ana görev döngüsünden hemen önce her zaman görev yapısına
     * başvuru olarak parametre alan grvBASLA() ile başlamalıdır.
     */
    grvBASLA(tutucu);

    /* Buraya bir kereye mahsus çalışacak kodlar. Örneğin bir giriş - çıkış
     * portunu ilkleme veya bir analog ucunu ilkleme kodları gibi.
     * Buradaki kodlar görevin yaşam süresi boyunca yalnızca bir kez
     * çalışacağı için ilklendirme işlemlerini yapmak için idealdir.
     */

    /* Bu döngü bir görev bloğunun ana döngüsüdür. for(;;) biçiminde de
     * yazılabilir. Burada sonsuz döngüde kalmalı, kesinlikle break veya
     * return ile döngüden çıkılmamalıdır.
     */
    while(1) {
        // Buraya görev kodları ve bloklayıcı API çağrıları
    }

    /* Tüm görev işlevleri görev yapısına başvuru olarak parametre alan
     * grvBITIR() ile sonlanmalıdır. Akış sonsuz döngüden buraya buraya
     * gelmesi görevin bir daha çalışmamasına neden olabilir.
     */
    grvBITIR(tutucu);
}
```


## Kullanım Yönergeleri
Bir görevin bir olay beklemesi ya da gecikmesi gerekiyorsa CPU'yu boşuna meşgul etmemek, gerektiğinde beklemesi gereken görevi bloklayıp çalışmaya hazır başka bir görevin çalışmasını sağlamak Görevcinin temel amaçlarındandır. Ancak bu mekanizmanın düzgün bir şekilde işlemesi için görev bloğu içerisinde, klasik super-loop veya state-machine programlama mantığında kullanılan `return` ve `break` gibi dönüş ve döngü kırma komutları kesinlikle kullanılmamalıdır. Böyle yapmak görevin düzgün ve beklendiği gibi çalışmamasına neden olur. Bunun yerine aşağıdaki örnek durumlara uygun düşen API'ler kullanılmalıdır.  

Bir görevde gecikme yapılmak isteniyorsa:  
* `grvGECIK_MS()` - *işletim sistemlerinde `sleep()` işlevlerine benzer*
* `grvKOSULLU_GECIK_MS()` - *işletim sistemlerinde `sleep()` işlevlerine benzer*

Bir koşul veya olayın beklenmesi isteniyorsa:
* `grvKOSUL_BEKLE()`
* `grvBU_KOSULDA_BEKLE()`

Verilere erişimde senkronizasyon gerekiyorsa:
* `grvBAYRAK_BEKLE()` - *işletim sistemlerindeki `wait()` işlevlerine benzer*
* `grvBAYRAK_IMLE()` - *işletim sistemlerindeki `signal()` işlevlerine benzer*

Bazen bir görev kendi isteğiyle kontrolü çalışmak için bekleyen başka bir göreve vermek isteyebilir. Böyle bir durumda:
* `grvVAZGEC()` - *işletim sistemlerindeki `yield()` işlevlerine benzer*
* `grvKOSULA_DEK_VAZGEC()`  

Bu API' lerin örnek kullanımları için örnek programların kaynak dosyalarına 
bakılabilir.  
Bir diğer önemli konu, görev blokları içinde kullanılan değişkenlerdir. Görevci, yapısı gereği dinamik bellek yönetimi ve içerik değiştirme (context-switching) kullanmamaktadır. Görevcinin küçük ölçekli birçok aygıtta çalışabilmesini sağlayan en önemli özelliği az önce bahsedilen, bellek ve çalışma zamanı açısından küçük aygıtlar için çok maliyetli olan bu mekanizmaları kullanmamasıdır. Ancak görev blokları özünde birer işlev (function) olduğundan, görevler bloklandığında değişkenler normalde derleyici optimizasyonundan kaçamaz ve tuttukları verileri yitirirler. Bunu önlemek için değerini koruması gereken değişkenler tanımlanırken `static` niteleyicisi ile tanımlanmalıdır. Bu niteleyici derleyiciye ilgili değişkeni işlevden dönerken yok etmemesini, işlev bir dahaki sefer çağrıldığında bu değişkenin içeriğinin kullanılacağını bildirir.  

## Genel Özellikler
- Çok küçük miktarda RAM kullanır. Her bir *Görev Kontrol Bloğu* için *Normal kip*te **6 byte**, *Hafif kip*te ise yalnızca **2 byte** gerekir ve extra yığınlar (thread stack) kullanılmaz[^2].
- Oldukça taşınabilir. %100 saf C ile yazıldığından standart C destekleyen bütün derleyicilerde derlenebilir, mimariye özel assembly içermez. Bunun yerine farklı mimari portları için her mimarinin kendi port dizininde o mimariye özel tanımlamalar ve kodlar yazılmıştır.
- İçerik-değiştirme (context-switching)[^3] ve planlayıcı (scheduler)[^4] mekanizmaları olmadan çalışmasına rağmen bloklayıcı bekleme mekanizmaları sağlar.
- Okunaklı, derli ve toplu bir program arayüzü sağlar.
  
## Minimum Sistem Gereksinimleri
Hiçbir görev oluşturulmadan yalnızca Görevci çekirdeğinin gerektirdiği sistem kaynakları gereksinimleri her iki çalışma kipi için de aşağıdaki tabloda **yaklaşık olarak** verilmiştir.

| Kaynak    | Tür               | Normal kip    | Hafif kip     |
| :---      | :----:            | :---:         | :---:         |
| ROM       | Program belleği   | 260 word      | 20 word       |
| RAM       | Sistem belleği    | 11 byte       | 4 byte        |
| Stack     | Donanım stack     | 6 düzey       | 5 düzey       |
| Tik Timer | Timer donanım     | 1 Adet        | 1 Adet        |

Görüldüğü gibi *Normal kip*te Görevcinin görev yönetim mekanizması da derlendiği için **260 word**[^5] kadar program belleğine, 11 byte kadar da RAMe gerek duyar. Ancak hafif kipte görev yönetimini doğrudan programcı üstlendiğinden yalnızca **20 word** ROM ve **4 byte** RAM gibi oldukça az kaynak ister.  
Örnek bir karşılaştırma olması açısından, açık kaynaklı popüler bir RTOS olan
[FreeRTOS](https://www.freertos.org/index.html) yalnızca çekirdeği için 9 KByte ROM'a gerek duyar. FreeRTOS işlevsellik ve zaman kısıtı açısından çok daha gelişmiştir ki, *Hard real-time* ve *Soft real-time* uygulamalarının her ikisi için kullanılabilir. FreeRTOS priority, cooperative, preemptive, time slicing (round-robin) veya bunların karışımı biçiminde farklı planlayıcı yapılandırmalarına sahiptir. Ancak elbette bu gelişmişliğin maliyeti daha fazla ROM, RAM ve CPU zamanı gibi kaynak kullanımıdır.  
Görevci ise küçük bir *Soft real-time* niteliğinde olup küçük gömülü sistemlerde görev organizasyonu için ihtiyaç duyulabilecek birçok işlevselliğe sahiptir. Bu işlevselliklere geliştirme sürecinde yenileri eklenebilir.  

#### İsteğe Bağlı Görevci ile Tümleşik Kullanılabilecek Donanımlar
| Kaynak    | Tür               | Miktar    |
| :---      | :----:            | :---:     |
| UART      | Seri Haberleşme   | 1 Adet    |

Uygulama katmanı yazılırken kullanılan her görev ve değişkenler bu değerler üzerine eklenir. Görevci modülü statik bellek kullanımı yaptığından görevlerin bellekleri derleme zamanında oluşturulmaktadır. Dolayısıyla `gorevciypl.h` dosyasında tanımlanacak `grvMAX_GOREV_SAYISI` tanımlaması minimum sistem gereksinimlerine ek olarak ne kadar bellek kullanılacağını belirler.

Aşağıda, önceden belirtilen sistem bellek gereksinimlerine ek olarak oluşturulacak her görev (yalnızca görevin kendisi) için her iki kipte **yaklaşık** ne kadar RAM kullanılacağının hesabı şöyle yapılır:

| Normal kip                | Hafif kip                 |
| :-:                       | :-:                       |
| Görev sayısı * 6 byte     | Görev sayısı * 2 byte     |


[^1]: Görevcinin küçük ölçekli aygıtlarda çalışabilmesinin bir sınırı vardır. [Minimum Sistem Gereksinimleri](#minimum-sistem-gereksinimleri)ni karşılamayan aşırı küçük ve sınırlı aygıtlarda elbette kullanılamaz.

[^2]: Bu miktara kullanıcı kodunda eklenecek değişkenler <u>dahil değildir</u>.

[^3]: Bilindiği gibi gerçek işletim sistemlerinde her bir thread için büyük
miktarda (ör. 64 byte, 256 byte gibi) ayrı bir RAM alanı ayrılır ve her bir 
thread yalnızca kendisinin belleğe sahip olduğunu sanar. Bu alana o thread'in 
yığını (stack) denir. Bir thread scheduler tarafından bloklandığında bu 
thread'e ait CPU durum bayrakları, program counter (PC), stack pointer (SP) 
gibi donanım sistem değişkenleri bu alana saklanır. Aynı thread yeniden 
çalıştırılmak için seçildiğinde sistem değişkenleri bu alandan alınıp kendi 
yerleri olan donanım yazmaçlarına geri yüklenir. Bu içerik değiştirme işlemine 
context-switching denir ve küçük ölçekli denetleyiciler için zaman, ve bellek 
açısından çok maliyetli bir işlemdir. Hafif Görevleyici gibi makro tabanlı 
görev yöneticileri küçük ölçekli denetleyici aygıtları hem bu maliyetten 
kurtarır hem de basit ve hızlı bir biçimde görev yönetimi yapar.

[^4]: Yine bilindiği gibi gerçek işletim sistemlerinde thread' leri çalışma kuyrukları ve CPU arasında yönetmek için çalışan ve kendisi de bir tür thread olarak nitelendirilebilecek pre-emptive (araya girici) olabilen *scheduler* adında bir program vardır. Bu program thread' lerin bloklanması, bazı girdi olaylarının gerçekleşmesi, zamana bağlı periyodik olayların çalışması gibi durumlarda CPU kontrolünü o an çalışan bir thread' den alarak yönetim işlemlerini yapmak için bir süre çalışır. Bu da CPU zamanından yer. Görevci gibi makro tabanlı görev yöneticileri bu CPU harcama zamanını en aza indirir ancak bunun görevleri yalnızca belirtilen sırada çalıştırma gibi bir dezavantajı vardır. Ama bu dezavantaj, *scheduler* programının harcadığı CPU zamanı göz önüne alındığında göz ardı edilebilecek ölçektedir.

[^5]: Bellek birimleri `word` sözcüğü ile mimarilere göre göreceli 
belirtilmiştir. Mimarinin temel **bit** genişliğini ifade eder. Aynı mimari 
içinde program belleğinin bit genişliği ile RAM belleğin bit genişliği farklı 
olabilir. Bu yüzden bellek birimi olarak göreceli olan `word` kullanılmıştır.
