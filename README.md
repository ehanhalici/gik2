# GIK2 (Görüntü İşleme Kütüphanesi 2)

Bu proje, C dili ile yazılmış düşük seviyeli ve yüksek performanslı bir görüntü işleme kütüphanesidir. Temel amacı, PNG dosyaları üzerinde hızlı piksel manipülasyonu, filtreleme ve görüntü analizi işlemlerini gerçekleştirmektir.

## Özellikler

* **PNG İşleme:** `libpng` tabanlı, ham piksel verilerine doğrudan erişim sağlayan özelleştirilmiş bir işleme motoru.
* **Hızlı Algoritmalar:** C dilinin performansından yararlanarak görüntüler üzerinde eş zamanlı filtreleme ve dönüşüm işlemleri.
* **Minimalist Yapı:** Harici bağımlılıkları en aza indiren, taşınabilir ve modüler kod tabanı.
* **Geliştirme Ortamı:** `shell.nix` yapılandırması ile sistem bağımlılıklarını tek bir komutla yükleyebilen, tamamen izole edilmiş bir geliştirme ortamı.

## Yapı

* `main.c`: Kütüphane fonksiyonlarının test edildiği ve işleme hattının yönetildiği ana giriş noktası.
* `pngProcess.h`: PNG dosyalarını okuma, yazma ve piksel verilerini belleğe alma işlemlerini yöneten başlık dosyası.
* `gik.h`: Kütüphanenin temel fonksiyon tanımlarını ve veri yapılarını içeren ana başlık dosyası.
* `Makefile`: Projenin derlenmesi için optimize edilmiş, `libpng` ve diğer sistem kütüphanelerini bağlayan yapılandırma dosyası.

## Kurulum ve Derleme

Proje dosyalarını yerel dizininize kopyaladıktan sonra derleme işlemi için aşağıdaki adımları takip edebilirsiniz:

1. **Bağımlılıklar:** Eğer sisteminizde `libpng` ve `gcc` yüklü değilse, proje içindeki `shell.nix` dosyasını kullanarak bağımlılıkları otomatik yükleyebilirsiniz:

```bash
   nix-shell

```

2. **Derleme:**
Projeyi derlemek için terminalde ana dizindeyken şu komutu çalıştırın:

```bash
make

```


3. **Çalıştırma:**
Derleme işlemi başarıyla tamamlandıktan sonra oluşturulan `gik2` yürütülebilir dosyasını çalıştırarak örnek görüntüleri işleyebilirsiniz:

```bash
./gik2

```

