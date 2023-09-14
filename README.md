# Görevci
Küçük ölçekli mikrodenetleyiciler için geliştirilen gömülü projelerin 
kurtarıcısı!  
Görevci daha çok küçük ölçekli gömülü projelere hitap eden bir kütüphanedir.
Görevci, zaman ve donanım tabanlı olaylara bağlı çalışan işlemleri CPU'yu
bloklamadan yönetmeyi amaçlar. Böylece görevler olaylara etkili bir biçimde
yanıt verir ve hiçbir olay kaçırılmadan işlenebilir.

## Görevciyi Edinme ve Projenize Ekleme
Görevciyi projelerinizde kullanmadan önce, kullandığınız aygıtın portunun 
desteklendiğinden emin olun. Eğer kullandığınız aygıt için bir port desteği 
yoksa [Issues](https://github.com/kozmotronik/Gorevci/issues) bölümünde bir 
konu açarak istek yapabilirsiniz.  

Görevciyi birkaç farklı yolla projelerinize entegre edebilirsiniz.  
Bunlardan ilki repoyu klonlayarak kaynak dosyalarını indirip proje ağacınıza 
eklemektir. Bunu yapmak için, dosyaları projenizin olduğu dizine ekledikten 
sonra geliştirme ortamınızda da (IDE) bu dosyaları eklemeli ardından proje 
özellikleri bölümnden include yollarını IDE'ye tanıtmalısınız. İnclude yolları 
tanıtılmazsa derleme başarısız olur. Bu aşamadan sonra uygulamanızı 
derleyebilirsiniz.

İkinci yöntem ise Görevciyi projenize bir git alt modülü (submodule) olarak 
eklemektir.  

Yukarıdaki işlemlerin adım adım anlatımları için
[Görevci Wiki](https://github.com/kozmotronik/Gorevci/wiki/G%C3%B6revci-Wiki) 
sayfasına bakınız.
  
## Lisans
Lütfen lisans metnini dikkatlice okuyun ama lisansa çok takılmayın. Bu lisansın 
özeti şöyle:  
*Burada sağlanan kaynak kodlarını arzuladığınız gibi kullanın. Ancak sağlanan 
üründe için hiçbir teminat verilmemekle birlikte, kullanımdan doğacak hiçbir 
sorumluluk da kabul edilmemektedir.*
* [MIT](LICENSE) - Orijinal metin.
* [MIT-TR](LISANS_tr.md) - Lisans metninin Türkçe sürümü.

## Özellikler
Düşük kaynak kullanır ve farklı platformlara kolayca adapte edilebilir. Diğer 
özellikler için 
[Genel Özellikler](https://github.com/kozmotronik/Gorevci/wiki/Genel-%C3%96zellikler) 
sayfasına bakınız.  
Yaklaşık sistem gereksinimleri için [Minimum Sistem Gereksinimleri](https://github.com/kozmotronik/Gorevci/wiki/Minimum-Sistem-Gereksinimleri) sayfasına bakınız.
 
## Sorun Bildirme
Lütfen sorunları ve eklenmesini istediğiniz özellikleri 
[Issues](https://github.com/kozmotronik/Gorevci/issues) bölümünden bildirerek 
bu projenin gelişmesine katkı sağlayın.  
Bir özellik istemeden önce bu projenin küçük ölçekli aygıtlar için 
geliştirildiğini göz önünde bulundurunuz.
